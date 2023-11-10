#pragma once

#include "vox/renderer/texture.h"

namespace Vox {
    class OpenGLTexture2D final : public Texture2D {
    public:
        explicit OpenGLTexture2D(const std::string &path);
        ~OpenGLTexture2D() override;

        uint32_t getWidth() const override { return mWidth; }
        uint32_t getHeight() const override { return mHeight; }

        void bind(uint32_t slot) const override;

    private:
        std::string mPath;
        uint32_t mWidth, mHeight;
        uint32_t mRendererID;
    };
}
