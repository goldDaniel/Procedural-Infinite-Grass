#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/gtc/quaternion.hpp>

#include "terrainChunk.h"

#include "player.h"
#include "shapeRenderer.h"

class GLBulletDebugRenderer : public btIDebugDraw
{
private:
    int debugMode;

    ShapeRenderer* sh;

public:
    GLBulletDebugRenderer(int maxVertsPerDrawCall)
    {
        sh = new ShapeRenderer(maxVertsPerDrawCall);
    }

    virtual ~GLBulletDebugRenderer()
    {
        delete sh;
    }

    void begin(glm::mat4 view, glm::mat4 proj)
    {
        sh->setColor(glm::vec3(1.f, 1.f, 1.f));
        sh->setProjectionMatrix(proj);
        sh->begin(view);
    }

    void end()
    {
        sh->end();
    }

    virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
    {
        sh->line(from.getX(), from.getY(), from.getZ(),
                 to.getX(),   to.getY(),   to.getZ());
    }


    virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
    {

    }

    virtual void reportErrorWarning(const char* warningString)
    {

    }

    virtual void draw3dText(const btVector3& location,const char* textString)
    {

    }

    virtual void setDebugMode(int debugMode)
    {
        //this->debugMode = debugMode;
    }

    virtual int  getDebugMode() const 
    { 
        return btIDebugDraw::DBG_DrawWireframe;
    }
};

class PhysicsSim
{
private:
    
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatcher;
    btBroadphaseInterface* overlappingCache; 
    btSequentialImpulseConstraintSolver* solver;

    btDiscreteDynamicsWorld* dynamicWorld;
    
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    GLBulletDebugRenderer* debugRenderer;
    

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

        dynamicWorld->setGravity(btVector3(0, -32, 0));



        debugRenderer = new GLBulletDebugRenderer(8192*4);

        debugRenderer->setDebugMode( debugRenderer->getDebugMode()| btIDebugDraw::DBG_DrawWireframe );

        dynamicWorld->setDebugDrawer(debugRenderer);
    }

    ~PhysicsSim()
    {
        delete dynamicWorld;
        delete solver;
        delete overlappingCache;
        delete collisionDispatcher;
        delete collisionConfig;
    }
    
    void createPlayerRigidBody(Player* player)
    {
        
        // btCollisionShape* colShape = new btBoxShape(btVector3(32.f,32.f,32.f));
		btCollisionShape* colShape = new btSphereShape(btScalar(8.f));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(32.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
		{	
            colShape->calculateLocalInertia(mass,localInertia);
        }

        startTransform.setOrigin(btVector3(0,256,0));
    
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        player->setRigidBody(body);
        dynamicWorld->addRigidBody(body);
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

    //immediate mode style API, this is VERY VERY SLOW
    void renderDebug(glm::mat4 view, glm::mat4 proj)
    {
        debugRenderer->begin(view, proj);
        dynamicWorld->debugDrawWorld();
        debugRenderer->end();
    }
};

#endif