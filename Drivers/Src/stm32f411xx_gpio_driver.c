/*
 * stm32f411xx_gpio_driver.c
 *
 *  Created on: Jul 9, 2026
 *      Author: madhav-gaur
 */


#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx.h"


/*
 *  Peripheral Clock Setup
 */

/*************************************************************************************************
 * @fn                         -  GPIO_PeriClockControl
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -  It returns nothing because it configures
 *
 * @note                       -
 *
 *
 **************************************************************************************************/


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


/**
 * @brief Initializes the peripheral/module.
 *
 * Configures all required hardware resources, enables clocks,
 * initializes GPIOs and peripheral registers, and prepares the
 * module for operation. This function must be called before
 * using any other functions of the module.
 */

/*************************************************************************************************
 * @fn                         -  GPIO_Init
 *
 * @brief                      -  
 *
 * @param[in]                  -  Address of the created GPIO Handle structure 
 *
 * @return                     -  It returns nothing because it configures
 *
 * @note                       -
 *
 *
 *************************************************************************************************/


void GPIO_Init(GPIO_Handle_t   *pGPIOHandle)
{
    GPIO_PeriClockControl(pGPIOHandle->pGPIOx,ENABLE) ;

    // 1. Configure the mode of the gpio pin

	uint32_t temp = 0 ; // Temporary Register

	/*Check for the error of shift of clearing and setting */

    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode) << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2) ;
        pGPIOHandle->pGPIOx->MODER   &=  ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber*2)               ;
        pGPIOHandle->pGPIOx->MODER   |=   temp                                                                ;
    }else{

        // Interrupt configuration 

        SYSCFG_PCLK_EN() ;

        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT      )
        {
            // 1 . Configure the  FTSR = Falling Trigger Selection Register

            EXTI->FTSR     |=    (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) ;

             // Clearing the corresponding RTSR bit 

		    EXTI->RTSR     &=   ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

        }
        else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT )
        {
             //  1 . Configure the  RTSR = Rising  Trigger Selection Register

		    EXTI->RTSR   |=  (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		    // Clearing the corresponding FTSR bit 

		    EXTI->FTSR   &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
             //  1 . Configure the  both RTSR AND FTSR 

		    EXTI->FTSR   |=  (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		    EXTI->RTSR   |=  (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }

          // 2 . Configure the GPIO port selection in SYSCFG_EXTICR

		  /*
           * The STM32 provides four EXTI configuration registers (EXTICR1–EXTICR4)
           * for selecting the GPIO port source of EXTI lines 0–15. Instead of defining
           * these registers separately, they are represented as an array EXTICR[4].
           *
           * Each EXTICR register contains configuration fields for four EXTI lines,
           * with each field occupying 4 bits. To determine the register corresponding
           * to a given pin number, we divide the pin number by 4. The remainder
           * (pin number % 4) identifies the field within that register. Since each
           * field is 4 bits wide, the bit position is calculated as:
           *
           *      register_index = pinNumber / 4;
           *      bit_position   = (pinNumber % 4) * 4;
           *
           * This approach allows any EXTI line to be configured using a common
           * formula without handling EXTICR1–EXTICR4 individually.
           */

           uint8_t temp1  = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4 ;
		   uint8_t temp2  = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4 ;

		   uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx) ;
           

		   
           SYSCFG->EXTICR[temp1] |= portcode << (temp2*4) ;

            //  3 . Enable the EXTI interrupt delivery using IMR (Interrupt Mask Regsiter)
           
		   EXTI->IMR    |=  (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

    }

    // 2 . Configuring the output type

        temp = 0 ;

        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType) << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)      ;
        pGPIOHandle->pGPIOx->OTYPER        &=         ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)    ;
        pGPIOHandle->pGPIOx->OTYPER        |=  temp                                                           ;    
        
    // 3 . Configuring the output speed

        temp = 0 ;

        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed) << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber*2)  ;
        pGPIOHandle->pGPIOx->OSPEEDR       &=          ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber*2) ;
        pGPIOHandle->pGPIOx->OSPEEDR       |=  temp                                                           ;

    // 4 . Configuring the pull up / pull down 

        temp = 0 ;

        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl) << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber*2) ;
        pGPIOHandle->pGPIOx->PUPDR         &=          ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber*2)      ;
        pGPIOHandle->pGPIOx->PUPDR         |=  temp                                                                ;

    // 5 . Configuring the alternate functionality

       if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
       {
          // Configure the Alternate function Regsiter

    	/*
    	 * Each GPIO pin configured in Alternate Function (AF) mode uses a 4-bit field
    	 * in either the AFRL or AFRH register.
    	 *
    	 * - AFRL (Alternate Function Low Register) configures pins 0–7.
    	 * - AFRH (Alternate Function High Register) configures pins 8–15.
    	 *
    	 * Register selection:
    	 *      pin / 8 == 0  -> Use AFRL
    	 *      pin / 8 == 1  -> Use AFRH
    	 *
    	 * Bit position calculation:
    	 *      field_index = pin % 8
    	 *      bit_position = field_index * 4
    	 *
    	 * Example 1: Pin 6
    	 *      6 / 8 = 0  -> AFRL selected
    	 *      6 % 8 = 6
    	 *      bit_position = 6 * 4 = 24
    	 *      Therefore, the AF field occupies bits [27:24] of AFRL.
    	 *
    	 * Example 2: Pin 8
    	 *      8 / 8 = 1  -> AFRH selected
    	 *      8 % 8 = 0
    	 *      bit_position = 0 * 4 = 0
    	 *      Therefore, the AF field occupies bits [3:0] of AFRH.
    	 *
    	 * In general:
    	 *      Register      = (pin < 8) ? AFRL : AFRH
    	 *      Bit Position  = (pin % 8) * 4
    	 */

         uint32_t temp1 , temp2 ;
         temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)/8 ;
         temp2 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)%8 ;
         pGPIOHandle->pGPIOx->AFR[temp1]   &=    ~(0xF << 4*temp2)            ;
         pGPIOHandle->pGPIOx->AFR[temp1]   |=     (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2)) ;
       }
}

