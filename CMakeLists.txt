# This file is generated only once,
# and is not re-generated. 
# User is free to modify the file as much as necessary.

# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.22)

# ------------------------------------------------------------------------------

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# ------------------------------------------------------------------------------

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# ------------------------------------------------------------------------------

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME ttsy-oscillator)
# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# ------------------------------------------------------------------------------

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# ------------------------------------------------------------------------------

# Add OneMessage (3rd-party publish-subscribe message framework)
# add_subdirectory(Middlewares/Third_Party/OneMessage-4.4/src om.out)

# Pass om_config.h to OmeMessage
# target_include_directories(OneMessage PUBLIC User/Config)

# Pass stm32cubemx path and macro to OneMessage
# target_link_libraries(OneMessage PUBLIC stm32cubemx)

# Add
# target_include_directories(OneMessage INTERFACE
#     Middlewares/Third_Party/OneMessage-4.4/src
#     Middlewares/Third_Party/OneMessage-4.4/src/app
#     Middlewares/Third_Party/OneMessage-4.4/src/core
#     Middlewares/Third_Party/OneMessage-4.4/src/comp
# )

# ------------------------------------------------------------------------------

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
    # OneMessage
)

# ------------------------------------------------------------------------------

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# ------------------------------------------------------------------------------

# Add User sources
file(GLOB USER_SOURCES
    User/Board/*.c
    User/Drivers/*.c
    User/Tasks/*.c
)

# Add User includes
set(USER_INCLUDES
    User/Config/
    User/Drivers/
    User/Drivers/GUI/
    User/Tasks/
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${USER_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${USER_INCLUDES}
)

# ------------------------------------------------------------------------------

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)
