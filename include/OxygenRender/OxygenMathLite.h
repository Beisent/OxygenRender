#pragma once
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <random>

#ifdef DOUBLE_PRECISION
using real = double;
#else
using real = float;
#endif

namespace OxygenMathLite
{
    struct Vec2;
    struct Vec3;
    struct Vec4;

    inline Vec2 operator*(real scalar, const Vec2 &vec);
    inline Vec3 operator*(real scalar, const Vec3 &vec);
    inline Vec4 operator*(real scalar, const Vec4 &vec);

    namespace Constants
    {
#ifdef DOUBLE_PRECISION
        constexpr real PI = 3.141592653589793238462643383279502884;
        constexpr real Epsilon = 1e-12;
#else
        constexpr real PI = 3.14159265358979323846f;
        constexpr real Epsilon = 1e-6f;
#endif
        constexpr real TWO_PI = 2.0f * PI;
        constexpr real HALF_PI = 0.5f * PI;
        constexpr real DEG_TO_RAD = PI / 180.0f;
        constexpr real RAD_TO_DEG = 180.0f / PI;
    }

    // ====================== Vec2 ======================
    struct Vec2
    {
        real x = 0, y = 0;
        constexpr Vec2() : x(0), y(0) {}
        constexpr Vec2(real x, real y) : x(x), y(y) {}

        Vec2 operator+(const Vec2 &o) const { return {x + o.x, y + o.y}; }
        Vec2 operator-(const Vec2 &o) const { return {x - o.x, y - o.y}; }
        Vec2 operator*(real s) const { return {x * s, y * s}; }
        Vec2 operator/(real s) const { return {x / s, y / s}; }
        Vec2 operator-() const { return {-x, -y}; }

        Vec2 &operator+=(const Vec2 &o)
        {
            x += o.x;
            y += o.y;
            return *this;
        }
        Vec2 &operator-=(const Vec2 &o)
        {
            x -= o.x;
            y -= o.y;
            return *this;
        }
        Vec2 &operator*=(real s)
        {
            x *= s;
            y *= s;
            return *this;
        }
        Vec2 &operator/=(real s)
        {
            x /= s;
            y /= s;
            return *this;
        }

        real length() const { return std::sqrt(x * x + y * y); }
        real lengthSquared() const { return x * x + y * y; }
        Vec2 normalize() const
        {
            real len = length();
            return len < Constants::Epsilon ? Vec2{0, 0} : Vec2{x / len, y / len};
        }
        void normalizeSelf()
        {
            real len = length();
            if (len < Constants::Epsilon)
            {
                x = y = 0;
            }
            else
            {
                x /= len;
                y /= len;
            }
        }

        real dot(const Vec2 &o) const { return x * o.x + y * o.y; }
        real cross(const Vec2 &o) const { return x * o.y - y * o.x; }
        Vec2 perpendicular() const { return {-y, x}; }
        Vec2 rotate(real rads) const { return {x * std::cos(rads) - y * std::sin(rads), x * std::sin(rads) + y * std::cos(rads)}; }
        Vec2 project(const Vec2 &o) const
        {
            real len2 = o.lengthSquared();
            return len2 < Constants::Epsilon ? Vec2{0, 0} : (dot(o) / len2) * o;
        }
        Vec2 reflect(const Vec2 &normal) const
        {
            Vec2 n = normal.normalize();
            return *this - 2 * dot(n) * n;
        }
        void clear() { x = y = 0; }
        bool isZero() const { return x == 0 && y == 0; }
        bool isUnit() const { return std::fabs(lengthSquared() - 1) < Constants::Epsilon; }

