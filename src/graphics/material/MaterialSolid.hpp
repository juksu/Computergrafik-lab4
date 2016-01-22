#ifndef MATERIALSOLID_H
#define MATERIALSOLID_H

#include "Material.hpp"

class MaterialSolid : public Material
{
	private:
		glm::dvec3 color;
	public:
		void setColor( glm::dvec3 color ){ this->color = color; }
		virtual glm::dvec3 getColor() const { return color; }
};


#endif // MATERIALSOLID
