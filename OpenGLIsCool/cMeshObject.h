#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class cMeshObject
{
public:
	cMeshObject();
	~cMeshObject()  {};
	std::string meshName;
	glm::vec4 position;		//xyz
	glm::vec4 normal;		//normal
	glm::vec4 colourRGBA;	//RGBA
	glm::vec2 textUV;		//textures
	glm::vec3 orientation;	//Rotation:XYZ
	float scale;
	bool isWireframe;
};
