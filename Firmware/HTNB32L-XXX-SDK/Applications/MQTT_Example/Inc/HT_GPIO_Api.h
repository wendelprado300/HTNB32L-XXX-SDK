/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 Copyright (c) 2023 HT Micron Semicondutors S.A.
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

*/

/*!
 * \file HT_GPIO_Api.h
 * \brief GPIO API used for HTNB32L-XXX MQTT Example. 
 * \author HT Micron Advanced R&D
 * \link https://github.com/htmicron
 * \version 0.1
 * \date February 23, 2023
 */

#ifndef __HT_GPIO_API_H__
#define __HT_GPIO_API_H__

#include "stdint.h"
#include "main.h"
#include "pad_qcx212.h"
#include "gpio_qcx212.h"
#include "HT_Fsm.h"

/* Defines  ------------------------------------------------------------------*/

#define BUTTON_PAD_ALT_FUNC      PAD_MuxAlt0                                    /**</ Button pin alternate function. */
#define LED_PAD_ALT_FUNC         PAD_MuxAlt0                                    /**</ LED pin alternate function. */

#define LED_ON  1                                                               /**</ LED state enable. */
#define LED_OFF 0                                                               /**</ LED state disable. */

/*
----------------------------------------------------  Table 1. GPIO Table. -------------------------------------------------------------
__________________________________________________________________________________________________________________________________________________________________
| Pad ID |       GPIO Number    |  Pin Number  | Instance | Pull (default:options) |       AF0      |      AF1      |    AF2      |      AF3      |  AF4 |   AF5  |
|--------|----------------------|--------------|----------|------------------------|----------------|---------------|-------------|---------------|------|--------|
|   11   |        GPIO0         |      0       |    0     |      B-PU:nppd         |  GPIO0         |  SPI0_SSn1    |  UART2_RXD  |      –        |   –  |    –   | 
|   12   |        GPIO1         |      1       |    0     |      B-PU:nppd         |  GPIO1         |      –	      |  UART2_TXD  |      –        |   –  |    –   | 
|   13   |        GPIO2         |      2       |    0     |      B-PU:nppd         |  GPIO2/Timer0  |  UART0_RTSn   |  UART2_RXD  |    SPI1_SSn0  |   –  |  PWM0  |
|   14   |        GPIO3         |      3       |    0     |      B-PU:nppd         |  GPIO3         |  UART0_CTSn   |  UART2_TXD  |    SPI1_MOSI  |   –  |  PWM1  |
|   15   |        GPIO4         |      4       |    0     |      B-PU:nppd         |  GPIO4         |  UART0_RXD    |  I2C1_SDA   |    SPI1_MISO  |   –  |  PWM2  |
|   16   |        GPIO5         |      5       |    0     |      B-PU:nppd         |  GPIO5         |  UART0_TXD    |  I2C1_SCL   |    SPI1_SCLK  |   –  |  PWM3  |
|   17   |        GPIO6         |      6       |    0     |      B-PU:nppd         |  GPIO6/Timer1  |  SPI0_SSn0    |  I2C0_SDA   |    UART1_RTSn |   –  |  PWM4  |
|   18   |        GPIO7         |      7       |    0     |      B-PU:nppd         |  GPIO7         |  SPI0_MOSI    |  I2C0_SCL   |    UART1_CTSn |   –  |  PWM5  |
|   19   |        GPIO13        |      13      |    0     |      B-PU:nppd         |  GPIO13        |  SPI0_MISO    |  I2C1_SDA   |    UART1_RXD  |   –  |  PWM0  |
|   20   |        GPIO12        |      12      |    0     |      B-PU:nppd         |  GPIO12        |  SPI0_SCLK    |  I2C1_SCL   |    UART1_TXD  |   –  |  PWM1  |
|   21   |        GPIO16        |      0       |    1     |      B-PU:nppd         |  GPIO16        |  UART1_RTSn   |  SPI0_SSn0  |    UART0_RTSn |   –  |  PWM2  |
|   22   |        GPIO11        |      11      |    0     |      B-PU:nppd         |  GPIO11        |  UART1_CTSn   |  SPI0_MOSI  |    UART0_CTSn |   –  |  PWM3  |
|   23   |        GPIO14        |      14      |    0     |      B-PU:nppd         |  GPIO14        |  UART1_RXD    |  SPI0_MISO  |    UART0_RXD  |   –  |  PWM4  |
|   24   |        GPIO15        |      15      |    0     |      B-PU:nppd         |  GPIO15        |  UART1_TXD    |  SPI0_SCLK  |    UART0_TXD  |   –  |  PWM5  |
|   25   |        GPIO10        |      10      |    0     |      B-PU:nppd         |  GPIO10/Timer3 |  I2C0_SCL     |      –      |    SPI1_SSn1  |   –  |  PWM0  |
|   26   |        GPIO8         |      8       |    0     |      B-PU:nppd         |  GPIO8/Timer2  |  I2C0_SDA     |      –      |      –        |   –  |  PWM1  |
|   27   |        GPIO17        |      1       |    1     |      B-PU:nppd         |  GPIO17        |  I2C1_SCL     |      –      |      –        |   –  |  PWM2  |
|   28   |        GPIO9         |      9       |    0     |      B-PU:nppd         |  GPIO9         |  I2C1_SDA     |      –      |      –        |   –  |  PWM3  |
|   29   |        GPIO18        |      2       |    1     |      B-PU:nppd         |  GPIO18        |  UART2_RXD    |      –      |      –        |   –  |  PWM4  |
|   30   |        GPIO19        |      3       |    1     |      B-PU:nppd         |  GPIO19/Timer4 |  UART2_TXD    |      –      |      –        |   –  |  PWM5  |
|   31   |   AON_GPIO0 (GPIO20)	|      4       |    1     |      DIO-PD:nppu       |  GPIO20/Timer5 |      –        |      –      |      –        |   –  |    –   |
|   32   |   AON_GPIO1 (GPIO21)	|      5       |    1     |      DIO-PD:nppu       |  GPIO21        |      –        |      –      |      –        |   –  |    –   |
|   33   |   AON_GPIO2 (GPIO22)	|      6       |    1     |      DIO-PD:nppu       |  GPIO22        |      –        |      –      |      –        |   –  |    –   |
|   34   |   AON_GPIO3 (GPIO23)	|      7       |    1     |      DIO-PD:nppu       |  GPIO23        |      –        |      –      |      –        |   –  |    –   |
|   35   |   AON_GPIO4 (GPIO24)	|      8       |    1     |      DIO-PD:nppu       |  GPIO24        |      –        |      –      |      –        |   –  |    –   |
|   36   |   AON_GPIO5 (GPIO25)	|      9       |    1     |      DIO-PD:nppu       |  GPIO25        |      –        |      –      |      –        |   –  |    –   |
___________________________________________________________________________________________________________________________________________________________________

-> B  : Bidirectional digital with CMOS input		
-> DIO: Digital input output		
-> NP : pdpu = default no-pull with programmable options following the colon (:)		
-> PD : nppu = default pull-down with programmable options following the colon (:)		
-> PU : nppd = default pull-up with programmable options following the colon (:)		

* Disclaimer:
- Although Table 1 shows the list of all QCX212 pins, some of these GPIOs are not available on HTNB32L-XXX. Please refer to HTNB32L-XXX datasheet 
in order to check the oficial pinout.

*/

