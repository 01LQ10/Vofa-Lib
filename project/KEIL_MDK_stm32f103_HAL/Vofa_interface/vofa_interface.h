#ifndef _VOFA_INTERFACE_H_
#define _VOFA_INTERFACE_H_

#include "vofa.h"
#include "main.h"

uint8_t vofa_interface_init(void);
uint8_t vofa_interface_send(uint8_t *data, uint16_t len);
uint8_t vofa_interface_recv(uint8_t *data, uint16_t len);


void recv_buff_write(void);



#endif
