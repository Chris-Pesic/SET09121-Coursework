cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Games_Engineering)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####

#SFML
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#TinyXML2
add_subdirectory("lib/tinyxml2")
set(TINYXML2_INCS "lib/tinyxml2/include")

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

#### Coursework ####

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(main ${SOURCES})
target_include_directories(main PRIVATE ${SFML_INCS} ${TINYXML2_INCS})
target_link_libraries(main sfml-graphics sfml-audio tinyxml2)
