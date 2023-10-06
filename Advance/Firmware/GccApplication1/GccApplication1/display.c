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


void init_display(){
	// SH_CP/SH_DS/SH_ST set to output
	DDRC |= (SH_CP) | (SH_DS) | (SH_ST);
	// Ds1/Ds2/Ds3/Ds4 set to output
	DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);

}

//Populate the array ‘disp_characters[]’ by separating the four digits of ‘number’
//and then looking up the segment pattern from ‘seg_pattern[]’
void seperate_and_load_characters(uint16_t number, uint8_t decimal_pos){
	//TODO: finish this function
	//1. Separate each digit from ‘number’
	// e.g. if value to display is 1230 the separated digits will be
	// ‘1’, ‘2’, ‘3’ and ‘0’
	//2. Lookup pattern required to display each digit from ‘seg_pattern[]’
	// and store this pattern in appropriate position of ‘disp_characters[]’
	// e.g. For digit ‘0’ in example above disp_characters[0] = seg_pattern[0]
	//3. For the project you may modify this pattern to add decimal point at
	// the position ‘decemal_pos’
	
	disp_characters[0] = seg_pattern[number/1000];
	disp_characters[1] = seg_pattern[number/100%10];
	disp_characters[2] = seg_pattern[number/10%10];
	disp_characters[3] = seg_pattern[number%10];
		
}

//Render a single digit from ‘disp_characters[]’ on the display at ‘disp_position’
void send_next_character_to_display(void){
	//TODO: finish this function
	//1. Based on ‘disp_position’, load the digit to send to a local variable
	//2. Send this bit pattern to the shift-register as in Q2.2
	//3. Disable all digits
	//4. Latch the output by toggling SH_ST pin as in Q2.2
	//5. Now, depending on the value of pos, enable the correct digit
	// (i.e. set Ds1, Ds2, Ds3 and Ds4 appropriately)
	//6. Increment ‘disp_position’ so the next of the 4 digits will be displayed
	// when function is called again from ISR (reset ‘disp_position’ after 3)
		uint8_t currentdigit = disp_characters[disp_position];
		
		PORTC &= ~(SH_CP) & ~(SH_ST);
		
		for (int i = 7; i > -1; i--){
			if (currentdigit & (1 << i)) {
				PORTC |= (SH_DS); // Set SH_DS to 1
				} else {
				PORTC &= ~(SH_DS); // Set SH_DS to 0
			}
			PORTC |= SH_CP;
			PORTC &= ~(SH_CP);
		}
	
	PORTD |= (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7);
	
		PORTC |= SH_ST;
		PORTC &= ~(SH_ST);
	
	    // 5. Enable the correct digit
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
	    
	    // 6. Increment ‘disp_position’ and reset after 3
	    disp_position++;
	    if (disp_position > 3) {
		    disp_position = 0; // Reset after displaying all 4 digits
	    }
	
}