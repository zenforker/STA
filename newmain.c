
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#pragma config FOSC = INTOSC    // Oscillator Selection bits (INTOSC oscillator: CLKIN function disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
#pragma config LPBOR = ON      // Brown-out Reset Selection bits (BOR disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF   

// Flash Memory Self-Write Protection (Write protection off)

// define constants
#define _XTAL_FREQ  8000000
#define _SLOW_FLASH 20000
#define _FAST_FLASH 10000
#define _TIMER_PER  20

//#define FULL_MODE
//#define DEV_BOARD   
#ifdef  DEV_BOARD
#define LED_ON      PORTAbits.RA1 = 1
#define LED_OFF     PORTAbits.RA1 = 0
#else
#define LED_ON      PORTAbits.RA1 = 0
#define LED_OFF     PORTAbits.RA1 = 1
#endif

#define SWITCH      PORTAbits.RA2

// declare variables
bool     pin_state;
uint16_t blink_count  = 0;
uint16_t led_period   = 0;
//bool     button_press = false;
uint8_t  Prev_state;
/*           Pic10F322
 *              _______
 *        RA0  |       | RA 3
 *        VSS  |       | VDD
 *        RA 1 |_______| RA 2
 * 
 * The development board has LED as high driven while the actual
 * PCB has it low driven
 */
void main( void )
{
    /*  Pin management  */
    LATA   = 0x00;
    TRISA  = 0x0D; //0b00001101, was 0x0B=0b00001011
    ANSELA = 0x00;
    WPUA   = 0x00;
//    OPTION_REGbits.nWPUEN = 1;

    pin_state = PORTAbits.RA1;              
    LED_ON;
    
    while ( 1 )
    {
        if ( PORTAbits.RA2 == 0 )
        {
            __delay_ms(15);
            if ( PORTAbits.RA2 == 0 )
            {                
                while (PORTAbits.RA2 == 0) ;
                
                pin_state = ~pin_state;
                PORTAbits.RA1 = pin_state;
                __delay_ms(10);
            }                    
        }   
    }
}
