#include "Context.h"

#include <glad/glad.h>


unsigned int g_X;
unsigned int g_Y;

Context::Context(const Config& config)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    // settings.attributeFlags = sf::ContextSettings::Core;
    // This is no longer necessary due to the Mac Support update.

    if (config.m_isFullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "Pixel Fantasy",
            sf::Style::Fullscreen, settings);
    }
    else {
        sf::VideoMode winMode(config.m_windowWidth, config.m_windowHeight);
        window.create(winMode, "Pixel Fantasy", sf::Style::Close, settings);
    }

    if (!gladLoadGL()) {

        exit(-1);
    }


    window.setFramerateLimit(60);

    glViewport(0, 0, window.getSize().x, window.getSize().y);

    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}