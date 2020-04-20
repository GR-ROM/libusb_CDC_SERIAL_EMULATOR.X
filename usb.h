/* 
 * File:   usb.h
 * Author: exp10d3r
 *
 * Created on March 12, 2016, 10:49 PM
 */

#ifndef USB_H
#define	USB_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define GET_STATUS 0x00
#define CLEAR_FEATURE 0x01
#define SET_FEATURE 0x03

/* USB standard request code */
#define STD_GET_STATUS_ZERO           0x8000
#define STD_GET_STATUS_INTERFACE      0x8100
#define STD_GET_STATUS_ENDPOINT       0x8200

#define STD_CLEAR_FEATURE_ZERO        0x0001
#define STD_CLEAR_FEATURE_INTERFACE   0x0101
#define STD_CLEAR_FEATURE_ENDPOINT    0x0201

#define STD_SET_FEATURE_ZERO          0x0003
#define STD_SET_FEATURE_INTERFACE     0x0103
#define STD_SET_FEATURE_ENDPOINT      0x0203

#define STD_SET_ADDRESS               0x0005
#define STD_GET_DESCRIPTOR            0x8006
#define STD_GET_DESCRIPTOR_DEVICE     0x8006
#define STD_GET_DESCRIPTOR_INTERFACE  0x8106
#define STD_GET_DESCRIPTOR_ENDPOINT   0x8206
#define STD_SET_DESCRIPTOR            0x0007
#define STD_GET_CONFIGURATION         0x8008
#define STD_SET_CONFIGURATION         0x0009
#define STD_GET_INTERFACE             0x810A
#define STD_SET_INTERFACE             0x010B
#define STD_SYNCH_FRAME               0x820C
    
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

/* HID Class */
// HID Descriptor Types
#define HID_INTF_ID             0x00
#define HID_EP 					1
#define HID_INT_OUT_EP_SIZE     64
#define HID_INT_IN_EP_SIZE      64
#define HID_NUM_OF_DSC          1
#define HID_RPT01_SIZE          29
    
    
    /* HID Interface Class Code */
#define HID_INTF                    0x03
    
#define DSC_HID                 0x21 // HID Class Descriptor
#define DSC_REPORT          0x22 // HID Report Descriptor 
#define DSC_HID_PHYSICAL        0x23
         
#define SIZE_OF_REPORT 0x8
#define HID_GET_REPORT 0xA101    
    
#define HID_CLASS               0xA1
// HID Request Codes
#define GET_REPORT              0x01 // Code for Get Report
#define GET_IDLE                0x02 // Code for Get Idle
#define GET_PROTOCOL            0x03 // Code for Get Protocol
#define SET_REPORT              0x09 // Code for Set Report
#define SET_IDLE                0x0A // Code for Set Idle
#define SET_PROTOCOL            0x0B // Code for Set Protocol

#define GET_LINE_CODING             0xA121
#define SET_LINE_CODING             0x2120
#define SET_CONTROL_LINE_STATE      0x2122

#define SEND_ENCAPSULATED_COMMAND   0x2100
#define GET_ENCAPSULATED_RESPONSE   0xA101
#define SEND_BREAK                  0x2123
    
#define EP_HSHK 1<<4
#define EP_CONDIS 1<<3
#define EP_OUT 1<<2
#define EP_IN 1<<1
#define EP_STALL 1    
    
#define OUT_PID 0x1
#define ACK_PID 0x2
#define DATA0_PID 0x3
#define PING_PID 0x4
#define NYET_PID 0x6
#define DATA2_PID 0x7
#define IN_PID 0x9
#define NAK_PID 0xa
#define DATA1_PID 0xB
#define SETUP_PID 0xD
#define STALL_PID 0xe
#define MDATA_PID 0xf
    
    #define _EP_IN      0x80
#define _EP_OUT     0x00
#define _EP01_OUT   0x01
#define _EP01_IN    0x81
#define _EP02_OUT   0x02
#define _EP02_IN    0x82
#define _EP03_OUT   0x03
#define _EP03_IN    0x83
#define _EP04_OUT   0x04
#define _EP04_IN    0x84
#define _EP05_OUT   0x05
#define _EP05_IN    0x85
#define _EP06_OUT   0x06
#define _EP06_IN    0x86
#define _EP07_OUT   0x07
#define _EP07_IN    0x87
#define _EP08_OUT   0x08
#define _EP08_IN    0x88
#define _EP09_OUT   0x09
#define _EP09_IN    0x89
#define _EP10_OUT   0x0A
#define _EP10_IN    0x8A
#define _EP11_OUT   0x0B
#define _EP11_IN    0x8B
#define _EP12_OUT   0x0C
#define _EP12_IN    0x8C
#define _EP13_OUT   0x0D
#define _EP13_IN    0x8D
#define _EP14_OUT   0x0E
#define _EP14_IN    0x8E
#define _EP15_OUT   0x0F
#define _EP15_IN    0x8F

    
#define _INTERRUPT        0x03            //Interrupt Transfer
    
