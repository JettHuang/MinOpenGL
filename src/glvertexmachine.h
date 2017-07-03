/* \brief
 *      vertex machine of OpenGL.
 *
 * \author  jett huang
 * \date    2010-9-12
 *
 * \change log
 *
 *
 */

#ifndef __GL_VERTEX_MACHINE_H__
#define __GL_VERTEX_MACHINE_H__

#include "../include/GL/gl.h"
#include "glmath.h"
#include "glcontext.h"
#include "gllistcmd.h"


/* --------------- the following function handle vertex ------------- */
/*
 * _params[0] enum variable
 */
void glopEnable(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0] enum variable
 */
void glopDisable(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0] GL_TRUE or GL_FALSE
 */
void glopEdgeFlag(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0]  red   [0.0, 1.0] float
 * _params[1]  green [0.0, 1.0] float
 * _params[2]  blue  [0.0, 1.0] float
 * _params[3]  alpha [0.0, 1.0] float
 */
void glopColor(PtrGLContext pContext, OpParam _params[]);

/* 
 * _params[0] x float
 * _params[1] y float
 * _params[2] z float
 */
void glopNormal(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0] x float
 * _params[1] y float
 * _params[2] z float
 * _params[3] w float
 */
void glopVertex(PtrGLContext pContext, OpParam _params[]);

/* _params[0] texture unit id
 * _params[1] s float
 * _params[2] t float
 * _params[3] r float
 * _params[4] q float
 */
void glopTexCoord(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0] paramName enum
 * _params[1,2,...] ...
 *
 */
void glopLightModel(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0] light[i]
 * _params[1] pname
 * _params[2,...] param
 */
void glopLight(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0] face
 * _params[1] mode
 */
void glopColorMaterial(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0] face
 * _params[1] pname
 * _params[2,..] param
 */
void glopMaterial(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0] mode
 */
void glopMatrixMode(PtrGLContext pContext, OpParam _params[]);

/*
 * none
 */
void glopPushMatrix(PtrGLContext pContext, OpParam _params[]);

/*
 * none
 */
void glopPopMatrix(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0]       degree float
 * _params[1,2,3]   x,y,z axies
 *
 */
void glopRotate(PtrGLContext pContext, OpParam _params[]);

/* 
 * _params[0]   x
 * _params[1]   y
 * _params[2]   z
 */
void glopTransform(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0]   x
 * _params[1]   y
 * _params[2]   z
 */
void glopScale(PtrGLContext pContext, OpParam _params[]);

/*
 * _params[0..15]
 *
 */
void glopLoadMatrix(PtrGLContext pContext, OpParam _params[]);
void glopLoadIdentity(PtrGLContext pContext, OpParam _params[]);
void glopMultMatrix(PtrGLContext pContext, OpParam _params[]);

#endif /* __GL_VERTEX_MACHINE_H__ */