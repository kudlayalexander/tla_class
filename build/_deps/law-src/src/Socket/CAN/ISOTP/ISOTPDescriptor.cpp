#include "law/Socket/CAN/ISOTP/ISOTPDescriptor.h"

namespace law::socket::can::isotp {
    ISOTPDescriptor::ISOTPDescriptor(Address sourceAddress, Address destinationAddress, std::string_view ifName) :
            CanDevice(ifName) {
        setSourceAddress(sourceAddress).setDestinationAddress(destinationAddress);
    }

    ISOTPDescriptor &ISOTPDescriptor::setSourceAddress(Address sourceAddress) noexcept {
        getNativeAddress().can_addr.tp.tx_id = (
                (getAddressModeByValue(sourceAddress) == AddressMode::BIT_29) ?
                (sourceAddress | CAN_EFF_FLAG) :
                sourceAddress
        );
        return *this;
    }

    ISOTPDescriptor &ISOTPDescriptor::setDestinationAddress(Address destinationAddress) noexcept {
        getNativeAddress().can_addr.tp.rx_id = (
                (getAddressModeByValue(destinationAddress) == AddressMode::BIT_29) ?
                (destinationAddress | CAN_EFF_FLAG) :
                destinationAddress
        );
        return *this;
    }

    ISOTPDescriptor::AddressMode ISOTPDescriptor::getAddressModeByValue(Address address) noexcept {
        static constexpr std::uint16_t kBit11MaxAddressValue = 0b11111111111;
        return (address > kBit11MaxAddressValue) ? AddressMode::BIT_29 : AddressMode::BIT_11;
    }
}