#define USB_UNICODE(a)			(a), 0x00

#define 	USB_DESCRIPTOR_CONFIGURATION   0x02
#define 	USB_DESCRIPTOR_DEVICE   0x01
#define 	USB_DESCRIPTOR_DEVICE_QUALIFIER   0x06
#define 	USB_DESCRIPTOR_ENDPOINT   0x05
#define 	USB_DESCRIPTOR_INTERFACE   0x04
#define 	USB_DESCRIPTOR_INTERFACE_POWER   0x08
#define 	USB_DESCRIPTOR_OTG   0x09
#define 	USB_DESCRIPTOR_OTHER_SPEED   0x07
#define 	USB_DESCRIPTOR_STRING   0x03
#define 	USB_FEATURE_DEVICE_REMOTE_WAKEUP   1
#define 	USB_FEATURE_DEVICE_REMOTE_WAKEUP   1
#define 	USB_FEATURE_ENDPOINT_HALT   0
#define 	USB_FEATURE_ENDPOINT_STALL   0
#define 	USB_FEATURE_TEST_MODE   2
#define 	USB_FEATURE_TEST_MODE   2
#define 	USB_REQUEST_CLEAR_FEATURE   1
#define 	USB_REQUEST_GET_CONFIGURATION   8
#define 	USB_REQUEST_GET_DESCRIPTOR   6
#define 	USB_REQUEST_GET_INTERFACE   10
#define 	USB_REQUEST_GET_STATUS   0
#define 	USB_REQUEST_SET_ADDRESS   5
#define 	USB_REQUEST_SET_CONFIGURATION   9
#define 	USB_REQUEST_SET_DESCRIPTOR   7
#define 	USB_REQUEST_SET_FEATURE   3
#define 	USB_REQUEST_SET_INTERFACE   11
#define 	USB_REQUEST_SYNCH_FRAME   12
    
#define 	USB_SETUP_DEVICE_TO_HOST   0x80
#define 	USB_SETUP_HOST_TO_DEVICE   0x00
#define 	USB_SETUP_RECIPIENT_DEVICE   0x00
#define 	USB_SETUP_RECIPIENT_ENDPOINT   0x02
#define 	USB_SETUP_RECIPIENT_INTERFACE   0x01
#define 	USB_SETUP_RECIPIENT_OTHER   0x03
#define 	USB_SETUP_TYPE_CLASS   0x20
#define 	USB_SETUP_TYPE_STANDARD   0x00
#define 	USB_SETUP_TYPE_VENDOR   0x40
#define 	USB_TRANSFER_TYPE_BULK   0x02
#define 	USB_TRANSFER_TYPE_CONTROL   0x00
#define 	USB_TRANSFER_TYPE_INTERRUPT   0x03
#define 	USB_TRANSFER_TYPE_ISOCHRONOUS   0x01
    
#define _DEFAULT 0x80
    
    #define DESC_CONFIG_WORD(a) (a&0xFF),((a>>8)&0xFF)
    
    
   
 typedef struct  _USB_DEVICE_DESCRIPTOR
{
    uint8_t bLength;               // Length of this descriptor.
    uint8_t bDescriptorType;       // DEVICE descriptor type (USB_DESCRIPTOR_DEVICE).
    uint16_t bcdUSB;                // USB Spec Release Number (BCD).
    uint8_t bDeviceClass;          // Class code (assigned by the USB-IF). 0xFF-Vendor specific.
    uint8_t bDeviceSubClass;       // Subclass code (assigned by the USB-IF).
    uint8_t bDeviceProtocol;       // Protocol code (assigned by the USB-IF). 0xFF-Vendor specific.
    uint8_t bMaxPacketSize0;       // Maximum packet size for endpoint 0.
    uint16_t idVendor;              // Vendor ID (assigned by the USB-IF).
    uint16_t idProduct;             // Product ID (assigned by the manufacturer).
    uint16_t bcdDevice;             // Device release number (BCD).
    uint8_t iManufacturer;         // Index of String Descriptor describing the manufacturer.
    uint8_t iProduct;              // Index of String Descriptor describing the product.
    uint8_t iSerialNumber;         // Index of String Descriptor with the device's serial number.
    uint8_t bNumConfigurations;    // Number of possible configurations.
} USB_DEVICE_DESCRIPTOR;
    
    typedef struct USB_CONFIGURATION_DESCRIPTOR{
        uint8_t  bLength;             // size of descriptor in bytes          
        uint8_t  bDescriptorType;     // config descriptor code
        uint8_t  wTotalLengthL;       // config+interface+endpoint descriptors
        uint8_t  wTotalLengthH;
        uint8_t  bNumInterfaces;      // Number of interfaces
        uint8_t  bConfigurationValue; // Value to select this interface
        uint8_t  iConfiguration;      // Index of string descriptor
        uint8_t  bmAttributes;        // Powered (self or hub)
        uint8_t  bMaxPower;           // Power usage
    } USB_CONFIGURATION_DESCRIPTOR_t;
    
   typedef struct USB_INTERFACE_DESCRIPTOR {
    unsigned char bLength;            // size of descriptor in bytes
    unsigned char bDescriptorType;    // interface descriptor code
    unsigned char bInterfaceNumber;   // Number of interfaces
    unsigned char bAlternateSetting;  // Value to select alt interface
    unsigned char bNumEndpoints;      // Num endpoints used
    unsigned char bInterfaceClass;    // Class code
    unsigned char bInterfaceSubClass; // Subclass code
    unsigned char bInterfaceProtocol; // Protocol code
    unsigned char iInterface;         // Index of String Descriptor
} USB_INTERFACE_DESCRIPTOR_t;

