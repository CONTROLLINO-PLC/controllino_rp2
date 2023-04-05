/*
 * Copyright (c) 2023 CONTROLLINO GmbH.
 *
 * SPDX-License-Identifier: MIT
 */
 
#include "hw_platform.h"
#include <SPI.h>
 
#define _hw_spi_t SPIClassRP2040
hw_spi_t PLATFORM_SPI_HW = (hw_spi_t)   &SPI;
const unsigned int PLATFORM_SPI_SPEED = 1000000;
const int PLATFORM_SPI_MOSI =           19;
const int PLATFORM_SPI_MISO =           16;
const int PLATFORM_SPI_SCK =            18;
 
/* Init SPI interface */
platform_err_code_t platform_spi_init(hw_spi_t spi_hw, unsigned int speed, int mosi_pin, int miso_pin, int sck_pin)
{
    // Check arguments
    if ((spi_hw != (hw_spi_t)&SPI && spi_hw != (hw_spi_t)&SPI1) ||
        (mosi_pin < 0 || mosi_pin > 29) ||
        (miso_pin < 0 || miso_pin > 29) ||
        (sck_pin < 0 || sck_pin > 29) ||
        (speed < 1000 || speed > 2000000))
    {
        return PLATFORM_ARGUMENT_ERR;
    }
    SPIClassRP2040* _SPI = (SPIClassRP2040*)spi_hw;
    _SPI->setTX(mosi_pin);
    _SPI->setRX(miso_pin);
    _SPI->setSCK(sck_pin);
    _SPI->begin(false);
    return PLATFORM_OK;
}
 
/* Change SPI settings */
platform_err_code_t platform_spi_set_config(hw_spi_t spi_hw, unsigned int speed, platform_spi_mode_t mode, platform_spi_bit_order_t bit_order)
{

    // Check arguments
    if ((mode != PLATFORM_SPI_MODE_0 && mode != PLATFORM_SPI_MODE_1 && mode != PLATFORM_SPI_MODE_2 && mode != PLATFORM_SPI_MODE_3) ||
        (bit_order != PLATFORM_SPI_LSBFIRST && bit_order != PLATFORM_SPI_MSBFIRST) ||
        (speed < 1000 || speed > 2000000))
    {
        return PLATFORM_ARGUMENT_ERR;
    }
    // Set SPI settings
    uint32_t _clock = (uint32_t)speed;
    BitOrder _bitOrder = bit_order == PLATFORM_SPI_MSBFIRST ? MSBFIRST : LSBFIRST;
    SPIMode _dataMode;
    switch (mode)
    {
    case PLATFORM_SPI_MODE_0:
        _dataMode = SPI_MODE0;
        break;
    case PLATFORM_SPI_MODE_1:
        _dataMode = SPI_MODE1;
        break;
    case PLATFORM_SPI_MODE_2:
        _dataMode = SPI_MODE2;
        break;
    case PLATFORM_SPI_MODE_3:
        _dataMode = SPI_MODE3;
        break;
    }
    SPIClassRP2040* _SPI = (SPIClassRP2040*)spi_hw;
    _SPI->beginTransaction(SPISettings(_clock, _bitOrder, _dataMode));
    return PLATFORM_OK;
}
 
/* Write specified number of bytes to an SPI device */
platform_err_code_t platform_spi_write(hw_spi_t spi_hw, uint8_t* txdata, size_t len)
{
    SPIClassRP2040* _SPI = (SPIClassRP2040*)spi_hw;
    _SPI->transfer(txdata, nullptr, len);
    return PLATFORM_OK;
}
 
/* Write and read specified number of bytes over SPI */
platform_err_code_t platform_spi_write_read(hw_spi_t spi_hw, uint8_t* txdata, uint8_t* rxdata, size_t len)
{
    SPIClassRP2040* _SPI = (SPIClassRP2040*)spi_hw;
    _SPI->transfer(txdata, rxdata, len);
    return PLATFORM_OK;
}