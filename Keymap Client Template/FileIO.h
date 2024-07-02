#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

class FileIO {
public:
    inline static std::string getRoamingStatePath()
    {
        return (getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\");
    }

    inline static std::string getClientPath()
    {
        return getRoamingStatePath() + "TemplateClient\\";
    }

    inline static bool doesClientPathExist(const std::string& path)
    {
        return std::filesystem::exists(getClientPath() + path);
    }

    inline static void createPath(const std::string& path)
    {
        std::filesystem::create_directory(getClientPath() + path);
    }

    inline static void deletePath(const std::string& path)
    {
        std::filesystem::remove(path);
    }

    inline static bool setupClientPath()
    {
        if (!std::filesystem::exists(getClientPath())) {
            std::filesystem::create_directory(getClientPath());
            return true;
        }
        return false;
    }

    static void writeFile(const std::string& filePath, const std::string& content)
    {
        std::ofstream file(getClientPath() + filePath);
        if (file.is_open()) {
            file << content;
            file.close();
        }
        else {
            std::cout << "[fileio.h]: 51, Unable to open the file for writing" << std::endl;
        }
    }

    static std::string readFile(const std::string& filePath, bool noError = false)
    {
        std::ifstream file(getClientPath() + filePath);
        std::string content;
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (content.size() > 1) {
                    content += "\r\n";
                }

                content += line;
            }
            file.close();
        }
        else {
            if (noError) {
				return content;
			}

            std::cout << "[fileio.h]: 67, Unable to open the file for reading" << std::endl;
        }
        return content;
    }
};