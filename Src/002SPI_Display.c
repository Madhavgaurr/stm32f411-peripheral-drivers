/*
 *                                         5tft_lcd.c
 *
 *                                 Created on: Jul 7, 2026
 *                                     Author: madhav-gaur
 */


/*
  #include "stm32f411xx.h" → Microcontroller-specific header file (contains register definitions and peripheral addresses).
  #include <string.h> → Standard string library (strcpy, memcpy, strlen, etc.).
  #include "img.h" → User-defined header file (probably contains image data or function declarations).
  #include <stdint.h> → Fixed-width integer types (uint8_t, uint16_t, int32_t, etc.).
  #include <math.h> → Mathematical functions (sin, cos, sqrt, pow, etc.).
*/

#include "stm32f411xx.h"
#include  <string.h>
#include "img.h"
#include <stdint.h>
#include <math.h>

#define  TRACE_HALF_W  1

#define  LCD_W         240
#define  LCD_H         320

/* colors (RGB565) */
#define  COL_BG        0x0000   // black background
#define  COL_TRACE     0x07E0   // green trace
#define  COL_GRID      0x2104   // faint grid (optional)

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



/*************************************************************************************************
 *                                         TFT-LCD
 *
 *
 *
 *************************************************************************************************/


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

#define  LCD_PORT      GPIOB
#define  LCD_RST_PIN   5
#define  LCD_CS_PIN    6
#define  LCD_DC_PIN    7


/*

*/
void Spi2GpioInits(void)
{
          GPIO_Handle_t SpiPins , ConfigPins                          ;


          // SPI , SCK  AF5

          SpiPins.pGPIOx     = GPIOB                                   ;

          SpiPins.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN  ;
          SpiPins.GPIO_PinConfig.GPIO_PinAltFunMode  = 5                ;
          SpiPins.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_PP  ;
          SpiPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD     ;
          SpiPins.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST  ;


          SpiPins.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_13   ;
          GPIO_Init(&SpiPins) ;   // SCLK

          SpiPins.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_15   ;
          GPIO_Init(&SpiPins) ;   // MOSI

          // RST, CS, DC -> plain GPIO output

          ConfigPins.pGPIOx = GPIOB;
          ConfigPins.GPIO_PinConfig.GPIO_PinMode      = GPIO_MODE_OUTPUT ;
          ConfigPins.GPIO_PinConfig.GPIO_PinOPType    = GPIO_OP_TYPE_PP  ;
          ConfigPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD   ;
          ConfigPins.GPIO_PinConfig.GPIO_PinSpeed     = GPIO_SPEED_FAST  ;

          ConfigPins.GPIO_PinConfig.GPIO_PinNumber = LCD_RST_PIN         ;
          GPIO_Init(&ConfigPins);
          ConfigPins.GPIO_PinConfig.GPIO_PinNumber = LCD_CS_PIN;
          GPIO_Init(&ConfigPins);
          ConfigPins.GPIO_PinConfig.GPIO_PinNumber = LCD_DC_PIN;
          GPIO_Init(&ConfigPins);

          // Idle CS high (deselected) until we actually talk to the display
          GPIO_WriteToOutputPin(LCD_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void Spi2_Inits(void)
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
    lcd_reset();

    lcd_write_cmd(0x01); //systick_msec_delay(150); // software reset
    lcd_write_cmd(0x11); //systick_msec_delay(120); // sleep out

    lcd_write_cmd(0x3A);
    lcd_write_data(0x55);               // 16-bit RGB565

    lcd_write_cmd(0x36);
    lcd_write_data(0x00);               // MADCTL, plain orientation for now

    //lcd_write_cmd(0x21);                // display inversion on — many ST7789 panels need this

    lcd_write_cmd(0x29); //systick_msec_delay(50) ;  // display ON
}


void lcd_set_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
    lcd_write_cmd(0x2A);        // Column address set
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


void lcd_draw_image(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint16_t *img_data) {
    lcd_write_cmd(0x2A); // CASET
    lcd_write_data(x0 >> 8); lcd_write_data(x0 & 0xFF);
    lcd_write_data((x0+w-1) >> 8); lcd_write_data((x0+w-1) & 0xFF);

    lcd_write_cmd(0x2B); // PASET
    lcd_write_data(y0 >> 8); lcd_write_data(y0 & 0xFF);
    lcd_write_data((y0+h-1) >> 8); lcd_write_data((y0+h-1) & 0xFF);

    lcd_write_cmd(0x2C); // RAMWR

    TFT_Select() ;
    GPIO_WriteToOutputPin(LCD_PORT, LCD_DC_PIN, GPIO_PIN_SET);
    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        uint8_t hi = img_data[i] >> 8;
        uint8_t lo = img_data[i] & 0xFF;
        SPI_SendData(SPI2, &hi, 1);
        SPI_SendData(SPI2, &lo, 1);
    }
    TFT_Unselect() ;
}








