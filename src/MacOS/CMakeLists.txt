if(APPLE)
    SET(CMAKE_C_COMPILER "/usr/bin/clang")
    SET(CMAKE_C_FLAGS "-mmacosx-version-min=11.3")
    add_executable(${PROJECT_NAME}-MacOS main.m)

    find_library(APPKIT AppKit)
    find_library(GAME_CONTROLLER GameController)

    target_link_libraries(${PROJECT_NAME}-MacOS ${APPKIT} ${GAME_CONTROLLER})
endif()