cmake_minimum_required(VERSION 3.5)

project(Chess)

set(CMAKE_CXX_STANDARD 20)
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

# ============================================================================
# Compilation shaders
# ============================================================================


set(SHADER_SRC_DIR ${SOURCE_DIR}/shader)
set(SHADER_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shader)
find_program(GLSLC glslc REQUIRED)

file(GLOB SHADER_FILES 
    ${SHADER_SRC_DIR}/*.rgen 
    ${SHADER_SRC_DIR}/*.rchit 
    ${SHADER_SRC_DIR}/*.rmiss
    ${SHADER_SRC_DIR}/*.comp
    ${SHADER_SRC_DIR}/*.glsl
)

file(MAKE_DIRECTORY ${SHADER_OUT_DIR})

set(SPV "")
foreach(SHADER ${SHADER_FILES})
    get_filename_component(NAME ${SHADER} NAME)
    

    get_filename_component(EXT ${SHADER} EXT)
    if(EXT STREQUAL ".glsl")
        if(NAME MATCHES "vert\\.glsl$")
            set(STAGE "vert")
        elseif(NAME MATCHES "frag\\.glsl$")
            set(STAGE "frag")
        elseif(NAME MATCHES "comp\\.glsl$")
            set(STAGE "comp")
        elseif(NAME MATCHES "rgen\\.glsl$")
            set(STAGE "rgen")
        elseif(NAME MATCHES "rchit\\.glsl$")
            set(STAGE "rchit")
        elseif(NAME MATCHES "rmiss\\.glsl$")
            set(STAGE "rmiss")
        else()
            message(FATAL_ERROR "Unknown shader stage for ${NAME}")
        endif()
    elseif(EXT STREQUAL ".rgen")
        set(STAGE "rgen")
    elseif(EXT STREQUAL ".rchit")
        set(STAGE "rchit")
    elseif(EXT STREQUAL ".rmiss")
        set(STAGE "rmiss")
    elseif(EXT STREQUAL ".comp")
        set(STAGE "comp")
    endif()

    add_custom_command(
        OUTPUT ${SHADER_OUT_DIR}/${NAME}.spv
        COMMAND "${GLSCL}" --target-env=vulkan1.4 -fshader-stage=${STAGE} -I ${SHADER_SRC_DIR} ${SHADER} -o ${SHADER_OUT_DIR}/${NAME}.spv
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${NAME} as ${STAGE}"
    )
    list(APPEND SPV ${SHADER_OUT_DIR}/${NAME}.spv)
endforeach()
        
add_custom_target(shader DEPENDS ${SPV})

# ============================================================================
# Executable
# ============================================================================

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} shader)

target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR} ${stb_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw Vulkan::Vulkan glm::glm)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/image/PNGs/No shadow/1x"
)