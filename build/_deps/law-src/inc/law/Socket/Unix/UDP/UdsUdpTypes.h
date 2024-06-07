#pragma once

#include "law/Types.h"
#include "law/Socket/Unix/UnixAddress/UnixAddress.h"

namespace law::socket::uds::udp {
    struct ReadResult {
        sockaddr_un clientAddress;
        std::size_t recBytes;
    };

    using ReadResultRet = ErrorCode::CustomError<ReadResult>;

    struct ClientPayload {
        Blob blob;
        UnixAddress address;
    };

    struct ClientPayloadView {
        BlobView blob;
        UnixAddress address;
    };

    using ClientPayloadRet = ErrorCode::CustomError<ClientPayload>;
    using ClientPayloadViewRet = ErrorCode::CustomError<ClientPayloadView>;
}