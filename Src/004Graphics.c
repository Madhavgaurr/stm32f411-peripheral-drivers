/*
 * 004Graphics.c
 *
 *  Created on: Sep 20, 2026
 *      Author: madhav-gaur
 */

#include  "stm32f411xx.h"
#include "math.h"


/**
 * @brief TFT LCD pin configuration macros.
 *
 * These macros define the GPIO port and pin connections used for
 * interfacing the TFT LCD module with the microcontroller.
 *
 * The configuration includes:
 * - Chip Select (CS): Used to select the TFT LCD device on the SPI bus.
 * - Data/Command (DC): Selects between sending commands and display data.
 * - Reset (RST): Used to reset the LCD controller during initialization.
 *
 * Modify these definitions according to the hardware connection
 * between the MCU and TFT LCD module.
 */


/*
  LCD_PORT → GPIO port used by the display control pins.
  LCD_RST_PIN → Reset pin; resets the display hardware.
  LCD_CS_PIN → Chip Select pin; enables or disables communication with the display.
  LCD_DC_PIN → Data/Command pin; selects whether the transmitted byte is a command or display data.
*/

#define    LCD_PORT      GPIOB
#define    LCD_RST_PIN   5
#define    LCD_CS_PIN    6
#define    LCD_DC_PIN    7

/*
  TFT_Select() → Selects or enables the TFT display (usually by pulling the CS pin low).
  TFT_Unselect() → Deselects or disables the TFT display.
  TFT_CommandMode() → Switches the display to command mode (DC pin low).
  TFT_DataMode() → Switches the display to data mode (DC pin high).
*/
void TFT_Select(void)  ;
void TFT_Unselect(void);
void TFT_CommandMode(void);
void TFT_DataMode(void);





void Spi2GpioInits(void)
{
	GPIO_Handle_t  SPI_PINS , CONFIG_PINS ;

	/* Configuration of MOSI(Master Out Slave In) and SCLK(Serial Clock) */

	/*When configuring a GPIO pin for an alternate function such as SPI, I²C, UART, etc., first check the MCU datasheet/reference manual for
	  the correct Alternate Function (AF) mapping for that pin, then configure the corresponding AF number in the appropriate AFRL or AFRH
	  register.*/

	SPI_PINS.pGPIOx = GPIOB ;
	SPI_PINS.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN ;
	SPI_PINS.GPIO_PinConfig.GPIO_PinAltFunMode  = 5               ;
    SPI_PINS.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_PP ;
    SPI_PINS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD    ;
    SPI_PINS.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST ;

    SPI_PINS.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_13   ;
    GPIO_Init(&SPI_PINS) ;   // SCLK

    SPI_PINS.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_15   ;
    GPIO_Init(&SPI_PINS) ;   // MOSI

    // RST, CS, DC -> plain GPIO output

    CONFIG_PINS.pGPIOx = GPIOB;
    CONFIG_PINS.GPIO_PinConfig.GPIO_PinMode      = GPIO_MODE_OUTPUT ;
    CONFIG_PINS.GPIO_PinConfig.GPIO_PinOPType    = GPIO_OP_TYPE_PP  ;
    CONFIG_PINS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD   ;
    CONFIG_PINS.GPIO_PinConfig.GPIO_PinSpeed     = GPIO_SPEED_FAST  ;

    CONFIG_PINS.GPIO_PinConfig.GPIO_PinNumber = LCD_RST_PIN         ;
    GPIO_Init(&CONFIG_PINS);
    CONFIG_PINS.GPIO_PinConfig.GPIO_PinNumber = LCD_CS_PIN;
    GPIO_Init(&CONFIG_PINS);
    CONFIG_PINS.GPIO_PinConfig.GPIO_PinNumber = LCD_DC_PIN;
    GPIO_Init(&CONFIG_PINS);

    // Idle CS high (deselected) until we actually talk to the display
    GPIO_WriteToOutputPin(LCD_PORT, LCD_CS_PIN, GPIO_PIN_SET);

}

void Spi2Inits(void)
{
	    SPI_Handle_t Spi2Handle;

	    Spi2Handle.pSpix = SPI2;
	    Spi2Handle.SPIConfig.SPI_BusConfig  = SPI_BUS_CONFIG_FULL_DUPLEX;
	    Spi2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	    Spi2Handle.SPIConfig.SPI_SclkSpeed  = SPI_SCLK_SPEED_DIV2; // slower for bring-up
	    Spi2Handle.SPIConfig.SPI_DFF        = SPI_DFF8BITS;
	    Spi2Handle.SPIConfig.SPI_CPOL       = SPI_CPOL_LOW;   // FIX: was HIGH — ST7789 wants Mode 0
	    Spi2Handle.SPIConfig.SPI_CPHA       = SPI_CPHA_LOW;
	    Spi2Handle.SPIConfig.SPI_SSM        = SPI_SSM_EN;

	    SPI_PeriClockControl(SPI2, ENABLE);   // clock ON first, before touching CR1
	    SPI_Init(&Spi2Handle);
	    SPI_SSIConfig(SPI2, ENABLE);          // internal NSS high, avoids MODF error
	    SPI2->CR1 |= (1 << SPI_CR1_SPE);      // enable peripheral
}


