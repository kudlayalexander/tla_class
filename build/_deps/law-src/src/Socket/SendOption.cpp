#include <sys/socket.h>

#include "law/Socket/SendOption.h"

namespace law::socket {
    SendOption SendOption::operator|(const SendOption &wo) const noexcept {
        return {getAsIntegral() | wo.getAsIntegral()};
    }

    Confirm::Confirm() noexcept: SendOption(MSG_CONFIRM) {}

    DontRoute::DontRoute() noexcept: SendOption(MSG_DONTROUTE) {}

    Eor::Eor() noexcept: SendOption(MSG_EOR) {}

    More::More() noexcept: SendOption(MSG_MORE) {}

    NoSignal::NoSignal() noexcept: SendOption(MSG_NOSIGNAL) {}
}