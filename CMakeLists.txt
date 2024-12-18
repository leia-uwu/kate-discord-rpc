cmake_minimum_required(VERSION 3.16)

# uhm so most if this was copied from plasma-sdk
# because i wanted to see a project that had an out of tree kate plugin
# i just stripped what i think was not needed, there may be more not needed stuff idk

project(kate-discord-rpc)
set(PROJECT_VERSION "0.0.1")

set(QT_MIN_VERSION "6.0.1")
set(KF6_MIN_VERSION "6.0.1")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMDeprecationSettings)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS Config CoreAddons TextEditor WidgetsAddons)

# discord rpc
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)
# copied from supertux
set(BUILD_EXAMPLES OFF CACHE BOOL "Skip Discord's sample programs" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/discord-rpc EXCLUDE_FROM_ALL)
add_library(LibDiscord INTERFACE IMPORTED)
set_target_properties(LibDiscord PROPERTIES
    INTERFACE_LINK_LIBRARIES "discord-rpc"
    # discord-rpc doesn't provide includes itself
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/discord-rpc/include"
)
set(BUILD_SHARED_LIBS ON)

set(DISCORD_ID "1317961408369004637")

add_subdirectory(src)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
