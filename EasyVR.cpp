#include "EasyVR.h"
#include "ncurses.h"

/* Setup UART @ 9600 baud */ 
Serial EasyVrUART(PTC17, PTC16, 9600);  //tx = PTD3(D12); rx = PTD1(D13)


void delay(int ms) {
    ThisThread::sleep_for(ms);
}

// char getcWithTimeout(int timeout, Stream& EasyVrUART) {
//     char data;
//     Timer timer;
//     timer.start();
//     // Store the timer's start time
//     int start_time = timer.read_ms();
//     while (!EasyVrUART.readable()){
//         // Compare the current time with the start time
//         int time = timer.read_ms();
//         if (time - start_time > timeout) {
//             return -1;
//         }
//     }
//     data = EasyVrUART.getc();
//     return data;
// }

// /**
//  * @brief Initialize the EasyVR module for UART communication
//  * This function initializes the EasyVR module for communication over UART by returning a reference to the Stream object used for communication.
//  * 
//  * @return Returns a reference to the Stream object used for UART communication
//  */
Serial& EasyVR_init(void){
    return EasyVrUART;
}

// /**
//  * @brief Establish a bridge with a host device using UART
//  * This function sends a marker to the host device to indicate that it is getcy to establish a bridge.
//  * It then waits for a response from the host device to establish the type of bridge (normal or boot).
//  * 
//  * @param EasyVrUART Reference to a Stream object used for UART communication
//  * @return Returns the type of bridge established (BRIDGE_NONE, BRIDGE_NORMAL, or BRIDGE_BOOT)
//  */
int EasyVR::bridgeRequested(Stream& EasyVrUART)
{
    /* local variables */
    int bridge = BRIDGE_NONE; 
    bool request = false;
    int rx = 0, tx = 0;
    int bytesgetc, bytesWritten;
    // /* send marker to host device to indicate getcy to establish bridge */
    while(rx != 0x6F){
        tx = 0x62;                      //send b to wake up
        tx = EasyVrUART.putc(tx);
        printf("sent %x\r\n", tx);
        if(EasyVrUART.readable()){
            rx = EasyVrUART.getc();
        }
        if(rx < 0) {
            printf("0 bytes getc\r\n");
        }else printf("getc: %x\r\n", rx);
    }
    tx = 0x20;
    tx = EasyVrUART.putc(tx);
    printf("sent %x\r\n", tx);
    if(tx < 0) {
        printf("error: 0 bytes written\r\n");
    } else printf("Wrote: %x\r\n", tx);
    for (int t=0; t<150; ++t)
    {    
        printf("Starting loop #%d\r\n", t);
        /* sleep for 10ms to avoid overloading the system */
        ThisThread::sleep_for(10); 
        rx = EasyVrUART.getc();
        if(rx < 0) {
            printf("0 bytes getc\r\n");
            continue;
        }else printf("getc: %x\r\n", rx);
        /* checks and sets the type of bridge */
        if (!request)
        {
            if (rx == 0xBB)
            {
                tx = 0xCC;
                bytesWritten = EasyVrUART.putc(tx);
                if(bytesgetc <= 0) {
                    printf("0 bytes written\r\n");
                }
                request = true;
                continue;
            }
            request = false;
        }
        else
        {   
            if (rx == 0xDD) 
            {
                tx = 0xEE;
                bytesWritten = EasyVrUART.putc(tx);
                if(bytesgetc <= 0) {
                    printf("0 bytes written\r\n");
                }
                bridge = BRIDGE_NORMAL;
                break;
            }
            if (rx == 0xAA) 
            {
                tx = 0xFF;
                bytesWritten = EasyVrUART.putc(tx);
                if(bytesgetc <= 0) {
                    printf("0 bytes written\r\n");
                }
                bridge = BRIDGE_BOOT;
                break;
            }
            request = false;
        }
    }
    printf("returning: %d\r\n", bridge);
    return bridge;
}


// int EasyVR::bridgeRequested(Stream& pcSerial)
// {
//   pcSerial.putc(0x99);
//   // look for a request header
//   int bridge = BRIDGE_NONE;
//   bool request = false;
//   int t, rx;
//   for (t=0; t<150; ++t)
//   {
//     delay(10);
//     rx = pcSerial.getc();
//     if (rx < 0)
//       continue;
//     if (!request)
//     {
//       if (rx == 0xBB)
//       {
//         pcSerial.putc(0xCC);
//         delay(1); // flush not reliable on some core libraries
//         request = true;
//         continue;
//       }
//       request = false;
//     }
//     else
//     {
//       if (rx == 0xDD)
//       {
//         pcSerial.putc(0xEE);
//         delay(1); // flush not reliable on some core libraries
//         bridge = BRIDGE_NORMAL;
//         break;
//       }
//       if (rx == 0xAA)
//       {
//         pcSerial.putc(0xFF);
//         delay(1); // flush not reliable on some core libraries
//         bridge = BRIDGE_BOOT;
//         break;
//       }
//       request = false;
//     }
//   }
//   return bridge;
// }