//GPIO19 - BLUE BUTTON
#define BLUE_BUTTON_INSTANCE          1                                         /**</ Blue button pin instance. */
#define BLUE_BUTTON_PIN               3                                         /**</ Blue button pin number. */
#define BLUE_BUTTON_PAD_ID            30                                        /**</ Blue button Pad ID. */
#define BLUE_BUTTON_MASK              (uint32_t)(1 << BLUE_BUTTON_PIN)          /**</ Blue button mask. */

//GPIO7 - WHITE BUTTON
#define WHITE_BUTTON_INSTANCE          0                                        /**</ White button pin instance. */
#define WHITE_BUTTON_PIN               7                                        /**</ White button pin number. */
#define WHITE_BUTTON_PAD_ID            18                                       /**</ White button Pad ID. */
#define WHITE_BUTTON_MASK              (uint32_t)(1 << WHITE_BUTTON_PIN)        /**</ White button mask. */

//GPIO3 - BLUE LED
#define BLUE_LED_INSTANCE             0                                         /**</ Blue LED pin instance. */
#define BLUE_LED_PIN                  3                                         /**</ Blue LED pin number. */
#define BLUE_LED_PAD_ID               14                                        /**</ Blue LED Pad ID. */

//GPIO4 - WHITE LED
#define WHITE_LED_INSTANCE             0                                        /**</ White LED pin instance. */
#define WHITE_LED_PIN                  4                                        /**</ White LED pin number. */
#define WHITE_LED_PAD_ID               15                                       /**</ White LED Pad ID. */

//GPIO5 - GREEN LED
#define GREEN_LED_INSTANCE             0                                        /**</ Green LED pin instance. */
#define GREEN_LED_PIN                  5                                        /**</ Green LED pin number. */
#define GREEN_LED_PAD_ID               16                                       /**</ Green LED Pad ID. */

/* Typedefs  ------------------------------------------------------------------*/

/**
 * \enum HT_Led_Type
 * \brief HTNB32L-XXX Demo Board leds.
 */
typedef enum {
    HT_BLUE_LED = 0,
    HT_WHITE_LED,
    HT_GREEN_LED
} HT_Led_Type;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_GPIO_ButtonInit(void)
 * \brief Initialize blue and white buttons in the GPIO pins
 *        with internal pull-up resistors.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_GPIO_ButtonInit(void);

/*!******************************************************************
 * \fn void HT_GPIO_LedInit(void)
 * \brief Initialize blue, white and green LEDs in the GPIO pins.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_GPIO_LedInit(void);

/*!******************************************************************
 * \fn void HT_GPIO_WritePin(uint16_t pin, uint32_t instance, uint16_t value)
 * \brief Writes the value in pin of instance. Used to turn on/off LEDs.
 *
 * \param[in] uint16_t pin                              GPIO port number
 * \param[in] uint32_t instance                         GPIO instance
 * \param[in] uint16_t value                            value to write
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_GPIO_WritePin(uint16_t pin, uint32_t instance, uint16_t value);

#endif /* __HT_GPIO_API_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
