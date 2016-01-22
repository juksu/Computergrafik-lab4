#ifndef MATERIALTEXTURED_H
#define MATERIALTEXTURED_H

#include "Material.hpp"
#include "string"

class MaterialTextured : public Material
{
	private:
		std::string texture;
	public:
		virtual glm::dvec3 getColor() const { return glm::dvec3(0,0,0); }
};

#endif // MATERIALTEXTURED_H
