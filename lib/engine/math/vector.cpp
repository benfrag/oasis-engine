#include "engine/math/vector.h"

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{

}

float Vector3::dot(const Vector3& other)
{
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3& b)
{
    return Vector3(
        y * b.z - z * b.y,
        z * b.x - x * b.z,
        x * b.y - y * b.x
    );
}

float Vector3::magnitude()
{
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized()
{
    float mag = magnitude();
    // Guard against division by zero
    if (mag == 0) return Vector3(0, 0, 0);
    return Vector3(x / mag, y / mag, z / mag);
}
