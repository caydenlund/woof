#include <iostream>
#include <csignal>
#include <unistd.h>
#include <string>
#include <regex>

void sig_handler(int sig)
{
    std::cout << "Signal " << sig << " received." << std::endl;
    exit(0);
}

void print_usage(std::string program_name)
{
    std::cout << "Usage: " << program_name << " [-n] <file_or_directory>" << std::endl;
    std::cout << "  -n: watch for file changes every n seconds" << std::endl;
}

int main(int argc, char *argv[])
{
    int poll_interval = 1.0;

    if (argc > 1)
    {
        std::string arg = argv[1];
        std::regex r("^-\\d+$");
        if (std::regex_match(arg, r))
        {
            arg = arg.substr(1);
            poll_interval = std::stoi(arg);
        }
        else
        {
            std::cout << "Invalid argument: " << arg << std::endl << std::endl;
            print_usage(argv[0]);
            return 1;
        }
    }
    std::signal(SIGINT, sig_handler);
    std::signal(SIGTERM, sig_handler);

    while (1)
    {
        std::cout << "Woof!" << std::endl;
        sleep(poll_interval);
    }

    return 0;
}