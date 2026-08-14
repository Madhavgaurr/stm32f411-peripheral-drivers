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
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

/************************************************************************************************
 ***************************************ARM Cortex Processor Specific Details*********************
 *************************************************************************************************/

 #define NVIC_ISER0                  (*( volatile uint32_t*)0xE000E100)
 #define NVIC_ISER1                  (*( volatile uint32_t*)0xE000E104)
 #define NVIC_ISER2                  (*( volatile uint32_t*)0xE000E108)
 #define NVIC_ISER3                  (*( volatile uint32_t*)0xE000E10C)



 #define NVIC_ICER0                  (*( volatile uint32_t*)0xE000E180)
 #define NVIC_ICER1                  (*( volatile uint32_t*)0xE000E184)
 #define NVIC_ICER2                  (*( volatile uint32_t*)0xE000E188)
 #define NVIC_ICER3                  (*( volatile uint32_t*)0xE000E18C)

 #define SYSTICK_BASE_ADDR   0xE000E010UL

 #define NVIC_IPR_BASE_ADDR          (( volatile uint32_t*)0xE000E400)

 #define NO_PR_BITS_IMPLEMENTED      4 


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
#define SPI4_I2S4_BASE_ADDR                  ( APB2_PERIPHERAL_BASE_ADDR + 0x3400 )
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
      volatile uint32_t MODER           ;
      volatile uint32_t OTYPER          ;
      volatile uint32_t OSPEEDR         ;
      volatile uint32_t PUPDR           ;
      volatile uint32_t IDR             ;
      volatile uint32_t ODR             ;
      volatile uint32_t BSRR            ;
      volatile uint32_t LCKR            ;
      volatile uint32_t AFR[2]          ;
}GPIO_RegDef_t;



// typedef struct
// {
//       volatile uint32_t      S3M1AR     ;
//       volatile uint32_t      S3FCR           ;
//       volatile uint32_t      S4CR         ;
//       volatile uint32_t      S4NDTR         ;
//       volatile uint32_t      S4PAR             ;
//       //volatile uint32_t              ;
// }DMA_RegDef_t;


typedef struct 
{
      volatile uint32_t IMR    ;
      volatile uint32_t EMR    ;
      volatile uint32_t RTSR   ;
      volatile uint32_t FTSR   ;
      volatile uint32_t SWIER  ;
      volatile uint32_t PR     ;
      
}EXTI_RegDef_t;

typedef struct 
{
      volatile uint32_t MEMRMP    ;
      volatile uint32_t PMC       ;
      volatile uint32_t EXTICR[4] ;  
      volatile uint32_t CMPCR     ;   
}SYSCFG_RegDef_t ;




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


typedef struct 
{
      volatile uint32_t   CR1            ;
      volatile uint32_t   CR2            ;
      volatile uint32_t   SR             ;
      volatile uint32_t   DR             ;
      volatile uint32_t   CRCPR          ;
      volatile uint32_t   RXCRCR         ;
      volatile uint32_t   TXCRCR         ;
      volatile uint32_t   I2SCFGR        ;
      volatile uint32_t   I2SPR          ;
}SPI_RegDef_t;




#define      RCC      ((RCC_RegDef_t*)(RCC_BASE_ADDR))


#define      GPIOA    ((GPIO_RegDef_t*)(GPIOA_BASE_ADDR))
#define      GPIOB    ((GPIO_RegDef_t*)(GPIOB_BASE_ADDR))
#define      GPIOC    ((GPIO_RegDef_t*)(GPIOC_BASE_ADDR))
#define      GPIOD    ((GPIO_RegDef_t*)(GPIOD_BASE_ADDR))
#define      GPIOE    ((GPIO_RegDef_t*)(GPIOE_BASE_ADDR))
#define      GPIOH    ((GPIO_RegDef_t*)(GPIOH_BASE_ADDR))

// SYSCFG & EXTI

#define    EXTI                 ((EXTI_RegDef_t*)(EXTI_BASE_ADDR))
#define    SYSCFG               ((SYSCFG_RegDef_t*)(SYSCFG_BASE_ADDR))

#define    SPI1                 ((SPI_RegDef_t*)(SPI1_I2S1_BASE_ADDR))
#define    SPI2                 ((SPI_RegDef_t*)(SPI2_I2S2_BASE_ADDR))
#define    SPI3                 ((SPI_RegDef_t*)(SPI3_I2S3_BASE_ADDR))
#define    SPI4                 ((SPI_RegDef_t*)(SPI4_I2S4_BASE_ADDR))
#define    SPI5                 ((SPI_RegDef_t*)(SPI5_I2S5_BASE_ADDR))

//                               SPI PERIPHERAL ENABLE MACRO 
/*                               Clock Enable and Disable Macros for SPIx   peripheral                        */

#define    SPI1_PCLK_EN()         RCC->APB2ENR  |=  (1<<12)
#define    SPI2_PCLK_EN()         RCC->APB1ENR  |=  (1<<14)
#define    SPI3_PCLK_EN()         RCC->APB1ENR  |=  (1<<15)
#define    SPI4_PCLK_EN()         RCC->APB2ENR  |=  (1<<13)
#define    SPI5_PCLK_EN()         RCC->APB2ENR  |=  (1<<20)


#define    SPI1_PCLK_DI()         RCC->APB2ENR  &=  ~(1<<12)
#define    SPI2_PCLK_DI()         RCC->APB1ENR  &=  ~(1<<14)
#define    SPI3_PCLK_DI()         RCC->APB1ENR  &=  ~(1<<15)
#define    SPI4_PCLK_DI()         RCC->APB2ENR  &=  ~(1<<13)
#define    SPI5_PCLK_DI()         RCC->APB2ENR  &=  ~(1<<20)






