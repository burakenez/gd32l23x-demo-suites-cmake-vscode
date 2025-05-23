/*!
    \file    gd32l23x_syscfg.c
    \brief   SYSCFG driver

    \version 2025-02-10, V2.2.0, firmware for GD32L23x, add support for GD32L235
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

#include "gd32l23x_syscfg.h"

/*!
    \brief      reset the SYSCFG registers
    \param[in]  none
    \param[out] none
    \retval     none
*/
void syscfg_deinit(void)
{
    rcu_periph_reset_enable(RCU_SYSCFGRST);
    rcu_periph_reset_disable(RCU_SYSCFGRST);
}

/*!
    \brief      configure the GPIO pin as EXTI Line
    \param[in]  exti_port: specify the GPIO port used in EXTI
                only one parameter can be selected which is shown as below:
      \arg        EXTI_SOURCE_GPIOx(x = A,B,C,D,F): EXTI GPIO port
    \param[in]  exti_pin: specify the EXTI line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_SOURCE_PINx(GPIOAx = 0..15, GPIOBx = 0..15, GPIOCx = 0..15, GPIODx = 0..6,8,9, GPIOFx = 0,1): EXTI GPIO pin
    \param[out] none
    \retval     none
*/
void syscfg_exti_line_config(uint8_t exti_port, uint8_t exti_pin)
{
    uint32_t clear_exti_mask = ~((uint32_t)EXTI_SS_MASK << (EXTI_SS_MSTEP(exti_pin)));
    uint32_t config_exti_mask = ((uint32_t)exti_port) << (EXTI_SS_MSTEP(exti_pin));

    switch(exti_pin / EXTI_SS_JSTEP) {
    case EXTISS0:
        /* clear EXTI source line(0..3) */
        SYSCFG_EXTISS0 &= clear_exti_mask;
        /* configure EXTI source line(0..3) */
        SYSCFG_EXTISS0 |= config_exti_mask;
        break;
    case EXTISS1:
        /* clear EXTI source line(4..7) */
        SYSCFG_EXTISS1 &= clear_exti_mask;
        /* configure EXTI source line(4..7) */
        SYSCFG_EXTISS1 |= config_exti_mask;
        break;
    case EXTISS2:
        /* clear EXTI source line(8..11) */
        SYSCFG_EXTISS2 &= clear_exti_mask;
        /* configure EXTI source line(8..11) */
        SYSCFG_EXTISS2 |= config_exti_mask;
        break;
    case EXTISS3:
        /* clear EXTI source line(12..15) */
        SYSCFG_EXTISS3 &= clear_exti_mask;
        /* configure EXTI source line(12..15) */
        SYSCFG_EXTISS3 |= config_exti_mask;
        break;
    default:
        break;
    }
}

/*!
    \brief      enable remap pin function for small packages
    \param[in]  remap_pin
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_PA11_PA12_REMAP: PA11 PA12 remap
      \arg        SYSCFG_BOOT0_PD3_REMAP: BOOT0 PD3 remap
      \arg        SYSCFG_PA8_REMAP: PA8 remap (only for GD32L235)
      \arg        SYSCFG_PD0_PD1_PD2_REMAP: PD0 PD1 PD2 remap (only for GD32L235)
      \arg        SYSCFG_PA11_PA12_PB6_PB8_REMAP: PA11 PA12 PB6 PB8 remap (only for GD32L235)
    \param[out] none
    \retval     none
*/
void syscfg_pin_remap_enable(uint32_t remap_pin)
{
    SYSCFG_CFG0 |= remap_pin;
}

/*!
    \brief      disable remap pin function for small packages
    \param[in]  remap_pin
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_PA11_PA12_REMAP: PA11 PA12 remap
      \arg        SYSCFG_BOOT0_PD3_REMAP: BOOT0 PD3 remap
      \arg        SYSCFG_PA8_REMAP: PA8 remap (only for GD32L235)
      \arg        SYSCFG_PD0_PD1_PD2_REMAP: PD0 PD1 PD2 remap (only for GD32L235)
      \arg        SYSCFG_PA11_PA12_PB6_PB8_REMAP: PA11 PA12 PB6 PB8 remap (only for GD32L235)
    \param[out] none
    \retval     none
*/
void syscfg_pin_remap_disable(uint32_t remap_pin)
{
    SYSCFG_CFG0 &= (uint32_t)~remap_pin;
}

/*!
    \brief      enable PBx(x=6,7,8,9) high current capability
    \param[in]  syscfg_gpio
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_PB6_HIGH_CURRENT: PB6 pin high current capability
      \arg        SYSCFG_PB7_HIGH_CURRENT: PB7 pin high current capability
      \arg        SYSCFG_PB8_HIGH_CURRENT: PB8 pin high current capability
      \arg        SYSCFG_PB9_HIGH_CURRENT: PB9 pin high current capability
    \param[out] none
    \retval     none
*/
void syscfg_high_current_enable(uint32_t syscfg_gpio)
{
    SYSCFG_CFG0 |= syscfg_gpio;
}

