#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D() : x(0.0), y(0.0) {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

bool Vector2D::operator==(const Vector2D& vec) const
{
    return (x == vec.x && y == vec.y);
}

bool Vector2D::operator!=(const Vector2D& vec) const
{
    return !(*this == vec);
}

// Set x and y values of vector instance
void Vector2D::Set(double newX, double newY)
{
    this->x = newX;
    this->y = newY;
}

// Returns magnitude of vector instance
double Vector2D::GetMagnitude() const
{
    return std::sqrt(x * x + y * y);
}

// Returns squared magnitude of vector instance
// More efficient than GetMagnitude(), doesn't require sqrt
double Vector2D::GetMagnitudeSqr() const
{
    return x * x + y * y;
}

// Returns unit vector of vector instance
Vector2D Vector2D::GetUnitVector() const
{
    double mag = GetMagnitude();
    if (mag != 0) // Prevent division by zero
    {
        return Vector2D(x / mag, y / mag);
    }
    return Vector2D(); // Return zero vector if magnitude is zero
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