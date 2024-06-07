#pragma once

#include "law/Socket/CAN/CanDevice.h"

namespace law::socket::can::isotp {
    class ISOTPDescriptor final : public CanDevice {
    public:
        using Address = std::uint32_t;

    private:
        enum class AddressMode {
            BIT_11,
            BIT_29
        };

    public:
        ISOTPDescriptor() noexcept = default;

        ISOTPDescriptor(Address sourceAddress, Address destinationAddress, std::string_view ifName);

        ISOTPDescriptor &setSourceAddress(Address sourceAddress) noexcept;

        ISOTPDescriptor &setDestinationAddress(Address destinationAddress) noexcept;

    private:
        static AddressMode getAddressModeByValue(Address address) noexcept;
    };
}