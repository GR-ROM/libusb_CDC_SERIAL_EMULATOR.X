#include "usb.h"
#include "usbdesc.h"
#include "usb_cdc.h"
#include <xc.h>
#include <string.h>
#include <stdlib.h>


static LINE_CODING line;

uint8_t* obuf;
uint16_t wCount;
uint8_t cdc_state;
uint8_t current_transaction_len;

static volatile uint8_t ep_data_buffer[192] __at(BD_DATA_ADDR);

void process_cdc_request(USB_SETUP_t *usb_setup)
{   
    uint16_t request=(usb_setup->bRequest | (usb_setup->bmRequestType << 8));
    switch (request)
    {   
        case GET_LINE_CODING:
            ctl_send(&line, 7);
            break;  
        case SET_LINE_CODING:
            ctl_recv(&line, 7);
            break;  
        case SET_CONTROL_LINE_STATE:
            ctl_send(0, 0);
            break;   
        case SEND_BREAK:
            ctl_send(0, 0);
            break;
        case SEND_ENCAPSULATED_COMMAND:
            ctl_send(0, 0);
            break;
    }
}

uint8_t cdc_get_state()
{
    return cdc_state;
}

void cdc_init_endpoints()
{
    line.dwDTERate=9600;
    line.bParityType=0;
    line.bCharFormat=0;
    line.bDataBits=8;
    configure_ep_in(1, &ep_data_buffer[EP1_IN_OFFSET], EP1_BUFF_SIZE,  0x00);
    configure_ep_out(1, &ep_data_buffer[EP1_OUT_OFFSET], EP1_BUFF_SIZE, _USIE | _DTSEN);
    configure_ep_in(2, &ep_data_buffer[EP2_IN_OFFSET], EP2_BUFF_SIZE, 0x00);
    UEP1=EP_IN | EP_OUT | EP_HSHK;
    UEP2=EP_IN | EP_HSHK;
    cdc_state=CDC_TX_READY;
}

void cdc_tx_buffer(uint8_t* buf, uint16_t len)
{
    if (cdc_state!=CDC_TX_READY)
    {
        return;
    }
    cdc_state=CDC_TX_BUSY;    
    obuf=buf;
    wCount=len;
    current_transaction_len=min(EP1_BUFF_SIZE, wCount);
    usb_tx_packet(1, obuf, current_transaction_len);
}

void handle_cdc_tx()
{
    if (cdc_state==CDC_TX_COMPLETING)
    {
        cdc_state==CDC_TX_READY;
        return;
    } else
    if (cdc_state==CDC_TX_BUSY)
    {
        wCount-=current_transaction_len;
        obuf+=current_transaction_len;
        if (wCount>0)
        {
            current_transaction_len=min(EP1_BUFF_SIZE, wCount);
            usb_tx_packet(1, obuf, current_transaction_len);
            return;
        } else        
        if (wCount==0)
        { 
            if (current_transaction_len==EP1_BUFF_SIZE)
            {
                current_transaction_len=0;
                cdc_state=CDC_TX_BUSY_ZLP;
                usb_tx_packet(1, 0, 0);
                return;
            } else cdc_state=CDC_TX_COMPLETING;
        }
    } else
    if (cdc_state==CDC_TX_BUSY_ZLP)
    {
        cdc_state=CDC_TX_COMPLETING;
    }
}

void cdc_rx(uint8_t ep, uint8_t* buf, uint16_t len)
{
    if (ep==1)
    {
        usb_tx_packet(1, &buf[0], len);
    }
}

uint8_t ctl_rx(USB_SETUP_t* usb_setup, uint8_t* buf, uint16_t len)
{
    uint16_t request=(usb_setup->bRequest | (usb_setup->bmRequestType << 8));
    if (request==SET_LINE_CODING)
    {
        if (((LINE_CODING*)buf)->dwDTERate>SERIAL_PORT_MAX_SPEED)
        {
            // to do
            // set_serial_parameters(baudrate, bits, etc...
            return (uint8_t)-1; // if host trying to set a speed that exceeding maximum limit of the bus, we send STALL response
        }
    }
    return 0;
}

void init_usb_cdc()
{
    usb_rx_callback_t cdc_rx_callback=cdc_rx;
    usb_ctl_rx_callback_t ctl_rx_callback=ctl_rx;
    register_ctl_callback(ctl_rx_callback);
    register_data_callback(cdc_rx_callback);
    wCount=0;
    cdc_state=CDC_TX_BUSY;
}