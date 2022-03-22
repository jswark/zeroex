#pragma once

#include <iostream>

#include "shaderSystem/shaders.h"
#include "render/graphicObject.h"

#include <glm/glm.hpp>

class Render : public GraphicObject {
public:
    Render(const std::string& vs, const std::string& fs) : vShaderPath(vs), fShaderPath(fs) {}

	void render() override;
	bool init() override;

	~Render() {
		cleanup();
	}

	glm::mat4 view = glm::mat4(0);

private:
	const std::string vShaderPath;
	const std::string fShaderPath;
	std::unique_ptr<ShaderProgram> shaderProg;

	gl::GLuint VBO, containerVAO;

	void cleanup();
};
