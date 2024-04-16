cmake_minimum_required(VERSION "3.20.0")

add_subdirectory(deps/glfw)

project("Ignarus")
add_executable(${PROJECT_NAME} 
                src/main.cpp
                src/shadeutil.cpp
                src/Renderer.cpp
                src/VertexBuffer.cpp
                src/IndexBuffer.cpp
                deps/GLAD/src/glad.c
                 )

target_link_libraries(${PROJECT_NAME}
                        glfw
                        )


target_include_directories(${PROJECT_NAME}
                    PUBLIC deps/glfw/include 
                    PUBLIC deps/GLAD/include
                    PUBLIC include
)

add_custom_target(copy_resources ALL 
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res
        ${PROJECT_BINARY_DIR}/res
        COMMENT "Copying resources into the build directory")

add_dependencies("Ignarus" copy_resources)