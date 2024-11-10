// Texture.cpp

#include "Texture.h"
#include "stb_image.h"

Texture::Texture(GLuint id, const std::string& path) : id(id), path(path) {
    // Usa stb_image para obtener la medida de la textura sin cargarla en memoria
    int channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data) {
        stbi_image_free(data);  // Liberar la imagen después de obtener las dimensiones
    } else {
        width = height = 0;  // Si falla la carga, asigna 0
    }
}

Texture::~Texture() {
    if (id) {
        glDeleteTextures(1, &id);
    }
}

int Texture::getWidth() const { return width; }
int Texture::getHeight() const { return height; }
const std::string& Texture::getPath() const { return path; }

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
