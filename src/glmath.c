/* \brief
 *		implementation of glmath.
 * 
 * \authro	jett huang
 * \date	2010-9-3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <assert.h>

#include "glmath.h"


/* copy */
void copy_matrix4(PtrMatrix4 dst, PtrMatrix4 src)
{
	memcpy(dst, src, sizeof(Matrix4));
}

void copy_vector4(PtrVector4 dst, PtrVector4 src)
{
	memcpy(dst, src, sizeof(Vector4));
}

void copy_vector3(PtrVector3 dst, PtrVector3 src)
{
	memcpy(dst, src, sizeof(Vector3));
}

/* \brief
 *		make a identity matrix4
 */
void make_identity_matrix(PtrMatrix4 m)
{
	m->_m[0][0] = 1.0; m->_m[0][1] = 0.0; m->_m[0][2] = 0.0; m->_m[0][3] = 0.0;
	m->_m[1][0] = 0.0; m->_m[1][1] = 1.0; m->_m[1][2] = 0.0; m->_m[1][3] = 0.0;
	m->_m[2][0] = 0.0; m->_m[2][1] = 0.0; m->_m[2][2] = 1.0; m->_m[2][3] = 0.0;
	m->_m[3][0] = 0.0; m->_m[3][1] = 0.0; m->_m[3][2] = 0.0; m->_m[3][3] = 1.0;
}

/* \brief
 *		check if a matrix is a identity matrix
 */
int is_identity_matrix(PtrMatrix4 m)
{
	int i, j;

	for (i=0; i<4; ++i)
		for (j=0; j<4; ++j)
		{
			if (i == j) {
				if (m->_m[i][j] != 1.0) return 0;
			} else {
				if (m->_m[i][j] != 0.0) return 0;
			}
		}
	
	return 1;
}

/* \brief
 *		transposition of matrix
 */
void transpose_matrix(PtrMatrix4 m, PtrMatrix4 result)
{
	int i, j;

	for (i=0; i<4; ++i)
		for (j=0; j<4; ++j)
		{
			result->_m[j][i] = m->_m[i][j];
		}
}

/* \brief
 *		get the sub-matrix3x3, excluding the row i, col j
 * \param
 *		m	(in)	the pointer to matrix4
 *		i	(in)	the number of row
 *		j   (in)	the number of column
 *		sub	(out)	sub-matrix
 * \comment
 *		the layout of sub is:
 *
 */
static
void get_sub3x3_matrix(PtrMatrix4 m, int i, int j, float sub[3][3])
{
	int ti, tj, idst, jdst;

	for (ti=0; ti<4; ++ti)
	{
		idst = (ti < i) ? ti : (ti - 1);
		for (tj=0; tj<4; ++tj)
		{
			jdst = (tj < j) ? tj : (tj - 1);
			if (ti != i && tj != j) {
				sub[idst][jdst] = m->_m[ti][tj];
			}
		} /* end for tj */
	} /* end for ti */
}

/* \brief
 *		calculate the determinant
 *  |                        |
 *  | m11,  m12,  m13,  m14  |
 *  | m21,  m22,  m23,  m24  | 
 *  | m31,  m32,  m33,  m34  |
 *  | m41,  m42,  m43,  m44  |
 *  |                        |
 *  Result is:
 *      m11 * (m22*(m33*m44 - m34*m43) + m23*(m34*m42 - m32*m44) + m24*(m32*m43 - m33*m42))
 *     -m12 * (m21*(m33*m44 - m34*m43) + m23*(m34*m41 - m31*m44) + m24*(m31*m43 - m33*m41))
 *     +m13 * (m21*(m32*m44 - m34*m42) + m22*(m34*m41 - m31*m44) + m24*(m31*m42 - m32*m41))
 *     -m14 * (m21*(m32*m43 - m33*m42) + m22*(m33*m41 - m31*m43) + m23*(m31*m42 - m32*m41))
 *
 */
static
float determinant_matrix(PtrMatrix4 m)
{
	float det, result, sign;
	float sub[3][3];
	int i;

	result = 0.0; sign = 1.0;

	for (i=0; i<4; ++i, sign *= -1.0)
	{
		get_sub3x3_matrix(m, 0, i, sub);
		det = sub[0][0] * (sub[1][1]*sub[2][2] - sub[1][2]*sub[2][1])
			 +sub[0][1] * (sub[2][0]*sub[1][2] - sub[1][0]*sub[2][2])
			 +sub[0][2] * (sub[1][0]*sub[2][1] - sub[2][0]*sub[1][1]);
		
		result += m->_m[0][i] * det * sign;
	}

	return result;
}

/* \brief
 *		invert a matrix
 *  |                        |
 *  | m11,  m12,  m13,  m14  |
 *  | m21,  m22,  m23,  m24  | 
 *  | m31,  m32,  m33,  m34  |
 *  | m41,  m42,  m43,  m44  |
 *  |                        |
 */
