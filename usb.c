#include "usb.h"
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include "usbdesc.h"
#include "usb_cdc.h"

// Description: tiny USB stack for Microchip PIC microcontrollers
// Features: HID class, CDC class supported
// Author: Roman Grinev
// Date 30.05.2017

// Useful link http://www.beyondlogic.org/usbnutshell/usb4.shtml

#define FULL_SPEED

/* BDT entry structure definition */
typedef union BD_STAT
{
    uint8_t Val;
    struct{
        //If the CPU owns the buffer then these are the values
        unsigned BC8:1;         //bit 8 of the byte count
        unsigned BC9:1;         //bit 9 of the byte count
        unsigned BSTALL:1;      //Buffer Stall Enable
        unsigned DTSEN:1;       //Data Toggle Synch Enable
        unsigned INCDIS:1;      //Address Increment Disable
        unsigned KEN:1;         //BD Keep Enable
        unsigned DTS:1;         //Data Toggle Synch Value
        unsigned UOWN:1;        //USB Ownership
    };
    struct{
        //if the USB module owns the buffer then these are
        // the values
        unsigned BC8:1;         //bit 8 of the byte count
        unsigned BC9:1;         //bit 9 of the byte count
        unsigned PID0:1;        //Packet Identifier
        unsigned PID1:1;
        unsigned PID2:1;
        unsigned PID3:1;
        unsigned :1;
        unsigned UOWN:1;        //USB Ownership
    };
    struct{
        unsigned :2;
        unsigned PID:4;         //Packet Identifier
        unsigned :2;
    };
} BD_STAT_t;                      //Buffer Descriptor Status Register

    typedef struct BD_entry
    {
        BD_STAT_t STAT;
        uint8_t CNT;
        uint8_t* ADR;
    } BD_entry_t;
    
    typedef struct BD_endpoint
    {
        BD_entry_t out;
        BD_entry_t in;
    } BD_endpoint_t;
    
enum transaction_stage{
    SETUP,
    DATA_IN,
    DATA_OUT,
    STATUS_IN,
    WAIT_SETUP
};

static volatile BD_endpoint_t endpoints[EP_NUM_MAX] __at(BD_BASE_ADDR);

static volatile USB_SETUP_t usb_setup __at(BD_DATA_ADDR);
static volatile uint8_t ep_data_buffer[192] __at(BD_DATA_ADDR);
//!!!!ALL EP BUFFERS MUST BE SAME SIZE!!!!

static volatile uint8_t dev_addr;
static const uint16_t status=0x0000;
static volatile uint8_t control_need_zlp;
static volatile uint8_t data_need_zlp;
static volatile uint8_t ctl_stage;
static volatile uint8_t state;
//static int synch;

static uint8_t current_cnf=0;
static uint8_t alt_if=0;
static uint8_t idle_rate=1;
static uint8_t active_protocol=0;


usb_rx_callback_t data_callback;
usb_ctl_rx_callback_t ctl_callback;
USB_SETUP_t last_request;
uint8_t* received_data;
uint16_t received_data_len;

static volatile uint8_t request_handled=0;
// for multitransactions over the CONTROL pipe
static volatile uint16_t wCount;
static volatile uint8_t* ibuf;
static volatile uint8_t* obuf;

void soft_detach();



void usb_buf_in_rdy(uint8_t ep);
void usb_buf_out_rdy(uint8_t ep);
void reset_usb();
void init_usb();
void SetupStage();
void DataInStage();
void DataOutStage();
void WaitForSetupStage(void);
void process_standart_request();
void usb_poll();

#define LEDA_ON LATC6=1;
#define LEDA_OFF LATC6=0;
#define LEDB_ON LATC7=1;
#define LEDB_OFF LATC7=0;

#define LEDA_TOGGLE LATC6=!LATC6;
#define LEDB_TOGGLE LATC7=!LATC7;

