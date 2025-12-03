#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

class LeaderboardManager {
public:
    LeaderboardManager(const std::string& url) : firebaseUrl(url) {}

    bool SendScore(const std::string& name, int waves) {
        std::ostringstream json;
        json << "{\"name\":\"" << name << "\",\"waves\":" << waves << "}";

        HINTERNET hInternet = InternetOpenA("Game", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) return false;

		//reference variables for crackurl, basically pass by references
        URL_COMPONENTSA uc = { sizeof(uc) };
        char host[256], path[256];
        uc.lpszHostName = host;
        uc.dwHostNameLength = 256;
        uc.lpszUrlPath = path;
        uc.dwUrlPathLength = 256;

		//puts firebase url into host and path /leaderboard.json, 
        if (!InternetCrackUrlA(firebaseUrl.c_str(), 0, 0, &uc)) {
            InternetCloseHandle(hInternet);
            return false;
        }
		//connects to firebase
        HINTERNET hConnect = InternetConnectA(hInternet, host, INTERNET_DEFAULT_HTTPS_PORT,
            NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (!hConnect) {
            InternetCloseHandle(hInternet);
            return false;
        }

		//request to post data of name and waves to firebase
        HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path, NULL, NULL,
            NULL, INTERNET_FLAG_SECURE, 0);
        if (!hRequest) {
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return false;
        }

        std::string data = json.str();
        bool success = HttpSendRequestA(hRequest, "Content-Type: application/json\r\n",
            -1, (void*)data.c_str(), data.length());

        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        std::cout << (success ? "Score sent: " : "Failed: ") << name << " - " << waves << std::endl;
        return success;
    }

private:
    //std::string firebaseUrl = "https://cpts122pa9-default-rtdb.firebaseio.com/";
    std::string firebaseUrl;
};