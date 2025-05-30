/*!
    \file    main.c
    \brief   RTC calendar demo

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

#include "gd32l23x.h"
#include <stdio.h>
#include "gd32l233r_eval.h"
#include "systick.h"

#define RTC_CLOCK_SOURCE_IRC32K
//#define RTC_CLOCK_SOURCE_LXTAL
#define BKP_VALUE    0x32F0

rtc_parameter_struct   rtc_initpara;
rtc_alarm_struct  rtc_alarm;
__IO uint32_t prescaler_a = 0, prescaler_s = 0;

void rtc_setup(void);
void rtc_show_time(void);
void rtc_show_alarm(void);
uint8_t usart_input_threshold(uint32_t value);
void rtc_pre_config(void);
uint32_t RTCSRC_FLAG = 0; 
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    systick_config();
    gd_eval_com_init(EVAL_COM);
    printf("\n\r  ****************** RTC calendar demo ******************\n\r");

    /* enable PMU and BKP clock */
    rcu_periph_clock_enable(RCU_PMU);
    rcu_periph_clock_enable(RCU_BKP);
    /* enable the access of the RTC registers */
    pmu_backup_write_enable();
    /* get RTC clock entry selection */
    RTCSRC_FLAG = GET_BITS(RCU_BDCTL, 8, 9);
    rtc_pre_config();

    /* check if RTC has aready been configured */
    if((BKP_VALUE != RTC_BKP0) || (0x00 == RTCSRC_FLAG)) {
    /* backup data register value is not correct or not yet programmed
        or RTC clock source is not configured (when the first time the program 
        is executed or data in RCU_BDCTL is lost due to Vbat feeding) */
        rtc_setup();
    } else {
        /* detect the reset source */
        if(RESET != rcu_flag_get(RCU_FLAG_PORRST)) {
            printf("power on reset occurred....\n\r");
        } else if(RESET != rcu_flag_get(RCU_FLAG_EPRST)) {
            printf("external reset occurred....\n\r");
        }
        printf("no need to configure RTC....\n\r");

        rtc_show_time();
    }

    rcu_all_reset_flag_clear();
    /* configure the leds */
    gd_eval_led_init(LED1);
    gd_eval_led_on(LED1);

    while(1) {
        rtc_show_time();
        delay_ms(1000);
    }
}

/*!
    \brief      RTC configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_pre_config(void)
{
#if defined (RTC_CLOCK_SOURCE_IRC32K)
    rcu_osci_on(RCU_IRC32K);
    rcu_osci_stab_wait(RCU_IRC32K);
    rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);

    prescaler_s = 0x13F;
    prescaler_a = 0x63;
#elif defined (RTC_CLOCK_SOURCE_LXTAL)
    rcu_osci_on(RCU_LXTAL);
    rcu_osci_stab_wait(RCU_LXTAL);
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    prescaler_s = 0xFF;
    prescaler_a = 0x7F;
#else
#error RTC clock source should be defined.
#endif /* RTC_CLOCK_SOURCE_IRC32K */

    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}

/*!
    \brief      use hyperterminal to setup RTC time and alarm
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_setup(void)
{
    /* setup RTC time value */
    uint32_t tmp_year = 0xFF, tmp_month = 0xFF, tmp_day = 0xFF;
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = 0x16;
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = 0x30;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;

    /* current time input */
    printf("=======Configure RTC Time========\n\r");
    printf("  please set the last two digits of current year:\n\r");
    while(tmp_year == 0xFF) {
        tmp_year = usart_input_threshold(99);
        rtc_initpara.year = tmp_year;
    }
    printf("  20%0.2x\n\r", tmp_year);

    printf("  please input month:\n\r");
    while(tmp_month == 0xFF) {
        tmp_month = usart_input_threshold(12);
        rtc_initpara.month = tmp_month;
    }
    printf("  %0.2x\n\r", tmp_month);

    printf("  please input day:\n\r");
    while(tmp_day == 0xFF) {
        tmp_day = usart_input_threshold(31);
        rtc_initpara.date = tmp_day;
    }
    printf("  %0.2x\n\r", tmp_day);

    printf("  please input hour:\n\r");
    while(tmp_hh == 0xFF) {
        tmp_hh = usart_input_threshold(23);
        rtc_initpara.hour = tmp_hh;
    }
    printf("  %0.2x\n\r", tmp_hh);

    printf("  please input minute:\n\r");
    while(tmp_mm == 0xFF) {
        tmp_mm = usart_input_threshold(59);
        rtc_initpara.minute = tmp_mm;
    }
    printf("  %0.2x\n\r", tmp_mm);

    printf("  please input second:\n\r");
    while(tmp_ss == 0xFF) {
        tmp_ss = usart_input_threshold(59);
        rtc_initpara.second = tmp_ss;
    }
    printf("  %0.2x\n\r", tmp_ss);

    /* RTC current time configuration */
    if(ERROR == rtc_init(&rtc_initpara)) {
        printf("\n\r** RTC time configuration failed! **\n\r");
    } else {
        printf("\n\r** RTC time configuration success! **\n\r");
        rtc_show_time();
        RTC_BKP0 = BKP_VALUE;
    }
}

/*!
    \brief      display the current time
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_show_time(void)
{
    rtc_current_time_get(&rtc_initpara);

    printf("\r\nCurrent time: 20%0.2x-%0.2x-%0.2x", \
           rtc_initpara.year, rtc_initpara.month, rtc_initpara.date);
    printf(" : %0.2x:%0.2x:%0.2x \r\n", \
           rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
}

/*!
    \brief      get the input character string and check if it is valid
    \param[in]  value : usart input value
    \param[out] none
    \retval     input value in BCD mode
*/
uint8_t usart_input_threshold(uint32_t value)
{
    uint32_t index = 0;
    uint32_t tmp[2] = {0, 0};

    while(index < 2) {
        while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_RBNE));
        tmp[index++] = usart_data_receive(EVAL_COM);
        if((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39)) {
            printf("\n\r please input a valid number between 0 and 9 \n\r");
            index--;
        }
    }

    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
    if(index > value) {
        printf("\n\r please input a valid number between 0 and %d \n\r", value);
        return 0xFF;
    }

    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) << 4);
    return index;
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM, (uint8_t) ch);
    while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TC));
    return ch;
}
