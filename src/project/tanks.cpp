//
// Created by adrian on 14. 11. 2019.
//
#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>
#include "tank.h"
#include "scene.h"
#include "desert.h"
#include "Wind.h"
#include "EndBackground.h"

const unsigned int SIZE = 512;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    Scene endScene;
    bool animate = true;
    int tankCount;

    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position.z = -15.0f;
        scene.camera = move(camera);

        // Add space background
        auto desertFloor = std::make_unique<Desert>();
        desertFloor->scale = {30.f,1.f,30.f};
        desertFloor->position.y = -7.7f;
        scene.objects.push_back(move(desertFloor));

        // Add players to the scene
        auto player1 = std::make_unique<Tank>();
        player1->onTurn=true;
        player1->position.y = -7;
        player1->position.x = -6;
        player1->turnedAt = 1;
        player1->name = "player1";
        scene.objects.push_back(move(player1));

        auto player2 = std::make_unique<Tank>();
        player2->position.y = -7;
        player2->position.x = 6;
        player2->rotation.z = -ppgso::PI/2.0f;
        player2->turnedAt = 0;
        player2->name = "player2";
        scene.objects.push_back(move(player2));

        auto wind = std::make_unique<Wind>();
        scene.wind = move(wind);
    }

    void initEndScene(){
        endScene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position.z = -15.0f;
        endScene.camera = move(camera);

        endScene.objects.push_back(std::make_unique<EndBackground>());
    }

public:
    /*!
     * Construct custom game window
     */
    SceneWindow() : Window{"Tank wars", 2*SIZE, SIZE} {
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
        initEndScene();
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override {
        scene.keyboard[key] = action;

        // Reset
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            initScene();
        }

        // Pause
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            animate = !animate;
        }
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override {
        scene.cursor.x = cursorX;
        scene.cursor.y = cursorY;
    }

    /*!
     * Handle cursor buttons
     * @param button Mouse button being manipulated
     * @param action Mouse bu
     * @param mods
     */
    void onMouseButton(int button, int action, int mods) override {
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            scene.cursor.left = action == GLFW_PRESS;

            if (scene.cursor.left) {
                // Convert pixel coordinates to Screen coordinates
                double u = (scene.cursor.x / width - 0.5f) * 2.0f;
                double v = - (scene.cursor.y / height - 0.5f) * 2.0f;

                // Get mouse pick vector in world coordinates
                auto direction = scene.camera->cast(u, v);
                auto position = scene.camera->position;

                // Get all objects in scene intersected by ray
                auto picked = scene.intersect(position, direction);

                // Go through all objects that have been picked
                for (auto &obj: picked) {
                    // Pass on the click event
                    obj->onClick(scene);
                }
            }
        }
        if(button == GLFW_MOUSE_BUTTON_RIGHT) {
            scene.cursor.right = action == GLFW_PRESS;
        }
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = animate ? (float) glfwGetTime() - time : 0;

        time = (float) glfwGetTime();

        // Set gray background
        glClearColor(.5f, .5f, .5f, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tankCount=0;
        std::string name;
        for ( auto& obj : scene.objects ) {
            auto tank = dynamic_cast<Tank*>(obj.get());
            if (tank) {
                tankCount++;
                name = tank->name;
            }
        }

        // Update and render all objects
        if(tankCount==2) {
            scene.update(dt);
            scene.render();
        }else {
            EndBackground::texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(name+".bmp"));
            endScene.update(dt);
            endScene.render();
        }
    }
};


int main() {
    // Initialize our window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}