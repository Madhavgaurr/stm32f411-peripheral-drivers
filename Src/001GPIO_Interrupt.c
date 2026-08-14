#include "stm32f411xx.h"




int main()
{
    GPIO_Handle_t GpioLed  = {0}   ;
    GPIO_Handle_t GpioButton = {0} ;




    SYSCFG_PCLK_EN () ;


	GpioLed.pGPIOx = GPIOA;






	GpioLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_12    ;
	GpioLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUTPUT  ;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST   ;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_PP   ;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD      ;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GpioLed);

    // Button Configuration

    GpioButton.pGPIOx = GPIOC ;
    GpioButton.GPIO_PinConfig.GPIO_PinNumber   = GPIO_PIN_NO_15     ;
    GpioButton.GPIO_PinConfig.GPIO_PinMode     = GPIO_MODE_IT_FT    ;
    GpioButton.GPIO_PinConfig.GPIO_PinSpeed    = GPIO_SPEED_FAST    ;
    GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD     ;



    GPIO_PeriClockControl(GPIOC, ENABLE);
	GPIO_Init(&GpioButton);


    // LED Working Well
	GPIO_WriteToOutputPin(GPIOA,GPIO_PIN_NO_12, DISABLE) ;




    // IRQ Configuration

   GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10 ,NVIC_IRQ_PRIO15) ;
   GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10,ENABLE)          ;


   while(1)
   {

	   if(!(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_15) & 1))
	   {
		   GPIO_WriteToOutputPin(GPIOA,GPIO_PIN_NO_12, DISABLE) ;
	   }
   }




    while(1) ;


}


//void EXTI9_5_IRQHandler(void)
//{
//    GPIO_IRQHandling(GPIO_PIN_NO_5)            ;
//    GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12) ;
//}

void EXTI15_10_IRQHandler(void)
{
     GPIO_IRQHandling(GPIO_PIN_NO_15)            ;
     GPIO_WriteToOutputPin(GPIOA,GPIO_PIN_NO_12, ENABLE) ;

}



/*
                                    Debugging the Interrupt
                                    pb5 , pb12

*/
