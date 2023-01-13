#ifndef __LCD_H__
#define __LCD_H__

#include "N5110.h"

#define MAX_LEN 14
#define MAX_NUM_STRINGS 6

N5110 lcd(PTC9,PTC0,PTC7,PTD2,PTD1,PTC11);  // K64F - pwr from 3V3

void lcd_init(void);
void write_to_display(char* msg);

void lcd_init(void){
    lcd.init();
    lcd.setContrast(0.4);
    // these are default settings so not strictly needed
    lcd.normalMode();      // normal colour mode
    lcd.setBrightness(0.5); // put LED backlight on 50%

    lcd.clear();
    lcd.refresh();
    ThisThread::sleep_for(500);
}

void write_to_display(const char* input_string) {
    /*split into up to 5 strings with a max length of 13 char */
    char output_strings[MAX_NUM_STRINGS][MAX_LEN];
    int num_strings;

    /* Split the input string into smaller strings */
    size_t input_string_len = strlen(input_string);
    num_strings = input_string_len / MAX_LEN;
    if (input_string_len % MAX_LEN != 0) {
        (num_strings)++;
    }
    if (num_strings > MAX_NUM_STRINGS) {
        num_strings = MAX_NUM_STRINGS;
    }

    int index = 0;
    for (int i = 0; i < num_strings-1; i++) {
        int j;
        for (j = 0; j < MAX_LEN-1; j++) {
            if (index >= input_string_len-1) {
                break;
            }
            output_strings[i][j] = input_string[index];
            index++;
        }
        output_strings[i][j] = '\0';
    }

    /* Write the smaller strings to the display */
    lcd.clear(); // clear buffer at start of every loop
    for (int i = 0; i < num_strings; i++) {
        lcd.printString(output_strings[i], 0, i);
    }
    lcd.refresh();
    ThisThread::sleep_for(500);
}

#endif /* __LCD_H__ */