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


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix,modelMatrix2,modelMatrix3, projectionMatrix;



float direction_x = 1.0f;
float direction_y = 1.0f;
bool run = false;

//left paddle
glm::vec3 left_position = glm::vec3(-4.8, 0, 0);
glm::vec3 left_movement = glm::vec3(0, 0, 0);

//right paddle
glm::vec3 right_position = glm::vec3(4.8, 0, 0);
glm::vec3 right_movement = glm::vec3(0, 0, 0);

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0, 0, 0);

float default_speed = 2.0f;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}


void ProcessInput() {
    left_movement = glm::vec3(0, 0, 0);
    right_movement = glm::vec3(0, 0, 0);
    
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
                        run = true;
                        break;
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_W]) {
        left_movement.y = 2.0f;
    }
    if (keys[SDL_SCANCODE_S]) {
        left_movement.y = -2.0f;
    }
    if (keys[SDL_SCANCODE_UP]) {
        right_movement.y = 2.0f;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        right_movement.y = -2.0f;
    }

}
float lastTicks = 0.0f;

void Update() {

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    if(ball_position.x >= 4.8f || ball_position.x <= -4.8f){
        gameIsRunning = false;
    }
    left_position += left_movement * default_speed * deltaTime;
    right_position += right_movement * default_speed * deltaTime;
    
    
    if (left_position.y < -2.55f){
        left_position.y = -2.55f;
    }
    if (left_position.y > 2.55f){
        left_position.y = 2.55f;
    }
    if (right_position.y < -2.55f){
        right_position.y = -2.55f;
    }
    if (right_position.y > 2.55f){
        right_position.y = 2.55f;
    }
    
    if (run){
        ball_position.x += direction_x * 2.5 * deltaTime;
        ball_position.y += direction_y * 2.5 * deltaTime;
    }
    
    //collision detection
    float leftxdist = fabs(left_position.x - ball_position.x) - ((0.4f + 0.4f) / 2.0f);
    float leftydist = fabs(left_position.y - ball_position.y) - ((2.0f + 0.4f) / 2.0f);
    float rightxdist = fabs(right_position.x - ball_position.x) - ((0.4f + 0.4f) / 2.0f);
    float rightydist = fabs(right_position.y - ball_position.y) - ((2.0f + 0.4f) / 2.0f);
    
    if ((leftxdist<=0 && leftydist<=0) || (rightxdist <=0 && rightydist <=0)){
        direction_x = -direction_x;
    }
    if (ball_position.y >= 3.55f || ball_position.y <= -3.55f){
        direction_y = -direction_y;
    }
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, ball_position);
    
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetModelMatrix(modelMatrix);
    float vertices_s[] = {  -0.2f, -0.2f,   0.2f, -0.2f,   0.2f, 0.2f,   -0.2f, -0.2f,   0.2f, 0.2f,   -0.2f, 0.2f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_s);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    
    float vertices_p[] = { 0.2f, 1.0f, 0.2f, -1.0f, -0.2f, -1.0f,0.2f, 1.0f,-0.2f, -1.0f,-0.2f,1.0f };
    
    modelMatrix2 = glm::mat4(1.0f);
    modelMatrix2 = glm::translate(modelMatrix2,glm::vec3(right_position.x,right_position.y,0.0f));
    program.SetModelMatrix(modelMatrix2);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_p);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    
    modelMatrix3 = glm::mat4(1.0f);
    modelMatrix3 = glm::translate(modelMatrix3,glm::vec3(left_position.x,left_position.y,0.0f));
    program.SetModelMatrix(modelMatrix3);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_p);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    
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

