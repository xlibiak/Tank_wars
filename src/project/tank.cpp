//
// Created by adria on 18. 11. 2019.
//

#include "tank.h"
#include "scene.h"
#include "projectile.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Tank::mesh;
std::unique_ptr<ppgso::Texture> Tank::texture;
std::unique_ptr<ppgso::Shader> Tank::shader;




Tank::Tank(){
    // Scale the default model
    scale *= 0.001f;
    onTurn = false;
    cannonHeight = 0.0f;
    turningAnimationT = 0.0f;
    auto hBar = std::make_unique<HealtBar>();
    hBar->position = this->position;
    hBar->position.y += 0.5f;
    healtBar = move(hBar);
    hitted = 0;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("corsair.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Tank.obj");
}

bool Tank::update(Scene &scene, float dt) {
    for ( auto& obj : scene.objects ) {
        auto projectile = dynamic_cast<Projectile*>(obj.get());
        if (projectile) {
            if (onTurn and distance(position, projectile->position) < 1) {
                // Explode
                /*auto explosion = std::make_unique<Explosion>();
                explosion->position = position;
                explosion->scale = scale * 3.0f;
                scene.objects.push_back(move(explosion));*/
                projectile->hit = true;
                // Die
                if(hitted){
                    return false;
                } else{
                    healtBar->retexture();
                    hitted = 1;
                }
            }
            generateModelMatrix();
            return true;
        }
    }

    if(scene.keyboard[GLFW_KEY_SPACE]) {
        if(onTurn){
            onTurn= false;
            auto projectile = std::make_unique<Projectile>();
            projectile->position = position + glm::vec3(rotation.z/10,0.3f,0.0f);
            projectile->rotation = glm::vec3(0.0f, -rotation.z/(cannonHeight+1), 0.0f);
            projectile->speed =  glm::vec3(rotation.z*2, cannonHeight, 0.0f);
            projectile->rotMomentum=  glm::vec3(0.0f,-rotation.z/2,0.0f);
            scene.objects.push_back(move(projectile));
            for ( auto& obj : scene.objects ) {
                if (obj.get() == this)
                    continue;
                auto tank = dynamic_cast<Tank*>(obj.get());
                if (tank) {
                    tank->onTurn=true;
                    scene.camera->position = glm::vec3((position.x + tank->position.x)/2,5.0f,-25.0f);
                }
            }
        }
    }

    if (onTurn) {
        scene.camera->position=position + glm::vec3(0.0f,5.0f,-10.0f);
        // Hit detection
        /*for ( auto& obj : scene.objects ) {
            // Ignore self in scene
            if (obj.get() == this)
                continue;

            // We only need to collide with asteroids, ignore other objects
            auto asteroid = dynamic_cast<Asteroid*>(obj.get());
            if (!asteroid) continue;

            if (distance(position, asteroid->position) < asteroid->scale.y) {
                // Explode
                auto explosion = std::make_unique<Explosion>();
                explosion->position = position;
                explosion->scale = scale * 3.0f;
                scene.objects.push_back(move(explosion));

                // Die
                return false;
            }
        }*/

        // Keyboard controls
        if (scene.keyboard[GLFW_KEY_LEFT]) {
            position.x += 5 * dt;
            if (rotation.z != ppgso::PI / 2.0f) {
                turningAnimationT = 0.005f;
            } else {
                turningAnimationT = 0.f;
            }
        } else if (scene.keyboard[GLFW_KEY_RIGHT]) {
            position.x -= 5 * dt;
            if (rotation.z != -ppgso::PI / 2.0f) {
                turningAnimationT = -0.005f;
            } else {
                turningAnimationT = 0.f;
            }
        }
        turnedAt += turningAnimationT;
        if (turnedAt > 1) {
            turnedAt = 1;
        }
        if (turnedAt < 0) {
            turnedAt = 0;
        }
        rotation.z = glm::lerp(-ppgso::PI / 2.0f, ppgso::PI / 2.0f, turnedAt);


        if (scene.keyboard[GLFW_KEY_UP]) {
            if (cannonHeight < 3) {
                cannonHeight += 0.01f;
            }
        } else if (scene.keyboard[GLFW_KEY_DOWN]) {
            if (cannonHeight > 0) {
                cannonHeight -= 0.01f;
            }
        }
    }
    healtBar->update(*this, dt);
    generateModelMatrix();
    return true;
}

void Tank::render(Scene &scene) {
    healtBar->render(scene);
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

void Tank::onClick(Scene &scene) {
    std::cout << "Player has been clicked!" << std::endl;
}

