/**
 * @file   main.cpp
 * @author Jacob Corr
 * @date   22/01/2023
 * @brief  IOT Coursework main file. Creates a connection with google cloud platform
 *         & runs a demo of a voice controlled teaching assistant with TTS capability
 */

#include "MQTT.h"
#include "functions.h"
#include "EZvr.h"
#include "Joystick.h"

#include "LCD.h"
#include <stdio.h>
#include <iostream>
#include <string>

// std::string extracted_str;
// std::stringstream ss;

int main()
{
    /* local variables */                                           //FTDI.printf("main\r\n");printf("main\r\n");
    int count = 0, timed = 0;                                        
    char pub[128], display[128], number[32];
    Ticker timeOut;

    /* assign interrupts */
    btn1.rise(handleButtonRise);
    A.rise(handleButtonRise);
    B.rise(handleButtonRise);
    Y.rise(handleButtonRise);
    START.rise(handleButtonRise);
    BACK.rise(handleButtonRise);
    L.rise(handleButtonRise);
    R.rise(handleButtonRise);
    serial.attach(&rx_interrupt, Serial::RxIrq);                    // printf("attach \r\n");

    /* begin init */
    lcd_init();
    write_to_display("initialising");
    
    
    /* connect to network */
    if(init_network() < 0){
        printf("network init. failed! \r\n");
        return -1;
    }

    /* generate jwt for MQTT connection */
    generate_jwt_token(password, JWT_BUF_SIZE);

    /*connect to network*/
    connect_network();

    /* Establish MQTT connection. */
    if(MQTT_connect(password, mqtt_client_id) < 0){
        printf("MQTT failed! \r\n");
        return -1;
    }

    /* Subscribe to MQTT topic */
    int rc = mqttClient->subscribe(mqtt_topic_sub.c_str(), MQTT::QOS0, handleMqttMessage);
        if (rc != MQTT::SUCCESS) {
            printf("ERROR: rc from MQTT subscribe is %d\r\n", rc);
        } else isSubscribed = true;

    publish_message(init);
    ThisThread::sleep_for(5000);

    /* while loop constantly checks for connection or if a message needs to be send or parsed*/
    while(1) {
        /* Client is disconnected */
        if(!mqttClient->isConnected()){        
            break;  
        }
        /* Wait for message */
        if(mqttClient->yield(100) != MQTT::SUCCESS) {
            break;   
        }
        /* Received a message. */
        if(MessageArrived) {
            MessageArrived = false;
            printf("\r\nMessage arrived:\r\n%s\r\n", messageBuffer);
        }
        /* publish a test message on button press*/
        if(testPublish) {
            sprintf(pub, "testing %d", count);
            count++;
            publish_message(pub);
        }
        /* Respond to char on Rx buf */
        if(qflg){
        /* non-blocking serial read */                                  //FTDI.printf("qflg trig\r\n");
            while(!rx_buf.empty()){
                evt = rx_buf.get();
                if(evt.status == osEventMessage) {
                /* Event Message received */                            //FTDI.printf("OSEM trig\r\n");   
                    cmdBuffer += static_cast<char>(evt.value.v);        //FTDI.printf("buf: %s\r\n",buffer.c_str());
                }
            }
            qflg = 0;
        }
        /* End of line on serial detected */ 
        if(EOL == 1) {   
            EOL = 0;                                                    //FTDI.printf("%s\r\n",buffer.c_str());
            /* If command*/
            if(detect_command(pub)){
                /* generate response and publish to GCP*/
                if(respond(pub)){                                       //FTDI.printf("pub recv %s", pub);
                publish_message(pub);
                }
            }
            /* Print a timeout message for 1s if timed out */
            if(detect_timeout(display)){
                guard = 1;                                              //FTDI.printf("timed out");
                timeOut.attach(&display_int, 1);
                write_to_display("timed out!");
            }
            /* Handle Math Operations */
            if(math == 1){
                if(detect_command(number)){
                    maths_op(number);
                    /* if finished, publish answer on MQTT */
                    if(math == 0){
                        publish_message(num_str);
                    }
                }
            }          
            cmdBuffer.clear();
        }
        /* If no action taken, prompt user */ 
        if(guard == 0){
            write_to_display("Please speak now");
        }
        
    }
    /* client has disconnected */
    printf("The client has disconnected.\r\n");
    terminate_session();
}

/**
 * @brief Callback function called when button is pushed.
 */
void handleButtonRise() {                                       //printf("button press\r\n");
    testPublish = true;
}

