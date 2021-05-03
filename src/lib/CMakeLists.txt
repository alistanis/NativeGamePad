add_subdirectory(MacOS)

add_library(${PROJECT_NAME} STATIC NGP_Internal.h NGP_GamePad.c)

if (APPLE)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-MacOS)
endif()