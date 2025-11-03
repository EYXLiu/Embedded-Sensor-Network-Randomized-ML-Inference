cmake_minimum_required(VERSION 3.15)
project(MyFreeRTOSDemo C)

set(CMAKE_OSX_ARCHITECTURES x86_64)

# FreeRTOS kernel library
add_library(freertos_kernel STATIC
    FreeRTOS-Kernel/croutine.c
    FreeRTOS-Kernel/event_groups.c
    FreeRTOS-Kernel/list.c
    FreeRTOS-Kernel/queue.c
    FreeRTOS-Kernel/stream_buffer.c
    FreeRTOS-Kernel/tasks.c
    FreeRTOS-Kernel/timers.c
    FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/port.c
    FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
    FreeRTOS-Kernel/portable/MemMang/heap_3.c
)

target_include_directories(freertos_kernel PUBLIC
    FreeRTOS-Kernel/include
    FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix
    FreeRTOS-Kernel/portable/MemMang
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Your main executable
add_executable(RTOSDemo 
   src/main.c
   src/sensor.c
   src/base_station.c
)

# Link FreeRTOS + POSIX pthread
target_link_libraries(RTOSDemo PRIVATE freertos_kernel pthread)
