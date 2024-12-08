#include "Log.h"
#include "../pch.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

namespace Comet
{
    std::shared_ptr<spdlog::logger> Log::s_instance;

    std::shared_ptr<spdlog::logger> &Log::getLogger()
    {
        assert(s_instance && "Logger not initialized");
        return s_instance;
    }

    void Log::init()
    {
        s_instance = spdlog::stdout_color_mt<spdlog::async_factory>("async_logger");
        s_instance->set_level(spdlog::level::trace);
        s_instance->set_pattern("Comet Engine: %^%H:%M:%S:%e [%p-%t] [%1!L] [%s] - %v%$");
    }
}