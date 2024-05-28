#include "TLA2024.h"
#include <iostream>
 
int i2c_fd;
 
static int connectToI2C(const char* i2c_path, uint8_t i2c_address) {
    i2c_fd = open(i2c_path, O_RDWR);
 
    if (i2c_fd == -1) {
        std::cout << "Unable to connect to i2c" << std::endl;
        return -1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, i2c_address) < 0) {
        std::cout << "Error when connecting to I2C\n" << std::endl;
        return -1;
    }
    
    unsigned char buf[1] = {};
    buf[0] = 0;
    if (write(i2c_fd, buf, 1) != 1) {
        std::cout << "Error when trying to write to I2C" << std::endl;;
        return -1;
    }
    std::cout << "Device connected on address " << std::hex << std::uppercase << i2c_address << std::nouppercase << std::dec << std::endl;
    return 0;
};
 
static void disconnectFromI2C() {
    close(i2c_fd);
};
 
static int writeRegister(const char* i2c_path, uint8_t i2c_address, uint8_t register_pointer, uint16_t value) {  
    if (connectToI2C(i2c_path, i2c_address) < 0) {
        return -1;
    }
 
    unsigned char buf[3] = {register_pointer, (uint8_t)(value >> 8), (uint8_t)(0xFF)};
   
    uint16_t returned = write(*i2c_path, buf, 3);

    if (returned == -1) {
        std::cout << "Error during writing" << std::endl;
        return -1;
    }
 
    disconnectFromI2C();
    return 0;
};
 
static uint16_t readRegister(const char* i2c_path, uint8_t i2c_address, uint8_t register_pointer) {
    // pridumat vmesto null
    // if (connectToI2C(i2c_path, i2c_address) < 0) {
    //     return NULL;
    // }
    if (connectToI2C(i2c_path, i2c_address) < 0) {
        std::cout << "Error when connecting  during readRegister()" << std::endl;           
    }

    uint16_t returned;
    unsigned char writeBuf[1] = { register_pointer };
    returned = write(*i2c_path, writeBuf, 1);
    if (returned == -1) {
        std::cout << "Error during writing" << std::endl;
    }

    usleep(500000);
 
    unsigned char readBuf[2] = { };
    returned = read(*i2c_path, readBuf, 2);
    if (returned == -1) {
        std::cout << "Error during reading" << std::endl;
    }
    
    usleep(500000);
 
    disconnectFromI2C();
 
    uint16_t value = ((readBuf[0] << 8) | (readBuf[1]));
    return value;
};
 
TLA2024::TLA2024(const char *i2c_path_, uint8_t i2c_address_) {
    i2c_path = i2c_path_;
    i2c_address = i2c_address_;
    dr = REGISTER_CONFIG_DR_1600_SPS;
    pga = REGISTER_CONFIG_PGA_2_048V;
    mux = REGISTER_CONFIG_MUX_0_1;
    mode = REGISTER_CONFIG_MOD_SINGLE;
    os = REGISTER_CONFIG_OS_SINGLE_CONVERSION;
    conversion_time = getConversionTime();
}
 
void TLA2024::setPGA(uint16_t pga_) {
    pga = pga_;
}
 
uint16_t TLA2024::getPGA() {
    return pga;
}
 
void TLA2024::setDR(uint16_t dr_) {
    dr = dr_;
}
 
uint16_t TLA2024::getDR() {
    return dr;
}
 
void TLA2024::setMUX(uint16_t mux_) {
    mux = mux_;
}
 
uint16_t TLA2024::getMUX() {
    return mux;
}
 
void TLA2024::setOS(uint16_t os_) {
    os = os_;
}
 
uint16_t TLA2024::getOS() {
    return os;
}
 
void TLA2024::setMode(uint16_t mode_) {
    mode = mode_;
}
 
uint16_t TLA2024::getMode() {
    return mode;
}
 
// void TLA2024::setI2CPath(char* i2c_path_) {
//     i2c_path = i2c_path_;
// }
 
// const char* TLA2024::getI2CPath() {
//     return i2c_path;
// }
 
// void TLA2024::setI2CAddress(uint8_t i2c_address_) {
//     i2c_address = i2c_address_;
// }
 
// uint8_t TLA2024::getI2CAddress() {
//     return i2c_address;
// }
 
void TLA2024::setConfigurationRegister() {
    cfg_reg = dr | mode | pga | mux | os;
}
 
uint16_t TLA2024::getConfigurationRegister() {
    return cfg_reg;
}
 
// Conversions in the TLA202x settle within a single cycle,
// which means the conversion time equals 1 / DR.
 
void TLA2024::setConversionTime(uint16_t dr) {
    switch (dr) {
        case (REGISTER_CONFIG_DR_128_SPS):
            conversion_time = 1000000 / 128;
        case (REGISTER_CONFIG_DR_250_SPS):
            conversion_time = 1000000 / 250 + 5;
        case (REGISTER_CONFIG_DR_490_SPS):
            conversion_time = 1000000 / 490 + 5;
        case (REGISTER_CONFIG_DR_920_SPS):
            conversion_time = 1000000 / 920 + 5;
        case (REGISTER_CONFIG_DR_1600_SPS):
            conversion_time = 1000000 / 1600 + 5;
        case (REGISTER_CONFIG_DR_2400_SPS):
            conversion_time = 1000000 / 2400 + 5;
        case (REGISTER_CONFIG_DR_3300_SPS):
            conversion_time = 1000000 / 3300 + 5;
        default:
            conversion_time = 625;
    }
 
    conversion_time = (uint16_t)(conversion_time * 1.1);
}
 
uint16_t TLA2024::getConversionTime() {
    return conversion_time;
}
 
int16_t TLA2024::getLastConversion() {
    conversion_time = getConversionTime();
    usleep(conversion_time);
 
    do
    {
        usleep(10);
    } while (REGISTER_CONFIG_OS_BUSY == (readRegister(i2c_path, i2c_address, cfg_reg) & REGISTER_CONFIG_OS_MASK));
    
    std::cout << "get conversion" << std::endl;

    uint16_t conversion_content = readRegister(i2c_path, i2c_address, REGISTER_POINTER_CONVERSION) >> 4;

    std::cout << std::hex << conversion_content << std::dec << std::endl;
 
    if (conversion_content > 0x07FF) {
        conversion_content |= 0xF000;
    }
 
    return (int16_t)conversion_content;
}
 
/*  
    Read the conversion data register or configuration register
    by using two I2C communication frames.
 
    The first frame is an I2C write operation where 
    the R/W bit at the end of the slave address is 0 to indicate a write.
    In this frame, the host sends the register pointer that points
    to the register to read from. 
 
    The second frame is an I2C read operation where the R/W bit
    at the end of the slave address is 1 to indicate a read.
    The TLA202x transmits the contents of the register in this second I2C frame.
    The master can terminate the transmission after any byte by not acknowledging 
    or issuing a START or STOP condition.
 
    When repeatedly reading the same register, the register pointer
    does not need to be written every time again because the TLA202x store
    the value of the register pointer until a write operation modifies the value. 
*/
 
int16_t TLA2024::readAdc() {
    // set register config
    uint16_t config = getConfigurationRegister();
 
    // write register config
    writeRegister(i2c_path, i2c_address, REGISTER_POINTER_CONFIGURATION, config);
 
    // get last conversion
    int16_t conversion_content = getLastConversion();
    return conversion_content;
}
