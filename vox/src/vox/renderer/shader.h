#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Vox {
    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual const std::string &getName() const = 0;

        static std::shared_ptr<Shader> create(const std::string &filepath);
        static std::shared_ptr<Shader> create(const std::string &name, const std::string &vertexSrc,
                                              const std::string &fragmentSrc);
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
