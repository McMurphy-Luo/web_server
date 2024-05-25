#ifndef WEB_SERVER_HTTP_RESPONSE_H_
#define WEB_SERVER_HTTP_RESPONSE_H_

#pragma once

#include "web_server/web_server.h"
#include "web_server/u8string.h"
#include "stream_writer.h"
#include "ref_counter.h"

NAMESPACE_BEGIN

class HttpResponse
  : public RefCounter<ThreadUnsafeCounter>
{
public:
  explicit HttpResponse();

protected:

public:
  void SetStatuCode(int code);
  void SetHeader(const U8String& header, const U8String& value);
  void SetHeader(U8String&& header, U8String&& value);
  void SetHeader(const U8String& header, U8String&& value);
  void SetHeader(U8String&& header, const U8String& value);
  void ClearHeader(const U8String& header);
  void SetBody(U8String&& body);
  void SetBody(const U8String& body);
  void SetBody(char* buf, size_t buf_size);

private:

};

NAMESPACE_END

#endif // WEB_SERVER_HTTP_RESPONSE_H_
