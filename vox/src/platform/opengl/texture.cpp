#include "platform/opengl/texture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace Vox {
    OpenGLTexture2D::OpenGLTexture2D(const std::string &path) : mPath(path) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            throw std::runtime_error("Failed to load image!");
        }
        mWidth = width;
        mHeight = height;

        glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
        glTextureStorage2D(mRendererID, 1, GL_RGB8, mWidth, mHeight);

        glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &mRendererID);
    }

    void OpenGLTexture2D::bind(const uint32_t slot) const {
        glBindTextureUnit(slot, mRendererID);
    }
}
