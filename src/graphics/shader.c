//
// Created by gravypod on 9/20/17.
//

#include "shader.h"

#include "src/util/files.h"

shader_program make_shader(const char const *vertex_name, const char const *fragment_name)
{
    static char compile_message[1024];
    static int success;

    GLuint program = glCreateProgram();
    GLchar *vert = read_file(vertex_name);
    GLchar *frag = read_file(fragment_name);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER),
           fs = glCreateShader(GL_FRAGMENT_SHADER);

    // Send source to GPU
    glShaderSource(vs, 1, (const GLchar * const *) &vert, NULL);
    glShaderSource(fs, 1, (const GLchar * const *) &frag, NULL);

    // Aompile source
    glCompileShader(vs);
    glCompileShader(fs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, sizeof(compile_message), NULL, compile_message);
        printf("\t\tVS Compilation Failed\n%s\n", compile_message);
    }


    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, sizeof(compile_message), NULL, compile_message);
        printf("\t\tFS Compilation Failed\n%s\n", compile_message);
    }


    // Connect shaders
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // Link program together
    glLinkProgram(program);

    // TODO: Check for link stats

    // Free strings
    free(vert);
    free(frag);

    // Remove unneeded shader
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}