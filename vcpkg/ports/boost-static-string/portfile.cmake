# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/static_string
    REF boost-1.83.0
    SHA512 c3150f362aa771d6e41f20cd7ebac895c7eb039fdf80a04d65933e8f9c574f214a7164121b0a38b7f6d7e5a9d4a315d98eefec663fceb4251ce7dce50b7e9dee
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})
