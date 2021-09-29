cc_library(
    name = "woof",
    srcs = ["woof.cc"],
    hdrs = ["woof.h"],
    visibility = ["//visibility:public"]
)

cc_binary(
    name = "executable",
    srcs = ["executable.cc"],
    deps = ["woof", "@argh//argh"]
)
