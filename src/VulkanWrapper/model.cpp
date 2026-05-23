#include"model.h"
#include <cstring>


model::model(VkContext ctx, VkPipelineDescription pipeline, VkCommand* command): ctx(ctx),pipeline(pipeline),cmd(command){
    vertices = quad(100,10);
    verticesBuffer = createVertexBuffer(ctx, cmd, vertices);

    matrix = glm::mat4(1.0f);
    matrixBuffer = createUnifomBuffer(ctx, cmd, sizeof(glm::mat4));
    
    memcpy(matrixBuffer.data, &matrix, sizeof(glm::mat4));
}

model::~model()
{
    cleanup();
}

std::vector<Vertex> quad(uint32_t height,uint32_t width)
{

    float halfHeight = height / 2.0f;
    float halfWidth = width / 2.0f;

    std::vector<Vertex> vertices;

    int index[4] = { -1,  1, -1,  1 };
    int indexh[4] = { -1, -1,  1,  1 };

    for(int i = 0; i < 4; i++){
        Vertex v;
        v.position = glm::vec4(index[i] * halfWidth, indexh[i] * halfHeight,0.0f,1.0f);
        v.normal = glm::vec4(0.0f,0.0f,1.0f,0.0f);
        v.texcoord = glm::vec2(0.0f);
        vertices.push_back(v);
    }
    return vertices;
}

void model::cleanup() {
    destroyBuffer(ctx, matrixBuffer);
    destroyBuffer(ctx, verticesBuffer);
    destroyTexture(ctx, texel);
}