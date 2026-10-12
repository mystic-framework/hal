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
/// @file os_detection.cpp
/// @brief This file contains implementation of `os_detection.cppm` for Windows.
/// @author thedevmystic (Surya) <thedevmystic@gmail>
/// @license Apache-2.0
/// ------------------------------------------------------------------------------------------------

module;

#include <windows.h>

module mystic.hal.os_detection;

namespace {

using RtlGetVersionPtr = LONG(WINAPI*)(PRTL_OSVERSIONINFOW);

// NOLINTBEGIN(readability-identifier-naming)
struct RTL_OSVERSIONINFOXW {
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  WCHAR szCSDVersion[128];  // NOLINT(modernize-avoid-c-arrays)
  WORD wServicePackMajor;
  WORD wServicePackMinor;
  WORD wSuiteMask;
  BYTE wProductType;
  BYTE wReserved;
};
// NOLINTEND(readability-identifier-naming)

// Helper function to write an integer to a buffer and return the new buffer position.
inline char* WriteInt(char* buffer, int value) noexcept {
  if (value == 0) {
    *buffer++ = '0';
    return buffer;
  }

  // Versions are typically non-negative, but we handle negative values just in case.
  if (value < 0) {
    *buffer++ = '-';
    value = -value;
  }

  char temp[12];  // Enough to hold int max value. NOLINT(modernize-avoid-c-arrays)
  int pos = 0;

  while (value > 0) {
    temp[pos++] = static_cast<char>('0' + (value % 10));
    value /= 10;
  }

  // Reverse the digits into the buffer
  for (int i = pos - 1; i >= 0; --i) {
    *buffer++ = temp[i];
  }

  return buffer;
}

// Helper to get RtlGetVersion function pointer
RtlGetVersionPtr GetRtlGetVersion() {
  HMODULE h_ntdll = ::GetModuleHandleW(L"ntdll.dll");
  if (!h_ntdll) {
    return nullptr;
  }
  auto rtl_get_version =
      reinterpret_cast<RtlGetVersionPtr>(::GetProcAddress(h_ntdll, "RtlGetVersion"));
  if (!rtl_get_version) {
    return nullptr;
  }
  return rtl_get_version;
}

// Helper to get RTL_OSVERSIONINFOXW structure
bool GetOSVersionInfo(RTL_OSVERSIONINFOXW& osvi) {
  RtlGetVersionPtr rtl_get_version = GetRtlGetVersion();
  if (!rtl_get_version) {
    return false;
  }
  osvi.dwOSVersionInfoSize = sizeof(osvi);
  return rtl_get_version(reinterpret_cast<PRTL_OSVERSIONINFOW>(&osvi)) == 0;
}

// Helper to check if the OS is a Windows Server version
bool IsWindowsServer(const RTL_OSVERSIONINFOXW& osvi) {
  return osvi.wProductType == VER_NT_SERVER || osvi.wProductType == VER_NT_DOMAIN_CONTROLLER;
}

// Helper to parse the Windows version from RTL_OSVERSIONINFOXW
mystic::hal::OperatingSystemVersion ParseWindowsVersion(const RTL_OSVERSIONINFOXW& osvi) {
  return mystic::hal::OperatingSystemVersion{
      .major = static_cast<int>(osvi.dwMajorVersion),
      .minor = static_cast<int>(osvi.dwMinorVersion),
      .patch = 0,  // Windows doesn't typically use patch numbers
      .build = static_cast<int>(osvi.dwBuildNumber),
  };
}

// Helper to determine the Windows variant from RTL_OSVERSIONINFOXW
mystic::hal::WindowsVariant DetermineWindowsVariant(const RTL_OSVERSIONINFOXW& osvi) {
  bool is_server = IsWindowsServer(osvi);
  switch (osvi.dwMajorVersion) {
    case 6:
      switch (osvi.dwMinorVersion) {
        case 1:
          return is_server ? mystic::hal::WindowsVariant::kWindowsServer2008
                           : mystic::hal::WindowsVariant::kWindows7;
        case 2:
          return is_server ? mystic::hal::WindowsVariant::kWindowsServer2012
                           : mystic::hal::WindowsVariant::kWindows8;
        case 3:
          return is_server ? mystic::hal::WindowsVariant::kWindowsServer2012
                           : mystic::hal::WindowsVariant::kWindows81;
        default:
          return mystic::hal::WindowsVariant::kOther;
      }
    case 10:
      if (is_server) {
        if (osvi.dwBuildNumber >= 26100) {
          return mystic::hal::WindowsVariant::kWindowsServer2025;
        } else if (osvi.dwBuildNumber >= 20348) {
          return mystic::hal::WindowsVariant::kWindowsServer2022;
        } else if (osvi.dwBuildNumber >= 17763) {
          return mystic::hal::WindowsVariant::kWindowsServer2019;
        } else if (osvi.dwBuildNumber >= 14393) {
          return mystic::hal::WindowsVariant::kWindowsServer2016;
        } else if (osvi.dwBuildNumber >= 9200) {
          return mystic::hal::WindowsVariant::kWindowsServer2012;
        } else if (osvi.dwBuildNumber >= 6001) {
          return mystic::hal::WindowsVariant::kWindowsServer2008;
        } else {
          return mystic::hal::WindowsVariant::kOther;
        }
      } else {
        if (osvi.dwBuildNumber >= 22000) {
          return mystic::hal::WindowsVariant::kWindows11;
        } else {
          return mystic::hal::WindowsVariant::kWindows10;
        }
      }

    default:
      return mystic::hal::OperatingSystemVariant::kOther;
  }
}

}  // namespace

