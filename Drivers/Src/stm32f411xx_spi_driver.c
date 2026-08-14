/*
 * stm32f411xx_spi_driver.c
 *
 *  Created on: Jul 17, 2026
 *      Author: madhav-gaur
 */

#include "stm32f411xx_spi_driver.h"

static void     spi_txe_interrupt_handle(SPI_Handle_t  *pHandle) ;
static void    spi_rxne_interrupt_handle(SPI_Handle_t  *pHandle) ;
static void spi_ovr_err_interrupt_handle(SPI_Handle_t  *pHandle) ;

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi)
{
    if(EnorDi==ENABLE)
    {
        if(pSPIx==SPI1)
        {
            SPI1_PCLK_EN() ;
        }else if(pSPIx==SPI2)
        {
            SPI2_PCLK_EN() ;
        }else if(pSPIx==SPI3)
        {
            SPI3_PCLK_EN() ;
        }else if(pSPIx==SPI4)
        {
            SPI4_PCLK_EN() ;
        }else if(pSPIx==SPI5)
        {
            SPI5_PCLK_EN() ;
        }
    }else{
         if(pSPIx==SPI1)
        {
            SPI1_PCLK_DI() ;
        }else if(pSPIx==SPI2)
        {
            SPI2_PCLK_DI() ;
        }else if(pSPIx==SPI3)
        {
            SPI3_PCLK_DI() ;
        }else if(pSPIx==SPI4)
        {
            SPI4_PCLK_DI() ;
        }else if(pSPIx==SPI5)
        {
            SPI5_PCLK_DI() ;
        }
    }
}


void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    SPI_PeriClockControl(pSPIHandle->pSpix,ENABLE) ;
    
    // Configuring the SPI_CR1 Register

	pSPIHandle->pSpix->CR1     &= ~(7<<SPI_CR1_BR) ;

    // 1 . . Configuring the Mode of the Device 

    uint32_t tempreg       = 0 ;

    tempreg |=   pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR ;

	// 2 . Configuring the Bus Config

	if(pSPIHandle->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_FULL_DUPLEX)
	{
		// BIDI mode should be cleared
		tempreg     &= ~(1<<15) ;
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_HALF_DUPLEX)
	{
		// BIDI mode should be set
		tempreg     |=  (1<<15) ;
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_SIMPLEX_RX_ONLY)
	{
		// BIDI mode should be cleared
		tempreg     &= ~(1<<15) ;

		// RX_ONLY bit must be set

		tempreg     |=  (1<<10) ;
	}


	// 3 . Configuring the SPI serial clock (baud rate) .

	

	tempreg         |=   pSPIHandle->SPIConfig.SPI_SclkSpeed <<  SPI_CR1_BR   ;

	// 4 . Configuring the DFF (Data frame format) .

	tempreg         |=   pSPIHandle->SPIConfig.SPI_DFF       <<  SPI_CR1_DFF  ;

	// 5 . Configuring the CPOL (Clock Polarity)

	tempreg         |=   pSPIHandle->SPIConfig.SPI_CPOL      <<  SPI_CR1_CPOL ;

	// 6 . Configuring the CPHA (Clock Phase)

	tempreg         |=   pSPIHandle->SPIConfig.SPI_CPHA      <<  SPI_CR1_CPHA ;


	// 7 . Configuring the SSM (Software Slave Management)

	tempreg         |=   pSPIHandle->SPIConfig.SPI_SSM       <<  SPI_CR1_SSM  ;




	pSPIHandle->pSpix->CR1    |= tempreg    ;


}


/*
 *  SPI DeInit Setup
 */

/*******************************************************************************
 * @fn                         -  SPI_DeInit
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -
 *
 * @note                       -
 *
 * Status Registers are house for status flag , they flag different events by either providing 0
 * or 1 over the repected bit 
 * Error Event
 * Bus Idle Event
 * Etc 
 ********************************************************************************/


void SPI_DeInit(SPI_RegDef_t  *pSPIx)
{
}



/*
 *  SPI SendData Setup
 */

