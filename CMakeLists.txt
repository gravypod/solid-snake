cmake_minimum_required(VERSION 3.5)
project(engine)

find_package( PythonInterp 2.7 REQUIRED )
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



# VENDOR DUKTAPE
set(VENDOR_DUKTAPE_SOURCES ${CMAKE_SOURCE_DIR}/lib/duktape/duktape.c ${CMAKE_SOURCE_DIR}/lib/duktape/duktape.h ${CMAKE_SOURCE_DIR}/lib/duktape/duk_config.h ${CMAKE_SOURCE_DIR}/lib/duktape/duk_source_meta.json)
add_custom_command(
        OUTPUT ${VENDOR_DUKTAPE_SOURCES}
        PRE_BUILD
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/vendor/duktape/tools/configure.py --output-directory ${CMAKE_SOURCE_DIR}/lib/duktape/ -DDUK_USE_FASTINT -UDUK_USE_ES6_PROXY
)
add_custom_target(localduktape
        ALL
        DEPENDS ${VENDOR_DUKTAPE_SOURCES})

set(VENDOR_GLAD_SOURCES ${CMAKE_SOURCE_DIR}/lib/glad/glad.c ${CMAKE_SOURCE_DIR}/lib/glad/glad.h)
add_custom_command(
	OUTPUT ${VENDOR_GLAD_SOURCES}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/glad/
	PRE_BUILD
	COMMAND ${PYTHON_EXECUTABLE} -m glad --generator c --out-path ${CMAKE_SOURCE_DIR}/lib/glad/ --spec gl --omit-khrplatform --local-files
)
add_custom_target(localglad
	ALL
	DEPENDS ${VENDOR_GLAD_SOURCES})


#        lib/glad/glad.c
#	lib/duktape/duktape.c lib/duktape/duktape.h lib/duktape/duk_config.h

# ALL SOURCES
set(SOURCE_FILES
	${VENDOR_GLAD_SOURCES} ${VENDOR_DUKTAPE_SOURCES}
        src/engine/main.c
        src/engine/scripting/interface.c src/engine/scripting/interface.h
        src/engine/scripting/script.h
        src/engine/util/llist.c src/engine/util/llist.h
        src/engine/util/files.c src/engine/util/files.h
        src/engine/graphics/quadmesh.c src/engine/graphics/quadmesh.h
        src/engine/graphics/shader.c src/engine/graphics/shader.h
        src/engine/graphics/texture.c src/engine/graphics/texture.h lib/stb/stb_image.h
        src/engine/graphics/style.c src/engine/graphics/style.h
        lib/linmath/linmath.h src/engine/scripting/callbacks.c src/engine/scripting/callbacks.h src/engine/graphics/screen.c src/engine/graphics/screen.h)

set(CMAKE_INCLUDE_CURRENT_DIR  ON)



add_executable(engine ${SOURCE_FILES})

# VENDOR DUKTAPE
add_dependencies(engine localduktape localglad)

# GL, GLFW, DL, M
target_link_libraries(engine dl m glfw GL vulkan)

# STANDARDS
set_property(TARGET engine PROPERTY C_STANDARD 99)
enable_c_compiler_flag_if_supported("-Wall")
enable_c_compiler_flag_if_supported("-Wextra")
enable_c_compiler_flag_if_supported("-pedantic")

# COPY SOURCE OF GAME INTO BUILD DIR
add_custom_command(
        TARGET engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/game/
        ${CMAKE_CURRENT_BINARY_DIR}/)
