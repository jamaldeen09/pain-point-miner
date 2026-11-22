#include <curl/curl.h>
#include <stdexcept>
#include <string>
#include "http_client.hpp"

namespace 
{
    size_t write_to_string_cb(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t total_size = size * nmemb;
        auto* out = static_cast<std::string*>(userp);
        out->append(static_cast<const char*>(contents), total_size);
        return total_size;
    }
} // namespace

namespace http_client
{
    // Makes a GET request to an endpoint and returns the status code
    // and the response body returned

    // url - The URL/endpoint we wish to make a request to
    // headers - The headers that will be sent to the server
    HTTP_RESULT GET(const std::string &url, std::string user_agent ,struct curl_slist *headers) {
        // CURL is a typedef and under the hood is basically "void"
        // so CURL *curl represents a void pointer

        // This initializes libcurl's functionality before any
        // communication on the network occurs. If it fails an
        // std::runtime_error is thrown
        CURL *curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize libcurl");
        }

        // response_body - This is meant to store the response returned by the API
        // curl_slist - This is meant to store the headers that we intend to send to the server
        std::string response_body;

        // curl_easy_setopt basically tells libcurl how to behave.

        // CURLOPT_URL - The URL to work on
        // CURLOPT_HTTPHEADER - Custom HTTP Headers
        // CURLOPT_WRITEFUNCTION - Normally libcurl sends the response body to stdout unless a 
        //                         write callback is registered CURLOPT_WRITEFUNCTION
        // CURLOPT_WRITEDATA - Data pointer to pass to the write callback
        // CURLOPT_FOLLOWLOCATION - This instructs libcurl to automatically follow HTTP 
        //                          3xx redirects by issuing a new request to the URL specified in the server's Location: header
        // CURLOPT_TIMEOUT - This sets the maximum time in seconds that an entire libcurl transfer operation 
        //                   is allowed to take from start to finish
        // CURLOPT_USERAGENT - This handles the User-Agent: header
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());

        // curl_easy_perform() makes the request to the target endpoint and res stores
        // whatever curl_easy_perform returns
        CURLcode res = curl_easy_perform(curl);

        // A long integer that stores the status_code returned by the server
        // we can use this to check if a server returneded a 200 OK or not
        long http_status_code = 0;

        // This just checks if the curl operation suceeded
        // and if it did we extract the returned status code and store it
        // in the "http_status_code" variable
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status_code);
        }

        // we first check if headers were set and if so curl_slist_free_all 
        // deallocates the headers which is a linked list and releases all memory associated with it
        if (headers) {
            curl_slist_free_all(headers);
        }

        // End the libcurl easy session and free all memory and resources associated with the
        // "curl" handle defined at the top
        curl_easy_cleanup(curl);

        // If the curl operation failed throw an error
        if (res != CURLE_OK) {
            throw std::runtime_error(std::string("HTTP GET failed: ") + curl_easy_strerror(res));
        }

        // If the stats code is above 300 or below 200 throw an error
        if (http_status_code < 200 || http_status_code >= 300) {
            throw std::runtime_error("HTTP GET returned status " + std::to_string(http_status_code) + " for URL: " + url);
        }
    
        return {response_body, http_status_code};
    }
} // namespace http_client