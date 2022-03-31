#include "Application.h"
#include "stb_image.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int MAP_SIZE = 9;
const float BLOCK_SIZE = 1.0f;

Camera camera(glm::vec3(-8.0f, 2.0f, 0.0f));

auto deltaTime = sf::seconds(0);	// time between current frame and last frame
auto lastFrame = sf::seconds(0);

Application::Application(const Config& config)
    : m_context(config)
    , m_drag(false)
    , m_config(config)
    , m_skyboxRenderer()
    , m_characterRenderer()
{
}

float g_timeElapsed = 0;

void Application::runLoop()
{

    // camera
    float lastX = m_config.m_windowWidth / 2.0f;
    float lastY = m_config.m_windowHeight / 2.0f;
    bool firstMouse = true;

    // timing
    sf::Clock clock; // starts the clock

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    Shader ourShader("../Shaders/vertexShader.vs", "../Shaders/fragmentShader.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // World space positions of our cubes. It has nothing to do with our program, so far.
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.10f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("../Resources/Textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("../Resources/Textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


    while (m_context.window.isOpen()) {
        // per-frame time logic
        // --------------------
        auto currentFrame = clock.getElapsedTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleEvents();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
       
        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) camera.m_windowWidth / (float) camera.m_windowHeight, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        


         //render boxes
        glBindVertexArray(VAO);
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                model = glm::translate(model, glm::vec3(static_cast<float>(i - MAP_SIZE / 2), -2.0f, static_cast<float>(j - MAP_SIZE / 2)));
                /*float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));*/
                ourShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }


        // The skybox needs to be rendered before character in order for the character's background color to be skybox
        m_skyboxRenderer.render(camera);
       

            std::vector<int> pos{ 0, 8, int(currentFrame.asSeconds() * 50) };
            m_characterRenderer.render(camera, pos);
        

        m_context.window.display();
    }

    

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    /*glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);*/
        m_context.window.display();
}


void Application::handleEvents()
{
    sf::Event e;
    while (m_context.window.pollEvent(e)) {
        switch (e.type) {
        case sf::Event::Closed:
            m_context.window.close();
            break;
        case sf::Event::KeyPressed:
            handleKeyBoardEvents(e.key.code);
            break;
        case sf::Event::MouseWheelScrolled:
            camera.ProcessMouseScroll(e.mouseWheelScroll.delta);
            break;
        case sf::Event::MouseButtonPressed:
            switch (e.mouseButton.button) {
                case (sf::Mouse::Middle):
                    m_drag = true;
                    m_lastX = e.mouseButton.x;
                    m_lastY = e.mouseButton.y;
                    /*std::cout << "Middle pressed! ";
                    std::cout << "last X:" << m_lastX << " last Y:" << m_lastY << std::endl;*/
                    break;
            }
            break;
        case sf::Event::MouseMoved:
            if (m_drag) {
                float xoffset = e.mouseMove.x - m_lastX;
                float yoffset = e.mouseMove.y - m_lastY;
                /*std::cout << "Mouse moved! ";
                std::cout << "xoffset:" << xoffset << " y offset:" << yoffset << std::endl;*/

                m_lastX = e.mouseMove.x;
                m_lastY = e.mouseMove.y;

                camera.ProcessMouseMovement(xoffset, yoffset);
            }
            break;
        case sf::Event::MouseButtonReleased:
            switch (e.mouseButton.button) {
                case sf::Mouse::Middle:
                    m_drag = false;
                    /*std::cout << "Middle released!" << std::endl;*/
                    break;
            }
            break;
        //case sf::Event::Resized:
        //    glViewport(0, 0, e.size.width, e.size.height);
        //    break;

        default:
            break;
        }
    }
}

void Application::handleKeyBoardEvents(sf::Keyboard::Key e) {
    switch (e) {
    case sf::Keyboard::Escape:
        m_context.window.close();
        break;
    case sf::Keyboard::W:
        camera.ProcessKeyboard(FORWARD, deltaTime.asSeconds());
        break;
    case sf::Keyboard::S:
        camera.ProcessKeyboard(BACKWARD, deltaTime.asSeconds());
        break;
    case sf::Keyboard::A:
        camera.ProcessKeyboard(LEFT, deltaTime.asSeconds());
        break;
    case sf::Keyboard::D:
        camera.ProcessKeyboard(RIGHT, deltaTime.asSeconds());
        break;
    default:
        break;
    }
}

void Application::popState()
{
    /* m_isPopState = true;*/
}

void Application::turnOffMouse()
{
    m_context.window.setMouseCursorVisible(false);
}

void Application::turnOnMouse()
{
    m_context.window.setMouseCursorVisible(true);
}





unsigned int Application::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}