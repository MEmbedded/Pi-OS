set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER /opt/homebrew/bin/aarch64-elf-gcc)
set(CMAKE_ASM_COMPILER /opt/homebrew/bin/aarch64-elf-gcc)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS_INIT
        "-mcpu=cortex-a53 -ffreestanding"
)

set(CMAKE_ASM_FLAGS_INIT
        "-mcpu=cortex-a53"
)