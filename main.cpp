// #include "TLSSocket.h"
// #include "MQTTClientMbedOs.h"
#include "MQTT.h"
// #include "mbed-trace/mbed_trace.h"
// #include "mbed_events.h"
// #include "NTPClient.h"
// #include "jwtgen.h"
// #include "mbedtls/error.h"
#include "functions.h"
// #include <string>
// #include <fstream>
#include <stdio.h>
// #include <iostream>

/* Buffer size for messages */
//#define MESSAGE_BUFFER_SIZE 512

int main()
{
    int count = 0;
    /* initialise network connection */
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
    /* Enable button 1 for publishing a message. */
    InterruptIn btn1(BUTTON1);
    btn1.rise(handleButtonRise);

    /* while loop constantly checks for connection or if a message needs to be send or parsed*/
    while(1) {
        /* Client is disconnected. */
        if(!mqttClient->isConnected()){        
            break;  }
        /* Waits a message and handles keepalive. */
        if(mqttClient->yield(100) != MQTT::SUCCESS) {
            break;   }
        /* Received a message. */
        if(MessageArrived) {
            MessageArrived = false;
            printf("\r\nMessage arrived:\r\n%s\r\n", messageBuffer);
        }
        /* Button is pushed - publish a message. */
        if(isPublish) {
            char pub[1024];
            sprintf(pub, "testing %d", count);
            count++;
            publish_message(pub);
        }
    }

    /* client has disconnected */
    printf("The client has disconnected.\r\n");
    terminate_session();


}

