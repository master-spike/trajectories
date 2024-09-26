cmake_minimum_required(VERSION 3.20)

project(trajectories)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(thirdparty)

set(IMGUI_PATH  "${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui")

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(OpenGL)

find_package(SDL2 REQUIRED)

file(GLOB IMGUI_SOURCES
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp

    ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)

add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH}/backends)
target_include_directories("ImGui" PUBLIC ${SDL2_INCLUDE_DIRS})

set(TRAJLIB_PATH "${CMAKE_CURRENT_LIST_DIR}/lib")
set(TRAJLIB_INLCUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")

add_library(trajlib STATIC ${TRAJLIB_PATH}/orbit.cpp)
target_include_directories(trajlib PUBLIC ${TRAJLIB_INLCUDE_DIRS})
target_link_libraries(trajlib PUBLIC glm::glm-header-only)

add_custom_target(shaders
    ${CMAKE_COMMAND}
    -D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
    -D DESTINATION_DIR=${CMAKE_CURRENT_BINARY_DIR}
    -P ${CMAKE_CURRENT_LIST_DIR}/CopyShaders.cmake
    comment "Copying shaders." VERBATIM
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/orbit_view_window.cpp
    src/scene.cpp
    src/load_shaders.cpp
)

add_dependencies(${PROJECT_NAME} shaders)

target_link_libraries(${PROJECT_NAME} PUBLIC ImGui SDL2 OpenGL::GL glm::glm-header-only trajlib)
target_include_directories(${PROJECT_NAME} PUBLIC ImGui ${SDL2_INCLUDE_DIRS})

