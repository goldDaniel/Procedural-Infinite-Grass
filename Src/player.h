#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "shapeRenderer.h"

class Player
{

private:
    
    ShapeRenderer* sh;

public:

    glm::mat4 transform = glm::mat4(1.f);

    Player()
    {
        sh = new ShapeRenderer();
        transform = glm::translate(transform, glm::vec3(0, 256,0));
    }

    ~Player()
    {
        delete sh;
    }

    void update(float dt)
    {
        transform *= glm::rotate(glm::pi<float>() * dt, glm::vec3(1, 1, 1));
    }

    void draw(glm::mat4 view, glm::mat4 proj)
    {
        sh->setProjectionMatrix(proj);
        sh->begin(view * transform);

        glm::vec3 min(-5, -5, -5);
        glm::vec3 max(5, 5, 5);

        sh->box(min, max);

        sh->end();
    }
};

#endif