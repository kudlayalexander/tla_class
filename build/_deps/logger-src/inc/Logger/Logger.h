#pragma once

#include <csignal>
#include <syslog.h>

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/chrono.h>

namespace Logger {
    enum class MessagePriority {
        EMERGENCY = LOG_EMERG,
        ALERT = LOG_ALERT,
        CRITICAL = LOG_CRIT,
        ERROR = LOG_ERR,
        WARNING = LOG_WARNING,
        NOTICE = LOG_NOTICE,
        INFO = LOG_INFO,
        DEBUG = LOG_DEBUG
    };

    class Logger {
    private:
        MessagePriority currentPriority = MessagePriority::INFO;

    private:
        explicit Logger() noexcept;

    public:
        Logger(const Logger &) = delete;

        Logger(Logger &&) = delete;

        Logger &operator=(const Logger &) = delete;

        Logger &operator=(Logger &&) = delete;

    public:
        static Logger &getInstance() noexcept;

        ~Logger() noexcept;

        void setCurrentPriority(MessagePriority newPriority) noexcept;

        [[nodiscard]] MessagePriority getCurrentPriority() const noexcept;

        void write(std::string_view log, MessagePriority priority) noexcept;

    private:
        static void onSignalHandler(siginfo_t *, void *);

        static bool isOnlyLoggerSubscribeToSignal(int) noexcept;

//        static void printStack(siginfo_t *info, void *ctx);
    };

    extern Logger &logger;
}

#define LOGGER_COMMON_WRITE(color, priorityStr, priority, moduleName, f, ...)   \
    ::Logger::logger.write(                                                     \
        fmt::format(                                                            \
            fg(color),                                                          \
            FMT_STRING("[{:%F %H:%M:%S}] [" priorityStr "] -> {:s}: {:s}\n"),   \
            std::chrono::floor<std::chrono::milliseconds>(                      \
                std::chrono::system_clock::now()),                              \
            fmt::format(                                                        \
                fmt::emphasis::bold | fg(color),                                \
                FMT_STRING("{:s}"),                                             \
                moduleName),                                                    \
            fmt::format(FMT_STRING(f) __VA_OPT__(,) __VA_ARGS__)                \
        ),                                                                      \
        priority)

#define LOGGER_COMMON_WRITE_EXTENDED(color, priorityStr, priority, moduleName, f, ...)  \
    ::Logger::logger.write(                                                             \
        fmt::format(                                                                    \
            fg(color),                                                                  \
            FMT_STRING(                                                                 \
                "[{:%F %H:%M:%S}] [" priorityStr "] "                                   \
                "[" __FILE_NAME__ "({:s}:{:d})] -> "                                    \
                "{:s}: {:s}\n"),                                                        \
            std::chrono::floor<std::chrono::milliseconds>(                              \
                std::chrono::system_clock::now()),                                      \
            __PRETTY_FUNCTION__,                                                        \
            __LINE__,                                                                   \
            fmt::format(                                                                \
                fmt::emphasis::bold | fg(color),                                        \
                FMT_STRING("{:s}"),                                                     \
                moduleName),                                                            \
            fmt::format(FMT_STRING(f) __VA_OPT__(,) __VA_ARGS__)                        \
        ),                                                                              \
        priority                                                                        \
    )


#define EMERGENCY_MESSAGE_COLOR fmt::color::maroon
#define ALERT_MESSAGE_COLOR fmt::color::dark_orange
#define CRITICAL_MESSAGE_COLOR fmt::color::red
#define ERROR_MESSAGE_COLOR fmt::color::orange_red
#define WARNING_MESSAGE_COLOR fmt::color::yellow
#define NOTICE_MESSAGE_COLOR fmt::color::cornflower_blue
#define INFO_MESSAGE_COLOR fmt::color::green
#define DEBUG_MESSAGE_COLOR fmt::color::medium_aquamarine


#define LOGGER_EMERGENCY(moduleName, ...) LOGGER_COMMON_WRITE_EXTENDED(EMERGENCY_MESSAGE_COLOR, "EMERGENCY", ::Logger::MessagePriority::EMERGENCY, moduleName, __VA_ARGS__)
#define LOGGER_ALERT(moduleName, ...) LOGGER_COMMON_WRITE_EXTENDED(ALERT_MESSAGE_COLOR, "ALERT", ::Logger::MessagePriority::ALERT, moduleName, __VA_ARGS__)
#define LOGGER_CRITICAL(moduleName, ...) LOGGER_COMMON_WRITE_EXTENDED(CRITICAL_MESSAGE_COLOR, "CRITICAL", ::Logger::MessagePriority::CRITICAL, moduleName, __VA_ARGS__)
#define LOGGER_ERROR(moduleName, ...) LOGGER_COMMON_WRITE(ERROR_MESSAGE_COLOR, "ERROR", ::Logger::MessagePriority::ERROR, moduleName, __VA_ARGS__)
#define LOGGER_WARNING(moduleName, ...) LOGGER_COMMON_WRITE(WARNING_MESSAGE_COLOR, "WARNING", ::Logger::MessagePriority::WARNING, moduleName, __VA_ARGS__)
#define LOGGER_NOTICE(moduleName, ...) LOGGER_COMMON_WRITE(NOTICE_MESSAGE_COLOR, "NOTICE", ::Logger::MessagePriority::NOTICE, moduleName, __VA_ARGS__)
#define LOGGER_INFO(moduleName, ...) LOGGER_COMMON_WRITE(INFO_MESSAGE_COLOR, "INFO", ::Logger::MessagePriority::INFO, moduleName, __VA_ARGS__)
#define LOGGER_DEBUG(moduleName, ...) LOGGER_COMMON_WRITE_EXTENDED(DEBUG_MESSAGE_COLOR, "DEBUG", ::Logger::MessagePriority::DEBUG, moduleName, __VA_ARGS__)

#define LOGGER_SET_PRIORITY(priority) ::Logger::logger.setCurrentPriority(priority)
#define LOGGER_GET_PRIORITY() ::Logger::logger.getCurrentPriority()