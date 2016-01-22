#ifndef LIGHT_H
#define LIGHT_H

#include "../../lib/glm/glm.hpp"

class Light
{
	private:
		glm::dvec3 color;
	
	public:
		void setColor( glm::dvec3 color ){ this->color = color; }
		virtual glm::dvec3 getColor() const { return color; }
};

#endif // LIGHT_H