/*!
    \brief      disable PBx(x=6,7,8,9) high current capability
    \param[in]  syscfg_gpio
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_PB6_HIGH_CURRENT: PB6 pin high current capability
      \arg        SYSCFG_PB7_HIGH_CURRENT: PB7 pin high current capability
      \arg        SYSCFG_PB8_HIGH_CURRENT: PB8 pin high current capability
      \arg        SYSCFG_PB9_HIGH_CURRENT: PB9 pin high current capability
    \param[out] none
    \retval     none
*/
void syscfg_high_current_disable(uint32_t syscfg_gpio)
{
    SYSCFG_CFG0 &= (uint32_t)~syscfg_gpio;
}

/*!
    \brief      set the IRQ_LATENCY value
    \param[in]  irq_latency: IRQ_LATENCY value (0x00 - 0xFF)
    \param[out] none
    \retval     none
*/
void irq_latency_set(uint8_t irq_latency)
{
    uint32_t reg;

    reg = SYSCFG_CPU_IRQ_LAT & (~(uint32_t)SYSCFG_CPU_IRQ_LAT_IRQ_LATENCY);
    reg |= (uint32_t)(IRQ_LATENCY(irq_latency));

    SYSCFG_CPU_IRQ_LAT = reg;
}

/*!
    \brief      get the current boot mode
    \param[in]  none
    \param[out] none
    \retval     the boot mode
      \arg        SYSCFG_BOOTMODE_FLASH: boot from the main flash
      \arg        SYSCFG_BOOTMODE_SYSTEM: boot from the system flash memory
      \arg        SYSCFG_BOOTMODE_SRAM: boot from the embedded SRAM
*/
uint8_t syscfg_bootmode_get(void)
{
    /* get the bootmode */
    uint8_t bootmode = (uint8_t)(SYSCFG_CFG0 & SYSCFG_CFG0_BOOT_MODE);

    return bootmode;
}

#ifdef GD32L235
/*!
    \brief      insert wait state in read accesses of SRAM0/SRAM1 when LDO is 1.1V
    \param[in]  none
    \param[out] none
    \retval     none
*/
void syscfg_sram_waitstate_insert(void)
{
    SYSCFG_CFG1 |= SYSCFG_CFG1_SRAM_LP_WATISTATE;
}

/*!
    \brief      cancel insertion of wait state in read accesses of SRAM0/SRAM1 when LDO is 1.1V
    \param[in]  none
    \param[out] none
    \retval     none
*/
void syscfg_sram_waitstate_cancel(void)
{
    SYSCFG_CFG1 &= (uint32_t)~SYSCFG_CFG1_SRAM_LP_WATISTATE;
}

/*!
    \brief      connect TIMER0/14/40 break input to the selected parameter
    \param[in]  syscfg_lock: specify the parameter to be connected
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_LOCK_LOCKUP: Cortex-M23 lockup output connected to the break input
      \arg        SYSCFG_LOCK_SRAM_PARITY_ERROR: SRAM_PARITY check error connected to the break input
      \arg        SYSCFG_LOCK_LVD: LVD interrupt connected to the break input
    \param[out] none
    \retval     none
*/
void syscfg_lock_config(uint32_t syscfg_lock)
{
    uint32_t reg;
    reg = SYSCFG_CFG1;
    /* clear LOCKUP_LOCK, SRAM_PARITY_ERROR_LOCK and LVD_LOCK bits */
    reg &= (uint32_t)~(SYSCFG_CFG1_LOCKUP_LOCK | SYSCFG_CFG1_SRAM_PARITY_ERROR_LOCK | SYSCFG_CFG1_LVD_LOCK);
    /* configure SYSCFG lock */
    reg |= syscfg_lock;

    SYSCFG_CFG1 = reg;
}

/*!
    \brief      check if the specified flag in SYSCFG_CFG1 is set or not
    \param[in]  syscfg_flag: specify the flag in SYSCFG_CFG1 to check
      \arg        SYSCFG_FLAG_SRAM_PCEF: SRAM parity check error flag
    \param[out] none
    \retval     the syscfg_flag state returned (SET or RESET)
  */
FlagStatus syscfg_flag_get(uint32_t syscfg_flag)
{
    if((SYSCFG_CFG1 & syscfg_flag) != (uint32_t)RESET) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear the flag in SYSCFG_CFG1 by writing 1
    \param[in]  syscfg_flag: specify the flag in SYSCFG_CFG1 to clear
      \arg        SYSCFG_FLAG_SRAM_PCEF: SRAM parity check error flag
    \param[out] none
    \retval     none
*/
void syscfg_flag_clear(uint32_t syscfg_flag)
{
    SYSCFG_CFG1 |= syscfg_flag;
}

#endif /* GD32L235 */
