//
// Created by adria on 18. 11. 2019.
//
#pragma once
#include <ppgso/ppgso.h>

#include "object.h"
#include "HealtBar.h"

class HealtBar;

#ifndef PPGSO_TANK_H
#define PPGSO_TANK_H


class Tank final : public Object{
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;


public:
    /*!
     * Create a new player
     */
    Tank();

    /*!
     * Update player position considering keyboard inputs
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render player
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;


    /*!
     * Player click event
     * @param scene
     */
    void onClick(Scene &scene) override ;

    bool onTurn;
    float cannonHeight;
    float turningAnimationT;
    float turnedAt;
    int hitted;

    std::unique_ptr<HealtBar> healtBar;

};


#endif //PPGSO_TANK_H
