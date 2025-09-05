#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace OxyRender
{
    class ResourcesManager
    {
    private:
        std::vector<std::filesystem::path> m_searchPaths;

        ResourcesManager()
        {
            namespace fs = std::filesystem;
            fs::path base = fs::current_path();

            // 默认搜索路径
            m_searchPaths.push_back(base / "resources");
            m_searchPaths.push_back(base.parent_path() / "resources");
            m_searchPaths.push_back(base / "shaders");
            m_searchPaths.push_back(base.parent_path() / "shaders");
        }

        ~ResourcesManager() = default;

    public:
        ResourcesManager(const ResourcesManager &) = delete;
        ResourcesManager &operator=(const ResourcesManager &) = delete;

        static ResourcesManager &getInstance()
        {
            static ResourcesManager instance;
            return instance;
        }

        void addSearchPath(const std::string &path)
        {
            m_searchPaths.push_back(std::filesystem::path(path));
        }

        std::string getCurrentPath() const
        {
            return std::filesystem::current_path().string();
        }

        std::string resolve(const std::string &name) const
        {
            namespace fs = std::filesystem;
            fs::path path(name);

            // 如果name是绝对路径且存在，直接返回
            if (path.is_absolute() && fs::exists(path))
                return fs::absolute(path).string();

            // 否则，尝试从搜索路径中查找
            for (const auto &dir : m_searchPaths)
            {
                // 检查直接拼接的路径
                fs::path fullPath = dir / path;
                if (fs::exists(fullPath))
                    return fs::absolute(fullPath).string();

                // 递归遍历目录查找文件
                if (fs::exists(dir) && fs::is_directory(dir))
                {
                    try
                    {
                        for (const auto &entry : fs::recursive_directory_iterator(dir))
                        {
                            if (entry.is_regular_file() && entry.path().filename() == path.filename())
                            {
                                return fs::absolute(entry.path()).string();
                            }
                        }
                    }
                    catch (const fs::filesystem_error &ex)
                    {
                        std::cerr << "Warning: Cannot access directory " << dir << ": " << ex.what() << std::endl;
                    }
                }
            }

            throw std::runtime_error("Resource not found: " + name);
        }
    };
}
