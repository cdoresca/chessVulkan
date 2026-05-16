#include"application.h"
#include "VulkanWrapper/IBuilder.h"
#include "VulkanWrapper/shader.h"

app::app(){
    build();
}

void app::build(){

    glfwInit();
    window = glfwCreateWindow(800, 600, "Chess", NULL, NULL);

    camera = buildCamera(glm::vec3(0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f, 1.0f,0.0f));

    IBuilder<VkContext>* ctxBuilder = new vulkanContextBuilder(window);
    director<VkContext> dir(ctxBuilder);
    dir.make();
    ctx = ctxBuilder->get();

    IBuilder<VkDisplay>* displayBuilder = new vulkanDisplayBuilder(ctx, window);
    director<VkDisplay> dirDisplay(displayBuilder);
    dirDisplay.make();
    display = displayBuilder->get();

    //shader
    shader vertexShader(ctx,"shader/vert.glsl",VK_SHADER_STAGE_VERTEX_BIT);
    shader fragmentShader(ctx,"shader/frag.glsl",VK_SHADER_STAGE_FRAGMENT_BIT);

    //descriptor
    m_descriptor = new descriptor(ctx); 

    //command
    cmd = new VkCommand(ctx);

    //pipeline
    IBuilder<VkPipelineDescription>* pipelineBuilder = new vulkanPipelineBuilder(ctx,display,m_descriptor); 
}