#pragma once
#include <math.h>

#define PI 3.14159265358f
#define DEG_TO_RAD(deg) ((PI / 180.0f) * deg)
#define TWO_THIRDSF (2.0 / 3.0f)

template <class T>
struct Vector3d;

template <class T, class Y>
T Lerp(T x, T y, Y t);

template <class T>
struct Vertex2d
{
	T x;
	T y;

	Vertex2d()
	{
	}

	Vertex2d(T x, T y)
	{
		this->x = x;
		this->y = y;
	}
};

template <class T>
struct Triangle2d
{
	Vertex2d<T> p1;
	Vertex2d<T> p2;
	Vertex2d<T> p3;

	Triangle2d()
	{
	}

	Triangle2d(const Vertex2d<T>& p1, const Vertex2d<T>& p2, const Vertex2d<T>& p3)
	{
		this.p1 = p1;
		this.p2 = p2;
		this.p3 = p3;
	}

	Triangle2d(T p1x, T p1y, T p2x, T p2y, T p3x, T p3y)
	{
		p1.x = p1x;
		p1.y = p1y;
		p2.x = p2x;
		p2.y = p2y;
		p3.x = p3x;
		p3.y = p3y;
	}
};

template <class T>
struct Vertex3d
{
	T x;
	T y;
	T z;

	Vertex3d()
	{
	}

