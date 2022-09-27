add_executable(example main.cpp)
target_link_libraries(example PRIVATE sfml-graphics)


# Copy DLLs needed for runtime on Windows
if(WIN32)
    if (BUILD_SHARED_LIBS)
        add_custom_command(TARGET example POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:sfml-graphics>
                $<TARGET_FILE:sfml-window>
                $<TARGET_FILE:sfml-system>
                $<TARGET_FILE_DIR:example>)
    endif()
endif()