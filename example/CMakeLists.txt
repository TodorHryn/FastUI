
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

#Controls
add_executable(FastUI-Controls
        controls.cpp
)
target_link_libraries(FastUI-Controls PUBLIC FastUI)

#Calculator
add_executable(FastUI-Calculator
        calculator.cpp
)
target_link_libraries(FastUI-Calculator PUBLIC FastUI)

#Chat
add_executable(FastUI-Chat
    chat.cpp
)
target_link_libraries(FastUI-Chat PUBLIC FastUI)

#Copy library dlls to folder with executable
add_custom_command(TARGET FastUI-Controls POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:FastUI-Controls>)
