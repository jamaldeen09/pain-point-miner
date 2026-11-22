#include <iostream>
#include <curl/curl.h>
#include "http_client.hpp"
using http_client::HTTP_RESULT;
using http_client::GET;
using std::string;

int main(void)
{
    string url = "https://api.github.com/users/jamaldeen09";
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    HTTP_RESULT http_res = GET(url, "painpoint-miner/0.0.1", headers);
    std::cout << "HTTP status code: " << http_res.http_status_code << '\n';
    std::cout << "Response body: " << http_res.response_body << '\n';
}
