project(CMSIS LANGUAGES C CXX ASM)

add_library(CMSIS INTERFACE)

target_include_directories(CMSIS INTERFACE
    ${DRIVERS_DIR}/CMSIS/
    ${DRIVERS_DIR}/CMSIS/GD/GD32L23x/Include

	# Added directory of "gd32l23x_libopt.h".
    ${CMAKE_SOURCE_DIR}/Application/Core/Inc
    )
