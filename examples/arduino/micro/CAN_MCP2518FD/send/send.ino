// demo: CAN-BUS Shield, send data
// loovee@seeed.cc
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

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
        Serial.println("CAN init fail, retry...");
        delay(100);
    }
    Serial.println("CAN init ok!");
}

unsigned char stmp[8] = { 0 };

void loop() {
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    stmp[7] = stmp[7] + 1;
    if (stmp[7] == 100) {
        stmp[7] = 0;
        stmp[6] = stmp[6] + 1;

        if (stmp[6] == 100) {
            stmp[6] = 0;
            stmp[5] = stmp[5] + 1;
        }
    }

    CAN.sendMsgBuf(0x00, 0, 8, stmp);
    delay(1000);                       // send every 1000ms
    Serial.println("CAN BUS sendMsgBuf ok!");
}

// END FILE
