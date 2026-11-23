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
/// @file platform_ints.hpp
/// @brief This file contains macros defining the platform-specific fixed-size integer types.
/// @author thedevmystic (Surya) <thedevmystic@gmail.com>
/// @license Apache-2.0
///
/// This file defines platform-specific fixed-size integer types for Windows and Linux 64-bit
/// platforms.
///
/// It defines:
/// - MYSTIC_INT8: 8-bit signed integer.
/// - MYSTIC_INT16: 16-bit signed integer.
/// - MYSTIC_INT32: 32-bit signed integer.
/// - MYSTIC_INT64: 64-bit signed integer.
///
/// - MYSTIC_UINT8: 8-bit unsigned integer.
/// - MYSTIC_UINT16: 16-bit unsigned integer.
/// - MYSTIC_UINT32: 32-bit unsigned integer.
/// - MYSTIC_UINT64: 64-bit unsigned integer.
///
/// - MYSTIC_INTMAX: Maximum signed integer type for the platform.
/// - MYSTIC_UINTMAX: Maximum unsigned integer type for the platform.
///
/// - MYSTIC_INTPTR: Signed integer type capable of holding a pointer.
/// - MYSTIC_UINTPTR: Unsigned integer type capable of holding a pointer.
/// - MYSTIC_PTRDIFF: Signed integer type for pointer differences.
///
/// - MYSTIC_SIZE: Unsigned integer type for sizes.
///
/// - MYSTIC_MAX_ALIGN: Maximum alignment type.
///
/// - MYSTIC_NULLTYPE: Nullptr type.
///
/// This file also defines several macros for integer limits.
/// - MYSTIC_INT8_MIN: Minimum value for 8-bit signed integer.
/// - MYSTIC_INT8_MAX: Maximum value for 8-bit signed integer.
///
/// - MYSTIC_INT16_MIN: Minimum value for 16-bit signed integer.
/// - MYSTIC_INT16_MAX: Maximum value for 16-bit signed integer.
///
/// - MYSTIC_INT32_MIN: Minimum value for 32-bit signed integer.
/// - MYSTIC_INT32_MAX: Maximum value for 32-bit signed integer.
///
/// - MYSTIC_INT64_MIN: Minimum value for 64-bit signed integer.
/// - MYSTIC_INT64_MAX: Maximum value for 64-bit signed integer.
///
/// - MYSTIC_UINT8_MAX: Maximum value for 8-bit unsigned integer.
/// - MYSTIC_UINT16_MAX: Maximum value for 16-bit unsigned integer.
/// - MYSTIC_UINT32_MAX: Maximum value for 32-bit unsigned integer.
/// - MYSTIC_UINT64_MAX: Maximum value for 64-bit unsigned integer.
/// @note: Minimum values for unsigned integers are always 0, so they are not defined.
///
/// - MYSTIC_INTMAX_MIN: Minimum value for maximum signed integer type.
/// - MYSTIC_INTMAX_MAX: Maximum value for maximum signed integer type.
///
/// - MYSTIC_UINTMAX_MAX: Maximum value for maximum unsigned integer type.
///
/// - MYSTIC_INTPTR_MIN: Minimum value for signed integer type capable of holding a pointer.
/// - MYSTIC_INTPTR_MAX: Maximum value for signed integer type capable of holding a pointer.
///
/// - MYSTIC_UINTPTR_MAX: Maximum value for unsigned integer type capable of holding a pointer.
///
/// - MYSTIC_PTRDIFF_MIN: Minimum value for signed integer type for pointer differences.
/// - MYSTIC_PTRDIFF_MAX: Maximum value for signed integer type for pointer differences.
///
/// - MYSTIC_SIZE_MAX: Limit for size type.
///
/// @note: This file is an internal file for HAL module and is intended to be included in other
/// header files and should not be included directly in source files. To use actual types, use
/// types.cppm from Common module instead.
/// ------------------------------------------------------------------------------------------------

#ifndef MYSTIC_HAL_PUBLIC_HAL_PLATFORM_INTS_HPP_
#define MYSTIC_HAL_PUBLIC_HAL_PLATFORM_INTS_HPP_

#include "hal/compiler_detection_macros.hpp"
#include "hal/hal_export_marker.hpp"

/// Signed Integer Types
#if MYSTIC_COMPILER == MYSTIC_COMPILER_GCC || MYSTIC_COMPILER == MYSTIC_COMPILER_CLANG
#  define MYSTIC_INT8 __INT8_TYPE__
#  define MYSTIC_INT16 __INT16_TYPE__
#  define MYSTIC_INT32 __INT32_TYPE__
#  define MYSTIC_INT64 __INT64_TYPE__
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_MSVC
#  define MYSTIC_INT8 __int8
#  define MYSTIC_INT16 __int16
#  define MYSTIC_INT32 __int32
#  define MYSTIC_INT64 __int64
#else
#  define MYSTIC_INT8 signed char
#  define MYSTIC_INT16 signed short
#  define MYSTIC_INT32 signed int
#  define MYSTIC_INT64 signed long long
#endif

/// Check for signed integer types
#ifndef MYSTIC_SKIP_PLATFORM_INT_CHECKS
static_assert(sizeof(MYSTIC_INT8) == 1, "MYSTIC_INT8 must be 1 byte");
static_assert(sizeof(MYSTIC_INT16) == 2, "MYSTIC_INT16 must be 2 bytes");
static_assert(sizeof(MYSTIC_INT32) == 4, "MYSTIC_INT32 must be 4 bytes");
static_assert(sizeof(MYSTIC_INT64) == 8, "MYSTIC_INT64 must be 8 bytes");
#endif

