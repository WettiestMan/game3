set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.30)

project(game3 VERSION 0.1 LANGUAGES C CXX)

add_executable(game3
    src/main.cpp)

target_compile_features(game3 PRIVATE cxx_std_23)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib REQUIRED)

target_include_directories(game3 PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(game3 PRIVATE ${raylib_INCLUDE_DIRS})

target_link_libraries(game3 PRIVATE ${raylib_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	    target_compile_options(game3 PRIVATE -fsanitize=address)
        target_link_options(game3 PRIVATE -fsanitize=address)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(game3 PRIVATE /fsanitize=address)
        target_link_options(game3 PRIVATE /fsanitize=address)
    endif()
endif()

if(MINGW)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(game3 PRIVATE -lstdc++exp)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_link_libraries(game3 PRIVATE -lc++)
    endif()

    target_link_libraries(game3 PRIVATE -lopengl32 -lgdi32 -lwinmm)

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_options(game3 PRIVATE -mwindows)
    endif()
endif()

add_custom_target(
    copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets
)
