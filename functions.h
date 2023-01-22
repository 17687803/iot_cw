/**
 * @file   EZvr.h
 * @author Jacob Corr
 * @date   22/01/2023
 * @brief  Header file containing functions for the project
 */

#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "TLSSocket.h"
#include "MQTTClientMbedOs.h"
#include "MQTT.h"
#include "mbed-trace/mbed_trace.h"
#include "mbed_events.h"
#include "NTPClient.h"
#include "jwtgen.h"
#include "mbedtls/error.h"

#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>
 
#define MESSAGE_BUFFER_SIZE 512
/* globals */

/* Buffers for messages */
char messageBuffer[MESSAGE_BUFFER_SIZE];
char* password = new char[JWT_BUF_SIZE];
char temp[MESSAGE_BUFFER_SIZE] = {};

/* MQTT Client IDs */
std::string mqtt_client_id = std::string("projects/") + PROJ_ID + "/locations/" + REGION + "/registries/" + REGISTRY + "/devices/" + DEV_ID;
std::string mqtt_topic_pub = std::string("/devices/") + DEV_ID  + "/events";
std::string mqtt_topic_sub = std::string("/devices/") + DEV_ID  + "/commands/#";

/* Flags */
static volatile bool testPublish = false;
static unsigned int msg_init = 0;
static volatile bool MessageArrived = false;
static volatile bool isSubscribed = false;

/* Network Global Parameters */
NetworkInterface* network = NULL;
TLSSocket *socket = new TLSSocket;
MQTTClient* mqttClient = NULL;

/* Initialise IO */
InterruptIn btn1(BUTTON1);
InterruptIn A(PTB9);
InterruptIn B(PTD0);
InterruptIn Y(PTC12);
InterruptIn START(PTC5);
InterruptIn BACK(PTB19);
InterruptIn L(PTB18);
InterruptIn R(PTB3);


/* Function prototypes */
int getPassword(char *buf, size_t buf_size);
int init_network(void);
int connect_network(void);
int MQTT_connect(char* password, std::string mqtt_client_id);
int publish_message(char* input);
void generate_jwt_token(char* password, int size);
void handleMqttMessage(MQTT::MessageData& md);
void config_io(void);
void handleButtonRise(void);
void handleButton2Rise(void);
bool MQTT_subscribe(MQTTClient* mqttClient, std::string mqtt_topic_sub);
bool compose_message(char* messageBuffer, char* temp);
void terminate_session(void);


    

/**
 * @brief This function initializes the default network interface and establishes a connection to the network. If any errors
 * occur during the initialization process, an error message is printed
 *
 * @return 0 if the network interface was initialized and connected successfully, or -1 if an error occurred.
 */
int init_network(void){
    printf("Getting default instance...\r\n");
    network = NetworkInterface::get_default_instance();
    if (!network) {
        printf("network interface not found!\n");
        return -1;
    }
    nsapi_size_or_error_t ret = network->connect();
    if (ret) {
        printf("connect() error code :%d\n", ret);
        return -1;
    }
    printf("Network interface opened successfully.\r\n");
    printf("\r\n");
    return 0;
}

/**
 * @brief Generate a JWT token and save it to a buffer.
 * If the token cannot be generated, an error message is printed.
 *
 * @param password A buffer to save the generated JWT token to. The buffer should be at least size bytes in size.
 * @param size The size of the password buffer in bytes.
 * @return void
 */
void generate_jwt_token(char* password, int size){
    /* get server time */
    NTPClient ntp(network);
    ntp.set_server("time.google.com", 123);
    time_t now = ntp.get_timestamp();
    set_time(now);
    /* JWT - Set JWT as password.  */
    if(getPassword(password, size) != 0) {
        printf("ERROR: Failed to generate JWT.\r\n");
    }
}

/**
 * @brief  Generates a password using the MBED-OS JWT generator. 
 *
 * @param password A buffer to save the generated JWT token to. The buffer should be at least size bytes in size.
 * @param size The size of the password buffer in bytes.
 * @return 0 on success, -1 on failure.
 */
int getPassword(char *buf, size_t buf_size) {
    int ret = -1;
    size_t len = 0;
    time_t now = time(NULL);
    /* using mbed-jwtgen lib */
    if(JwtGenerator::getJwt(buf, buf_size - 1, &len, SSL_CLIENT_PRIVATE_KEY_PEM,
            PROJ_ID, now, now + TIME_JWT_EXP) == JwtGenerator::SUCCESS) {
        buf[len] = '\0';
        ret = 0;
    }
    return ret;
}

/**
 * @brief This function establishes a network connection using the specified host name and port. 
 * It also sets the root CA certificate and client certificate/key to be used for secure connections. 
 * If any errors occur during the connection process, an error message is printed. 
 *
 * @return 0 if the connection was established successfully, or -1 if an error occurred.
 */
