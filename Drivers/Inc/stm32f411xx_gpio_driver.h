/*
 * stm32f411xx_gpio_driver.h
 *
 *  Created on: Jul 9, 2026
 *      Author: madhav-gaur
 */

#ifndef INC_STM32F411XX_GPIO_DRIVER_H_
#define INC_STM32F411XX_GPIO_DRIVER_H_

#include "stm32f411xx.h"



/* Structure definition containing all possible configuration options for a GPIO pin. */

typedef struct{
     uint8_t   GPIO_PinMode        ;
     uint8_t   GPIO_PinNumber      ;
     uint8_t   GPIO_PinSpeed       ;
     uint8_t   GPIO_PinPuPdControl ;
     uint8_t   GPIO_OPType         ;
     uint8_t   GPIO_PinAltFunMode  ;
}GPIO_PinConfig_t;

typedef struct{
    /* Pointer to the register structure of GPIO peripheral */
    GPIO_RegDef_t        *pGPIOx        ;
    /* Structure variable of type GPIO_PinConfig_t.
     * It contains all the member variables required to configure
     * a GPIO pin according to the desired settings.
     */
    GPIO_PinConfig_t     GPIO_PinConfig ;
}GPIO_Handle_t;

/************************************************************************************************
 *                               APIs supported by this driver
 *                    For more info about APIs check the function definition
 * **********************************************************************************************/

//                                    Peripheral Clock Setup

void    GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx , uint8_t EnorDi) ;

//                                     GPIO Init and DeInit

void    GPIO_Init(GPIO_Handle_t   *pGPIOHandle) ;
void    GPIO_DeInit(GPIO_RegDef_t *pGPIOx     ) ;


/*                                      Data read and write                                      */

uint8_t  GPIO_ReadFromInputPin(GPIO_RegDef_t  *pGPIOx , uint8_t PinNumber)               ;
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t  *pGPIOx)                                  ;
void GPIO_WriteToOutputPin(GPIO_RegDef_t  *pGPIOx , uint8_t PinNumber,uint8_t Value)     ;
void GPIO_WriteToOutputPort(GPIO_RegDef_t  *pGPIOx , uint16_t Value)                     ;
void GPIO_ToggleOutputPin(GPIO_RegDef_t  *pGPIOx , uint8_t PinNumber)                    ;

/*
 *  IRQ configuration and IRQ handling
 *
 *  GPIO Interrupt Configuration
 *
 *  1) Enable GPIO + SYSCFG clocks
 *  2) Configure GPIO pin as input
 *  3) Configure pull-up / pull-down
 *  4) Map GPIO pin → EXTI line (SYSCFG)
 *  5) Configure EXTI trigger (Rising/Falling/Both)
 *  6) Unmask EXTI interrupt line
 *  7) Enable NVIC IRQ + set priority
 *  8) Write ISR (IRQ Handler)
 *  9) Clear EXTI pending flag inside ISR
 *
 *  EXTI0 line can be connected to GPIO pin 0 of any port (PA0, PB0, PC0, ...).
 *  The selection is done through the SYSCFG_EXTICR registers.
 *  Once selected, EXTI0 generates an interrupt request to the NVIC
 *  through the EXTI controller.
 */

void GPIO_IRQInterruptConfig(uint8_t IRQNumber ,  uint8_t EnorDi)            ;
void GPIO_IRQPriorityConfig(uint8_t IRQNumber , uint8_t IRQPriority)         ;
void GPIO_IRQHandling(uint8_t PinNumber)                                     ;

// GPIO → SYSCFG → EXTI → NVIC → ISR








#endif /* INC_STM32F411XX_GPIO_DRIVER_H_ */
