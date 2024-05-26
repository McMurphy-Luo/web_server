#ifndef WEB_SERVER_HTTP_REQUEST_H_
#define WEB_SERVER_HTTP_REQUEST_H_

#include "web_server/web_server.h"
#include "ref_counter.h"

NAMESPACE_BEGIN

class HttpRequest
  : public RefCounter<ThreadUnsafeCounter>
{
public:



};


NAMESPACE_END

#endif // WEB_SERVER_HTTP_REQUEST_H_
