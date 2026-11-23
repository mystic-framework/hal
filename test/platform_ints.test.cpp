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
/// @file platform_ints.test.cpp
/// @brief This file contains unit tests for the platform integer macros.
/// @author thedevmystic (Surya) <thedevmystic@gmail.com>
/// @license Apache-2.0
/// ------------------------------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>

#include "hal/platform_ints.hpp"

// The platform integer macros are only defined for 64-bit platforms.
static_assert(sizeof(void*) == 8, "platform_ints.hpp supports 64-bit platforms only");

namespace {

// Widens any integer to a common type so that limit comparisons never trigger sign-compare or
// integral-promotion issues.
template <typename T>
constexpr std::int64_t AsSigned(T value) {
  return static_cast<std::int64_t>(value);
}

template <typename T>
constexpr std::uint64_t AsUnsigned(T value) {
  return static_cast<std::uint64_t>(value);
}

}  // namespace

TEST_CASE("Signed integer types have correct size and signedness.",
          "[mystic][hal][platform_ints][signed_types]") {
  SECTION("MYSTIC_INT8") {
    static_assert(sizeof(MYSTIC_INT8) == 1);
    static_assert(std::is_signed_v<MYSTIC_INT8>);
    REQUIRE(sizeof(MYSTIC_INT8) == 1);
    REQUIRE(std::is_signed_v<MYSTIC_INT8>);
  }

  SECTION("MYSTIC_INT16") {
    static_assert(sizeof(MYSTIC_INT16) == 2);
    static_assert(std::is_signed_v<MYSTIC_INT16>);
    REQUIRE(sizeof(MYSTIC_INT16) == 2);
    REQUIRE(std::is_signed_v<MYSTIC_INT16>);
  }

  SECTION("MYSTIC_INT32") {
    static_assert(sizeof(MYSTIC_INT32) == 4);
    static_assert(std::is_signed_v<MYSTIC_INT32>);
    REQUIRE(sizeof(MYSTIC_INT32) == 4);
    REQUIRE(std::is_signed_v<MYSTIC_INT32>);
  }

  SECTION("MYSTIC_INT64") {
    static_assert(sizeof(MYSTIC_INT64) == 8);
    static_assert(std::is_signed_v<MYSTIC_INT64>);
    REQUIRE(sizeof(MYSTIC_INT64) == 8);
    REQUIRE(std::is_signed_v<MYSTIC_INT64>);
  }
}

TEST_CASE("Unsigned integer types have correct size and signedness.",
          "[mystic][hal][platform_ints][unsigned_types]") {
  SECTION("MYSTIC_UINT8") {
    static_assert(sizeof(MYSTIC_UINT8) == 1);
    static_assert(std::is_unsigned_v<MYSTIC_UINT8>);
    REQUIRE(sizeof(MYSTIC_UINT8) == 1);
    REQUIRE(std::is_unsigned_v<MYSTIC_UINT8>);
  }

  SECTION("MYSTIC_UINT16") {
    static_assert(sizeof(MYSTIC_UINT16) == 2);
    static_assert(std::is_unsigned_v<MYSTIC_UINT16>);
    REQUIRE(sizeof(MYSTIC_UINT16) == 2);
    REQUIRE(std::is_unsigned_v<MYSTIC_UINT16>);
  }

  SECTION("MYSTIC_UINT32") {
    static_assert(sizeof(MYSTIC_UINT32) == 4);
    static_assert(std::is_unsigned_v<MYSTIC_UINT32>);
    REQUIRE(sizeof(MYSTIC_UINT32) == 4);
    REQUIRE(std::is_unsigned_v<MYSTIC_UINT32>);
  }

  SECTION("MYSTIC_UINT64") {
    static_assert(sizeof(MYSTIC_UINT64) == 8);
    static_assert(std::is_unsigned_v<MYSTIC_UINT64>);
    REQUIRE(sizeof(MYSTIC_UINT64) == 8);
    REQUIRE(std::is_unsigned_v<MYSTIC_UINT64>);
  }
}