int connect_network(void){
    printf("Connecting to host %s:%d ...\r\n", MQTT_SERVER_HOST_NAME, MQTT_SERVER_PORT);
    nsapi_error_t ret = socket->open(network);
    if (ret != NSAPI_ERROR_OK) {
        printf("Could not open socket! Returned %d\n", ret);
        return -1;  }
    ret = socket->set_root_ca_cert(SSL_CA_PEM);
    if (ret != NSAPI_ERROR_OK) {
        printf("Could not set ca cert! Returned %d\n", ret);
        return -1;  }
    ret = socket->set_client_cert_key(SSL_CLIENT_CERT_PEM, SSL_CLIENT_PRIVATE_KEY_PEM);
    if (ret != NSAPI_ERROR_OK) {
        printf("Could not set keys! Returned %d\n", ret);
        return -1;  }
    ret = socket->connect(MQTT_SERVER_HOST_NAME, MQTT_SERVER_PORT);
    if (ret != NSAPI_ERROR_OK) {
        printf("Could not connect! Returned %d\n", ret);
        return -1;  }
    printf("Connection established.\r\n");
    return 0;
}

/**
 * @brief Connect to MQTT server with provided client ID and password.
 * 
 * @param password Password to use for MQTT connection
 * @param mqtt_client_id Client ID to use for MQTT connection
 * @return int 0 on success, -1 on failure
 */
int MQTT_connect(char* password, std::string mqtt_client_id){
    printf("MQTT client connecting...\r\n");
    /* Initialize MQTT connection data */
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion      = 4;
    data.clientID.cstring = (char *)mqtt_client_id.c_str();
    data.username.cstring = (char *)"ignored";
    data.password.cstring = (char *)password;
    /* Create MQTT client and attempt connection */
    mqttClient = new MQTTClient(socket);
    int rc = mqttClient->connect(data);
    if (rc != MQTT::SUCCESS) {
        printf("ERROR: rc from MQTT connect is %d\r\n", rc);
        return -1;
    }
    printf("Client connected!\r\n");
    return 0;
}

/**
 * @brief Publishes a message to the subscribed MQTT topic.
 * Compose a message using the specified input string and publishes it to the specified topic. 
 *
 * @param input The input string to be used to compose the message.
 * @return 0 if the message was published successfully, or the error code if an error occurred.
 */
int publish_message(char* input){
    /* reset flag */
    testPublish = false;
    /* Compose the message using the input string and messageBuffer */
    if(!(compose_message(messageBuffer, input)))
    {
        printf("message failed to be written\r\n");
        return -1;
    }
    /* Initialize MQTT message structure */
    MQTT::Message message;
    message.retained = false;
    message.dup = false;
    /* Set message properties. */
    message.payload = (void*)messageBuffer;
    message.qos = MQTT::QOS0;
    message.id = 0;
    message.payloadlen = strlen(messageBuffer);
    /* Publish the message. */
    printf("\r\nAttempting to publish to topic %s:\r\n%s\r\n", mqtt_topic_pub.c_str(), messageBuffer);
    int rc = mqttClient->publish(mqtt_topic_pub.c_str(), message);
    if(rc != MQTT::SUCCESS)
    {
        printf("ERROR: rc from MQTT publish is %d\r\n", rc);
        return rc;
    }
    printf("Message published.\r\n");
    return rc;
}

/**
 * @brief Compose a JSON compatible MQTT message from a given string and save it to a buffer.
 *
 * @param messageBuffer A reference to a buffer to save the composed message to.
 * @param temp The string to use to compose the message.
 * @return True if the message was successfully composed and saved to the buffer, or false if an error occurred.
 */
bool compose_message(char* messageBuffer, char* temp){
    /* Compose message string */
    std::string initStr = std::string("{\"cloudRegion\":\"") + REGION + "\",\"deviceId\":\"" 
                + DEV_ID + "\",\"registryId\":\"" + REGISTRY + "\",\"text\":\"" + temp + "\",\"hops\":1}"; 
    /* Check if message exceeds buffer size */
    if(initStr.length() > MESSAGE_BUFFER_SIZE){
        printf("Message exceeded buffer length!\r\n");
        return false;
    }
    /* Copy message to buffer */
    strncpy(messageBuffer, initStr.c_str(), MESSAGE_BUFFER_SIZE);
    return true;
}
/**
 * @brief Handle incoming MQTT message.
 * Callback that copies the payload of the message to a buffer, and sets a flag indicating that a message has arrived.
 *
 * @param md MQTT::MessageData object containing the incoming message
 */
void handleMqttMessage(MQTT::MessageData& md)
{
    /* Copy payload to the buffer.*/
    MQTT::Message &message = md.message;
    memcpy(messageBuffer, message.payload, message.payloadlen);
    messageBuffer[message.payloadlen] = '\0';
    /* Set flag */
    MessageArrived = true;
}



/**
 * @brief Terminates MQTT session.
 * This function will unsubscribe from any subscribed topics, disconnect from the MQTT broker, and release any
 * allocated resources such as the MQTT client, the network socket, and the network interface.
 */
void terminate_session(void){
    if(mqttClient) {
            if(isSubscribed) {
                mqttClient->unsubscribe(mqtt_topic_sub.c_str());
                mqttClient->setMessageHandler(mqtt_topic_sub.c_str(), 0);
            }
            if(mqttClient->isConnected()) 
                mqttClient->disconnect();
            delete mqttClient;
        }
        if(socket) {
            socket->close();
            delete socket;
        }
        if(network) {
            network->disconnect();
        }
    printf("session terminated.\r\n");
}

#endif /* __FUNCTIONS_H__ */