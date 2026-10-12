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

#include <fcntl.h>
#include <cstring>
#include <sys/utsname.h>
#include <unistd.h>

module mystic.hal.os_detection;

namespace {

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
inline const char* GetLinuxId() noexcept {
  int fd = ::open("/etc/os-release", O_RDONLY);
  if (fd < 0) {
    return nullptr;  // Failed to open fd
  }

  char buffer[4096];  // 4KB NOLINT(modernize-avoid-c-arrays)
  ssize_t bytes_read = ::read(fd, buffer, sizeof(buffer) - 1);
  ::close(fd);

  if (bytes_read <= 0) {
    return nullptr;  // Failed to read fd
  }
  buffer[bytes_read] = '\0';

  // Helper lambda to check if a string starts with a given prefix
  auto starts_with = [](const char* str, const char* prefix) -> bool {
    return std::strncmp(str, prefix, std::strlen(prefix)) == 0;
  };

  char* saveptr = nullptr;
  char* line = ::strtok_r(buffer, "\n", &saveptr);
  while (line != nullptr) {
    if (starts_with(line, "ID=")) {
      char* val = line + 3;  // Skip "ID="

      // Strip leading quotes if present
      if (*val == '"' || *val == '\'') {
        ++val;
      }

      // Strip trailing quotes if present
      size_t len = std::strlen(val);
      if (len > 0 && (val[len - 1] == '"' || val[len - 1] == '\'')) {
        val[len - 1] = '\0';
      }

      return val;
    }

    line = ::strtok_r(nullptr, "\n", &saveptr);
  }
  return nullptr;
}

inline mystic::hal::LinuxVariant ParseLinuxId(const char* id) noexcept {
  if (id == nullptr) {
    return mystic::hal::LinuxVariant::kUnknown;
  }

  // Helper lambda to check if a string starts with a given prefix
  auto starts_with = [](const char* str, const char* prefix) -> bool {
    return std::strncmp(str, prefix, std::strlen(prefix)) == 0;
  };

  if (starts_with(id, "debian")) {
    return mystic::hal::LinuxVariant::kDebian;
  } else if (starts_with(id, "ubuntu")) {
    return mystic::hal::LinuxVariant::kUbuntu;
  } else if (starts_with(id, "linuxmint") || starts_with(id, "mint")) {
    return mystic::hal::LinuxVariant::kMint;
  } else if (starts_with(id, "kali")) {
    return mystic::hal::LinuxVariant::kKali;
  } else if (starts_with(id, "rhel") || starts_with(id, "redhat")) {
    return mystic::hal::LinuxVariant::kRHEL;
  } else if (starts_with(id, "centos")) {
    return mystic::hal::LinuxVariant::kCentOS;
  } else if (starts_with(id, "fedora")) {
    return mystic::hal::LinuxVariant::kFedora;
  } else if (starts_with(id, "arch")) {
    return mystic::hal::LinuxVariant::kArch;
  } else if (starts_with(id, "manjaro")) {
    return mystic::hal::LinuxVariant::kManjaro;
  } else if (starts_with(id, "endeavouros")) {
    return mystic::hal::LinuxVariant::kEndeavourOS;
  } else if (starts_with(id, "opensuse-leap") || starts_with(id, "opensuseleap")) {
    return mystic::hal::LinuxVariant::kOpenSUSELeap;
  } else if (starts_with(id, "opensuse-tumbleweed") || starts_with(id, "opensusetumbleweed")) {
    return mystic::hal::LinuxVariant::kOpenSUSETumbleweed;
  } else if (starts_with(id, "sles") || starts_with(id, "suseenterprise")) {
    return mystic::hal::LinuxVariant::kSUSEEnterprise;
  } else if (starts_with(id, "opensuse") || starts_with(id, "suse")) {
    return mystic::hal::LinuxVariant::kOpenSUSE;
  } else if (starts_with(id, "alpine")) {
    return mystic::hal::LinuxVariant::kAlpine;
  } else if (starts_with(id, "gentoo")) {
    return mystic::hal::LinuxVariant::kGentoo;
  } else if (starts_with(id, "nixos")) {
    return mystic::hal::LinuxVariant::kNixOS;
  } else {
    return mystic::hal::LinuxVariant::kOther;  // Other known distributions
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
  ptr = WriteInt(ptr, version.patch);
  // Since Linux doesn't typically have a build number, we won't append it here.
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
  struct utsname buffer{};
  if (uname(&buffer) != 0) {
    return OperatingSystemVersion{};  // Return default version (0.0.0)
  }
  return ParseVersionString(buffer.release);
}

OperatingSystemVariant GetOperatingSystemVariant() noexcept {
  const char* id = GetLinuxId();
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

}  // namespace runtime

}  // namespace mystic::hal
