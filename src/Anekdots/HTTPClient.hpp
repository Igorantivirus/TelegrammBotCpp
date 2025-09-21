#pragma once

#include <iostream>
#include <string>
#include <map>

#include <curl/curl.h>

class HttpClient
{
public:

    std::string getRequastParam(const std::string& url, const std::map<std::string, std::string>& json) const
    {
        std::string nortmUrl = url + '?';
        for (const auto& [key, value] : json)
            nortmUrl += key + '=' + value + '&';
        nortmUrl.pop_back();
        return getRequest(nortmUrl);
    }

    std::string getRequest(const std::string& url) const
    {
        CURL* curl;
        CURLcode res;
        std::string result;

        curl = curl_easy_init();

        if (!curl)
        {
            std::cerr << "Error of init CURL" << std::endl;
            return "";
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
        return result;
    }

private:

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
};