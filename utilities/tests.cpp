// This file contains tests for the utilities in this folder.

#include <csignal>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

#include "file_watcher.hpp"

void assert_equal(std::string expected, std::string actual, std::string test)
{
    if (expected != actual)
    {
        std::cerr << std::endl
                  << "==================" << std::endl;
        std::cerr << test << ":\nExpected: "
                  << expected << "\nbut got: "
                  << actual << std::endl;
        std::cerr << "==================" << std::endl;
        exit(1);
    }
}

// Test that the file watcher correctly watches an existing file for changes.
void test_file_watcher_modify_file()
{
    std::string test_file = "test_file_watcher.txt";

    std::ofstream test_file_create_stream(test_file);
    test_file_create_stream.close();

    FileWatcher watcher(test_file, "echo \"New file contents.\" > " + test_file);

    sleep(1);

    std::remove(test_file.c_str());

    std::ofstream test_file_write_stream(test_file);
    test_file_write_stream << "Trigger a file change.";
    test_file_write_stream.close();

    sleep(1);

    watcher.trigger_callback();

    std::ifstream test_file_read_stream(test_file);

    std::string contents;
    std::getline(test_file_read_stream, contents);

    assert_equal("New file contents.", contents, "FileWatcher: Modified");

    std::remove(test_file.c_str());
}

int main()
{
    test_file_watcher_modify_file();
}
