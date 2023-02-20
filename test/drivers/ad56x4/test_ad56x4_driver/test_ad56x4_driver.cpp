#include <unity.h>
#include "unity_test_runner.h"
#include "hw_platform.h"
#include "ad56x4.h"
 
static ad56x4_cfg_t cfg;
static ad56x4_t dev;
static ad56x4_err_code_t ret;

void ad56x4_cs_select(ad56x4_t* dev) {}
void ad56x4_cs_deselect(ad56x4_t* dev) {}
 
void setUp(void)
{
    ad56x4_set_default_cfg(&cfg);
    ret = ad56x4_init_hw(&dev, &cfg);
}
 
void tearDown(void)
{}
 
void test_ad56x4_set_default_cfg()
{
    ad56x4_cfg_t def_cfg;
    ad56x4_set_default_cfg(&def_cfg);
    TEST_ASSERT_EQUAL(PLATFORM_SPI_MOSI, def_cfg.mosi_pin);
    TEST_ASSERT_EQUAL(PLATFORM_SPI_MISO, def_cfg.miso_pin);
    TEST_ASSERT_EQUAL(PLATFORM_SPI_SCK, def_cfg.sck_pin);
    TEST_ASSERT_EQUAL(AD56X4_CS, def_cfg.cs_pin);
    TEST_ASSERT_EQUAL(PLATFORM_SPI_SPEED, def_cfg.spi_speed);
    TEST_ASSERT_EQUAL(PLATFORM_SPI_MODE_1, def_cfg.spi_mode);
    TEST_ASSERT_EQUAL(PLATFORM_SPI_MSBFIRST, def_cfg.spi_bit_order);
    TEST_ASSERT_EQUAL(AD56X4_RESOLUTION, def_cfg.resolution);
}
 
void test_ad56x4_init_hw_ok()
{
    TEST_ASSERT_EQUAL(PLATFORM_OK, ret);
}

void test_ad56x4_generic_write_ok()
{
    ret = ad56x4_generic_write(&dev, AD56X4_CMD_SW_RESET, AD56X4_CH_DONT_CARE, AD56X4_SW_RST_FULL);
    TEST_ASSERT_EQUAL(PLATFORM_OK, ret);
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ad56x4_set_default_cfg);
    RUN_TEST(test_ad56x4_init_hw_ok);
    RUN_TEST(test_ad56x4_generic_write_ok);
    return UNITY_END();
}