        friend std::ostream &operator<<(std::ostream &os, const Vec2 &v)
        {
            std::ostringstream xs, ys;
            xs << v.x;
            ys << v.y;
            size_t w = std::max(xs.str().length(), ys.str().length()) + 3;
            os << "[" << std::setw(w) << v.x << "," << std::setw(w) << v.y << "]";
            return os;
        }
    };
    // ====================== Vec3 ======================
    struct Vec3
    {
        real x = 0.0f, y = 0.0f, z = 0.0f;
        constexpr Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
        constexpr Vec3(real x, real y, real z) : x(x), y(y), z(z) {}
        constexpr Vec3(const Vec3 &o) : x(o.x), y(o.y), z(o.z) {}
        constexpr Vec3(Vec3 &&o) : x(o.x), y(o.y), z(o.z) {}
        Vec3 &operator=(const Vec3 &o)
        {
            x = o.x;
            y = o.y;
            z = o.z;
            return *this;
        }
        Vec3 &operator=(Vec3 &&o)
        {
            x = o.x;
            y = o.y;
            z = o.z;
            return *this;
        }
        ~Vec3() = default;
        static Vec3 Zero() { return {0, 0, 0}; }
        static Vec3 One() { return {1, 1, 1}; }
        static Vec3 Up() { return {0, 1, 0}; }
        static Vec3 Down() { return {0, -1, 0}; }
        static Vec3 Left() { return {-1, 0, 0}; }
        static Vec3 Right() { return {1, 0, 0}; }
        static Vec3 Forward() { return {0, 0, 1}; }
        static Vec3 Backward() { return {0, 0, -1}; }
        Vec3 operator+(const Vec3 &o) const { return {x + o.x, y + o.y, z + o.z}; }
        Vec3 operator-(const Vec3 &o) const { return {x - o.x, y - o.y, z - o.z}; }
        Vec3 operator*(real s) const { return {x * s, y * s, z * s}; }
        Vec3 operator/(real s) const { return {x / s, y / s, z / s}; }
        Vec3 operator-() const { return {-x, -y, -z}; }
        inline bool operator==(const Vec3 &o) const { return x == o.x && y == o.y && z == o.z; }
        inline bool operator!=(const Vec3 &o) const { return !(*this == o); }
        Vec3 &operator+=(const Vec3 &o)
        {
            x += o.x;
            y += o.y;
            z += o.z;
            return *this;
        }
        Vec3 &operator-=(const Vec3 &o)
        {
            x -= o.x;
            y -= o.y;
            z -= o.z;
            return *this;
        }
        Vec3 &operator*=(real s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        Vec3 &operator/=(real s)
        {
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }
        real length() const { return std::sqrt(x * x + y * y + z * z); }
        real lengthSquared() const { return x * x + y * y + z * z; }
        Vec3 normalize() const
        {
            real len = length();
            if (len < Constants::Epsilon)
                return {0, 0, 0};
            return {x / len, y / len, z / len};
        }
        void normalizeSelf()
        {
            real len = length();
            if (len < Constants::Epsilon)
            {
                x = y = z = 0;
                return;
            }
            x /= len;
            y /= len;
            z /= len;
        }
        inline real dot(const Vec3 &o) const { return x * o.x + y * o.y + z * o.z; }
        Vec3 cross(const Vec3 &o) const { return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x}; }
        Vec3 reflect(const Vec3 &normal) const
        {
            Vec3 n = normal.normalize();
            return *this - 2 * this->dot(n) * n;
        }
        Vec3 project(const Vec3 &other) const
        {
            real lenSq = other.lengthSquared();
            if (lenSq < Constants::Epsilon)
                return {0, 0, 0};
            return (this->dot(other) / lenSq) * other;
        }
        void clear() { x = y = z = 0; }
        inline bool isZero() const { return x == 0 && y == 0 && z == 0; }
        inline bool isUnit() const { return std::fabs(lengthSquared() - 1) < Constants::Epsilon; }
        friend std::ostream &operator<<(std::ostream &os, const Vec3 &v)
        {
            std::ostringstream xs, ys, zs;
            xs << v.x;
            ys << v.y;
            zs << v.z;
            size_t width = std::max({xs.str().length(), ys.str().length(), zs.str().length()}) + 3;
            os << "[" << std::setw(width) << v.x << "," << std::setw(width) << v.y << "," << std::setw(width) << v.z << "]";
            return os;
        }
    };
    struct Vec4
    {
        real x = 0, y = 0, z = 0, w = 0;
        constexpr Vec4() : x(0), y(0), z(0), w(0) {}
        constexpr Vec4(real x, real y, real z, real w = 1.0f) : x(x), y(y), z(z), w(w) {}
        Vec4 operator+(const Vec4 &o) const { return {x + o.x, y + o.y, z + o.z, w + o.w}; }
        Vec4 operator-(const Vec4 &o) const { return {x - o.x, y - o.y, z - o.z, w - o.w}; }
        Vec4 operator*(real s) const { return {x * s, y * s, z * s, w * s}; }
        Vec4 operator/(real s) const { return {x / s, y / s, z / s, w / s}; }
        Vec4 &operator+=(const Vec4 &o)
        {
            x += o.x;
            y += o.y;
            z += o.z;
            w += o.w;
            return *this;
        }
        Vec4 &operator-=(const Vec4 &o)
        {
            x -= o.x;
            y -= o.y;
            z -= o.z;
            w -= o.w;
            return *this;
        }
        Vec4 &operator*=(real s)
        {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }
        real dot(const Vec4 &o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }
        real length() const { return std::sqrt(dot(*this)); }
        real lengthSquared() const { return dot(*this); }
        Vec4 normalize() const
        {
            real l = length();
            return l < Constants::Epsilon ? Vec4{0, 0, 0, 0} : (*this) / l;
        }
        void normalizeSelf()
        {
            real l = length();
            if (l < Constants::Epsilon)
            {
                x = y = z = w = 0;
            }
            else
            {
                x /= l;
                y /= l;
                z /= l;
                w /= l;
            }
        }
        friend std::ostream &operator<<(std::ostream &os, const Vec4 &v)
        {
            std::ostringstream xs, ys, zs, ws;
            xs << v.x;
            ys << v.y;
            zs << v.z;
            ws << v.w;
            size_t width = std::max({xs.str().length(), ys.str().length(), zs.str().length(), ws.str().length()}) + 2;
            os << "[" << std::setw(width) << v.x << "," << std::setw(width) << v.y << "," << std::setw(width) << v.z << "," << std::setw(width) << v.w << "]";
            return os;
        }
    };

