#ifndef MATERIALTEXTURED_H
#define MATERIALTEXTURED_H

#include "Material.hpp"
#include "string"

class MaterialTextured : public Material
{
	private:
		glm::dvec3* texture;
	public:
		void setTexture( glm::dvec3* texture ){ this->texture = texture; }
		virtual glm::dvec3 getColor() const { return glm::dvec3(0,0,0); }
};

#endif // MATERIALTEXTURED_H