int invert_matrix(PtrMatrix4 m, PtrMatrix4 result)
{
	float det = determinant_matrix(m);
	float oneOverDet;
	float sub[3][3];
	int i, j, sign;
	
	/* assert del is not 0.0 */
	if (fabs(det) < 10e-6) {
#ifdef _DEBUG
		printf("INFO: invert_matrix, no inverse of matrix\n");
#endif
		return 1;
	}
	
	oneOverDet = 1.0f / det;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<4; ++j)
		{
			sign = 1 - ((i+j)%2) * 2;
			get_sub3x3_matrix(m, j, i, sub);
			
			det = sub[0][0] * (sub[1][1]*sub[2][2] - sub[1][2]*sub[2][1])
				 +sub[0][1] * (sub[2][0]*sub[1][2] - sub[1][0]*sub[2][2])
			     +sub[0][2] * (sub[1][0]*sub[2][1] - sub[2][0]*sub[1][1]);
			result->_m[i][j] = (float)sign * det * oneOverDet;
		}
	}

	return 0;
 }

/* \brief 
 *		make a rotation matrix
 * \param
 *		m		(in/out)	pointer to matrix
 *		theta	(in)		angle to rotation(in degree)
 *		v3		(in)		the axies to around
 * \comenet
 *
 *  | Nx*Nx*(1-cos(theta)) + cos(theta),    Nx*Ny*(1-cos(theta)) - Nz*sin(theta),  Nx*Nz*(1-cos(theta)) + Ny*sin(theta) |
 *	| Nx*Ny*(1-cos(theta)) + Nz*sin(theta), Ny*Ny*(1-cos(theta)) + cos(theta),     Ny*Nz*(1-cos(theta)) - Nx*sin(theta) |
 *	| Nx*Nz*(1-cos(theta)) - Ny*sin(theta), Ny*Nz*(1-cos(theta)) + Nx*sin(theta),  Nz*Nz*(1-cos(theta)) + cos(theta)    | 
 *
 */
void make_rotate_matrix(PtrMatrix4 m, float theta, PtrVector3 v3)
{
	float sintheta, costheta;
	Vector3 N = *v3;

	theta = theta * GL_PI / 180.0f;
	sintheta = (float)sin(theta);
	costheta = (float)cos(theta);
	normalize_vector3(&N);

	m->_m[0][0] = N._v[0] * N._v[0] * (1.0f - costheta) + costheta;
	m->_m[1][0] = N._v[0] * N._v[1] * (1.0f - costheta) + N._v[2] * sintheta;
	m->_m[2][0] = N._v[0] * N._v[2] * (1.0f - costheta) - N._v[1] * sintheta;
	m->_m[3][0] = 0.0;

	m->_m[0][1] = N._v[0] * N._v[1] * (1.0f - costheta) - N._v[2] * sintheta;
	m->_m[1][1] = N._v[1] * N._v[1] * (1.0f - costheta) + costheta;
	m->_m[2][1] = N._v[1] * N._v[2] * (1.0f - costheta) + N._v[0] * sintheta;
	m->_m[3][1] = 0.0;

	m->_m[0][2] = N._v[0] * N._v[2] * (1.0f - costheta) + N._v[1] * sintheta;
	m->_m[1][2] = N._v[1] * N._v[2] * (1.0f - costheta) - N._v[0] * sintheta;
	m->_m[2][2] = N._v[2] * N._v[2] * (1.0f - costheta) + costheta;
	m->_m[3][2] = 0.0f;

	m->_m[0][3] = 0.0f;
	m->_m[1][3] = 0.0f;
	m->_m[2][3] = 0.0f;
	m->_m[3][3] = 1.0f;
}

/* \brief
 *		make a scale matrix
 * \param
 *		m		(in/out)	pointer to matrix
 *		v3		(in)		a scale vector
 */
void make_scale_matrix(PtrMatrix4 m, PtrVector3 v3)
{
	m->_m[0][0] = v3->_v[0]; m->_m[0][1] = 0.0;       m->_m[0][2] = 0.0;       m->_m[0][3] = 0.0;
	m->_m[1][0] = 0.0;       m->_m[1][1] = v3->_v[1]; m->_m[1][2] = 0.0;       m->_m[1][3] = 0.0;
	m->_m[2][0] = 0.0;       m->_m[2][1] = 0.0;       m->_m[2][2] = v3->_v[2]; m->_m[2][3] = 0.0;
	m->_m[3][0] = 0.0;       m->_m[3][1] = 0.0;       m->_m[3][2] = 0.0;       m->_m[3][3] = 1.0;	
}

/* \brief
 *		make a transform matrix
 * \param
 *		m		(in/out)	pointer to matrix
 *		v3		(in)		transform
 */
