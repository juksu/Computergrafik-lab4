#ifndef WAVEFRONTOBJREADER_H
#define WAVEFRONTOBJREADER_H
#include <string>
#include "../lib/glm/glm.hpp"
#include "../graphics/surface/Mesh.hpp"

class WavefrontOBJReader
{
	private :
		glm::dvec3 readdVec3FromString( std::string str );
		glm::dvec2 readdVec2FromString( std::string str );
		std::string readStringInformation( std::string str );
	
	public :
		void readOBJ( std::string filename, Mesh* mesh );
};


#endif // WAVEFRONTOBJREADER_H
