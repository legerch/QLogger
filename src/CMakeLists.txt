cmake_minimum_required(VERSION 3.5)

# Set project properties
set(PROJECT_NAME qlogger)
set(PROJECT_VERSION_SEMANTIC 0.0.1)
set(PROJECT_VERSION_CPP_MIN 17)

# Set project options
# Options availables for external project start with "EXT_OPT_QLOGGER_XYZ", those options can be use in top CMakeFiles.
# All options are disabled by default.
# List of available options :
# - Currently no options available

# Set project configuration
project(${PROJECT_NAME} LANGUAGES CXX)
project(${PROJECT_NAME} VERSION "${PROJECT_VERSION_SEMANTIC}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)   # Specific to Qt
set(CMAKE_AUTOMOC ON)   # Specific to Qt
set(CMAKE_AUTORCC ON)   # Specific to Qt

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

# Configure file project - File containing macro that can be used in project
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/config.h")

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

# Manage library files
set(PROJECT_HEADERS
    config.h

    qloggerglobal.h

    qlogger/qloghandler.h
)

set(PROJECT_SOURCES
    qlogger/qloghandler.cpp
)

set(PROJECT_UI

)

set(PROJECT_RSC

)

set(PROJECT_FILES ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${PROJECT_UI} ${PROJECT_RSC})

# Platform dependant stuff
# Windows (for both x86/x64)
if(WIN32)

endif()

# MacOS (for both x86/x64)
if(UNIX AND APPLE)

endif()

# Linux, BSD, Solaris, Minix (for both x86/x64)
if(UNIX AND NOT APPLE)   

endif()

# Add files to the library
add_library(${PROJECT_NAME} SHARED ${PROJECT_FILES})

# Set version of library
set_target_properties(${PROJECT_NAME} PROPERTIES 
                        VERSION ${PROJECT_VERSION}
                        SOVERSION ${PROJECT_VERSION_MAJOR})

# Link needed libraries
# Qt Library
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# Compile needed definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE QLOGGER_LIB)

# Definition which depends on options
# Ex :
# if(EXT_OPT_QLOGGER_XYZ)
#   target_compile_definitions(${PROJECT_NAME} PRIVATE QLOGGER_ENABLE_XYZ)
# endif()

# Directories to includes
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
