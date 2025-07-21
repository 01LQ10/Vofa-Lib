#ifndef __VOFA_BASIC_H__
#define __VOFA_BASIC_H__

#include "vofa.h"
#include "vofa_interface.h"

extern vofa_t vofa_handle;

#define vofa_basic_log(fmt, ...) vofa_log(&vofa_handle, fmt, ##__VA_ARGS__)

void vofa_basic_init(void);
void vofa_basic_loop(void);

#endif
