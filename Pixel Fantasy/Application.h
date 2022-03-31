#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <memory>
#include <vector>

#include "Context.h"
#include "Shader.h"
#include "SkyboxRenderer.h"
#include "CharacterRenderer.h"
#include <SFML/Window/Event.hpp>
#include <glad/glad.h>
#include <iostream>

class Application {

public:
    Application(const Config& config);

    void runLoop();

    void popState();


    const sf::Window& getWindow() const
    {
        return m_context.window;
    }

    void turnOffMouse();
    void turnOnMouse();

private:
    void handleEvents();
    void handleKeyBoardEvents(sf::Keyboard::Key e);
    unsigned int loadCubemap(std::vector<std::string> faces);

    Context m_context;
    bool m_drag;
    float m_lastX, m_lastY;
    SkyboxRenderer m_skyboxRenderer;
    CharacterRenderer m_characterRenderer;
    const Config& m_config;
};

#endif // APPLICATION_H_INCLUDED