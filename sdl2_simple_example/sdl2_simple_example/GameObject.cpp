//#include "GameObject.h"
//#include <GL/glew.h>
//#include <stb_image.h>
//
//Transform::Transform() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) {}
//
//Mesh::Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices) : vertices(vertices), indices(indices) {}
//
//void Mesh::setupMesh() {
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
//
//    // Atributo de posición del vértice
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindVertexArray(0);
//}
//
//Texture::Texture(const std::string& path) : filepath(path), id(0){}
//
//void Texture::loadTexture() {
//    glGenTextures(1, &id);
//    glBindTexture(GL_TEXTURE_2D, id);
//
//    int width, height, nrChannels;
//    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
//    if (data) {
//		GLenum format = nrChannels == 4 ? GL_RGB : GL_RGBA;
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//    else {
//		printf("Failed to load texture\n");
//    }
//	stbi_image_free(data);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//}
//
//GameObject::GameObject(Mesh* mesh, Texture* texture) : mesh(mesh), texture(texture) {}
//
//void GameObject::render() {
//    if (texture)
//    {
//        glBindTexture(GL_TEXTURE_2D, texture->id);
//    }if (mesh)
//    {
//        glBindVertexArray(mesh->VAO);
//		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//    }
//}