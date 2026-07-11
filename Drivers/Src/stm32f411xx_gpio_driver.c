/*
 * stm32f411xx_gpio_driver.c
 *
 *  Created on: Jul 9, 2026
 *      Author: madhav-gaur
 */


#include "stm32f411xx_gpio_driver.h"


void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx , uint8_t EnorDi)
{
    if(EnorDi==ENABLE)
    {
        if(pGPIOx==GPIOA)
        {
            GPIOA_PCLK_EN() ;
        }else if(pGPIOx==GPIOB)
        {
            GPIOB_PCLK_EN() ;
        }else if(pGPIOx==GPIOC)
        {
            GPIOC_PCLK_EN() ;
        }else if(pGPIOx==GPIOD)
        {
            GPIOD_PCLK_EN() ;
        }else if(pGPIOx==GPIOE)
        {
            GPIOE_PCLK_EN() ;
        }else if(pGPIOx==GPIOH)
        {
            GPIOH_PCLK_EN() ;
        }
    }else{
         if(pGPIOx==GPIOA)
        {
            GPIOA_PCLK_DI() ;
        }else if(pGPIOx==GPIOB)
        {
            GPIOB_PCLK_DI() ;
        }else if(pGPIOx==GPIOC)
        {
            GPIOC_PCLK_DI() ;
        }else if(pGPIOx==GPIOD)
        {
            GPIOD_PCLK_DI() ;
        }else if(pGPIOx==GPIOE)
        {
            GPIOE_PCLK_DI() ;
        }else if(pGPIOx==GPIOH)
        {
            GPIOH_PCLK_DI() ;
        }
    }
}

