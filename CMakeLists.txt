cmake_minimum_required(VERSION 3.30)

# Import CMake modules
include(cmake/MysticImportCMake.cmake)
mystic_import_cmake()

# Banner
include(MysticAsciiBanner)
mystic_ascii_banner("Mystic FWK: Hal")

# Project Configuration
include(MysticProject)
mystic_project()
project(${MYSTICHAL_PROJECT_ARGUMENTS})

# Setup Options
include(MysticSetupOptions)
mystic_setup_options()
mystic_setup_third_party_options("Catch2")

# Since HAL is quite platform dependent, we will provide an option to specify that it's
# being built locally, this disables some tests which are only runnable in CI/CD.
option(MYSTICHAL_LOCAL_BUILD "HAL is being built locally." OFF)

# Include Third Party
include(MysticIncludeThirdParty)
mystic_include_third_party()

# Target
add_library(mystic_hal ${MYSTICHAL_LIB_TYPE})
add_library(Mystic::Hal ALIAS mystic_hal)

# File Groups
file(GLOB_RECURSE MYSTICHAL_PUBLIC_MODULES  CONFIGURE_DEPENDS "source/public/*.cppm")
file(GLOB_RECURSE MYSTICHAL_PUBLIC_HEADERS  CONFIGURE_DEPENDS "source/public/*.hpp")
file(GLOB_RECURSE MYSTICHAL_PRIVATE_MODULES CONFIGURE_DEPENDS "source/private/*.cppm")
if(WIN32)
  file(GLOB_RECURSE MYSTICHAL_PRIVATE_SOURCES CONFIGURE_DEPENDS "source/private/windows/*.cpp")
else()
  file(GLOB_RECURSE MYSTICHAL_PRIVATE_SOURCES CONFIGURE_DEPENDS "source/private/linux/*.cpp")
endif()
file(GLOB_RECURSE MYSTICHAL_TEST_FILES "test/*.test.cpp")

# Compilation Units
target_sources(mystic_hal
  PUBLIC
    FILE_SET CXX_MODULES FILES
      ${MYSTICHAL_PUBLIC_MODULES}
      ${MYSTICHAL_PRIVATE_MODULES}
    FILE_SET HEADERS FILES
      ${MYSTICHAL_PUBLIC_HEADERS}
  PRIVATE
    ${MYSTICHAL_PRIVATE_SOURCES}
)

# Include Directories
include(GNUInstallDirs)
target_include_directories(mystic_hal
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/public>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/mystic>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/private
)

# Compile Options
include(MysticCompileOptions)
mystic_compile_options(mystic_hal)

# Add MYSTIC_HAL_LOCAL_BUILD definition to the target
if(MYSTICHAL_LOCAL_BUILD)
  target_compile_definitions(mystic_hal PUBLIC MYSTIC_HAL_LOCAL_BUILD)
endif()

# Export Controls
include(MysticExports)
mystic_exports(mystic_hal
  MACRO_NAME "MYSTICHAL_EXPORTS"
)

# Testing
include(MysticTest)
mystic_enable_testing()
mystic_test(mystic_hal_test
  SOURCES ${MYSTICHAL_TEST_FILES}
  DEPENDENCIES Mystic::Hal
)

# Coverage
include(MysticCoverage)
mystic_coverage(mystic_hal mystic_hal_test)

# Linting
include(MysticLint)
mystic_lint(mystic_hal)

# Sanitizers
include(MysticSanitizers)
mystic_sanitizers(mystic_hal)

# Installation
include(MysticInstall)
mystic_install(mystic_hal)
