/// ------------------------------------------------------------------------------------------------
/// Copyright 2026-present The Mystic Framework Authors
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
/// ------------------------------------------------------------------------------------------------
/// @file os_detection.test.cpp
/// @brief This file contains unit tests for the os detection module.
/// @author thedevmystic (Surya) <thedevmystic@gmail
/// @license Apache-2.0
/// ------------------------------------------------------------------------------------------------

import mystic.hal.os_detection;

#include <string>

#include <catch2/catch_test_macros.hpp>

#include "hal/os_detection_macros.hpp"

// On GitHub Actions, the OS for Linux is Ubuntu and for Windows is Windows Server 2025.

// Since these tests rely on a specific OS, we'll skip on local builds.
#ifndef MYSTIC_HAL_LOCAL_BUILD

TEST_CASE("Compile-Time OS Detection", "[hal][os][compile_time]") {
  SECTION("Detect OS at Compile Time") {
    constexpr auto kOS = mystic::hal::compile_time::GetOperatingSystem();

#  if MYSTIC_OS == MYSTIC_OS_LINUX
    REQUIRE(kOS == mystic::hal::OperatingSystem::kLinux);
    REQUIRE(std::string(mystic::hal::compile_time::GetOperatingSystemAsString()) == "Linux");
#  else
    REQUIRE(kOS == mystic::hal::OperatingSystem::kUnknown);
    REQUIRE(std::string(mystic::hal::compile_time::GetOperatingSystemAsString()) == "Unknown");
#  endif
  }
}

TEST_CASE("Runtime OS Detection", "[hal][os][runtime]") {
  SECTION("Detect OS at Runtime") {
    auto os = mystic::hal::runtime::GetOperatingSystem();
    auto linux_version = mystic::hal::runtime::GetLinuxVersion();

#  if MYSTIC_OS == MYSTIC_OS_LINUX
    REQUIRE(os == mystic::hal::OperatingSystem::kLinux);
    REQUIRE(std::string(mystic::hal::compile_time::GetOperatingSystemAsString()) == "Linux");
    REQUIRE(linux_version == mystic::hal::LinuxVersion::kUbuntu);
    REQUIRE(std::string(mystic::hal::runtime::GetLinuxVersionAsString()) == "Ubuntu");
#  else
    REQUIRE(os == mystic::hal::OperatingSystem::kUnknown);
    REQUIRE(std::string(mystic::hal::compile_time::GetOperatingSystemAsString()) == "Unknown");
    REQUIRE(linux_version == mystic::hal::LinuxVersion::kUnknown);
    REQUIRE(std::string(mystic::hal::runtime::GetLinuxVersionAsString()) == "Unknown");
#  endif
  }
}

TEST_CASE("Compile-Time OS Detection", "[hal][os][compile_time]") {
  SECTION("Detect OS at Compile Time") {
    constexpr auto kOS = mystic::hal::compile_time::GetOperatingSystem();

#  if MYSTIC_OS == MYSTIC_OS_WINDOWS
    REQUIRE(kOS == mystic::hal::OperatingSystem::kWindows);
    REQUIRE(std::string(mystic::hal::compile_time::GetOperatingSystemAsString()) == "Windows");
#  else
    REQUIRE(kOS == mystic::hal::OperatingSystem::kUnknown);
    REQUIRE(std::string(mystic::hal::compile_time::GetOperatingSystemAsString()) == "Unknown");
#  endif
  }
}

TEST_CASE("Runtime OS Detection", "[hal][os][runtime]") {
  SECTION("Detect OS at Runtime") {
    auto os = mystic::hal::runtime::GetOperatingSystem();
    auto windows_version = mystic::hal::runtime::GetWindowsVersion();

#  if MYSTIC_OS == MYSTIC_OS_WINDOWS
    REQUIRE(os == mystic::hal::OperatingSystem::kWindows);
    REQUIRE(std::string(mystic::hal::compile_time::GetOperatingSystemAsString()) == "Windows");
    REQUIRE(windows_version == mystic::hal::WindowsVersion::kWindowsServer2025);
    REQUIRE(std::string(mystic::hal::runtime::GetWindowsVersionAsString()) ==
            "Windows Server 2025");
#  else
    REQUIRE(os == mystic::hal::OperatingSystem::kUnknown);
    REQUIRE(std::string(mystic::hal::compile_time::GetOperatingSystemAsString()) == "Unknown");
    REQUIRE(windows_version == mystic::hal::WindowsVersion::kUnknown);
    REQUIRE(std::string(mystic::hal::runtime::GetWindowsVersionAsString()) == "Unknown");
#  endif
  }
}
#endif  // MYSTIC_HAL_LOCAL_BUILD
