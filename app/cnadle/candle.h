#ifndef __CANDLE_H__
#define __CANDLE_H__

#include "vofa.h"
#include "vofa_interface.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

void drawpoint(uint8_t x, uint8_t y, uint8_t v);
void draw_circle(uint8_t x0, uint8_t y0, int r);
void clear_gram(void);


void vofa_candle_init(void);
void vofa_candle_run(void);

#endif
