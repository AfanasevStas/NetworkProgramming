#pragma once
#include <string>

#ifdef WSERROR_EXPORTS
#define WSERROR_API __declspec(dllexport)
#else
#define WSERROR_API __declspec(dllimport)
#endif

WSERROR_API std::string GetWSErrorMessage(int errorCode);