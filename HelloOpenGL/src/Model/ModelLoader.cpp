#include "ModelLoader.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* Mesh class */
Mesh::Mesh(std::vector<MeshVertex>& vertices, std::vector<unsigned int>& indices, std::vector<MeshTexture>& textures)
	: m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), m_Textures(std::move(textures))
{
	SetupGLData();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_Vao);
	glDeleteBuffers(1, &m_Vbo);
	glDeleteBuffers(1, &m_Ibo);
	std::cout << "mesh destructor" << std::to_string(m_Vao) << "/" << std::to_string(m_Vbo) << "/" << std::to_string(m_Ibo) << "/" << std::endl;
}

Mesh::Mesh(const Mesh& other)
{
	m_Vertices = std::move(other.m_Vertices);
	m_Indices = std::move(other.m_Indices);
	m_Textures = std::move(other.m_Textures);
	m_Vao = other.m_Vao;
	m_Vbo = other.m_Vbo;
	m_Ibo = other.m_Ibo;
	std::cout << "mesh copied" << std::endl;
}

Mesh::Mesh(Mesh&& other) noexcept
{
	m_Vertices = std::move(other.m_Vertices);
	m_Indices = std::move(other.m_Indices);
	m_Textures = std::move(other.m_Textures);
	m_Vao = other.m_Vao;
	m_Vbo = other.m_Vbo;
	m_Ibo = other.m_Ibo;
	other.m_Vao = 0;
	other.m_Vbo = 0;
	other.m_Ibo = 0;
	std::cout << "mesh moved" << std::endl;
}

void Mesh::Draw(Shader& shader)
{
	//TODO: proper textures loading (if there are several ones for one mesh) meaning sampler2D[] buffer and idk how I should handle it in the shader but well
	bool diffuseSet = 0, specularSet = 0;
	for (int i = 0; i < m_Textures.size(); i++)
	{
		MeshTexture& texture = m_Textures[i]; //maybe using ref will destruct the mesh when leaving the for scope
		if (texture.type == DIFFUSE)
		{
			if (!diffuseSet)
			{
				shader.SetUniform1i("u_Material.diffuseMap", texture.texIndex);
				diffuseSet = 1;
			}
			else std::cout << "Diffuse already set" << std::endl;
		}
		if (texture.type == SPECULAR) shader.SetUniform1i("u_Material.specularMap", texture.texIndex);
	}
	shader.SetUniform1f("u_Material.shininess", 0.5f); //with shininess map
	glBindVertexArray(m_Vao);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//private
void Mesh::SetupGLData()
{
	//TODO: my current VertexBuffer implementation is tied to "float* vertices" data so it's handmade here! (+it allows me to review opengl basis)
	glGenVertexArrays(1, &m_Vao);
	glGenBuffers(1, &m_Vbo);
	glGenBuffers(1, &m_Ibo);

	glBindVertexArray(m_Vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(MeshVertex), m_Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) offsetof(MeshVertex, normal)); // 3*sizeof(float)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) offsetof(MeshVertex, textureUV)); // 3*sizeof(float)+3*sizeof(float)=6*sizeof(float)

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


/* Model class */
Model::Model(const char* filePath, bool flipUVOnLoad)
	: m_FilePath(filePath), m_FlipUVOnLoad(flipUVOnLoad)
{
	m_AssetPath = m_FilePath.substr(0, m_FilePath.find_last_of("/")); //TODO: perhaps a better way exists
	Load();
}

Model::~Model()
{
	std::cout << "model destructor" << std::endl;
}

void Model::Draw(Shader& shader)
{
	for (int i = 0; i < m_Textures.size(); i++) m_Textures[i].Bind(i);
	for (int i = 0; i < m_Meshes.size(); i++) m_Meshes[i].Draw(shader);
	for (int i = 0; i < m_Textures.size(); i++) m_Textures[i].Unbind();
}

//private
void Model::Load()
{
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_EMBEDDED_TEXTURES_LEGACY_NAMING, 1);
	unsigned int importFlags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
	if (m_FlipUVOnLoad) importFlags |= aiProcess_FlipUVs;
	const aiScene* scene = importer.ReadFile(m_FilePath, importFlags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR: Assimp could not load model " << m_FilePath << ": " << importer.GetErrorString() << std::endl;
		return;
	}
	m_Meshes.reserve(scene->mNumMeshes);
	m_Textures.reserve(scene->mNumMaterials * 2); // *2 because currently asking for both DIFFUSE & SPECULAR textures (considering only 1 texture per texture type per mesh)
	m_CachedTextures.reserve(scene->mNumMaterials * 2);
	if(scene->mNumTextures>0) LoadEmbeddedTextures(scene);
	ProcessNode(scene->mRootNode, scene);
	m_CachedTextures.clear();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, m_Meshes);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh>& outMeshes)
{
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;
	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces);
	textures.reserve(scene->mNumMaterials * 2);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex vertex;
		//TODO: PARENTNODETRANSFORMWITHMAT4
		vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		if (mesh->HasNormals()) vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		//NOTE: what are the other texture coords (layer 1/2/3) for?
		if (mesh->mTextureCoords[0]) vertex.textureUV = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		else vertex.textureUV = { 0.0f, 0.0f };
		vertices.emplace_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) indices.emplace_back(face.mIndices[j]);
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		LoadMaterialTextures(mat, aiTextureType_DIFFUSE, DIFFUSE, textures);
		LoadMaterialTextures(mat, aiTextureType_SPECULAR, SPECULAR, textures);
	}
	if (textures.size() == 0) 
		std::cout << "uh" << std::endl;
	outMeshes.emplace_back(vertices, indices, textures);
}

void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType, std::vector<MeshTexture>& outTextures)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString texturePath;
		mat->GetTexture(type, i, &texturePath);
		std::string textureAssetPath = m_AssetPath + "/" + texturePath.C_Str();
		if (auto x = m_CachedTextures.find(textureAssetPath); x != m_CachedTextures.end())
		{
			outTextures.emplace_back(x->second, textureType);
			continue;
		}
		m_Textures.emplace_back(textureAssetPath);
		m_CachedTextures.emplace(textureAssetPath, m_Textures.size() - 1); // in multithreaded content should have an atomic integer GetNextIndex() or stg like that ig
		outTextures.emplace_back(m_Textures.size() - 1, textureType);
		std::cout << "External texture loaded: " << textureAssetPath << std::endl;
	}
}

void Model::LoadEmbeddedTextures(const aiScene* scene)
{
	for (int i = 0; i < scene->mNumTextures; i++)
	{
		aiTexture* texture = scene->mTextures[i];

		unsigned char* textureData = reinterpret_cast<unsigned char*>(texture->pcData);
		int texSize = texture->mHeight == 0 ? texture->mWidth : texture->mWidth * texture->mHeight;

		m_Textures.emplace_back(textureData, texSize);
		m_CachedTextures.emplace(m_AssetPath + "/*" + std::to_string(i), m_Textures.size() - 1);
		std::cout << "Embedded texture loaded: *" << std::to_string(i) << std::endl;
	}
}