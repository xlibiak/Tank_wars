//
// Created by adria on 13. 12. 2019.
//

#ifndef PPGSO_WIND_H
#define PPGSO_WIND_H
#include <ppgso/ppgso.h>

#include "object.h"

class Wind final : public Object{
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;
public:
    Wind();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void generateWindDirection();

    int direction;
};


#endif //PPGSO_WIND_H
