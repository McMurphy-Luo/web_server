#include "http_response.h"
#include <algorithm>

using NAMESPACE::HttpResponse;
using NAMESPACE::U8String;

static struct HttpStatusString {
  int code;
  const char* description;
};

static enum {
  kStatusDirty = 1 << 0,
  kHeaderDirty = 1 << 1
};

static constexpr HttpStatusString kHttpCodes[] = {
  {100, "Continue"},
  {101, "Switching Protocols"},
  {102, "Processing"},
  {103, "Early Hints"},
  {200, "OK"},
  {201, "Created"},
  {202, "Accepted"},
  {203, "Non - Authoritative Information"},
  {204, "No Content"},
  {205, "Reset Content"},
  {206, "Partial Content"},
  {207, "Multi - Status"},
  {208, "Already Reported"},
  {226, "IM Used"},
  {300, "Multiple Choices"},
  {301, "Moved Permanently"},
  {302, "Found"},
  {303, "See Other"},
  {304, "Not Modified"},
  {307, "Temporary Redirect"},
  {308, "Permanent Redirect"},
  {400, "Bad Request"},
  {401, "Unauthorized"},
  {402, "Payment Required"},
  {403, "Forbidden"},
  {404, "Not Found"},
  {405, "Method Not Allowed"},
  {406, "Not Acceptable"},
  {407, "Proxy Authentication Required"},
  {408, "Request Timeout"},
  {409, "Conflict"},
  {410, "Gone"},
  {411, "Length Required"},
  {412, "Precondition Failed"},
  {413, "Content Too Large"},
  {414, "URI Too Long"},
  {415, "Unsupported Media Type"},
  {416, "Range Not Satisfiable"},
  {417, "Expectation Failed"},
  {418, "I'm a teapot"},
  {421, "Misdirected Request"},
  {422, "Unprocessable Content"},
  {423, "Locked"},
  {424, "Failed Dependency"},
  {425, "Too Early"},
  {426, "Upgrade Required"},
  {428, "Precondition Required"},
  {429, "Too Many Requests"},
  {431, "Request Header Fields Too Large"},
  {451, "Unavailable For Legal Reasons"},
  {500, "Internal Server Error"},
  {501, "Not Implemented"},
  {502, "Bad Gateway"},
  {503, "Service Unavailable"},
  {504, "Gateway Timeout"},
  {505, "HTTP Version Not Supported"},
  {506, "Variant Also Negotiates"},
  {507, "Insufficient Storage"},
  {508, "Loop Detected"},
  {510, "Not Extended"},
  {511, "Network Authentication Required"}
};

static const char* kUknown = "Uknown";

static const char* StatusStringFromStatusCode(int code) {
  auto comparator = [](const HttpStatusString& item, int value) {
    return item.code < value;
  };
  auto it = std::lower_bound(std::begin(kHttpCodes), std::end(kHttpCodes), code, comparator);
  if (it == std::end(kHttpCodes)) {
    return kUknown;
  }
  return it->description;
}

HttpResponse::HttpResponse() {
  header_.reserve(5);
  memset(status_buf_, 0, std::size(status_buf_));
}

HttpResponse::~HttpResponse() {

}

void HttpResponse::SetStatuCode(int code) {
  status_code_ = code;
}

void HttpResponse::SetHeader(const U8String& header, const U8String& value) {
  
}

void HttpResponse::SetHeader(U8String&& header, U8String&& value) {

}

void HttpResponse::SetHeader(const U8String& header, U8String&& value) {

}

void HttpResponse::SetHeader(U8String&& header, const U8String& value) {

}

void HttpResponse::RemoveHeader(const U8String& header) {
  size_t removed_count = header_.erase(header);
  flags_ |= kStatusDirty;
}

U8String::const_pointer HttpResponse::StatusBuffer() {
  return status_buf_.c_str();
}

U8String::size_type HttpResponse::StatusBufferSize() {

}

U8String::const_pointer HttpResponse::HeaderBuffer() {

}

U8String::size_type HttpResponse::HeaderBufferSize() {

}