	Vertex3d(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vertex3d(const Vector3d<T>& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	Vector3d<T> operator+(const Vertex3d<T>& other) const
	{
		return Vector3d<T>(x + other.x, y + other.y, z + other.z);
	}

	Vector3d<T> operator-(const Vertex3d<T>& other) const
	{
		return Vector3d<T>(x - other.x, y - other.y, z - other.z);
	}

	Vector3d<T> operator*(const T& scalar) const
	{
		return Vector3d<T>(x * scalar, y * scalar, x * scalar);
	}

	Vertex3d<T> operator-() const
	{
		return Vector3d<T>(-x, -y, -z);
	}
};

template <class T>
struct Vertex4d
{
	T x;
	T y;
	T z;
	T w;

	Vertex4d()
	{
	}

	Vertex4d(T x, T y, T z, T w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vertex4d(const Vertex3d<T>& vert, T w) : Vertex4d(vert.x, vert.y, vert.z, w)
	{
	}

	void DivideByW()
	{
		if (w != 0.0f)
		{
			x /= w;
			y /= w;
			z /= w;
		}
	}
};

template <class T>
struct Triangle3d
{
	Vertex3d<T> p1;
	Vertex3d<T> p2;
	Vertex3d<T> p3;

	Triangle3d()
	{
	}

	Triangle3d(const Vertex3d<T>& p1, const Vertex3d<T>& p2, const Vertex3d<T>& p3)
	{
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}

	Triangle3d(T p1x, T p1y, T p1z, T p2x, T p2y, T p2z, T p3x, T p3y, T p3z)
	{
		p1.x = p1x;
		p1.y = p1y;
		p1.z = p1z;
		p2.x = p2x;
		p2.y = p2y;
		p2.z = p2z;
		p3.x = p3x;
		p3.y = p3y;
		p3.z = p3z;
	}

	Vector3d<T> GetNormal()
	{
		return Vector3d<T>({ p2.x - p1.x, p2.y - p1.y, p2.z - p1.z })
			.Cross({ p3.x - p2.x, p3.y - p2.y, p3.z - p2.z })
			.Normalize();
	}

	Vertex3d<T> GetCenter()
	{
		Vertex3d<T> edgeCenter;
		edgeCenter.x = Lerp(p1.x, p2.x, .5f);
		edgeCenter.y = Lerp(p1.y, p2.y, .5f);
		edgeCenter.z = Lerp(p1.z, p2.z, .5f);

		Vertex3d<T> centerPoint = 
		{
			Lerp(p3.x, edgeCenter.x, TWO_THIRDSF),
			Lerp(p3.y, edgeCenter.y, TWO_THIRDSF),
			Lerp(p3.z, edgeCenter.z, TWO_THIRDSF)
		};

		return centerPoint;
	}
};

template <class T>
struct Vector2d
{
	T x;
	T y;

	Vector2d()
	{
	}

	Vector2d(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2d(const Vertex2d<T>& vert)
	{
		x = vert.x;
		y = vert.y;
	}

	T Length()
	{
		return sqrt(x * x + y * y);
	}

	Vector2d<T>& Normalize()
	{
		T length = Length();
		if (length != 0)
		{
			x /= length;
			y /= length;
		}

		return *this;
	}

	Vector2d<T> Cross(const Vector2d<T>& other)
	{
		//TODO:
		return Vector2d<T>();
		/*return Vector3d<T>(
			(y * other.z) - (z * other.y),
			(z * other.x) - (x * other.z),
			(x * other.y) - (y * other.x));*/
	}

	T Dot(const Vector2d<T>& other)
	{
		return x * other.x + y * other.y;
	}

	Vector2d<T>& operator /=(T scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vector2d<T> operator-(const Vector2d<T>& other)
	{
		Vector2d<T> temp = { x - other.x, y - other.y };
		return temp;
	}
};

template <class T>
struct Vector3d
{
	T x;
	T y;
	T z;

	Vector3d()
	{
	}

	Vector3d(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3d(const Vertex3d<T>& vert)
	{
		x = vert.x;
		y = vert.y;
		z = vert.z;
	}

	T Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3d<T>& Normalize()
	{
		T length = Length();
		if (length != 0)
		{
			x /= length;
			y /= length;
			z /= length;
		}

		return *this;
	}

	Vector3d<T> Cross(const Vector3d<T>& other)
	{
		return Vector3d<T>(
			(y * other.z) - (z * other.y),
			(z * other.x) - (x * other.z),
			(x * other.y) - (y * other.x));
	}

	T Dot(const Vector3d<T>& other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3d<T> operator+(const Vector3d<T>& other) const
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	Vector3d<T> operator-(const Vector3d<T>& other) const
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	Vector3d<T> operator*(const T& sclar) const
	{
		return { x * sclar, y * sclar, z * sclar };
	}

	Vector3d<T> operator-() const
	{
		return { -x, -y, -z };
	}
};

template <class T>
struct Matrix4x4
{
	union
	{
		T a[4][4];
		T f[16];
	};

	Matrix4x4() = default;

	Matrix4x4(const Matrix4x4<T>& other)
	{
		memcpy(a, other.a, sizeof(f));
	}

	Matrix4x4(const T elements[16])
	{
		memcpy(a, elements, sizeof(f));
	}

	void Zero()
	{
		memset(a, 0, sizeof(f));
	}

	void Identity()
	{
		Zero();
		a[0][0] = 1;
		a[1][1] = 1;
		a[2][2] = 1;
		a[3][3] = 1;
	}

	void SetTranslation(const Vertex3d<T>& position)
	{
		a[3][0] = position.x;
		a[3][1] = position.y;
		a[3][2] = position.z;
	}

	Vertex3d<T> GetTranslation()
	{
		return Vertex3d<T>(a[3][0], a[3][1], a[3][2]);
	}

	void SetRotationX(T angle)
	{
		T sin = std::sin(angle);
		T cos = std::cos(angle);

		a[1][1] = cos;
		a[1][2] = sin;
		a[2][1] = -sin;
		a[2][2] = cos;
	}

	void SetRotationY(T angle)
	{
		T sin = std::sin(angle);
		T cos = std::cos(angle);

		a[0][0] = cos;
		a[0][2] = -sin;
		a[2][0] = sin;
		a[2][2] = cos;
	}

	void SetRotationZ(T angle)
	{
		T sin = std::sin(angle);
		T cos = std::cos(angle);

		a[0][0] = cos;
		a[0][1] = sin;
		a[1][0] = -sin;
		a[1][1] = cos;
	}

	void SetScaleX(T scale)
	{
		a[0][0] = scale;
	}

	void SetScaleY(T scale)
	{
		a[1][1] = scale;
	}

	void SetScaleZ(T scale)
	{
		a[2][2] = scale;
	}

	void Transpose()
	{
		T temp;
		temp = f[1];
		f[1] = f[4];
		f[4] = temp;

		temp = f[2];
		f[2] = f[8];
		f[2] = temp;

		temp = f[3];
		f[3] = f[12];
		f[12] = temp;

		temp = f[6];
		f[6] = f[9];
		f[9] = temp;

		temp = f[7];
		f[7] = f[13];
		f[13] = temp;

		temp = f[11];
		f[11] = f[14];
		f[14] = temp;
	}

	inline T& operator()(int x, int y)
	{
		return a[x][y];
	}

	inline T& operator()(int x)
	{
		return ((T*)a)[x];
	}

	/*(function() {
	const getI = (name, x, y) => name + '[' + x + '][' + y + ']';
	for (let x = 0; x < 4; x++)
	for (let y = 0; y < 4; y++)
	{

	let mult = '';
	for (let i = 0; i < 4; i++)
	{
	if (mult != '') mult += ' + ';
	mult += getI('a', x, i) + ' * ' + getI('o.a', i, y);
	}
	let out = getI('temp.a', x, y) + ' = ' + mult + ';';
	console.log(out);
	}
	}())*/

	Matrix4x4<T> operator*(const Matrix4x4<T>& other) const
	{
		const Matrix4x4<T>& o = other;
		Matrix4x4<T> temp;
		temp.a[0][0] = a[0][0] * o.a[0][0] + a[0][1] * o.a[1][0] + a[0][2] * o.a[2][0] + a[0][3] * o.a[3][0];
		temp.a[0][1] = a[0][0] * o.a[0][1] + a[0][1] * o.a[1][1] + a[0][2] * o.a[2][1] + a[0][3] * o.a[3][1];
		temp.a[0][2] = a[0][0] * o.a[0][2] + a[0][1] * o.a[1][2] + a[0][2] * o.a[2][2] + a[0][3] * o.a[3][2];
		temp.a[0][3] = a[0][0] * o.a[0][3] + a[0][1] * o.a[1][3] + a[0][2] * o.a[2][3] + a[0][3] * o.a[3][3];
		temp.a[1][0] = a[1][0] * o.a[0][0] + a[1][1] * o.a[1][0] + a[1][2] * o.a[2][0] + a[1][3] * o.a[3][0];
		temp.a[1][1] = a[1][0] * o.a[0][1] + a[1][1] * o.a[1][1] + a[1][2] * o.a[2][1] + a[1][3] * o.a[3][1];
		temp.a[1][2] = a[1][0] * o.a[0][2] + a[1][1] * o.a[1][2] + a[1][2] * o.a[2][2] + a[1][3] * o.a[3][2];
		temp.a[1][3] = a[1][0] * o.a[0][3] + a[1][1] * o.a[1][3] + a[1][2] * o.a[2][3] + a[1][3] * o.a[3][3];
		temp.a[2][0] = a[2][0] * o.a[0][0] + a[2][1] * o.a[1][0] + a[2][2] * o.a[2][0] + a[2][3] * o.a[3][0];
		temp.a[2][1] = a[2][0] * o.a[0][1] + a[2][1] * o.a[1][1] + a[2][2] * o.a[2][1] + a[2][3] * o.a[3][1];
		temp.a[2][2] = a[2][0] * o.a[0][2] + a[2][1] * o.a[1][2] + a[2][2] * o.a[2][2] + a[2][3] * o.a[3][2];
		temp.a[2][3] = a[2][0] * o.a[0][3] + a[2][1] * o.a[1][3] + a[2][2] * o.a[2][3] + a[2][3] * o.a[3][3];
		temp.a[3][0] = a[3][0] * o.a[0][0] + a[3][1] * o.a[1][0] + a[3][2] * o.a[2][0] + a[3][3] * o.a[3][0];
		temp.a[3][1] = a[3][0] * o.a[0][1] + a[3][1] * o.a[1][1] + a[3][2] * o.a[2][1] + a[3][3] * o.a[3][1];
		temp.a[3][2] = a[3][0] * o.a[0][2] + a[3][1] * o.a[1][2] + a[3][2] * o.a[2][2] + a[3][3] * o.a[3][2];
		temp.a[3][3] = a[3][0] * o.a[0][3] + a[3][1] * o.a[1][3] + a[3][2] * o.a[2][3] + a[3][3] * o.a[3][3];
		return temp;
	}

	Vertex4d<T> operator*(const Vertex4d<T>& vert) const
	{
		Vertex4d<T> temp;
		temp.x = (a[0][0] * vert.x) + (a[1][0] * vert.y) + (a[2][0] * vert.z) + (a[3][0] * vert.w);
		temp.y = (a[0][1] * vert.x) + (a[1][1] * vert.y) + (a[2][1] * vert.z) + (a[3][1] * vert.w);
		temp.z = (a[0][2] * vert.x) + (a[1][2] * vert.y) + (a[2][2] * vert.z) + (a[3][2] * vert.w);
		temp.w = (a[0][3] * vert.x) + (a[1][3] * vert.y) + (a[2][3] * vert.z) + (a[3][3] * vert.w);
		return temp;
	}

	Matrix4x4<T>& operator=(const Matrix4x4<T>& other)
	{
		memcpy(a, other.a, sizeof(a));
		return *this;
	}

	Matrix4x4<T>& operator*=(const Matrix4x4<T>& other)
	{
		return *this = *this * other;
	}
};

typedef Vertex2d<float> Vert2df;
typedef Vertex2d<int> Vert2di;

typedef Vertex3d<float> Vert3df;
typedef Vertex3d<int> Vert3di;

typedef Vertex4d<float> Vert4df;
typedef Vertex4d<int> Vert4di;

typedef Triangle2d<float> Tri2df;
typedef Triangle2d<int>   Tri2di;

typedef Triangle3d<float> Tri3df;
typedef Triangle3d<int>   Tri3di;

typedef Vector2d<float>	  Vec2df;
typedef Vector2d<int>     Vec2di;

typedef Vector3d<float>	  Vec3df;
typedef Vector3d<int>     Vec3di;

typedef Matrix4x4<float> Mat4x4f;

template <class T, class Y = float>
T Lerp(T x, T y, Y t)
{
	return static_cast<T>(x + t * (y - x));
}

inline bool FloatEquals(float a, float b, float epsilon = 0.000001f)
{
	return fabs(a - b) <= epsilon;
}