/*******************************************************************************
 * @fn                         -  SPI_SendData
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -
 *
 * @note                       -
 *
 * SPI_SendData()
 *
 * Blocking SPI transmit API.
 * The function returns only after all bytes have been loaded into the Tx buffer.
 *
 * Parameters:
 *  pTxBuffer : Pointer to transmit data.
 *  Len       : Number of bytes to transmit.
 *
 * Flow:
 *  1. Repeat until Len becomes 0.
 *  2. Wait until TXE (Tx buffer empty) flag is set.
 *  3. Write data to the SPI_DR register.
 *     - DFF = 0 : Transmit 8 bits (1 byte).
 *     - DFF = 1 : Transmit 16 bits (2 bytes).
 *  4. Update buffer pointer and remaining length.
 *
 * Note:
 *  Writing to SPI_DR loads the Tx buffer.
 *  Reading SPI_DR returns data from the Rx buffer.
 ********************************************************************************/


uint8_t   SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName)
{           
	        if(pSPIx->SR  & FlagName)
			{
				return FLAG_SET ;
			}
            return FLAG_RESET ;
}

void SPI_SendData(SPI_RegDef_t  *pSPIx    , uint8_t *pTxBuffer , uint32_t Len)
{
	while(Len!=0)
	{   
		// 1 . Wait until TXE flag is set    , need watchdogs in case of failure 
		//while(!(pSPIx->SR  & (1 << SPI_SR_TXE))) ;

		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET );

		// 2 . 

		if(pSPIx->CR1 & (1<<SPI_CR1_DFF))
		{
            // 16 bit DFF
			pSPIx->DR   = *((uint16_t*)pTxBuffer) ;
			Len -=    2                           ;
			(uint16_t*)pTxBuffer++                ;
		}else{
			// 8  bit DFF 

			pSPIx->DR   = *(pTxBuffer) ;
			Len -=    1                ;
			pTxBuffer++                ;
		}
	}
}

/*
 *  SPI ReceiveData Setup
 */

/*******************************************************************************
 * @fn                         -  SPI_SendData
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -
 *
 * @note                       -
 *
 * SPI_SendData()
 *
 * Blocking SPI transmit API.
 * The function returns only after all bytes have been loaded into the Tx buffer.
 *
 * Parameters:
 *  pTxBuffer : Pointer to transmit data.
 *  Len       : Number of bytes to transmit.
 *
 * Flow:
 *  1. Repeat until Len becomes 0.
 *  2. Wait until TXE (Tx buffer empty) flag is set.
 *  3. Write data to the SPI_DR register.
 *     - DFF = 0 : Transmit 8 bits (1 byte).
 *     - DFF = 1 : Transmit 16 bits (2 bytes).
 *  4. Update buffer pointer and remaining length.
 *
 * Note:
 *  Writing to SPI_DR loads the Tx buffer.
 *  Reading SPI_DR returns data from the Rx buffer.
 ********************************************************************************/

void SPI_ReceiveData(SPI_RegDef_t  *pSPIx    , uint8_t *pRxBuffer , uint32_t Len)
{
	while(Len!=0)
	{   
		// 1 . Wait until RXNE flag is set    , need watchdogs in case of failure 
		//while(!(pSPIx->SR  & (1 << SPI_SR_TXE))) ;

		while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG) == FLAG_RESET );

		// 2 . 

		if(pSPIx->CR1 & (1<<SPI_CR1_DFF))
		{
            // 16 bit DFF
			*((uint16_t*)pRxBuffer) = pSPIx->DR ;
			Len -=    2                           ;
			(uint16_t*)pRxBuffer++                ;
		}else{
			// 8  bit DFF 

			*(pRxBuffer) = pSPIx->DR  ;
			Len -=    1                ;
			pRxBuffer++                ;
		}
	}
}


void SPI_PeripheralControl(SPI_RegDef_t *pSPIx , uint8_t Enordi)
{   

	if(Enordi == ENABLE)
	{
		pSPIx->CR1   |=   (1<<SPI_CR1_SPE) ;
	}else{
		pSPIx->CR1   &=  ~(1<<SPI_CR1_SPE) ;
	}
     
}

void SPI_SSIConfig(SPI_RegDef_t  *pSPIx , uint8_t EnorDi  )
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1   |=   (1<<SPI_CR1_SSI) ;
	}else{
		pSPIx->CR1   &=  ~(1<<SPI_CR1_SSI) ;
	}
}

/*
                                         SPI Interrupts 
*/

