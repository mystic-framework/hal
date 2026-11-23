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
/// @file compiler_detection.test.cpp
/// @brief This file contains unit tests for the compiler detection module.
/// @author thedevmystic (Surya) <thedevmystic@gmail.com>
/// @license Apache-2.0
/// ------------------------------------------------------------------------------------------------

import mystic.hal.compiler_detection;

#include <string>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "hal/compiler_detection_macros.hpp"

TEST_CASE("ToString() functions return expected strings.",
          "[mystic][hal][compiler_detection][to_string]") {
  using mystic::hal::ToString;

  SECTION("Compiler enum") {
    auto compiler = GENERATE(values({mystic::hal::Compiler::kUnknown,
                                     mystic::hal::Compiler::kMsvc,
                                     mystic::hal::Compiler::kClang,
                                     mystic::hal::Compiler::kGcc,
                                     mystic::hal::Compiler::kIntel}));

    const char* compiler_str = ToString(compiler);
    REQUIRE(compiler_str != nullptr);
    REQUIRE_FALSE(std::string(compiler_str).empty());
  }

  SECTION("CompilerVersion struct") {
    mystic::hal::CompilerVersion version{.major = 1, .minor = 2, .patch = 3};
    const char* version_str = ToString(version);

    REQUIRE(version_str != nullptr);
    REQUIRE(std::string(version_str) == "1.2.3");
  }
}

TEST_CASE("Correct Compiler enum is returned at compile time.",
          "[mystic][hal][compiler_detection][compile_time_compiler]") {
  constexpr auto compiler = mystic::hal::compile_time::GetCompiler();
  static_assert(mystic::hal::compile_time::GetCompiler() == compiler);

#if MYSTIC_COMPILER == MYSTIC_COMPILER_MSVC
  REQUIRE(compiler == mystic::hal::Compiler::kMsvc);
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_CLANG
  REQUIRE(compiler == mystic::hal::Compiler::kClang);
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_GCC
  REQUIRE(compiler == mystic::hal::Compiler::kGcc);
#elif MYSTIC_COMPILER == MYSTIC_COMPILER_INTEL
  REQUIRE(compiler == mystic::hal::Compiler::kIntel);
#else
  REQUIRE(compiler == mystic::hal::Compiler::kUnknown);
#endif
}

TEST_CASE("CompilerVersion struct initializes correctly.",
          "[mystic][hal][compiler_detection][compiler_version_init]") {
  // Default constructor should initialize to zeroed values.
  mystic::hal::CompilerVersion version_default;
  REQUIRE(version_default.major == 0);
  REQUIRE(version_default.minor == 0);
  REQUIRE(version_default.patch == 0);

  // Designated initializers should populate fields as provided.
  mystic::hal::CompilerVersion version{.major = 18, .minor = 1, .patch = 6};
  REQUIRE(version.major == 18);
  REQUIRE(version.minor == 1);
  REQUIRE(version.patch == 6);
}

TEST_CASE("GetCompilerVersion() returns valid version components.",
          "[mystic][hal][compiler_detection][get_compiler_version]") {
  constexpr auto version = mystic::hal::compile_time::GetCompilerVersion();
  static_assert(mystic::hal::compile_time::GetCompilerVersion().major == version.major);

  REQUIRE(version.major >= 0);
  REQUIRE(version.minor >= 0);
  REQUIRE(version.patch >= 0);
  REQUIRE(version.major == MYSTIC_COMPILER_VERSION_MAJOR);
  REQUIRE(version.minor == MYSTIC_COMPILER_VERSION_MINOR);
  REQUIRE(version.patch == MYSTIC_COMPILER_VERSION_PATCH);
}

TEST_CASE("GetCompilerVersionAsInt() returns valid version integer.",
          "[mystic][hal][compiler_detection][get_compiler_version_int]") {
  constexpr int version_int = mystic::hal::compile_time::GetCompilerVersionAsInt();
  static_assert(mystic::hal::compile_time::GetCompilerVersionAsInt() == version_int);

#if MYSTIC_COMPILER != MYSTIC_COMPILER_UNKNOWN
  REQUIRE(version_int > 0);
#endif
  REQUIRE(version_int == MYSTIC_COMPILER_VERSION);
}
