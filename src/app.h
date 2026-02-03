#pragma once
#include <vector>

class App
{
public:
    App();
    ~App();
    void run();
    unsigned int make_entity();
    void set_up_opengl();
    unsigned int make_texture(const char *filename);

    unsigned int cameraID;

private:
    void set_up_glfw();

    unsigned int entity_count = 0;
    GLFWwindow *window;

    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> textures;

    unsigned int shader;
};