/*******************************************************************************
 * @fn                         -  GPIO_DeInit
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -  It returns nothing because it configures
 *
 * @note                       -
 *
 *
 ********************************************************************************/


 void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
 {
               if(pGPIOx == GPIOA)
		       {
			       GPIOA_REG_RESET();
		       }else if(pGPIOx == GPIOB)
		       {
			       GPIOB_REG_RESET();
		       }else if(pGPIOx == GPIOC)
		       {
			       GPIOC_REG_RESET();
		       }else if(pGPIOx == GPIOD)
		       {
			       GPIOD_REG_RESET();
		       }else if(pGPIOx == GPIOE)
		       {
			       GPIOE_REG_RESET();
		       }else if(pGPIOx == GPIOH)
               {
                   GPIOH_REG_RESET();
               }
 }

/*******************************************************************************
 * @fn                         -  GPIO_DeInit
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -  It returns nothing because it configures
 *
 * @note                       -
 *
 *
 ********************************************************************************/

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber)
{
    
    uint8_t value ;

    value = (uint8_t)((pGPIOx->IDR) >> PinNumber & (0x1)) ;

    return value  ;
}

/**************************************************************************************************
 * @fn                         -  GPIO_DeInit
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -  It returns nothing because it configures
 *
 * @note                       -
 *
 *
 **************************************************************************************************/

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    uint16_t value ;

    value = (uint16_t)(pGPIOx->IDR) ;

    return value ;
}

/*******************************************************************************
 * @fn                         -  GPIO_DeInit
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -  It returns nothing because it configures
 *
 * @note                       -
 *
 *
 ********************************************************************************/

 void GPIO_WriteToOutputPin(GPIO_RegDef_t  *pGPIOx , uint8_t PinNumber,uint8_t Value)
 {
     if(Value==GPIO_PIN_SET)
     {
        // Write 1 to ODR at the bit field corresponding to the pin number
        pGPIOx->ODR     |=   (0x1 << PinNumber) ;
     }else{
        // Write 0 to ODR at the bit field corresponding to the pin number
	    pGPIOx->ODR     &=  ~(1<< PinNumber)    ;
     }
 }

/*******************************************************************************
 * @fn                         -  GPIO_DeInit
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -  It returns nothing because it configures
 *
 * @note                       -
 *
 *
 ********************************************************************************/

void GPIO_WriteToOutputPort(GPIO_RegDef_t  *pGPIOx , uint16_t Value)
{
	pGPIOx->ODR =  Value ;
}

/*******************************************************************************
 * @fn                         -  GPIO_DeInit
 *
 * @brief                      -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]                  -  Base address of the gpio peripheral
 * @param[in]                  -  Enable or Disable macros
 * @param[in]                  -
 *
 * @return                     -  It returns nothing because it configures
 *
 * @note                       -
 *
 *
 ********************************************************************************/

void GPIO_ToggleOutputPin(GPIO_RegDef_t  *pGPIOx , uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber) ;
}

/*
 *  IRQ configuration and IRQ handling
 */

// The NVIC (Nested Vectored Interrupt Controller) manages all interrupt requests in the Cortex-M 
// processor. Each interrupt source is assigned an IRQ number, and the NVIC uses this number to 
// control whether the interrupt is enabled, disabled, pending, or prioritized.

// The ISER (Interrupt Set Enable Registers) are used to enable interrupts. Writing a 1 to the 
// corresponding bit enables the interrupt associated with that IRQ number. Since a microcontroller 
// can support many interrupts, multiple ISER registers are provided, with each register controlling 
// 32 IRQs.

// The ICER (Interrupt Clear Enable Registers) are used to disable interrupts. Writing a 1 to a bit 
// clears the corresponding enable bit and prevents that interrupt from reaching the processor.

// The IPR (Interrupt Priority Registers) are used to assign priorities to interrupts. A lower 
// priority number represents a higher interrupt priority. When multiple interrupts occur 
// simultaneously, the NVIC services the interrupt with the highest priority first. Priority 
// registers allow critical tasks to interrupt less important ones.

