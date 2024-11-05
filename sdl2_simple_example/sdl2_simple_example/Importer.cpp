#include "Importer.h"
#include "stb_image.h"
#include <iostream>

GLuint Importer::loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* imageData = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
    if (imageData == nullptr) {
        std::cerr << "Error: No se pudo cargar la textura " << path << std::endl;
        return 0;
    }
    GLuint textID;
    glGenTextures(1, &textID);
    glBindTexture(GL_TEXTURE_2D, textID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);
    return textID;
}

void Importer::loadFBX(const std::string& filePath) {
    const aiScene* scene = aiImportFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        std::cerr << "Error en cargar el archivo: " << filePath << "\n" << aiGetErrorString() << std::endl;
        return;
    }

    meshes.clear();
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* aimesh = scene->mMeshes[i];
        Mesh mesh;

        for (unsigned int v = 0; v < aimesh->mNumVertices; v++) {
            mesh.vertices.push_back(aimesh->mVertices[v].x);
            mesh.vertices.push_back(aimesh->mVertices[v].y);
            mesh.vertices.push_back(aimesh->mVertices[v].z);

            if (aimesh->HasTextureCoords(0)) {
                mesh.uvCoords.push_back(aimesh->mTextureCoords[0][v].x);
                mesh.uvCoords.push_back(aimesh->mTextureCoords[0][v].y);
            }
        }

        for (unsigned int f = 0; f < aimesh->mNumFaces; f++) {
            const aiFace& face = aimesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.indices.push_back(face.mIndices[j]);
            }
        }
        meshes.push_back(mesh);
    }
}

const std::vector<Mesh>& Importer::getMeshes() const {
    return meshes;
}
