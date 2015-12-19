#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "../../lib/glm/glm.hpp"

class SpotLight : public Light
{
	private:
		glm::vec3 position;
		glm::vec3 direction;
		double falloffAlpha1;
		double falloffAlpha2;
		
	public:
		void setPosition( glm::vec3 position ){ this->position = position; }
		glm::vec3 getPosition(){ return position; }
		void setDirection( glm::vec3 direction ){ this->direction = direction; }
		glm::vec3 getDirection(){ return direction; }
		void setFalloffAlpha1( double falloffAlpha1 ){ this->falloffAlpha1 = falloffAlpha1; }
		double getFalloffAlpha1(){ return falloffAlpha1; }
		void setFalloffAlpha2( double falloffAlpha2 ){ this->falloffAlpha2 = falloffAlpha2; }
		double getFalloffAlpha2(){ return falloffAlpha2; }
};

#endif // SPOTLIGHT_H