TEST_CASE("Signed integer limits match std::numeric_limits.",
          "[mystic][hal][platform_ints][signed_limits]") {
  SECTION("int8") {
    static_assert(AsSigned(MYSTIC_INT8_MIN) == AsSigned(std::numeric_limits<MYSTIC_INT8>::min()));
    static_assert(AsSigned(MYSTIC_INT8_MAX) == AsSigned(std::numeric_limits<MYSTIC_INT8>::max()));
    REQUIRE(AsSigned(MYSTIC_INT8_MIN) == AsSigned(std::numeric_limits<MYSTIC_INT8>::min()));
    REQUIRE(AsSigned(MYSTIC_INT8_MAX) == AsSigned(std::numeric_limits<MYSTIC_INT8>::max()));
  }

  SECTION("int16") {
    static_assert(AsSigned(MYSTIC_INT16_MIN) == AsSigned(std::numeric_limits<MYSTIC_INT16>::min()));
    static_assert(AsSigned(MYSTIC_INT16_MAX) == AsSigned(std::numeric_limits<MYSTIC_INT16>::max()));
    REQUIRE(AsSigned(MYSTIC_INT16_MIN) == AsSigned(std::numeric_limits<MYSTIC_INT16>::min()));
    REQUIRE(AsSigned(MYSTIC_INT16_MAX) == AsSigned(std::numeric_limits<MYSTIC_INT16>::max()));
  }

  SECTION("int32") {
    static_assert(AsSigned(MYSTIC_INT32_MIN) == AsSigned(std::numeric_limits<MYSTIC_INT32>::min()));
    static_assert(AsSigned(MYSTIC_INT32_MAX) == AsSigned(std::numeric_limits<MYSTIC_INT32>::max()));
    REQUIRE(AsSigned(MYSTIC_INT32_MIN) == AsSigned(std::numeric_limits<MYSTIC_INT32>::min()));
    REQUIRE(AsSigned(MYSTIC_INT32_MAX) == AsSigned(std::numeric_limits<MYSTIC_INT32>::max()));
  }

  SECTION("int64") {
    static_assert(AsSigned(MYSTIC_INT64_MIN) == AsSigned(std::numeric_limits<MYSTIC_INT64>::min()));
    static_assert(AsSigned(MYSTIC_INT64_MAX) == AsSigned(std::numeric_limits<MYSTIC_INT64>::max()));
    REQUIRE(AsSigned(MYSTIC_INT64_MIN) == AsSigned(std::numeric_limits<MYSTIC_INT64>::min()));
    REQUIRE(AsSigned(MYSTIC_INT64_MAX) == AsSigned(std::numeric_limits<MYSTIC_INT64>::max()));
  }

  SECTION("MIN is strictly less than MAX") {
    REQUIRE(MYSTIC_INT8_MIN < MYSTIC_INT8_MAX);
    REQUIRE(MYSTIC_INT16_MIN < MYSTIC_INT16_MAX);
    REQUIRE(MYSTIC_INT32_MIN < MYSTIC_INT32_MAX);
    REQUIRE(MYSTIC_INT64_MIN < MYSTIC_INT64_MAX);
  }
}

