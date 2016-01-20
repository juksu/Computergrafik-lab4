#ifndef MESH_H
#define MESH_H

#include "Surface.hpp"
#include "../../lib/glm/glm.hpp"
#include <vector>

class Mesh : public Surface
{
	private:
		
		// verticesPerFace holds the index to the first faceVertices for face i
		// the number of vertices per face is then verticesPerFace[i+1] - verticesPerFace[i]
		// the same for textureCoordinatesPerFace and normalsPerFace
		std::vector<size_t> verticesPerFace;
		std::vector<size_t> textureCoordinatesPerFace;
		std::vector<size_t> normalsPerFace;
		std::vector<size_t> faceVertices;
		std::vector<size_t> faceNormals;
		std::vector<size_t> faceTextureCoordinates;
		std::vector<glm::dvec3> vertices;
		std::vector<glm::dvec3> normals;
		std::vector<glm::dvec2> textureCoordinates;

	public:
		Mesh(){
			verticesPerFace.push_back( 0 );
			textureCoordinatesPerFace.push_back( 0 );
			normalsPerFace.push_back( 0 );
		}
		void setVerticesPerFace( std::vector<size_t> verticesPerFace ){ this->verticesPerFace = verticesPerFace; }
		void addNumOfVerticesPerFace( size_t numVertices ){
			verticesPerFace.push_back( numVertices + verticesPerFace.back() ); }
		
		void setTextureCoordinatesPerFace( std::vector<size_t> textureCoordinatesPerFace ) {			
			this->textureCoordinatesPerFace =  textureCoordinatesPerFace;  }
		void addNumOfTextureCoordinatesPerFace( size_t numTexture ){
			textureCoordinatesPerFace.push_back( numTexture + textureCoordinatesPerFace.back() ); }
		
		void setNormalsPerFace( std::vector<size_t> normalsPerFace ){ this->normalsPerFace = normalsPerFace; }
		void addNumOfNormalsPerFace( size_t numNormals ){
			normalsPerFace.push_back( numNormals + normalsPerFace.back() ); }
		
		void setFaceVerticesRef( std::vector<size_t> faceVertices ){ this->faceVertices = faceVertices; }
		void addFaceVertexRef( size_t vertex ){ faceVertices.push_back( vertex ); }
		
		void setFaceNormals( std::vector<size_t> faceNormals ){ this->faceNormals = faceNormals; }
		void addFaceNormalRef( size_t faceNormal ){ faceNormals.push_back( faceNormal ); }
		
		void setFaceTextureCoordinates( std::vector<size_t> faceTextureCoordinates ){ 
			this->faceTextureCoordinates = faceTextureCoordinates; }
		void addFaceTextureCoordinateRef( size_t faceTexture ){ faceTextureCoordinates.push_back( faceTexture ); }
		
		void setVertices( std::vector<glm::dvec3> vertices ){ this->vertices = vertices; }
		void addVertex( glm::dvec3 vertex ){ vertices.push_back( vertex ); }
		
		void setNormals( std::vector<glm::dvec3> normals ){ this->normals = normals; }
		void addNormal( glm::dvec3 normal ){ normals.push_back( normal ); };
		
		void setTextureCoordinates( std::vector<glm::dvec2> textureCoordinates ){ 
			this->textureCoordinates = textureCoordinates; }
		void addTextureCoordinate( glm::dvec2 textureCoordinate ){ textureCoordinates.push_back( textureCoordinate ); }
		
		void printMesh();
		
		virtual IntersectionResult* intersect( glm::dvec3 point, glm::dvec3 ray );		
};

#endif // MESH_H
