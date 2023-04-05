#include <SPI.h>
#include <Wire.h>

bts71220_dcr_reg_t dcr_reg;

void setup() {
  Serial1.begin(115200);
  while (!Serial1) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial1.println("Initializing..");
  pinMode(NEO_CORE_DO0, OUTPUT);
  pinMode(NEO_CORE_DO1, OUTPUT);
  pinMode(NEO_CORE_DO4, OUTPUT);
  pinMode(NEO_CORE_DO5, OUTPUT);
  digitalWrite(NEO_CORE_DO0, HIGH);
  digitalWrite(NEO_CORE_DO1, HIGH);
  digitalWrite(NEO_CORE_DO4, HIGH);
  digitalWrite(NEO_CORE_DO5, HIGH);
}

void loop(void) {
  delay(500);
  Serial1.print("Power: ");
  Serial1.print(readVoltageSuply());
  Serial1.print(" mV");
  enableCurrentSenseDO(0);
  Serial1.print("\tDO0 I: ");
  Serial1.print(readCurrentSenseDO());
  Serial1.print(" mA");
  enableCurrentSenseDO(1);
  Serial1.print("\tDO1 I: ");
  Serial1.print(readCurrentSenseDO());
  Serial1.print(" mA");
  enableCurrentSenseDO(4);
  Serial1.print("\tDO4 I: ");
  Serial1.print(readCurrentSenseDO());
  Serial1.print(" mA");
  enableCurrentSenseDO(5);
  Serial1.print("\tDO5 I: ");
  Serial1.print(readCurrentSenseDO());
  Serial1.print(" mA");
  Serial1.print("\tTemperature Board: ");
  Serial1.print(readBoardTemperature(), 2);
  Serial1.print(" °C");
  Serial1.print(" RP2040: ");
  Serial1.print(analogReadTemp(3.3F), 2);
  Serial1.println(" °C");
}