TEST_CASE("Unsigned integer limits match std::numeric_limits.",
          "[mystic][hal][platform_ints][unsigned_limits]") {
  SECTION("uint8") {
    static_assert(AsUnsigned(MYSTIC_UINT8_MAX) ==
                  AsUnsigned(std::numeric_limits<MYSTIC_UINT8>::max()));
    REQUIRE(AsUnsigned(MYSTIC_UINT8_MAX) == AsUnsigned(std::numeric_limits<MYSTIC_UINT8>::max()));
  }

  SECTION("uint16") {
    static_assert(AsUnsigned(MYSTIC_UINT16_MAX) ==
                  AsUnsigned(std::numeric_limits<MYSTIC_UINT16>::max()));
    REQUIRE(AsUnsigned(MYSTIC_UINT16_MAX) == AsUnsigned(std::numeric_limits<MYSTIC_UINT16>::max()));
  }

  SECTION("uint32") {
    static_assert(AsUnsigned(MYSTIC_UINT32_MAX) ==
                  AsUnsigned(std::numeric_limits<MYSTIC_UINT32>::max()));
    REQUIRE(AsUnsigned(MYSTIC_UINT32_MAX) == AsUnsigned(std::numeric_limits<MYSTIC_UINT32>::max()));
  }

  SECTION("uint64") {
    static_assert(AsUnsigned(MYSTIC_UINT64_MAX) ==
                  AsUnsigned(std::numeric_limits<MYSTIC_UINT64>::max()));
    REQUIRE(AsUnsigned(MYSTIC_UINT64_MAX) == AsUnsigned(std::numeric_limits<MYSTIC_UINT64>::max()));
  }

  SECTION("MAX values wrap to zero when incremented") {
    REQUIRE(static_cast<MYSTIC_UINT8>(MYSTIC_UINT8_MAX + 1U) == 0);
    REQUIRE(static_cast<MYSTIC_UINT16>(MYSTIC_UINT16_MAX + 1U) == 0);
    REQUIRE(static_cast<MYSTIC_UINT32>(MYSTIC_UINT32_MAX + 1U) == 0);
    REQUIRE(static_cast<MYSTIC_UINT64>(MYSTIC_UINT64_MAX + 1U) == 0);
  }
}

TEST_CASE("Maximum integer types are 64-bit with correct limits.",
          "[mystic][hal][platform_ints][intmax]") {
  static_assert(sizeof(MYSTIC_INTMAX) == 8);
  static_assert(std::is_signed_v<MYSTIC_INTMAX>);
  static_assert(sizeof(MYSTIC_UINTMAX) == 8);
  static_assert(std::is_unsigned_v<MYSTIC_UINTMAX>);

  REQUIRE(sizeof(MYSTIC_INTMAX) == 8);
  REQUIRE(std::is_signed_v<MYSTIC_INTMAX>);
  REQUIRE(sizeof(MYSTIC_UINTMAX) == 8);
  REQUIRE(std::is_unsigned_v<MYSTIC_UINTMAX>);

  REQUIRE(AsSigned(MYSTIC_INTMAX_MIN) == AsSigned(std::numeric_limits<MYSTIC_INTMAX>::min()));
  REQUIRE(AsSigned(MYSTIC_INTMAX_MAX) == AsSigned(std::numeric_limits<MYSTIC_INTMAX>::max()));
  REQUIRE(AsUnsigned(MYSTIC_UINTMAX_MAX) == AsUnsigned(std::numeric_limits<MYSTIC_UINTMAX>::max()));
}

TEST_CASE("Pointer integer types can hold a pointer.", "[mystic][hal][platform_ints][intptr]") {
  static_assert(sizeof(MYSTIC_INTPTR) == sizeof(void*));
  static_assert(std::is_signed_v<MYSTIC_INTPTR>);
  static_assert(sizeof(MYSTIC_UINTPTR) == sizeof(void*));
  static_assert(std::is_unsigned_v<MYSTIC_UINTPTR>);

  REQUIRE(sizeof(MYSTIC_INTPTR) == sizeof(void*));
  REQUIRE(std::is_signed_v<MYSTIC_INTPTR>);
  REQUIRE(sizeof(MYSTIC_UINTPTR) == sizeof(void*));
  REQUIRE(std::is_unsigned_v<MYSTIC_UINTPTR>);

  SECTION("Pointer round-trips through the integer types") {
    int value = 42;
    void* original = &value;

    auto as_signed = reinterpret_cast<MYSTIC_INTPTR>(original);
    auto as_unsigned = reinterpret_cast<MYSTIC_UINTPTR>(original);

    REQUIRE(reinterpret_cast<void*>(as_signed) == original);    // NOLINT
    REQUIRE(reinterpret_cast<void*>(as_unsigned) == original);  // NOLINT
  }

  SECTION("Limits") {
    REQUIRE(AsSigned(MYSTIC_INTPTR_MIN) == AsSigned(std::numeric_limits<MYSTIC_INTPTR>::min()));
    REQUIRE(AsSigned(MYSTIC_INTPTR_MAX) == AsSigned(std::numeric_limits<MYSTIC_INTPTR>::max()));
    REQUIRE(AsUnsigned(MYSTIC_UINTPTR_MAX) ==
            AsUnsigned(std::numeric_limits<MYSTIC_UINTPTR>::max()));
  }
}

