set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.30)

project(game3 VERSION 0.1 LANGUAGES C CXX)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

add_executable(game3
    src/main.cpp
    src/ResourceManager.cpp
    src/entities/Player.cpp
    src/Game.cpp
    src/entities/Background.cpp
    src/entities/PlayerProjectile.cpp
    src/ResourceElements.cpp
    src/entities/EnemyProjectile.cpp
    src/entities/EnemyMissile.cpp
    src/entities/EnemyProjectileManager.cpp
    src/entities/EnemyUfo.cpp
    src/entities/EnemyUfoManager.cpp
    src/CollisionManager.cpp
    src/entities/EnemyFighter.cpp)

target_compile_features(game3 PRIVATE cxx_std_23)

find_package(raylib REQUIRED)

CPMAddPackage(
    NAME magic_enum
    GITHUB_REPOSITORY Neargye/magic_enum
    GIT_TAG v0.9.7 # Where `x.y.z` is the release version you want to use.
)

target_include_directories(game3 PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(game3 PRIVATE ${raylib_INCLUDE_DIRS})

target_link_libraries(game3 PRIVATE magic_enum::magic_enum)
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
