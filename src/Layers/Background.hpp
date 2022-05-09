#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../OpenGL-Core/LayerGL.hpp"

class BackgroundLayer : public LayerGL {
public:
    BackgroundLayer();
    
    BackgroundLayer(float r, float g, float b, float alpha);

    void render() override;

    void setColor(float r, float g, float b, float alpha);

    void setColor(glm::vec3 color);

    void setColor(glm::vec4 color);

    void setColorRGB255(int r, int g, int b);

    void setTransparency(float alpha);

private:
    glm::vec4 m_color = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

};

#endif