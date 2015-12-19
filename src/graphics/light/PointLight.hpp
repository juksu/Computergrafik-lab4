#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "../../lib/glm/glm.hpp"

class PointLight : public Light
{
	private:
		glm::vec3 position;
	public:
		void setPosition( glm::vec3 position ){ this->position = position; }
		glm::vec3 getPosition(){ return position; }
};

#endif // POINTLIGHT_H
