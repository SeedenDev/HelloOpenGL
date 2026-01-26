#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Geometry
{
    struct Transform2D
    {
        glm::vec2 position = glm::vec2(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec2 scale = glm::vec2(1.0f);
    };

    struct Transform3D
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
    };
};

namespace MathUtil
{
    glm::mat4 ComputeModelMatrix(Geometry::Transform3D transform);
}