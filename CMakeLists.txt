cmake_minimum_required(VERSION 3.20)

project(kernel C ASM)
set(CMAKE_C_STANDARD 11)

add_executable(kernel.elf
        src/boot.S
        src/systemReset.S
        src/kernel.c
        src/MailBox.c
        src/Helper.c
        src/GraphicsDemo.c
)

target_include_directories(kernel.elf PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(kernel.elf PRIVATE
        $<$<COMPILE_LANGUAGE:C>:
        -ffreestanding
        -O2
        -Wall
        -Wextra
        >
)

target_link_options(kernel.elf PRIVATE
        -nostdlib
        -nostartfiles
        -T${CMAKE_SOURCE_DIR}/src/linker.ld
)

add_custom_command(
        TARGET kernel.elf POST_BUILD
        COMMAND aarch64-elf-objcopy
        -O binary
        kernel.elf
        ${CMAKE_BINARY_DIR}/kernel8.img
        COMMAND ${CMAKE_COMMAND} -E sleep 2
        COMMAND ${CMAKE_SOURCE_DIR}/make_image.sh
)