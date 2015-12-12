#ifndef XMLREADER_H
#define XMLREADER_H

#include <string>
#include "../lib/tinyxml2/tinyxml2.h"


class XMLReader
{
	private:
		std::string filename;
		tinyxml2::XMLDocument xmlDocument;
		
	public:
		XMLReader(){ tinyxml2::XMLDocument xmlDocument; };
		bool loadFile( std::string filename );
		std::string getOutputFilename();
		void readxml();
};

#endif //XMLREADER_H
