#include <glm/glm.hpp>

#pragma once


struct Vertex
{
public:
	glm::vec3 vPos;
	glm::vec3 vNormals;
	glm::vec2 vTextureCoordinates;
	glm::vec3 vRGB;
};