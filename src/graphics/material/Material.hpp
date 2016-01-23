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
		double refractionIndex;
	
	public:
		void setPhong( double ka, double kd, double ks, int exponent ) {
				this->phongKA = ka; this->phongKD = kd; this->phongKS = ks; this->phongExponent = exponent; }
		double getPhongKA() const { return phongKA; }
		double getPhongKD() const { return phongKD; }
		double getPhongKS() const { return phongKS; }
		double getPhongExponent() const { return phongExponent; }
		void setReflectance( double reflectance ) { this->reflectance = reflectance; }
		double getReflectance() const { return reflectance; }
		void setTransmittance( double transmittance ) { this->transmittance = transmittance; }
		double getTransmittance() const { return transmittance; }
		void setRefractionIndex( double refractionIndex ) {this->refractionIndex = refractionIndex; }
		double getRefractionIndex() const { return refractionIndex; }
		
		virtual glm::dvec3 getColor() const = 0;
};

#endif // MATERIAL_H
