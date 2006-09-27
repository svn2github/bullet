/*
Copyright (c) 2003-2006 Gino van den Bergen / Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/



#ifndef btTransform_H
#define btTransform_H

#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"



class btTransform {
	

public:
	
	enum { 
		TRANSLATION = 0x01,
		ROTATION    = 0x02,
		RIGID       = TRANSLATION | ROTATION,  
		SCALING     = 0x04,
		LINEAR      = ROTATION | SCALING,
		AFFINE      = TRANSLATION | LINEAR
	};

	btTransform() {}

	explicit SIMD_FORCE_INLINE btTransform(const btQuaternion& q, 
		const btVector3& c = btVector3(btScalar(0), btScalar(0), btScalar(0))) 
		: m_basis(q),
		m_origin(c),
		m_type(RIGID)
	{}

	explicit SIMD_FORCE_INLINE btTransform(const btMatrix3x3& b, 
		const btVector3& c = btVector3(btScalar(0), btScalar(0), btScalar(0)), 
		unsigned int type = AFFINE)
		: m_basis(b),
		m_origin(c),
		m_type(type)
	{}


		SIMD_FORCE_INLINE void mult(const btTransform& t1, const btTransform& t2) {
			m_basis = t1.m_basis * t2.m_basis;
			m_origin = t1(t2.m_origin);
			m_type = t1.m_type | t2.m_type;
		}

		void multInverseLeft(const btTransform& t1, const btTransform& t2) {
			btVector3 v = t2.m_origin - t1.m_origin;
			if (t1.m_type & SCALING) {
				btMatrix3x3 inv = t1.m_basis.inverse();
				m_basis = inv * t2.m_basis;
				m_origin = inv * v;
			}
			else {
				m_basis = btMultTransposeLeft(t1.m_basis, t2.m_basis);
				m_origin = v * t1.m_basis;
			}
			m_type = t1.m_type | t2.m_type;
		}

	SIMD_FORCE_INLINE btVector3 operator()(const btVector3& x) const
	{
		return btVector3(m_basis[0].dot(x) + m_origin[0], 
			m_basis[1].dot(x) + m_origin[1], 
			m_basis[2].dot(x) + m_origin[2]);
	}

	SIMD_FORCE_INLINE btVector3 operator*(const btVector3& x) const
	{
		return (*this)(x);
	}

	SIMD_FORCE_INLINE btMatrix3x3&       getBasis()          { return m_basis; }
	SIMD_FORCE_INLINE const btMatrix3x3& getBasis()    const { return m_basis; }

	SIMD_FORCE_INLINE btVector3&         getOrigin()         { return m_origin; }
	SIMD_FORCE_INLINE const btVector3&   getOrigin()   const { return m_origin; }

	btQuaternion getRotation() const { 
		btQuaternion q;
		m_basis.getRotation(q);
		return q;
	}
	template <typename Scalar2>
		void setValue(const Scalar2 *m) 
	{
		m_basis.setValue(m);
		m_origin.setValue(&m[12]);
		m_type = AFFINE;
	}

	
	void setFromOpenGLMatrix(const btScalar *m)
	{
		m_basis.setFromOpenGLSubMatrix(m);
		m_origin[0] = m[12];
		m_origin[1] = m[13];
		m_origin[2] = m[14];
	}

	void getOpenGLMatrix(btScalar *m) const 
	{
		m_basis.getOpenGLSubMatrix(m);
		m[12] = m_origin[0];
		m[13] = m_origin[1];
		m[14] = m_origin[2];
		m[15] = btScalar(1.0f);
	}

	SIMD_FORCE_INLINE void setOrigin(const btVector3& origin) 
	{ 
		m_origin = origin;
		m_type |= TRANSLATION;
	}

	SIMD_FORCE_INLINE btVector3 invXform(const btVector3& inVec) const;



	SIMD_FORCE_INLINE void setBasis(const btMatrix3x3& basis)
	{ 
		m_basis = basis;
		m_type |= LINEAR;
	}

	SIMD_FORCE_INLINE void setRotation(const btQuaternion& q)
	{
		m_basis.setRotation(q);
		m_type = (m_type & ~LINEAR) | ROTATION;
	}

	SIMD_FORCE_INLINE void scale(const btVector3& scaling)
	{
		m_basis = m_basis.scaled(scaling);
		m_type |= SCALING;
	}

	void setIdentity()
	{
		m_basis.setIdentity();
		m_origin.setValue(btScalar(0.0), btScalar(0.0), btScalar(0.0));
		m_type = 0x0;
	}

	SIMD_FORCE_INLINE bool isIdentity() const { return m_type == 0x0; }

	btTransform& operator*=(const btTransform& t) 
	{
		m_origin += m_basis * t.m_origin;
		m_basis *= t.m_basis;
		m_type |= t.m_type; 
		return *this;
	}

	btTransform inverse() const
	{ 
		if (m_type)
		{
			btMatrix3x3 inv = (m_type & SCALING) ? 
				m_basis.inverse() : 
			m_basis.transpose();

			return btTransform(inv, inv * -m_origin, m_type);
		}

		return *this;
	}

	btTransform inverseTimes(const btTransform& t) const;  

	btTransform operator*(const btTransform& t) const;

private:

	btMatrix3x3 m_basis;
	btVector3   m_origin;
	unsigned int      m_type;
};


SIMD_FORCE_INLINE btVector3
btTransform::invXform(const btVector3& inVec) const
{
	btVector3 v = inVec - m_origin;
	return (m_basis.transpose() * v);
}

SIMD_FORCE_INLINE btTransform 
btTransform::inverseTimes(const btTransform& t) const  
{
	btVector3 v = t.getOrigin() - m_origin;
	if (m_type & SCALING) 
	{
		btMatrix3x3 inv = m_basis.inverse();
		return btTransform(inv * t.getBasis(), inv * v, 
			m_type | t.m_type);
	}
	else 
	{
		return btTransform(m_basis.transposeTimes(t.m_basis),
			v * m_basis, m_type | t.m_type);
	}
}

SIMD_FORCE_INLINE btTransform 
btTransform::operator*(const btTransform& t) const
{
	return btTransform(m_basis * t.m_basis, 
		(*this)(t.m_origin), 
		m_type | t.m_type);
}	



#endif