typedef struct USB_ENDPOINT_DESCRIPTOR {
    unsigned char  bLength;          // size of descriptor in bytes 
    unsigned char  bDescriptorType;  // endpoint descriptor type
    unsigned char  bEndpointAddress; // endpoint bitmap
    unsigned char  bmAttributes;     // attributes bitmap
    unsigned short wMaxPacketSize;   // max packet size
    unsigned char  bInterval;        // polling interval (ignored for control) 
} USB_ENDPOINT_DESCRIPTOR_t;   
    
typedef struct USB_SETUP{
        uint8_t bmRequestType;
        uint8_t bRequest;
        uint8_t wValueL;
        uint8_t wValueH;
        uint8_t wIndexL;
        uint8_t wIndexH;
        uint16_t wLen;
    }USB_SETUP_t;
    
    
typedef struct LineCoding{
	uint32_t DTERRate;
	uint8_t CharFormat;
	uint8_t ParityType;
	uint8_t DataBits;
} LineCoding_t;

/* Buffer Descriptor Status Register Initialization Parameters */
#define _BSTALL     0x04        //Buffer Stall enable
#define _DTSEN      0x08        //Data Toggle Synch enable
#define _INCDIS     0x10        //Address increment disable
#define _KEN        0x20        //SIE keeps buff descriptors enable
#define _DAT0       0x00        //DATA0 packet expected next
#define _DAT1       0x40        //DATA1 packet expected next
#define _DTSMASK    0x40        //DTS Mask
#define _USIE       0x80        //SIE owns buffer
#define _UCPU       0x00        //CPU owns buffer

#define _STAT_MASK  0xFF

/* Buffer Descriptor Status Register Initialization Parameters */
#define _BSTALL     0x04        //Buffer Stall enable
#define _DTSEN      0x08        //Data Toggle Synch enable
#define _INCDIS     0x10        //Address increment disable
#define _KEN        0x20        //SIE keeps buff descriptors enable
#define _DAT0       0x00        //DATA0 packet expected next
#define _DAT1       0x40        //DATA1 packet expected next
#define _DTSMASK    0x40        //DTS Mask
#define _USIE       0x80        //SIE owns buffer
#define _UCPU       0x00        //CPU owns buffer

#define _STAT_MASK  0xFF


   
    enum usb_state{
    DEFAULT,
    POWERED,
    ATTACHED,
    DETACHED,
    ADDRESS_PENDING,
    ADDRESSED,
    CONFIGURED,
    SLEEP
};
    
 enum{PIPE_BUSY,PIPE_IDLE};
    
    typedef struct USB_PIPE
    {
        int endpoint_buffer_len;
        char* buffer;
        int current_transaction_len;
        int total_bytes_to_transmit;
        uint8_t flags;
    } USB_PIPE_t;
    
#define NETWORK_CONNECTION          0x00
#define RESPONSE_AVAILABLE          0x01
#define SERIAL_STATE                0x20
/* Device Class Code */
#define CDC_DEVICE                  0x02

/* Communication Interface Class Code */
#define COMM_INTF                   0x02

/* Communication Interface Class SubClass Codes */
#define ABSTRACT_CONTROL_MODEL      0x02

/* Communication Interface Class Control Protocol Codes */
#define V25TER                      0x01    // Common AT commands ("Hayes(TM)")


/* Data Interface Class Codes */
#define DATA_INTF                   0x0A

/* Data Interface Class Protocol Codes */
#define NO_PROTOCOL                 0x00    // No class specific protocol required


/* Communication Feature Selector Codes */
#define ABSTRACT_STATE              0x01
#define COUNTRY_SETTING             0x02

/* Functional Descriptors */
/* Type Values for the bDscType Field */
#define CS_INTERFACE                0x24
#define CS_ENDPOINT                 0x25

