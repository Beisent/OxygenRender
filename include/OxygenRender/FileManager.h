#pragma once
#include <string>
#include <filesystem>
namespace OxyRender
{
    class FileManager
    {
    private:
        FileManager() = default;

    public:
        FileManager(const FileManager &) = delete;
        FileManager &operator=(const FileManager &) = delete;
        static FileManager &getInstance()
        {
            static FileManager instance;
            return instance;
        }
        std::string getResourcesPath()
        {
            return std::filesystem::current_path().string() + "\\resources\\";
        }
    };

    class ResourcesManager : public FileManager
    {
    private:
    public:
    };
    class ShadersManager : public FileManager
    {
    private:
    public:
    };
}