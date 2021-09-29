// src/woof/woof.cc
// v0.1.0
//
// Author: Cayden Lund
//   Date: 09/26/2021
//
// This file contains the implementation of the woof library.
//
// Copyright (C) 2021 Cayden Lund <https://github.com/shrimpster00>
// License: MIT (opensource.org/licenses/MIT)

#include "woof.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unistd.h>
#include <filesystem>

namespace woof
{
    // The woof::woof class is the main class for the woof library.
    // Use this class to watch a file or directory for changes.

    // The woof::woof class constructor.
    // Accepts three arguments: the path to the file or directory to watch, the command to execute
    // when the file or directory changes, and the polling interval.
    //
    //   * std::string path     - The path to the file or directory to watch.
    //   * std::string callback - The command to execute when the file or directory changes.
    //   * int polling_interval - The polling interval (in seconds).
    woof::woof(std::string path, std::string callback, int polling_interval)
    {
        this->path = path;
        this->callback = callback;
        this->polling_interval = polling_interval;

        reset_file_map();
    }

    // The woof::woof class constructor.
    // Accepts four arguments: the path to the file or directory to watch, the command to execute
    // when the file or directory changes, the polling interval, and whether or not to run
    // the command on startup.
    //
    //   * std::string path     - The path to the file or directory to watch.
    //   * std::string callback - The command to execute when the file or directory changes.
    //   * int polling_interval - The polling interval (in seconds).
    //   * bool run_on_startup  - Whether or not to run the command on startup.
    woof::woof(std::string path, std::string callback, int polling_interval, bool run_on_startup)
    {
        this->path = path;
        this->callback = callback;
        this->polling_interval = polling_interval;

        if (run_on_startup)
        {
            run_callback();
        }

        reset_file_map();
    }

    // Begins polling for changes.
    void woof::start_poll()
    {
        std::cout << "Watching for changes in " << path
                  << " every " << polling_interval << " second(s)."
                  << std::endl
                  << std::endl;

        while (true)
        {
            sleep(polling_interval);
            if (check_files())
            {
                run_callback();
            }
        }
    }

    // Runs the callback command.
    void woof::run_callback()
    {
        std::system(callback.c_str());
    }

    // A method to signal the woof class to check for changes.
    bool woof::check_files()
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

    // A method to reset the file map.
    void woof::reset_file_map()
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
}
