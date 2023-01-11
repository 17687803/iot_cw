#ifndef __EZVR_H__
#define __EZVR_H__

#include "mbed.h"
#include <string>

RawSerial FTDI(PTC17, PTC16, 115200);       //UART3
RawSerial serial(USBTX, USBRX, 115200);

Queue<char, 128> rx_buf;
EventQueue queue;
int qflg = 0;
int EOL = 0;
osEvent evt = {};
std::string cmdBuffer = "";

bool detect_command(char *pub);

/* Some strings */
char init[] = "Initialisation complete... Welcome!";
char greet[] = "Hello Jacob. How can i help you today?";

/**
 * @brief This function detects and extracts a specific command from a string buffer
 * The extracted word is then saved as a null-terminated character array, passed in by reference.
 * 
 * @param msg Pointer to the null-terminated character array where the extracted word will be saved
 * @return True if the command is found, False otherwise
*/
bool detect_command(char *msg){ 
    /* Find position of the strings "Word: "&"\r\n"  */
    std::size_t start = cmdBuffer.find("Word: ");
    std::size_t end = cmdBuffer.find("\r\n");
    /* Check if both starting and ending positions are found */
    if(start != std::string::npos && end != std::string::npos) {
        /* Extract the word from cmdBuffer */
        std::size_t wordStart = cmdBuffer.find("=", start) + 2;             
        string word = cmdBuffer.substr(wordStart, end-wordStart);
        /* Allocate memory & copy word */
        strcpy(msg, word.c_str());
        return true;
    }
    return false;
}
#endif /* __EZVR_H__ */