#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};



// Core log macros
#if RING_DEBUG || RING_RELEASE
#define RING_INFO(...)     ::Log::GetLogger()->info(__VA_ARGS__)
#define RING_TRACE(...)    ::Log::GetLogger()->trace(__VA_ARGS__)
#define RING_WARN(...)     ::Log::GetLogger()->warn(__VA_ARGS__)
#define RING_ERROR(...)    ::Log::GetLogger()->error(__VA_ARGS__)
#define RING_FATAL(...)    ::Log::GetLogger()->fatal(__VA_ARGS__)

#else	
#define RING_INFO(...)     
#define RING_TRACE(...)    
#define RING_WARN(...)     
#define RING_ERROR(...)    
#define RING_FATAL(...)    
#endif

#ifdef RING_DEBUG
#define RING_ENABLE_ASSERTS
#endif

#ifdef RING_ENABLE_ASSERTS
#define RING_ASSERT(x, ...) { if(!(x)) { RING_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define RING_ASSERT(x, ...)
#endif