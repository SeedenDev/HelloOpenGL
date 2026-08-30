#include "ModelLoader.h"

#include <glad/glad.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* Mesh class */
Mesh::Mesh(std::vector<MeshVertex>& vertices, std::vector<unsigned int>& indices, Material& material)
	: m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), m_Material(material)
{
	SetupGLData();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_Vao);
	glDeleteBuffers(1, &m_Vbo);
	glDeleteBuffers(1, &m_Ibo);
	//std::cout << "mesh destructor" << std::to_string(m_Vao) << "/" << std::to_string(m_Vbo) << "/" << std::to_string(m_Ibo) << "/" << std::endl;
}

void Mesh::Draw(Shader& shader)
{
	m_Material.BindTo(shader);
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
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, textureUV)); // 3*sizeof(float)+3*sizeof(float)=6*sizeof(float)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) offsetof(MeshVertex, normal)); // 3*sizeof(float)
	
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
	//std::cout << "model destructor" << std::endl;
}

void Model::Draw(Shader& shader)
{
	for (int i = 0; i < m_Meshes.size(); i++)
	{
		Mesh& mesh = m_Meshes[i];
		const Material& mat = mesh.GetMaterial();
		// Binding textures here to avoid to pass Model object into the Mesh object (waiting for asset system perhaps in a real engine)
		if (mat.HasDiffuseTexture())
		{
			m_Textures[mat.GetDiffuseTexture()].Bind(20);
			shader.SetUniform1i("u_Material.diffuseMap", 20);
		}
		if (mat.HasSpecularTexture())
		{
			m_Textures[mat.GetSpecularTexture()].Bind(21);
			shader.SetUniform1i("u_Material.specularMap", 21);
		}
		if (mat.HasEmissiveTexture())
		{
			m_Textures[mat.GetEmissiveTexture()].Bind(22);
			shader.SetUniform1i("u_Material.emissiveMap", 22);
		}
		mesh.Draw(shader);
	}
}

//private
void Model::Load()
{
	std::cout << "Loading model " << m_AssetPath << std::endl;
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
	ProcessNode(scene->mRootNode, scene, glm::mat4(1));
	m_CachedTextures.clear();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform)
{
	aiMatrix4x4 mat = node->mTransformation;
	const glm::mat4 nodeTransform = glm::mat4(
		mat.a1, mat.a2, mat.a3, mat.a4,
		mat.b1, mat.b2, mat.b3, mat.b4,
		mat.c1, mat.c2, mat.c3, mat.c4,
		mat.d1, mat.d2, mat.d3, mat.d4
	) * parentTransform;

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, nodeTransform, m_Meshes);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, nodeTransform);
	}
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& nodeTransform, std::vector<Mesh>& outMeshes)
{
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;
	Material material;
	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex vertex;
		vertex.position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f) * nodeTransform;
		if (mesh->HasNormals()) vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) * glm::mat3(nodeTransform);
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
		//TODO: smth to cache materials and so in Mesh no material stored, only reference to an existing one (avoid duplicated item+batch rendering of every mesh using the same mat in the future)
		// => well, jus the matIndex lmao
		material.SetDiffuseTexture(GetMaterialTextureIndex(mat, aiTextureType_DIFFUSE));
		material.SetSpecularTexture(GetMaterialTextureIndex(mat, aiTextureType_SPECULAR));
		material.SetEmissiveTexture(GetMaterialTextureIndex(mat, aiTextureType_EMISSIVE));
		LoadMaterialProperties(mat, material);
	}
	outMeshes.emplace_back(vertices, indices, material);
}

int Model::GetMaterialTextureIndex(aiMaterial* mat, aiTextureType type)
{
	//std::cout << "Loading texture type " << std::to_string(type) << ": found " << std::to_string(mat->GetTextureCount(type)) << std::endl;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString texturePath;
		mat->GetTexture(type, i, &texturePath);
		std::string textureAssetPath = m_AssetPath + "/" + texturePath.C_Str();
		if (auto x = m_CachedTextures.find(textureAssetPath); x != m_CachedTextures.end())
		{
			return x->second;
		}
		m_Textures.emplace_back(textureAssetPath);
		m_CachedTextures.emplace(textureAssetPath, m_Textures.size() - 1); // in multithreaded content should have an atomic integer GetNextIndex() or smth like that ig
		//std::cout << "External texture loaded: " << textureAssetPath << std::endl;
		return m_Textures.size() - 1;
	}
	return -1;
}

void Model::LoadMaterialProperties(aiMaterial* mat, Material& outMaterial)
{
	aiColor3D color(0.f, 0.f, 0.f);
	float value = 0.0f;

	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	outMaterial.SetAmbientColor(glm::vec3(color.r, color.b, color.g));

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	outMaterial.SetDiffuseColor(glm::vec3(color.r, color.b, color.g));

	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	outMaterial.SetSpecularColor(glm::vec3(color.r, color.b, color.g));

	mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
	outMaterial.SetEmissiveColor(glm::vec3(color.r, color.b, color.g));

	mat->Get(AI_MATKEY_SHININESS, value);
	outMaterial.SetSpecularShininess(value);

	if (AI_SUCCESS != mat->Get(AI_MATKEY_SHININESS_STRENGTH, value)) value = 1.0f;
	outMaterial.SetSpecularStrength(value);
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
		//std::cout << "Embedded texture loaded: *" << std::to_string(i) << std::endl;
	}
}