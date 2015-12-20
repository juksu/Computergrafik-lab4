#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.hpp"
//~ #include "../../lib/glm/glm.hpp"

class PointLight : public Light
{
	private:
		glm::dvec3 position;
	public:
		void setPosition( glm::dvec3 position ){ this->position = position; }
		glm::dvec3 getPosition(){ return position; }
};

#endif // POINTLIGHT_H
