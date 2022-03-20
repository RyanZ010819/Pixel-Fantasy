#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <glm/glm.hpp>

struct Entity {
    Entity()
        : position(glm::vec3(0.f))
        , rotation(glm::vec3(0.f))
        , velocity(glm::vec3(0.f))
    {
    }

    Entity(const glm::vec3& pos, const glm::vec3& rot)
        : position(pos)
        , rotation(rot)
        , velocity(glm::vec3(0.f))
    {
    }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 velocity;
};

#endif // ENTITY_H_INCLUDED