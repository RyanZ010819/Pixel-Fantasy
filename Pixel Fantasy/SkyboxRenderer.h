#pragma once

#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"

#include <vector>

class SkyboxRenderer
{
	public:
		SkyboxRenderer();
		void render(Camera& camera);
		unsigned int loadCubemap();

	private:
		Shader m_shader;
		unsigned int m_textureID;
		unsigned int m_skyboxVAO;
		unsigned int m_skyboxVBO;
	
};

