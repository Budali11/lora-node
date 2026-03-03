cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME lora-node)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
file(GLOB_RECURSE USER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/PhaseLoRa/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/lr11xx_driver/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/lr1121_common/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/lr1121_modem/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/lr1121_printers/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/examples/lr1121_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/examples/lr1121_ping_pong/lr1121_ping_pong.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-0in91/OLED/OLED_0in91.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-0in91/GUI/GUI_Paint.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-0in91/Fonts/*!{CN}.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-GUI/*.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-0in91/Example/ImageData.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-0in91/Example/OLED_0in91_test.c
)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${USER_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/threads.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-GUI/menu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/MultiButton/multi_button.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/module_redirect.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/PhaseLoRa/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/MultiButton/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-GUI/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/lr11xx_driver/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/lr1121_common/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/lr1121_modem/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/lr1121/lr1121_printers/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/examples/
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Core-1121/examples/lr1121_ping_pong/
    # ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-0in91/OLED/
    # ${CMAKE_CURRENT_SOURCE_DIR}/Module/OLED-0in91/Example/
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

# Copy compile_commands.json to build folder after build
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_BINARY_DIR}/../compile_commands.json
    COMMENT "Copying compile_commands.json to build folder"
)
