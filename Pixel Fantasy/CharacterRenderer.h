#pragma once
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"

#include <vector>

class CharacterRenderer
{
public:
	CharacterRenderer();
	void render(Camera& camera, std::vector<int> characterPosition);
	unsigned int loadID();

private:
	Shader m_shader;
	unsigned int m_textureID;
	unsigned int m_characterVAO;
	unsigned int m_characterVBO;
};
