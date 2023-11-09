#include "vox/renderer/orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Vox {
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : mProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), mViewMatrix(1.0f) {
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }

    void OrthographicCamera::recalculateViewMatrix() {
        const glm::mat4 transform = translate(glm::mat4(1.0f), mPosition) *
                                    rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0, 0, 1));

        mViewMatrix = inverse(transform);
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }
}
