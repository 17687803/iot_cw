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

bool detect_command(char *msg);
bool respond(char *command);

/* Some strings */
char init[] = "Initialisation complete... Welcome!";
char auth[] = "Please say the password to authorise identity";
char greet[] = "Hello Jacob. How can i help you today?";
char query[] = "What can i help you with?";
char query2[] = "Can i help with anything else?";
char subject[] = "Which subject do you need help with?";
char maths[] = "Yes i can help with Maths. What would you like me to do?";
char add[] = "Pick some numbers to add, say zero when finished.";
char mult[] = "Pick some numbers to multiply, say zero when finished.";
char divide[] = "Pick some numbers to divide, say zero when finished.";
char sub[] = "Pick some numbers to subtract, say zero when finished.";
char spell[] = "What would you like me to spell?";
char xylo[] = "Xylophone is spelled x, y, l, o, p, h, o, n, e";
char acqu[] = "Acquiesce is spelled a, c, q, u, i, e, s, c, e";

/**
* @brief  respond to a command
* This function responds to a command by comparing it to string literals
* If the command does not match any strings, the function returns false
*
* @param  command : char pointer representing the command
* @return bool    : true if the command is "PASSWORD" otherwise false
*/
bool respond(char *command){          
    /* Decide on response */                                                                //FTDI.printf("res recv %s", command);
    if(std::strcmp(command, "BLYTHE") == 0) {
        strcpy(command, auth);                                                              //FTDI.printf("res sent %s", command);
        return true;                                                            
    }
    if(std::strcmp(command, "PASSWORD") == 0) {
        strcpy(command, greet);                                                             //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "HELP") == 0) {
        strcpy(command, query);                                                             //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "SPELL") == 0) {
        strcpy(command, spell);                                                             //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "MATHS") == 0) {
        strcpy(command, maths);                                                             //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "ADD") == 0) {
        strcpy(command, add);                                                               //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "MULT") == 0) {
        strcpy(command, mult);                                                              //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "DIVIDE") == 0) {
        strcpy(command, divide);                                                            //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "SUBTRACT") == 0) {
        strcpy(command, sub);                                                            //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "XYLOPHONE") == 0) {
        strcpy(command, xylo);                                                            //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "ACQUIESCE") == 0) {
        strcpy(command, acqu);                                                            //FTDI.printf("res sent %s", command);
        return true;
    }
    if(std::strcmp(command, "ZERO") == 0) {
        strcpy(command, query2);                                                            //FTDI.printf("res sent %s", command);
        return true;
    }
    return false;
}

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
    /* Or find position of the strings "Command: "&"\r\n"  */
    start = cmdBuffer.find("Command: ");
    end = cmdBuffer.find("\r\n");
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