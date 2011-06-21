#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

class Vector3
{	
public:
	double x,y,z;

	//Constructors
	Vector3();
	Vector3(double ix, double iy, double iz);

	//Overloads
	Vector3& operator=(const Vector3 &vector);
	Vector3 operator+(const Vector3 &vector) const;
	void operator+=(const Vector3 &vector);
	Vector3 operator-(const Vector3 &vector) const;
	void operator-=(const Vector3 &vector);
	Vector3 operator*(double scalar);
	void operator*=(double scalar);
	double operator*(const Vector3 &vector) const;
	bool operator==(const Vector3 &vector) const;
	bool operator!=(const Vector3 &vector) const;
	bool operator<(const Vector3 &vector) const;
	bool operator>(const Vector3 &vector) const;
	bool operator<=(const Vector3 &vector) const;
	bool operator>=(const Vector3 &vector) const;
	void operator%=(const Vector3 &vector);
	Vector3 operator%(const Vector3 &vector) const;

	//Public Functions
	double GetMagnitude();
	double GetSqMagnitude();
	void Normalize();
	void Clear();
	void Invert();

};

#endif