void make_transform_matrix(PtrMatrix4 m, PtrVector3 v3)
{
	m->_m[0][0] = 1.0; m->_m[0][1] = 0.0; m->_m[0][2] = 0.0; m->_m[0][3] = v3->_v[0];
	m->_m[1][0] = 0.0; m->_m[1][1] = 1.0; m->_m[1][2] = 0.0; m->_m[1][3] = v3->_v[1];
	m->_m[2][0] = 0.0; m->_m[2][1] = 0.0; m->_m[2][2] = 1.0; m->_m[2][3] = v3->_v[2];
	m->_m[3][0] = 0.0; m->_m[3][1] = 0.0; m->_m[3][2] = 0.0; m->_m[3][3] = 1.0;
}

/* \brief
 *		multiply a matrix lhs by a another matrix rhs
 * \param
 *		lhs		(in)	pointer to matrix
 *		rhs		(in)	pointer to matrix
 *		result	(out)	pointer to matrix
 * \comment
 *		result can equal with lhs
 */
void multiply_matrix_by_matrix(PtrMatrix4 lhs, PtrMatrix4 rhs, PtrMatrix4 result)
{
	Matrix4 mat;
	int i, j, k;
	
	for (i=0; i<4; ++i)
	{
		for (j=0; j<4; ++j)
		{
			mat._m[i][j] = 0.0;
			for (k=0; k<4; ++k)
				mat._m[i][j] += lhs->_m[i][k] * rhs->_m[k][j];
		}
	}

	/* do copy */
	copy_matrix4(result, &mat);
}

/* \brief
 *		multiply a matrix by a scalar
 * \param
 *		lhs		(in)	pointer to matrix
 *		scalar	(in)	a scalar
 *		result	(out)	pointer to a matrix
 * \comment
 *		result can equal with m
 */
void multiply_matrix_by_scalar(PtrMatrix4 lhs, float scalar, PtrMatrix4 result)
{
	result->_m[0][0] = lhs->_m[0][0] * scalar; 
	result->_m[0][1] = lhs->_m[0][1] * scalar;
	result->_m[0][2] = lhs->_m[0][2] * scalar;
	result->_m[0][3] = lhs->_m[0][3] * scalar;

	result->_m[1][0] = lhs->_m[1][0] * scalar;
	result->_m[1][1] = lhs->_m[1][1] * scalar;
	result->_m[1][2] = lhs->_m[1][2] * scalar;
	result->_m[1][3] = lhs->_m[1][3] * scalar;

	result->_m[2][0] = lhs->_m[2][0] * scalar;
	result->_m[2][1] = lhs->_m[2][1] * scalar;
	result->_m[2][2] = lhs->_m[2][2] * scalar;
	result->_m[2][3] = lhs->_m[2][3] * scalar;

	result->_m[3][0] = lhs->_m[3][0] * scalar;
	result->_m[3][1] = lhs->_m[3][1] * scalar;
	result->_m[3][2] = lhs->_m[3][2] * scalar;
	result->_m[3][3] = lhs->_m[3][3] * scalar;
}

/* \brief
 *		multiply a matrix with a vector3
 * \param
 *		lhs		(in)	pointer to matrix
 *		v3		(in)	pointer to vector3
 *		result	(out)	pointer to vector3
 */
void multiply_matrix_by_vector3(PtrMatrix4 lhs, PtrVector3 v3, PtrVector3 result)
{
	Vector3 v;

	v._v[0] = lhs->_m[0][0] * v3->_v[0] + lhs->_m[0][1] * v3->_v[1] + lhs->_m[0][2] * v3->_v[2];
	v._v[1] = lhs->_m[1][0] * v3->_v[0] + lhs->_m[1][1] * v3->_v[1] + lhs->_m[1][2] * v3->_v[2];
	v._v[2] = lhs->_m[2][0] * v3->_v[0] + lhs->_m[2][1] * v3->_v[1] + lhs->_m[2][2] * v3->_v[2];

	copy_vector3(result, &v);
}

/* \brief
 *		multiply a matrix with a vector4
 * \param
 *		lhs		(in)	pointer to matrix
 *		v4		(in)	pointer to vector4
 *		result	(out)	pointer to vector4
 */
void multiply_matrix_by_vector4(PtrMatrix4 lhs, PtrVector4 v4, PtrVector4 result)
{
	Vector4 v;

	v._v[0] = lhs->_m[0][0] * v4->_v[0] + lhs->_m[0][1] * v4->_v[1] + lhs->_m[0][2] * v4->_v[2] + lhs->_m[0][3] * v4->_v[3];
	v._v[1] = lhs->_m[1][0] * v4->_v[0] + lhs->_m[1][1] * v4->_v[1] + lhs->_m[1][2] * v4->_v[2] + lhs->_m[1][3] * v4->_v[3];
	v._v[2] = lhs->_m[2][0] * v4->_v[0] + lhs->_m[2][1] * v4->_v[1] + lhs->_m[2][2] * v4->_v[2] + lhs->_m[2][3] * v4->_v[3];
	v._v[3] = lhs->_m[3][0] * v4->_v[0] + lhs->_m[3][1] * v4->_v[1] + lhs->_m[3][2] * v4->_v[2] + lhs->_m[3][3] * v4->_v[3];

	copy_vector4(result, &v);
}

