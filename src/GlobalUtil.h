#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <string>

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

namespace Curl
{
    static size_t WriteToStringCallback(void* ptr, size_t size, size_t count, std::string* data)
    {
        data->append((char*)ptr, size * count);
        return size * count;
    }

    const std::string GetRemoteImage(const std::string& url);
}

//TODO: I don't know how to name it better neither what a name a file that contains them could be so it's here for now
namespace Common
{
    class HasPosition
    {
    protected:
        glm::vec3 m_Position = glm::vec3(0.0f);

    public:

        void SetPosition(const glm::vec3& position)
        {
            m_Position = position;
            OnPositionUpdate(position);
        }
        inline const glm::vec3& GetPosition() const { return m_Position; }

    protected:

        virtual void OnPositionUpdate(const glm::vec3& position) {}
    };

    class HasDirection
    {
    protected:
        glm::vec3 m_Direction = glm::vec3(0.0f);

    public:

        void SetDirection(const glm::vec3& direction)
        {
            m_Direction = direction;
            OnDirectionUpdate(direction);
        }
        inline const glm::vec3& GetDirection() const { return m_Direction; }

    protected:

        virtual void OnDirectionUpdate(const glm::vec3& direction) {}
    };
}