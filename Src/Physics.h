#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>

#include "terrainChunk.h"

#include "shapeRenderer.h"

class PhysicsSim
{
private:
    
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatcher;
    btBroadphaseInterface* overlappingCache; 
    btSequentialImpulseConstraintSolver* solver;

    btDiscreteDynamicsWorld* dynamicWorld;
    
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    ShapeRenderer* sh;

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

        dynamicWorld->setGravity(btVector3(0, -4, 0));



        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(8.f));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(5.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
		{	
            colShape->calculateLocalInertia(mass,localInertia);
        }

        startTransform.setOrigin(btVector3(0,256,0));
    
        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicWorld->addRigidBody(body);

        sh = new ShapeRenderer();
    }

    ~PhysicsSim()
    {
        delete dynamicWorld;
        delete solver;
        delete overlappingCache;
        delete collisionDispatcher;
        delete collisionConfig;
    }
    

    void createTerrainCollisionShapes(std::vector<TerrainChunk*> chunks)
    {
        for(TerrainChunk* chunk : chunks)
        {
            btTriangleIndexVertexArray* idxVertArr = 
                    new btTriangleIndexVertexArray(chunk->getNumIndices() / 3,
                                                   chunk->getIndexBuffer(),
                                                   sizeof(int) * 3,
                                                   chunk->getNumVertices(),
                                                   (btScalar*)&chunk->getPositionBuffer()[0],
                                                   sizeof(glm::vec3));
            btVector3 min(chunk->getWorldMin().x,
                          chunk->getWorldMin().y,
                          chunk->getWorldMin().z);
            btVector3 max(chunk->getWorldMax().x,
                          chunk->getWorldMax().y,
                          chunk->getWorldMax().z);

            btCollisionShape* trimeshShape  = 
                    new btBvhTriangleMeshShape(idxVertArr,true,min,max);
	        
            collisionShapes.push_back(trimeshShape);


           

            btTransform startTransform;
            startTransform.setIdentity();

             //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
            btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(0.f,myMotionState,trimeshShape,btVector3(0,0,0));
            btRigidBody* body = new btRigidBody(rbInfo);

            body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

            dynamicWorld->addRigidBody(body);
        }
    }

    void step()
    {
        dynamicWorld->stepSimulation(1.f/60.f, 10);

        // for (int j=dynamicWorld->getNumCollisionObjects()-1; j>=0 ;j--)
		// {
		// 	btCollisionObject* obj = dynamicWorld->getCollisionObjectArray()[j];
		// 	btRigidBody* body = btRigidBody::upcast(obj);
		// 	if (body && body->getMotionState())
		// 	{
		// 		btTransform trans;
		// 		body->getMotionState()->getWorldTransform(trans);
		// 		printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
		// 	}
		// }
    }

    void renderDebug(glm::mat4 view, glm::mat4 proj)
    {
        sh->setColor(glm::vec3(1.f, 0.3f, 0.3f));
        sh->setProjectionMatrix(proj);
        sh->begin(view);
        for (int j=dynamicWorld->getNumCollisionObjects()-1; j>=0 ;j--)
		{
			btCollisionObject* obj = dynamicWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);

                glm::vec3 pos(trans.getOrigin().getX(), 
                              trans.getOrigin().getY(), 
                              trans.getOrigin().getZ());

                btVector3 min;
                btVector3 max;
                body->getAabb(min, max);

                sh->box(glm::vec3(min.getX(), min.getY(), min.getZ()),
                        glm::vec3(max.getX(), max.getY(), max.getZ()));
			}
		}
        sh->end();
    }
};

#endif