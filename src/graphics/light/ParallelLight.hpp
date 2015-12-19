#ifndef PARALLELLIGHT_H
#define PARALLELLIGHT_H

#include "../../lib/glm/glm.hpp"

class ParallelLight : public Light
{
	private:
		glm::vec3 direction;
	public:
		void setDirection( glm::vec3 direction ){ this->direction = direction; }
		glm::vec3 getDirection(){ return direction; }
};

#endif // PARALLELLIGHT_H