void SPI_IRQInterruptConfig(uint8_t IRQNumber ,  uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
        if(IRQNumber<=31)
		{

			// Program ISER0 Register

			NVIC_ISER0  |= ( 1 << IRQNumber ) ;

		}else if( IRQNumber>31  && IRQNumber<64 )
		{

			// Program ISER1 Register

			NVIC_ISER1  |= ( 1 << (IRQNumber%32) ) ;

		}else if( IRQNumber>=64 && IRQNumber<96 ){

			// Program ISER2 Register

            NVIC_ISER2  |= ( 1 << (IRQNumber%64) ) ;
		}
	}else{
        if(IRQNumber<=31)
		{

        	// Program ICER0 Register

			NVIC_ICER0  |= ( 1 << IRQNumber ) ;

		}else if( IRQNumber>31  && IRQNumber<64 )
		{

			// Program ICER1 Register

			NVIC_ICER1  |= ( 1 << ( IRQNumber % 32 ) ) ;

		}else if( IRQNumber>=64 && IRQNumber<96 ){

			// Program ICER2 Register

			NVIC_ICER2  |= ( 1 << ( IRQNumber % 64 ) ) ;

		}
	}
}

void SPI_IRQPriorityConfig(uint8_t IRQNumber , uint8_t IRQPriority)
{
	// 1 . Find out the ipr register 

	uint8_t iprx         = IRQNumber/4     ;
	uint8_t iprx_section = IRQNumber%4     ;

	uint8_t shift_amount =  (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED) ;

	*(NVIC_IPR_BASE_ADDR + (iprx))  |=  (IRQPriority << shift_amount)   ;
}

/*
                             # SPI Interrupt-Based Transmission Flow
							           Non-blocking API 


1. Save the **transmit buffer address** and **transmit buffer length** in global (or driver handle)
 variables so that the Interrupt Service Routine (ISR) can access them during transmission.

2. Mark the SPI peripheral state as **Busy in Transmission (SPI_BUSY_IN_TX)**. This prevents any 
other code from taking control of the same SPI peripheral until the current transmission is 
complete.

3. Enable the **TXEIE (Transmit Buffer Empty Interrupt Enable)** bit in the SPI Control Register 
(CR2). Once enabled, whenever the **TXE** flag is set in the SPI Status Register (SR), the SPI 
peripheral generates an interrupt request.

4. The actual data transmission is performed inside the **SPI Interrupt Service Routine (ISR)**. 
Each time the TXE interrupt occurs, the ISR loads the next data into the SPI Data Register (DR).
 This process continues until all data has been transmitted, after which the ISR disables the TXE
  interrupt and marks the SPI peripheral as ready.

*/


uint8_t SPI_SendDataIT(SPI_Handle_t  *pSPIHandle    , uint8_t *pTxBuffer , uint32_t Len )
{   
	uint8_t state = pSPIHandle->TxState ;

	if(state!=SPI_BUSY_IN_TX)
	{
		// 1.

	pSPIHandle->pTxBuffer  = pTxBuffer      ;
	pSPIHandle->TxLen      = Len            ;

	// 2.

	pSPIHandle->TxState    = SPI_BUSY_IN_TX ;

	// 3.

	pSPIHandle->pSpix->CR2  |=  ( 1 << SPI_CR2_TXEIE ) ;
	}

	return state ;
	
}

uint8_t SPI_ReceiveDataIT(SPI_Handle_t  *pSPIHandle , uint8_t *pRxBuffer , uint32_t Len )
{
	uint8_t state = pSPIHandle->RxState ;

	if(state!=SPI_BUSY_IN_RX)
	{
	
	// 1.

	pSPIHandle->pRxBuffer  = pRxBuffer      ;
	pSPIHandle->RxLen      = Len            ;

	// 2.

	pSPIHandle->RxState    = SPI_BUSY_IN_RX ;

	// 3.

	pSPIHandle->pSpix->CR2  |=  ( 1 << SPI_CR2_RXNEIE ) ;
	}

	return state ;
}

