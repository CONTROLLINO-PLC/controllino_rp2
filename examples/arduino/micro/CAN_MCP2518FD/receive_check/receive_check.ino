// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13
/*
    Modified for use with Controllino Micro
    by Controllino firmware team 2024
*/
#include <SPI.h>
#include "mcp2518fd_can.h"

mcp2518fd CAN(MCP2518FD_DEFAULT_CS_PIN);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    CAN.setSPI(&SPI1);
    while (CAN_OK != CAN.begin(CAN_500KBPS, MCP2518FD_20MHz)) { // init can bus : baudrate = 500k
        Serial.println("CAN init fail, retry...");
        delay(100);
    }
    Serial.println("CAN init ok!");
}

void loop() {
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) { // check if data coming
        CAN.readMsgBuf(&len, buf); // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();

        Serial.println("-----------------------------");
        Serial.print("Get data from ID: 0x");
        Serial.println(canId, HEX);

        for (int i = 0; i < len; i++) { // print the data
            Serial.print(buf[i], HEX);
            Serial.print("\t");
        }
        Serial.println();
    }
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
