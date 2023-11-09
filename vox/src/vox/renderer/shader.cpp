#include "vox/renderer/shader.h"

#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>

#include <glad/glad.h>

namespace Vox {

    Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const GLchar *source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);
        glCompileShader(vertexShader);

        GLint compiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);

            // TODO: log this error
            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);

            // TODO: log this error
            return;
        }

        mRendererID = glCreateProgram();
        glAttachShader(mRendererID, vertexShader);
        glAttachShader(mRendererID, fragmentShader);
        glLinkProgram(mRendererID);

        GLint linked = 0;
        glGetProgramiv(mRendererID, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(mRendererID, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(mRendererID);
            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);

            // TODO: log this error
            return;
        }

        glDetachShader(mRendererID, vertexShader);
        glDetachShader(mRendererID, fragmentShader);
    }

    Shader::~Shader() {
        glDeleteProgram(mRendererID);
    }

    void Shader::bind() const {
        glUseProgram(mRendererID);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }
}