    inline Vec2 operator*(real scalar, const Vec2 &v) { return v * scalar; }
    inline Vec3 operator*(real scalar, const Vec3 &v) { return v * scalar; }
    inline Vec4 operator*(real scalar, const Vec4 &v) { return v * scalar; }

    // ====================== Mat2 ======================
    struct Mat2
    {
        real m00 = 1, m01 = 0, m10 = 0, m11 = 1;
        Mat2() = default;
        Mat2(real a, real b, real c, real d) : m00(a), m01(b), m10(c), m11(d) {}
        static Mat2 Zero()
        {
            return Mat2(0, 0, 0, 0);
        }
        static Mat2 Identity()
        {
            return Mat2(1, 0, 0, 1);
        }
        static Mat2 Rotation(real rads)
        {
            real c = std::cos(rads), s = std::sin(rads);
            return {c, -s, s, c};
        }

        Mat2(const Mat2 &o) : m00(o.m00), m01(o.m01), m10(o.m10), m11(o.m11) {}
        Mat2(Mat2 &&o) : m00(o.m00), m01(o.m01), m10(o.m10), m11(o.m11) {}
        Mat2 &operator=(const Mat2 &o)
        {
            m00 = o.m00;
            m01 = o.m01;
            m10 = o.m10;
            m11 = o.m11;
            return *this;
        }
        Mat2 &operator=(Mat2 &&o)
        {
            m00 = o.m00;
            m01 = o.m01;
            m10 = o.m10;
            m11 = o.m11;
            return *this;
        }

        Vec2 operator*(const Vec2 &v) const { return {m00 * v.x + m01 * v.y, m10 * v.x + m11 * v.y}; }
        Mat2 operator*(const Mat2 &o) const { return {m00 * o.m00 + m01 * o.m10, m00 * o.m01 + m01 * o.m11, m10 * o.m00 + m11 * o.m10, m10 * o.m01 + m11 * o.m11}; }
        Mat2 operator*(const real &b) const
        {
            return {b * m00, b * m01, b * m10, b * m11};
        }
        Mat2 operator/(const real &b) const
        {
            return {m00 / b, m01 / b, m10 / b, m11 / b};
        }
        Mat2 operator+(const Mat2 &o) const
        {
            return {m00 + o.m00, m01 + o.m01, m10 + o.m10, m11 + o.m11};
        }
        Mat2 operator-(const Mat2 &o) const
        {
            return {m00 - o.m00, m01 - o.m01, m10 - o.m10, m11 - o.m11};
        }
        Mat2 &operator+=(const Mat2 &o)
        {
            m00 += o.m00;
            m01 += o.m01;
            m10 += o.m10;
            m11 += o.m11;
            return *this;
        }
        Mat2 &operator-=(const Mat2 &o)
        {
            m00 -= o.m00;
            m01 -= o.m01;
            m10 -= o.m10;
            m11 -= o.m11;
            return *this;
        }
        Mat2 &operator*=(const Mat2 &o)
        {
            *this = *this * o;
            return *this;
        }
        Mat2 &operator*=(const real &b)
        {
            m00 *= b;
            m01 *= b;
            m10 *= b;
            m11 *= b;
            return *this;
        }
        Mat2 &operator/=(const real &b)
        {
            m00 /= b;
            m01 /= b;
            m10 /= b;
            m11 /= b;
            return *this;
        }
        real Trace()
        {
            return m00 + m11;
        }
        real Det()
        {
            return m00 * m11 - m01 * m10;
        }
        Mat2 Transpose()
        {
            return {m00, m10, m01, m11};
        }
        Mat2 Inv()
        {
            real det = Det();
            if (std::abs(det) < Constants::Epsilon)
                throw("Matrix is singular");
            return Mat2(m11 / det, -m01 / det, -m10 / det, m00 / det);
        }

