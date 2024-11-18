cmake_minimum_required(VERSION 3.26)

# Project name and settings
project(oop)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include custom options
include(cmake/Options.cmake)

###############################################################################

# External dependencies using FetchContent (for SFML)
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(WARNINGS_AS_ERRORS OFF)

# Fetch SFML
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.1
        GIT_SHALLOW    1
)
FetchContent_MakeAvailable(SFML)

###############################################################################

# Find other external dependencies if necessary
find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

###############################################################################

# Define executable and include all necessary source files
add_executable(${PROJECT_NAME}
        main.cpp
        generated/src/Helper.cpp
        game/src/Camera.cpp
        game/src/Character.cpp
        game/src/Level.cpp
        game/src/Game.cpp
        game/src/LevelEditor.cpp
        env_fixes.h
)

# Include compiler flags
include(cmake/CompilerFlags.cmake)

###############################################################################

# Include directories for SFML and other dependencies
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE generated/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)

# Link directories and link necessary libraries
target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system Threads::Threads)

if(APPLE)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE X11)
endif()

###############################################################################

# Set up installation rules
install(TARGETS ${PROJECT_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

# Copy dependencies (for Windows if using SFML Audio)
if (WIN32 AND TARGET sfml-audio)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_SOURCE_DIR}/extlibs/bin/x64/openal32.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Custom file copying for additional resources
include(cmake/CopyHelper.cmake)
copy_files(FILES tastatura.txt)
# Additional resource directories can be copied like this:
# copy_files(FILES tastatura.txt config.json DIRECTORY images sounds)
