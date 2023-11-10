#pragma once

namespace Vox {
    class Timestep {
    public:
        explicit Timestep(const float time = 0.0f) : mTime(time) {
        }

        explicit operator float() const { return mTime; }

        float getSeconds() const { return mTime; }
        float getMilliseconds() const { return mTime * 1000.0f; }

    private:
        float mTime;
    };
}
