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
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "hal/os_detection_macros.hpp"

// On GitHub Actions, the OS for Linux is Ubuntu and for Windows is Windows Server 2025.

TEST_CASE("ToString() functions return expected strings.",
          "[mystic][hal][os_detection][to_string]") {
  using mystic::hal::ToString;

  SECTION("OperatingSystem enum") {
    auto os = GENERATE(values({mystic::hal::OperatingSystem::kWindows,
                               mystic::hal::OperatingSystem::kMacOS,
                               mystic::hal::OperatingSystem::kLinux,
                               mystic::hal::OperatingSystem::kUnknown}));

    const char* os_str = ToString(os);
    REQUIRE(os_str != nullptr);
    REQUIRE_FALSE(std::string(os_str).empty());
  }

  SECTION("WindowsVariant enum") {
    auto variant = GENERATE(values({mystic::hal::WindowsVariant::kUnknown,
                                    mystic::hal::WindowsVariant::kWindows7,
                                    mystic::hal::WindowsVariant::kWindows8,
                                    mystic::hal::WindowsVariant::kWindows81,
                                    mystic::hal::WindowsVariant::kWindows10,
                                    mystic::hal::WindowsVariant::kWindows11,
                                    mystic::hal::WindowsVariant::kWindowsServer2008,
                                    mystic::hal::WindowsVariant::kWindowsServer2012,
                                    mystic::hal::WindowsVariant::kWindowsServer2016,
                                    mystic::hal::WindowsVariant::kWindowsServer2019,
                                    mystic::hal::WindowsVariant::kWindowsServer2022,
                                    mystic::hal::WindowsVariant::kWindowsServer2025,
                                    mystic::hal::WindowsVariant::kOther}));

    const char* variant_str = ToString(variant);
    REQUIRE(variant_str != nullptr);
    REQUIRE_FALSE(std::string(variant_str).empty());
  }

  SECTION("LinuxVariant enum") {
    auto variant = GENERATE(values({mystic::hal::LinuxVariant::kUnknown,
                                    mystic::hal::LinuxVariant::kDebian,
                                    mystic::hal::LinuxVariant::kUbuntu,
                                    mystic::hal::LinuxVariant::kMint,
                                    mystic::hal::LinuxVariant::kKali,
                                    mystic::hal::LinuxVariant::kRHEL,
                                    mystic::hal::LinuxVariant::kCentOS,
                                    mystic::hal::LinuxVariant::kFedora,
                                    mystic::hal::LinuxVariant::kArch,
                                    mystic::hal::LinuxVariant::kManjaro,
                                    mystic::hal::LinuxVariant::kEndeavourOS,
                                    mystic::hal::LinuxVariant::kOpenSUSE,
                                    mystic::hal::LinuxVariant::kOpenSUSELeap,
                                    mystic::hal::LinuxVariant::kSUSEEnterprise,
                                    mystic::hal::LinuxVariant::kOpenSUSETumbleweed,
                                    mystic::hal::LinuxVariant::kAlpine,
                                    mystic::hal::LinuxVariant::kGentoo,
                                    mystic::hal::LinuxVariant::kNixOS,
                                    mystic::hal::LinuxVariant::kOther}));

    const char* variant_str = ToString(variant);
    REQUIRE(variant_str != nullptr);
    REQUIRE_FALSE(std::string(variant_str).empty());
  }

  SECTION("OperatingSystemVersion struct") {
    mystic::hal::OperatingSystemVersion version{.major = 1, .minor = 2, .patch = 3, .build = 4};
    const char* version_str = ToString(version);

#if MYSTIC_OS == MYSTIC_OS_WINDOWS
    // On Windows, we expect string to be in the format "major.minor.build"
    REQUIRE(std::string(version_str) == "1.2.4");
#else
    // On Linux, we expect string to be in the format "major.minor.patch"
    REQUIRE(std::string(version_str) == "1.2.3");
#endif
  }

  SECTION("OperatingSystemInfo struct") {
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
    mystic::hal::OperatingSystemInfo windows_info(
        mystic::hal::OperatingSystem::kWindows,
        static_cast<mystic::hal::OperatingSystemVariant>(mystic::hal::WindowsVariant::kWindows10),
        mystic::hal::OperatingSystemVersion{.major = 10, .minor = 0, .patch = 0, .build = 19042});
    const char* windows_info_str = ToString(windows_info);

    REQUIRE(windows_info_str != nullptr);
    REQUIRE_FALSE(std::string(windows_info_str).empty());
    REQUIRE(std::string(windows_info_str) == "Windows (Windows 10) Version: 10.0.19042");
#else
    mystic::hal::OperatingSystemInfo linux_info(
        mystic::hal::OperatingSystem::kLinux,
        static_cast<mystic::hal::OperatingSystemVariant>(mystic::hal::LinuxVariant::kUbuntu),
        mystic::hal::OperatingSystemVersion{.major = 20, .minor = 4, .patch = 1, .build = 0});
    const char* linux_info_str = ToString(linux_info);

    REQUIRE(linux_info_str != nullptr);
    REQUIRE_FALSE(std::string(linux_info_str).empty());
    REQUIRE(std::string(linux_info_str) == "Linux (Ubuntu) Version: 20.4.1");
#endif
  }
}