/* \brief 
 *		cross product of two vector3
 * \param
 *		lhs		(in)	pointer to vector3
 *		rhs		(in)	pointer to vector3
 *		result	(out)	pointre to vector3
 * \comment
 *		lhs	can equal with result
 */
void cross_vector3(PtrVector3 lhs, PtrVector3 rhs, PtrVector3 result)
{
	Vector3 v3;

	v3._v[0] = lhs->_v[1] * rhs->_v[2] - lhs->_v[2] * rhs->_v[1];
	v3._v[1] = lhs->_v[2] * rhs->_v[0] - lhs->_v[0] * rhs->_v[2];
	v3._v[2] = lhs->_v[0] * rhs->_v[1] - lhs->_v[1] * rhs->_v[0];

	copy_vector3(result, &v3);
}

/* \brief
 *		dot product of two vector3
 * \param
 *		lhs		(in)	pointer to vector3
 *		rhs		(in)	pointer to vector3
 * \ret
 *		return the dot product
 */
float dot_vector3(PtrVector3 lhs, PtrVector3 rhs)
{
	return lhs->_v[0] * rhs->_v[0] + lhs->_v[1] * rhs->_v[1] + lhs->_v[2] * rhs->_v[2];
}

/* \brief
 *		normalize the vector3
 * \param
 *		v3		(in)	pointer to vector3
 */
void normalize_vector3(PtrVector3 v3)
{
	float mag;

	mag = (float)sqrt(v3->_v[0] * v3->_v[0] + v3->_v[1] * v3->_v[1] + v3->_v[2] * v3->_v[2]);
	assert(mag > 0.0f);
	/* get the 1.0 / mag */
	mag = 1.0f / mag;

	v3->_v[0] *= mag;
	v3->_v[1] *= mag;
	v3->_v[2] *= mag;
}

/* \brief 
 *      vector3 subtract operation
 */
void sub_vector3(PtrVector3 lhs, PtrVector3 rhs, PtrVector3 res)
{
    res->_v[0] = lhs->_v[0] - rhs->_v[0];
    res->_v[1] = lhs->_v[1] - rhs->_v[1];
    res->_v[2] = lhs->_v[2] - rhs->_v[2];
}

/* \brief 
 *      vector3 add operation
 */
void add_vector3(PtrVector3 lhs, PtrVector3 rhs, PtrVector3 res)
{
    res->_v[0] = lhs->_v[0] + rhs->_v[0];
    res->_v[1] = lhs->_v[1] + rhs->_v[1];
    res->_v[2] = lhs->_v[2] + rhs->_v[2];
}

/*
 */
int invert_matrix_second(PtrMatrix4 mat, PtrMatrix4 result)
{
	int i,j,k,l, n;
	float max,tmp,t;

	 
	float *m, *r;

	m = &(mat->_m[0][0]);
	r = &(result->_m[0][0]);
	n = 4;

	/* identity the matrix */
	for(i=0;i<n*n;i++) r[i]=0.0;
	for(i=0;i<n;i++) r[i*n+i]=1.0;

	for(j=0;j<n;j++) {
		 /* recherche du nombre de plus grand module sur la colonne j */
		 max=m[j*n+j];
		 k=j;
		 for(i=j+1;i<n;i++)	
			if (fabs(m[i*n+j])>fabs(max)) {		 
				k=i; 
				max=m[i*n+j];
			}
		 /* non intersible matrix */
		 if (max==0.0) return 1;

		 /* permutation des lignes j et k */
		 if (k!=j) {
			for(i=0;i<n;i++) {		
				tmp=m[j*n+i];	
				m[j*n+i]=m[k*n+i];
				m[k*n+i]=tmp;
				tmp=r[j*n+i];	
				r[j*n+i]=r[k*n+i];
				r[k*n+i]=tmp;
			}
		 }
			
		/* multiplication de la ligne j par 1/max */
		max=1/max;
		for(i=0;i<n;i++) {
			m[j*n+i]*=max;
			r[j*n+i]*=max;
		}
			
		for(l=0;l<n;l++) if (l!=j) {
			t=m[l*n+j];
			for(i=0;i<n;i++) {
				m[l*n+i]-=m[j*n+i]*t;
				r[l*n+i]-=r[j*n+i]*t;
			}
		}
	}

	return 0;
}
