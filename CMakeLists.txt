cmake_minimum_required(VERSION "3.20.0")

add_subdirectory(deps/glfw)

project("Ignarus")
add_executable(${PROJECT_NAME} 
                src/main.cpp
                src/shadeutil.cpp
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