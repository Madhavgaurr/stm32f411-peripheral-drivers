/*
 * stm32f411xx_spi_driver.h
 *
 *  Created on: Jul 17, 2026
 *      Author: madhav-gaur
 */

#ifndef INC_STM32F411XX_SPI_DRIVER_H_
#define INC_STM32F411XX_SPI_DRIVER_H_

#include "stm32f411xx.h"


/*
     Configuration structure of SPIx peripheral 
*/

typedef struct 
{
    uint8_t SPI_DeviceMode  ; 
    uint8_t SPI_BusConfig   ; 
    uint8_t SPI_SclkSpeed   ; 
    uint8_t SPI_DFF          ; 
    uint8_t SPI_CPOL        ; 
    uint8_t SPI_CPHA        ; 
    uint8_t SPI_SSM         ; 
}SPI_Config_t;


/*
     Handle structure of SPIx peripheral 
*/

typedef struct 
{
    SPI_RegDef_t   *pSpix     ; /*!<This holds base address of SPIx peripheral >*/
    SPI_Config_t   SPIConfig  ; 
    uint8_t       *pTxBuffer  ; /*!<To store the app. Tx buffer address>*/
    uint8_t       *pRxBuffer  ; /*!<To store the app. Rx buffer address>*/
    uint32_t       TxLen      ; /*!<To store Tx Len  >*/
    uint32_t       RxLen      ; /*!<To store Rx Len  >*/
    uint8_t        TxState    ; /*!<To store Rx Len  >*/
    uint8_t        RxState    ; /*!<To store Rx Len  >*/
}SPI_Handle_t ;



/*****************************************************************************************
 *                                 APIs supported by this driver
 *                    For more info about APIs check the function definition
 * ***************************************************************************************/


void SPI_PeriClockControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi) ;
void SPI_SSIConfig(SPI_RegDef_t  *pSPIx , uint8_t EnorDi         ) ;
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx , uint8_t Enordi         ) ;
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName) ;
void SPIClearOVRFlag(SPI_RegDef_t  *pSPIx)                         ;
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle ) ;
void SPI_CloseReception(SPI_Handle_t *pSPIHandle ) ;


/*
                                     Application Callback
*/

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle , uint8_t AppEv) ;

/*
 *  Init and De-init
 */

void SPI_Init(SPI_Handle_t *pSPIHandle) ;
void SPI_DeInit(SPI_RegDef_t  *pSPIx)   ;


/*
 *  Data send and receive 
 */

 void SPI_SendData(SPI_RegDef_t  *pSPIx    , uint8_t *pTxBuffer , uint32_t Len )    ;
 void SPI_ReceiveData(SPI_RegDef_t  *pSPIx , uint8_t *pRxBuffer , uint32_t Len )    ;


 uint8_t SPI_SendDataIT(SPI_Handle_t  *pSPIHandle    , uint8_t *pTxBuffer , uint32_t Len )    ;
 uint8_t SPI_ReceiveDataIT(SPI_Handle_t  *pSPIHandle , uint8_t *pRxBuffer , uint32_t Len )    ;


 /*
 *  IRQ Configuration and ISR Handling 
 */

void SPI_IRQInterruptConfig(uint8_t IRQNumber ,  uint8_t EnorDi)            ;
void SPI_IRQPriorityConfig(uint8_t IRQNumber , uint8_t IRQPriority)         ;
void SPI_IRQHandling(SPI_Handle_t  *pHandle)                                ;





/********************************************************************************************** 
 *                             Bit Position Definitions of SPI Peripheral
 *                 Used to represent a specific bit position inside a register.
***********************************************************************************************/

/******************************Bit Mask Macros for the SPI Control Register 1*********************/

#define   SPI_CR1_CPHA            0
#define   SPI_CR1_CPOL            1
#define   SPI_CR1_MSTR            2
#define   SPI_CR1_BR              3
#define   SPI_CR1_SPE             6
#define   SPI_CR1_LSB_FIRST       7
#define   SPI_CR1_SSI             8
#define   SPI_CR1_SSM             9
#define   SPI_CR1_RXONLY          10
#define   SPI_CR1_DFF             11
#define   SPI_CR1_CRC_NEXT        12
#define   SPI_CR1_CRC_EN          13
#define   SPI_CR1_BIDI_OE         14
#define   SPI_CR1_BIDI_MODE       15

/******************************Bit Mask Macros for the SPI Control Register 2*********************/

#define   SPI_CR2_RXDMAEN         0
#define   SPI_CR2_TXDMAEN         1
#define   SPI_CR2_SSOE            2
#define   SPI_CR2_FRF             4
#define   SPI_CR2_ERRIE           5
#define   SPI_CR2_RXNEIE          6
#define   SPI_CR2_TXEIE           7


/*********************************Bit Mask Macros for the SPI Status Register*********************/

#define   SPI_SR_RXNE             0
#define   SPI_SR_TXE              1
#define   SPI_SR_CHSIDE           2
#define   SPI_SR_UDR              3
#define   SPI_SR_CRC_ERR          4
#define   SPI_SR_MODF             5
#define   SPI_SR_OVR              6
#define   SPI_SR_BSY              7
#define   SPI_SR_FRE              8


/*
                                         SPI Application States
*/

#define SPI_READY         0
#define SPI_BUSY_IN_RX    1
#define SPI_BUSY_IN_TX    2


/* Possible SPI  Application Events */

#define SPI_TX_EVENT_CMPLT            1
#define SPI_RX_EVENT_CMPLT            2
#define SPI_EVENT_OVR_ERR             3
#define SPI_EVENT_CRC_ERR             4


/*
 *  @SPI_DeviceMode
 */

#define SPI_DEVICE_MODE_MASTER    1
#define SPI_DEVICE_MODE_SLAVE     0




/*
 *  @SPI_SclkSpeed
 */


#define SPI_SCLK_SPEED_DIV2      0
#define SPI_SCLK_SPEED_DIV4      1
#define SPI_SCLK_SPEED_DIV8      2
#define SPI_SCLK_SPEED_DIV16     3
#define SPI_SCLK_SPEED_DIV32     4
#define SPI_SCLK_SPEED_DIV64     5
#define SPI_SCLK_SPEED_DIV128    6
#define SPI_SCLK_SPEED_DIV256    7


/*
 *  @SPI_SclkSpeed
 */



#define SPI_BUS_CONFIG_FULL_DUPLEX        1
#define SPI_BUS_CONFIG_HALF_DUPLEX        2
#define SPI_BUS_CONFIG_SIMPLEX_RX_ONLY    3


/*
 *  @SPI_DFF
 */

#define SPI_DFF8BITS     0
#define SPI_DFF16BITS    1


/*
 *  @SPI_CPOL
 */

#define SPI_CPOL_HIGH    1
#define SPI_CPOL_LOW     0 


/*
 *  @SPI_CPHA
 */

#define SPI_CPHA_HIGH    1
#define SPI_CPHA_LOW     0 

/*
 *  @SPI_SSM
 */

#define SPI_SSM_EN     1
#define SPI_SSM_DI     0 


#define   SPI_TXE_FLAG     ( 1 << SPI_SR_TXE )
#define   SPI_RXNE_FLAG    ( 1 << SPI_SR_RXNE)


#endif /* INC_STM32F411XX_SPI_DRIVER_H_ */