TEST_CASE("OperatingSystemVariant alias resolves correctly based on OS.",
          "[mystic][hal][os_detection][variant_alias]") {
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
  static_assert(std::is_same_v<mystic::hal::OperatingSystemVariant, mystic::hal::WindowsVariant>,
                "OperatingSystemVariant should alias to WindowsVariant on Windows.");
#elif MYSTIC_OS == MYSTIC_OS_LINUX
  static_assert(std::is_same_v<mystic::hal::OperatingSystemVariant, mystic::hal::LinuxVariant>,
                "OperatingSystemVariant should alias to LinuxVariant on Linux.");
#else
  static_assert(std::is_same_v<mystic::hal::OperatingSystemVariant, mystic::hal::LinuxVariant>,
                "OperatingSystemVariant should alias to LinuxVariant on non-Windows OS.");
#endif
}

TEST_CASE("Correct OperatingSystem enum is returned at compile time.",
          "[mystic][hal][os_detection][compile_time_os]") {
  auto os = mystic::hal::compile_time::GetOperatingSystem();
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
  REQUIRE(os == mystic::hal::OperatingSystem::kWindows);
#elif MYSTIC_OS == MYSTIC_OS_MACOS
  REQUIRE(os == mystic::hal::OperatingSystem::kMacOS);
#elif MYSTIC_OS == MYSTIC_OS_LINUX
  REQUIRE(os == mystic::hal::OperatingSystem::kLinux);
#else
  REQUIRE(os == mystic::hal::OperatingSystem::kUnknown);
#endif
}

TEST_CASE("Compile-time and runtime OS detection are consistent.",
          "[mystic][hal][os_detection][compile_runtime_consistency]") {
  auto compile_time_os = mystic::hal::compile_time::GetOperatingSystem();
  auto runtime_os = mystic::hal::runtime::GetOperatingSystem();

  REQUIRE(compile_time_os == runtime_os);
}

