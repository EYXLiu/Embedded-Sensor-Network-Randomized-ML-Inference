cmake_minimum_required(VERSION 3.15)
project(EmbeddedML C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_C_STANDARD 11)

# ----------------------------
# FreeRTOS kernel
# ----------------------------
file(GLOB FREERTOS_SRCS
    FreeRTOS-Kernel/*.c
    FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/*.c
    FreeRTOS-Kernel/portable/MemMang/*.c
)

add_library(freertos_kernel STATIC ${FREERTOS_SRCS})

target_include_directories(freertos_kernel PUBLIC
    FreeRTOS-Kernel/include
    FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix
    FreeRTOS-Kernel/portable/MemMang
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ----------------------------
# TensorFlow Lite Micro includes
# ----------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro/tensorflow
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro/tensorflow/lite
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro/tensorflow/lite/core/api
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro/tensorflow/lite/micro
    ${CMAKE_CURRENT_SOURCE_DIR}/.venv/lib/python3.13/site-packages/tensorflow/include/external/gemmlowp
    ${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers/flatbuffers/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro/tensorflow/lite/kernels/internal
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro/tensorflow/lite/kernels/internal/fixedpoint
    ${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro/third_party
    ${CMAKE_CURRENT_SOURCE_DIR}/tflite-micro/third_party/ruy
)

# ----------------------------
# TensorFlow Lite Micro sources
# Automatically include all .cc files in relevant directories
# ----------------------------
set(TFLM_SRCS
    # Core
    tflite-micro/tensorflow/lite/core/c/common.cc
    tflite-micro/tensorflow/lite/core/api/flatbuffer_conversions.cc
    tflite-micro/tensorflow/compiler/mlir/lite/core/api/error_reporter.cc

    # Micro interpreter
    tflite-micro/tensorflow/lite/micro/micro_allocator.cc
    tflite-micro/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.cc
    tflite-micro/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.cc
    tflite-micro/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.cc
    tflite-micro/tensorflow/lite/micro/micro_interpreter.cc
    tflite-micro/tensorflow/lite/micro/micro_interpreter_context.cc
    tflite-micro/tensorflow/lite/micro/micro_interpreter_graph.cc
    tflite-micro/tensorflow/lite/micro/micro_resource_variable.cc
    tflite-micro/tensorflow/lite/micro/micro_log.cc
    tflite-micro/tensorflow/lite/micro/micro_context.cc
    tflite-micro/tensorflow/lite/micro/micro_op_resolver.cc
    tflite-micro/tensorflow/lite/micro/micro_utils.cc
    tflite-micro/tensorflow/lite/micro/memory_helpers.cc
    tflite-micro/tensorflow/lite/micro/debug_log.cc

    # Micro memory planners
    tflite-micro/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc
    tflite-micro/tensorflow/lite/micro/memory_planner/linear_memory_planner.cc

    # Kernels
    tflite-micro/tensorflow/lite/micro/kernels/fully_connected.cc
    tflite-micro/tensorflow/lite/micro/kernels/activations.cc
    tflite-micro/tensorflow/lite/micro/kernels/add.cc
    tflite-micro/tensorflow/lite/micro/kernels/conv.cc
    tflite-micro/tensorflow/lite/micro/kernels/depthwise_conv.cc
    tflite-micro/tensorflow/lite/micro/kernels/pooling.cc
    tflite-micro/tensorflow/lite/micro/kernels/softmax.cc
    tflite-micro/tensorflow/lite/micro/kernels/quantize.cc
    tflite-micro/tensorflow/lite/micro/kernels/reshape.cc

    tflite-micro/tensorflow/lite/micro/kernels/kernel_util.cc

    tflite-micro/tensorflow/lite/kernels/internal/common.cc
    tflite-micro/tensorflow/lite/micro/kernels/add_common.cc
    tflite-micro/tensorflow/lite/micro/kernels/activations_common.cc
    tflite-micro/tensorflow/lite/micro/kernels/conv_common.cc
    tflite-micro/tensorflow/lite/micro/kernels/softmax_common.cc
    tflite-micro/tensorflow/lite/micro/kernels/pooling_common.cc
    tflite-micro/tensorflow/lite/micro/kernels/depthwise_conv_common.cc
    tflite-micro/tensorflow/lite/micro/kernels/fully_connected_common.cc
    tflite-micro/tensorflow/lite/micro/kernels/quantize_common.cc
    tflite-micro/tensorflow/lite/micro/kernels/reshape_common.cc

    # Internal kernels/utilities
    tflite-micro/tensorflow/lite/kernels/internal/tensor_utils.cc
    tflite-micro/tensorflow/lite/kernels/internal/tensor_ctypes.cc
    tflite-micro/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.cc
    tflite-micro/tensorflow/lite/kernels/internal/portable_tensor_utils.cc
    tflite-micro/tensorflow/lite/kernels/kernel_util.cc

    tflite-micro/tensorflow/compiler/mlir/lite/schema/schema_utils.cc
    tflite-micro/tensorflow/lite/micro/tflite_bridge/flatbuffer_conversions_bridge.cc
    tflite-micro/tensorflow/lite/micro/tflite_bridge/micro_error_reporter.cc
    tflite-micro/tensorflow/lite/micro/flatbuffer_utils.cc
    tflite-micro/tensorflow/lite/micro/micro_allocation_info.cc
    tflite-micro/tensorflow/lite/array.cc
    tflite-micro/tensorflow/lite/kernels/internal/quantization_util.cc
    tflite-micro/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.cc
)

# ----------------------------
# Your app sources
# ----------------------------
file(GLOB APP_SRCS
    src/*.c
    src/*.cpp
    models/*.cc
)

# ----------------------------
# Executable
# ----------------------------
add_executable(embml ${APP_SRCS} ${TFLM_SRCS})

target_link_libraries(embml PRIVATE freertos_kernel pthread)
