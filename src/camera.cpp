#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera buildCamera(glm::vec3 pos,glm::vec3 look,glm::vec3 up){
    Camera cam;

    cam.view = glm::lookAt(pos,look,up);

    cam.proj = glm::ortho(0.0f,400.0f,400.0f,0.0f,0.1f,100.0f);

    return cam;
}