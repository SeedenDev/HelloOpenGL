#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <assimp/scene.h>

#include "Texture.h"
#include "Shader.h"

struct MeshVertex
{
	glm::vec3 position, normal;
	glm::vec2 textureUV;
};

enum TextureType
{
	DIFFUSE, SPECULAR
};

struct MeshTexture
{
	unsigned int texIndex; //TODO: reference to the texture stored in an asset system with all the loaded textures so no duplicatedif loading a model twice.
	TextureType type;

	MeshTexture(unsigned int texIndex, TextureType t)
		: texIndex(texIndex), type(t)
	{
		std::cout << "MeshTexture constructor" << std::endl;
	}

	~MeshTexture()
	{
		std::cout << "MeshTexture destructor" << std::endl;
	}

	MeshTexture(const MeshTexture& other)
		: texIndex(other.texIndex), type(other.type)
	{
		std::cout << "MeshTexture copied" << std::endl;
	}

	MeshTexture(MeshTexture&& other) noexcept
		: texIndex(other.texIndex), type(other.type)
	{
		std::cout << "MeshTexture moved" << std::endl;
	}

	MeshTexture& operator=(MeshTexture&& other) noexcept
	{
		std::cout << "MeshTexture moved with op=&&" << std::endl;
		if (this != &other)
		{
			texIndex = other.texIndex;
			type = other.type;
			std::cout << "effectively moved" << std::endl;
		}
		return *this;
	}
	MeshTexture& operator=(MeshTexture& other) noexcept
	{
		std::cout << "MeshTexture moved with op=&" << std::endl;
		if (this != &other)
		{
			texIndex = other.texIndex;
			type = other.type;
			std::cout << "effectively moved" << std::endl;
		}
		return *this;
	}
};

class Mesh
{
private:
	std::vector<MeshVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<MeshTexture> m_Textures;

	unsigned int m_Vao, m_Vbo, m_Ibo;

public:
	Mesh(std::vector<MeshVertex>& vertices, std::vector<unsigned int>& indices, std::vector<MeshTexture>& textures);
	~Mesh();
	Mesh(const Mesh& other);
	Mesh(Mesh&& other) noexcept;

	void Draw(Shader& shader);

private:
	void SetupGLData();
};

class Model
{
private:
	const std::string m_FilePath;
	const bool m_FlipUVOnLoad;
	std::string m_AssetPath;
	std::vector<Mesh> m_Meshes;
	std::vector<Texture> m_Textures;
	std::unordered_map<std::string, unsigned int> m_CachedTextures;

public:
	Model(const char* path, bool flipUVOnLoad=1);
	~Model();

	void Draw(Shader& shader);

private:
	void Load();
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh>& outMeshes);
	//TODO: better material loading no textures (=just colors), and all the other texture types and mat properties (ambient, reflective, shininess)
	//tldr: make a new material system
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType, std::vector<MeshTexture>& outTextures);
	void LoadEmbeddedTextures(const aiScene*);
};