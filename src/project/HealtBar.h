//
// Created by adria on 11. 12. 2019.
//

#ifndef PPGSO_HEALTBAR_H
#define PPGSO_HEALTBAR_H

#include <ppgso/ppgso.h>

#include "object.h"
#include "OnTurnIndicator.h"
#include "tank.h"

// Forward declare a tank
class Tank;
class OnTurnIndicator;

class HealtBar final{
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;
public:
    HealtBar();

    bool update(Tank &tank,  float dt);

    void render(Scene &scene);

    void retexture();

    // Object properties
    glm::vec3 position{0,0,0};
    glm::vec3 rotation{0,0,0};
    glm::vec3 scale{1,1,1};
    glm::mat4 modelMatrix{1};

    std::unique_ptr<OnTurnIndicator> ontIndicator;
    bool onTurn;

protected:
    void generateModelMatrix();
};


#endif //PPGSO_HEALTBAR_H
