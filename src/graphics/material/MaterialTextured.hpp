#ifndef MATERIALTEXTURED_H
#define MATERIALTEXTURED_H

#include "Material.hpp"
#include <string>
#include <iostream>

class MaterialTextured : public Material
{
	private:
		size_t width;
		size_t height;
		glm::dvec3* texture;
	public:
		void setTexture( glm::dvec3* texture ){ this->texture = texture; }
		glm::dvec3* getTexture(){ return texture; }
		void setDimension( size_t width, size_t height ) { this->width = width; this->height = height; }
		virtual glm::dvec3 getColor( double x, double y ) const;
};

#endif // MATERIALTEXTURED_H