void usb_buf_in_rdy(uint8_t ep)
{
    endpoints[ep].in.STAT.Val&=_DTSMASK;
    endpoints[ep].in.STAT.DTS=!endpoints[ep].in.STAT.DTS;
    endpoints[ep].in.STAT.Val|=_USIE | _DTSEN;  
}
void usb_buf_out_rdy(uint8_t ep)
{
    endpoints[ep].out.STAT.Val&=_DTSMASK;
    endpoints[ep].out.STAT.DTS=!endpoints[ep].out.STAT.DTS;
    endpoints[ep].out.STAT.Val|=_USIE | _DTSEN;  
}

void register_ctl_callback(usb_ctl_rx_callback_t _callback)
{
    received_data=0;
    received_data_len=0;
    ctl_callback=_callback;
}

void register_data_callback(usb_rx_callback_t _callback)
{
    data_callback=_callback;
}

void ep_stall(uint8_t ep)
{
    endpoints[ep].in.STAT.Val=_BSTALL | _USIE;
    endpoints[ep].out.STAT.Val=_BSTALL | _USIE;
}

void reset_usb()
{
    memset(0x200, 0x00, 0x100);
    UADDR=0;
    UIR=0;
    UIEbits.TRNIE=1;
    UIEbits.URSTIE=1;
    USBIE=1;
    UIEbits.SOFIE=0;
    UIEbits.STALLIE=1;
    UIEbits.ACTVIE=0;
    UIEbits.IDLEIE=1;
    
    UCFGbits.PPB0=0;
    UCFGbits.PPB1=0;
    UCFGbits.UPUEN=1;
 
#ifdef LOW_SPEED
    UCFGbits.FSEN=0;
#endif
#ifdef FULL_SPEED
    UCFGbits.FSEN=1;
#endif
   
    UCONbits.SUSPND=0;
    UCONbits.RESUME=0;
    UCONbits.PPBRST=0;
    UCONbits.USBEN=1;
    UIRbits.TRNIF = 0; 
    UIRbits.TRNIF = 0;
    UIRbits.TRNIF = 0;
    UIRbits.TRNIF = 0;
    // Enable EP0
    configure_ep_out(0, &usb_setup, EP0_BUFF_SIZE, 0x00);
    configure_ep_in(0, &ep_data_buffer[EP0_IN_OFFSET], EP0_BUFF_SIZE, 0x00);
    UEP0=EP_IN | EP_OUT | EP_HSHK;
    // Arming EP0 for receiving a first SETUP request
    WaitForSetupStage();
    state=ATTACHED;
}

void init_usb()
{
    LEDB_ON
    ctl_callback=0;
    data_callback=0;
    state=DETACHED;
    dev_addr=0;
    reset_usb();
}

void configure_ep_in(uint8_t ep, uint8_t* buf, uint8_t len, uint8_t mode)
{
    endpoints[ep].in.ADR=buf;
    endpoints[ep].in.CNT=len;
    endpoints[ep].in.STAT.Val=mode;
}

void configure_ep_out(uint8_t ep, uint8_t* buf, uint8_t len, uint8_t mode)
{
    endpoints[ep].out.ADR=buf;
    endpoints[ep].out.CNT=len;
    endpoints[ep].out.STAT.Val=mode;
}

void ctl_send(uint8_t* data, uint16_t len)
{
    request_handled=1;
    if (len>EP0_BUFF_SIZE && (len % EP0_BUFF_SIZE)==0) control_need_zlp=1; else control_need_zlp=0;
    wCount=len;
    ibuf=data;
}

void ctl_recv(uint8_t* data, uint16_t len)
{
    request_handled=1;
    if (len>EP0_BUFF_SIZE && (len % EP0_BUFF_SIZE)==0) control_need_zlp=1; else control_need_zlp=0;
    // save request context
    memcpy(&last_request, &usb_setup, 8);
    received_data=data;
    received_data_len=len;
    wCount=len;
    obuf=(char*)data;
}

