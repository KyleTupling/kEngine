#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2D::Vector2D(double x, double y)
{
    this->x = x;
    this->y = y;
}

// Set x and y values of vector instance
void Vector2D::Set(double newX, double newY)
{
    this->x = newX;
    this->y = newY;
}

// Returns magnitude of vector instance
double Vector2D::GetMagnitude()
{
    return sqrt(this->x * this->x + this->y * this->y);
}

// Returns squared magnitude of vector instance
// More efficient than GetMagnitude(), doesn't require sqrt
double Vector2D::GetMagnitudeSqr()
{
    return (this->x * this->x + this->y * this->y);
}

// Returns unit vector of vector instance
Vector2D Vector2D::GetUnitVector()
{
    double mag = sqrt(this->x * this->x + this->y * this->y);
    Vector2D unitVec = this->Divide(1000);
    return unitVec;
}

Vector2D Vector2D::Add(Vector2D vec1, Vector2D vec2)
{
    Vector2D newVec(vec1.x + vec2.x, vec1.y + vec2.y);
    return newVec;
}

Vector2D Vector2D::Subtract(Vector2D vec1, Vector2D vec2)
{
    Vector2D newVec(vec1.x - vec2.x, vec1.y - vec2.y);
    return newVec;
}

Vector2D Vector2D::Add(const Vector2D& vec) const
{
    return Vector2D(this->x + vec.x, this->y + vec.y);
}

Vector2D Vector2D::operator+(const Vector2D& vec) const
{
    return this->Add(vec);
}

Vector2D Vector2D::Subtract(const Vector2D& vec) const
{
    return Vector2D(this->x - vec.x, this->y - vec.y);
}

Vector2D Vector2D::operator-(const Vector2D& vec) const
{
    return this->Subtract(vec);
}

Vector2D Vector2D::Multiply(double scalar) const
{
    return Vector2D(this->x * scalar, this->y * scalar);
}

Vector2D Vector2D::operator*(double scalar) const 
{
    return this->Multiply(scalar);
}

Vector2D operator*(double scalar, const Vector2D& vec)
{
    return vec.Multiply(scalar);
}

Vector2D Vector2D::Divide(double scalar) const
{
    return Vector2D(this->x / scalar, this->y / scalar);
}

Vector2D Vector2D::operator/(double scalar) const
{
    return this->Divide(scalar);
}

Vector2D operator/(double scalar, const Vector2D& vec)
{
    return vec.Divide(scalar);
}