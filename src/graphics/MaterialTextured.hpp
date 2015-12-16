#ifndef MATERIALTEXTURED_H
#define MATERIALTEXTURED_H

#include "Material.hpp"
#include "string"

class MaterialTextured : public Material
{
	private:
		std::string texture;
};

#endif // MATERIALTEXTURED_H
