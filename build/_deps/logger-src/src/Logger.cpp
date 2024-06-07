#include <law/Signal/Signal.h>

//#define BACKWARD_HAS_DWARF 1
//#define BACKWARD_HAS_LIBUNWIND 1
//
//#include <backward-cpp/backward.hpp>

#include "Logger/Logger.h"

namespace Logger {
    Logger &logger = Logger::getInstance();

    static void (*stdoutWriteFunc)(std::string_view, MessagePriority);

    void dummyWrite(std::string_view, MessagePriority) noexcept {}

    void stdoutWrite(std::string_view message, MessagePriority prio) noexcept {
        if (prio < MessagePriority::NOTICE) {
            static_cast<void>(::write(STDERR_FILENO, message.data(), message.size()));
        } else {
            static_cast<void>(::write(STDOUT_FILENO, message.data(), message.size()));
        }
    }

    Logger::Logger() noexcept {
        if (getppid() == 1) {
            stdoutWriteFunc = &dummyWrite;
        } else {
            stdoutWriteFunc = &stdoutWrite;
        }

        openlog(program_invocation_name, LOG_PID, LOG_USER);
        law::signal::SignalHandler::getInstance().subscribe(
                [](int, siginfo_t *si, void *uctx) {
                    onSignalHandler(si, uctx);
                },
                SIGBUS, SIGFPE, SIGILL, SIGSEGV, SIGTRAP, SIGSTKFLT, SIGIOT, SIGKILL, SIGXCPU,
                SIGINT, SIGQUIT, SIGHUP, SIGTERM, SIGABRT, SIGCONT, SIGSTOP, SIGTSTP, SIGSYS
        );
    }

    Logger &Logger::getInstance() noexcept {
        static Logger logger;
        return logger;
    }

    Logger::~Logger() noexcept {
        closelog();
    }

    void Logger::setCurrentPriority(MessagePriority newPriority) noexcept {
        currentPriority = newPriority;
    }

    MessagePriority Logger::getCurrentPriority() const noexcept {
        return currentPriority;
    }

    void Logger::write(std::string_view log, MessagePriority priority) noexcept {
        using namespace std::chrono;
        if (priority <= currentPriority) {
            syslog(static_cast<int>(priority), "%s", log.data());
            stdoutWriteFunc(log, priority);
        }
    }

    void Logger::onSignalHandler(siginfo_t *si, void *) {
        static constexpr char moduleName[] = "LoggerModule";
        switch (si->si_signo) {
            case SIGBUS:
            case SIGFPE:
            case SIGILL:
            case SIGSEGV:
            case SIGTRAP:
            case SIGSTKFLT:
            case SIGXCPU:
                LOGGER_CRITICAL(
                        moduleName,
                        "Emergency signal '{:s}' (signum: {:d})",
                        strsignal(si->si_signo),
                        si->si_signo);
//                printStack(si, uctx);
                std::exit(EXIT_FAILURE);
            default:
                LOGGER_INFO(
                        moduleName,
                        "Caught signal '{:s}' (signum: {:d})",
                        strsignal(si->si_signo),
                        si->si_signo);

                if (isOnlyLoggerSubscribeToSignal(si->si_signo)) {
                    std::exit(EXIT_SUCCESS);
                }

                break;
        }
    }

    bool Logger::isOnlyLoggerSubscribeToSignal(int signal) noexcept {
        return (law::signal::SignalHandler::getInstance().getSignalSubscribersCount(signal) == 1);
    }

//    void Logger::printStack(siginfo_t *info, void *ctx) {
//        auto uctx = static_cast<ucontext_t *>(ctx);

//        backward::StackTrace st;
//        if (auto errorAddress = reinterpret_cast<void *>(uctx->uc_mcontext.arm_pc); errorAddress) {
//            st.load_from(errorAddress, 32, reinterpret_cast<void *>(uctx),
//                         info->si_addr);
//        } else {
//            st.load_here(32, reinterpret_cast<void *>(uctx), info->si_addr);
//        }
//
//        backward::Printer printer;
//        printer.address = true;
//        printer.print(st, stderr);

//        psiginfo(info, nullptr);
//    }
}