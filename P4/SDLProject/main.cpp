#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#define PLATFORM_COUNT 17
#define ENEMY_COUNT 3
#include "stb_image.h"

#include "Entity.h"

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *enemies;
};

GameState state;
GLuint fontTextId;


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0,-1.0f,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-10.0f,0);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("george_0.png");
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};
    
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint wallTextID = LoadTexture("platformPack_tile013.png");
    
    for (int i = 0; i < 12; ++ i){
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = wallTextID;
        state.platforms[i].position = glm::vec3(i-4.5f,-3.5f,0);
        state.platforms[i].Update(0,NULL,NULL,0);
    }
    
    for (int i = 10; i < 17; ++ i){
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = wallTextID;
        state.platforms[i].position = glm::vec3(i-4.5f-10,-1,0);
        state.platforms[i].Update(0,NULL,NULL,0);
    }
    
    for (int i = 14; i < PLATFORM_COUNT; ++ i){
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = wallTextID;
        state.platforms[i].position = glm::vec3(i-8,-2.5,0);
        state.platforms[i].Update(0,NULL,NULL,0);
    }
    state.enemies = new Entity[ENEMY_COUNT];
    state.enemies[0].aiState = WALKING;
    state.enemies[0].position = glm::vec3(2.8,-2.5f,0);
    
    state.enemies[1].aiState = WAITING;
    state.enemies[1].position = glm::vec3(-2,0,0);
    
    state.enemies[2].aiState = WAITING;
    state.enemies[2].position = glm::vec3(-5,0,0);
    

    for (int i = 0; i < ENEMY_COUNT; ++i){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].speed = 0.6f;
        state.enemies[i].height = 0.7f;
        state.enemies[i].width = 0.7f;
        state.enemies[i].textureID = LoadTexture("platformChar_idle.png");
        state.enemies[i].Update(0,state.player,NULL,0);
    }

    
    
}

void ProcessInput() {
    state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_SPACE:
                        break;
                    case SDLK_UP:
                        state.player->position.y += 3.0f;
                        break;
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x -= 2.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x += 2.0f;
        state.player->animIndices = state.player->animRight;
    }
    else if (keys[SDL_SCANCODE_UP]) {
    }
    if (glm::length(state.player->movement) > 1.0f) {
    }
}

void drawCharacter(ShaderProgram* program,char s,GLuint textID,float i){
    modelMatrix = glm::mat4(1.0f);
    program->SetModelMatrix(modelMatrix);
    float u = (float)(s %16)/16;
    float v = (float)(s /16)/16;
    
    float width = 1.0f / (float)16;
    float height = 1.0f / (float)16;
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -2.0f+i*0.5f, -0.5f, -1.5f+i*0.5f, -0.5f, -1.5f+i*0.5f, 0.5f, -2.0f+i*0.5f, -0.5f, -1.5f+i*0.5f, 0.5f, -2.0f+i*0.5f, 0.5f };
    
    glBindTexture(GL_TEXTURE_2D, textID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void drawText(ShaderProgram* program,std::string text,GLuint textID){
    for (int i = 0; i < text.length();++i){
        drawCharacter(program, text[i], textID, i);
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return; }
    while (deltaTime >= FIXED_TIMESTEP) {
        state.player->Update(FIXED_TIMESTEP,NULL,state.platforms,PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    for (int i = 0; i < ENEMY_COUNT; ++i){
        state.enemies[i].Update(FIXED_TIMESTEP,state.player,state.platforms,PLATFORM_COUNT);
    }


}



void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < PLATFORM_COUNT; ++ i){
        state.platforms[i].Render(&program);
    }
    
    GLuint fontTextID = LoadTexture("pixel_font.png");
    state.player->Render(&program);

    
    for (int i = 0; i < ENEMY_COUNT; ++i){
        state.player->checkCollision(&state.enemies[i]);
        
        if (state.player->hitBottom){
            state.enemies[i].aiState = DEAD;
        }else if (state.player->checkCollision(&state.enemies[i])&&state.enemies[i].aiState != DEAD ){
            state.player->isActive = false;
            std::string text = "You Lose";
            drawText(&program,text,fontTextID);
        }else if (state.player->isActive == false){
            std::string text = "You Lose";
            drawText(&program,text,fontTextID);
        }
        state.enemies[i].Render(&program);
    }
    
    
    if (state.enemies[0].aiState == DEAD && state.enemies[1].aiState == DEAD && state.enemies[2].aiState == DEAD){
        std::string text = "You Win";
        drawText(&program,text,fontTextID);
    }
    SDL_GL_SwapWindow(displayWindow);
}




void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
