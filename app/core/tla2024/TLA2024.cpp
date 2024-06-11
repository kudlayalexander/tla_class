#include "TLA2024.h"

core::TLA2024::TLA2024() {
    i2cFd = 0;
    dr = DR_1600_SPS;
    fsr = FSR_2_048V;
    mux = MUX_0_1;
    mode = MOD_SINGLE;
    os = OS_SINGLE_CONVERSION;
    setConversionTime();
}

bool core::TLA2024::connectToSlave(std::string_view i2cPath, uint8_t i2cAddress) {
    if (i2cFd > 0) {
        close(i2cFd);
    }

    LOGGER_INFO(kModuleName, "Connecting to: '{:s}'", i2cPath);

    i2cFd = open(i2cPath, O_RDWR);

    if (i2cFd < 0) {
        LOGGER_ERROR(kModuleName, "Unable to open file descriptor, path: '{:s}'", i2cPath);
        return 0;
    }
    else {
        if (ioctl(i2cFd, I2C_SLAVE, i2cAddress) < 0) {
            int addressAsInt = int(i2cAddress);
            LOGGER_ERROR(kModuleName, "File descriptor doesn't exist, i2c path: '{:s}', i2c address: {:d}", i2cPath, addressAsInt);
            return 0;
        }

        LOGGER_INFO(kModuleName, "File descriptor successfully opened: '{:s}'", i2cPath);
    }

    return 1;
};

void core::TLA2024::disconnect() {
    if (i2cFd > 0) {
        close(i2cFd);
    }
    LOGGER_INFO(kModuleName, "File descriptor was closed: '{:s}'");
};

void core::TLA2024::setConfiguration(bool continuous) {
    uint16_t config = RESERVED_ALWAYS;

    if (continuous) {
        config |= MOD_CONTINUOUS;   
    } 
    else {
        config |= MOD_SINGLE;
    }

    config |= fsr;
    config |= dr;
    config |= mux;

    config |= OS_SINGLE_CONVERSION;

    writeRegister(REGISTER_POINTER_CONFIGURATION, config);
    writeRegister(REGISTER_POINTER_LOWTHRESH, 0x0000);
    writeRegister(REGISTER_POINTER_HITHRESH, 0x8000);
}

int16_t core::TLA2024::readRaw(uint16_t mux_) {
    setMultiplexerConfig(mux_);
    setConfiguration(false);

    conversionTime = getConversionTime();
    uint16_t regVal;

    do {
        usleep(conversionTime);
        regVal = readRegister(REGISTER_POINTER_CONFIGURATION);

    } while (( regVal & OS_MASK ) == 0);

    uint16_t result = readRegister(REGISTER_POINTER_CONVERSION) >> 4;

    if (result > 0x07FF) {
        result |= 0xF000;
    }
    return (int16_t)result;
}

float core::TLA2024::readVoltage(uint16_t mux_) {
    uint16_t rawVoltage = readRaw(mux_);
    return calculateVoltage(rawVoltage);
}


uint16_t core::TLA2024::readRegister(uint8_t reg) {
    uint8_t buffer[3];

    if (write(i2cFd, &reg, 1) != 1)
        return 0;

    if (read(i2cFd, buffer, 2) != 2)
        return 0;

    return ((buffer[0] << 8) | buffer[1]);
}

int16_t core::TLA2024::writeRegister(uint8_t reg, uint16_t data) {
    uint8_t buffer[3];
    buffer[0] = reg;
    buffer[1] = (uint8_t)(data >> 8);
    buffer[2] = (uint8_t)(data & 0x00FF);

    if (write(i2cFd, buffer, 3) != 3) {
        return -1;
    }

    return 3;
}

uint16_t core::TLA2024::getConversionTime() {
    return conversionTime;
}
 
void core::TLA2024::setConversionTime() {
    switch (dr) {
        case (DR_128_SPS):
            conversionTime = 1000000 / 128 + 5;
            break;
        case (DR_250_SPS):
            conversionTime = 1000000 / 250 + 5;
            break;
        case (DR_490_SPS):
            conversionTime = 1000000 / 490 + 5;
            break;
        case (DR_920_SPS):
            conversionTime = 1000000 / 920 + 5;
            break;
        case (DR_1600_SPS):
            conversionTime = 1000000 / 1600 + 5;
            break;
        case (DR_2400_SPS):
            conversionTime = 1000000 / 2400 + 5;
            break;
        case (DR_3300_SPS):
            conversionTime = 1000000 / 3300 + 5;
            break;
        default:
            conversionTime = 625;
            break;
    }
 
    conversionTime = (uint16_t)(conversionTime * 1.1);
}

void core::TLA2024::setFullScaleRange(uint16_t fsr_) {
    fsr = fsr_;
}
 
uint16_t core::TLA2024::getFullScaleRange() {
    return fsr;
}
 
void core::TLA2024::setDataRate(uint16_t dr_) {
    dr = dr_;
    setConversionTime();
}
 
uint16_t core::TLA2024::getDataRate() {
    return dr;
}
 
void core::TLA2024::setMultiplexerConfig(uint16_t mux_) {
    mux = mux_;
}
 
uint16_t core::TLA2024::getMultiplexerConfig() {
    return mux;
}
 
void core::TLA2024::setOS(uint16_t os_) {
    os = os_;
}
 
uint16_t core::TLA2024::getOS() {
    return os;
}
 
void core::TLA2024::setMode(uint16_t mode_) {
    mode = mode_;
}
 
uint16_t core::TLA2024::getMode() {
    return mode;
}

float core::TLA2024::calculateVoltage(uint16_t voltageBits) {
    uint16_t signMask = 0x800;
    uint16_t numbersMask = 0x7FF;

    bool negative = (signMask & voltageBits) >> 11;
    voltageBits &= numbersMask;

    float voltageReal;
    float maxVoltage;

    switch(this->fsr) {
        case FSR_0_256V:
            maxVoltage = 0.256f;
            break;
        case FSR_0_512V:
            maxVoltage = 0.512f;
            break;
        case FSR_1_024V:
            maxVoltage = 1.024f;
            break;
        case FSR_2_048V:
            maxVoltage = 2.048f;
            break;
        case FSR_4_096V:
            maxVoltage = 4.096f;
            break;
        case FSR_6_144V:
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
