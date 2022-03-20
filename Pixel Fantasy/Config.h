#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

struct Config {
    int m_windowWidth = 1280;
    int m_windowHeight = 720;
    bool m_isFullscreen = false;
    int m_renderDistance = 16;
    int m_fov = 90;
};

#endif // CONFIG_H_INCLUDED
