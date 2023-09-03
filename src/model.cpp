#include "model.hpp"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum dataFormat;
        GLenum internalFormat;
        if (nrComponents == 1)
        {
            dataFormat = GL_RED;
            internalFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            dataFormat = GL_RGB;
            internalFormat = gamma ? GL_SRGB : GL_RGB;
        }
        else if (nrComponents == 4)
        {
            dataFormat = GL_RGBA;
            internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
        }
        else
        {
            std::cout << "ERROR | MODEL | TEXTURE: Unknown format.\n" << path << std::endl;
            stbi_image_free(data);
            // TO - DO: End function here, don't continue
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERROR | MODEL | TEXTURE: Failed to load texture at: " << path << "\n" << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::Draw(Camera& camera, Shader& shader, DirectionalLight* directionalLight, std::vector<Light*> pointLights, InputManager* inputManager)
{
    shader.Use();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 view = camera.GetViewMatrix();
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);
    // <TEMP>
    // TO - DO: MOVE THIS SECTION SOMEWHERE ELSE
    // This does not belong here silly boy
    float t = static_cast<float>(glfwGetTime());
    // Check for model rotation:
    if (inputManager->keyboard.GetKeyState(GLFW_KEY_UP))
        m_model = glm::rotate(m_model, 0.0025f, glm::vec3(1.0f, 0.0f, 0.0f));
    if (inputManager->keyboard.GetKeyState(GLFW_KEY_DOWN))
        m_model = glm::rotate(m_model, 0.0025f, glm::vec3(-1.0f, 0.0f, 0.0f));
    if (inputManager->keyboard.GetKeyState(GLFW_KEY_LEFT))
        m_model = glm::rotate(m_model, 0.0025f, glm::vec3(0.0f, -1.0f, 0.0f));
    if (inputManager->keyboard.GetKeyState(GLFW_KEY_RIGHT))
        m_model = glm::rotate(m_model, 0.0025f, glm::vec3(0.0f, 1.0f, 0.0f));
    // Set the model
    shader.SetMat4("model", m_model);
    // Toggle the lighting model
    if (inputManager->keyboard.GetKeyState(GLFW_KEY_1))
    {
        if (!shader.blinnToggle)
        {
            shader.blinn = !shader.blinn;
            shader.blinnToggle = true;
        }
    }
    else
    {
        shader.blinnToggle = false;
    }
    // Toggle the gamma correction
    if (inputManager->keyboard.GetKeyState(GLFW_KEY_2))
    {
        if (!shader.gammaToggle)
        {
            shader.gamma = !shader.gamma;
            shader.gammaToggle = true;
        }
    }
    else
    {
        shader.gammaToggle = false;
    }
    // </TEMP>
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader, *directionalLight, pointLights);
}

glm::mat4 Model::GetModel()
{
    return m_model;
}

void Model::SetModel(glm::mat4 newModel)
{
    m_model = newModel;
}

void Model::LoadModel(std::string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR | ASSIMP | " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }

}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<MeshTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "material.diffuse", true);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<MeshTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "material.specular", false);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<MeshTexture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "material.normal", false);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<MeshTexture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", false);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    // 5. shininess
    float shininess = 0.0f;
    aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures, shininess);
}

std::vector<MeshTexture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, bool gamma)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size(); j++)
        {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            MeshTexture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory, gamma);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}