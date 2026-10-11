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
/// @file os_detection.hpp
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
/// ------------------------------------------------------------------------------------------------

module;

#include "hal/os_detection_macros.hpp"

#if MYSTIC_OS == MYSTIC_OS_WINDOWS
#  include <windows.h>
#elif MYSTIC_OS == MYSTIC_OS_LINUX
#  include <fcntl.h>
#  include <unistd.h>
#endif

export module mystic.hal.os_detection;

namespace mystic::hal {

/// @brief Enum representing different operating systems.
export enum class OperatingSystem : int {  // NOLINT(performance-enum-size)
  kWindows = MYSTIC_OS_WINDOWS,            ///< Represents the Windows operating system.
  kMacOS = MYSTIC_OS_MACOS,                ///< Represents the macOS operating system.
  kLinux = MYSTIC_OS_LINUX,                ///< Represents the Linux operating system
  kUnix = MYSTIC_OS_UNIX,                  ///< Represents other Unix-like operating systems.
  kUnknown = MYSTIC_OS_UNKNOWN,            ///< Represents an unknown operating system.
};

/// @brief Enum representing different version of Windows OS.
export enum class WindowsVersion : int {  // NOLINT(performance-enum-size)
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

/// @brief Enum representing different versions of Linux OS.
export enum class LinuxVersion : int {  // NOLINT(performance-enum-size)
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

namespace compile_time {

/// @brief Returns the operating system at compile time.
/// @return The operating system at compile time.
export constexpr OperatingSystem GetOperatingSystem() noexcept {
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
  return OperatingSystem::kWindows;
#elif MYSTIC_OS == MYSTIC_OS_MACOS
  return OperatingSystem::kMacOS;
#elif MYSTIC_OS == MYSTIC_OS_LINUX
  return OperatingSystem::kLinux;
#elif MYSTIC_OS == MYSTIC_OS_UNIX
  return OperatingSystem::kUnix;
#else
  return OperatingSystem::kUnknown;
#endif
}

/// @brief Returns a string representation of the operating system at compile time.
/// @return A string representation of the operating system at compile time.
export constexpr const char* GetOperatingSystemAsString() noexcept {
  switch (GetOperatingSystem()) {
    case OperatingSystem::kWindows:
      return "Windows";
    case OperatingSystem::kMacOS:
      return "macOS";
    case OperatingSystem::kLinux:
      return "Linux";
    case OperatingSystem::kUnix:
      return "Unix";
    default:
      return "Unknown";
  }
}

}  // namespace compile_time

namespace runtime {

/// @brief Returns the operating system at runtime. (This is same as compile time)
/// @return The operating system at runtime.
export constexpr OperatingSystem GetOperatingSystem() noexcept {
  return compile_time::GetOperatingSystem();
}

/// @brief Returns a string representation of the operating system at runtime. (This is same as
/// compile time)
/// @return A string representation of the operating system at runtime.
export constexpr const char* GetOperatingSystemAsString() noexcept {
  return compile_time::GetOperatingSystemAsString();
}

/// @brief Returns the version of Windows OS at runtime.
/// @return The version of Windows OS at runtime.
export inline WindowsVersion GetWindowsVersion() noexcept {
#if MYSTIC_OS == MYSTIC_OS_WINDOWS
  HMODULE h_ntdll = ::GetModuleHandleW(L"ntdll.dll");
  if (!h_ntdll) {
    return WindowsVersion::kUnknown;
  }

  auto RtlGetVersion =
      reinterpret_cast<RtlGetVersionPtr>(::GetProcAddress(h_ntdll, "RtlGetVersion"));
  if (!RtlGetVersion) {
    return WindowsVersion::kUnknown;
  }

  RTL_OSVERSIONINFOXW osvi{};
  osvi.dwOSVersionInfoSize = sizeof(osvi);
  if (RtlGetVersion(reinterpret_cast<PRTL_OSVERSIONINFOW>(&osvi)) != 0) {
    return WindowsVersion::kUnknown;
  }

  const bool is_server =
      (osvi.wProductType == VER_NT_SERVER || osvi.wProductType == VER_NT_DOMAIN_CONTROLLER);

  switch (osvi.dwMajorVersion) {
    case 6:
      switch (osvi.dwMinorVersion) {
        case 1:
          return is_server ? WindowsVersion::kWindowsServer2008 : WindowsVersion::kWindows7;  // R2
        case 2:
          return is_server ? WindowsVersion::kWindowsServer2012 : WindowsVersion::kWindows8;  // R1
        case 3:
          return is_server ? WindowsVersion::kWindowsServer2012 : WindowsVersion::kWindows81;  // R2
        default:
          return WindowsVersion::kOther;
      }
    case 10:
      if (is_server) {
        if (osvi.dwBuildNumber >= 26100) {
          return WindowsVersion::kWindowsServer2025;
        } else if (osvi.dwBuildNumber >= 20348) {
          return WindowsVersion::kWindowsServer2022;
        } else if (osvi.dwBuildNumber >= 17763) {
          return WindowsVersion::kWindowsServer2019;
        } else if (osvi.dwBuildNumber >= 14393) {
          return WindowsVersion::kWindowsServer2016;
        } else if (osvi.dwBuildNumber >= 9200) {
          return WindowsVersion::kWindowsServer2012;
        } else if (osvi.dwBuildNumber >= 6001) {
          return WindowsVersion::kWindowsServer2008;
        } else {
          return WindowsVersion::kOther;
        }
      } else {
        if (osvi.dwBuildNumber >= 22000) {
          return WindowsVersion::kWindows11;
        } else {
          return WindowsVersion::kWindows10;
        }
      }
    default:
      return WindowsVersion::kOther;
  }
#else
  return WindowsVersion::kUnknown;
#endif
}

/// @brief Returns a string representation of the Windows version at runtime.
/// @return A string representation of the Windows version at runtime.
export inline const char* GetWindowsVersionAsString() noexcept {
  switch (GetWindowsVersion()) {
    case WindowsVersion::kWindows7:
      return "Windows 7";
    case WindowsVersion::kWindows8:
      return "Windows 8";
    case WindowsVersion::kWindows81:
      return "Windows 8.1";
    case WindowsVersion::kWindows10:
      return "Windows 10";
    case WindowsVersion::kWindows11:
      return "Windows 11";
    case WindowsVersion::kWindowsServer2008:
      return "Windows Server 2008";
    case WindowsVersion::kWindowsServer2012:
      return "Windows Server 2012";
    case WindowsVersion::kWindowsServer2016:
      return "Windows Server 2016";
    case WindowsVersion::kWindowsServer2019:
      return "Windows Server 2019";
    case WindowsVersion::kWindowsServer2022:
      return "Windows Server 2022";
    case WindowsVersion::kWindowsServer2025:
      return "Windows Server 2025";
    case WindowsVersion::kOther:
      return "Other";
    default:
      return "Unknown";
  }
}

/// @brief Returns the version of Linux OS at runtime.
/// @return The version of Linux OS at runtime.
export inline LinuxVersion GetLinuxVersion() noexcept {
#if MYSTIC_OS == MYSTIC_OS_LINUX
  int fd = ::open("/etc/os-release", O_RDONLY);
  if (fd < 0) {
    return LinuxVersion::kUnknown;
  }

  char buffer[4096];  // 4KB NOLINT(modernize-avoid-c-arrays)
  ssize_t bytes_read = ::read(fd, buffer, sizeof(buffer) - 1);
  ::close(fd);

  if (bytes_read <= 0) {
    return LinuxVersion::kUnknown;
  }
  buffer[bytes_read] = '\0';

  // Helper lambda to go to next line
  auto next_line = [](char*& str) -> char* {
    if (str == nullptr || *str == '\0') {
      return nullptr;
    }

    while (*str != '\0' && *str != '\n') {
      ++str;
    }

    if (*str != '\0') {
      *str = '\0';  // Null-terminate the current line
      ++str;        // Move to the start of the next line
    } else {
      str = nullptr;  // No more lines
    }

    return str;
  };

  // Helper to get string length (strlen)
  auto str_length = [](const char* str) -> size_t {
    size_t len = 0;
    while (str[len] != '\0') {
      ++len;
    }
    return len;
  };

  // Helper to check if a line starts with a given prefix
  auto starts_with = [](const char* str, const char* prefix) -> bool {
    while (*prefix && *str == *prefix) {
      ++str;
      ++prefix;
    }
    return *prefix == '\0';
  };

  char* line = buffer;
  while (next_line(line)) {
    if (starts_with(line, "ID=")) {
      char* val = line + 3;  // Skip "ID="

      // Strip leading quotes if present
      if (*val == '"' || *val == '\'') {
        ++val;
      }

      // Strip trailing quotes if present
      size_t len = str_length(val);
      if (len > 0 && (val[len - 1] == '"' || val[len - 1] == '\'')) {
        val[len - 1] = '\0';
      }

      // Map the ID to LinuxVersion enum
      if (starts_with(val, "debian")) {
        return LinuxVersion::kDebian;
      } else if (starts_with(val, "ubuntu")) {
        return LinuxVersion::kUbuntu;
      } else if (starts_with(val, "linuxmint") || starts_with(val, "mint")) {
        return LinuxVersion::kMint;
      } else if (starts_with(val, "kali")) {
        return LinuxVersion::kKali;
      } else if (starts_with(val, "rhel") || starts_with(val, "redhat")) {
        return LinuxVersion::kRHEL;
      } else if (starts_with(val, "centos")) {
        return LinuxVersion::kCentOS;
      } else if (starts_with(val, "fedora")) {
        return LinuxVersion::kFedora;
      } else if (starts_with(val, "arch")) {
        return LinuxVersion::kArch;
      } else if (starts_with(val, "manjaro")) {
        return LinuxVersion::kManjaro;
      } else if (starts_with(val, "endeavouros")) {
        return LinuxVersion::kEndeavourOS;
      } else if (starts_with(val, "opensuse-leap") || starts_with(val, "opensuseleap")) {
        return LinuxVersion::kOpenSUSELeap;
      } else if (starts_with(val, "opensuse-tumbleweed") ||
                 starts_with(val, "opensusetumbleweed")) {
        return LinuxVersion::kOpenSUSETumbleweed;
      } else if (starts_with(val, "opensuse") || starts_with(val, "suse")) {
        return LinuxVersion::kOpenSUSE;
      } else if (starts_with(val, "sles") || starts_with(val, "suseenterprise")) {
        return LinuxVersion::kSUSEEnterprise;
      } else if (starts_with(val, "alpine")) {
        return LinuxVersion::kAlpine;
      } else if (starts_with(val, "gentoo")) {
        return LinuxVersion::kGentoo;
      } else if (starts_with(val, "nixos")) {
        return LinuxVersion::kNixOS;
      } else {
        return LinuxVersion::kOther;  // Other known distributions
      }
    }
  }

  return LinuxVersion::kUnknown;
#else
  return LinuxVersion::kUnknown;
#endif
}

/// @brief Returns a string representation of the Linux version at runtime.
/// @return A string representation of the Linux version at runtime.
export inline const char* GetLinuxVersionAsString() noexcept {
  switch (GetLinuxVersion()) {
    case LinuxVersion::kDebian:
      return "Debian";
    case LinuxVersion::kUbuntu:
      return "Ubuntu";
    case LinuxVersion::kMint:
      return "Linux Mint";
    case LinuxVersion::kKali:
      return "Kali Linux";
    case LinuxVersion::kRHEL:
      return "Red Hat Enterprise Linux";
    case LinuxVersion::kCentOS:
      return "CentOS";
    case LinuxVersion::kFedora:
      return "Fedora";
    case LinuxVersion::kArch:
      return "Arch Linux";
    case LinuxVersion::kManjaro:
      return "Manjaro";
    case LinuxVersion::kEndeavourOS:
      return "EndeavourOS";
    case LinuxVersion::kOpenSUSE:
      return "openSUSE";
    case LinuxVersion::kOpenSUSELeap:
      return "openSUSE Leap";
    case LinuxVersion::kOpenSUSETumbleweed:
      return "openSUSE Tumbleweed";
    case LinuxVersion::kSUSEEnterprise:
      return "SUSE Linux Enterprise";
    case LinuxVersion::kAlpine:
      return "Alpine Linux";
    case LinuxVersion::kGentoo:
      return "Gentoo Linux";
    case LinuxVersion::kNixOS:
      return "NixOS";
    case LinuxVersion::kOther:
      return "Other";
    default:
      return "Unknown";
  }
}

}  // namespace runtime

}  // namespace mystic::hal