void usb_tx_packet(uint8_t ep, uint8_t* buf, uint16_t len)
{  
    if (state!=CONFIGURED) return;
    if (buf!=endpoints[ep].in.ADR) memcpy(endpoints[ep].in.ADR, buf, len);
    if (len % EP1_BUFF_SIZE==0) data_need_zlp=1; else data_need_zlp=0;
    LEDA_TOGGLE
    endpoints[ep].in.CNT=len;
    usb_buf_in_rdy(ep);
}

int usb_rx_packet(uint8_t* buf, uint8_t len)
{
    int bytes_received_from_host=0;
    if (state!=CONFIGURED) return -1;
    return bytes_received_from_host;
}

void process_standart_request()
{
    uint16_t len=0;
    int request=(usb_setup.bRequest | (usb_setup.bmRequestType << 8));
    switch (request)
    {
        case STD_GET_STATUS_INTERFACE:
            ctl_send(&status, 2);
            break;
        case STD_CLEAR_FEATURE_INTERFACE:
            ctl_send(0, 0);
            break;
        case STD_SET_FEATURE_INTERFACE:
            ctl_send(0, 0);
            break;            
        case STD_GET_INTERFACE:
            ctl_send(&alt_if, 1);
            break;
        case STD_SET_INTERFACE:
            ctl_send(0, 0);
            alt_if=usb_setup.wValueH;
            break;
        case STD_GET_STATUS_ENDPOINT:
            ctl_send(&status, 2);
            break;
        case STD_CLEAR_FEATURE_ENDPOINT:
            ctl_send(0, 0);
            break;
        case STD_SET_FEATURE_ENDPOINT:
            ctl_send(0, 0);
            break;
        case STD_SYNCH_FRAME:
         //   ctl_recv(&synch, 2);
         // useful when isochronous transactions 
            break;
        case STD_GET_STATUS_ZERO:
            ctl_send(&status, 2);
            break;
        case STD_CLEAR_FEATURE_ZERO:
            ctl_send(0, 0);
            break;
        case STD_SET_FEATURE_ZERO:
            ctl_send(0, 0);
            break;
        case STD_SET_DESCRIPTOR:
            ctl_recv(0, usb_setup.wLen);
            break;
        case STD_GET_DESCRIPTOR_INTERFACE:
        case STD_GET_DESCRIPTOR_ENDPOINT:
        case STD_GET_DESCRIPTOR: // GET_DESCRIPTOR
            switch (usb_setup.wValueH)
            {
                case USB_DEVICE_DESCRIPTOR_TYPE: 
                    if (state==ADDRESSED) state=DEFAULT;
                    ctl_send(&device_dsc, sizeof(device_dsc));
                    break;
                case USB_CONFIGURATION_DESCRIPTOR_TYPE: 
                    len=usb_setup.wLen;
                    if (len>sizeof(cfgDescriptor)) len=sizeof(cfgDescriptor);
                    ctl_send(&cfgDescriptor[0], len);
                    break;
                case USB_STRING_DESCRIPTOR_TYPE: 
                    switch (usb_setup.wValueL)
                    {
                        case 0:
                            ctl_send(&strLanguage[0], sizeof(strLanguage));
                        break;
                        case 1:
                            ctl_send(&strManufacturer[0], sizeof(strManufacturer));
                        break;
                        case 2:
                            ctl_send(&strProduct[0], sizeof(strProduct));
                        break;
                        case 3:
                            ctl_send(&strSerial[0], sizeof(strSerial));
                        break;
                    }
                    break;
                case USB_INTERFACE_DESCRIPTOR_TYPE:  
                    if (usb_setup.wIndexL==0) ctl_send(&cfgDescriptor[9], 0x9);
                    if (usb_setup.wIndexL==1) ctl_send(&cfgDescriptor[44], 0x9);      
                    break;
                case USB_ENDPOINT_DESCRIPTOR_TYPE: 
                    if (usb_setup.wValueL==1)
                    {
                        ctl_send(&cfgDescriptor[28], 0x7);
                    }
                    if (usb_setup.wValueL==0x81)
                    {
                        ctl_send(&cfgDescriptor[28], 0x7);
                    }
                    break;
                case 6:  // QUALIFIER
                  ctl_send(&dev_qualifier[0], sizeof(dev_qualifier));
                break;
            }
            break;
        case STD_GET_CONFIGURATION:
            ctl_send(&current_cnf, 1);
            break;
        case STD_SET_CONFIGURATION:
            state=CONFIGURED; 
            LEDB_OFF
            cdc_init_endpoints();            
            data_need_zlp=0;
            ctl_send(0, 0);
            // allow to receive/send frames within EP1
            break;
        case STD_SET_ADDRESS:
            state=ADDRESS_PENDING;
            ctl_send(0, 0);
            dev_addr=usb_setup.wValueL;
            break;
    }
}

