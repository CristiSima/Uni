#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateElipse(const std::string &name, glm::vec3 center, float lengthX, float lengthY, glm::vec3 color, bool fill = false);
	Mesh* CreateTriangleEquilateral(const std::string &name, glm::vec3 center, float edgeLength, glm::vec3 color, bool fill=false);
    Mesh* CreateDuckWing(const std::string &name, glm::vec3 center, glm::vec3 color, bool fill=true);
    Mesh* CreateTarget(const std::string &name, glm::vec3 center, float outerRadius, float innerRadius, glm::vec3 color, bool fill=true);
	Mesh* CreateBuletIcon(const std::string &name, glm::vec3 center, glm::vec3 color, bool fill=true);
    // https://www.desmos.com/calculator/c3eniond7t
}