        friend std::ostream &operator<<(std::ostream &os, const Mat2 &m)
        {
            os << "[" << m.m00 << "," << m.m01 << "]\n"
               << "[" << m.m10 << "," << m.m11 << "]";
            return os;
        }
    };
    // ====================== Mat3 ======================
    struct Mat3
    {
        real m00 = 1, m01 = 0, m02 = 0;
        real m10 = 0, m11 = 1, m12 = 0;
        real m20 = 0, m21 = 0, m22 = 1;

        Mat3() = default;
        Mat3(real a, real b, real c,
             real d, real e, real f,
             real g, real h, real i)
            : m00(a), m01(b), m02(c),
              m10(d), m11(e), m12(f),
              m20(g), m21(h), m22(i) {}

        // 常用静态矩阵
        static Mat3 Zero()
        {
            return Mat3(0, 0, 0,
                        0, 0, 0,
                        0, 0, 0);
        }
        static Mat3 Identity()
        {
            return Mat3(1, 0, 0,
                        0, 1, 0,
                        0, 0, 1);
        }

        // 拷贝 / 移动
        Mat3(const Mat3 &o)
            : m00(o.m00), m01(o.m01), m02(o.m02),
              m10(o.m10), m11(o.m11), m12(o.m12),
              m20(o.m20), m21(o.m21), m22(o.m22) {}
        Mat3(Mat3 &&o)
            : m00(o.m00), m01(o.m01), m02(o.m02),
              m10(o.m10), m11(o.m11), m12(o.m12),
              m20(o.m20), m21(o.m21), m22(o.m22) {}
        Mat3 &operator=(const Mat3 &o)
        {
            m00 = o.m00;
            m01 = o.m01;
            m02 = o.m02;
            m10 = o.m10;
            m11 = o.m11;
            m12 = o.m12;
            m20 = o.m20;
            m21 = o.m21;
            m22 = o.m22;
            return *this;
        }
        Mat3 &operator=(Mat3 &&o)
        {
            m00 = o.m00;
            m01 = o.m01;
            m02 = o.m02;
            m10 = o.m10;
            m11 = o.m11;
            m12 = o.m12;
            m20 = o.m20;
            m21 = o.m21;
            m22 = o.m22;
            return *this;
        }

        // 运算符
        Vec3 operator*(const Vec3 &v) const
        {
            return {
                m00 * v.x + m01 * v.y + m02 * v.z,
                m10 * v.x + m11 * v.y + m12 * v.z,
                m20 * v.x + m21 * v.y + m22 * v.z};
        }
        Mat3 operator*(const Mat3 &o) const
        {
            return {
                m00 * o.m00 + m01 * o.m10 + m02 * o.m20,
                m00 * o.m01 + m01 * o.m11 + m02 * o.m21,
                m00 * o.m02 + m01 * o.m12 + m02 * o.m22,

                m10 * o.m00 + m11 * o.m10 + m12 * o.m20,
                m10 * o.m01 + m11 * o.m11 + m12 * o.m21,
                m10 * o.m02 + m11 * o.m12 + m12 * o.m22,

                m20 * o.m00 + m21 * o.m10 + m22 * o.m20,
                m20 * o.m01 + m21 * o.m11 + m22 * o.m21,
                m20 * o.m02 + m21 * o.m12 + m22 * o.m22};
        }
        Mat3 operator*(const real &b) const
        {
            return {m00 * b, m01 * b, m02 * b,
                    m10 * b, m11 * b, m12 * b,
                    m20 * b, m21 * b, m22 * b};
        }
        Mat3 operator/(const real &b) const
        {
            return {m00 / b, m01 / b, m02 / b,
                    m10 / b, m11 / b, m12 / b,
                    m20 / b, m21 / b, m22 / b};
        }
        Mat3 operator+(const Mat3 &o) const
        {
            return {m00 + o.m00, m01 + o.m01, m02 + o.m02,
                    m10 + o.m10, m11 + o.m11, m12 + o.m12,
                    m20 + o.m20, m21 + o.m21, m22 + o.m22};
        }
        Mat3 operator-(const Mat3 &o) const
        {
            return {m00 - o.m00, m01 - o.m01, m02 - o.m02,
                    m10 - o.m10, m11 - o.m11, m12 - o.m12,
                    m20 - o.m20, m21 - o.m21, m22 - o.m22};
        }

