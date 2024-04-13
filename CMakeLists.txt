cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(mqtt_motor_ctrl C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#Caso seja a Raspberry Pi Pico W
set(PICO_BOARD pico_w)

pico_sdk_init()

set( SOURCES
        main.c
        motor.c
        mqtt.c
)

set( HEADERS
        motor.h
        mqtt.h
        wifi_config.h
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} )

pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(${CMAKE_PROJECT_NAME}
        tinyusb_board
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        pico_lwip_mqtt
        hardware_pwm
    )
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
