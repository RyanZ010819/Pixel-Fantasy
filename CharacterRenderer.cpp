#include "CharacterRenderer.h"

unsigned int loadTexture();

const int MAP_SIZE = 9;

CharacterRenderer::CharacterRenderer() : m_shader("../Shaders/character.vs", "../Shaders/character.fs") {

    float characterVertices[] = {
        // positions          // texture Coords
        

        -0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
        -0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
        -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
        -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
        -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
        -0.3f,  0.3f,  0.3f,  1.0f, 0.0f,

        
    };

    // character VAO, VBO
    glGenVertexArrays(1, &m_characterVAO);
    glGenBuffers(1, &m_characterVBO);

    glBindVertexArray(m_characterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_characterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(characterVertices), characterVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // load textures
    // -------------
    m_textureID = loadTexture();

    // shader configuration
    // --------------------
    m_shader.use();
    m_shader.setInt("character", 0);
}

void CharacterRenderer::render(Camera& camera, std::vector<int> characterPosition) {

    m_shader.use();

    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(static_cast<float>(characterPosition[0] - MAP_SIZE / 2), -1.0f, static_cast<float>(characterPosition[1] - MAP_SIZE / 2)));
    model = glm::rotate(model, glm::radians(float(characterPosition[2])), glm::vec3(1,0,0));
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    m_shader.setMat4("model", model);
    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);
    

    // Draw character
    glBindVertexArray(m_characterVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 4);
    glBindVertexArray(0);
}

unsigned int loadTexture()
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char* data = stbi_load("../Resources/Textures/Head.png", &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << "../Resources/Textures/Head.png" << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}