TEST_CASE("Pointer difference type is signed 64-bit with correct limits.",
          "[mystic][hal][platform_ints][ptrdiff]") {
  static_assert(sizeof(MYSTIC_PTRDIFF) == 8);
  static_assert(std::is_signed_v<MYSTIC_PTRDIFF>);
  REQUIRE(sizeof(MYSTIC_PTRDIFF) == 8);
  REQUIRE(std::is_signed_v<MYSTIC_PTRDIFF>);

  REQUIRE(sizeof(MYSTIC_PTRDIFF) == sizeof(std::ptrdiff_t));
  REQUIRE(AsSigned(MYSTIC_PTRDIFF_MIN) == AsSigned(std::numeric_limits<MYSTIC_PTRDIFF>::min()));
  REQUIRE(AsSigned(MYSTIC_PTRDIFF_MAX) == AsSigned(std::numeric_limits<MYSTIC_PTRDIFF>::max()));

  SECTION("Holds the difference between two pointers") {
    int array[8] = {};  // NOLINT
    MYSTIC_PTRDIFF diff = &array[7] - &array[2];
    REQUIRE(diff == 5);
    diff = &array[2] - &array[7];
    REQUIRE(diff == -5);
  }
}

TEST_CASE("Size type is unsigned 64-bit with correct limit.",
          "[mystic][hal][platform_ints][size]") {
  static_assert(sizeof(MYSTIC_SIZE) == 8);
  static_assert(std::is_unsigned_v<MYSTIC_SIZE>);
  REQUIRE(sizeof(MYSTIC_SIZE) == 8);
  REQUIRE(std::is_unsigned_v<MYSTIC_SIZE>);

  REQUIRE(sizeof(MYSTIC_SIZE) == sizeof(std::size_t));
  REQUIRE(AsUnsigned(MYSTIC_SIZE_MAX) == AsUnsigned(std::numeric_limits<MYSTIC_SIZE>::max()));
  REQUIRE(AsUnsigned(MYSTIC_SIZE_MAX) == AsUnsigned(std::numeric_limits<std::size_t>::max()));

  SECTION("Can hold the size of an object") {
    MYSTIC_SIZE size = sizeof(std::int64_t);
    REQUIRE(size == sizeof(std::int64_t));
  }
}

TEST_CASE("MYSTIC_MAX_ALIGN satisfies maximum alignment requirements.",
          "[mystic][hal][platform_ints][max_align]") {
  static_assert(alignof(MYSTIC_MAX_ALIGN) >= alignof(std::max_align_t));
  static_assert(alignof(MYSTIC_MAX_ALIGN) >= alignof(std::int64_t));
  static_assert(alignof(MYSTIC_MAX_ALIGN) >= alignof(long double));
  static_assert(alignof(MYSTIC_MAX_ALIGN) >= alignof(void*));

  REQUIRE(alignof(MYSTIC_MAX_ALIGN) >= alignof(std::max_align_t));
  REQUIRE(alignof(MYSTIC_MAX_ALIGN) >= alignof(std::int64_t));
  REQUIRE(alignof(MYSTIC_MAX_ALIGN) >= alignof(long double));
  REQUIRE(alignof(MYSTIC_MAX_ALIGN) >= alignof(void*));

  // Should behave like std::max_align_t: a POD-like type.
  REQUIRE(std::is_trivially_copyable_v<MYSTIC_MAX_ALIGN>);
  REQUIRE(std::is_standard_layout_v<MYSTIC_MAX_ALIGN>);
}

TEST_CASE("MYSTIC_NULLTYPE is the nullptr type.", "[mystic][hal][platform_ints][nulltype]") {
  static_assert(std::is_same_v<MYSTIC_NULLTYPE, std::nullptr_t>);
  REQUIRE(std::is_same_v<MYSTIC_NULLTYPE, std::nullptr_t>);

  MYSTIC_NULLTYPE null_value = nullptr;
  int* ptr = null_value;
  REQUIRE(ptr == nullptr);
}
