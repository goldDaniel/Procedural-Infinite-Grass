#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "camera.h"
#include "Physics.h"
#include "renderer.h"
#include "terrainChunkGenerator.h"
#include "terrainChunk.h"

#include "glad/glad.h"

#include "utils.h"

#include <cassert>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>

#include <SDL2/SDL.h>

class Game
{
public:

    Game() : running(false)
    {

        initWindow();

        cam = new Camera();
        
        chunks = generateChunks(8);
        
        physics = new PhysicsSim();
        physics->createTerrainCollisionShapes(chunks);
        
        renderer = new Renderer(window);
        renderer->setTerrain(chunks);

        player = new Player();

        physics->createPlayerRigidBody(player);

        std::cout << "PHYSICS MESH GENERATION COMPLETE" << std::endl;
    }

    ~Game()
    {
        delete renderer;
        
        for(TerrainChunk* chunk : chunks)
        {
            delete chunk;
        }
        chunks.clear();

        delete physics;
        delete cam;
    }

    void run()
    {
        assert(!running);
        running = true;

        bool left = false;
        bool right = false;
        bool up = false;
        bool down = false;

        float prevTime = SDL_GetTicks() / 1000.f;

        SDL_Event event;

        while (running) 
        {
            float currTime = SDL_GetTicks() / 1000.f;
            float dt = currTime - prevTime;
            prevTime = currTime;
            
            while (SDL_PollEvent(&event)) 
            {
                if (event.type == SDL_QUIT) 
                {
                    running = false;
                }

                if(event.type == SDL_MOUSEMOTION)
                {
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        cam->processMouseMovement(event.motion.xrel, event.motion.yrel);
                    }	
                }

                if(event.type == SDL_KEYDOWN)
                {
                    switch( event.key.keysym.sym )
                    {
                        case SDLK_a:
                            left = true;
                            break;
                        case SDLK_d:
                            right = true;
                            break;
                        case SDLK_w:
                            up = true;
                            break;
                        case SDLK_s:
                            down = true;
                            break;
                        default:
                            break;
                    }		
                }
                if(event.type == SDL_KEYUP)
                {
                    switch( event.key.keysym.sym )
                    {
                        case SDLK_a:
                            left = false;
                            break;
                        case SDLK_d:
                            right = false;
                            break;
                        case SDLK_w:
                            up = false;
                            break;
                        case SDLK_s:
                            down = false;
                            break;
                        default:
                            break;
                    }		
                }
            }

            
            float forceScale = 2048.f;
            if(up)
            {
                glm::vec3 forceDir = player->getPosition() - cam->getPosition();
                forceDir.y = 0;
                forceDir = glm::normalize(forceDir);
                player->applyForce(forceDir*forceScale);
            }
            if(down)
            {
                glm::vec3 forceDir = player->getPosition() - cam->getPosition();
                forceDir.y = 0;
                forceDir = -glm::normalize(forceDir);
                player->applyForce(forceDir*forceScale);
            }
            if(left)
            {
                glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::pi<float>() / 2.f, glm::vec3(0, 1, 0));
                glm::vec3 forceDir = player->getPosition() - cam->getPosition();
                forceDir.y = 0;
                forceDir = glm::normalize(forceDir);
                forceDir = rotation * glm::vec4(forceDir, 1.f);
                player->applyForce(forceDir*forceScale);
            }
            if(right)
            {
                glm::mat4 rotation = glm::rotate(glm::mat4(1.f), -glm::pi<float>() / 2.f, glm::vec3(0, 1, 0));
                glm::vec3 forceDir = player->getPosition() - cam->getPosition();
                forceDir.y = 0;
                forceDir = glm::normalize(forceDir);
                forceDir = rotation * glm::vec4(forceDir, 1.f);
                player->applyForce(forceDir*forceScale);
            }

            physics->step();
            cam->followTarget(player->getPosition());
            

            int w,h;
            SDL_GetWindowSize(window, &w, &h);
            glViewport(0, 0, w, h);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderer->queueModel(player->getModel(), player->getTransform());
            renderer->draw(cam->getViewMatrix());
            


            SDL_GL_SwapWindow(window);
        }
    }


private: 

    bool running;

    std::vector<TerrainChunk*> chunks;

    Camera* cam;

    PhysicsSim* physics;

    Renderer* renderer;

    SDL_Window* window = NULL;
    SDL_GLContext maincontext;

    static const int SCREEN_FULLSCREEN = 0;
    static const int SCREEN_WIDTH  = 1280;
    static const int SCREEN_HEIGHT = 720;

    Player* player;

    void initWindow()
    {
        // Initialize SDL 
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("Couldn't initialize SDL\n");
            exit(2);
        }
        atexit (SDL_Quit);
        SDL_GL_LoadLibrary(NULL); 

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        // Also request a depth buffer
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        const char* TITLE = "Procedural Grass Demo";

        // Create the window
        if(SCREEN_FULLSCREEN) 
        {
            window = SDL_CreateWindow(
                TITLE, 
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
            );
        } 
        else 
        {
            window = SDL_CreateWindow(
                TITLE, 
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
            );
        }
        if(window == NULL)
        {
            printf("Couldn't set video mode\n");
            exit(2);
        } 

        maincontext = SDL_GL_CreateContext(window);
        if(maincontext == NULL) 
        {
            printf("Failed to create OpenGL context\n");
            exit(2);
        }
        

        // Check OpenGL properties
        printf("OpenGL loaded\n");
        gladLoadGLLoader(SDL_GL_GetProcAddress);
        printf("Vendor:   %s\n", glGetString(GL_VENDOR));
        printf("Renderer: %s\n", glGetString(GL_RENDERER));
        printf("Version:  %s\n", glGetString(GL_VERSION));

        // Use v-sync
        SDL_GL_SetSwapInterval(1);

        // Disable depth test and face culling.
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        int w,h;
        SDL_GetWindowSize(window, &w, &h);
    }
};


#endif