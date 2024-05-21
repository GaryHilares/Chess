# Project settings
cmake_minimum_required(VERSION 3.3.0)
project(Chess VERSION 0.1.0)

include(CTest)
enable_testing()

# Build settings
set(CMAKE_BUILD_TYPE "Debug")

# Build process
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(BUILD_TYPES "Debug" "Release")
if(NOT ("${CMAKE_BUILD_TYPE}" IN_LIST BUILD_TYPES))
    message(FATAL_ERROR "CMAKE_BUILD_TYPE not recognized: ${CMAKE_BUILD_TYPE} not in ${BUILD_TYPES}")
endif()

set(SFML_ROOT_DIR "extlibs/SFML-2.5.1")
set(SFML_DIR "${SFML_ROOT_DIR}/lib/cmake/SFML")

if(WIN32)
    find_package(SFML COMPONENTS main graphics window system REQUIRED)
else()
    find_package(SFML COMPONENTS graphics window system REQUIRED)
endif()

set(CPPSources
    "src/model/GameState.cpp"
    "src/GameUI.cpp"
    "src/main.cpp"
    "src/model/Piece.cpp"
    "src/model/BoardCoordinate.cpp"
    )


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
make_directory("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(Chess WIN32 ${CPPSources})
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(Chess ${CPPSources})
endif()

if(WIN32)
    set(LINK_LIBRARIES sfml-main sfml-graphics sfml-window sfml-system stdc++fs)
else()
    set(LINK_LIBRARIES sfml-graphics sfml-window sfml-system stdc++fs)
endif()

set(INCLUDE_DIRS ${SFML_ROOT_DIR}/include)
target_include_directories(Chess PRIVATE ${INCLUDE_DIRS})
target_link_libraries(Chess ${LINK_LIBRARIES})

if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(SFML_DLL_FILENAMES "sfml-graphics-2.dll" "sfml-system-2.dll" "sfml-window-2.dll")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Tests")
        set(SFML_DLL_FILENAMES "sfml-graphics-d-2.dll" "sfml-system-d-2.dll" "sfml-window-d-2.dll")
    endif()

    foreach(FILENAME ${SFML_DLL_FILENAMES})
        file(COPY "${SFML_ROOT_DIR}/bin/${FILENAME}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    endforeach()
endif()

make_directory("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")
file(COPY "${PROJECT_SOURCE_DIR}/ChessPieces.png" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)