// For example, on STM32F4, IRQ numbers 0–31 are controlled by NVIC ISER0/ICER0, IRQ numbers 32–63 
// by ISER1/ICER1, and so on. The NVIC uses these registers together to determine which interrupts 
// are enabled, which are disabled, and the order in which pending interrupts are serviced.



// NVIC Register Selection Using IRQ Number
// Each NVIC ISER, ICER, ISPR, and ICPR register controls 32 interrupt lines.
// To find which NVIC register corresponds to a particular IRQ number, divide the IRQ number by 32.
// To find the bit position of that IRQ within the selected register, take the remainder after division by 32.
// Formula
// register_number = IRQn / 32;
// bit_position    = IRQn % 32;
// Example: IRQ Number 40
// Register Number = 40 / 32 = 1
// Therefore, use ISER1, ICER1, ISPR1, or ICPR1 depending on the operation.
// Bit Position = 40 % 32 = 8
// Therefore, bit 8 in the selected register corresponds to IRQ 40.
// Enabling IRQ 40
// NVIC->ISER[1] = (1 << 8);

// Applications of the Same Calculation
//    ISER → Enable an interrupt.
//    ICER → Disable an interrupt.
//    ISPR → Set an interrupt as pending.
//    ICPR → Clear a pending interrupt.


/*
 * The Cortex-M NVIC provides Interrupt Priority Registers (IPR) to assign
 * priorities to interrupts. In STM32 devices based on Cortex-M4, there are
 * 60 IPR registers. Each IPR register is 32 bits wide and is divided into
 * four 8-bit priority fields. Therefore, a single IPR register can store
 * the priority information of four different interrupts.
 *
 * To locate the priority field corresponding to a particular IRQ number:
 *
 *      iprx      = IRQNumber / 4;
 *      section   = IRQNumber % 4;
 *
 * Since each priority field occupies 8 bits, the bit position of the field
 * within the register is:
 *
 *      shift_amount = section * 8;
 *
 * Note that although each field is 8 bits wide, not all bits are implemented.
 * For example, in STM32F4 only the upper 4 bits of each priority field are
 * used to represent the interrupt priority, while the lower bits are reserved.
 *
 * This arrangement allows the NVIC to efficiently store and manage the
 * priorities of all external interrupts.
 * 
 * iprx         = IRQNumber / 4;
 * section      = IRQNumber % 4;
 * shift_amount = (IRQNumber % 4) * 8;
 */

void GPIO_IRQInterruptConfig(uint8_t IRQNumber ,  uint8_t EnorDi)
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
            printf("%d",IRQNumber) ;

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

/*
 *  Interrupt Priority Handling 
 */

/*******************************************************************************
 * @fn                         -  GPIO_PeriClockControl
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
 *
 ********************************************************************************/

void GPIO_IRQPriorityConfig(uint8_t IRQNumber , uint8_t IRQPriority)
{
    // 1 . Find out the ipr register 

	uint8_t iprx         = IRQNumber/4     ;
	uint8_t iprx_section = IRQNumber%4     ;

	uint8_t shift_amount =  (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED) ;

	*(NVIC_IPR_BASE_ADDR + (iprx))  |=  (IRQPriority << shift_amount)   ;
}

/*
 * Interrupt Handling Flow in STM32 Cortex-M
 *
 * Suppose a button connected to PA0 generates an interrupt through EXTI0.
 *
 * 1. The button press causes a rising or falling edge on PA0.
 *
 * 2. The EXTI peripheral detects the configured edge and sets the
 *    corresponding pending bit in the EXTI Pending Register (EXTI_PR).
 *
 * 3. If the EXTI line is unmasked and enabled, EXTI sends an interrupt
 *    request to the NVIC through the corresponding IRQ channel
 *    (EXTI0 -> IRQ number 6 in STM32F4).
 *
 * 4. The NVIC receives the interrupt request and sets its own pending
 *    status internally. Therefore both EXTI and NVIC maintain pending
 *    information.
 *
 * 5. The processor checks whether:
 *      - interrupts are globally enabled,
 *      - the interrupt is enabled in NVIC,
 *      - no higher priority interrupt is currently executing.
 *
 * 6. If accepted, the Cortex-M hardware automatically saves the current
 *    CPU context (R0-R3, R12, LR, PC, xPSR) onto the stack.
 *
 * 7. The processor then uses the IRQ number to index into the Vector Table.
 *    The Vector Table contains addresses of ISR functions.
 *
 * 8. The address stored in the EXTI0 vector entry is loaded into the PC,
 *    causing execution of EXTI0_IRQHandler().
 *
 * 9. Inside the ISR, the required interrupt handling code is executed.
 *
 * 10. The EXTI pending flag must be cleared by software. If EXTI_PR is not
 *     cleared, the interrupt request remains pending and the ISR may be
 *     entered repeatedly.
 *
 * 11. When the ISR returns, the processor automatically restores the saved
 *     context from the stack and resumes execution of the interrupted code.
 */

void GPIO_IRQHandling(uint8_t PinNumber)                                    
{
    // Clearing the EXTI PR (Pending Register)

	if(EXTI->PR & (1<<PinNumber))
	{
		// Clear
		EXTI->PR   |= (1<<PinNumber) ;
	}
}