/**/


/*************************************************************************************************
 *                        ST7789 hardware vertical-scroll commands
 *
 *  0x33  VSCRDEF   Vertical Scrolling Definition
 *        TFA (top fixed area) + VSA (scrolling area) + BFA (bottom fixed area) = panel height
 *
 *  0x37  VSCAD     Vertical Scroll Start Address (VSP)
 *        Tells the controller which GRAM row is currently mapped to the TOP of the
 *        scrolling window. Incrementing this each frame makes the whole scrolling
 *        area appear to slide upward by one row, with NO SPI redraw of old rows.
 *************************************************************************************************/

void lcd_vscroll_def(uint16_t tfa, uint16_t vsa, uint16_t bfa)
{
    lcd_write_cmd(0x33);
    lcd_write_data(tfa >> 8); lcd_write_data(tfa & 0xFF);
    lcd_write_data(vsa >> 8); lcd_write_data(vsa & 0xFF);
    lcd_write_data(bfa >> 8); lcd_write_data(bfa & 0xFF);
}

void lcd_vscroll_addr(uint16_t vsp)
{
    lcd_write_cmd(0x37);
    lcd_write_data(vsp >> 8);
    lcd_write_data(vsp & 0xFF);
}

/*************************************************************************************************
 *  lcd_draw_wave_row()
 *
 *  Renders ONE horizontal line (the display rotated 90deg mentally: each row = one
 *  time-sample, x-position along that row = instantaneous signal amplitude).
 *  This is the line that gets written into GRAM just ahead of the scroll pointer,
 *  so it "enters" the visible window as the scroll offset advances past it.
 *************************************************************************************************/

static void lcd_draw_wave_row(uint16_t row, float phase)
{
    const int16_t center  = LCD_W / 2;
    const int16_t amp     = 90;     // signal swing in pixels
    const uint8_t thickness = 2;    // trace half-thickness, in pixels

    int16_t x_signal = center + (int16_t)(amp * sinf(phase));
    if (x_signal < thickness)            x_signal = thickness;
    if (x_signal > LCD_W - 1 - thickness) x_signal = LCD_W - 1 - thickness;

    lcd_set_window(0, row, LCD_W - 1, row);

    TFT_Select();
    TFT_DataMode();

    uint8_t bg[2] = { COL_BG    >> 8, COL_BG    & 0xFF };
    uint8_t fg[2] = { COL_TRACE >> 8, COL_TRACE & 0xFF };

    for (uint16_t x = 0; x < LCD_W; x++)
    {
        if (x >= (uint16_t)(x_signal - thickness) && x <= (uint16_t)(x_signal + thickness))
            SPI_SendData(SPI2, fg, 2);
        else if (x == (uint16_t)center)          // faint centerline for reference
            { uint8_t g[2]={COL_GRID>>8,COL_GRID&0xFF}; SPI_SendData(SPI2, g, 2); }
        else
            SPI_SendData(SPI2, bg, 2);
    }

    TFT_Unselect();
}

/*************************************************************************************************
 *  lcd_wave_scroll_run()
 *
 *  Continuously scrolls a sine-wave "signal" trace down the display.
 *  - phase_step controls the apparent frequency of the wave along the scroll axis
 *  - delay controls scroll speed (rows/sec) -> replace the busy loop with a
 *    systick/timer-based millisecond delay for real timing accuracy
 *************************************************************************************************/

static void crude_delay(volatile uint32_t count)
{
    while (count--) { __asm__("nop"); }
}

