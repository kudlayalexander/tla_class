#include "TLA2024.h"
#include <iostream>
 
int i2cFd;


TLA2024::TLA2024() {
    int fd = 0;
    dr = REGISTER_CONFIG_DR_1600_SPS;
    fsr = REGISTER_CONFIG_FSR_2_048V;
    mux = REGISTER_CONFIG_MUX_0_1;
    mode = REGISTER_CONFIG_MOD_SINGLE;
    os = REGISTER_CONFIG_OS_SINGLE_CONVERSION;
    setConversionTime(dr);
}

bool TLA2024::init(const char* i2cPath, uint8_t i2cAddress) {
    if (this->i2cFd > 0) {
        close(this->i2cFd);
    }
    std::cout << "Connecting to " << i2cPath << std::endl;

    this->i2cFd = open(i2cPath, O_RDWR);

    if (this->i2cFd < 0) {
        std::cout << "Unable to open file descriptor" << std::endl;
        return 0;
    }
    else {
        if (ioctl(this->i2cFd, I2C_SLAVE, i2cAddress) < 0) {
            std::cout << "File descriptor doesn't exist" << std::endl;
            return 0;
        }

        std::cout << "File descriptor successfully opened;" << std::endl;
    }

    return 1;
};
 
void TLA2024::prepareForReading(uint16_t mux, bool continuous) {
    uint16_t config = 0x0003;

    if (continuous) {
        config |= REGISTER_CONFIG_MOD_CONTINUOUS;   
    } 
    else {
        config |= REGISTER_CONFIG_MOD_SINGLE;
    }

    config |= this->fsr;
    config |= this->dr;
    config |= mux;

    config |= REGISTER_CONFIG_OS_SINGLE_CONVERSION;

    writeRegister(REGISTER_POINTER_CONFIGURATION, config);
    writeRegister(REGISTER_POINTER_LOWTHRESH, 0x0000);
    writeRegister(REGISTER_POINTER_HITHRESH, 0x8000);
}

int16_t TLA2024::readAdc(uint16_t mux) {
    prepareForReading(mux, false);

    conversionTime = getConversionTime();
    uint16_t regVal ;

    do {
        usleep(conversionTime);
        regVal = readRegister(REGISTER_POINTER_CONFIGURATION);

    } while (( regVal & REGISTER_CONFIG_OS_MASK ) == 0);

    uint16_t result = readRegister(REGISTER_POINTER_CONVERSION) >> 4;

    if (result > 0x07FF) {
        result |= 0xF000;
    }
    return (int16_t)result;
}


uint16_t TLA2024::readRegister(uint8_t reg) {
    if (write(this->i2cFd, &reg, 1) != 1)
        return 0;

    if (read(this->i2cFd, this->buffer, 2) != 2)
        return 0;

    return ((buffer[0] << 8) | buffer[1]);
}

int16_t TLA2024::writeRegister(uint8_t reg, uint16_t data) {
    this->buffer[0] = reg;
    this->buffer[1] = (uint8_t)(data >> 8);
    this->buffer[2] = (uint8_t)(data & 0x00FF);

    if (write(this->i2cFd, this->buffer, 3) != 3) {
        return -1;
    }

    return 3;
}

uint16_t TLA2024::getConversionTime() {
    return conversionTime;
}

// Conversions in the TLA202x settle within a single cycle,
// which means the conversion time equals 1 / DR.
 
void TLA2024::setConversionTime(uint16_t dr) {
    switch (dr) {
        case (REGISTER_CONFIG_DR_128_SPS):
            conversionTime = 1000000 / 128 + 5;
            break;
        case (REGISTER_CONFIG_DR_250_SPS):
            conversionTime = 1000000 / 250 + 5;
            break;
        case (REGISTER_CONFIG_DR_490_SPS):
            conversionTime = 1000000 / 490 + 5;
            break;
        case (REGISTER_CONFIG_DR_920_SPS):
            conversionTime = 1000000 / 920 + 5;
            break;
        case (REGISTER_CONFIG_DR_1600_SPS):
            conversionTime = 1000000 / 1600 + 5;
            break;
        case (REGISTER_CONFIG_DR_2400_SPS):
            conversionTime = 1000000 / 2400 + 5;
            break;
        case (REGISTER_CONFIG_DR_3300_SPS):
            conversionTime = 1000000 / 3300 + 5;
            break;
        default:
            conversionTime = 625;
            break;
    }
 
    conversionTime = (uint16_t)(conversionTime * 1.1);
}

void TLA2024::setFullScaleRange(uint16_t fsr_) {
    fsr = fsr_;
}
 
uint16_t TLA2024::getFullScaleRange() {
    return fsr;
}
 
void TLA2024::setDataRate(uint16_t dr_) {
    dr = dr_;
}
 
uint16_t TLA2024::getDataRate() {
    return dr;
}
 
void TLA2024::setMultiplexerConfig(uint16_t mux_) {
    mux = mux_;
}
 
uint16_t TLA2024::getMultiplexerConfig() {
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

float TLA2024::calculateVoltage(uint16_t voltageBits) {
    if (voltageBits == 0x800) return 0;

    uint16_t signMask = 0x800;
    uint16_t numbersMask = 0x7FF;

    bool negative = (signMask & voltageBits) >> 11;
    voltageBits &= numbersMask;

    float voltageReal;
    float maxVoltage;

    switch(this->fsr) {
        case REGISTER_CONFIG_FSR_0_256V:
            maxVoltage = 0.256f;
            break;
        case REGISTER_CONFIG_FSR_0_512V:
            maxVoltage = 0.512f;
            break;
        case REGISTER_CONFIG_FSR_1_024V:
            maxVoltage = 1.024f;
            break;
        case REGISTER_CONFIG_FSR_2_048V:
            maxVoltage = 2.048f;
            break;
        case REGISTER_CONFIG_FSR_4_096V:
            maxVoltage = 4.096f;
            break;
        case REGISTER_CONFIG_FSR_6_144V:
            maxVoltage = 6.144f;
            break;
        default:
            maxVoltage = 2.048f;
            break;
    }

    voltageReal = voltageBits * maxVoltage / 2047;

    if (negative) {
        voltageReal = voltageReal * (-1.0f);
    }

    return voltageReal;
}
