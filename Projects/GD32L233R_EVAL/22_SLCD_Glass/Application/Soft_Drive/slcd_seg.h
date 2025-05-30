/*!
    \file    slcd_seg.h
    \brief   the header file of the slcd segment driver

    \version 2025-02-18, V2.4.0, demo for GD32L23x
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef LCD_SEG_H
#define LCD_SEG_H

#include "gd32l23x.h"

typedef enum {
    INTEGER = 0,
    FLOAT = 1,
    TIME = 2
} slcd_display_enum;

/* configure SLCD peripheral */
void slcd_seg_configuration(void);
/* write one digit to the SLCD DATA register */
void slcd_seg_digit_display(uint8_t ch, uint8_t position);
/* write a integer(6 digits) to SLCD DATA register */
void slcd_seg_number_display(uint32_t num);
/* write a float number(6 digits which has 2 decimal) to SLCD DATA register */
void slcd_seg_floatnumber_display(float num);
/* clear data in the SLCD DATA register */
void slcd_seg_digit_clear(uint8_t position);
/* clear all the SLCD DATA register */
void slcd_seg_clear_all(void);

#endif /* LCD_SEG_H */
