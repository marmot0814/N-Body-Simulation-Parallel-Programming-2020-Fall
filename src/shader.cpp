#include "shader.h"

Shader::Shader(const GLchar* vertex_path, const GLchar* fragment_path) {
    // 1. retrieve the vertex/fragment source code from filePath

    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vertex_file;
    std::ifstream fragment_file;

    // ensures ifstream objects can throw exceptions:
    vertex_file.exceptions(std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::badbit);
    try {
        // open files
        vertex_file.open(vertex_path);
        fragment_file.open(fragment_path);
        std::stringstream vertex_stream, fragment_stream;

        // read file's buffer contents into streams
        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        // close file handlers
        vertex_file.close();
        fragment_file.close();

        // convert stream into string
        vertex_code = vertex_stream.str();
        fragment_code = fragment_stream.str();
    } catch (const std::ifstream::failure& e) {
        std::cout << "ERROR - read file failed" << std::endl;
    }

    const GLchar* vertex_code_chars = vertex_code.c_str();
    const GLchar* fragment_code_chars = fragment_code.c_str();

    // 2. compile shaders

    GLuint vertex, fragment;
    GLint success;
    GLchar info_log[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code_chars, NULL);
    glCompileShader(vertex);

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cout << "ERROR - vertex shader compilation failed:\n"
                  << info_log << std::endl;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code_chars, NULL);
    glCompileShader(fragment);

    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cout << "ERROR - fragment shader compilation failed:\n"
                  << info_log << std::endl;
    }

    // shader program
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    // print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        std::cout << "ERROR - shader link failed:\n"
                  << info_log << std::endl;
    }

    // delete the shaders as they're linked into our program now and no
    // longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
