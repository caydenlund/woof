#include <iostream>
#include <csignal>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <string>
#include <regex>

#include "utilities/file_watcher.hpp"

void sig_handler(int sig)
{
    std::cout << std::endl
              << "Caught signal " << sig << ". Exiting cleanly." << std::endl;
    exit(0);
}

void print_usage(std::string program_name)
{
    std::cerr << "Usage: " << program_name << " [-n] <file_or_directory> <command>" << std::endl;
    std::cerr << "   -n: watch for file changes every n seconds." << std::endl;
    std::cerr << std::endl
              << "Example: " << program_name << " -2 ./woof.cpp g++ ./woof.cpp"
              << std::endl
              << std::endl;
}

int main(int argc, char *argv[])
{
    int poll_interval = 1.0;

    if (argc < 3)
    {
        print_usage(argv[0]);
        return 1;
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    args.begin();

    std::regex poll_interval_regex("^-\\d+$");
    if (std::regex_match(args[0], poll_interval_regex))
    {
        poll_interval = std::stoi(std::string(args[0]).substr(1));
        args.erase(args.begin(), args.begin() + 1);
        args.begin();
    }

    std::string file_or_directory = args[0];
    if (!std::filesystem::exists(file_or_directory))
    {
        std::cerr << "File or directory \"" << file_or_directory << "\" does not exist." << std::endl
                  << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    std::string command = "";
    for (int i = 1; i < args.size(); i++)
    {
        command += args[i] + " ";
    }

    std::signal(SIGINT, sig_handler);
    std::signal(SIGTERM, sig_handler);

    FileWatcher watcher(file_or_directory, command);

    std::cout << "Watching for changes in " << file_or_directory
              << " every " << poll_interval << " second(s)."
              << std::endl
              << std::endl;

    while (true)
    {
        watcher.trigger_callback();
        sleep(poll_interval);
    }

    return 0;
}
