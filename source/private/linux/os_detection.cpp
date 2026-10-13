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
/// @brief This file contains implementation of `os_detection.cppm` for Linux.
/// @author thedevmystic (Surya) <thedevmystic@gmail>
/// @license Apache-2.0
/// ------------------------------------------------------------------------------------------------

module;

#include <cstring>
#include <fcntl.h>
#include <sys/utsname.h>
#include <unistd.h>

module mystic.hal.os_detection;

import mystic.hal.detail;

namespace {

// Helper function to parse the version string into OperatingSystemVersion struct.
inline mystic::hal::OperatingSystemVersion ParseVersionString(const char* version_str) noexcept {
  mystic::hal::OperatingSystemVersion version{};
  if (version_str == nullptr) {
    return version;  // Return default version (0.0.0)
  }

  const char* ptr = version_str;

  // Helper lambda to parse an integer from the string and advance the pointer.
  auto parse_int = [](const char*& ptr) -> int {
    int value = 0;
    // Parse the digits
    while (*ptr >= '0' && *ptr <= '9') {
      value = (value * 10) + (*ptr - '0');
      ++ptr;
    }
    // Skip the dot if present
    if (*ptr == '.') {
      ++ptr;
    }
    return value;
  };

  version.major = parse_int(ptr);
  version.minor = parse_int(ptr);
  version.patch = parse_int(ptr);
  version.build = 0;  // Build number is not typically available in Linux, set to 0.

  return version;
}

// Helper function to retrieve id from `/etc/os-release`.
inline bool GetLinuxId(char* out, size_t out_size) noexcept {
  if (out == nullptr || out_size == 0) {
    return false;
  }
  out[0] = '\0';

  int fd = ::open("/etc/os-release", O_RDONLY);
  if (fd < 0) {
    return false;  // Failed to open fd
  }

  char buffer[4096];  // 4KB NOLINT(modernize-avoid-c-arrays)
  ssize_t bytes_read = ::read(fd, buffer, sizeof(buffer) - 1);
  ::close(fd);

  if (bytes_read <= 0) {
    return false;  // Failed to read fd
  }
  buffer[bytes_read] = '\0';

  char* saveptr = nullptr;
  char* line = ::strtok_r(buffer, "\n", &saveptr);
  while (line != nullptr) {
    if (mystic::hal::detail::StartsWith(line, "ID=")) {
      const char* val = line + 3;  // Skip "ID="

      // Strip leading quote if present
      if (*val == '"' || *val == '\'') {
        ++val;
      }

      // Copy, stopping at a trailing quote or end of string
      size_t i = 0;
      while (val[i] != '\0' && val[i] != '"' && val[i] != '\'' && i + 1 < out_size) {
        out[i] = val[i];
        ++i;
      }
      out[i] = '\0';
      return i > 0;
    }
    line = ::strtok_r(nullptr, "\n", &saveptr);
  }
  return false;
}

inline mystic::hal::LinuxVariant ParseLinuxId(const char* id) noexcept {
  if (id == nullptr) {
    return mystic::hal::LinuxVariant::kUnknown;
  }

  if (mystic::hal::detail::StartsWith(id, "debian")) {
    return mystic::hal::LinuxVariant::kDebian;
  } else if (mystic::hal::detail::StartsWith(id, "ubuntu")) {
    return mystic::hal::LinuxVariant::kUbuntu;
  } else if (mystic::hal::detail::StartsWith(id, "linuxmint") ||
             mystic::hal::detail::StartsWith(id, "mint")) {
    return mystic::hal::LinuxVariant::kMint;
  } else if (mystic::hal::detail::StartsWith(id, "kali")) {
    return mystic::hal::LinuxVariant::kKali;
  } else if (mystic::hal::detail::StartsWith(id, "rhel") ||
             mystic::hal::detail::StartsWith(id, "redhat")) {
    return mystic::hal::LinuxVariant::kRHEL;
  } else if (mystic::hal::detail::StartsWith(id, "centos")) {
    return mystic::hal::LinuxVariant::kCentOS;
  } else if (mystic::hal::detail::StartsWith(id, "fedora")) {
    return mystic::hal::LinuxVariant::kFedora;
  } else if (mystic::hal::detail::StartsWith(id, "arch")) {
    return mystic::hal::LinuxVariant::kArch;
  } else if (mystic::hal::detail::StartsWith(id, "manjaro")) {
    return mystic::hal::LinuxVariant::kManjaro;
  } else if (mystic::hal::detail::StartsWith(id, "endeavouros")) {
    return mystic::hal::LinuxVariant::kEndeavourOS;
  } else if (mystic::hal::detail::StartsWith(id, "opensuse-leap") ||
             mystic::hal::detail::StartsWith(id, "opensuseleap")) {
    return mystic::hal::LinuxVariant::kOpenSUSELeap;
  } else if (mystic::hal::detail::StartsWith(id, "opensuse-tumbleweed") ||
             mystic::hal::detail::StartsWith(id, "opensusetumbleweed")) {
    return mystic::hal::LinuxVariant::kOpenSUSETumbleweed;
  } else if (mystic::hal::detail::StartsWith(id, "sles") ||
             mystic::hal::detail::StartsWith(id, "suseenterprise")) {
    return mystic::hal::LinuxVariant::kSUSEEnterprise;
  } else if (mystic::hal::detail::StartsWith(id, "opensuse") ||
             mystic::hal::detail::StartsWith(id, "suse")) {
    return mystic::hal::LinuxVariant::kOpenSUSE;
  } else if (mystic::hal::detail::StartsWith(id, "alpine")) {
    return mystic::hal::LinuxVariant::kAlpine;
  } else if (mystic::hal::detail::StartsWith(id, "gentoo")) {
    return mystic::hal::LinuxVariant::kGentoo;
  } else if (mystic::hal::detail::StartsWith(id, "nixos")) {
    return mystic::hal::LinuxVariant::kNixOS;
  } else {
    return mystic::hal::LinuxVariant::kOther;  // Other known distributions
  }
}

}  // namespace

namespace mystic::hal::runtime {

OperatingSystemVersion GetOperatingSystemVersion() noexcept {
  struct utsname buffer{};
  if (uname(&buffer) != 0) {
    return OperatingSystemVersion{};  // Return default version (0.0.0)
  }
  return ParseVersionString(buffer.release);
}

OperatingSystemVariant GetOperatingSystemVariant() noexcept {
  char id[64];  // NOLINT(modernize-avoid-c-arrays)
  if (!GetLinuxId(id, sizeof(id))) {
    return static_cast<OperatingSystemVariant>(LinuxVariant::kUnknown);
  }
  LinuxVariant linux_variant = ParseLinuxId(id);
  return static_cast<OperatingSystemVariant>(linux_variant);
}

OperatingSystemInfo GetOperatingSystemInfo() noexcept {
  OperatingSystemInfo info;
  info.os = GetOperatingSystem();
  info.variant = GetOperatingSystemVariant();
  info.version = GetOperatingSystemVersion();
  return info;
}

}  // namespace mystic::hal::runtime
