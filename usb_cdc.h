/* 
 * File:   usb_cdc.h
 * Author: grinev
 *
 * Created on 13 ???? 2017 ?., 9:08
 */

#ifndef USB_CDC_H
#define	USB_CDC_H

#ifdef	__cplusplus
extern "C" {
#endif

void init_usb_cdc();
void handle_cdc_tx();
void cdc_init_endpoints();
void process_cdc_request(USB_SETUP_t *usb_setup);
void cdc_tx_buffer(uint8_t* buf, uint16_t len);
uint8_t cdc_get_state();


#ifdef	__cplusplus
}
#endif

#endif	/* USB_CDC_H */