/*************************************LCD command/data helpers***********************************/

    // CS = Chip Select, active LOW.
    // Pulling this low tells the ST7789 "you are now the target of the
    // upcoming SPI transaction." The display ignores everything on
    // SCK/MOSI/DC while CS is high. Must go low BEFORE sending any
    // command or data byte, and stay low for the full transaction.
void TFT_Select(void)      {GPIO_WriteToOutputPin(GPIOB,LCD_CS_PIN,GPIO_PIN_RESET);} // Chip Select

    // Releases the display from the bus once the transaction is done.
    // Required if other SPI devices share this bus (e.g. flash chip
    // later) — only one device's CS should be low at a time.
void TFT_Unselect(void)    {GPIO_WriteToOutputPin(GPIOB,LCD_CS_PIN,GPIO_PIN_SET)  ;} // Chip Unselect

    // DC = Data/Command select.
    // LOW  = the next byte shifted over MOSI is a COMMAND (register
    // address), e.g. 0x2A = CASET, 0x29 = Display ON.
    // This only matters while CS is low — DC has no effect otherwise.
void TFT_CommandMode(void) {GPIO_WriteToOutputPin(GPIOB,LCD_DC_PIN,GPIO_PIN_RESET);} // DC = 0

    // DC HIGH = the next byte(s) are DATA — either a parameter for the
    // command just sent (e.g. MADCTL's orientation byte), or raw pixel
    // color values during a RAMWR pixel-write session.
void TFT_DataMode(void)    {GPIO_WriteToOutputPin(GPIOB,LCD_DC_PIN,GPIO_PIN_SET)  ;} // DC = 1


void lcd_write_cmd(uint8_t cmd)
{
         TFT_Select()              ;
         TFT_CommandMode()         ;
         SPI_SendData(SPI2,&cmd,1) ;
         TFT_Unselect()            ;
}

void lcd_write_data(uint8_t data)
{
         TFT_Select()              ;
         TFT_DataMode()            ;
         SPI_SendData(SPI2,&data,1) ;
         TFT_Unselect()            ;
}


/*

*/
void lcd_reset(void)
{
    GPIO_WriteToOutputPin(LCD_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_PORT, LCD_RST_PIN, GPIO_PIN_SET);
}


void lcd_init(void)
{
	lcd_reset() ;

	lcd_write_cmd(0x01); // 0x01 SWRESET Software Reset (resets the display controller)

	lcd_write_cmd(0x11); // 0x11 SLPOUT  Sleep Out (exits sleep mode)

	lcd_write_cmd(0x3A); // 0x3A COLMOD  Interface Pixel Format (e.g., 0x55 for 16-bit/pixel RGB565)

    lcd_write_data(0x55);               // 16-bit RGB565

	//lcd_write_data(0x06);                  // 18-bit RGB666

	lcd_write_cmd(0x36);
	// lcd_write_data(0x00);                  // MADCTL, plain orientation for now


	lcd_write_cmd(0x29);                   // 0x29 DISPON Display On


}


void lcd_set_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
	lcd_write_cmd(0x2A);          // 0x2A   CASET  Column Address Set (defines X start/end)
	lcd_write_data(xs >> 8);
	lcd_write_data(xs & 0xFF);
	lcd_write_data(xe >> 8);
	lcd_write_data(xe & 0xFF);

	lcd_write_cmd(0x2B);        // Row address set
	lcd_write_data(ys >> 8);
	lcd_write_data(ys & 0xFF);
	lcd_write_data(ye >> 8);
	lcd_write_data(ye & 0xFF);

	lcd_write_cmd(0x2C);        // Memory write
}





void lcd_fill_rect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color)
{
    uint32_t i;
    uint8_t data[2];

    data[0]=color>>8;
    data[1]=color;

    lcd_set_window(x,y,x+w-1,y+h-1);

    GPIO_WriteToOutputPin(LCD_PORT,LCD_DC_PIN,GPIO_PIN_SET);
    TFT_Select()   ;

    for(i=0;i<(uint32_t)w*h;i++)
        SPI_SendData(SPI2,data,2);

    TFT_Unselect() ;
}


void lcd_fill_screen(uint16_t color)
{
	    uint32_t i;
	    uint8_t data[2];

	    data[0]=color>>8;
	    data[1]=color;

	    lcd_set_window(0,0,240,320);

	    GPIO_WriteToOutputPin(LCD_PORT,LCD_DC_PIN,GPIO_PIN_SET);
	    TFT_Select()   ;

	    for(i=0;i<(uint32_t)240*320;i++)
	        SPI_SendData(SPI2,data,2);

	    TFT_Unselect() ;
}

void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{


	    uint8_t data[2];

	    data[0]=color>>8;
	    data[1]=color;

	    lcd_set_window(x,y,x,y);

	    GPIO_WriteToOutputPin(LCD_PORT,LCD_DC_PIN,GPIO_PIN_SET);
	    TFT_Select()   ;
	    SPI_SendData(SPI2,data,2);


}









int main()
{
	        Spi2GpioInits();

	        Spi2Inits();

	        lcd_init() ;
	        lcd_fill_rect(0,0,240,320,0xffc0) ;


}
