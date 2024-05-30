#include <stdint.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define I2C_DEFAULT_PATH "/dev/i2c-2" 

#define REGISTER_CONFIG_DEFAULT (0x8583)

#ifndef TLA2024_H
#define TLA2024_H

class TLA2024 {
    public:
        enum OperationSystem {
            OS_MASK =  0X8000,
            OS_BUSY =  0x0000,
            OS_NOT_BUSY =  0x8000,
            OS_SINGLE_CONVERSION =  0x8000
        };

        enum MultiplexerConfiguration {
            MUX_MASK = 0x7000 ,
            MUX_0_1 = 0x0000,
            MUX_0_3 = 0x1000, 
            MUX_1_3 = 0x2000, 
            MUX_2_3 = 0x3000, 
            MUX_0_GND = 0x4000,
            MUX_1_GND = 0x5000,
            MUX_2_GND = 0x6000,
            MUX_3_GND = 0x7000 
        };

        enum FullScaleRange {
            FSR_MASK = 0x0E00,
            FSR_6_144V = 0x0000,
            FSR_4_096V = 0x0200, 
            FSR_2_048V = 0x0400,
            FSR_1_024V = 0x0600, 
            FSR_0_512V = 0x0800, 
            FSR_0_256V = 0x0A00 
        };

        enum OperatingMode {
            MOD_MASK = 0x0100,
            MOD_CONTINUOUS = 0x0000,
            MOD_SINGLE = 0x0100
        };

        enum DataRate {
            DR_MASK = 0x00E0,
            DR_128_SPS = 0x0000,
            DR_250_SPS = 0x0020,
            DR_490_SPS = 0x0040,
            DR_920_SPS = 0x0060,
            DR_1600_SPS = 0x0080,
            DR_2400_SPS = 0x00A0,
            DR_3300_SPS = 0x00C0
        };
        
        enum I2CAddress {
            I2C_ADDRESS_1 = 0x48,
            I2C_ADDRESS_2 = 0x49,
            I2C_ADDRESS_3 = 0x4B
        };

        TLA2024();

        bool connectToSlave(const char *i2cPath = I2C_DEFAULT_PATH, uint8_t i2cAddress = I2C_ADDRESS_1);
        void disconnect();

        void prepareForReading(uint16_t mux, bool continuous);
        int16_t readAdc(uint16_t mux);

        int16_t getLastConversion();
 
        void setFullScaleRange(uint16_t fsr_);
        uint16_t getFullScaleRange();
 
        void setDataRate(uint16_t dr_);
        uint16_t getDataRate();
 
        void setMultiplexerConfig(uint16_t mux_);
        uint16_t getMultiplexerConfig();
 
        void setOS(uint16_t os_);
        uint16_t getOS();
 
        void setMode(uint16_t mode_);
        uint16_t getMode();
 
        void setConversionTime(uint16_t dr_);
        uint16_t getConversionTime();

        // 12-bit long input with value of voltage in binary
        float calculateVoltage(uint16_t voltageBits);
    private:
        int i2cFd;
        uint16_t dr;
        uint16_t fsr;
        uint16_t mux;
        uint16_t mode;
        uint16_t os;
        uint16_t conversionTime;

        uint8_t address;
        int16_t writeRegister(uint8_t reg, uint16_t value);
        uint16_t readRegister(uint8_t reg);
        uint8_t buffer[3];

        enum RegisterPointer {
            REGISTER_POINTER_MASK = 0x03,
            REGISTER_POINTER_CONVERSION = 0x00,
            REGISTER_POINTER_CONFIGURATION = 0x01,
            REGISTER_POINTER_LOWTHRESH = 0x02,
            REGISTER_POINTER_HITHRESH = 0x03
        };

        enum RegisterConfigReserved {
            RESERVED_MASK = 0x001F,
            RESERVED_ALWAYS = 0x0003
        };
};

#endif