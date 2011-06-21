#include "Vector3.h"

Vector3::Vector3()
{
	x = y = z = 0.0;
}

Vector3::Vector3(double ix, double iy, double iz)
{
	x = ix;
	y = iy;
	z = iz;
}

Vector3& Vector3::operator=(const Vector3 &vector)
{
	if (this != &vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}
	return *this;
}

Vector3 Vector3::operator+(const Vector3 &vector) const
{
	return Vector3(x+vector.x, y+vector.y, z+vector.z);
}

void Vector3::operator+=(const Vector3 &vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
}

Vector3 Vector3::operator-(const Vector3 &vector) const
{
	return Vector3(x-vector.x, y-vector.y, z-vector.z);
}

void Vector3::operator-=(const Vector3 &vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
}

Vector3 Vector3::operator*(double scalar)
{
	return Vector3(x*scalar, y*scalar, z*scalar);
}

void Vector3::operator*=(double scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

double Vector3::operator*(const Vector3 &vector) const
{
	return x*vector.x + y*vector.y + z*vector.z;
}

bool Vector3::operator==(const Vector3 &vector) const
{
	if(x == vector.x && y == vector.y && z == vector.z)
		return true;
	else
		return false;
}

bool Vector3::operator!=(const Vector3 &vector) const
{
	if(x != vector.x || y != vector.y || z != vector.z)
		return true;
	else
		return false;
}

bool Vector3::operator<(const Vector3 &vector) const
{
	if(x < vector.x && y < vector.y && z < vector.z)
		return true;
	else
		return false;
}

bool Vector3::operator>(const Vector3 &vector) const
{
	if(x > vector.x && y > vector.y && z > vector.z)
		return true;
	else
		return false;
}

bool Vector3::operator<=(const Vector3 &vector) const
{
	if(x <= vector.x && y <= vector.y && z <= vector.z)
		return true;
	else
		return false;
}

bool Vector3::operator>=(const Vector3 &vector) const
{
	if(x >= vector.x && y >= vector.y && z >= vector.z)
		return true;
	else
		return false;
}

Vector3 Vector3::operator%(const Vector3 &vector) const
{
	return Vector3(y*vector.z - z*vector.y,
				   z*vector.x - x*vector.z,
				   x*vector.y - y*vector.x);
}

void Vector3::operator%=(const Vector3 &vector)
{
	Vector3 temp(y*vector.z - z*vector.y,
				   z*vector.x - x*vector.z,
				   x*vector.y - y*vector.x);
	x = temp.x;
	y = temp.y;
	z = temp.z;

}

double Vector3::GetSqMagnitude()
{
	return x*x + y*y + z*z;
}

double Vector3::GetMagnitude()
{
	return sqrt(GetSqMagnitude());
}

void Vector3::Normalize()
{
	double length = GetMagnitude();
	if (length > 0)
	{
		*this *= (1.0/length);
	}

}

void Vector3::Clear()
{
	x = y = z = 0;
}

void Vector3::Invert()
{
	*this *= -1.0;
}