#ifndef MATERIALSOLID_H
#define MATERIALSOLID_H

#include "Material.hpp"
#include "../../lib/glm/glm.hpp"

class MaterialSolid : public Material
{
	private:
		glm::vec3 color;
	public:
		void setColor( glm::vec3 color ){ this->color = color; }
		virtual glm::vec3 getColor(){ return color; }
};


#endif // MATERIALSOLID
