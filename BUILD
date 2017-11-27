package(default_visibility = ["//visibility:public"])

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
    "envoy_cc_library",
)

envoy_cc_binary(
    name = "envoy",
    repository = "@envoy",
    deps = [
        ":auth_request_config",
        "@envoy//source/exe:envoy_main_entry_lib",
    ],
)

envoy_cc_library(
    name = "auth_request_lib",
    srcs = ["auth_request.cc"],
    hdrs = ["auth_request.h"],
    repository = "@envoy",
    deps = [
        "@envoy//source/exe:envoy_common_lib",
    ],
)

envoy_cc_library(
    name = "auth_request_config",
    srcs = ["auth_request_config.cc"],
    repository = "@envoy",
    deps = [
        ":auth_request_lib",
        "@envoy//source/exe:envoy_common_lib",
    ],
)