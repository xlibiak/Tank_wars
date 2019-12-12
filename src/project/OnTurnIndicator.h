//
// Created by adria on 11. 12. 2019.
//

#ifndef PPGSO_ONTURNINDICATOR_H
#define PPGSO_ONTURNINDICATOR_H

#include <ppgso/ppgso.h>

#include "object.h"
#include "HealtBar.h"

class HealtBar;

class OnTurnIndicator {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;
    float circleTime = 0.f;
public:
    OnTurnIndicator();

    bool update(HealtBar &hBar,  float dt);

    void render(Scene &scene);

    // Object properties
    glm::vec3 position{0,0,0};
    glm::vec3 rotation{0,0,0};
    glm::vec3 scale{1,1,1};
    glm::mat4 modelMatrix{1};



protected:
    void generateModelMatrix();
};


#endif //PPGSO_ONTURNINDICATOR_H