namespace mystic::hal {

/// --- String Conversion Functions ---

const char* ToString(const OperatingSystemVersion& version) noexcept {
  thread_local char buffer[32];  // NOLINT(modernize-avoid-c-arrays)
  char* ptr = buffer;
  ptr = WriteInt(ptr, version.major);
  *ptr++ = '.';
  ptr = WriteInt(ptr, version.minor);
  *ptr++ = '.';
  ptr = WriteInt(ptr, version.build);
  // Windows doesn't typically use patch numbers, so we'll leave it.
  *ptr = '\0';
  return buffer;
}

const char* ToString(const OperatingSystemInfo& info) noexcept {
  thread_local char buffer[128];  // NOLINT(modernize-avoid-c-arrays)
  char* ptr = buffer;
  char* const end = buffer + sizeof(buffer) - 1;

  // Helper lambda to append a string to the buffer safely
  auto append_str = [&](const char* str) {
    if (str == nullptr) {
      return;
    }
    while (*str != '\0' && ptr < end) {
      *ptr++ = *str++;
    }
  };

  append_str(ToString(info.os));
  append_str(" (");
  append_str(ToString(info.variant));
  append_str(") Version: ");
  append_str(ToString(info.version));
  *ptr = '\0';

  return buffer;
}

namespace runtime {

OperatingSystemVersion GetOperatingSystemVersion() noexcept {
  RTL_OSVERSIONINFOXW osvi{};
  if (!GetOSVersionInfo(osvi)) {
    return OperatingSystemVersion{.major = 0, .minor = 0, .patch = 0, .build = 0};
  }
  return ParseWindowsVersion(osvi);
}

OperatingSystemVariant GetOperatingSystemVariant() noexcept {
  RTL_OSVERSIONINFOXW osvi{};
  if (!GetOSVersionInfo(osvi)) {
    return OperatingSystemVariant::kUnknown;
  }
  return DetermineWindowsVariant(osvi);
}

OperatingSystemInfo GetOperatingSystemInfo() noexcept {
  OperatingSystemInfo info;
  info.os = GetOperatingSystem();
  info.variant = GetOperatingSystemVariant();
  info.version = GetOperatingSystemVersion();
  return info;
}

}  // namespace runtime

}  // namespace mystic::hal
