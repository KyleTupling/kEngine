#pragma once
#include <iostream>

class Vector2D
{
public:
    double x;
    double y;

    Vector2D();
    Vector2D(double x, double y);

    // Comparison operators
    bool operator==(const Vector2D& vec) const;
    bool operator!=(const Vector2D& vec) const;

    void Set(double newX, double newY);

    double GetMagnitude() const;
    double GetMagnitudeSqr() const;
    Vector2D GetUnitVector() const;

    inline static double Dot(const Vector2D& vec1, const Vector2D& vec2);
    inline static double Cross(const Vector2D& vec1, const Vector2D& vec2);

    // Addition with vector
    Vector2D Add(const Vector2D& vec) const;
    Vector2D operator+(const Vector2D& vec) const;

    // Subtraction with vector
    Vector2D Subtract(const Vector2D& vec) const;
    Vector2D operator-(const Vector2D& vec) const;

    // Multiplication by scalar
    Vector2D Multiply(double scalar) const;
    Vector2D operator*(double scalar) const; // vec * scalar syntax
    friend Vector2D operator*(double scalar, const Vector2D& vec); // scalar * vec syntax

    // Division by scalar
    Vector2D Divide(double scalar) const;
    Vector2D operator/(double scalar) const; // vec / scalar syntax
};