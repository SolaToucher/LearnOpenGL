cmake_minimum_required(VERSION 3.10)

project(A_Shader)

set(CMAKE_CXX_STANDARD 17)

find_package(GLFW3 REQUIRED)
find_package(GLAD)
find_package(GLM)
find_package(Assimp)

if(GLM_FOUND)
    message ("glm found")
    include_directories(${GLM_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Cannot find glm")
endif()

if(GLFW3_FOUND)
    message ("glfw found")
    include_directories(${GLFW3_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Cannot find glfw")
endif()

if(GLAD_FOUND)
    message ("glad found")
    include_directories(${GLAD_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Cannot find glad")
endif()

if(assimp_FOUND)
    message ("assimp found")
    include_directories(${assimp_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Cannot find assimp")
endif()

aux_source_directory(. DIR_SRCS)

add_executable(A_Light ${DIR_SRCS})

target_include_directories(A_Light
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/external/
)

target_link_libraries(A_Light ${GLFW3_LIBRARY} ${GLAD_LIBRARIES} ${assimp_LIBRARIES} dl pthread)