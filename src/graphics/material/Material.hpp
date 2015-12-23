#ifndef MATERIAL_H
#define MATERIAL_H

#include "../../lib/glm/glm.hpp"

class Material
{
	private:
		double phongKA;
		double phongKD;
		double phongKS;
		int phongExponent;
		double reflectance;
		double transmittance;
		double refraction;
	
	public:
		void setPhong( double ka, double kd, double ks, int exponent );
		double getPhongKA(){ return phongKA; }
		double getPhongKD(){ return phongKD; }
		double getPhongKS(){ return phongKS; }
		double getPhongExponent(){ return phongExponent; }
		void setReflactance( double reflectance ) { this->reflectance = reflectance; }
		void setTransmittance( double transmittance ) { this->transmittance = transmittance; }
		void setRefraction( double refraction ) {this->refraction = refraction; }
		
		virtual glm::dvec3 getColor() = 0;
};

inline void Material::setPhong( double ka, double kd, double ks, int exponent )
{
	this->phongKA = ka; this->phongKD = kd; this->phongKS = ks; this->phongExponent = exponent;
}

#endif // MATERIAL_H
