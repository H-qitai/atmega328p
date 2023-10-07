/*
 * display.h
 *
 * Created: 29/09/2023 6:32:56 pm
 *  Author: qhua835
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

// linkers

void display_init();

const uint8_t number[10];

void send_next_character_to_display();
void seperate_and_load_characters(uint16_t number, uint8_t decimal_pos);


#endif /* DISPLAY_H_ */