//                            Clock Control Macros for GPIO Peripheral

#define            GPIOA_PCLK_EN()           RCC->AHB1ENR   |=   (1<<0)
#define            GPIOB_PCLK_EN()           RCC->AHB1ENR   |=   (1<<1)
#define            GPIOC_PCLK_EN()           RCC->AHB1ENR   |=   (1<<2)
#define            GPIOD_PCLK_EN()           RCC->AHB1ENR   |=   (1<<3)
#define            GPIOE_PCLK_EN()           RCC->AHB1ENR   |=   (1<<4)
#define            GPIOH_PCLK_EN()           RCC->AHB1ENR   |=   (1<<7)


#define            GPIOA_PCLK_DI()           RCC->AHB1ENR   &=   ~(1<<0)
#define            GPIOB_PCLK_DI()           RCC->AHB1ENR   &=   ~(1<<1)
#define            GPIOC_PCLK_DI()           RCC->AHB1ENR   &=   ~(1<<2)
#define            GPIOD_PCLK_DI()           RCC->AHB1ENR   &=   ~(1<<3)
#define            GPIOE_PCLK_DI()           RCC->AHB1ENR   &=   ~(1<<4)
#define            GPIOH_PCLK_DI()           RCC->AHB1ENR   &=   ~(1<<7)

//

/**********************************Macros to reset GPIOx peripheral*******************************/         
/*do while condition zero loop : This is a technique in 'C' programming to execute multiple 'C'
 statements using single 'C' macro                                                               */

#define    GPIOA_REG_RESET()    do{ RCC->AHB1RSTR |= (1<<0)  ; RCC->AHB1RSTR &= ~(1<<0)  ;  }while(0)
#define    GPIOB_REG_RESET()    do{ RCC->AHB1RSTR |= (1<<1)  ; RCC->AHB1RSTR &= ~(1<<1)  ;  }while(0)
#define    GPIOC_REG_RESET()    do{ RCC->AHB1RSTR |= (1<<2)  ; RCC->AHB1RSTR &= ~(1<<2)  ;  }while(0)
#define    GPIOD_REG_RESET()    do{ RCC->AHB1RSTR |= (1<<3)  ; RCC->AHB1RSTR &= ~(1<<3)  ;  }while(0)
#define    GPIOE_REG_RESET()    do{ RCC->AHB1RSTR |= (1<<4)  ; RCC->AHB1RSTR &= ~(1<<4)  ;  }while(0)
#define    GPIOH_REG_RESET()    do{ RCC->AHB1RSTR |= (1<<7)  ; RCC->AHB1RSTR &= ~(1<<7)  ;  }while(0)


// SYSCFG Enabling Macro

#define    SYSCFG_PCLK_EN()     ( RCC->APB2ENR |=  (1<<14) )


/**********************************************************************************************
 *                                    Generic Macros
 * ********************************************************************************************/

#define ENABLE            1
#define DISABLE           0

#define SET               ENABLE 
#define RESET             DISABLE

#define FLAG_RESET        RESET
#define FLAG_SET          SET

#define GPIO_PIN_SET      SET
#define GPIO_PIN_RESET    RESET


#define GPIO_BASEADDR_TO_CODE(x)                       ( (x==GPIOA) ? 0 :\
                                                         (x==GPIOB) ? 1 :\
									   (x==GPIOC) ? 2 :\
									   (x==GPIOD) ? 3 :\
									   (x==GPIOE) ? 4 :\
									   (x==GPIOH) ? 7 :0)

/*
    In STM32, an Interrupt Request Number (IRQ Number) is the number used by the NVIC
    (Nested Vector Interrupt Controller) to identify a particular interrupt source.

                                Think of it like this:

                                    Peripheral/Event
                                          ↓
                                  Generates Interrupt
                                          ↓
                                  Assigned IRQ Number
                                          ↓
                                  NVIC uses IRQ Number
                                          ↓
                              Corresponding ISR executes

    For EXTI, the mapping is not always 1-to-1.

    EXTI Line → IRQ Number Mapping (STM32F4)

    EXTI Line     | IRQ Name         | IRQ Number
    ---------------------------------------------
    EXTI0         | EXTI0_IRQn       | 6
    EXTI1         | EXTI1_IRQn       | 7
    EXTI2         | EXTI2_IRQn       | 8
    EXTI3         | EXTI3_IRQn       | 9
    EXTI4         | EXTI4_IRQn       | 10
    EXTI5-9       | EXTI9_5_IRQn     | 23
    EXTI10-15     | EXTI15_10_IRQn   | 40
*/

#define IRQ_NO_EXTI0         6
#define IRQ_NO_EXTI1         7
#define IRQ_NO_EXTI2         8
#define IRQ_NO_EXTI3         9
#define IRQ_NO_EXTI4         10
#define IRQ_NO_EXTI9_5       23
#define IRQ_NO_EXTI15_10     40

/*******************************************Priority Levels**************************************/

#define NVIC_IRQ_PRIO0       0
#define NVIC_IRQ_PRIO1       1
#define NVIC_IRQ_PRIO2       2
#define NVIC_IRQ_PRIO3       3
#define NVIC_IRQ_PRIO4       4
#define NVIC_IRQ_PRIO5       5
#define NVIC_IRQ_PRIO6       6
#define NVIC_IRQ_PRIO7       7
#define NVIC_IRQ_PRIO8       8
#define NVIC_IRQ_PRIO9       9
#define NVIC_IRQ_PRIO10      10
#define NVIC_IRQ_PRIO11      11
#define NVIC_IRQ_PRIO12      12
#define NVIC_IRQ_PRIO13      13
#define NVIC_IRQ_PRIO14      14
#define NVIC_IRQ_PRIO15      15



#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx_spi_driver.h"


#endif /* STM32F411XX_H_ */