void lcd_wave_scroll_run(void)
{
    /* whole panel is one scrolling region: TFA=0, VSA=320, BFA=0 */
    lcd_vscroll_def(0, LCD_H, 0);

    uint16_t vsp = 0;          // current scroll start address
    float    phase = 0.0f;
    const float phase_step = 0.18f;

    /* clear screen once at start */
    for (uint16_t r = 0; r < LCD_H; r++)
        lcd_draw_wave_row(r, phase);

    while (1)
    {
        /* the row about to scroll OFF the top becomes free GRAM space —
           reuse it to draw the newest sample, which will scroll in at
           the bottom of the window one full revolution later */
        uint16_t write_row = vsp;

        lcd_draw_wave_row(write_row, phase);

        vsp = (vsp + 1) % LCD_H;
        lcd_vscroll_addr(vsp);

        phase += phase_step;
        crude_delay(400);      // TODO: replace with systick_msec_delay(15) once implemented
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////


static int16_t prev_x[LCD_H];

/*************************************************************************************************
 *  lcd_paint_segment()
 *  Paints a short horizontal segment (the trace thickness) at (xc, y) in one color.
 *  Uses a single SPI burst over a tight column window instead of per-pixel windows.
 *************************************************************************************************/
static void lcd_paint_segment(uint16_t y, int16_t xc, uint16_t color)
{
    if (xc < 0) return;

    int16_t x0 = xc - TRACE_HALF_W;
    int16_t x1 = xc + TRACE_HALF_W;
    if (x0 < 0) x0 = 0;
    if (x1 > LCD_W - 1) x1 = LCD_W - 1;

    lcd_set_window((uint16_t)x0, y, (uint16_t)x1, y);

    TFT_Select();
    TFT_DataMode();

    uint8_t px[2] = { color >> 8, color & 0xFF };
    for (int16_t x = x0; x <= x1; x++)
        SPI_SendData(SPI2, px, 2);

    TFT_Unselect();
}

/*************************************************************************************************
 *  lcd_wave_flow_init()
 *  Clears the screen and marks every row as "not yet drawn" so the first
 *  animation frame paints the full curve once.
 *************************************************************************************************/
void lcd_wave_flow_init(void)
{
    /* full-screen black clear, one SPI burst per row */
    lcd_set_window(0, 0, LCD_W - 1, LCD_H - 1);
    TFT_Select();
    TFT_DataMode();
    uint8_t bg[2] = { COL_BG >> 8, COL_BG & 0xFF };
    for (uint32_t i = 0; i < (uint32_t)LCD_W * LCD_H; i++)
        SPI_SendData(SPI2, bg, 2);
    TFT_Unselect();

    for (uint16_t y = 0; y < LCD_H; y++)
        prev_x[y] = -1;   // invalid -> forces first-frame draw, nothing to erase
}



/*************************************************************************************************
 *  lcd_wave_flow_run()
 *
 *  Draws a sine curve running the full height of the panel and animates it by
 *  advancing `phase` every frame -- the crests visibly travel down (or up, if
 *  you negate phase_step) the screen continuously, which is the "flowing
 *  analog signal" look. Only pixels that actually change row-to-row are
 *  repainted, so it stays responsive without DMA.
 *************************************************************************************************/
void lcd_wave_flow_run(void)
{
    const float wavelength  = 60.0f;   // px per full sine cycle along the height
    const float phase_step  = 0.35f;   // how far the wave "travels" per frame
    const int16_t center    = LCD_W / 2;
    const int16_t amp       = 90;

    lcd_wave_flow_init();

    float phase = 0.0f;

    while (1)
    {
        for (uint16_t y = 0; y < LCD_H; y++)
        {
            float angle = (2.0f * 3.14159265f * (y + phase)) / wavelength;
            int16_t x_new = center + (int16_t)(amp * sinf(angle));

            if (x_new < TRACE_HALF_W)              x_new = TRACE_HALF_W;
            if (x_new > LCD_W - 1 - TRACE_HALF_W)   x_new = LCD_W - 1 - TRACE_HALF_W;

            if (x_new != prev_x[y])
            {
                if (prev_x[y] >= 0)
                    lcd_paint_segment(y, prev_x[y], COL_BG);     // erase old position

                lcd_paint_segment(y, x_new, COL_TRACE);          // paint new position
                prev_x[y] = x_new;
            }
        }

        phase += phase_step;             // advance the wave -> visible flow
        crude_delay(1500);                // TODO: swap for systick_msec_delay(~20)
    }
}




int main()
{   
    /* Initialize the GPIO pins for SPI communication (SCLK and MOSI). */
    Spi2GpioInits();

    /* */
    Spi2_Inits();

    lcd_init();

    //lcd_wave_flow_run();
    lcd_draw_image(0, 0, 240, 320, image_eesha) ;

    while(1);
}
