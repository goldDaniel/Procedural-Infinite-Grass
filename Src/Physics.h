#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>

class PhysicsSim
{
public:
    PhysicsSim()
    {
        collisionConfig = new btDefaultCollisionConfiguration();
        collisionDispatcher = new btCollisionDispatcher(collisionConfig);
        overlappingCache = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver;

        dynamicWorld = new btDiscreteDynamicsWorld(collisionDispatcher, 
                                                   overlappingCache, 
                                                   solver, 
                                                   collisionConfig);

        dynamicWorld->setGravity(btVector3(0, -10, 0));
    }

    ~PhysicsSim()
    {
        delete dynamicWorld;
        delete solver;
        delete overlappingCache;
        delete collisionDispatcher;
        delete collisionConfig;
    }


private:
    
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatcher;
    btBroadphaseInterface* overlappingCache; 
    btSequentialImpulseConstraintSolver* solver;

    btDiscreteDynamicsWorld* dynamicWorld;
};

#endif