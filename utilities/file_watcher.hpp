#ifndef FILE_WATCHER_HPP
#define FILE_WATCHER_HPP

#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_map>

enum class FileChangeType
{
    CREATED,
    MODIFIED,
    DELETED
};

class FileWatcher
{
public:
    FileWatcher(std::string path, std::string callback);
    void trigger_callback();

private:
    std::string path;
    std::string callback;
    std::unordered_map<std::string, int64_t> file_map;
    bool check_files();
    void reset_file_map();
};

// Constructor for a new file watcher. The path is the file or directory to watch.
// If the path is a directory, it will enter the directory recursively.
FileWatcher::FileWatcher(std::string path, std::string callback)
{
    this->path = path;
    this->callback = callback;

    reset_file_map();
}

// Triggers the callback function if a file has changed.
void FileWatcher::trigger_callback()
{
    if (check_files())
    {
        std::system(callback.c_str());
    }
}

// Resets the file map to the current state of the files in the directory.
void FileWatcher::reset_file_map()
{
    file_map.clear();
    if (std::filesystem::is_regular_file(path))
    {
        file_map[path] = std::filesystem::last_write_time(path).time_since_epoch().count();
    }
    else
    {
        for (auto &path_entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (path_entry.is_regular_file())
            {
                file_map[path_entry.path().string()] = path_entry.last_write_time().time_since_epoch().count();
            }
        }
    }
}

// Checks whether the files in the map have changed.
bool FileWatcher::check_files()
{
    if (std::filesystem::is_regular_file(path))
    {
        int64_t last_write_time = std::filesystem::last_write_time(path).time_since_epoch().count();
        if (last_write_time != file_map[path])
        {
            file_map[path] = last_write_time;
            return true;
        }
        return false;
    }
    else
    {
        // We use a variable to store the result of the check
        // so that we can continue updating the file map after
        // finding a change.
        bool changed = false;
        // For each file in the map, perform the following checks.
        for (std::pair<const std::string, int64_t> &file_entry : file_map)
        {
            std::string file = file_entry.first;
            int64_t last_write_time = file_entry.second;

            int64_t new_write_time = std::filesystem::last_write_time(file).time_since_epoch().count();

            // If the file doesn't exist, it was deleted.
            if (!std::filesystem::exists(file))
            {
                file_map.erase(file);
                changed = true;
            }
            // If the file's last write time has changed, it was modified.
            else if (last_write_time != new_write_time)
            {
                file_map[file] = new_write_time;
                changed = true;
            }
        }
        if (changed)
        {
            return true;
        }
        // If no files in the map have changed, check whether any new files have been added.
        // Since this can be an expensive operation, we only do it if we know that no files in the map have changed.
        for (auto &path_entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (path_entry.is_regular_file() && file_map.find(path_entry.path().string()) == file_map.end())
            {
                file_map[path_entry.path().string()] = std::filesystem::last_write_time(path_entry.path().string()).time_since_epoch().count();
                return true;
            }
        }
        return false;
    }
}

#endif