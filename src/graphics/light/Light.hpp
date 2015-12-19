#ifndef LIGHT_H
#define LIGHT_H

#include "../../lib/glm/glm.hpp"

class Light
{
	private:
		glm::vec3 color;
	
	public:
		void setColor( glm::vec3 color ){ this->color = color; }
		glm::vec3 getColor(){ return color; }
};

#endif // LIGHT_H
