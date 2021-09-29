// src/woof/executable.cc
// v0.1.0
//
// Author: Cayden Lund
//   Date: 09/28/2021
//
// This file contains the woof executable.
//
// Copyright (C) 2021 Cayden Lund <https://github.com/shrimpster00>
// License: MIT (opensource.org/licenses/MIT)

#include "woof.h"
#include "argh/argh.h"

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <filesystem>

// Catch interruptions and exit cleanly.
void sig_handler(int sig)
{
    std::cout << std::endl
              << "Caught signal " << sig << ". Exiting cleanly." << std::endl;
    exit(0);
}

// Print usage information.
void print_usage(std::string program_name)
{
    std::cerr << "Usage: " << program_name << " [-n] <file_or_directory> <command>"
              << std::endl
              << "   -n: watch for file changes every n seconds."
              << std::endl
              << std::endl
              << "Example: " << program_name << " -2 ./woof.cpp g++ ./woof.cpp"
              << std::endl
              << std::endl;
}

// Main program entry point.
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        print_usage(argv[0]);
        return 1;
    }

    argh::argh args(argc, argv);

    int poll_interval = 1;

    if (args["-n"])
    {
        poll_interval = std::stoi(args("-n"));
    }

    if (!std::filesystem::exists(args[1]))
    {
        std::cerr << "File or directory \"" << args[1] << "\" does not exist."
                  << std::endl
                  << std::endl;
        print_usage(args[0]);
        return 1;
    }

    std::stringstream command;

    for (int i = 2; i < args.size(); i++)
    {
        command << args[i];
    }

    std::signal(SIGINT, sig_handler);
    std::signal(SIGTERM, sig_handler);

    woof::woof watcher(args[1], command.str(), poll_interval);

    watcher.start_poll();

    return 0;
}