int get_state()
{
    return state;
}

void SetupStage()
{
    // Disarm EP0 in the both directions
    endpoints[0].in.STAT.UOWN=0;
    endpoints[0].out.STAT.UOWN=0;
    ctl_stage=SETUP;
    request_handled=0;
    // handle USB chapter 9 standard request
    process_standart_request();
    // handle USB class-specific request
    process_cdc_request(&usb_setup);
    // !PUT HERE YOUR CUSTOM REQUESTS HANDLERS
    
    // END OF HANDLING    
    if (request_handled==0)
    {
        ep_stall(0);
    } else
    {
        if (usb_setup.bmRequestType & 0x80)
        {
            DataInStage();
            ctl_stage=DATA_IN;
            endpoints[0].out.CNT=0x00;
            endpoints[0].out.STAT.Val=_USIE | _DTSEN | _DAT1;
            
            endpoints[0].in.STAT.Val=_USIE | _DTSEN | _DAT1;
        } else
        {
            ctl_stage=DATA_OUT;
            endpoints[0].in.CNT=0;
            endpoints[0].in.STAT.Val=_USIE | _DTSEN | _DAT1;
            
            endpoints[0].out.CNT=EP0_BUFF_SIZE;
            endpoints[0].out.STAT.Val=_USIE | _DTSEN | _DAT1;
        }
    }
    UCONbits.PKTDIS=0;
}

void DataInStage()
{
    uint8_t current_transfer_len=0;
    current_transfer_len=min(wCount, EP0_BUFF_SIZE);
    endpoints[0].in.CNT=current_transfer_len;
    memcpy(&ep_data_buffer[EP0_IN_OFFSET], ibuf, current_transfer_len);
    ibuf+=current_transfer_len;
    wCount-=current_transfer_len;
}

void DataOutStage()
{
    uint8_t current_transfer_len=0;
    current_transfer_len=endpoints[0].out.CNT;
    memcpy(obuf, &usb_setup, current_transfer_len);
    obuf+=current_transfer_len;
    wCount-=current_transfer_len;
}

void StatusStage()
{
    ctl_stage=STATUS_IN;
    endpoints[0].in.ADR=&ep_data_buffer[EP0_IN_OFFSET];
    endpoints[0].in.CNT=0;
    endpoints[0].in.STAT.Val=_USIE | _DTSEN | _DAT1; 
}

void WaitForSetupStage(void)
{ 
    ctl_stage=SETUP;
    request_handled=0;
    control_need_zlp=0;
    endpoints[0].out.CNT=EP0_BUFF_SIZE;
    endpoints[0].out.ADR=(uint8_t*)&usb_setup;
    endpoints[0].out.STAT.Val=_USIE | _DTSEN;
    UCONbits.PKTDIS=0;
}

void UnSuspend(void)
{
    UCONbits.SUSPND=0;   // Bring USB module out of power conserve
    UIEbits.ACTVIE=0;
    UIR &= 0xFB;
}

