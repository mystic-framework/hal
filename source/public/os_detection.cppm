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
/// @file os_detection.cppm
/// @brief This file contains runtime logic for detecting the OS.
/// @author thedevmystic (Surya) <thedevmystic@gmail>
/// @license Apache-2.0
///
/// This file exposes two sets of functions:
/// - mystic::hal::compile_time: These functions expose the same macros defined in
///   `os_detection_macros.hpp`, but as constexpr functions. This allows for compile-time
///   evaluation of the OS.
/// - mystic::hal::runtime: These functions expose runtime logic for detecting the
///   os. This detects the actual os the program is running on, like Windows 10 vs 11, etc.
///
/// @sa os_detection_macros.hpp for preprocessor macros.
/// @sa os_detection.cpp for implementation.
/// ------------------------------------------------------------------------------------------------

module;

#include "hal/hal_export_marker.hpp"
#include "hal/os_detection_macros.hpp"

export module mystic.hal.os_detection;

import mystic.hal.detail;

namespace mystic::hal {

/// @brief Enum representing different operating systems.
export enum class OperatingSystem : int {  // NOLINT(performance-enum-size)
  kWindows = 0,                            ///< Represents the Windows operating system.
  kMacOS,                                  ///< Represents the macOS operating system.
  kLinux,                                  ///< Represents the Linux operating system
  kUnknown,                                ///< Represents an unknown operating system.
};

/// @brief Enum representing different variants of Windows OS.
export enum class WindowsVariant : int {  // NOLINT(performance-enum-size)
  kUnknown = 0,                           ///< Represents an unknown version of Windows.
  kWindows7,                              ///< Represents Windows 7.
  kWindows8,                              ///< Represents Windows 8.
  kWindows81,                             ///< Represents Windows 8.1.
  kWindows10,                             ///< Represents Windows 10.
  kWindows11,                             ///< Represents Windows 11.
  kWindowsServer2008,                     ///< Represents Windows Server 2008.
  kWindowsServer2012,                     ///< Represents Windows Server 2012.
  kWindowsServer2016,                     ///< Represents Windows Server 2016.
  kWindowsServer2019,                     ///< Represents Windows Server 2019.
  kWindowsServer2022,                     ///< Represents Windows Server 2022.
  kWindowsServer2025,                     ///< Represents Windows Server 2025.
  kOther,  ///< Represents other versions of Windows not explicitly listed.
};

/// @brief Enum representing different variants of Linux OS.
export enum class LinuxVariant : int {  // NOLINT(performance-enum-size)
  kUnknown = 0,                         ///< Represents an unknown version of Linux.
  // Debian Family
  kDebian,  ///< Represents Debian Linux.
  kUbuntu,  ///< Represents Ubuntu Linux.
  kMint,    ///< Represents Linux Mint.
  kKali,    ///< Represents Kali Linux.

  // Red Hat Family
  kRHEL,    ///< Represents Red Hat Enterprise Linux.
  kCentOS,  ///< Represents CentOS Linux.
  kFedora,  ///< Represents Fedora Linux.

  // Arch Family
  kArch,         ///< Represents Arch Linux.
  kManjaro,      ///< Represents Manjaro Linux.
  kEndeavourOS,  ///< Represents EndeavourOS Linux.

  // SUSE Family
  kOpenSUSE,            ///< Represents openSUSE Linux.
  kOpenSUSELeap,        ///< Represents openSUSE Leap Linux.
  kSUSEEnterprise,      ///< Represents SUSE Linux Enterprise.
  kOpenSUSETumbleweed,  ///< Represents openSUSE Tumbleweed Linux.

  // Independent Distributions
  kAlpine,  ///< Represents Alpine Linux.
  kGentoo,  ///< Represents Gentoo Linux.
  kNixOS,   ///< Represents NixOS Linux.