/// Unsigned Integer Types
#if MYSTIC_COMPILER == MYSTIC_COMPILER_GCC || MYSTIC_COMPILER == MYSTIC_COMPILER_CLANG
#  define MYSTIC_UINT8 __UINT8_TYPE__
#  define MYSTIC_UINT16 __UINT16_TYPE__
#  define MYSTIC_UINT32 __UINT32_TYPE__
#  define MYSTIC_UINT64 __UINT64_TYPE__
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_MSVC
#  define MYSTIC_UINT8 unsigned __int8
#  define MYSTIC_UINT16 unsigned __int16
#  define MYSTIC_UINT32 unsigned __int32
#  define MYSTIC_UINT64 unsigned __int64
#else
#  define MYSTIC_UINT8 unsigned char
#  define MYSTIC_UINT16 unsigned short
#  define MYSTIC_UINT32 unsigned int
#  define MYSTIC_UINT64 unsigned long long
#endif

/// Check for unsigned integer types
#ifndef MYSTIC_SKIP_PLATFORM_INT_CHECKS
static_assert(sizeof(MYSTIC_UINT8) == 1, "MYSTIC_UINT8 must be 1 byte");
static_assert(sizeof(MYSTIC_UINT16) == 2, "MYSTIC_UINT16 must be 2 bytes");
static_assert(sizeof(MYSTIC_UINT32) == 4, "MYSTIC_UINT32 must be 4 bytes");
static_assert(sizeof(MYSTIC_UINT64) == 8, "MYSTIC_UINT64 must be 8 bytes");
#endif

/// Integer Max Types
// Max integer types are their 64-bit variants.
#define MYSTIC_INTMAX MYSTIC_INT64
#define MYSTIC_UINTMAX MYSTIC_UINT64

/// Pointer Types
// 64-bit integer types can hold a pointer address in a 64-bit system.
#define MYSTIC_INTPTR MYSTIC_INT64
#define MYSTIC_UINTPTR MYSTIC_UINT64
// Pointer differences can also be held by a 64-bit signed pointer type.
#define MYSTIC_PTRDIFF MYSTIC_INT64

/// Size Type
// It is 64-bit unsigned integer type on 64-bit platforms.
#define MYSTIC_SIZE MYSTIC_UINT64

/// Max Alignment Type similar to std::max_align_t, which is a POD type with the maximum alignment
/// requirement for any scalar type.
namespace mystic::detail {
#if MYSTIC_COMPILER == MYSTIC_COMPILER_MSVC
using MaxAlign = double;
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_GCC || MYSTIC_COMPILER == MYSTIC_COMPILER_CLANG
// Match GCC and Clang's definition
struct MYSTIC_HAL_INTERNAL_EXPORT MaxAlign {
  long long max_align_dummy1 __attribute__((__aligned__(__alignof__(long long))));  // NOLINT
  long double max_align_dummy2 __attribute__((__aligned__(__alignof__(long double))));
};
#else
union MYSTIC_HAL_INTERNAL_EXPORT MaxAlign {
  long long max_align_dummy1;
  long double max_align_dummy2;
  void* max_align_dummy3;
};
#endif
}  // namespace mystic::detail
#define MYSTIC_MAX_ALIGN mystic::detail::MaxAlign

/// Nullptr Type
#define MYSTIC_NULLTYPE decltype(nullptr)

/// Limits of Signed Integer Types
// int8
#define MYSTIC_INT8_MIN (-127 - 1)
#define MYSTIC_INT8_MAX 127
// int16
#define MYSTIC_INT16_MIN (-32767 - 1)
#define MYSTIC_INT16_MAX 32767
// int32
#define MYSTIC_INT32_MIN (-2147483647 - 1)
#define MYSTIC_INT32_MAX 2147483647
// int64
#define MYSTIC_INT64_MIN (MYSTIC_INT64)(-9223372036854775807LL - 1)
#define MYSTIC_INT64_MAX (MYSTIC_INT64)9223372036854775807LL

/// Limits for Unsigned Integer Types
// uint8
#define MYSTIC_UINT8_MAX 0xffU
// uint16
#define MYSTIC_UINT16_MAX 0xffffU
// uint32
#define MYSTIC_UINT32_MAX 0xffffffffU
// uint64
#define MYSTIC_UINT64_MAX (MYSTIC_UINT64)0xffffffffffffffffULL

/// Limits for Maximum Integer Types
// intmax
#define MYSTIC_INTMAX_MIN MYSTIC_INT64_MIN
#define MYSTIC_INTMAX_MAX MYSTIC_INT64_MAX
// uintmax
#define MYSTIC_UINTMAX_MAX MYSTIC_UINT64_MAX

/// Limits for Pointer Types
// intptr
#define MYSTIC_INTPTR_MIN MYSTIC_INT64_MIN
#define MYSTIC_INTPTR_MAX MYSTIC_INT64_MAX
// uintptr
#define MYSTIC_UINTPTR_MAX MYSTIC_UINT64_MAX

/// Limits for Pointer Difference Type
#define MYSTIC_PTRDIFF_MIN MYSTIC_INT64_MIN
#define MYSTIC_PTRDIFF_MAX MYSTIC_INT64_MAX

/// Limits for Size Type
#define MYSTIC_SIZE_MAX MYSTIC_UINT64_MAX

#endif  // MYSTIC_HAL_PUBLIC_HAL_PLATFORM_INTS_HPP_
