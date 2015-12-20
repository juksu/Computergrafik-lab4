#ifndef LIGHTCONTAINER_H
#define LIGHTCONTAINER_H

#include "AmbientLight.hpp"
#include "PointLight.hpp"
#include "ParallelLight.hpp"
#include "SpotLight.hpp"


#include <vector>



class LightContainer
{
	private:
		std::vector<AmbientLight*> ambientLights;
		std::vector<PointLight*> pointLights;
		std::vector<ParallelLight*> parallelLights;
		std::vector<SpotLight*> spotLights;
		
	public:
		void addAmbientLight( AmbientLight* ambientLight ){ ambientLights.push_back( ambientLight ); }
		std::vector<AmbientLight*> getAmbientLights(){ return ambientLights; }
		void addPointLight( PointLight* pointLight ){ pointLights.push_back( pointLight ); }
		std::vector<PointLight*> getPointLights(){ return pointLights; }
		void addParallelLight( ParallelLight* parallelLight ){ parallelLights.push_back( parallelLight ); }
		std::vector<ParallelLight*> getParallelLights(){ return parallelLights; }
		void addSpotlLight( SpotLight* spotlLight ){ spotLights.push_back( spotlLight ); }
		std::vector<SpotLight*> getSpotLights(){ return spotLights; }
};

#endif // LIGHTCONTAINER_H
