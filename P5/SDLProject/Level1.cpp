#include <stdio.h>

#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 1
unsigned int level1_data[] =
{
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
    2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
    2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};


void Level1::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->position = glm::vec3(12,-3,0);
    state.player->movement = glm::vec3(0);
    state.player->entityType = PLAYER;
    state.player->acceleration = glm::vec3(0,-10.0f,0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");
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
    
    
    state.enemies = new Entity[1];
    state.enemies[0].position = glm::vec3(2,-4,0);
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0,-10,0);
    state.enemies[0].speed = 1.3f;
    state.enemies[0].textureID = Util::LoadTexture("platformChar_idle.png");
    state.enemies[0].height = 0.7f;
    state.enemies[0].width = 0.7f;

}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    state.enemies[0].Update(deltaTime, state.player, NULL, 0, state.map);

}

void Level1::Render(ShaderProgram *program) {
    
    if (state.enemies[0].checkCollision(state.player)&& state.enemies[0].isActive ){
        state.player->lives -= 1;
        state.player->position =glm::vec3(12,-3,0);
        if (state.player->lives == 0){
             state.player->isActive = false;
        }
        }
        if (!state.enemies[0].isActive){
            state.nextScene = 2;
        }

    state.enemies[0].Render(program);
    state.map->Render(program);
    state.player->Render(program);
}
