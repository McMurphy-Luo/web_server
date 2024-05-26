#ifndef WEB_SERVER_HTTP_RESPONSE_H_
#define WEB_SERVER_HTTP_RESPONSE_H_

#pragma once

#include "web_server/web_server.h"
#include "web_server/u8string.h"
#include "stream_writer.h"
#include "ref_counter.h"
#include <unordered_map>

NAMESPACE_BEGIN

class HttpResponse
  : public RefCounter<ThreadUnsafeCounter>
{
public:
  explicit HttpResponse();

protected:
  HttpResponse(const HttpResponse&) = delete;
  HttpResponse(HttpResponse&&) = delete;
  HttpResponse& operator=(const HttpResponse&) = delete;
  HttpResponse& operator=(HttpResponse&&) = delete;
  virtual ~HttpResponse() override;

public:
  void SetStatuCode(int code);
  void SetHeader(const U8String& header, const U8String& value);
  void SetHeader(U8String&& header, U8String&& value);
  void SetHeader(const U8String& header, U8String&& value);
  void SetHeader(U8String&& header, const U8String& value);
  void RemoveHeader(const U8String& header);
  U8String::const_pointer StatusBuffer();
  U8String::size_type StatusBufferSize();
  U8String::const_pointer HeaderBuffer();
  U8String::size_type HeaderBufferSize();

private:
  int status_code_ = 0;
  std::unordered_map<U8String, U8String> header_;
  int flags_ = 0;
  char status_buf_[64];
  char* dynamic_status_buf_ = nullptr;
  size_t status_buf_len_ = 0;
  U8String header_buf_;
};

NAMESPACE_END

#endif // WEB_SERVER_HTTP_RESPONSE_H_
