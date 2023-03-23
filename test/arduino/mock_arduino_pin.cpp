#ifdef NATIVE_ENV
 
#include "Arduino.h"
 
extern "C" void __pinMode(pin_size_t ulPin, PinMode ulMode) {}
extern "C" void digitalWrite(pin_size_t ulPin, PinStatus ulVal) {}
extern "C" PinStatus digitalRead(pin_size_t ulPin) { return LOW; }
extern "C" void analogWrite(pin_size_t pin, int val) {}

extern uint8_t TEST_ANALOG_READ_ADC __attribute__((weak, alias("__TEST_ANALOG_READ_ADC")));
uint8_t __TEST_ANALOG_READ_ADC = 1;
extern "C" int analogRead(pin_size_t pin) { return TEST_ANALOG_READ_ADC; }
 
#endif