TEST_CASE("OperatingSystemInfo struct initializes correctly.",
          "[mystic][hal][os_detection][os_info_init]") {
  // Default constructor should initialize to unknown values.
  mystic::hal::OperatingSystemInfo info_default;
  REQUIRE(info_default.os == mystic::hal::OperatingSystem::kUnknown);
  REQUIRE(info_default.variant == static_cast<mystic::hal::OperatingSystemVariant>(0));
  REQUIRE(info_default.version.major == 0);
  REQUIRE(info_default.version.minor == 0);
  REQUIRE(info_default.version.patch == 0);
  REQUIRE(info_default.version.build == 0);

  // Parameterized constructor should initialize to provided values (Differ due to Variant
  // aliasing).
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
  mystic::hal::OperatingSystemInfo info(
      mystic::hal::OperatingSystem::kWindows,
      static_cast<mystic::hal::OperatingSystemVariant>(mystic::hal::WindowsVariant::kWindows10),
      mystic::hal::OperatingSystemVersion{.major = 10, .minor = 0, .patch = 0, .build = 19042});

  REQUIRE(info.os == mystic::hal::OperatingSystem::kWindows);
  REQUIRE(info.variant == static_cast<mystic::hal::OperatingSystemVariant>(
                              mystic::hal::WindowsVariant::kWindows10));
  REQUIRE(info.version.major == 10);
  REQUIRE(info.version.minor == 0);
  REQUIRE(info.version.patch == 0);
  REQUIRE(info.version.build == 19042);
#else
  mystic::hal::OperatingSystemInfo info(
      mystic::hal::OperatingSystem::kLinux,
      static_cast<mystic::hal::OperatingSystemVariant>(mystic::hal::LinuxVariant::kUbuntu),
      mystic::hal::OperatingSystemVersion{.major = 20, .minor = 4, .patch = 1, .build = 0});

  REQUIRE(info.os == mystic::hal::OperatingSystem::kLinux);
  REQUIRE(info.variant ==
          static_cast<mystic::hal::OperatingSystemVariant>(mystic::hal::LinuxVariant::kUbuntu));
  REQUIRE(info.version.major == 20);
  REQUIRE(info.version.minor == 4);
  REQUIRE(info.version.patch == 1);
  REQUIRE(info.version.build == 0);
#endif
}

TEST_CASE("OperatingSystemVersion struct initializes correctly.",
          "[mystic][hal][os_detection][os_version_init]") {
  mystic::hal::OperatingSystemVersion version{.major = 1, .minor = 2, .patch = 3, .build = 4};
  REQUIRE(version.major == 1);
  REQUIRE(version.minor == 2);
  REQUIRE(version.patch == 3);
  REQUIRE(version.build == 4);
}

TEST_CASE("GetOperatingSystemVersion() returns a valid version struct.",
          "[mystic][hal][os_detection][get_os_version]") {
  auto version = mystic::hal::runtime::GetOperatingSystemVersion();
  REQUIRE(version.major >= 0);
  REQUIRE(version.minor >= 0);
  REQUIRE(version.patch >= 0);
  REQUIRE(version.build >= 0);
}

TEST_CASE("GetOperatingSystemVariant() returns a valid variant.",
          "[mystic][hal][os_detection][get_os_variant]") {
  auto variant = mystic::hal::runtime::GetOperatingSystemVariant();
  // Since the variant is an enum, we can only check that it's not the default unknown value.
  REQUIRE(variant != static_cast<mystic::hal::OperatingSystemVariant>(0));

// Since these tests rely on a specific OS, we'll skip on local builds.
#ifndef MYSTIC_HAL_LOCAL_BUILD

#  if MYSTIC_OS == MYSTIC_OS_WINDOWS
  // Since we know the GitHub Actions runner is Windows Server 2025, we can check for that.
  // Though it's flaky and must be keep in sync with the actual runner OS.
  REQUIRE(variant == static_cast<mystic::hal::OperatingSystemVariant>(
                         mystic::hal::WindowsVariant::kWindowsServer2025));
#  else
  // Since we know the GitHub Actions runner is Ubuntu, we can check for that.
  REQUIRE(variant ==
          static_cast<mystic::hal::OperatingSystemVariant>(mystic::hal::LinuxVariant::kUbuntu));
#  endif

#endif  // MYSTIC_HAL_LOCAL_BUILD
}

TEST_CASE("GetOperatingSystemInfo() returns a valid info struct.",
          "[mystic][hal][os_detection][get_os_info]") {
  auto info = mystic::hal::runtime::GetOperatingSystemInfo();
  REQUIRE(info.os != mystic::hal::OperatingSystem::kUnknown);
  REQUIRE(info.variant != static_cast<mystic::hal::OperatingSystemVariant>(0));
  REQUIRE(info.version.major >= 0);
  REQUIRE(info.version.minor >= 0);
  REQUIRE(info.version.patch >= 0);
  REQUIRE(info.version.build >= 0);
}
