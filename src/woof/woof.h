// src/woof/woof.h
// v0.1.0
//
// Author: Cayden Lund
//   Date: 09/26/2021
//
// This file contains the headers for the woof library.
//
// Copyright (C) 2021 Cayden Lund <https://github.com/shrimpster00>
// License: MIT (opensource.org/licenses/MIT)

#ifndef WOOF_H
#define WOOF_H

#include <string>
#include <unordered_map>

// The woof namespace contains all of the woof functionality and utilities.
namespace woof
{
    // The woof::woof class is the main class for the woof library.
    // Use this class to watch a file or directory for changes.
    class woof
    {
    public:
        // The woof::woof class constructor.
        // Accepts three arguments: the path to the file or directory to watch, the command to execute
        // when the file or directory changes, and the polling interval.
        //
        //   * std::string path     - The path to the file or directory to watch.
        //   * std::string callback - The command to execute when the file or directory changes.
        //   * int polling_interval - The polling interval (in seconds).
        woof(std::string path, std::string callback, int polling_interval);

        // The woof::woof class constructor.
        // This version of the constructor accepts a fourth argument: a boolean
        // indicating whether or not to run the callback on startup.
        //
        //   * std::string path     - The path to the file or directory to watch.
        //   * std::string callback - The command to execute when the file or directory changes.
        //   * int polling_interval - The polling interval (in seconds).
        //   * bool run_on_startup  - A boolean indicating whether or not to run the callback on startup.
        woof(std::string path, std::string callback, int polling_interval, bool run_on_startup);

        // Begins polling for changes.
        void start_poll();

    private:
        // Runs the callback.
        void run_callback();

        // A method to signal the woof class to check for changes.
        // This method should be called periodically.
        bool check_files();

        // Resets the map of files to their last modified times.
        void reset_file_map();

        // The path to the file or directory to watch.
        std::string path;

        // The command to execute when the file or directory changes.
        std::string callback;

        // The polling interval (in seconds).
        int polling_interval;

        // The map of files to their last modified times.
        std::unordered_map<std::string, int64_t> file_map;
    };
}

#endif