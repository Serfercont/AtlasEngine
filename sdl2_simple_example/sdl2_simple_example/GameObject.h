//#ifndef GAMEOBJECT_H
//#define GAMEOBJECT_H
//
//#include <vector>
//#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <string>
//
//struct Transform {
//	glm::vec3 position;
//	glm::vec3 rotation;
//	glm::vec3 scale;
//
//	Transform();
//};
//
//struct Mesh {
//	std::vector<GLfloat> vertices;
//	std::vector<GLuint> indices;
//	GLuint VAO, VBO, EBO;
//
//	Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
//	void setupMesh();
//};
//
//struct Texture {
//	GLuint id;
//	std::string filepath;
//
//	Texture(const std::string& path);
//	void loadTexture();
//};
//
//class GameObject {
//public:
//	Transform transform;
//	Mesh* mesh;
//	Texture* texture;
//
//	GameObject(Mesh* mesh, Texture* texture);
//	void render();
//};
//
//#endif // !GAMEOBJECT_H
