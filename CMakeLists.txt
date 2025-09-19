cmake_minimum_required(VERSION 3.18)
project(pyclang-format)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(LLVM_VERSION 21.1.1)
set(LLVM_DOWNLOAD_URL "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/llvm-project-${LLVM_VERSION}.src.tar.xz")
set(LLVM_ENABLE_PROJECTS "clang" CACHE STRING "" FORCE)
set(LLVM_TARGETS_TO_BUILD "" CACHE STRING "" FORCE)
set(LLVM_INCLUDE_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_TOOLS ON CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_UTILS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(llvm-project
        URL ${LLVM_DOWNLOAD_URL}
        SOURCE_SUBDIR llvm
)
FetchContent_MakeAvailable(llvm-project)

execute_process(
        COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE pybind11_ROOT)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_format src/pyclang_format/_format.cpp)
target_link_libraries(_format PRIVATE clangFormat)
target_include_directories(_format PRIVATE
        ${llvm-project_BINARY_DIR}/include
        ${llvm-project_SOURCE_DIR}/llvm/include
        ${llvm-project_SOURCE_DIR}/clang/include)
install(TARGETS _format DESTINATION pyclang_format COMPONENT python)
