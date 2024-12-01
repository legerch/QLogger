cmake_minimum_required(VERSION 3.19)

# Set project properties
set(PROJECT_NAME qlogger)
set(PROJECT_VERSION_SEMANTIC 1.0.2)
set(PROJECT_VERSION_CPP_MIN 11)

# Set project configuration
project(${PROJECT_NAME} LANGUAGES CXX)
project(${PROJECT_NAME} VERSION "${PROJECT_VERSION_SEMANTIC}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Qt specific properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Define project options
## Example: option(LIBRARYNAME_BUILD_TESTS "Use to enable/disable build of unit-tests." ON)

# Manage global compiler options
## For MSVC: force to read source code as UTF-8 file (already default behaviour on GCC/Clang)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Set C++ standard to use
if(DEFINED CMAKE_CXX_STANDARD)
    if(${CMAKE_CXX_STANDARD} LESS ${PROJECT_VERSION_CPP_MIN})
        message(FATAL_ERROR "Project ${PROJECT_NAME} require at least C++ standard ${PROJECT_VERSION_CPP_MIN}")
    endif()
else()
    set(CMAKE_CXX_STANDARD ${PROJECT_VERSION_CPP_MIN})
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Project \"${PROJECT_NAME}\" compiled with C++ standard ${CMAKE_CXX_STANDARD}")

# Set needed packages
## Qt packages
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

## Other library packages
### Example: find_package(nlohmann_json 3.11.3 REQUIRED)

# Defines useful path variables for easier CMake configuration
set(PROJECT_FILE_GLOBAL "qloggerglobal.h")

# Manage library files
## Configure file project - File containing macros that can be used in project
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_FILE_GLOBAL}.in" "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_FILE_GLOBAL}")

## Set library files
set(PROJECT_HEADERS
    ${PROJECT_FILE_GLOBAL}

    base/baselogger.h

    log/logbinary.h
    log/logentry.h
    log/logformatter.h

    sinks/filedailylogger.h
    sinks/filelogger.h
    sinks/filerotatinglogger.h

    qloggerfactory.h
)

set(PROJECT_SOURCES
    base/baselogger.cpp

    log/logbinary.cpp
    log/logentry.cpp
    log/logformatter.cpp

    sinks/filedailylogger.cpp
    sinks/filelogger.cpp
    sinks/filerotatinglogger.cpp

    qloggerfactory.cpp
)

set(PROJECT_UI

)

set(PROJECT_RSC

)

set(PROJECT_FILES ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${PROJECT_UI} ${PROJECT_RSC})

# Add files to the library
add_library(${PROJECT_NAME} SHARED ${PROJECT_FILES})

# Set version of library
set_target_properties(${PROJECT_NAME} PROPERTIES 
                        VERSION ${PROJECT_VERSION}
                        SOVERSION ${PROJECT_VERSION_MAJOR})

# Link needed libraries
## Qt linking
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)

## Other library linking
### Example: target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Specify compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror=return-type -Wshadow -Wlogical-op -Wduplicated-cond -Wduplicated-branches>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Werror=return-type -Wshadow -Wlogical-op -Wduplicated-cond -Wduplicated-branches>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4251>
)

# Compile needed definitions
## Library related
target_compile_definitions(${PROJECT_NAME} PRIVATE QLOGGER_LIBRARY_BUILD)

# Specify include directories
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PROJECT_ROOT_DIR)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_ROOT_DIR})
