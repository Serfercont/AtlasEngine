#pragma once
#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture(GLuint id, const std::string& path);
    ~Texture();

    void bind() const;
    void unbind() const;

    int getWidth() const;
    int getHeight() const;
    const std::string& getPath() const;
    GLuint getTextureID() const { return id; }

private:
    GLuint id;
    std::string path; // Ruta del archivo de la textura
    int width;        // Ancho de la textura
    int height;       // Alto de la textura

};