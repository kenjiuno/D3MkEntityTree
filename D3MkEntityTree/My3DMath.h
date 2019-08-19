
#pragma once

#include <math.h>

namespace My3DMath
{
	// 
	struct Vtx3 {
		// 
		union {
			// 
			struct {
				// 
				float x, y, z;
			};
			// 
			float v[3];
		};

		// 
		Vtx3()
		{

		}
		// 
		Vtx3(float x, float y, float z)
			: x(x)
			, y(y)
			, z(z)
		{

		}
		// 
		float Len() const;
		// 
		Vtx3 operator +(const Vtx3 &s) const
		{
			Vtx3 v;
			v.x = x + s.x;
			v.y = y + s.y;
			v.z = z + s.z;
			return v;
		}
		// 
		Vtx3 operator -(const Vtx3 &s) const
		{
			Vtx3 v;
			v.x = x - s.x;
			v.y = y - s.y;
			v.z = z - s.z;
			return v;
		}
		// 
		void Empty()
		{
			x = y = z = 0;
		}
		// 
		Vtx3 Lerp(const Vtx3 &v, float s) const
		{
			return *this + (v - *this).Mult(s);
		}
		// 
		Vtx3 Mult(float s) const
		{
			Vtx3 v = *this;
			v.x *= s;
			v.y *= s;
			v.z *= s;
			return v;
		}
		// 
		const Vtx3 &operator +=(const Vtx3 &s)
		{
			*this = *this + s;
			return s;
		}
		// 
		Vtx3 operator *(const Vtx3 &s) const
		{
			Vtx3 v = *this;
			v.x *= s.x;
			v.y *= s.y;
			v.z *= s.z;
			return v;
		}
		// 
		const Vtx3 &operator *=(const Vtx3 &s)
		{
			*this = *this * s;
			return s;
		}
		// 
		void Direct(float w)
		{
			x = w;
			y = w;
			z = w;
		}
		// 
		float DotProduct(const Vtx3 &s) const
		{
			return x*s.x + y*s.y + z*s.z;
		}
		// 
		void Normalize()
		{
			float cx = x*x + y*y + z*z;
			if (cx == 0)
				return;
			x /= cx;
			y /= cx;
			z /= cx;
		}
		// 
		Vtx3 CrossProduct(const Vtx3 &s) const
		{
			Vtx3 v;
			v.x = s.z * y - z * s.y;
			v.y = s.x * z - x * s.z;
			v.z = s.y * x - y * s.x;
			return v;
		}
		// 
		Vtx3 operator -() const
		{
			Vtx3 v;
			v.x = -x;
			v.y = -y;
			v.z = -z;
			return v;
		}
	};
	// 
	struct Vtx2 {
		// 
		union {
			// 
			struct {
				// 
				float x, y;
			};
			// 
			float v[2];
		};
		// 
		void Empty()
		{
			x = y = 0;
		}
	};
	// 
	struct BBox3 {
		// 
		Vtx3 vmin, vmax;

		// 
		BBox3()
		{

		}
		// 
		void Empty();
		// 
		void Add(const Vtx3 &s)
		{
			vmin.x = min(vmin.x, s.x);
			vmin.y = min(vmin.y, s.y);
			vmin.z = min(vmin.z, s.z);
			vmax.x = max(vmax.x, s.x);
			vmax.y = max(vmax.y, s.y);
			vmax.z = max(vmax.z, s.z);
		}
		// 
		Vtx3 Center()
		{
			return Vtx3(
				(vmin.x + vmax.x) / 2,
				(vmin.y + vmax.y) / 2,
				(vmin.z + vmax.z) / 2
				);
		}
		// 
		Vtx3 Cube()
		{
			return Vtx3(
				vmax.x - vmin.x,
				vmax.y - vmin.y,
				vmax.z - vmin.z
				);
		}
	};
	// 
	struct Quatern {
		// 
		union {
			// 
			struct {
				// 
				float x, y, z, w;
			};
			// 
			float v[4];
		};

		// 
		void Identity()
		{
			x = y = z = 0;
			w = 1;
		}
		// 
		Quatern &Identify(const Vtx3 &v);
	};
	// 
	struct Mtx {
		// 
		union {
			// 
			struct {
				// 
				float 
					_11,_12,_13,_14,
					_21,_22,_23,_24,
					_31,_32,_33,_34,
					_41,_42,_43,_44;
			};
			// 
			float v[16];
			// 
			float m[4][4];
		};

		// 
		void Identity()
		{
			for (char y=0; y<4; y++)
				for (char x=0; x<4; x++)
					m[y][x] = (x == y) ? 1.f : 0;
		}
		// 
		void RotationQuaternion(const Quatern &q)
		{
#define X (q.x)
#define Y (q.y)
#define Z (q.z)
#define W (q.w)
			_11 = 1 - 2*Y*Y - 2*Z*Z;
			_21 = 2*X*Y - 2*W*Z;
			_31 = 2*X*Z + 2*W*Y;
			_41 = 0;
			_12 = 2*X*Y + 2*W*Z;
			_22 = 1 - 2*X*X - 2*Z*Z;
			_32 = 2*Y*Z - 2*W*X;
			_42 = 0;
			_13 = 2*X*Z - 2*W*Y;
			_23 = 2*Y*Z + 2*W*X;
			_33 = 1 - 2*X*X - 2*Y*Y;
			_43 = 0;
			_14 = 0;
			_24 = 0;
			_34 = 0;
			_44 = 1;
#undef	X
#undef	Y
#undef	Z
#undef	W
		}

		// 
		Mtx operator +(const Mtx &r) const
		{
			Mtx m;
			for (char x=0; x<16; x++)
				m.v[x] = v[x] + r.v[x];

			return m;
		}
		// 
		Mtx operator -(const Mtx &r) const
		{
			Mtx m;
			for (char x=0; x<16; x++)
				m.v[x] = v[x] - r.v[x];

			return m;
		}
		// 
		Mtx operator *(const Mtx &r) const
		{
			Mtx m;
			for (char y=0; y<4; y++)
				for (char x=0; x<4; x++)
					m.m[y][x] = 0
						+(this->m[y][0]*r.m[0][x])
						+(this->m[y][1]*r.m[1][x])
						+(this->m[y][2]*r.m[2][x])
						+(this->m[y][3]*r.m[3][x])
						;

			return m;
		}
		// 
		const Mtx &operator *=(const Mtx &r)
		{
			*this = *this * r;

			return *this;
		}

		// 
		static void RotateX(Mtx &m, float r)
		{
			m.Identity();

			m._22 = m._33 = cos(r);
			m._23 = sin(r);
			m._32 = -sin(r);
		}
		// 
		static void RotateY(Mtx &m, float r)
		{
			m.Identity();

			m._11 = m._33 = cos(r);
			m._13 = -sin(r);
			m._31 = sin(r);
		}
		// 
		static void RotateZ(Mtx &m, float r)
		{
			m.Identity();

			m._11 = m._22 = cos(r);
			m._12 = sin(r);
			m._21 = -sin(r);
		}

		// 
		friend Vtx3 operator *(const Vtx3 &v, const Mtx &m)
		{
			Vtx3 v1;
			char a;
			for (a=0; a<3; a++)
				v1.v[a] = 0
					+(v.x * m.m[0][a])
					+(v.y * m.m[1][a])
					+(v.z * m.m[2][a])
					+(      m.m[3][a])
					;
			return v1;
		}
	};
};
