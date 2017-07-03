/* \brief
 *      About 3D math. the layout of matrix is using colum major order.
 *      
 *      m11   m12   m13   m14              m[0]   m[4]   m[8]   m[12]
 *      m21   m22   m23   m24      --->    m[1]   m[5]   m[9]   m[13]
 *      m31   m32   m33   m34              m[2]   m[6]   m[10]  m[14]
 *      m41   m42   m43   m44              m[3]   m[7]   m[11]  m[15]
 *
 *
 * \author  jett huang
 * \date    2010-9-3
 *
 * \change log
 *
 *
 */

#ifndef __GL_MATH_H__
#define __GL_MATH_H__


#define GL_PI	3.1415926535897932384626433832795f

/* Matrix & Vector */
typedef struct {
    float _m[4][4];
}Matrix4, *PtrMatrix4;

typedef struct {
    float _v[4];
}Vector4, *PtrVector4;

typedef struct {
        float _v[3];
}Vector3, *PtrVector3;

/* copy */
void copy_matrix4(PtrMatrix4 dst, PtrMatrix4 src);
void copy_vector4(PtrVector4 dst, PtrVector4 src);
void copy_vector3(PtrVector3 dst, PtrVector3 src);

/* \brief
 *      make a identity matrix4
 * \param
 *      m   (in/out)    pointer to matrix
 */
void make_identity_matrix(PtrMatrix4 m);

/* \brief
 *      check if a matrix is a identity matrix
 * \param
 *      m   (in)    pointer to matrix
 * \ret
 *      return 0 if not identity, otherwise return none-zero.
 */
int is_identity_matrix(PtrMatrix4 m);

/* \brief
 *      transposition of matrix
 * \param
 *      m		(in)    pointer to matrix
 *      result	(out)	pointer to matrix
 * \comment
 *		m is not equal with result
 *
 */
void transpose_matrix(PtrMatrix4 m, PtrMatrix4 result);

/* \brief
 *      invert a matrix
 * \param
 *      m		(in)    pointer to matrix
 *		result	(in)	pointer to matrix
 * \ret
 *		return 0 if succeed, otherwise rehturn non-zeor.
 * \comment
 *		m is not equal with result
 */
int invert_matrix(PtrMatrix4 m, PtrMatrix4 result);

/* \brief 
 *      make a rotation matrix
 * \param
 *      m       (in/out)    pointer to matrix
 *      theta   (in)        angle to rotation(in degree)
 *      v3      (in)        the axies to around
 */
void make_rotate_matrix(PtrMatrix4 m, float theta, PtrVector3 v3);

/* \brief
 *      make a scale matrix
 * \param
 *      m       (in/out)    pointer to matrix
 *      v3      (in)        a scale vector
 */
void make_scale_matrix(PtrMatrix4 m, PtrVector3 v3);

/* \brief
 *      make a transform matrix
 * \param
 *      m       (in/out)    pointer to matrix
 *      v3      (in)        transform
 */
void make_transform_matrix(PtrMatrix4 m, PtrVector3 v3);

/* \brief
 *      multiply a matrix lhs by a another matrix rhs
 * \param
 *      lhs     (in)    pointer to matrix
 *      rhs     (in)    pointer to matrix
 *      result  (out)   pointer to matrix
 * \comment
 *      result can equal with lhs
 */
void multiply_matrix_by_matrix(PtrMatrix4 lhs, PtrMatrix4 rhs, PtrMatrix4 result);

/* \brief
 *      multiply a matrix by a scalar
 * \param
 *      lhs     (in)    pointer to matrix
 *      scalar  (in)    a scalar
 *      result  (out)   pointer to a matrix
 * \comment
 *      result can equal with m
 */
void multiply_matrix_by_scalar(PtrMatrix4 lhs, float scalar, PtrMatrix4 result);

/* \brief
 *      multiply a matrix with a vector3
 * \param
 *      lhs     (in)    pointer to matrix
 *      v3      (in)    pointer to vector3
 *      result  (out)   pointer to vector3
 */
void multiply_matrix_by_vector3(PtrMatrix4 lhs, PtrVector3 v3, PtrVector3 result);

/* \brief
 *      multiply a matrix with a vector4
 * \param
 *      lhs     (in)    pointer to matrix
 *      v4      (in)    pointer to vector4
 *      result  (out)   pointer to vector4
 */
void multiply_matrix_by_vector4(PtrMatrix4 lhs, PtrVector4 v4, PtrVector4 result);

/* \brief 
 *      cross product of two vector3
 * \param
 *      lhs     (in)    pointer to vector3
 *      rhs     (in)    pointer to vector3
 *      result  (out)   pointre to vector3
 * \comment
 *      lhs can equal with result
 */
void cross_vector3(PtrVector3 lhs, PtrVector3 rhs, PtrVector3 result);

/* \brief
 *      dot product of two vector3
 * \param
 *      lhs     (in)    pointer to vector3
 *      rhs     (in)    pointer to vector3
 * \ret
 *      return the dot product
 */
float dot_vector3(PtrVector3 lhs, PtrVector3 rhs);

/* \brief
 *		normalize the vector3
 * \param
 *		v3		(in)	pointer to vector3
 */
void normalize_vector3(PtrVector3 v3);

/* \brief 
 *      vector3 subtract operation
 * \param
 *      lhs     (in)    left operand
 *      rhs     (in)    right operand
 *      res     (out)   result
 * \comment
 *      lhs, rhs can equal with res
 */
void sub_vector3(PtrVector3 lhs, PtrVector3 rhs, PtrVector3 res);

/* \brief 
 *      vector3 add operation
 * \param
 *      lhs     (in)    left operand
 *      rhs     (in)    right operand
 *      res     (out)   result
 * \comment
 *      lhs, rhs can equal with res
 */
void add_vector3(PtrVector3 lhs, PtrVector3 rhs, PtrVector3 res);

/* \brief
 *		anthor way to calculate the inverse of matrix.
 * \ret
 *		return 0 if succeed. otherwise renturn non-zero.
 */
int invert_matrix_second(PtrMatrix4 m, PtrMatrix4 result);

#endif /* __GL_MATH_H__ */
