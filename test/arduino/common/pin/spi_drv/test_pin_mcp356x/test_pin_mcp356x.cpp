#include <unity.h>
#include "Arduino.h"
#include "wiring.h"

#define TEST_MCP3564_ADC_READING_ERROR 300
uint32_t TEST_MCP3564_ADC_DATA = 0x3FFFFF;

#ifdef CONTROLLINO_MICRO
extern ControllinoPin* _CONTROLLINO_MICRO_AI0;
#define TEST_MCP3564_ARDUINO_PIN CONTROLLINO_MICRO_AI0
#define _TEST_MCP3564_ARDUINO_PIN _CONTROLLINO_MICRO_AI0
#endif

void setUp(void)
{
#ifdef NATIVE_ENV
    initVariant();
#endif
    pinMode(TEST_MCP3564_ARDUINO_PIN, INPUT);
}
 
void tearDown(void)
{
    setDigitalThreshold(TEST_MCP3564_ARDUINO_PIN, ~0U);
}
 
void test_mcp3564_pin_definitions_ok()
{
    TEST_ASSERT_EQUAL(MCP3564_CH_CH0, _TEST_MCP3564_ARDUINO_PIN->getPin());
    TEST_ASSERT_EQUAL(ControllinoPin::MCP3564_PIN, _TEST_MCP3564_ARDUINO_PIN->getType());
}
 
void test_mcp3564_pin_pinMode_always_input()
{
    TEST_ASSERT_EQUAL(INPUT, _TEST_MCP3564_ARDUINO_PIN->getMode());
    pinMode(TEST_MCP3564_ARDUINO_PIN, OUTPUT);
    TEST_ASSERT_EQUAL(INPUT, _TEST_MCP3564_ARDUINO_PIN->getMode());
    pinMode(TEST_MCP3564_ARDUINO_PIN, INPUT_PULLUP);
    TEST_ASSERT_EQUAL(INPUT, _TEST_MCP3564_ARDUINO_PIN->getMode());
    pinMode(TEST_MCP3564_ARDUINO_PIN, INPUT_PULLDOWN);
    TEST_ASSERT_EQUAL(INPUT, _TEST_MCP3564_ARDUINO_PIN->getMode());
    pinMode(TEST_MCP3564_ARDUINO_PIN, OUTPUT_2MA);
    TEST_ASSERT_EQUAL(INPUT, _TEST_MCP3564_ARDUINO_PIN->getMode());
    pinMode(TEST_MCP3564_ARDUINO_PIN, OUTPUT_4MA);
    TEST_ASSERT_EQUAL(INPUT, _TEST_MCP3564_ARDUINO_PIN->getMode());
    pinMode(TEST_MCP3564_ARDUINO_PIN, OUTPUT_8MA);
    TEST_ASSERT_EQUAL(INPUT, _TEST_MCP3564_ARDUINO_PIN->getMode());
    pinMode(TEST_MCP3564_ARDUINO_PIN, OUTPUT_12MA);
    TEST_ASSERT_EQUAL(INPUT, _TEST_MCP3564_ARDUINO_PIN->getMode());
}
 
void test_mcp3564_pin_analogRead_ok()
{
    int adc_mcp3564;
    uint8_t dummySgn;
    uint32_t dummyRes;
    uint8_t mux;
    /* Default ADC resolution */
    analogReadResolution(23);
    int adcArduino = analogRead(TEST_MCP3564_ARDUINO_PIN);
    /* Test MUX config */
    mcp3564_iread(dev_mcp3564, MCP3564_REG_MUX, &mux, 1);
    TEST_ASSERT_EQUAL(_TEST_MCP3564_ARDUINO_PIN->getPin() | MCP3564_MUX_VIN_NEG_VREF_EXT_MINUS, mux);
    /* Compare analog read with ADC raw read */
    mcp3564_read_raw_adc(dev_mcp3564, (uint32_t*)&adc_mcp3564, &dummySgn, &dummyRes);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MCP3564_ADC_READING_ERROR, abs(adcArduino - adc_mcp3564));
    /* Compare two ADC readings one after the other two times */
    mcp3564_read_adc_mux(dev_mcp3564, (uint32_t*)&adc_mcp3564, MCP3564_MUX_VIN_POS_CH6, MCP3564_MUX_VIN_NEG_VREF_EXT_MINUS, 350);
    adcArduino = analogRead(TEST_MCP3564_ARDUINO_PIN);
    TEST_ASSERT_GREATER_THAN(TEST_MCP3564_ADC_READING_ERROR, abs(adcArduino - adc_mcp3564));
    mcp3564_read_adc_mux(dev_mcp3564, (uint32_t*)&adc_mcp3564, MCP3564_MUX_VIN_POS_CH6, MCP3564_MUX_VIN_NEG_VREF_EXT_MINUS, 350);
    adcArduino = analogRead(TEST_MCP3564_ARDUINO_PIN);
    TEST_ASSERT_GREATER_THAN(TEST_MCP3564_ADC_READING_ERROR, abs(adcArduino - adc_mcp3564));
}
 
void test_mcp3564_pin_analogWrite_does_nothing()
{
    /* Just test if compiles */
    analogWrite(TEST_MCP3564_ARDUINO_PIN, 0);
}
 
void test_mcp3564_pin_digitalWrite_does_nothing()
{
    /* Just test if compiles */
    digitalWrite(TEST_MCP3564_ARDUINO_PIN, HIGH);
}
 
void test_mcp3564_pin_get_set_digitalThreshold()
{
    TEST_ASSERT_EQUAL(~0U, getDigitalThreshold(TEST_MCP3564_ARDUINO_PIN));
    setDigitalThreshold(TEST_MCP3564_ARDUINO_PIN, TEST_MCP3564_ADC_DATA);
    TEST_ASSERT_EQUAL(TEST_MCP3564_ADC_DATA, getDigitalThreshold(TEST_MCP3564_ARDUINO_PIN));
}

void test_mcp3564_digitalRead_with_digitalThreshold()
{
    TEST_ASSERT_EQUAL(LOW, digitalRead(TEST_MCP3564_ARDUINO_PIN));
#ifdef NATIVE_ENV /* Equal case can only be tested in native enviroment */
    setDigitalThreshold(TEST_MCP3564_ARDUINO_PIN, TEST_MCP3564_ADC_DATA);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(TEST_MCP3564_ARDUINO_PIN));
#endif
    setDigitalThreshold(TEST_MCP3564_ARDUINO_PIN, analogRead(TEST_MCP3564_ARDUINO_PIN) + TEST_MCP3564_ADC_READING_ERROR);
    TEST_ASSERT_EQUAL(LOW, digitalRead(TEST_MCP3564_ARDUINO_PIN));
    //
    setDigitalThreshold(TEST_MCP3564_ARDUINO_PIN, 0);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(TEST_MCP3564_ARDUINO_PIN));
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mcp3564_pin_definitions_ok);
    RUN_TEST(test_mcp3564_pin_pinMode_always_input);
    RUN_TEST(test_mcp3564_pin_analogRead_ok);
    RUN_TEST(test_mcp3564_pin_analogWrite_does_nothing);
    RUN_TEST(test_mcp3564_pin_digitalWrite_does_nothing);
    RUN_TEST(test_mcp3564_pin_get_set_digitalThreshold);
    RUN_TEST(test_mcp3564_digitalRead_with_digitalThreshold);
    UNITY_END();
    return UNITY_END();
}
