# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

# Import Pico SDK helper
include(pico_sdk_import.cmake)

project(oled_ssd1306_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Example executable
add_executable(oled_example
    main.c
    OLED_SSD1306/oled.c
    OLED_SSD1306/ssd1306_i2c.c
)

# Includes for the example
target_include_directories(oled_example PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/OLED_SSD1306
)

# Link against Pico SDK libs
target_link_libraries(oled_example
    pico_stdlib
    hardware_i2c
)

# Enable USB stdio, disable UART by default (adjust as needed)
pico_enable_stdio_usb(oled_example 1)
pico_enable_stdio_uart(oled_example 0)

# Create UF2/ELF
pico_add_extra_outputs(oled_example)
