#include "GlobalUtil.h"

glm::mat4 MathUtil::ComputeModelMatrix(Geometry::Transform3D transform)
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, transform.position);
    //TODO: Rotations are euler angles for now so gimble lock + not sure about the tri-rotation
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, transform.scale);
    return model;
}