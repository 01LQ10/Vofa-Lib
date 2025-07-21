#ifndef __VOFA_PROTOCOL_JUSTFLOAT_H__
#define __VOFA_PROTOCOL_JUSTFLOAT_H__

#include "vofa.h"
#include <cstdarg>

uint8_t vofa_protocol_justfloat_send_data(vofa_t *handle);
uint8_t vofa_protocol_justfloat_send_image(vofa_t *handle);
uint8_t vofa_protocol_justfloat_send_txt(vofa_t *handle, const char *const fmt, ...);

#endif