  kOther,  ///< Represents other versions of Linux not explicitly listed.
};

/// @brief Alias for the variant based on the operating system type.
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
export using OperatingSystemVariant = WindowsVariant;
#else
export using OperatingSystemVariant = LinuxVariant;
#endif

/// @brief Struct to represent the OS version information.
/// On Windows, the version is represented as Major.Minor.Build, while on Linux, it is represented
/// as Major.Minor.Patch.
export struct MYSTIC_HAL_INTERNAL_EXPORT OperatingSystemVersion {
  int major = 0;  ///< Major version number.
  int minor = 0;  ///< Minor version number.
  int patch = 0;  ///< Patch version number (only relevant for Linux. In Windows, it is set to 0.)
  int build = 0;  ///< Build number (only relevant for Windows. In Linux, it is set to 0.)
};

/// @brief Operating system information.
export struct MYSTIC_HAL_INTERNAL_EXPORT OperatingSystemInfo {
  OperatingSystem os;              ///< The operating system type.
  OperatingSystemVariant variant;  ///< The operating system variant.
  OperatingSystemVersion version;  ///< The operating system version.

  /// @brief Default constructor initializing to unknown values.
  OperatingSystemInfo()
      : os(OperatingSystem::kUnknown),
        variant(static_cast<OperatingSystemVariant>(0)),
        version({.major = 0, .minor = 0, .patch = 0, .build = 0}) {}

  /// @brief Constructor to initialize the OperatingSystemInfo.
  /// @param os The operating system type.
  /// @param version The operating system version.
  /// @param variant The operating system variant.
  OperatingSystemInfo(OperatingSystem os,
                      OperatingSystemVariant variant,
                      OperatingSystemVersion version)
      : os(os), variant(variant), version(version) {}
};

/// --- String Conversion Functions ---

/// @brief Converts the OperatingSystem enum to a string representation.
/// @param os The operating system enum value.
/// @return A string representation of the operating system.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr const char* ToString(OperatingSystem os) noexcept {
  switch (os) {
    case OperatingSystem::kWindows:
      return "Windows";
    case OperatingSystem::kMacOS:
      return "macOS";
    case OperatingSystem::kLinux:
      return "Linux";
    case OperatingSystem::kUnknown:
    default:
      return "Unknown";
  }
}

/// @brief Converts the WindowsVariant enum to a string representation.
/// @param variant The Windows variant enum value.
/// @return A string representation of the Windows variant.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr const char* ToString(WindowsVariant variant) noexcept {
  switch (variant) {
    case WindowsVariant::kWindows7:
      return "Windows 7";
    case WindowsVariant::kWindows8:
      return "Windows 8";
    case WindowsVariant::kWindows81:
      return "Windows 8.1";
    case WindowsVariant::kWindows10:
      return "Windows 10";
    case WindowsVariant::kWindows11:
      return "Windows 11";
    case WindowsVariant::kWindowsServer2008:
      return "Windows Server 2008";
    case WindowsVariant::kWindowsServer2012:
      return "Windows Server 2012";
    case WindowsVariant::kWindowsServer2016:
      return "Windows Server 2016";
    case WindowsVariant::kWindowsServer2019:
      return "Windows Server 2019";
    case WindowsVariant::kWindowsServer2022:
      return "Windows Server 2022";
    case WindowsVariant::kWindowsServer2025:
      return "Windows Server 2025";
    case WindowsVariant::kOther:
      return "Other Windows Version";
    case WindowsVariant::kUnknown:
    default:
      return "Unknown Windows Version";
  }
}

/// @brief Converts the LinuxVariant enum to a string representation.
/// @param variant The Linux variant enum value.
/// @return A string representation of the Linux variant.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr const char* ToString(LinuxVariant variant) noexcept {
  switch (variant) {
    case LinuxVariant::kDebian:
      return "Debian";
    case LinuxVariant::kUbuntu:
      return "Ubuntu";
    case LinuxVariant::kMint:
      return "Linux Mint";
    case LinuxVariant::kKali:
      return "Kali Linux";
    case LinuxVariant::kRHEL:
      return "Red Hat Enterprise Linux";
    case LinuxVariant::kCentOS:
      return "CentOS";
    case LinuxVariant::kFedora:
      return "Fedora";
    case LinuxVariant::kArch:
      return "Arch Linux";
    case LinuxVariant::kManjaro:
      return "Manjaro";
    case LinuxVariant::kEndeavourOS:
      return "EndeavourOS";
    case LinuxVariant::kOpenSUSE:
      return "openSUSE";
    case LinuxVariant::kOpenSUSELeap:
      return "openSUSE Leap";
    case LinuxVariant::kSUSEEnterprise:
      return "SUSE Enterprise";
    case LinuxVariant::kOpenSUSETumbleweed:
      return "openSUSE Tumbleweed";
    case LinuxVariant::kAlpine:
      return "Alpine Linux";
    case LinuxVariant::kGentoo:
      return "Gentoo";
    case LinuxVariant::kNixOS:
      return "NixOS";
    case LinuxVariant::kOther:
      return "Other Linux Distribution";
    case LinuxVariant::kUnknown:
    default:
      return "Unknown Linux Distribution";
  }
}

