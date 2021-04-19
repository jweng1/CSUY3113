#include <stdio.h>

#include "menu.h"

void menu::Initialize() {
    state.nextScene = -1;
    state.enemies = NULL;
    state.map = NULL;
    state.player = new Entity();
}

void  menu::Update(float deltaTime) {

}

void menu::Render(ShaderProgram *program) {
    GLuint fontTexture = Util::LoadTexture("pixel_font.png");
    Util::DrawText(program, fontTexture, "Walmart Mario", 0.3, 0.05, glm::vec3(2.9,-3,0));
    Util::DrawText(program, fontTexture, "Press 'Enter' to begin", 0.2, 0.05, glm::vec3(2.5,-5,0));
}

