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


/**
 * @brief This function detects and extracts a specific command from a string buffer
 * Searches for the substring "Word: " in buffer and extracts the word following it, if it exists.
 * The extracted word is then saved as a null-terminated character array, passed in by reference.
 * @param pub Pointer to the null-terminated character array where the extracted word will be saved
 * @return True if the command is found, False otherwise
*/
bool detect_command(char *pub){ 
    std::size_t start = cmdBuffer.find("Word: ");
    std::size_t end = cmdBuffer.find("\r\n");
    if(start != std::string::npos && end != std::string::npos) {
        std::size_t wordStart = cmdBuffer.find("=", start) + 2;             FTDI.printf("command found\r\n");
        string word = cmdBuffer.substr(wordStart, end-wordStart);
        char pub[word.size()+1];
        strcpy(pub, word.c_str());
        return true;
    }
    return false;
}
#endif /* __EZVR_H__ */