/***************************************************************************************************
 *                      @brief   Interrupt-driven SPI transmission 
 *
 * SPI can generate interrupt requests for three categories of events:
 *
 *  1. TXE  (Transmit Buffer Empty)
 *     --------------------------------
 *     Event Flag        : TXE
 *     Enable Bit        : TXEIE
 *
 *     Indicates that the transmit buffer (DR) is empty and ready to accept
 *     the next data frame. Whenever TXE becomes SET and TXE interrupt is
 *     enabled, the SPI peripheral requests an interrupt.
 *
 *
 *  2. RXNE (Receive Buffer Not Empty)
 *     --------------------------------
 *     Event Flag        : RXNE
 *     Enable Bit        : RXNEIE
 *
 *     Indicates that a new data frame has been received and is waiting in
 *     the receive buffer. Software must read the DR register before the next
 *     frame arrives to prevent an overrun condition.
 *
 *
 *  3. Error Events
 *     --------------------------------
 *     Enable Bit        : ERRIE
 *
 *     The following error conditions share the same interrupt source:
 *
 *         MODF    - Mode fault
 *         OVR     - Overrun error
 *         CRCERR  - CRC calculation error
 *         FRE     - TI frame format error
 *
 *     Whenever ERRIE is enabled and any of the above flags becomes SET,
 *     the SPI peripheral generates an interrupt and software must determine
 *     which error occurred by examining the SPI status register (SR).
 *
 *
 * -----------------------------------------------------------------------------------------------
 *                                     Generic SPI Interrupt Flow
 * -----------------------------------------------------------------------------------------------
 *
 *                                    +----------------------+
 *                                    |   Enter SPI ISR      |
 *                                    +----------+-----------+
 *                                               |
 *                                               v
 *                                  Read SPI Status Register (SR)
 *                                               |
 *                           +-------------------+--------------------+
 *                           |                   |                    |
 *                           v                   v                    v
 *                      RXNE set ?          TXE set ?         Error Flag set ?
 *                           |                   |                    |
 *                           v                   v                    v
 *                      Receive Handler      Transmit Handler     Error Handler
 *
 * The ISR always begins by determining which event generated the interrupt.
 * Since multiple interrupt sources share the same IRQ line, software must
 * inspect the corresponding status flags before servicing the event.
 *
 *
 * -----------------------------------------------------------------------------------------------
 *                                   TXE Interrupt Processing
 * -----------------------------------------------------------------------------------------------
 *
 * TXE interrupt is responsible for continuously feeding the SPI Data Register during an interrupt
 * -driven transmission.
 *
 * Every time TXE becomes SET:
 *
 *      1. Determine frame size (8-bit or 16-bit).
 *
 *      2. Load one data frame into SPI->DR.
 *
 *            8-bit mode  -> write one byte
 *            16-bit mode -> write two bytes
 *
 *      3. Advance transmit buffer pointer.
 *
 *      4. Decrease remaining transmission length.
 *
 *      5. Check if all data has been transmitted.
 *
 *             Remaining Length == 0 ?
 *
 *                 YES
 *                     • Disable TXE interrupt.
 *                     • Mark SPI state as READY.
 *                     • Release application resources.
 *                     • Transmission completed.
 *
 *                 NO
 *                     • Exit ISR.
 *                     • Wait for next TXE interrupt.
 *                     • Repeat until all bytes are transmitted.
 *
 *
 * -----------------------------------------------------------------------------------------------
 *                                   Important Design Principle
 * -----------------------------------------------------------------------------------------------
 *
 * The CPU never waits for the SPI hardware.
 *
 * Instead:
 *
 *      TXE interrupt  ---> Hardware requests next byte
 *      ISR            ---> Loads one frame into DR
 *      Return         ---> CPU continues executing other tasks
 *      TXE again      ---> ISR loads next frame
 *
 * This continues until the entire transmit buffer has been transferred,
 * providing an efficient non-blocking SPI transmission mechanism.
 *
 **************************************************************************************************/



void SPI_IRQHandling(SPI_Handle_t  *pSPIHandle)
{
	uint8_t temp1 , temp2 ;

	temp1   =  pSPIHandle->pSpix->SR  & ( 1 << SPI_SR_TXE )   ;
	temp2   =  pSPIHandle->pSpix->CR2 & ( 1 << SPI_CR2_TXEIE) ;

	if( temp1  & temp2 )
	{
		spi_txe_interrupt_handle(pSPIHandle) ;
	}

	temp1   =  pSPIHandle->pSpix->SR  & ( 1 << SPI_SR_RXNE )   ;
	temp2   =  pSPIHandle->pSpix->CR2 & ( 1 << SPI_CR2_RXNEIE) ;


	if( temp1 & temp2 )
	{
		spi_rxne_interrupt_handle(pSPIHandle) ;
	}

	temp1   =  pSPIHandle->pSpix->SR  & ( 1 << SPI_SR_OVR )   ;
	temp2   =  pSPIHandle->pSpix->CR2 & ( 1 << SPI_CR2_ERRIE) ;

	if( temp1 & temp2 )
	{
		spi_ovr_err_interrupt_handle(pSPIHandle) ;
	}

}



