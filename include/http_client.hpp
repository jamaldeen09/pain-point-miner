#ifndef HTTP_RESULT_H
#define HTTP_RESULT_H

#include <string>
#include <curl/curl.h>

namespace http_client {
    struct HTTP_RESULT {
        std::string response_body;
        long http_status_code = 0;
    };
    HTTP_RESULT GET(const std::string &url, std::string user_agent, struct curl_slist *headers = nullptr);
} // namespace http_client

#endif // HTTP_RESULT_H
