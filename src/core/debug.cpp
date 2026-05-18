#include "debug.h"
#include <regex>
#include <filesystem>
#include <iostream>
#include <format>

#ifdef WINDOWS
#include <Windows.h>
#endif

std::string_view Debug::get_prefix(MessageTypes type)
{
#ifdef WINDOWS
	switch (type)
	{
	case MessageTypes::ePrint:
		return "";
	case MessageTypes::eOk:
		return "\x1B[32mOK: \033[0m";
	case MessageTypes::eInfo:
		return "\x1B[34mINFO: \033[0m";
	case MessageTypes::eWarning:
		return "\x1B[33m~WARNING: \033[0m";
	case MessageTypes::ePlease:
		return "\x1B[35m~PLEASE: \033[0m";
	case MessageTypes::eError:
		return "\x1B[31m!ERROR: \033[0m";
	case MessageTypes::eFatal:
		return "\x1B[31m!FATAL: \033[0m";
	default:
		return "!UNKNOWN: ";
	}
#else
	switch (type)
	{
	case MessageTypes::ePrint:
		return "";
	case MessageTypes::eOk:
		return "OK: ";
	case MessageTypes::eInfo:
		return "INFO: ";
	case MessageTypes::eWarning:
		return "~WARNING: ";
	case MessageTypes::eError:
		return "!ERROR: ";
	case MessageTypes::eFatal:
		return "!FATAL: ";
	default:
		return "!UNKNOWN: ";
	}
#endif
}

LONG WINAPI seh_unhandled_filter(_EXCEPTION_POINTERS* pExceptionInfo)
{
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == 0xE0'6D'73'63)
		return EXCEPTION_CONTINUE_SEARCH;

	std::string msg = "SEH Exception (Crash) caught!\n";
	msg += "Error Code: " + std::to_string(pExceptionInfo->ExceptionRecord->ExceptionCode) + "\n";

	Debug::winApiWindowShow("str_error", msg.c_str());
	Debug::fatalErrorMessage(msg.c_str());
	Debug::log.close();

	return EXCEPTION_EXECUTE_HANDLER;
}

static void seh_translator(unsigned int code, EXCEPTION_POINTERS*)
{
	char buf[64];
	snprintf(buf, sizeof(buf), "SEH Exception: 0x%08X", code);
	throw std::runtime_error(buf);
}

void Debug::initialize(const std::string& command_line)
{
	s_catch_exceptions = true;
	s_error_fatal	   = true;
	_command_line = command_line;
	std::set_terminate(cpp_terminate_handler);
	SetUnhandledExceptionFilter(seh_unhandled_filter);
#ifdef _WIN32
	_set_se_translator(seh_translator);
#endif
}

void Debug::initLogFile()
{
	static bool init{ false };
	if (init)
		return;

	init		  = true;
	auto dir_logs = Core::get().currentPath() / "logs";

	if (!std::filesystem::exists(dir_logs))
		std::filesystem::create_directories(dir_logs);

	log.open(dir_logs / "log", ".txt", true);
}

void Debug::fatalErrorMessage(std::string message)
{
	log.writeText(message);
	log.close();
	std::cerr << message << std::endl;
}

std::string Debug::pretty_stacktrace()
{
	return "(Stacktrace disabled for compatibility)";
}
