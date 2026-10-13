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
/// @file detail.cppm
/// @brief This submodule contains private helper functions.
/// @author thedevmystic (Surya) <thedevmystic@gmail>
/// @license Apache-2.0
/// ------------------------------------------------------------------------------------------------

module;

#include <cstring>

export module mystic.hal.detail;

namespace mystic::hal::detail {

/// @brief Helper function to check if a string starts with a given prefix
/// @param str The string to check.
/// @param prefix The prefix to look for.
/// @return True if `str` starts with `prefix`, false otherwise.
export constexpr bool StartsWith(const char* str, const char* prefix) noexcept {
  return std::strncmp(str, prefix, std::strlen(prefix)) == 0;
}

/// @brief Helper function to write an integer to a buffer and return the new buffer position.
/// @param buffer The buffer to write to.
/// @param value The integer value to write.
/// @return Pointer to the position in the buffer after the written integer.
export constexpr char* WriteInt(char* buffer, int value) noexcept {
  if (value == 0) {
    *buffer++ = '0';
    return buffer;
  }

  // Handle negative values
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

/// @brief Helper function to append a string to a buffer safely.
/// @param dest The destination buffer where the string will be appended.
/// @param end The end of the destination buffer (one past the last valid position).
/// @param src The source string to append.
/// @return Pointer to the position in the destination buffer after the appended string.
export constexpr char* AppendString(char* dest, const char* end, const char* src) noexcept {
  if (dest == nullptr || src == nullptr) {
    return dest;  // Return the original buffer if either is null
  }

  while (*src != '\0' && dest < end) {
    *dest++ = *src++;
  }

  return dest;
}

}  // namespace mystic::hal::detail