        Mat3 &operator+=(const Mat3 &o)
        {
            m00 += o.m00;
            m01 += o.m01;
            m02 += o.m02;
            m10 += o.m10;
            m11 += o.m11;
            m12 += o.m12;
            m20 += o.m20;
            m21 += o.m21;
            m22 += o.m22;
            return *this;
        }
        Mat3 &operator-=(const Mat3 &o)
        {
            m00 -= o.m00;
            m01 -= o.m01;
            m02 -= o.m02;
            m10 -= o.m10;
            m11 -= o.m11;
            m12 -= o.m12;
            m20 -= o.m20;
            m21 -= o.m21;
            m22 -= o.m22;
            return *this;
        }
        Mat3 &operator*=(const Mat3 &o)
        {
            *this = *this * o;
            return *this;
        }
        Mat3 &operator*=(const real &b)
        {
            m00 *= b;
            m01 *= b;
            m02 *= b;
            m10 *= b;
            m11 *= b;
            m12 *= b;
            m20 *= b;
            m21 *= b;
            m22 *= b;
            return *this;
        }
        Mat3 &operator/=(const real &b)
        {
            m00 /= b;
            m01 /= b;
            m02 /= b;
            m10 /= b;
            m11 /= b;
            m12 /= b;
            m20 /= b;
            m21 /= b;
            m22 /= b;
            return *this;
        }

        // 常用函数
        real Trace() const
        {
            return m00 + m11 + m22;
        }
        real Det() const
        {
            return m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);
        }
        Mat3 Transpose() const
        {
            return {
                m00, m10, m20,
                m01, m11, m21,
                m02, m12, m22};
        }
        Mat3 Inv() const
        {
            real det = Det();
            if (std::abs(det) < Constants::Epsilon)
                throw("Matrix is singular");

            return Mat3(
                (m11 * m22 - m12 * m21) / det,
                -(m01 * m22 - m02 * m21) / det,
                (m01 * m12 - m02 * m11) / det,

                -(m10 * m22 - m12 * m20) / det,
                (m00 * m22 - m02 * m20) / det,
                -(m00 * m12 - m02 * m10) / det,

                (m10 * m21 - m11 * m20) / det,
                -(m00 * m21 - m01 * m20) / det,
                (m00 * m11 - m01 * m10) / det);
        }
        friend std::ostream &operator<<(std::ostream &os, const Mat3 &m)
        {
            os << "[" << m.m00 << " " << m.m01 << "]\n"
               << "[" << m.m10 << " " << m.m11 << "]\n"
               << "[" << m.m20 << " " << m.m21 << "]\n";
            return os;
        }
    };
    // ====================== 工具函数 ======================
    namespace MathTools
    {
        inline real Clamp(real value, real min, real max) noexcept
        {
            return (value < min ? min : (value > max ? max : value));
        }
        inline real Lerp(real a, real b, real t) noexcept
        {
            return a + t * (b - a);
        }
        inline real ToRadians(real degrees) { return degrees * Constants::DEG_TO_RAD; }
        inline real ToDegrees(real radians) { return radians * Constants::RAD_TO_DEG; }

        template <typename T>
        void Swap(T &a, T &b) noexcept
        {
            T temp = a;
            a = b;
            b = temp;
        }

        inline real RandomRange(real min, real max)
        {
            static std::mt19937 rng(std::random_device{}());
            std::uniform_real_distribution<real> dist(min, max);
            return dist(rng);
        }

        inline Vec2 RandomUnitVector2()
        {
            real angle = MathTools::RandomRange(0, Constants::TWO_PI);
            return Vec2{std::cos(angle), std::sin(angle)};
        }
        inline Vec2 RandomInsideUnitCircle() { return RandomUnitVector2() * std::sqrt(MathTools::RandomRange(0, 1.0f)); }

    }

}