#ifndef PARALLELLIGHT_H
#define PARALLELLIGHT_H

#include "Light.hpp"
#include "../../lib/glm/glm.hpp"

class ParallelLight : public Light
{
	private:
		glm::dvec3 direction;
	public:
		void setDirection( glm::dvec3 direction ){ this->direction = direction; }
		glm::dvec3 getDirection(){ return direction; }
};

#endif // PARALLELLIGHT_H
