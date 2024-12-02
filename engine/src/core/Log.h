#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

namespace Comet
{

    class Log
    {
    public:
        Log() = delete;
        Log(const Log &) = delete;
        Log &operator=(const Log &) = delete;

        static void init();
        static std::shared_ptr<spdlog::logger> &getLogger();

    private:
        static std::shared_ptr<spdlog::logger> s_instance;
    };

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(Comet::Log::getLogger(), __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Comet::Log::getLogger(), __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(Comet::Log::getLogger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(Comet::Log::getLogger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(Comet::Log::getLogger(), __VA_ARGS__)

}