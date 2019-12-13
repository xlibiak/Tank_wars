//
// Created by adria on 13. 12. 2019.
//

#include "Wind.h"
#include "scene.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

Wind::Wind() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("red.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("arrow.obj");
    position = {-10.f,5.f,0.f};
    generateWindDirection();
}

bool Wind::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Wind::render(Scene &scene) {
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

void Wind::generateWindDirection() {
    direction = rand() % 2;
    if (direction == 0) direction = -1;
    if(direction==1){
        rotation.z = ppgso::PI / 2.0f;
    }else{
        rotation.z = -ppgso::PI / 2.0f;
    }
    int power = rand() % 9 + 2;
    scale = {1.f,2.f,power/10.f};
}

// shared resources
std::unique_ptr<ppgso::Mesh> Wind::mesh;
std::unique_ptr<ppgso::Shader> Wind::shader;
std::unique_ptr<ppgso::Texture> Wind::texture;