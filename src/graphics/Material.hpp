#ifndef MATERIAL_H
#define MATERIAL_H



class Material
{
	private:
		//~ glm::vec3 color;	// color only for material_solid
		double phongKA;
		double phongKD;
		double phongKS;
		int phongExponent;
		double reflectance;
		double transmittance;
		double refraction;
	
	public:
		void setPhong( double ka, double kd, double ks, int exponent );
		void setReflactance( double reflectance ) { this->reflectance = reflectance; }
		void setTransmittance( double transmittance ) { this->transmittance = transmittance; }
		void setRefraction( double refraction ) {this->refraction = refraction; }
};

inline void Material::setPhong( double ka, double kd, double ks, int exponent )
{
	this->phongKA = ka; this->phongKD = kd; this->phongKS = ks; this->phongExponent = exponent;
}

#endif // MATERIAL_H
