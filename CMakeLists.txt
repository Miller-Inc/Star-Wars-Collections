cmake_minimum_required(VERSION 3.22)
project(StarWarsCardGame CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(Vulkan REQUIRED)


# This assumes the SDL source is available in the third_party/sdl3 directory.
add_subdirectory(third_party/sdl3 EXCLUDE_FROM_ALL)
add_subdirectory(third_party/sdl3_net)
add_subdirectory(third_party/json)

set(STAR_WARS_CARD_GAME_SOURCES
        src/Windows/SW_Window.cpp
        src/Vulkan/VulkanSetup.cpp
        src/EngineTypes/Vector.cpp
        src/Network/NetworkManager.cpp
        src/EngineTypes/Array.cpp
        src/Game/GameInstance.cpp
        src/Windows/NetworkPlayWindow.cpp
        src/Windows/MainWindow.cpp
        src/Windows/WindowBase.cpp
)

set(STAR_WARS_CARD_GAME_HEADERS
        include/Windows/SW_Window.h
        third_party/stb/stb_image.h
        include/Vulkan/VulkanSetup.hpp
        include/EngineTypes/Vector.h
        include/EngineTypes/Image.h
        include/EngineTypes/String.h
        include/EngineTypes/Logger.h
        include/Game/GameInstance.h
        include/Network/NetworkManager.h
        include/EngineTypes/Array.h
        include/Windows/NetworkPlayWindow.h
        include/Windows/WindowBase.h
        include/Windows/MainWindow.h
        include/Game/WindowTypes.h
        include/EngineTypes/ResourceLoader.h
        src/EngineTypes/ResourceLoader.cpp
)



add_executable(StarWarsCardGame main.cpp
        ${STAR_WARS_CARD_GAME_HEADERS}
        ${STAR_WARS_CARD_GAME_SOURCES}
)

# This condition checks if the current platform is Windows.
if(WIN32)
    # The commands inside this block will only be executed on Windows.
    message(STATUS "Static linking is enabled for Windows.")

    # Check if the Visual Studio compiler is being used.
    if(MSVC)
        target_compile_options(StarWarsCardGame PRIVATE /MT)
        # Check if the MinGW compiler is being used.
    elseif(MINGW)
        # Pass the static linking flags for MinGW.
        target_link_options(StarWarsCardGame PRIVATE
                -static
                -static-libgcc
                -static-libstdc++
        )
    endif()
endif()

target_include_directories(StarWarsCardGame PRIVATE imgui third_party/imgui)
target_include_directories(StarWarsCardGame PRIVATE imgui third_party/imgui/backends)
target_include_directories(StarWarsCardGame PRIVATE stb third_party/stb)
target_include_directories(StarWarsCardGame PRIVATE SWCG include)
target_include_directories(StarWarsCardGame PRIVATE third_party/imrad/include)
target_include_directories(StarWarsCardGame PRIVATE third_party/json/include)

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
target_link_libraries(StarWarsCardGame PUBLIC SDL3::SDL3 SDL3_net::SDL3_net Vulkan::Vulkan)
target_link_libraries(StarWarsCardGame PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(StarWarsCardGame PUBLIC ${NLOHMANN_JSON_TARGET_NAME})

add_custom_command(TARGET StarWarsCardGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:StarWarsCardGame>/Resources
)