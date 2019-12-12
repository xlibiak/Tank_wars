//
// Created by adria on 11. 12. 2019.
//

#include "HealtBar.h"
#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
HealtBar::HealtBar() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("red.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");

    scale = glm::vec3 {1.,0.25,0.};
    auto onTurnI = std::make_unique<OnTurnIndicator>();
    onTurnI->position = this->position;
    onTurnI->position.y += 0.5f;
    ontIndicator = move(onTurnI);
}

bool HealtBar::update(Tank &tank,  float dt) {
    onTurn=tank.onTurn;
    position = tank.position;
    position.y += 1.f;
    generateModelMatrix();
    ontIndicator->update(*this, dt);
    return true;
}

void HealtBar::render(Scene &scene) {
    if(onTurn)ontIndicator->render(scene);
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void HealtBar::generateModelMatrix() {
    modelMatrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
}

void HealtBar::retexture() {
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("halfred.bmp"));
}

// shared resources
std::unique_ptr<ppgso::Mesh> HealtBar::mesh;
std::unique_ptr<ppgso::Shader> HealtBar::shader;
//std::unique_ptr<ppgso::Texture> HealtBar::texture;