static void     spi_txe_interrupt_handle(SPI_Handle_t  *pSPIHandle)
{
	
		if(pSPIHandle->pSpix->CR1 & (1<<SPI_CR1_DFF))
		{
            // 16 bit DFF
			pSPIHandle->pSpix->DR   = *((uint16_t*)pSPIHandle->pTxBuffer) ;
			pSPIHandle->TxLen -=    2                                  ;
			(uint16_t*)pSPIHandle->pTxBuffer++                         ;
		}else{
			// 8  bit DFF 

			pSPIHandle->pSpix->DR   = *(pSPIHandle->pTxBuffer) ;
			pSPIHandle->TxLen -=    1                       ;
			pSPIHandle->pTxBuffer++                         ;
		} 

		if(!(pSPIHandle->TxLen))
		{
			/*
             * Transmission is complete.
             * All bytes have been transferred (TxLen == 0), so close the SPI
             * transmission, restore the peripheral to the ready state, and
             * notify the application that the transmit operation has finished.
			 * This prevents interrupts from setting up of TXE flag
             */

			 SPI_CloseTransmission(pSPIHandle) ;
			 SPI_ApplicationEventCallback( pSPIHandle , SPI_TX_EVENT_CMPLT) ;
		}
}

static void    spi_rxne_interrupt_handle(SPI_Handle_t  *pSPIHandle)
{
	if(pSPIHandle->pSpix->CR1  & ( 1 << SPI_CR1_DFF ))
	{
            *((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)(pSPIHandle->pSpix->DR) ;
			pSPIHandle->RxLen -=2                                                   ;
			(uint16_t*)pSPIHandle->pRxBuffer--                                      ;

	}else{
		    *(pSPIHandle->pRxBuffer) = (uint8_t)(pSPIHandle->pSpix->DR)            ;
			pSPIHandle->RxLen -=  1                                                ;
			pSPIHandle->pRxBuffer--                                                ;

	}

	if(!(pSPIHandle->RxLen))
	{
		 SPI_CloseReception(pSPIHandle)                                 ;
		 SPI_ApplicationEventCallback( pSPIHandle , SPI_RX_EVENT_CMPLT) ;
	}
}

static void spi_ovr_err_interrupt_handle(SPI_Handle_t  *pSPIHandle)
{   
    uint8_t temp ;

	// 1. Clear the OVR flag 

	if(pSPIHandle->TxState !=SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSpix->DR ;
		temp = pSPIHandle->pSpix->SR ;
	}

	(void)temp ;

	// 2. Inform the application 

	 SPI_ApplicationEventCallback( pSPIHandle , SPI_EVENT_OVR_ERR) ;

	
}


void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle )
{
	         pSPIHandle->pSpix->CR2    &=   ~( 1 << SPI_CR2_TXEIE )         ;
			 pSPIHandle->pTxBuffer =  NULL                                  ;
			 pSPIHandle->TxLen     =  0                                     ;
			 pSPIHandle->TxState   =  SPI_READY                             ;
}


void SPI_CloseReception(SPI_Handle_t *pSPIHandle )
{
	         pSPIHandle->pSpix->CR2      &=    ~( 1 << SPI_CR2_RXNEIE )     ;
		     pSPIHandle->pRxBuffer =  NULL                                  ;
		     pSPIHandle->RxLen     =  0                                     ;
		     pSPIHandle->RxState   =  SPI_READY                             ;
}


void SPIClearOVRFlag(SPI_RegDef_t  *pSPIx)
{
	uint8_t temp      ;
	temp = pSPIx->DR  ;
	temp = pSPIx->SR  ;
	(void)temp        ; // I intentionally don't care about the value of this expression.
}

__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle , uint8_t AppEv)
{
      // Weak implementation the application will override this function 
}




