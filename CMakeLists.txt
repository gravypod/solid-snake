cmake_minimum_required(VERSION 3.8)
project(engine)

include(CheckCCompilerFlag)

# Source: https://stackoverflow.com/a/33266748/1127064
function(enable_c_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_C_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_c_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

set(SOURCE_FILES
        lib/glad/glad.c
        lib/duktape/duktape.c
        src/main.c
        src/scripting/interface.c src/scripting/interface.h
        src/scripting/script.h
        src/util/llist.c src/util/llist.h
        src/util/files.c src/util/files.h
        src/graphics/quadmesh.c src/graphics/quadmesh.h
        src/graphics/shader.c src/graphics/shader.h
        src/graphics/texture.c src/graphics/texture.h lib/stb/stb_image.h
        src/graphics/style.c src/graphics/style.h
        lib/linmath/linmath.h src/scripting/callbacks.c src/scripting/callbacks.h)

set(CMAKE_INCLUDE_CURRENT_DIR  ON)

add_executable(engine ${SOURCE_FILES})
target_link_libraries(engine dl m glfw GL)
set_property(TARGET engine PROPERTY C_STANDARD 99)
enable_c_compiler_flag_if_supported("-Wall")
enable_c_compiler_flag_if_supported("-Wextra")
#enable_c_compiler_flag_if_supported("-pedantic")
