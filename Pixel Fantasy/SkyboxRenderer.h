#pragma once

#include "Shader.h"
#include "stb_image.h"

#include <vector>

class SkyboxRenderer
{
	public:
		SkyboxRenderer();
		void render();
		unsigned int loadCubemap();

	private:
		Shader m_shader;
		unsigned int m_textureID;
	
};

