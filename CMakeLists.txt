cmake_minimum_required(VERSION 3.5)

project(Chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

#####################################
# LIBRARY
#####################################
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

message(STATUS "GLFW source dir: ${glfw_SOURCE_DIR}")
message(STATUS "GLFW binary dir: ${glfw_BINARY_DIR}")
message(STATUS "GLFW done")

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)
message(STATUS "GLM done")

find_package(Vulkan REQUIRED)
message(STATUS "Vulkan done")

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)
message(STATUS "STB done")

#####################################
# FILES
#####################################

file(GLOB_RECURSE SOURCE_FILES
    ${SOURCE_DIR}/*.cpp
    ${SOURCE_DIR}/*.h
)

foreach(SRC IN ITEMS ${SOURCE_FILES})
    get_filename_component(SRC_PATH "${SRC}" PATH)
    file(RELATIVE_PATH SRC_PATH_REL "${SOURCE_DIR}" "${SRC_PATH}")
    string(REPLACE "/" "\\" GROUPE_PATH "/${SRC_PATH_REL}")
    source_group("Source${GROUPE_PATH}" FILES "${SRC}")
endforeach()


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR} ${stb_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw Vulkan::Vulkan glm::glm)