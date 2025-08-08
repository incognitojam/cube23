#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace Vox {
    enum class ShaderUniformType {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool,
        Sampler2D
    };

    struct ShaderUniform {
        std::string name;
        ShaderUniformType type;
        uint32_t size;
        uint32_t offset;
        uint32_t location;
    };

    struct ShaderUniformBuffer {
        std::string name;
        uint32_t size;
        uint32_t binding;
        std::vector<ShaderUniform> uniforms;
    };

    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual const std::string &getName() const = 0;

        virtual void setInt(const std::string &name, int value) = 0;
        virtual void setFloat(const std::string &name, float value) = 0;
        virtual void setFloat2(const std::string &name, const glm::vec2 &value) = 0;
        virtual void setFloat3(const std::string &name, const glm::vec3 &value) = 0;
        virtual void setFloat4(const std::string &name, const glm::vec4 &value) = 0;
        virtual void setMat3(const std::string &name, const glm::mat3 &matrix) = 0;
        virtual void setMat4(const std::string &name, const glm::mat4 &matrix) = 0;

        static std::shared_ptr<Shader> create(const std::string &filepath);
        static std::shared_ptr<Shader> create(const std::string &name, const std::string &vertexSrc,
                                              const std::string &fragmentSrc);
        static std::shared_ptr<Shader> createFromSpirv(const std::string &name, const std::string &vertexSpvPath,
                                                       const std::string &fragmentSpvPath);
    };

    class ShaderLibrary {
    public:
        void add(const std::string &name, const std::shared_ptr<Shader> &shader);
        void add(const std::shared_ptr<Shader> &shader);
        std::shared_ptr<Shader> load(const std::string &filepath);
        std::shared_ptr<Shader> load(const std::string &name, const std::string &filepath);

        std::shared_ptr<Shader> get(const std::string &name);

        bool exists(const std::string &name) const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    };
}