void Suspend(void)
{
    UIEbits.ACTVIE=1;    // Enable bus activity interrupt
    UIR &= 0xEF;
    UCONbits.SUSPND=1;   // Put USB module in power conserve
}

void usb_poll()
{
    uint8_t PID=0;
    uint8_t ep=0;
    if (UIRbits.ACTVIF)
    {
        if (state==SLEEP)
        {
            state=CONFIGURED;
            UnSuspend();
        }
        UIRbits.ACTVIF=0;
    }
    if (UIRbits.IDLEIF)
    {
        if (state==CONFIGURED)
        {
            state=SLEEP;
            Suspend();
            UIRbits.IDLEIF=0;
        }
    }
    if (UIRbits.STALLIF)
    {
        if (UEP0bits.EPSTALL)
        {
            UEP0bits.EPSTALL=0;
            WaitForSetupStage();
        }
        UIRbits.STALLIF=0;
    }
    if (UIRbits.URSTIF)
    {
        URSTIF=0;
        if (state!=ADDRESS_PENDING) reset_usb();
    }
    while (UIRbits.TRNIF)
    {  
        ep=(uint8_t)(USTAT >> 3) & 0x7;
        if (ep==0)
        {   
            if (USTAT & 1<<2)
            {
                PID=endpoints[0].in.STAT.PID;
                if (PID==IN_PID)
                {  
                    if (state==ADDRESS_PENDING && UADDR==0)
                    { 
                        state=ADDRESSED;
                        UADDR=dev_addr;
                    }
                    if (ctl_stage==DATA_IN)
                    {
                        if (wCount==0) 
                        {
                            if (control_need_zlp)
                            {
                                control_need_zlp=0;
                                endpoints[0].in.CNT=0; 
                                usb_buf_in_rdy(0);
                            } else StatusStage();
                        } 
                        else
                        {
                            DataInStage();
                            usb_buf_in_rdy(0);
                        }
                    } else
                    if (ctl_stage==STATUS_IN)
                    {
                        WaitForSetupStage();
                    }
                }
            } else
            {   
                PID=endpoints[0].out.STAT.PID;
                if (PID==SETUP_PID)
                {
                    SetupStage();
                } else
                if (PID==OUT_PID)
                {   
                    if (ctl_stage==DATA_OUT)
                    {
                        DataOutStage();
                        if (wCount==0)
                        { 
                            if (control_need_zlp)
                            {
                                control_need_zlp=0;
                                endpoints[0].out.CNT=0; 
                                usb_buf_out_rdy(0);
                            } else
                            {
                                // return to the App data received from host
                                if (ctl_callback!=0)
                                {
                                    if (ctl_callback(&last_request, received_data, received_data_len)==0)
                                    {
                                        WaitForSetupStage();
                                    } else
                                    {
                                        ep_stall(0);
                                    }
                                } else WaitForSetupStage();
                            }
                        } else
                        {
                            usb_buf_out_rdy(0);
                        }
                    } else WaitForSetupStage();
                }
            }
        }
        else
        {
            if (USTAT & 1<<2)
            {
                PID=endpoints[ep].in.STAT.PID;
                if (PID==IN_PID)
                {
                    endpoints[ep].in.STAT.UOWN=0;
                    handle_cdc_tx();
                }
            }else
            {
                PID=endpoints[ep].out.STAT.PID;
                if (PID==OUT_PID)
                {
                    endpoints[ep].out.STAT.UOWN=0;
                    // ZLP
                    if (endpoints[ep].out.CNT % EP1_BUFF_SIZE && endpoints[ep].out.CNT>0)
                    {
                        endpoints[ep].in.CNT=0;
                        usb_buf_out_rdy(0);
                    }
              
                    if (data_callback!=0) data_callback(ep, endpoints[ep].out.ADR, endpoints[ep].out.CNT);
                    endpoints[ep].out.CNT=EP1_BUFF_SIZE;
                    usb_buf_out_rdy(ep);
                }
            }
        }
        TRNIF=0;
    }
}
