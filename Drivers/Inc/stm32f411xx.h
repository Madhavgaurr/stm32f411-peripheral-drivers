/*
 * stm32f411xx.h
 *
 *  Created on: Jul 3, 2026
 *      Author: madhav-gaur
 */

#ifndef STM32F411XX_H_
#define STM32F411XX_H_

/*

* stm32f411xx.h
*
* Created on: Jun 13, 2026
* ```
   Author: Madhav Gaur
  ```
*
* Description:
* ```
   This file contains the base address definitions, register
  ```
* ```
   structures for all STM32F411xx peripherals, and generic
  ```
* ```
   macros required for peripheral access and configuration.
  ```

*/

#include <stdint.h>

/*************************************************************************************************
 *                                 Embedded Flash Memory Interface
 *************************************************************************************************/



 /*************************************************************************************************
 *                                       Bus Base Addresses
 *************************************************************************************************/

#define PERIPHERAL_BASE_ADDR             0x40000000UL
#define APB1_PERIPHERAL_BASE_ADDR        0x40000000UL
#define APB2_PERIPHERAL_BASE_ADDR        0x40010000UL
#define AHB1_PERIPHERAL_BASE_ADDR        0x40020000UL
#define AHB2_PERIPHERAL_BASE_ADDR        0x50000000UL

/*************************************************************************************************
 *                                  AHB1 Peripheral Base Addresses
            GPIOA, GPIOB, GPIOC, GPIOD, GPIOH, DMA1, DMA2, RCC, CRC , Flash Interface Register
 *************************************************************************************************/

#define GPIOA_BASE_ADDR                  ( AHB1_PERIPHERAL_BASE_ADDR + 0x0000 )
#define GPIOB_BASE_ADDR                  ( AHB1_PERIPHERAL_BASE_ADDR + 0x0400 )
#define GPIOC_BASE_ADDR                  ( AHB1_PERIPHERAL_BASE_ADDR + 0x0800 )
#define GPIOD_BASE_ADDR                  ( AHB1_PERIPHERAL_BASE_ADDR + 0x0C00 )
#define GPIOE_BASE_ADDR                  ( AHB1_PERIPHERAL_BASE_ADDR + 0x1000 )
#define GPIOH_BASE_ADDR                  ( AHB1_PERIPHERAL_BASE_ADDR + 0x1C00 )
#define CRC_BASE_ADDR                    ( AHB1_PERIPHERAL_BASE_ADDR + 0x3000 )
#define RCC_BASE_ADDR                    ( AHB1_PERIPHERAL_BASE_ADDR + 0x3800 )
// FIR -> Flash Interface Register
#define FIR_BASE_ADDR                    ( AHB1_PERIPHERAL_BASE_ADDR + 0x3C00 )
#define DMA1_BASE_ADDR                   ( AHB1_PERIPHERAL_BASE_ADDR + 0X6000 )
#define DMA2_BASE_ADDR                   ( AHB1_PERIPHERAL_BASE_ADDR + 0X6400 )


/*************************************************************************************************
 *                                AHB2 Peripheral Base Addresses
*************************************************************************************************/

#define USBOTGFS_BASE_ADDR               ( AHB2_PERIPHERAL_BASE_ADDR + 0x0000 )

/*************************************************************************************************
 *                                APB1 Peripheral Base Addresses
*************************************************************************************************/

#define TIM2_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x0000 )
#define TIM3_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x0400 )
#define TIM4_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x0800 )
#define TIM5_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x0C00 )
#define RTC_BKP_BASE_ADDR               ( APB1_PERIPHERAL_BASE_ADDR + 0x2800 )
#define WWDG_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x2C00 )
#define IWDG_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x3000 )
#define I2S2ext_BASE_ADDR               ( APB1_PERIPHERAL_BASE_ADDR + 0x3400 )
#define SPI2_I2S2_BASE_ADDR             ( APB1_PERIPHERAL_BASE_ADDR + 0x3800 )
#define SPI3_I2S3_BASE_ADDR             ( APB1_PERIPHERAL_BASE_ADDR + 0x3C00 )
#define I2S3ext_BASE_ADDR               ( APB1_PERIPHERAL_BASE_ADDR + 0x4000 )
#define USART2_BASE_ADDR                ( APB1_PERIPHERAL_BASE_ADDR + 0x4400 )
#define I2C1_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x5400 )
#define I2C2_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x5800 )
#define I2C3_BASE_ADDR                  ( APB1_PERIPHERAL_BASE_ADDR + 0x5C00 )
#define PWR_BASE_ADDR                   ( APB1_PERIPHERAL_BASE_ADDR + 0x7000 )

/*************************************************************************************************
 *                                APB2 Peripheral Base Addresses
*************************************************************************************************/

