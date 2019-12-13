//
// Created by adria on 13. 12. 2019.
//

#ifndef PPGSO_ENDBACKGROUND_H
#define PPGSO_ENDBACKGROUND_H

#include <ppgso/ppgso.h>
#include "object.h"

class EndBackground : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    glm::vec2 textureOffset;
public:

    static std::unique_ptr<ppgso::Texture> texture;
    EndBackground();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};


#endif //PPGSO_ENDBACKGROUND_H