/// @brief Converts the OperatingSystemVersion struct to a string representation.
/// @param version The OperatingSystemVersion struct.
/// @return A string representation of the operating system version.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr const char* ToString(
    const OperatingSystemVersion& version) noexcept {
  thread_local char buffer[32];  // NOLINT(modernize-avoid-c-arrays)
  char* ptr = buffer;
  ptr = mystic::hal::detail::WriteInt(ptr, version.major);
  *ptr++ = '.';
  ptr = mystic::hal::detail::WriteInt(ptr, version.minor);
  *ptr++ = '.';
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
  // Windows uses build numbers and doesn't include patch numbers.
  ptr = mystic::hal::detail::WriteInt(ptr, version.build);
#else
  // Linux uses patch numbers and doesn't include build numbers.
  ptr = mystic::hal::detail::WriteInt(ptr, version.patch);
#endif
  *ptr = '\0';
  return buffer;
}

/// @brief Converts the OperatingSystemInfo struct to a string representation.
/// @param info The OperatingSystemInfo struct.
/// @return A string representation of the operating system information.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr const char* ToString(
    const OperatingSystemInfo& info) noexcept {
  thread_local char buffer[128];  // NOLINT(modernize-avoid-c-arrays)
  char* ptr = buffer;
  const char* end = buffer + sizeof(buffer) - 1;  // Reserve space for null terminator

  ptr = mystic::hal::detail::AppendString(ptr, end, ToString(info.os));
  ptr = mystic::hal::detail::AppendString(ptr, end, " (");
  ptr = mystic::hal::detail::AppendString(ptr, end, ToString(info.variant));
  ptr = mystic::hal::detail::AppendString(ptr, end, ") Version: ");
  ptr = mystic::hal::detail::AppendString(ptr, end, ToString(info.version));
  *ptr = '\0';

  return buffer;
}

namespace compile_time {

/// @brief Returns the operating system type at compile time.
/// @return The operating system type.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr OperatingSystem GetOperatingSystem() noexcept {
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
  return OperatingSystem::kWindows;
#elif MYSTIC_OS == MYSTIC_OS_MACOS
  return OperatingSystem::kMacOS;
#elif MYSTIC_OS == MYSTIC_OS_LINUX
  return OperatingSystem::kLinux;
#else
  return OperatingSystem::kUnknown;
#endif
}

}  // namespace compile_time

namespace runtime {

/// @brief Returns the operating system type at runtime. (This is same as compile time, since the
/// underlying OS cannot change at runtime.)
/// @return The operating system type.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr OperatingSystem GetOperatingSystem() noexcept {
  return compile_time::GetOperatingSystem();
}

/// @brief Returns the operating system version at runtime.
/// @return The operating system version.
export MYSTIC_HAL_INTERNAL_EXPORT OperatingSystemVersion GetOperatingSystemVersion() noexcept;

/// @brief Returns the operating system variant at runtime.
/// @return The operating system variant.
export MYSTIC_HAL_INTERNAL_EXPORT OperatingSystemVariant GetOperatingSystemVariant() noexcept;

/// @brief Returns the operating system information at runtime.
/// @return The operating system information.
export MYSTIC_HAL_INTERNAL_EXPORT OperatingSystemInfo GetOperatingSystemInfo() noexcept;

}  // namespace runtime

}  // namespace mystic::hal
