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
/// @file compiler_detection_macros.hpp
/// @brief This file contains macros for detecting the compiler being used to build the project.
/// @author thedevmystic (Surya) <thedevmystic@gmail.com>
/// @license Apache-2.0
///
/// This file provides a set of preprocessor macros that can be used to detect the compiler being
/// used to compile the project. It defines macros for various compilers, including MSVC, Clang,
/// GCC, and others. These macros can be used to conditionally compile code based on the detected
/// compiler.
///
/// @sa compiler_detection.cppm for compile-time functions that expose these macros as constexpr
/// functions.
/// ------------------------------------------------------------------------------------------------

#ifndef MYSTIC_HAL_PUBLIC_HAL_COMPILER_DETECTION_MACROS_HPP_
#define MYSTIC_HAL_PUBLIC_HAL_COMPILER_DETECTION_MACROS_HPP_

/// Compiler tags
/// These are pre-defined constant tags used as a identifiers for the compilers.
#define MYSTIC_COMPILER_MSVC 1
#define MYSTIC_COMPILER_CLANG 2
#define MYSTIC_COMPILER_GCC 3
#define MYSTIC_COMPILER_INTEL 4
#define MYSTIC_COMPILER_UNKNOWN 999

/// Compiler detection
/// This section sets the following macros:
/// MYSTIC_COMPILER: The detected compiler (one of the above tags).
/// MYSTIC_COMPILER_NAME: The name of the detected compiler as a string literal.
/// MYSTIC_COMPILER_VERSION: The version of the detected compiler as a string literal.
/// MYSTIC_COMPILER_VERSION_MAJOR: The major version of the detected compiler.
/// MYSTIC_COMPILER_VERSION_MINOR: The minor version of the detected compiler.
/// MYSTIC_COMPILER_VERSION_PATCH: The patch version of the detected compiler.
#if defined(_MSC_VER)
#  define MYSTIC_COMPILER MYSTIC_COMPILER_MSVC
#  define MYSTIC_COMPILER_NAME "MSVC"
#  define MYSTIC_COMPILER_VERSION _MSC_FULL_VER
#  define MYSTIC_COMPILER_VERSION_MAJOR (_MSC_FULL_VER / 10000000)
#  define MYSTIC_COMPILER_VERSION_MINOR ((_MSC_FULL_VER / 100000) % 100)
#  define MYSTIC_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
#elif defined(__clang__)
#  define MYSTIC_COMPILER MYSTIC_COMPILER_CLANG
#  define MYSTIC_COMPILER_NAME "Clang"
#  define MYSTIC_COMPILER_VERSION \
    (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#  define MYSTIC_COMPILER_VERSION_MAJOR __clang_major__
#  define MYSTIC_COMPILER_VERSION_MINOR __clang_minor__
#  define MYSTIC_COMPILER_VERSION_PATCH __clang_patchlevel__
#elif defined(__GNUC__) || defined(__GNUG__)
#  define MYSTIC_COMPILER MYSTIC_COMPILER_GCC
#  define MYSTIC_COMPILER_NAME "GCC"
#  define MYSTIC_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#  define MYSTIC_COMPILER_VERSION_MAJOR __GNUC__
#  define MYSTIC_COMPILER_VERSION_MINOR __GNUC_MINOR__
#  define MYSTIC_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
#elif defined(__INTEL_COMPILER)
#  define MYSTIC_COMPILER MYSTIC_COMPILER_INTEL
#  define MYSTIC_COMPILER_NAME "Intel"
#  define MYSTIC_COMPILER_VERSION __INTEL_LLVM_COMPILER
#  define MYSTIC_COMPILER_VERSION_MAJOR (__INTEL_LLVM_COMPILER / 10000)
#  define MYSTIC_COMPILER_VERSION_MINOR ((__INTEL_LLVM_COMPILER / 100) % 100)
#  define MYSTIC_COMPILER_VERSION_PATCH (__INTEL_LLVM_COMPILER % 100)
#else
#  define MYSTIC_COMPILER MYSTIC_COMPILER_UNKNOWN
#  define MYSTIC_COMPILER_NAME "Unknown"
#  define MYSTIC_COMPILER_VERSION 0
#  define MYSTIC_COMPILER_VERSION_MAJOR 0
#  define MYSTIC_COMPILER_VERSION_MINOR 0
#  define MYSTIC_COMPILER_VERSION_PATCH 0
#endif

/// Guardrail
/// This section checks if the compiler is unknown and raises a compile-time error if it is, unless
/// MYSTIC_ALLOW_UNKNOWN_COMPILERS is defined.
#if MYSTIC_COMPILER == MYSTIC_COMPILER_UNKNOWN && !defined(MYSTIC_ALLOW_UNKNOWN_COMPILERS)
#  error \
      "[MYSTIC] - HAL - Unknown compiler detected. Either use a supported compiler or define MYSTIC_ALLOW_UNKNOWN_COMPILERS to bypass this check."
#endif

#endif  // MYSTIC_HAL_PUBLIC_HAL_COMPILER_DETECTION_MACROS_HPP_
