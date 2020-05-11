
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
//#include "device_initialize.h"
//#include "pwm.h"
//#include "tmr2.h"
#include <stdbool.h>
#include <stdint.h>

//#pragma config MCLRE = OFF, CP = OFF, WDTE = OFF, FORC = INTRC

 // CONFIG

#pragma config FOSC = INTOSC    // Oscillator Selection bits (INTOSC oscillator: CLKIN function disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
// #pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
#pragma config LPBOR = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF    
// Flash Memory Self-Write Protection (Write protection off)

// define constants
#define _XTAL_FREQ  8000000
#define _SLOW_FLASH 20000
#define _FAST_FLASH 10000
#define _TIMER_PER  20

// declare variables
bool    pin_state;
uint16_t blink_count  = 0;
//uint8_t button_count = 0;
uint16_t led_period   = 0;
bool    button_press = false;
uint8_t Prev_state;
/*           Pic10F322

 *              _______
 *        RA0  |       | RA 3
 *        VSS  |       | VDD
 *        RA 1 |_______| RA 2
 */
void main( void )
{
    /*  Pin management  */
    LATA = 0x00;
    TRISA = 0x0B; 
    ANSELA = 0x00;
    WPUA = 0x00;
    OPTION_REGbits.nWPUEN = 1;

    // LFIOFR 31.25KHz_osc_not_ready; HFIOFS unstable; HFIOFR 16MHz_osc_not_ready; IRCF 8MHz;
    OSCCON = 0x60;
    // CLKROE disabled;
    CLKRCON = 0x00;    
//    PORTA  = 0x00;

    led_period = _FAST_FLASH;
    
    if( (STATUS & 0x18) == 0x18)        // is this Brown-Out Reset?
    {
        Prev_state = BORCON & 0xC0;     // If yes, return to the state before the reset

        switch (Prev_state)             // Check the previous status of the LED
        {
        case 0x00:                      // sleep;
            PORTAbits.RA2 = 0;
            break;
        case 0x40:                      // LED steady
            PORTAbits.RA2 = 1;
            break;
        case 0x80:
            led_period = _SLOW_FLASH;   // slow flash
            blink_count = led_period;
            PORTAbits.RA2 = 1;
            break;                      // else if status == slow_flash, slow_flash;
        case 0xC0:                      // fast flash
            led_period = _FAST_FLASH;   // on and off period
            blink_count = led_period;
            PORTAbits.RA2 = 1;
            break;                      // else if status == slow_flash, slow_flash;
        default:
            PORTAbits.RA2 = 0;
            break;
        }
    }
 
    
    while ( 1 )
    {
        if ( PORTAbits.RA1 == 0 )
        {
            __delay_ms(10);
            if ( PORTAbits.RA1 == 0 )
            {                
                while (PORTAbits.RA1 == 0) ;
                
                BORCON += 0x40;
                Prev_state = BORCON & 0xC0;                
                         
                switch (Prev_state)             // Check the previous status of the LED
                {
                    case 0x00:
//                    led_period = 0;
                        PORTAbits.RA2 = 1;
                        break;
                    case 0x40:
                        led_period = _SLOW_FLASH;
                        blink_count = led_period;
                        pin_state = PORTAbits.RA2;
                        break;
                    case 0x80:
                        led_period = _FAST_FLASH;
                        blink_count = led_period;
                        pin_state = PORTAbits.RA2;
                        break;
                    case 0xC0:
 //                   led_period = 0;
                        PORTAbits.RA2 = 0;
                        break;
                    default:
//                    led_period = 0;
                        PORTAbits.RA2 = 0;
                        break;
                }
            }                    
        }
//        while(PORTAbits.RA1 == 1) ;           
        if ((Prev_state != 0x00) & (Prev_state != 0xC0))
        {
            led_period--;
            if (led_period == 0)
            {
                led_period = blink_count;
                pin_state = ~pin_state;
                PORTAbits.RA2 = pin_state;
                __delay_ms(10);
            }
        }
    }
}
