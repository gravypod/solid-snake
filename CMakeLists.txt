cmake_minimum_required(VERSION 3.8)
project(engine)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_C_STANDARD c99)


set(SOURCE_FILES lib/glad/glad.c src/main.c lib/duktape/duktape.c src/scripting/interface.c src/scripting/interface.h src/scripting/script.h src/drawing.c src/drawing.h src/util/llist.c src/util/llist.h) # scripting.h scripting.c interfacing.c interfacing.h glad.c)

set(CMAKE_INCLUDE_CURRENT_DIR  ON)

add_executable(engine ${SOURCE_FILES})
target_link_libraries(engine dl m glfw glut GL GLU X11 Xxf86vm Xrandr pthread Xi)
set_property(TARGET engine PROPERTY C_STANDARD 99)
