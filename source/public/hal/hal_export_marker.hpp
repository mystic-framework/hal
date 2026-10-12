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
/// @file hal_export_marker.hpp
/// @brief This file contains export markers for the HAL module.
///
/// Since HAL module does not depend on any other modules, it defines it's own export marker. The
/// general engine export marker is defined in Common module which should be used over this.
///
/// @author thedevmystic (Surya) <thedevmystic@gmail>
/// @license Apache-2.0
/// ------------------------------------------------------------------------------------------------

#ifndef MYSTIC_HAL_PUBLIC_HAL_EXPORT_MARKER_HPP_
#define MYSTIC_HAL_PUBLIC_HAL_EXPORT_MARKER_HPP_

#if defined(_WIN32) || defined(_WIN64)
#  if defined(MYSTICHAL_EXPORTS)
#    define MYSTIC_HAL_INTERNAL_EXPORT __declspec(dllexport)
#  else
#    define MYSTIC_HAL_INTERNAL_EXPORT __declspec(dllimport)
#  endif
#else
#  define MYSTIC_HAL_INTERNAL_EXPORT __attribute__((visibility("default")))
#endif

#endif  // MYSTIC_HAL_PUBLIC_HAL_EXPORT_MARKER_HPP_
