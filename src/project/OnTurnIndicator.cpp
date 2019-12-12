//
// Created by adria on 11. 12. 2019.
//

#include "OnTurnIndicator.h"
#include "scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

OnTurnIndicator::OnTurnIndicator() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("green.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
    scale = glm::vec3{0.15,0.15,0.05};
}

bool OnTurnIndicator::update(HealtBar &hBar, float dt) {
    position = hBar.position;
    position.x += 0.5f;
    position.y += 0.5f;
    circleTime += dt;
    if(circleTime>1)circleTime=0;
    float theta = glm::lerp(0.f,2.0f * 3.1415926f,circleTime);
    float r = 0.1f;

    position.x += r * cosf(theta);//calculate the x component
    position.y += r * sinf(theta);//calculate the y component
    generateModelMatrix();
    return true;
}

void OnTurnIndicator::render(Scene &scene) {
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

void OnTurnIndicator::generateModelMatrix() {
    modelMatrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
}

std::unique_ptr<ppgso::Mesh> OnTurnIndicator::mesh;
std::unique_ptr<ppgso::Shader> OnTurnIndicator::shader;
std::unique_ptr<ppgso::Texture> OnTurnIndicator::texture;
