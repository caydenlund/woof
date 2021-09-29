# woof

Watch all the files!

Woof is a C++ library and command-line executable that provides a simple, powerful, and easy-to-use API
for watching a path for updates.

## Features:

* **Lightning-quick.** No recursion. No expensive computations. No regular expressions. No nonsense. Enjoy the performance benefits of plain old C++!
* **Easy to use.** Just point it to a path and go.
* **Versatile.** It *just works* in every situation.
* **Platform-agnostic.**
* **Independent.** woof relies on only one dependency outside of the standard libraries: [argh](https://github.com/shrimpster00/argh). It's automatically downloaded and compiled as necessary with Bazel.
* All that, and even **a bag of chips!**

*Note: actual chips not included.

# Example Usage:

## woof Library

    #include "woof/woof.h"

    int main(int argc, char *argv[])
    {
        woof::woof watcher("file.txt", "echo \"file.txt\" updated!", 2);
        watcher.start_poll();
    }

## woof Executable

    woof -n 2 file.txt echo "\"file.txt\"" updated!

# Class Members:

### `woof::woof(std::string path, std::string callback, int polling_interval)`

The woof::woof class constructor.

Accepts three arguments: the path to the file or directory to watch, the command to execute
when the file or directory changes, and the polling interval.

`std::string path` - The path to the file or directory to watch.

`std::string callback` - The command to execute when the file or directory changes.

`int polling_interval` - The polling interval (in seconds).

### `woof(std::string path, std::string callback, int polling_interval, bool run_on_startup)`

The woof::woof class constructor.

This version of the constructor accepts a fourth argument: a boolean indicating whether or not to run the callback on startup.

`std::string path` - The path to the file or directory to watch.

`std::string callback` - The command to execute when the file or directory changes.

`int polling_interval` - The polling interval (in seconds).

`bool run_on_startup` - A boolean indicating whether or not to run the callback on startup.

### `void start_poll()`

Begins polling for changes in the filesystem.

# Build:

woof is built using Google's Bazel utility.

For the library:

    $ cd src && bazel build //woof

And for the executable:

    $ cd src && bazel build //woof:executable

And that's it. That is seriously all it takes. The binaries will be put in the `bazel-bin` directory.
