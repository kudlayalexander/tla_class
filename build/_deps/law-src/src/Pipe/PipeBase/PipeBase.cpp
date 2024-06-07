#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <paths.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "law/Pipe/Base/PipeBase.h"
#include "law/RetCode/RetCode.h"

namespace law::pipe::detail {
    PipeBase::PipeBase(PipeType type, std::string_view cmd) {
        if (!init(type, cmd)) {
            throw std::system_error(errno, std::system_category());
        }
    }

    PipeBase::~PipeBase() noexcept {
        close();
    }

    Status PipeBase::init(PipeType type, std::string_view cmd) noexcept {
        pipeType = type;

        if (::pipe(fd) != 0) {
            return fromErrno();
        }

        pid_t pid;
        switch (pid = vfork()) {
        case -1:
            return fromErrno();
        case 0:
            if (pipeType == PipeType::READ) {
                ::close(fd[0]);
                int tpdes1 = fd[1];
                if (tpdes1 != STDOUT_FILENO) {
                    dup2(tpdes1, STDOUT_FILENO);
                    ::close(tpdes1);
                    tpdes1 = STDOUT_FILENO;
                }
            } else {
                if (fd[0] != STDIN_FILENO) {
                    dup2(fd[0], STDIN_FILENO);
                    ::close(fd[0]);
                }
                ::close(fd[1]);
            }
            execl(_PATH_BSHELL, "sh", "-c", cmd.data(), (char *)NULL);
            _exit(127);
        }
        childPid = pid;
        return {};
    }

    void PipeBase::close() noexcept {
        while (waitpid(childPid, nullptr, 0) == -1) {
            if (errno != EINTR) {
                break;
            }
            closeDescriptors();
        } 
    }
    
    Status PipeBase::write(std::string_view buffer) noexcept {
        if (pipeType == PipeType::READ) {
            return fromErrno();
        }
        return (::write(fd[1], buffer.data(), buffer.size()) == -1) ? fromErrno() : Status();
    }

    [[nodiscard]] PipeSize PipeBase::getPipeSize() const noexcept {
        const auto pipeSize = fcntl(fd[0], F_GETPIPE_SZ);
        if (pipeSize == -1) {
            return fromErrno();
        }
        return pipeSize;
    }

    ReadCountRet PipeBase::read(void *extBuffer, std::size_t size) const noexcept {
        if (pipeType == PipeType::WRITE) {
            return fromErrno();
        }
        
        const auto recBytes = ::read(fd[0], extBuffer, size);
        if (recBytes == -1) {
            return fromErrno();
        }

        return recBytes;
    }

    int PipeBase::getOutputFileDescriptor() const noexcept {
        return fd[1];
    }

    int PipeBase::getInputFileDescriptor() const noexcept {
        return fd[0];
    }

    void PipeBase::closeDescriptors() noexcept {
        if (fd[1] != -1) {
            ::close(fd[1]);
        }

        if (fd[0] != -1) {
            ::close(fd[0]);
        }
    }

    pid_t PipeBase::getChildPid() const noexcept {
        return childPid;
    }
}