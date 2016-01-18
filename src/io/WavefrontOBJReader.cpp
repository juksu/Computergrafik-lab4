#include "WavefrontOBJReader.hpp"
#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtx/string_cast.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

glm::dvec3 WavefrontOBJReader::readdVec3FromString( string str )
{	
	// create a stream for that string;
	glm::dvec3 vec;
	
	istringstream iss( str );
	
	iss >> vec[0];
	iss >> vec[1];
	iss >> vec[2];
	
	return vec;
}

glm::dvec2 WavefrontOBJReader::readdVec2FromString( string str )
{
	glm::dvec2 vec;
	istringstream iss( str );
	
	iss >> vec[0];
	iss >> vec[1];
	
	return vec;
}

string WavefrontOBJReader::readStringInformation( string str )
{
	string result;
	istringstream iss( str );
	iss >> result;
	return result;
}

void WavefrontOBJReader::readOBJ( string filename, Mesh* mesh )
{
	//~ cout << "reading obj" << endl;
	//~ fstream fstest;
	//~ string testfilename = filename;
	//~ testfilename.insert( 2, "test" );
	//~ fstest.open( testfilename, std::fstream::in | std::fstream::out | std::fstream::app );
	//~ if( fstest )
		//~ fstest << "test content";
	//~ else
		//~ cout << "no test " << testfilename <<  endl;
	//~ 
	cout << "Reading .obj-file " << filename << endl;
	
	ifstream fs;
	fs.open( filename, ifstream::in );
	
	std::vector<size_t> verticesPerFace;
	std::vector<size_t> faceVertices;
	std::vector<size_t> faceNormals;
	std::vector<size_t> faceTextureCoordinates;
	std::vector<glm::dvec3> vertices;
	std::vector<glm::dvec3> normals;
	std::vector<glm::dvec2> textureCoordinates;
	
	if( fs.is_open() )
	{
		// get lines in .obj file
		string line;
		
		 
		
		while( getline( fs, line ) )
		{
			// vertex points
			// vertex points may have an optional fourth component and defaults to 1.0 -> ignore
			if( line.compare( 0, 2, "v " ) == 0	) // vertex
			{
				// send the string to the vector reader, however exclude the element information
				glm::dvec3 vertex = readdVec3FromString(line.substr(2));
				
				//~ cout << "vertex" << endl;
				//~ cout << to_string(vertex) << endl;
				//~ cout << line << endl;
				
				vertices.push_back( vertex );
				mesh->addVertex( vertex );
			}
			// textureCoordinates
			// texture may have an optional third component w and defaults to 0 -> ignore
			if( line.compare( 0, 2, "vt" ) == 0	)
			{
				glm::dvec2 texCoordinate = readdVec2FromString(line.substr(2));
				
				// it may be that the texture coordinates are not within [0,1] -> in that case divide by max
				// (as once discussed in class)
				double max = 1;
				if( texCoordinate[0] > max )
					max = texCoordinate[0];
				if( texCoordinate[1] > max )
					max = texCoordinate[1];
				
				texCoordinate = texCoordinate / max;
				
				//~ cout << "vertexTexture" << endl;
				//~ cout << to_string(texCoordinate) << endl;
				//~ cout << line << endl;
				
				textureCoordinates.push_back( texCoordinate );
				mesh->addTextureCoordinate( texCoordinate );
			}
			// vertexNormal
			// keep in mind that they may not be unit vectors -> normalize
			if( line.compare( 0, 2, "vn" ) == 0	) 
			{
				glm::dvec3 normal = readdVec3FromString( line.substr(2) );
				normal = glm::normalize( normal );
				
				//~ cout << "vertexNormal" << endl;
				//~ cout << to_string(vertexNormal) << endl;
				//~ cout << line << endl;
				
				normals.push_back( normal );
				mesh->addNormal( normal );
			}
			// mtllib provides mtl file name where material can be found
			if( line.compare( 0, 6, "mtllib" ) == 0	)
			{
				/// TODO: not exactly clear how or if mtl is used - no mtl files provided?
				string mtllib = readStringInformation( line.substr(2) );
				
				//~ cout << "mtllib" << endl;
				//~ cout << mtllib << endl;
				//~ cout << line << endl;
			}
			// usemtl specifies material for element following it, name found in mtl file
			if( line.compare( 0, 6, "usemtl" ) == 0	)
			{
				/// TODO: not exactly clear how or if mtl is used - no mtl files provided?
				string usemtl = readStringInformation( line.substr(2) );
				//~ 
				//~ cout << "usemtl" << endl;
				//~ cout << usemtl << endl;
				//~ cout << line << endl;
			}
			// smooth shading
			if( line.compare( 0, 2, "s " ) == 0	)
			{
				/// TODO: s is for smooth shading but what does it?
				string smoothShading = readStringInformation( line.substr(2) );
				
				//~ cout << "smooth shading" << endl;
				//~ cout << smoothShading << endl;
				//~ cout << line << endl;
			}
			// plygonal face element
			// faces are defined using lists of vertex, texture and normal indices in the format v/vt/vn
			// a v must always be provided while vt and vn are optional
			if( line.compare( 0, 2, "f " ) == 0	)
			{
				istringstream iss( line.substr(2) );
				string result;
				iss >> result;
				
				size_t verticesFace = 0;
				size_t textureFace = 0;
				size_t normalFace = 0;
				
				// polygons such as quadrilaterals can be defined using more than three vertex/texture/normal indices
				while( iss.good() )
				{
					verticesFace++;
					
					// a vertex point must allways be provided in f
					
					/// TODO stoi: string to int, stoul: string to unsigned int
					
					size_t v = stoi( result, nullptr );	// stoi reads only till the first non-number character
					//~ size_t v = stoul( result, nullptr );	// stoi reads only till the first non-number character
					size_t vt = 0;
					size_t vn = 0;
					
					size_t sep1 = 0;				
					sep1 = result.find( '/', sep1 );	// first seperator followed by vt information
					
					if( sep1 != string::npos )
					{
						// it may be that there is no vt information, example: v//vn
						size_t sep2 = 0;
						sep2 = result.find( '/', sep1 + 1 );	// second separator followed by vn information
						if( sep2 != string::npos )		// vn information provided
						{
							normalFace++;
							vn = stoi( result.substr( sep2 + 1 ), nullptr );
						}
						
						if( sep1 + 1 != sep2 )		// vt informatin provided
						{
							textureFace++;
							vt = stoi( result.substr( sep1 + 1 ), nullptr );
						}
					}
					//~ cout << "v  " << v << endl;
					//~ cout << "vt " << vt << endl;
					//~ cout << "vn " << vn << endl;
					faceVertices.push_back( v );
					faceTextureCoordinates.push_back( vt );
					faceNormals.push_back( vn );
					
					// obj files refernces are 1 based, need 0 based
					mesh->addFaceVertexRef( v - 1 );
					
					// only add vn and vt information if one was provided
					if( vn != 0 )
						mesh->addFaceNormalRef( vn - 1 );
					if( vt != 0 )
					mesh->addFaceTextureCoordinateRef( vt - 1 );
									
					iss >> result;
				}
				
				//~ cout << "polygonal face element" << endl;
				//~ cout << line << endl;
				
				verticesPerFace.push_back( verticesFace );
				
				mesh->addNumOfVerticesPerFace( verticesFace );
				mesh->addNumOfTextureCoordinatesPerFace( textureFace );
				mesh->addNumOfNormalsPerFace( normalFace );
			}	
		}
	}
	else
		cout << "OBJ file can not be opened" << endl;
		
	//~ mesh->setVerticesPerFace( verticesPerFace );
	//~ mesh->setFaceVertices( faceVertices );
	//~ mesh->setFaceNormals( faceNormals );
	//~ mesh->setFaceTextureCoordinates( faceTextureCoordinates );
	//~ mesh->setVertices( vertices );		/// TODO segmentation fault
	//~ mesh->setNormals( normals );		/// TODO segmentation fault
	//~ mesh->setTextureCoordinates( textureCoordinates );		/// TODO segmentation fault
	
	// debug
	//~ cout << "verticesPerFace" << endl;
	//~ for( size_t i = 0; i < verticesPerFace.size(); i++ )
		//~ cout << verticesPerFace.at(i) << ", ";
	//~ cout << endl << "faceVertices" << endl;
	//~ for( size_t i = 0; i < faceVertices.size(); i++ )
		//~ cout << faceVertices.at(i) << ", ";
	//~ cout << endl << "faceNormals" << endl;
	//~ for( size_t i = 0; i < faceNormals.size(); i++ )
		//~ cout << faceNormals.at(i) << ", ";
	//~ cout << endl << "faceTextureCoordinates" << endl;
	//~ for( size_t i = 0; i < faceTextureCoordinates.size(); i++ )
		//~ cout << faceTextureCoordinates.at(i) << ", ";
	//~ cout << endl << "vertices" << endl;
	//~ for( size_t i = 0; i < vertices.size(); i++ )
		//~ cout << glm::to_string(vertices.at(i)) << ", ";
	//~ cout << endl << "normals" << endl;
	//~ for( size_t i = 0; i < normals.size(); i++ )
		//~ cout << glm::to_string(normals.at(i)) << ", ";
	//~ cout << endl << "textureCoordinates" << endl;
	//~ for( size_t i = 0; i < textureCoordinates.size(); i++ )
		//~ cout << glm::to_string(textureCoordinates.at(i)) << ", ";
	//~ cout << endl;		
	
	fs.close();
}
