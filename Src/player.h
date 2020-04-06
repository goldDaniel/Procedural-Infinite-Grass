#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "model.h"
#include "utils.h"

class Player
{
private:

    btRigidBody* body;

    float radius;
    Model* model;

public: 
    Player()
    {
        model = new Model("Assets/sphere.obj");
    }

    ~Player()
    {
        delete model;
    }

    void applyForce(glm::vec3 force)
    {
        body->activate();
        body->applyCentralForce(btVector3(force.x, force.y, force.z));
    }

    Model* getModel()
    {
        return model;   
    }

    glm::mat4 getTransform()
    {
        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);
        glm::mat4 transform =  bulletToGlm(trans);

        return transform;
    }

    glm::vec3 getPosition()
    {
        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);

        return glm::vec3(trans.getOrigin().getX(),
                         trans.getOrigin().getY(),
                         trans.getOrigin().getZ());
    }

    void setRigidBody(btRigidBody* rb)
    {
        body = rb;

    }
};

#endif