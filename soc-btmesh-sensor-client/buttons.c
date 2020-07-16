/***************************************************************************//**
 * @file  buttons.c
 * @brief Buttons implementation file
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "hal-config.h"
#include "native_gecko.h"
#include <gpiointerrupt.h>
#include <em_rtcc.h>
#include "buttons.h"

/***************************************************************************//**
 * @addtogroup Buttons
 * @{
 ******************************************************************************/

/** Timer Frequency used. */
#define TIMER_CLK_FREQ ((uint32_t)32768)
/** Convert msec to timer ticks. */
#define TIMER_MS_2_TIMERTICK(ms) ((TIMER_CLK_FREQ * ms) / 1000)
/// Number of ticks after which the press is considered long (1s)
#define TICKS_FOR_1_SECOND           TIMER_MS_2_TIMERTICK(1000)

/// button press timestamp for Push Button 0 press detection
static uint32_t pb0_press;

/*******************************************************************************
 * Button initialization. Configure pushbuttons PB0, PB1 as inputs.
 ******************************************************************************/
void button_init(void)
{
  // configure pushbutton PB0 and PB1 as inputs, with pull-up enabled
  GPIO_PinModeSet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, gpioModeInputPull, 1);
#ifndef FEATURE_ONE_BUTTON
  GPIO_PinModeSet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, gpioModeInputPull, 1);
#endif
}

/***************************************************************************//**
 * This is a callback function that is invoked each time a GPIO interrupt
 * in one of the pushbutton inputs occurs. Pin number is passed as parameter.
 *
 * @param[in] pin  Pin number where interrupt occurs
 *
 * @note This function is called from ISR context and therefore it is
 *       not possible to call any BGAPI functions directly. The button state
 *       change is signaled to the application using gecko_external_signal()
 *       that will generate an event gecko_evt_system_external_signal_id
 *       which is then handled in the main loop.
 ******************************************************************************/
void button_interrupt(uint8_t pin)
{
  uint32_t t_diff;
  if (pin == BSP_BUTTON0_PIN) {
    if (GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN) == 0) {
      // PB0 pressed - record RTCC timestamp
      pb0_press = RTCC_CounterGet();
    } else {
      // PB0 released - check if it was short or long (min 1 sec) press
      t_diff = RTCC_CounterGet() - pb0_press;
      if (t_diff < TICKS_FOR_1_SECOND) {
        gecko_external_signal(EXT_SIGNAL_PB0_PRESS);
      } else {
        gecko_external_signal(EXT_SIGNAL_PB0_MEDIUM_PRESS);
      }
    }
  }
  #ifndef FEATURE_ONE_BUTTON
  else if (pin == BSP_BUTTON1_PIN) {
    if (GPIO_PinInGet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN) == 1) {
      gecko_external_signal(EXT_SIGNAL_PB1_PRESS);
    }
  }
  #endif
}

/*******************************************************************************
 * Enable button interrupts for PB0, PB1. Both GPIOs are configured to trigger
 * an interrupt on the rising edge (button released).
 ******************************************************************************/
void enable_button_interrupts(void)
{
  GPIOINT_Init();
  /* configure interrupt for PB0 and PB1, both falling and rising edges */
  GPIO_ExtIntConfig(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, BSP_BUTTON0_PIN,
                    true, true, true);
  GPIOINT_CallbackRegister(BSP_BUTTON0_PIN, button_interrupt);
#ifndef FEATURE_ONE_BUTTON
  GPIO_ExtIntConfig(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, BSP_BUTTON1_PIN,
                    true, true, true);
  /* register the callback function that is invoked when interrupt occurs */
  GPIOINT_CallbackRegister(BSP_BUTTON1_PIN, button_interrupt);
#endif
}

/** @} (end addtogroup Buttons) */
