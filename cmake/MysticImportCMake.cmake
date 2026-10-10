# --------------------------------------------------------------------------------------------------
# Copyright 2026-present The Mystic Framework Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# --------------------------------------------------------------------------------------------------
# File: MysticImportCMake.cmake
# Description: This CMake module integrates the Mystic CMake framework into your project. It
# fetches the Mystic CMake repository, sets up the necessary paths, and includes the required
# modules for your project.
# Author: thedevmystic (Surya) <thedevmystic@gmail.com>
# License: Apache License 2.0
# --------------------------------------------------------------------------------------------------
# Usage:
# # Include this module in your CMakeLists.txt file:
# include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/MysticImportCMake.cmake")
# # Call this function to import Mystic CMake into your project:
# mystic_import_cmake()
# --------------------------------------------------------------------------------------------------

include(FetchContent)

# --------------------------------------------------------------------------------------------------
# Function: mystic_import_cmake
# Description: This function fetches the Mystic CMake repository, sets up the necessary paths,
# and includes the required modules for your project.
# --------------------------------------------------------------------------------------------------
function(mystic_import_cmake)
  set(MYSTICCMAKE_INTERNAL TRUE CACHE BOOL "" FORCE)
  set(MYSTIC_ASCII_BANNER_DISABLE TRUE)
  set(MYSTIC_MESSAGE_ONLY_ERRORS TRUE)

  # Set the repository URL and tag for Mystic CMake
  set(REPO_URL "https://github.com/mystic-framework/cmake.git")
  set(REPO_TAG "main") # Current latest release tag (or "main")
  set(SRC_DIR "${CMAKE_BINARY_DIR}/_deps/mystic_cmake-src")

  find_package(Git REQUIRED QUIET)

  # Check if the source directory exists and is a Git repository
  if(NOT EXISTS "${SRC_DIR}/.git")
    # If does not exist, clone the repository
    execute_process(
      COMMAND ${GIT_EXECUTABLE} clone --quiet ${REPO_URL} --branch ${REPO_TAG} "${SRC_DIR}"
      RESULT_VARIABLE RC
      OUTPUT_VARIABLE ERR
      ERROR_VARIABLE ERR
    )
  else()
    # If exists, fetch the latest changes and reset to the specified tag
    execute_process(
      COMMAND ${GIT_EXECUTABLE} -C "${SRC_DIR}" fetch --quiet origin ${REPO_TAG}
      RESULT_VARIABLE RC
      OUTPUT_VARIABLE ERR
      ERROR_VARIABLE ERR
    )
    if(RC EQUAL 0)
      execute_process(
        COMMAND ${GIT_EXECUTABLE} -C "${SRC_DIR}" reset --hard --quiet FETCH_HEAD
        RESULT_VARIABLE RC
        OUTPUT_VARIABLE ERR
        ERROR_VARIABLE ERR
      )
    endif()
  endif()

  # Check for errors during the fetch or reset process
  if(NOT RC EQUAL 0)
    message(FATAL_ERROR "[MYSTIC] - Failed to fetch the CMake module:\n${ERR}")
  endif()

  # Set the source directory for FetchContent
  set(FETCHCONTENT_SOURCE_DIR_MYSTIC_CMAKE "${SRC_DIR}")

  # Call FetchContent to make the Mystic CMake module available
  include(FetchContent)
  FetchContent_Declare(
    mystic_cmake
    SOURCE_DIR "${SRC_DIR}"
  )
  FetchContent_MakeAvailable(mystic_cmake)

  # Append the Mystic CMake module path to CMAKE_MODULE_PATH
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${mystic_cmake_SOURCE_DIR}/modules" PARENT_SCOPE)
endfunction()
