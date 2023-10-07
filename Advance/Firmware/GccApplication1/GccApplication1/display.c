/*
 * display.c
 *
 * Created: 29/09/2023 6:32:30 pm
 *  Author: qhua835
 */ 

#define F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "display.h"

// Macro for easier reading
#define SH_CP (1<<PC3)
#define SH_DS (1<<PC4)
#define SH_ST (1<<PC5)


const uint8_t seg_pattern[10] = {
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111 // 9
};

//4 characters to be displayed on Ds1 to Ds 4
static volatile uint8_t disp_characters[4]={0,0,0,0};

//The current digit (e.g. the 1's, the 10's) of the 4-digit number we're displaying
static volatile uint8_t disp_position = 0;


void display_init(){
	// This function initialise the display
	
	// SH_CP/SH_DS/SH_ST set to output
	DDRC |= (SH_CP) | (SH_DS) | (SH_ST);
	// Ds1/Ds2/Ds3/Ds4 set to output
	DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);

}

void seperate_and_load_characters(uint16_t number, uint8_t decimal_pos){
	// This function shifts the input number into their position in the position array.
	
	disp_characters[0] = seg_pattern[number/1000];
	disp_characters[1] = seg_pattern[number/100%10];
	disp_characters[2] = seg_pattern[number/10%10];
	disp_characters[3] = seg_pattern[number%10];
		
}

void send_next_character_to_display(void){
	// This function send the character currently in disp_character to the display
	
		uint8_t currentdigit = disp_characters[disp_position];
		
		// Disable Serial in and output
		PORTC &= ~(SH_CP) & ~(SH_ST);
		
		// Since it is 7 segment
		// Check with the seg_pattern which bits need to be logic 1 or 0
		for (int i = 7; i > -1; i--){
			if (currentdigit & (1 << i)) {
				PORTC |= (SH_DS); // Set SH_DS to 1
				} else {
				PORTC &= ~(SH_DS); // Set SH_DS to 0
			}
			// After it is set, toggling this will shift the bits into the latch
			PORTC |= SH_CP;
			PORTC &= ~(SH_CP);
		}
	
	
	// Ds1/Ds2/Ds3/Ds4 are cleared and wont display anything (active high)
	PORTD |= (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7);
	
		// Toggling this will latch the output
		PORTC |= SH_ST;
		PORTC &= ~(SH_ST);
	
	    // Enable the correct digit to be displayed on Ds1..4
	    switch (disp_position){
			case 0:
				PORTD &= ~(1<<PD4);
				break;
			case 1:
				PORTD &= ~(1<<PD5);
				break;
			case 2:
				PORTD &= ~(1<<PD6);
				break;
			case 3:
				PORTD &= ~(1<<PD7);
				break;
		}
	    
	    // Increment disp_position and reset after 3
	    disp_position++;
	    if (disp_position > 3) {
		    disp_position = 0; // Reset after displaying all 4 digits
	    }
	
}