#define TIM1_BASE_ADDR                  ( APB2_PERIPHERAL_BASE_ADDR + 0x0000 )
#define USART1_BASE_ADDR                ( APB2_PERIPHERAL_BASE_ADDR + 0x1000 )
#define USART6_BASE_ADDR                ( APB2_PERIPHERAL_BASE_ADDR + 0x1400 )
#define ADC1_BASE_ADDR                  ( APB2_PERIPHERAL_BASE_ADDR + 0x2000 )
#define SDIO_BASE_ADDR                  ( APB2_PERIPHERAL_BASE_ADDR + 0x2C00 )
#define SPI1_I2S1_BASE_ADDR             ( APB2_PERIPHERAL_BASE_ADDR + 0x3000 )
#define SPI4_I2S4_ADDR                  ( APB2_PERIPHERAL_BASE_ADDR + 0x3400 )
#define SYSCFG_BASE_ADDR                ( APB2_PERIPHERAL_BASE_ADDR + 0x3800 )
#define EXTI_BASE_ADDR                  ( APB2_PERIPHERAL_BASE_ADDR + 0x3C00 )
#define TIM9_BASE_ADDR                  ( APB2_PERIPHERAL_BASE_ADDR + 0x4000 )
#define TIM10_BASE_ADDR                 ( APB2_PERIPHERAL_BASE_ADDR + 0x4400 )
#define TIM11_BASE_ADDR                 ( APB2_PERIPHERAL_BASE_ADDR + 0x4800 )
#define SPI5_I2S5_BASE_ADDR             ( APB2_PERIPHERAL_BASE_ADDR + 0x5000 )


/*************************************************************************************************
 *                                  Peripheral Register Structures
*************************************************************************************************/

typedef struct
{
      volatile uint32_t PWR_CR          ;
      volatile uint32_t PWR_CSR         ;
}PWR_RegDef_t;



typedef struct
{
      volatile uint32_t CR              ;
      volatile uint32_t PLLCFGR         ;
      volatile uint32_t CFGR            ;
      volatile uint32_t CIR             ;
      volatile uint32_t AHB1RSTR        ;
      volatile uint32_t AHB2RSTR        ;
      volatile uint32_t RESERVED0[2]    ;
      volatile uint32_t APB1RSTR        ;
      volatile uint32_t APB2RSTR        ;
      volatile uint32_t RESERVED1[2]    ;
      volatile uint32_t AHB1ENR         ;
      volatile uint32_t AHB2ENR         ;
      volatile uint32_t RESERVED2[2]    ;
      volatile uint32_t APB1ENR         ;
      volatile uint32_t APB2ENR         ;
      volatile uint32_t RESERVED3[2]    ;
      volatile uint32_t AHB1LPENR       ;
      volatile uint32_t AHB2LPENR       ;
      volatile uint32_t RESERVED4[2]    ;
      volatile uint32_t APB1LPENR       ;
      volatile uint32_t APB2LPENR       ;
      volatile uint32_t RESERVED5[2]    ;
      volatile uint32_t BDCR            ;
      volatile uint32_t CSR             ;
      volatile uint32_t RESERVED6[2]    ;
      volatile uint32_t SSCGR           ;
      volatile uint32_t PLLI2SCFGR      ;
      volatile uint32_t DCKCFGR         ;
}RCC_RegDef_t;


typedef struct
{
      volatile uint32_t MEMRMP          ;
      volatile uint32_t PMC             ;
      volatile uint32_t EXTICR1         ;
      volatile uint32_t EXTICR2         ;
      volatile uint32_t EXTICR3         ;
      volatile uint32_t EXTICR4         ;
      volatile uint32_t CMPCR           ;
}SYSCFG_RegDef_t;

typedef struct
{
      volatile uint32_t MODER           ;
      volatile uint32_t OTYPER          ;
      volatile uint32_t OSPEEDR         ;
      volatile uint32_t PUPDR           ;
      volatile uint32_t IDR             ;
      volatile uint32_t ODR             ;
      volatile uint32_t BSRR            ;
      volatile uint32_t LCKR            ;
      volatile uint32_t AFRL            ;
      volatile uint32_t AFRH            ;

}GPIO_RegDef_t;


// DMA AND Interrupts remaining

typedef struct
{
      volatile uint32_t      S3M1AR     ;
      volatile uint32_t      S3FCR           ;
      volatile uint32_t      S4CR         ;
      volatile uint32_t      S4NDTR         ;
      volatile uint32_t      S4PAR             ;
      //volatile uint32_t              ;
}DMA_RegDef_t;


typedef struct
{
      volatile uint32_t SR              ;
      volatile uint32_t CR1             ;
      volatile uint32_t CR2             ;
      volatile uint32_t SMPR1           ;
      volatile uint32_t SMPR2           ;
      volatile uint32_t JOFR1           ;
      volatile uint32_t JOFR2           ;
      volatile uint32_t JOFR3           ;
      volatile uint32_t JOFR4           ;
      volatile uint32_t HTR             ;
      volatile uint32_t LTR             ;
      volatile uint32_t SQR1            ;
      volatile uint32_t SQR2            ;
      volatile uint32_t SQR3            ;
      volatile uint32_t JSQR            ;
      volatile uint32_t JDR1            ;
      volatile uint32_t JDR2            ;
      volatile uint32_t JDR3            ;
      volatile uint32_t JDR4            ;
      volatile uint32_t DR              ;
      volatile uint32_t CCR             ;
}ADC_RegDef_t;











#endif /* STM32F411XX_H_ */
