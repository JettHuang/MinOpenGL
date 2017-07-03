/* \brief
 *      图元组装与光栅化.
 *
 * \author  jett huang
 * \date    2010-10-03
 *
 * \change log:
 *     
 *
 */

#ifndef __GL_RASTERLIZE_H__
#define __GL_RASTERLIZE_H__

#include "glcontext.h"
#include "glvertexmachine.h"


/* \breif
 *      draw point
 */
void gl_draw_point(PtrGLContext pContext, PtrGLVertex vertex);

/* \brief
 *      draw line
 */
void gl_draw_line(PtrGLContext pContext, PtrGLVertex vertex0, PtrGLVertex vertex1);

/* \brief
 *      draw triangle
 */
void gl_draw_triangle(PtrGLContext pContext, PtrGLVertex vertex0, PtrGLVertex vertex1, PtrGLVertex vertex2);

/* \brief
 *      receive a vertex from vertex-machine.
 *
 */
void gl_receive_vertex(PtrGLContext pContext, PtrGLVertex vertex);

#endif /* __GL_RASTERLIZE_H__ */