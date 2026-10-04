#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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
    glm::mat4 ComputeModelMatrix(const Geometry::Transform3D& transform);
}

namespace Curl
{
    inline size_t WriteToStringCallback(void* ptr, size_t size, size_t count, std::string* data)
    {
        data->append((char*)ptr, size * count);
        return size * count;
    }

    std::string GetRemoteImage(const std::string& url);
}

namespace File {
    inline std::string LoadFileContent(const std::string& filePath)
    {
        // C++ way of reading file, on the basis of how to do it with the C API (could be a little bit quicker)
        //TODO: refactor with this UTF8 way of opening/accessing files everywhere in the project (and like i should just ALWAYS use it when i'm handling files)
        const std::filesystem::path utf8Path(std::u8string(filePath.begin(), filePath.end()));
        const std::u8string absolutePath = std::filesystem::absolute(utf8Path).u8string();
        std::cout << "File path: " << std::string(absolutePath.begin(), absolutePath.end()) << std::endl;

        std::ifstream stream(utf8Path, std::ios::binary);
        if (!stream) throw std::runtime_error("Failed to open file.");

        stream.seekg(0, std::ios::end);
        std::streampos size = stream.tellg();
        if (size < 0) throw std::runtime_error("File size is invalid: " + std::to_string(size));

        std::string contents;
        contents.resize(size);
        stream.seekg(0, std::ios::beg);
        stream.read(&contents[0], size);
        if (!stream) throw std::runtime_error("Failed to read file.");

        stream.close();
        return contents;
    }
}

//TODO: I don't know how to name it better neither what a name a file that contains them could be so it's here for now
namespace Common
{
    class HasPosition
    {
    protected:
        glm::vec3 m_Position = glm::vec3(0.0f);

    public:
        virtual ~HasPosition() = default;

        void SetPosition(const glm::vec3& position)
        {
            m_Position = position;
            OnPositionUpdate(position);
        }
        const glm::vec3& GetPosition() const { return m_Position; }

    protected:

        virtual void OnPositionUpdate(const glm::vec3& /*position*/) {}
    };

    class HasDirection
    {
    protected:
        glm::vec3 m_Direction = glm::vec3(0.0f);

    public:
        virtual ~HasDirection() = default;

        void SetDirection(const glm::vec3& direction)
        {
            m_Direction = direction;
            OnDirectionUpdate(direction);
        }
        const glm::vec3& GetDirection() const { return m_Direction; }

    protected:

        virtual void OnDirectionUpdate(const glm::vec3& /*direction*/) {}
    };
}