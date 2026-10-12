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
/// @file os_detection_macros.hpp
/// @brief This file contains macros for detecting the OS being used to build the project.
/// @author thedevmystic (Surya) <thedevmystic@gmail>
/// @license Apache-2.0
///
/// This file provides a set of preprocessor macros that can be used to detect the os
/// being used to compile the project. It defines macros for various OSes, including Windows,
/// MacOS, Linux, and others. These macros can be used to conditionally compile code based on the
/// detected OS. Use runtime counterpart when you need to detect the OS at runtime, like Windows 10
/// vs Windows 11, etc.
///
/// @sa os_detection.cppm for runtime architecture detection.
/// ------------------------------------------------------------------------------------------------

#ifndef MYSTIC_HAL_PUBLIC_HAL_OS_DETECTION_MACROS_HPP_
#define MYSTIC_HAL_PUBLIC_HAL_OS_DETECTION_MACROS_HPP_

/// OS tags
/// These are pre-defined constant tags used as identifiers for the OSes.
#define MYSTIC_OS_WINDOWS 1
#define MYSTIC_OS_MACOS 2
#define MYSTIC_OS_LINUX 3
#define MYSTIC_OS_UNKNOWN 999

/// OS detection
/// This section sets the following macros:
/// MYSTIC_OS: The detected OS (one of the above tags).
/// MYSTIC_OS_NAME: The name of the detected OS as a string literal.
#if defined(_WIN32) || defined(_WIN64)
#  define MYSTIC_OS MYSTIC_OS_WINDOWS
#  define MYSTIC_OS_NAME "Windows"
#elif defined(__APPLE__) || defined(__MACH__)
// Include TargetConditionals.h to check for iOS vs macOS
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#    define MYSTIC_OS MYSTIC_OS_MACOS
#    define MYSTIC_OS_NAME "macOS"
#  else
#    define MYSTIC_OS MYSTIC_OS_UNKNOWN
#    define MYSTIC_OS_NAME "Unknown"
#  endif
#elif defined(__linux__)
#  define MYSTIC_OS MYSTIC_OS_LINUX
#  define MYSTIC_OS_NAME "Linux"
#else
#  define MYSTIC_OS MYSTIC_OS_UNKNOWN
#  define MYSTIC_OS_NAME "Unknown"
#endif

/// Guardrail 1
/// This section checks if the OS is unknown and raises a compile-time error if it is,
/// unless the user has explicitly defined MYSTIC_ALLOW_UNKNOWN_OS to allow unknown OSes.
#if MYSTIC_OS == MYSTIC_OS_UNKNOWN && !defined(MYSTIC_ALLOW_UNKNOWN_OS)
#  error \
      "Unknown OS detected. Either use a supported OS or define MYSTIC_ALLOW_UNKNOWN_OS to allow unknown OSes."
#endif

/// Guardrail 2
/// This section checks if the OS is macOS and raises a compile-time error if it is,
/// unless the user has explicitly defined MYSTIC_ALLOW_MACOS to allow macOS.
/// Mystic Framework does not plan to support macOS in the near future.
#if MYSTIC_OS == MYSTIC_OS_MACOS && !defined(MYSTIC_ALLOW_MACOS)
#  error \
      "macOS detected. Mystic Framework does not support macOS. Either use a supported OS or define MYSTIC_ALLOW_MACOS to allow macOS."
#endif

#endif  // MYSTIC_HAL_PUBLIC_HAL_OS_DETECTION_MACROS_HPP_
