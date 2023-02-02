/*
 * Copyright (c) 2023 CONTROLLINO GmbH.
 *
 * SPDX-License-Identifier: MIT
 */
 
#include "wsen_temp.h"
#include "string.h"
 
/* Initializes default configuration */
void wsen_temp_set_default_cfg(wsen_temp_cfg_t* cfg)
{
    cfg->sda_pin = PLATFORM_I2C_SDA;
    cfg->scl_pin = PLATFORM_I2C_SCL;
    cfg->i2c_speed = PLATFORM_I2C_SPEED;
    cfg->i2c_addr = WSEN_TEMP_ADDRESS_SAO_GND;
    cfg->i2c = PLATFORM_I2C_HW;
    cfg->int_pin = WSEN_TEMP_INT;
}
 
/* Initializes hardware according to configuration */
wsen_temp_err_code_t wsen_temp_init(wsen_temp_t* dev, wsen_temp_cfg_t* cfg)
{
    // Init hardware I2C interface
    if (platform_i2c_init(cfg->i2c, cfg->i2c_speed, cfg->sda_pin, cfg->scl_pin) != WSEN_TEMP_OK)
        return WSEN_TEMP_INIT_ERR;
    // Init hardware reset and wsen_temp_err_code_t pins 
    platform_gpio_init(cfg->int_pin, PLATFORM_GPIO_OUT, PLATFORM_GPIO_PULL_DISABLED);
    // Set values from cfg
    dev->i2c_addr = cfg->i2c_addr;
    dev->i2c = cfg->i2c;
    dev->int_pin = cfg->int_pin;
    // Check coms
    uint8_t id;
    wsen_temp_err_code_t ret = wsen_temp_get_id(dev, &id);
    if ((ret != WSEN_TEMP_OK) || id != WSEN_TEMP_DEVICE_ID_VALUE)
        return WSEN_TEMP_INIT_ERR;
    return WSEN_TEMP_OK;
}
 
/* Writes data to register */
wsen_temp_err_code_t wsen_temp_generic_write(wsen_temp_t* dev, wsen_temp_reg_t reg, uint8_t* txdata, uint8_t txlen)
{
    uint8_t tx_buf[txlen + 1];
    tx_buf[0] = reg;
    memcpy(&tx_buf[1], txdata, txlen);
    return platform_i2c_write(dev->i2c, dev->i2c_addr, tx_buf, sizeof(tx_buf));
}
 
/* Reads data from register */
wsen_temp_err_code_t wsen_temp_generic_read(wsen_temp_t* dev, wsen_temp_reg_t reg, uint8_t* rxdata, uint8_t rxlen)
{
    if (platform_i2c_write(dev->i2c, dev->i2c_addr, &reg, 1) != WSEN_TEMP_OK)
        return WSEN_TEMP_I2C_ERR;
    return platform_i2c_read(dev->i2c, dev->i2c_addr, rxdata, rxlen);
}
 
/* Read the device id */
wsen_temp_err_code_t wsen_temp_get_id(wsen_temp_t* dev, uint8_t* id)
{
    return wsen_temp_generic_read(dev, WSEN_TEMP_REG_DEVICE_ID, id, 1);
}
 
/* Software reset */
wsen_temp_err_code_t wsen_temp_sw_rst(wsen_temp_t* dev) {
    wsen_temp_reset_t rst_reg;
    wsen_temp_err_code_t ret;
    // Set 1
    ret = wsen_temp_generic_read(dev, WSEN_TEMP_REG_SOFT_RESET, (uint8_t*)&rst_reg, 1);
    if (ret != WSEN_TEMP_OK)
        return ret;
    rst_reg.reset = 1;
    ret = wsen_temp_generic_write(dev, WSEN_TEMP_REG_SOFT_RESET, (uint8_t*)&rst_reg, 1);
    if (ret != WSEN_TEMP_OK)
        return ret;
    // Set 0
    ret = wsen_temp_generic_read(dev, WSEN_TEMP_REG_SOFT_RESET, (uint8_t*)&rst_reg, 1);
    if (ret != WSEN_TEMP_OK)
        return ret;
    rst_reg.reset = 0;
    return wsen_temp_generic_write(dev, WSEN_TEMP_REG_SOFT_RESET, (uint8_t*)&rst_reg, 1);
}
 
/* Set CONTROL register settings */
wsen_temp_err_code_t wsen_temp_set_ctrl(wsen_temp_t* dev, wsen_temp_ctrl_t* ctrl)
{
    return wsen_temp_generic_write(dev, WSEN_TEMP_REG_CTRL, (uint8_t*)ctrl, 1);
}
 
/* Get CONTROL register current state */
wsen_temp_err_code_t wsen_temp_get_ctrl(wsen_temp_t* dev, wsen_temp_ctrl_t* ctrl)
{
    return wsen_temp_generic_read(dev, WSEN_TEMP_REG_CTRL, (uint8_t*)ctrl, 1);
}
 
/* Get STATUS register current state */
wsen_temp_err_code_t wsen_temp_get_status(wsen_temp_t* dev, wsen_temp_status_t* status)
{
    return wsen_temp_generic_read(dev, WSEN_TEMP_REG_STATUS, (uint8_t*)status, 1);
}
 
/* Set temperature high limit for interrupt */
wsen_temp_err_code_t wsen_temp_set_high_lim(wsen_temp_t* dev, uint8_t limit)
{
    return wsen_temp_generic_write(dev, WSEN_TEMP_REG_HIGH_LIM, &limit, 1);
}
 
/* Get temperature high limit for interrupt */
wsen_temp_err_code_t wsen_temp_get_high_lim(wsen_temp_t* dev, uint8_t* limit)
{
    return wsen_temp_generic_read(dev, WSEN_TEMP_REG_HIGH_LIM, limit, 1);
}
 
/* Set temperature low limit for interrupt */
wsen_temp_err_code_t wsen_temp_set_low_lim(wsen_temp_t* dev, uint8_t limit)
{
    return wsen_temp_generic_write(dev, WSEN_TEMP_REG_LOW_LIM, &limit, 1);
}
 
/* Get temperature low limit for interrupt */
wsen_temp_err_code_t wsen_temp_get_low_lim(wsen_temp_t* dev, uint8_t* limit)
{
    return wsen_temp_generic_read(dev, WSEN_TEMP_REG_LOW_LIM, limit, 1);
}
 
/* Get raw temperature digital value */
wsen_temp_err_code_t wsen_temp_get_raw(wsen_temp_t* dev, uint16_t* temp)
{
    uint8_t tmp[2];
    wsen_temp_err_code_t ret;
    ret = wsen_temp_generic_read(dev, WSEN_TEMP_REG_DATA_T_L, &tmp[0], 1);
    if (ret != WSEN_TEMP_OK)
        return ret;
    ret = wsen_temp_generic_read(dev, WSEN_TEMP_REG_DATA_T_H, &tmp[1], 1);
    if (ret != WSEN_TEMP_OK)
        return ret;
    *temp = (uint16_t)(tmp[1] << 8) | (uint16_t)tmp[0];
    return WSEN_TEMP_OK;
}

/* Get temperature value in °C */
wsen_temp_err_code_t wsen_temp_get_celsius(wsen_temp_t* dev, float* temp)
{
    uint16_t raw_temp;
    wsen_temp_err_code_t ret = wsen_temp_get_raw(dev, &raw_temp);
    if (ret != WSEN_TEMP_OK)
        return ret;
    *temp = (float)raw_temp * 0.01F;
    return WSEN_TEMP_OK;
}