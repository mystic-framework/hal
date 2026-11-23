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
/// @file compiler_detection.cppm
/// @brief This file contains functions exposing compiler detection macros as constexpr functions,
/// allowing for compile-time evaluation of the OS.
/// @author thedevmystic (Surya) <thedevmystic@gmail>
/// @license Apache-2.0
///
/// This file exposes only compile-time functions:
/// - mystic::hal::compile_time: These functions expose the same macros defined in
///   `compiler_detection_macros.hpp`, but as constexpr functions. This allows for compile-time
///   evaluation of the compiler.
///
/// This does not expose runtime functions since compilers cannot change in runtime.
///
/// @sa compiler_detection_macros.hpp for preprocessor macros.
/// ------------------------------------------------------------------------------------------------

module;

#include "hal/compiler_detection_macros.hpp"
#include "hal/hal_export_marker.hpp"

export module mystic.hal.compiler_detection;

import mystic.hal.detail;

namespace mystic::hal {

// @brief Enum representing different compilers.
export enum class Compiler : int {  // NOLINT(performance-enum-size)
  kUnknown = 0,                     ///< Represents the Microsoft Visual C++ compiler.
  kMsvc,                            ///< Represents the Microsoft Visual C++ compiler.
  kClang,                           ///< Represents the Clang compiler.
  kGcc,                             ///< Represents the GNU Compiler Collection.
  kIntel,                           ///< Represents the Intel compiler.
};

/// @brief Struct to represent the compiler version information.
/// The version is represented as Major.Minor.Patch.
/// Formats for each compiler:
/// - MSVC: MM.mm.PPPP
/// - Clang: MM.mm.PP
/// - GCC: MM.mm.PP
/// - Intel: MMMM.mm.PP
export struct MYSTIC_HAL_INTERNAL_EXPORT CompilerVersion {
  int major = 0;  ///< Major version number.
  int minor = 0;  ///< Minor version number.
  int patch = 0;  ///< Patch version number.
};

/// --- String Conversion Functions ---

/// @brief Converts a Compiler enum value to its corresponding string representation.
/// @param compiler The Compiler enum value to convert.
/// @return A string representation of the compiler.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr const char* ToString(Compiler compiler) {
  switch (compiler) {
    case Compiler::kMsvc:
      return "MSVC";
    case Compiler::kClang:
      return "Clang";
    case Compiler::kGcc:
      return "GCC";
    case Compiler::kIntel:
      return "Intel";
    default:
      return "Unknown";
  }
}

/// @brief Converts a CompilerVersion struct to its corresponding string representation.
/// @param version The CompilerVersion struct to convert.
/// @return A string representation of the compiler version in the format "Major.Minor.Patch
export MYSTIC_HAL_INTERNAL_EXPORT constexpr const char* ToString(const CompilerVersion& version) {
  thread_local char buffer[32];  // NOLINT(modernize-avoid-c-arrays)
  char* ptr = buffer;
  ptr = detail::WriteInt(ptr, version.major);
  *ptr++ = '.';
  ptr = detail::WriteInt(ptr, version.minor);
  *ptr++ = '.';
  ptr = detail::WriteInt(ptr, version.patch);
  *ptr = '\0';
  return buffer;
}

namespace compile_time {

/// @brief Returns the detected compiler at compile time.
/// @return The detected compiler as a Compiler enum value.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr Compiler GetCompiler() {
#if MYSTIC_COMPILER == MYSTIC_COMPILER_MSVC
  return Compiler::kMsvc;
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_CLANG
  return Compiler::kClang;
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_GCC
  return Compiler::kGcc;
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_INTEL
  return Compiler::kIntel;
#else
  return Compiler::kUnknown;
#endif
}

/// @brief Returns the detected compiler version at compile time.
/// @return The detected compiler version as a CompilerVersion struct.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr CompilerVersion GetCompilerVersion() {
  return CompilerVersion{
      .major = MYSTIC_COMPILER_VERSION_MAJOR,
      .minor = MYSTIC_COMPILER_VERSION_MINOR,
      .patch = MYSTIC_COMPILER_VERSION_PATCH,
  };
}

/// @brief Returns the detected full compiler version integer at compile time.
/// @return The detected full compiler version as an integer.
export MYSTIC_HAL_INTERNAL_EXPORT constexpr int GetCompilerVersionAsInt() {
  return MYSTIC_COMPILER_VERSION;
}

}  // namespace compile_time

}  // namespace mystic::hal