/* bDscSubType in Functional Descriptors */
#define DSC_FN_HEADER               0x00
#define DSC_FN_CALL_MGT             0x01
#define DSC_FN_ACM                  0x02    // ACM - Abstract Control Management
#define DSC_FN_DLM                  0x03    // DLM - Direct Line Managment
#define DSC_FN_TELEPHONE_RINGER     0x04
#define DSC_FN_RPT_CAPABILITIES     0x05
#define DSC_FN_UNION                0x06
#define DSC_FN_COUNTRY_SELECTION    0x07
#define DSC_FN_TEL_OP_MODES         0x08
#define DSC_FN_USB_TERMINAL         0x09
/* more.... see Table 25 in USB CDC Specification 1.1 */

/* CDC Bulk IN transfer states */
#define CDC_TX_READY                0
#define CDC_TX_BUSY                 1
#define CDC_TX_BUSY_ZLP             2       // ZLP: Zero Length Packet
#define CDC_TX_COMPLETING           3

/* Line Coding Structure */
#define LINE_CODING_LENGTH          0x07

typedef union _LINE_CODING
{
    struct
    {
        uint8_t _byte[LINE_CODING_LENGTH];
    };
    struct
    {
        uint32_t   dwDTERate;          // Complex data structure
        uint8_t    bCharFormat;
        uint8_t    bParityType;
        uint8_t    bDataBits;
    };
} LINE_CODING;

typedef union _CONTROL_SIGNAL_BITMAP
{
    uint8_t _byte;
    struct
    {
        unsigned DTE_PRESENT;       // [0] Not Present  [1] Present
        unsigned CARRIER_CONTROL;   // [0] Deactivate   [1] Activate
    };
} CONTROL_SIGNAL_BITMAP;


/* Functional Descriptor Structure - See CDC Specification 1.1 for details */

/* Header Functional Descriptor */
typedef struct _USB_CDC_HEADER_FN_DSC
{
    uint8_t bFNLength;
    uint8_t bDscType;
    uint8_t bDscSubType;
    uint16_t bcdCDC;
} USB_CDC_HEADER_FN_DSC;

/* Abstract Control Management Functional Descriptor */
typedef struct _USB_CDC_ACM_FN_DSC
{
    uint8_t bFNLength;
    uint8_t bDscType;
    uint8_t bDscSubType;
    uint8_t bmCapabilities;
} USB_CDC_ACM_FN_DSC;

/* Union Functional Descriptor */
typedef struct _USB_CDC_UNION_FN_DSC
{
    uint8_t bFNLength;
    uint8_t bDscType;
    uint8_t bDscSubType;
    uint8_t bMasterIntf;
    uint8_t bSaveIntf0;
} USB_CDC_UNION_FN_DSC;

/* Call Management Functional Descriptor */
typedef struct _USB_CDC_CALL_MGT_FN_DSC
{
    uint8_t bFNLength;
    uint8_t bDscType;
    uint8_t bDscSubType;
    uint8_t bmCapabilities;
    uint8_t bDataInterface;
} USB_CDC_CALL_MGT_FN_DSC;

#if defined(_16F1455) || defined(_16F1454) || defined(_16F1459)
#define BD_BASE_ADDR 0x2000
#define BD_DATA_ADDR 0x2040
#define DB_DATA_LIMIT 0x2100
#endif
#if defined(_18F14K50) || defined(_18F13K50)
#define BD_BASE_ADDR 0x200
#define BD_DATA_ADDR 0x240
#define DB_DATA_LIMIT 0x300
#endif
#if defined(_18F2455) || defined(_18F2550) || defined(_18F4455) || defined(_18F4550)
#define BD_BASE_ADDR 0x400
#define BD_DATA_ADDR 0x440
#define DB_DATA_LIMIT 0x500
#endif

typedef void (*usb_rx_callback_t)(uint8_t ep, uint8_t* data, uint16_t len);
typedef uint8_t (*usb_ctl_rx_callback_t)(USB_SETUP_t* request, uint8_t* rx_data, uint16_t len);

void init_usb();
void usb_poll();
void ctl_send(uint8_t* data, uint16_t len);
void ctl_recv(uint8_t* data, uint16_t len);
void configure_ep_in(uint8_t ep, uint8_t* buf, uint8_t len, uint8_t mode);
void configure_ep_out(uint8_t ep, uint8_t* buf, uint8_t len, uint8_t mode);
void usb_tx_packet(uint8_t ep, uint8_t* buf, uint16_t len);
void register_ctl_callback(usb_ctl_rx_callback_t _callback);
void register_data_callback(usb_rx_callback_t _callback);
//int HID_rxbuffer(char* buf, uint8_t len); replaced by the callback!

#ifdef	__cplusplus
}
#endif

#endif	/* USB_H */

