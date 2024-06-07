#include <fcntl.h>

#include "law/File/OpenOption.h"

namespace law::file {
    OpenOption OpenOption::operator&(const OpenOption &of) const noexcept {
        return {getAsIntegral() & of.getAsIntegral()};
    }

    OpenOption OpenOption::operator|(const OpenOption &of) const noexcept {
        return {getAsIntegral() | of.getAsIntegral()};
    }

    Append::Append() noexcept: OpenOption(O_APPEND) {}

    Async::Async() noexcept: OpenOption(O_ASYNC) {}

    CloseOnExec::CloseOnExec() noexcept: OpenOption(O_CLOEXEC) {}

    Create::Create() noexcept: OpenOption(O_CREAT) {}

    Direct::Direct() noexcept: OpenOption(O_DIRECT) {}

    Directory::Directory() noexcept: OpenOption(O_DIRECTORY) {}

    Dsync::Dsync() noexcept: OpenOption(O_DSYNC) {}

    ExitIfExist::ExitIfExist() noexcept: OpenOption(O_EXCL) {}

    LargeFile::LargeFile() noexcept: OpenOption(O_LARGEFILE) {}

    NoAtime::NoAtime() noexcept: OpenOption(O_NOATIME) {}

    NoCTty::NoCTty() noexcept: OpenOption(O_NOCTTY) {}

    NoFollow::NoFollow() noexcept: OpenOption(O_NOFOLLOW) {}

    Nonblock::Nonblock() noexcept: OpenOption(O_NONBLOCK) {}

    NoDelay::NoDelay() noexcept: OpenOption(O_NDELAY) {}

    Sync::Sync() noexcept: OpenOption(O_SYNC) {}

    TempFile::TempFile() noexcept: OpenOption(O_TMPFILE) {}

    Truncate::Truncate() noexcept: OpenOption(O_TRUNC) {}

    ReadOnly::ReadOnly() noexcept: OpenOption(O_RDONLY) {}

    WriteOnly::WriteOnly() noexcept: OpenOption(O_WRONLY) {}

    ReadWrite::ReadWrite() noexcept: OpenOption(O_RDWR) {}
}