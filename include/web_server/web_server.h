#ifndef WEB_SERVER_H_
#define WEB_SERVER_H_

#pragma once

#ifdef _DEBUG
#define WEB_SERVER_DEBUG
#endif

#define STRICT
#define WIN32_LEAN_AND_MEAN

#ifdef WEB_SERVER_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif // WEB_SERVER_DEBUG

#define NAMESPACE web_server
#define NAMESPACE_BEGIN namespace NAMESPACE {
#define NAMESPACE_END }

#endif // WEB_SERVER_H_