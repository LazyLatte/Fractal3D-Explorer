cmake_minimum_required(VERSION 3.10.0)
project(Fractal3D-Explorer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

file(GLOB_RECURSE HEADERS 
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/include/fractals/*.h"
)

file(GLOB_RECURSE SOURCES 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW glfw glm::glm imgui::imgui)

set(SHADER_SOURCE_DIRS 
    ${CMAKE_SOURCE_DIR}/shaders
)
set(COPIED_SHADERS)

foreach(SHADER_DIR ${SHADER_SOURCE_DIRS})
    file(GLOB SHADER_FILES
        "${SHADER_DIR}/*.vert"
        "${SHADER_DIR}/*.frag"
        "${SHADER_DIR}/*.comp"
        "${SHADER_DIR}/*.glsl"
    )

    foreach(SHADER ${SHADER_FILES})
        get_filename_component(FILENAME ${SHADER} NAME)

        set(DEST_FILE ${CMAKE_CURRENT_BINARY_DIR}/shaders/${FILENAME})

        add_custom_command(
            OUTPUT ${DEST_FILE}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER} ${DEST_FILE}
            DEPENDS ${SHADER}
            COMMENT "Copying shader: ${FILENAME}"
        )

        list(APPEND COPIED_SHADERS ${DEST_FILE})
    endforeach()
endforeach()

add_custom_target(copy_shaders ALL DEPENDS ${COPIED_SHADERS})

include(CTest)
enable_testing()

