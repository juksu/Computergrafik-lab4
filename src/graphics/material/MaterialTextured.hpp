#ifndef MATERIALTEXTURED_H
#define MATERIALTEXTURED_H

#include "Material.hpp"
#include "string"
#include "../../lib/glm/glm.hpp"

class MaterialTextured : public Material
{
	private:
		std::string texture;
	public:
		virtual glm::vec3 getColor(){ return glm::vec3(0,0,0); }
};

#endif // MATERIALTEXTURED_H
