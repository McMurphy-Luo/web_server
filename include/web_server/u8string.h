#ifndef WEB_SERVER_U8STRING_H_
#define WEB_SERVER_U8STRING_H_

#pragma once

#include "web_server/web_server.h"
#include <string>

NAMESPACE_BEGIN
#ifdef __cpp_lib_char8_t
using U8String = std::u8string;
#else
using U8String = std::string;
#endif
NAMESPACE_END

#endif // WEB_SERVER_U8STRING_H_
