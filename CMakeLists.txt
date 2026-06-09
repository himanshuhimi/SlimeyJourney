cmake_minimum_required(VERSION 4.1)
project("game")

set(LIBRARIES
    SDL3
    SDL3_image
    SDL3_mixer
    SDL3_ttf
    tinyxml2
)
set(LINK_TARGETS "")
set(DIRECTORIES
    assets
    maps
)

foreach(lib ${LIBRARIES})
    find_package(${lib} REQUIRED)
    list(APPEND LINK_TARGETS ${lib}::${lib})
endforeach()

file(GLOB_RECURSE PRIVATE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*/private/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sprites/enemies/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sprites/enemies/private/*.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(${PROJECT_NAME} WIN32
        main.cpp
        ${PRIVATE_SOURCES}
    )
else()
    add_executable(${PROJECT_NAME}
        main.cpp
        ${PRIVATE_SOURCES}
    )
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE src include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${LINK_TARGETS}
    sqlite3
)
# foreach(directory ${DIRECTORIES})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${directory}
)