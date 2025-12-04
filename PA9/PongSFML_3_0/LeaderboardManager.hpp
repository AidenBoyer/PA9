#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

class LeaderboardManager {
public:
    struct Score
    {
        std::string Name;
		int Waves;
    };
    /*
    
     LeaderboardManager::Score* scores = leaderboard.GetLeaderBoard();
        for (int i = 0; i < 5; i++) {
            if (!scores[i].Name.empty()) {
                std::cout << (i + 1) << ". " << scores[i].Name
                         << " - " << scores[i].Waves << " waves" << std::endl;
            } else {
                std::cout << (i + 1) << ". (empty)" << std::endl;
            }
        }
    */
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

        std::cout << (success ? "Score sent: " : "Failed: ") << name << ": " << waves << std::endl;
        return success;
    }

    Score* GetLeaderBoard() {
        HINTERNET hInternet = InternetOpenA("Game", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) return nullptr;

        //reference variables for crackurl
        URL_COMPONENTSA uc = { sizeof(uc) };
        char host[256], path[256];
        uc.lpszHostName = host;
        uc.dwHostNameLength = 256;
        uc.lpszUrlPath = path;
        uc.dwUrlPathLength = 256;

        //parse firebase url into host and path
        if (!InternetCrackUrlA(firebaseUrl.c_str(), 0, 0, &uc)) {
            InternetCloseHandle(hInternet);
            return nullptr;
        }

        //connect to firebase
        HINTERNET hConnect = InternetConnectA(hInternet, host, INTERNET_DEFAULT_HTTPS_PORT,
            NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (!hConnect) {
            InternetCloseHandle(hInternet);
            return nullptr;
        }

        //request to GET leaderboard data from firebase
        HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", path, NULL, NULL,
            NULL, INTERNET_FLAG_SECURE, 0);
        if (!hRequest) {
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return nullptr;
        }

        if (!HttpSendRequestA(hRequest, NULL, 0, NULL, 0)) {
            InternetCloseHandle(hRequest);
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return nullptr;
        }

        //read response data
        std::string response;
        char buffer[4096];
        DWORD bytesRead;
        while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            response += buffer;
        }

        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        
        ParseAndSortScores(response);

        return topFiveScore;
    }

private:
	Score topFiveScore[5];
    void ParseAndSortScores(const std::string& json) {
        Score tempScores[100];
        int count = 0;

        size_t pos = 0;
        //parse all name/wave pairs from JSON response
        /*
        {
          "fkdsffsfs": {"name": "Andy", "waves": 5},
          "asdasdadad": {"name": "Elon", "waves": 2},
          "asdwasdw": {"name": "Bitcoin", "waves": 15}
        }
        */
        while (count < 100) {
            //goto name field
            size_t namePos = json.find("\"name\"", pos);
            if (namePos == std::string::npos) break;
            //start of name
            size_t nameStart = json.find("\"", namePos + 6);
            if (nameStart == std::string::npos) break;
            nameStart++;
            //end of name
            size_t nameEnd = json.find("\"", nameStart);
            if (nameEnd == std::string::npos) break;

            size_t wavesPos = json.find("\"waves\"", nameEnd);
            if (wavesPos == std::string::npos) break;

            size_t wavesStart = json.find(":", wavesPos);
            if (wavesStart == std::string::npos) break;
            wavesStart++;

            size_t wavesEnd = json.find_first_of(",}", wavesStart);
            if (wavesEnd == std::string::npos) break;

            //json voodoo parsing
            tempScores[count].Name = json.substr(nameStart, nameEnd - nameStart);
            tempScores[count].Waves = std::stoi(json.substr(wavesStart, wavesEnd - wavesStart));
            count++;
            pos = wavesEnd;
        }

        
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (tempScores[j].Waves < tempScores[j + 1].Waves) {
                    Score temp = tempScores[j];
                    tempScores[j] = tempScores[j + 1];
                    tempScores[j + 1] = temp;
                }
            }
        }

        //copy top 5 scores to result array
        for (int i = 0; i < 5 && i < count; i++) {
            topFiveScore[i] = tempScores[i];
        }

        //fill remaining slots with empty scores if less than 5
        for (int i = count; i < 5; i++) {
            topFiveScore[i] = { "", 0 };
        }
    }

    //std::string firebaseUrl = "https://cpts122pa9-default-rtdb.firebaseio.com/";
    std::string firebaseUrl;
};