#pragma once 
#include "glm/glm.hpp"

struct Camera
{
    glm::mat4 view;
    glm::mat4 proj;
};

Camera buildCamera(glm::vec3 pos,glm::vec3 look,glm::vec3 up);
