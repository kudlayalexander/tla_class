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
#define I2C_ADDRESS_1 (0x48) // GND
#define I2C_ADDRESS_2 (0x49) // VDD
#define I2C_ADDRESS_3 (0x4B) // SCL
 
 
//////// REGISTER POINTER CONFIGURATION
 
// The host can read the conversion data register from the TLA202x,
// or read and write the configuration register from and to the TLA202x,
// respectively. The value of the register pointer(RP), which is the first
// data byte after the slave address of a write transaction (R/W = 0),
// determines the register that is addressed. 
// Register data are sent with the most significant byte first, 
// followed by the least significant byte.
// Within each byte, data are transmitted most significant bit first.
 
#define RESITER_POINTER_MASK (0x03)
 
#define REGISTER_POINTER_CONVERSION (0x00) // ConversionDataRegister(RP = 00h)[reset= 0000h]
#define REGISTER_POINTER_CONFIGURATION (0x01) // ConfigurationRegister(RP = 01h)[reset= 8583h]
#define REGISTER_POINTER_LOWTHRESH (0x02) // Low threshold
#define REGISTER_POINTER_HITHRESH (0x03)  // High threshold
 
//////// REGISTER CONFIG CONFIGURATION
 
#define REGISTER_CONFIG_OS_MASK (0X8000) // bit 15
 
#define REGISTER_CONFIG_OS_BUSY (0x0000)
#define REGISTER_CONFIG_OS_NOT_BUSY (0x8000)
#define REGISTER_CONFIG_OS_SINGLE_CONVERSION (0x8000)
 
/// Input Multiplexer Configuration(TLA2024only)
/// These bits configure the input multiplexer.
#define REGISTER_CONFIG_MUX_MASK (0x7000) // bit 14-12
 
#define REGISTER_CONFIG_MUX_0_1 (0x0000) // AIN_P = AIN_0 & AIN_N = AIN_1 (default)
#define REGISTER_CONFIG_MUX_0_3 (0x1000) // AIN_P = AIN_0 & AIN_N = AIN_3
#define REGISTER_CONFIG_MUX_1_3 (0x2000) // AIN_P = AIN_1 & AIN_N = AIN_3
#define REGISTER_CONFIG_MUX_2_3 (0x3000) // AIN_P = AIN_2 & AIN_N = AIN_3
 
#define REGISTER_CONFIG_MUX_0_GND (0x4000) // AIN_P = AIN_0 & AIN_N = GND
#define REGISTER_CONFIG_MUX_1_GND (0x5000) // AIN_P = AIN_1 & AIN_N = GND
#define REGISTER_CONFIG_MUX_2_GND (0x6000) // AIN_P = AIN_2 & AIN_N = GND
#define REGISTER_CONFIG_MUX_3_GND (0x7000) // AIN_P = AIN_3 & AIN_N = GND
 
/// Programmable Gain Amplifier Configuration(TLA2022 and TLA2024 Only)
// These bits set the FSR of the programm able gain amplifier.
#define REGISTER_CONFIG_PGA_MASK (0x0E00) // bit 11-9
 
#define REGISTER_CONFIG_FSR_6_144V (0x0000) // 000 : FSR= +-6.144V
#define REGISTER_CONFIG_FSR_4_096V (0x0200) // 001 : FSR= +-4.096V
#define REGISTER_CONFIG_FSR_2_048V (0x0400) // 010 : FSR= +-2.048V (default)
#define REGISTER_CONFIG_FSR_1_024V (0x0600) // 011 : FSR= +-1.024V
#define REGISTER_CONFIG_FSR_0_512V (0x0800) // 100 : FSR= +-0.512V
#define REGISTER_CONFIG_FSR_0_256V (0x0A00) // 101 : FSR= +-0.256V
// #define REGISTER_CONFIG_PGA_0_256V (0x0C00) // 110 : FSR= +-0.256V
// #define REGISTER_CONFIG_PGA_0_256V (0x0E00) // 111 : FSR= +-0.256V
 
/// Operating Mode 
/// This bit controls the operating mode.
#define REGISTER_CONFIG_MOD_MASK (0x0100) // bit 8
 
#define REGISTER_CONFIG_MOD_CONTINUOUS (0x0000) // Continuous-conversion mode
#define REGISTER_CONFIG_MOD_SINGLE (0x0100) // 1 : Single-shot conversion mode or power-down state (default)
 
 
/// Data Rate
/// These bits control the data rate setting
#define REGISTER_CONFIG_DR_MASK (0x00E0) // bit 7-5
 
#define REGISTER_CONFIG_DR_128_SPS (0x0000) // 000 : DR = 128 SPS
#define REGISTER_CONFIG_DR_250_SPS (0x0020) // 001 : DR = 250 SPS
#define REGISTER_CONFIG_DR_490_SPS (0x0040) // 010 : DR = 490 SPS
#define REGISTER_CONFIG_DR_920_SPS (0x0060) // 011 : DR = 920 SPS
#define REGISTER_CONFIG_DR_1600_SPS (0x0080) // 100 : DR = 1600 SPS (default)
#define REGISTER_CONFIG_DR_2400_SPS (0x00A0) // 101 : DR = 2400 SPS
#define REGISTER_CONFIG_DR_3300_SPS (0x00C0) // 110 : DR = 3300 SPS
// #define REGISTER_CONFIG_DR_3300_SPS (0x00E0) // 111 : DR = 3300 SPS
 
/// Reserved 
/// Always write 03h
#define REGISTER_CONFIG_RESERVED_MASK (0x001F) // bit 4-0
#define REGISTER_CONFIG_RESERVED_ALWAYS (0x0003)

#define REGISTER_CONFIGURATION_DEFAULT (0x8583)
 
/// If the host initiates contact with the TLA202x but subsequently 
/// remains idle for 25 ms before completing a command, the TLA202x 
/// interface is reset
#define TIMEOUT_RECOVER_TIME (25)
 

#ifndef TLA2024_H
#define TLA2024_H

class TLA2024 {
    protected:
        int i2c_fd;
        uint16_t dr;
        uint16_t fsr;
        uint16_t mux;
        uint16_t mode;
        uint16_t os;
        uint16_t conversion_time;
 
    public:
    
        TLA2024();

        bool init(const char *i2c_path = I2C_DEFAULT_PATH, uint8_t i2c_address = I2C_ADDRESS_1);
        
        /*  
        Read the conversion data register or configuration register
        by using two I2C communication frames.
    
        The first frame is an I2C sendBytes operation where 
        the R/W bit at the end of the slave address is 0 to indicate a sendBytes.
        In this frame, the host sends the register pointer that points
        to the register to receiveBytes from. 
    
        The second frame is an I2C receiveBytes operation where the R/W bit
        at the end of the slave address is 1 to indicate a receiveBytes.
        The TLA202x transmits the contents of the register in this second I2C frame.
        The master can terminate the transmission after any byte by not acknowledging 
        or issuing a START or STOP condition.
    
        When repeatedly reading the same register, the register pointer
        does not need to be written every time again because the TLA202x store
        the value of the register pointer until a sendBytes operation modifies the value. 
        */
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

        // void writeAdc();
    private:
        uint8_t address;
        int16_t writeRegister(uint8_t reg, uint16_t value);
        uint16_t readRegister(uint8_t reg);
        uint8_t buffer[3];
};

#endif
