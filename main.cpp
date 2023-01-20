#include "MQTT.h"
#include "functions.h"
#include "EZvr.h"
#include "Joystick.h"

#include "LCD.h"
#include <stdio.h>
#include <iostream>
#include <string>
PwmOut piezo(PTC10);
int main()
{
    /* local variables */ 
    int count = 0, timed = 0;                                       //FTDI.printf("main\r\n");printf("main\r\n");
    char pub[128], display[128];
    Ticker timeOut;

    /* Initialise IO */
    
    InterruptIn btn1(BUTTON1);
    InterruptIn A(PTB9);
    InterruptIn B(PTD0);
    InterruptIn Y(PTC12);
    InterruptIn START(PTC5);
    InterruptIn BACK(PTB19);
    InterruptIn L(PTB18);
    InterruptIn R(PTB3);
    /* assign interrupts */
    btn1.rise(handleButtonRise);
    A.fall(handleButton2Rise);
    B.fall(handleButton2Rise);
    Y.fall(handleButton2Rise);
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
                write_to_display("timed out! Please try again");
            }          
            cmdBuffer.clear();
        }
        /* If no action taken, prompt user */ 
        if(guard == 0){
            write_to_display("Please speak now..");
        }
        //lcd.setBrightness(0.5);  
    }
    /* client has disconnected */
    printf("The client has disconnected.\r\n");
    terminate_session();
}

/**
 * @brief Callback function called when button is pushed.
 */
void handleButtonRise() {                                       //printf("button press\r\n");
    // Set the PWM signal to 50% duty cycle
    // piezo.write(0.5);
    // Turn on the buzzer
    // piezo = 1;
    // Wait for a short duration
    // ThisThread::sleep_for(50);
    // Turn off the buzzer
    // piezo = 0;
    testPublish = true;
}

/**
 * @brief Callback function called when button is pushed.
 */
void handleButton2Rise() {                                       //printf("button press\r\n");
    // Set the PWM signal to 50% duty cycle
    //piezo.write(0.5);
    // Turn on the buzzer
    //piezo = 1;
    // Wait for a short duration
    //ThisThread::sleep_for(50);
    // Turn off the buzzer
    //piezo = 0;
    testPublish = true;
}
