#pragma once

#include "law/Types.h"
#include "law/Pipe/PipeTypes.h"

namespace law::pipe::detail {
    class PipeBase {
    private:
        int fd[2] = {-1, -1};
        PipeType pipeType;
        pid_t childPid;

    public:
        PipeBase() noexcept = default;

        PipeBase(PipeType type, std::string_view cmd);

        ~PipeBase() noexcept; 

        Status init(PipeType type, std::string_view cmd) noexcept;

        void close() noexcept;

        Status write(std::string_view buffer) noexcept;

        [[nodiscard]] PipeSize getPipeSize() const noexcept;

        ReadCountRet read(void *extBuffer, std::size_t size) const noexcept;

        int getInputFileDescriptor() const noexcept;

        int getOutputFileDescriptor() const noexcept;

        pid_t getChildPid() const noexcept;

    private:
        void closeDescriptors() noexcept;
    };
} 