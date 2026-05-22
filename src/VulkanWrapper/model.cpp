#include"model.h"

model::model(renderer* r): render_(r){
    vertices = quad();
    verticesBuffer = render_->uploadVertex(vertices);
    matrix = glm::mat4(1.0f);
}

std::vector<Vertex> quad(uint32_t height,uint32_t width)
{

    float halfHeight = height / 2.0f;
    float halfWidth = width / 2.0f;

    std::vector<Vertex> vertices;

    int index[4] = { -1, 1, 1,-1};
    int indexh[4] = { -1, 1, -1, 1};

    for(int i = 0; i < 4; i++){
        Vertex v;
        v.position = glm::vec4(index[i] * halfWidth, index[i] * halfHeight,0.0f,1.0f);
        v.normal = glm::vec4(0.0f,0.0f,1.0f,0.0f);
        vertices.push_back(v);
    }
    return vertices;
}