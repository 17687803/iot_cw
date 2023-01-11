#include "MQTT.h"
#include "functions.h"
#include "EZvr.h"
#include <stdio.h>
#include <iostream>
#include <string>

void rx_interrupt() {
    int c = serial.getc();
    if(c == '\r') {
        qflg = 1;
    }
    if(c == '\n') {
        EOL = 1; 
        qflg = 1;
    }
    if(c >= 0) {
        rx_buf.put((char *)c);
        qflg = 1;
    }
}

int main()
{
    int count = 0;                                                  FTDI.printf("main\r\n");printf("main\r\n");
    char pub[1024];
    serial.attach(&rx_interrupt, Serial::RxIrq);                    // printf("attach \r\n");
    
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

    /* Enable button 1 for publishing a message */
    InterruptIn btn1(BUTTON1);
    btn1.rise(handleButtonRise);

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
        if(isPublish) {
            sprintf(pub, "testing %d", count);
            count++;
            publish_message(pub);
        }

        if(qflg){
        /* non-blocking serial read */                                  //FTDI.printf("qflg trig\r\n");
            while(!rx_buf.empty()){
                evt = rx_buf.get();
                if(evt.status == osEventMessage) {
                /* Event Message received */                            //FTDI.printf("OSEM trig\r\n");   
                    cmdBuffer += static_cast<char>(evt.value.v);           //FTDI. printf("buf: %s\r\n",buffer.c_str());
                }
            }
            qflg = 0;
        }
        if(EOL == 1) {   
            EOL = 0;                                                    //FTDI.printf("%s\r\n",buffer.c_str());
            if(detect_command(pub)){
                publish_message(pub);
            }            
            cmdBuffer.clear();
        }
    }

    /* client has disconnected */
    printf("The client has disconnected.\r\n");
    terminate_session();

}

