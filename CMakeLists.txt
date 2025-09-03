cmake_minimum_required(VERSION 3.22)
project(StarWarsCardGame)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(Vulkan REQUIRED)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(third_party/sdl3 EXCLUDE_FROM_ALL)

add_executable(StarWarsCardGame main.cpp)

target_include_directories(StarWarsCardGame PRIVATE imgui third_party/imgui)
target_include_directories(StarWarsCardGame PRIVATE imgui third_party/imgui/backends)

set(IMGUI_DIR third_party/imgui)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)

target_sources(StarWarsCardGame PRIVATE
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_BACKEND_DIR}/imgui_impl_vulkan.cpp
        ${IMGUI_BACKEND_DIR}/imgui_impl_sdl3.cpp
)

# Link to the actual SDL3 library.
target_link_libraries(StarWarsCardGame PRIVATE SDL3::SDL3)

target_link_libraries(StarWarsCardGame PRIVATE Vulkan::Vulkan)