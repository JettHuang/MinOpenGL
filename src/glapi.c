/* \brief
 *      GL接口函数的实现
 *
 * \author  jett huang
 * \date    2010-11-06
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../include/GL/gl.h"
#include "../include/GL/mygl.h"
#include "../include/GL/util.h"

#include "glcontext.h"
#include "glvertexmachine.h"
#include "gltexture.h"


static GLContext glcontext;
static GLushort win_x, win_y, win_w, win_h;
static const char *szClassName = "myGL-Jett";
static HINSTANCE hInstance;
static PTRRESHAPE win_reshape = NULL;
static PTRKEYBOARD win_keyboard = NULL;
static PTRDISPLAY win_display = NULL;
static PTRIDLE win_idle = NULL;
static HWND win_handle = 0;
static HBITMAP bitmapDIB = 0;
static HDC  hmemDC = 0;

/*
void glAccum (GLenum op, GLfloat value);
void glAlphaFunc (GLenum func, GLclampf ref);
GLboolean glAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences);
void glArrayElement (GLint i);
*/
void glActiveTexture(GLenum texture)
{
    tex_ActiveTexture(texture);
}

void glBegin (GLenum mode)
{
    glcontext._primitive_type = mode;
}

void glBindTexture (GLenum target, GLuint texture)
{
    tex_BindTexture(target, texture);
}

/*
void glBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
void glBlendFunc (GLenum sfactor, GLenum dfactor);
void glCallList (GLuint list);
void glCallLists (GLsizei n, GLenum type, const GLvoid *lists);
*/

void glClear (GLbitfield mask)
{
    GLbyte *prgba = glcontext._buf_color, *lines;
    GLushort *pdepth = glcontext._buf_depth;
    GLuint count = glcontext._buf_width * glcontext._buf_height;
    GLuint i, j;

    if ((mask & GL_COLOR_BUFFER_BIT) && prgba) {
        for (i=0; i<glcontext._buf_height; ++i) 
        {
            lines = prgba;
            for (j=0; j<glcontext._buf_width; ++j) {
                *lines++ = glcontext._clear_b;
                *lines++ = glcontext._clear_g;
                *lines++ = glcontext._clear_r;
            }
            prgba += glcontext._buf_color_align;
        }
    }
    if ((mask & GL_DEPTH_BUFFER_BIT) && pdepth) {
        for (i=0; i<count; ++i) *pdepth++ = glcontext._clear_depth;
    }
}

/*
void glClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
*/
void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    red   = CLAMP(red, 0.f, 1.f);
    green = CLAMP(green, 0.f, 1.f);
    blue  = CLAMP(blue, 0.f, 1.f);
    alpha = CLAMP(alpha, 0.f, 1.f);

    glcontext._clear_r = (GLbyte)(red * 255);
    glcontext._clear_g = (GLbyte)(green * 255);
    glcontext._clear_b = (GLbyte)(blue * 255);
    glcontext._clear_a = (GLbyte)(alpha * 255);
}

void glClearDepth (GLclampd depth)
{
    depth = CLAMP(depth, glcontext._viewport._zNear, glcontext._viewport._zFar);
    glcontext._clear_depth = (GLushort)(DEPTH * depth);
}

/*
void glClearIndex (GLfloat c);
void glClearStencil (GLint s);
void glClipPlane (GLenum plane, const GLdouble *equation);
void glColor3b (GLbyte red, GLbyte green, GLbyte blue);
void glColor3bv (const GLbyte *v);
*/

void glColor3d (GLdouble red, GLdouble green, GLdouble blue)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)CLAMP(red, 0.f, 1.f);
    _params[1].f = (GLfloat)CLAMP(green, 0.f, 1.f);
    _params[2].f = (GLfloat)CLAMP(blue, 0.f, 1.f);
    _params[3].f = 1.f;
    glopColor(&glcontext, _params);
}

void glColor3dv (const GLdouble *v)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)CLAMP(v[0], 0.f, 1.f);
    _params[1].f = (GLfloat)CLAMP(v[1], 0.f, 1.f);
    _params[2].f = (GLfloat)CLAMP(v[2], 0.f, 1.f);
    _params[3].f = 1.f;
    glopColor(&glcontext, _params);
}

void glColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)CLAMP(red, 0.f, 1.f);
    _params[1].f = (GLfloat)CLAMP(green, 0.f, 1.f);
    _params[2].f = (GLfloat)CLAMP(blue, 0.f, 1.f);
    _params[3].f = 1.f;
    glopColor(&glcontext, _params);
}

void glColor3fv (const GLfloat *v)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)CLAMP(v[0], 0.f, 1.f);
    _params[1].f = (GLfloat)CLAMP(v[1], 0.f, 1.f);
    _params[2].f = (GLfloat)CLAMP(v[2], 0.f, 1.f);
    _params[3].f = 1.f;
    glopColor(&glcontext, _params);
}

/*
void glColor3i (GLint red, GLint green, GLint blue);
void glColor3iv (const GLint *v);
void glColor3s (GLshort red, GLshort green, GLshort blue);
void glColor3sv (const GLshort *v);
*/

void glColor3ub (GLubyte red, GLubyte green, GLubyte blue)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)red / 255;
    _params[1].f = (GLfloat)green / 255;
    _params[2].f = (GLfloat)blue / 255;
    _params[3].f = 1.f;
    glopColor(&glcontext, _params);
}

void glColor3ubv (const GLubyte *v)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)v[0] / 255;
    _params[1].f = (GLfloat)v[1] / 255;
    _params[2].f = (GLfloat)v[2] / 255;
    _params[3].f = 1.f;
    glopColor(&glcontext, _params);
}

/*
void glColor3ui (GLuint red, GLuint green, GLuint blue);
void glColor3uiv (const GLuint *v);
void glColor3us (GLushort red, GLushort green, GLushort blue);
void glColor3usv (const GLushort *v);
void glColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
void glColor4bv (const GLbyte *v);
*/

void glColor4d (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)CLAMP(red, 0.f, 1.f);
    _params[1].f = (GLfloat)CLAMP(green, 0.f, 1.f);
    _params[2].f = (GLfloat)CLAMP(blue, 0.f, 1.f);
    _params[3].f = (GLfloat)CLAMP(alpha, 0.f, 1.f);
    glopColor(&glcontext, _params);
}

void glColor4dv (const GLdouble *v)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)CLAMP(v[0], 0.f, 1.f);
    _params[1].f = (GLfloat)CLAMP(v[1], 0.f, 1.f);
    _params[2].f = (GLfloat)CLAMP(v[2], 0.f, 1.f);
    _params[3].f = (GLfloat)CLAMP(v[3], 0.f, 1.f);
    glopColor(&glcontext, _params);
}

void glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)CLAMP(red, 0.f, 1.f);
    _params[1].f = (GLfloat)CLAMP(green, 0.f, 1.f);
    _params[2].f = (GLfloat)CLAMP(blue, 0.f, 1.f);
    _params[3].f = (GLfloat)CLAMP(alpha, 0.f, 1.f);
    glopColor(&glcontext, _params);
}

void glColor4fv (const GLfloat *v)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)CLAMP(v[0], 0.f, 1.f);
    _params[1].f = (GLfloat)CLAMP(v[1], 0.f, 1.f);
    _params[2].f = (GLfloat)CLAMP(v[2], 0.f, 1.f);
    _params[3].f = (GLfloat)CLAMP(v[3], 0.f, 1.f);
    glopColor(&glcontext, _params);
}

/*
void glColor4i (GLint red, GLint green, GLint blue, GLint alpha);
void glColor4iv (const GLint *v);
void glColor4s (GLshort red, GLshort green, GLshort blue, GLshort alpha);
void glColor4sv (const GLshort *v);
*/

void glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)red / 255;
    _params[1].f = (GLfloat)green / 255;
    _params[2].f = (GLfloat)blue / 255;
    _params[3].f = (GLfloat)alpha / 255;
    glopColor(&glcontext, _params);
}

void glColor4ubv (const GLubyte *v)
{
    OpParam _params[4];
    _params[0].f = (GLfloat)v[0] / 255;
    _params[1].f = (GLfloat)v[1] / 255;
    _params[2].f = (GLfloat)v[2] / 255;
    _params[3].f = (GLfloat)v[3] / 255;
    glopColor(&glcontext, _params);
}

/*
void glColor4ui (GLuint red, GLuint green, GLuint blue, GLuint alpha);
void glColor4uiv (const GLuint *v);
void glColor4us (GLushort red, GLushort green, GLushort blue, GLushort alpha);
void glColor4usv (const GLushort *v);

void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void glColorMaterial (GLenum face, GLenum mode);
void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
void glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
void glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
*/

void glCullFace (GLenum mode)
{
    glcontext._cull_mode = mode;
}

/*
void glDeleteLists (GLuint list, GLsizei range);
*/

void glDeleteTextures (GLsizei n, const GLuint *textures)
{
    tex_DeleteTexture(n, textures);
}

/*
void glDepthFunc (GLenum func);
void glDepthMask (GLboolean flag);
void glDepthRange (GLclampd zNear, GLclampd zFar);
*/

void glDisable (GLenum cap)
{
    OpParam _params[1];

    _params[0].e = cap;
    glopDisable(&glcontext, _params);
}

/*
void glDisableClientState (GLenum array);
void glDrawArrays (GLenum mode, GLint first, GLsizei count);
void glDrawBuffer (GLenum mode);
void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void glEdgeFlag (GLboolean flag);
void glEdgeFlagPointer (GLsizei stride, const GLvoid *pointer);
void glEdgeFlagv (const GLboolean *flag);
*/

void glEnable (GLenum cap)
{
    OpParam _params[1];

    _params[0].e = cap;
    glopEnable(&glcontext, _params);
}

/*
void glEnableClientState (GLenum array);
*/

void glEnd (void)
{
    glcontext._vertex_count = 0;
    glcontext._primitive_type = 0;
}

/*
void glEndList (void);
void glEvalCoord1d (GLdouble u);
void glEvalCoord1dv (const GLdouble *u);
void glEvalCoord1f (GLfloat u);
void glEvalCoord1fv (const GLfloat *u);
void glEvalCoord2d (GLdouble u, GLdouble v);
void glEvalCoord2dv (const GLdouble *u);
void glEvalCoord2f (GLfloat u, GLfloat v);
void glEvalCoord2fv (const GLfloat *u);
void glEvalMesh1 (GLenum mode, GLint i1, GLint i2);
void glEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
void glEvalPoint1 (GLint i);
void glEvalPoint2 (GLint i, GLint j);
void glFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer);
void glFinish (void);
*/

void glFlush (void)
{
    /* do nothing */
}

/*
void glFogf (GLenum pname, GLfloat param);
void glFogfv (GLenum pname, const GLfloat *params);
void glFogi (GLenum pname, GLint param);
void glFogiv (GLenum pname, const GLint *params);
*/

void glFrontFace (GLenum mode)
{
    glcontext._fontface_mode = mode;
}

void glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    OpParam _params[16];
    GLfloat deltaX, deltaY, deltaZ;

    deltaX = right - left;
    deltaY = top - bottom;
    deltaZ = zFar - zNear;
    if ((zNear <= 0.f) || (zFar <= 0.f) || (deltaX == 0.f) ||
        (deltaY == 0.f) || (deltaZ == 0.f)) {
        printf("glFrustum: invalid parameters\n");
        return;
    }
    
    _params[0].f = (2.f * zNear) / deltaX;
    _params[1].f = 0.f;
    _params[2].f = 0.f;
    _params[3].f = 0.f;
    _params[4].f = 0.f;
    _params[5].f = (2.f * zNear) / deltaY;
    _params[6].f = 0.f;
    _params[7].f = 0.f;
    _params[8].f = (right + left) / deltaX;
    _params[9].f = (top + bottom) / deltaY;
    _params[10].f= -(zFar + zNear) / deltaZ;
    _params[11].f= -1.f;
    _params[12].f= 0.f;
    _params[13].f= 0.f;
    _params[14].f= -2.f * zFar * zNear / deltaZ;
    _params[15].f= 0.f;
    glopMultMatrix(&glcontext, _params);
}

/* GLuint glGenLists (GLsizei range); */

void glGenTextures (GLsizei n, GLuint *textures)
{
    tex_GenTextures(n, textures);
}

/*
void glGetBooleanv (GLenum pname, GLboolean *params);
void glGetClipPlane (GLenum plane, GLdouble *equation);
void glGetDoublev (GLenum pname, GLdouble *params);
GLenum glGetError (void);
void glGetFloatv (GLenum pname, GLfloat *params);
void glGetIntegerv (GLenum pname, GLint *params);
void glGetLightfv (GLenum light, GLenum pname, GLfloat *params);
void glGetLightiv (GLenum light, GLenum pname, GLint *params);
void glGetMapdv (GLenum target, GLenum query, GLdouble *v);
void glGetMapfv (GLenum target, GLenum query, GLfloat *v);
void glGetMapiv (GLenum target, GLenum query, GLint *v);
void glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params);
void glGetMaterialiv (GLenum face, GLenum pname, GLint *params);
void glGetPixelMapfv (GLenum map, GLfloat *values);
void glGetPixelMapuiv (GLenum map, GLuint *values);
void glGetPixelMapusv (GLenum map, GLushort *values);
void glGetPointerv (GLenum pname, GLvoid* *params);
void glGetPolygonStipple (GLubyte *mask);
const GLubyte * glGetString (GLenum name);
void glGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params);
void glGetTexEnviv (GLenum target, GLenum pname, GLint *params);
void glGetTexGendv (GLenum coord, GLenum pname, GLdouble *params);
void glGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params);
void glGetTexGeniv (GLenum coord, GLenum pname, GLint *params);
void glGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params);
void glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params);
void glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
void glGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
void glHint (GLenum target, GLenum mode);
void glIndexMask (GLuint mask);
void glIndexPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
void glIndexd (GLdouble c);
void glIndexdv (const GLdouble *c);
void glIndexf (GLfloat c);
void glIndexfv (const GLfloat *c);
void glIndexi (GLint c);
void glIndexiv (const GLint *c);
void glIndexs (GLshort c);
void glIndexsv (const GLshort *c);
void glIndexub (GLubyte c);
void glIndexubv (const GLubyte *c);
void glInitNames (void);
void glInterleavedArrays (GLenum format, GLsizei stride, const GLvoid *pointer);
GLboolean glIsEnabled (GLenum cap);
GLboolean glIsList (GLuint list);
GLboolean glIsTexture (GLuint texture);
*/

void glLightModelf (GLenum pname, GLfloat param)
{
    OpParam _params[2];

    _params[0].e = pname;
    _params[1].f = param;
    glopLightModel(&glcontext, _params);
}

void glLightModelfv (GLenum pname, const GLfloat *params)
{
    OpParam _params[5];
    
    _params[0].e = pname;
    _params[1].f = params[0];
    _params[2].f = params[1];
    _params[3].f = params[2];
    _params[4].f = params[3];
    glopLightModel(&glcontext, _params);
}

void glLightModeli (GLenum pname, GLint param)
{
    OpParam _params[2];

    _params[0].e = pname;
    _params[1].i = param;
    glopLightModel(&glcontext, _params);
}

void glLightModeliv (GLenum pname, const GLint *params)
{
    OpParam _params[5];
    
    _params[0].e = pname;
    _params[1].f = (GLfloat)params[0];
    _params[2].f = (GLfloat)params[1];
    _params[3].f = (GLfloat)params[2];
    _params[4].f = (GLfloat)params[3];
    glopLightModel(&glcontext, _params);
}

void glLightf (GLenum light, GLenum pname, GLfloat param)
{
    OpParam _params[6];

    _params[0].e = light;
    _params[1].e = pname;
    _params[2].f = param;
    glopLight(&glcontext, _params);
}

void glLightfv (GLenum light, GLenum pname, const GLfloat *params)
{
    OpParam _params[6];
    
    _params[0].e = light;
    _params[1].e = pname;
    _params[2].f = (GLfloat)params[0];
    _params[3].f = (GLfloat)params[1];
    _params[4].f = (GLfloat)params[2];
    _params[5].f = (GLfloat)params[3];
    glopLight(&glcontext, _params);
}

void glLighti (GLenum light, GLenum pname, GLint param)
{
    OpParam _params[6];

    _params[0].e = light;
    _params[1].e = pname;
    _params[2].i = param;
    glopLight(&glcontext, _params);
}

void glLightiv (GLenum light, GLenum pname, const GLint *params)
{
    OpParam _params[6];
    
    _params[0].e = light;
    _params[1].e = pname;
    _params[2].f = (GLfloat)params[0];
    _params[3].f = (GLfloat)params[1];
    _params[4].f = (GLfloat)params[2];
    _params[5].f = (GLfloat)params[3];
    glopLight(&glcontext, _params);
}

/*
void glLineStipple (GLint factor, GLushort pattern);
void glLineWidth (GLfloat width);
void glListBase (GLuint base);
*/

void glLoadIdentity (void)
{
    glopLoadIdentity(&glcontext, NULL);
}

void glLoadMatrixd (const GLdouble *m)
{
    OpParam _params[16];
    
    _params[0].f = (GLfloat)m[0];
    _params[1].f = (GLfloat)m[1];
    _params[2].f = (GLfloat)m[2];
    _params[3].f = (GLfloat)m[3];
    _params[4].f = (GLfloat)m[4];
    _params[5].f = (GLfloat)m[5];
    _params[6].f = (GLfloat)m[6];
    _params[7].f = (GLfloat)m[7];
    _params[8].f = (GLfloat)m[8];
    _params[9].f = (GLfloat)m[9];
    _params[10].f= (GLfloat)m[10];
    _params[11].f= (GLfloat)m[11];
    _params[12].f= (GLfloat)m[12];
    _params[13].f= (GLfloat)m[13];
    _params[14].f= (GLfloat)m[14];
    _params[15].f= (GLfloat)m[15];
    glopLoadMatrix(&glcontext, _params);
}

void glLoadMatrixf (const GLfloat *m)
{
    OpParam _params[16];
    
    _params[0].f = m[0];
    _params[1].f = m[1];
    _params[2].f = m[2];
    _params[3].f = m[3];
    _params[4].f = m[4];
    _params[5].f = m[5];
    _params[6].f = m[6];
    _params[7].f = m[7];
    _params[8].f = m[8];
    _params[9].f = m[9];
    _params[10].f= m[10];
    _params[11].f= m[11];
    _params[12].f= m[12];
    _params[13].f= m[13];
    _params[14].f= m[14];
    _params[15].f= m[15];
    glopLoadMatrix(&glcontext, _params);
}

/*
void glLoadName (GLuint name);
void glLogicOp (GLenum opcode);
void glMap1d (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
void glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
void glMap2d (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
void glMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
void glMapGrid1d (GLint un, GLdouble u1, GLdouble u2);
void glMapGrid1f (GLint un, GLfloat u1, GLfloat u2);
void glMapGrid2d (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
void glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
void glMateriali (GLenum face, GLenum pname, GLint param);
void glMaterialiv (GLenum face, GLenum pname, const GLint *params);
*/

void glMaterialf (GLenum face, GLenum pname, GLfloat param)
{
    OpParam _params[6];

    _params[0].e = face;
    _params[1].e = pname;
    _params[2].f = param;
    glopMaterial(&glcontext, _params);
}

void glMaterialfv (GLenum face, GLenum pname, const GLfloat *params)
{
    OpParam _params[6];

    _params[0].e = face;
    _params[1].e = pname;
    _params[2].f = params[0];
    _params[3].f = params[1];
    _params[4].f = params[2];
    _params[5].f = params[3];
    glopMaterial(&glcontext, _params);
}

void glMatrixMode (GLenum mode)
{
    OpParam _params[1];

    _params[0].e = mode;
    glopMatrixMode(&glcontext, _params);
}

void glMultMatrixd (const GLdouble *m)
{
    OpParam _params[16];
    
    _params[0].f = (GLfloat)m[0];
    _params[1].f = (GLfloat)m[1];
    _params[2].f = (GLfloat)m[2];
    _params[3].f = (GLfloat)m[3];
    _params[4].f = (GLfloat)m[4];
    _params[5].f = (GLfloat)m[5];
    _params[6].f = (GLfloat)m[6];
    _params[7].f = (GLfloat)m[7];
    _params[8].f = (GLfloat)m[8];
    _params[9].f = (GLfloat)m[9];
    _params[10].f= (GLfloat)m[10];
    _params[11].f= (GLfloat)m[11];
    _params[12].f= (GLfloat)m[12];
    _params[13].f= (GLfloat)m[13];
    _params[14].f= (GLfloat)m[14];
    _params[15].f= (GLfloat)m[15];
    glopMultMatrix(&glcontext, _params);
}

void glMultMatrixf (const GLfloat *m)
{
    OpParam _params[16];
    
    _params[0].f = m[0];
    _params[1].f = m[1];
    _params[2].f = m[2];
    _params[3].f = m[3];
    _params[4].f = m[4];
    _params[5].f = m[5];
    _params[6].f = m[6];
    _params[7].f = m[7];
    _params[8].f = m[8];
    _params[9].f = m[9];
    _params[10].f= m[10];
    _params[11].f= m[11];
    _params[12].f= m[12];
    _params[13].f= m[13];
    _params[14].f= m[14];
    _params[15].f= m[15];
    glopMultMatrix(&glcontext, _params);
}

/*
void glNewList (GLuint list, GLenum mode);
void glNormal3b (GLbyte nx, GLbyte ny, GLbyte nz);
void glNormal3bv (const GLbyte *v);
*/

void glNormal3d (GLdouble nx, GLdouble ny, GLdouble nz)
{
    OpParam _params[3];

    _params[0].f = (GLfloat)nx;
    _params[1].f = (GLfloat)ny;
    _params[2].f = (GLfloat)nz;
    glopNormal(&glcontext, _params);
}

void glNormal3dv (const GLdouble *v)
{
    OpParam _params[3];

    _params[0].f = (GLfloat)v[0];
    _params[1].f = (GLfloat)v[1];
    _params[2].f = (GLfloat)v[2];
    glopNormal(&glcontext, _params);
}

void glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz)
{
    OpParam _params[3];

    _params[0].f = nx;
    _params[1].f = ny;
    _params[2].f = nz;
    glopNormal(&glcontext, _params);
}

void glNormal3fv (const GLfloat *v)
{
    OpParam _params[3];

    _params[0].f = v[0];
    _params[1].f = v[1];
    _params[2].f = v[2];
    glopNormal(&glcontext, _params);
}

/*
void glNormal3i (GLint nx, GLint ny, GLint nz);
void glNormal3iv (const GLint *v);
void glNormal3s (GLshort nx, GLshort ny, GLshort nz);
void glNormal3sv (const GLshort *v);

void glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
*/

void glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    OpParam _params[16];
    GLfloat deltaX, deltaY, deltaZ;

    deltaX = right - left;
    deltaY = top - bottom;
    deltaZ = zFar - zNear;
    if ((deltaX == 0.f) || (deltaY == 0.f) || (deltaZ == 0.f)) {
        printf("glOrtho: invalid parameters\n");
        return;
    }
    
    _params[0].f = 2.f / deltaX;
    _params[1].f = 0.f;
    _params[2].f = 0.f;
    _params[3].f = 0.f;
    _params[4].f = 0.f;
    _params[5].f = 2.f / deltaY;
    _params[6].f = 0.f;
    _params[7].f = 0.f;
    _params[8].f = 0.f;
    _params[9].f = 0.f;
    _params[10].f= -2.f / deltaZ;
    _params[11].f= 0.f;
    _params[12].f= -(right + left) / deltaX;
    _params[13].f= -(top + bottom) / deltaY;
    _params[14].f= -(zFar + zNear) / deltaZ;
    _params[15].f= 1.f;
    glopMultMatrix(&glcontext, _params);
}

/*
void glPassThrough (GLfloat token);
void glPixelMapfv (GLenum map, GLsizei mapsize, const GLfloat *values);
void glPixelMapuiv (GLenum map, GLsizei mapsize, const GLuint *values);
void glPixelMapusv (GLenum map, GLsizei mapsize, const GLushort *values);
void glPixelStoref (GLenum pname, GLfloat param);
void glPixelTransferf (GLenum pname, GLfloat param);
void glPixelTransferi (GLenum pname, GLint param);
void glPixelZoom (GLfloat xfactor, GLfloat yfactor);
void glPointSize (GLfloat size);
void glPolygonMode (GLenum face, GLenum mode);
void glPolygonOffset (GLfloat factor, GLfloat units);
void glPolygonStipple (const GLubyte *mask);
void glPopAttrib (void);
void glPopClientAttrib (void);
*/

void glPixelStorei (GLenum pname, GLint param)
{
    if (pname != GL_UNPACK_ALIGNMENT || param != 1) {
        printf("ERROR, glPixelStorei(), invalid parameter\n");
    }
}

void glPopMatrix (void)
{
    glopPopMatrix(&glcontext, NULL);
}

/*
void glPopName (void);
void glPrioritizeTextures (GLsizei n, const GLuint *textures, const GLclampf *priorities);
void glPushAttrib (GLbitfield mask);
void glPushClientAttrib (GLbitfield mask);
*/

void glPushMatrix (void)
{
    glopPushMatrix(&glcontext, NULL);
}

/*
void glPushName (GLuint name);
void glRasterPos2d (GLdouble x, GLdouble y);
void glRasterPos2dv (const GLdouble *v);
void glRasterPos2f (GLfloat x, GLfloat y);
void glRasterPos2fv (const GLfloat *v);
void glRasterPos2i (GLint x, GLint y);
void glRasterPos2iv (const GLint *v);
void glRasterPos2s (GLshort x, GLshort y);
void glRasterPos2sv (const GLshort *v);
void glRasterPos3d (GLdouble x, GLdouble y, GLdouble z);
void glRasterPos3dv (const GLdouble *v);
void glRasterPos3f (GLfloat x, GLfloat y, GLfloat z);
void glRasterPos3fv (const GLfloat *v);
void glRasterPos3i (GLint x, GLint y, GLint z);
void glRasterPos3iv (const GLint *v);
void glRasterPos3s (GLshort x, GLshort y, GLshort z);
void glRasterPos3sv (const GLshort *v);
void glRasterPos4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glRasterPos4dv (const GLdouble *v);
void glRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glRasterPos4fv (const GLfloat *v);
void glRasterPos4i (GLint x, GLint y, GLint z, GLint w);
void glRasterPos4iv (const GLint *v);
void glRasterPos4s (GLshort x, GLshort y, GLshort z, GLshort w);
void glRasterPos4sv (const GLshort *v);
void glReadBuffer (GLenum mode);
void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void glRectd (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void glRectdv (const GLdouble *v1, const GLdouble *v2);
void glRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void glRectfv (const GLfloat *v1, const GLfloat *v2);
void glRecti (GLint x1, GLint y1, GLint x2, GLint y2);
void glRectiv (const GLint *v1, const GLint *v2);
void glRects (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
void glRectsv (const GLshort *v1, const GLshort *v2);
GLint glRenderMode (GLenum mode);
*/

void glRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)angle;
    _params[1].f = (GLfloat)x;
    _params[2].f = (GLfloat)y;
    _params[3].f = (GLfloat)z;
    glopRotate(&glcontext, _params);
}

void glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    OpParam _params[4];

    _params[0].f = angle;
    _params[1].f = x;
    _params[2].f = y;
    _params[3].f = z;
    glopRotate(&glcontext, _params);
}

void glScaled (GLdouble x, GLdouble y, GLdouble z)
{
    OpParam _params[3];
    _params[0].f = (GLfloat)x;
    _params[1].f = (GLfloat)y;
    _params[2].f = (GLfloat)z;
    glopScale(&glcontext, _params);
}

void glScalef (GLfloat x, GLfloat y, GLfloat z)
{
    OpParam _params[3];
    _params[0].f = x;
    _params[1].f = y;
    _params[2].f = z;
    glopScale(&glcontext, _params);
}

/*
void glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
void glSelectBuffer (GLsizei size, GLuint *buffer);
void glStencilFunc (GLenum func, GLint ref, GLuint mask);
void glStencilMask (GLuint mask);
void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
void glTexCoord1d (GLdouble s);
void glTexCoord1dv (const GLdouble *v);
void glTexCoord1f (GLfloat s);
void glTexCoord1fv (const GLfloat *v);
void glTexCoord1i (GLint s);
void glTexCoord1iv (const GLint *v);
void glTexCoord1s (GLshort s);
void glTexCoord1sv (const GLshort *v);
*/

void glShadeModel (GLenum mode)
{
    /* do nothing now */
}

void glTexCoord2d (GLdouble s, GLdouble t)
{
    OpParam _params[5];

    _params[0].e = GL_TEXTURE0;
    _params[1].f = (GLfloat)s;
    _params[2].f = (GLfloat)t;
    _params[3].f = 0.f;
    _params[4].f = 1.f;
    glopTexCoord(&glcontext, _params);
}

void glTexCoord2dv (const GLdouble *v)
{
    OpParam _params[5];

    _params[0].e = GL_TEXTURE0;
    _params[1].f = (GLfloat)v[0];
    _params[2].f = (GLfloat)v[1];
    _params[3].f = 0.f;
    _params[4].f = 1.f;
    glopTexCoord(&glcontext, _params);
}

void glTexCoord2f (GLfloat s, GLfloat t)
{
    OpParam _params[5];

    _params[0].e = GL_TEXTURE0;
    _params[1].f = (GLfloat)s;
    _params[2].f = (GLfloat)t;
    _params[3].f = 0.f;
    _params[4].f = 1.f;
    glopTexCoord(&glcontext, _params);
}

void glTexCoord2fv (const GLfloat *v)
{
    OpParam _params[5];

    _params[0].e = GL_TEXTURE0;
    _params[1].f = (GLfloat)v[0];
    _params[2].f = (GLfloat)v[1];
    _params[3].f = 0.f;
    _params[4].f = 1.f;
    glopTexCoord(&glcontext, _params);
}

void glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t)
{
    OpParam _params[5];

    _params[0].e = target;
    _params[1].f = s;
    _params[2].f = t;
    _params[3].f = 0.f;
    _params[4].f = 1.f;
    glopTexCoord(&glcontext, _params);
}

/*
void glTexCoord2i (GLint s, GLint t);
void glTexCoord2iv (const GLint *v);
void glTexCoord2s (GLshort s, GLshort t);
void glTexCoord2sv (const GLshort *v);
void glTexCoord3d (GLdouble s, GLdouble t, GLdouble r);
void glTexCoord3dv (const GLdouble *v);
void glTexCoord3f (GLfloat s, GLfloat t, GLfloat r);
void glTexCoord3fv (const GLfloat *v);
void glTexCoord3i (GLint s, GLint t, GLint r);
void glTexCoord3iv (const GLint *v);
void glTexCoord3s (GLshort s, GLshort t, GLshort r);
void glTexCoord3sv (const GLshort *v);
void glTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void glTexCoord4dv (const GLdouble *v);
void glTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void glTexCoord4fv (const GLfloat *v);
void glTexCoord4i (GLint s, GLint t, GLint r, GLint q);
void glTexCoord4iv (const GLint *v);
void glTexCoord4s (GLshort s, GLshort t, GLshort r, GLshort q);
void glTexCoord4sv (const GLshort *v);

void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
*/

/*
void glTexEnvf (GLenum target, GLenum pname, GLfloat param);
void glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
*/

void glTexEnvi (GLenum target, GLenum pname, GLint param)
{
    tex_TexEnvi(target, pname, param);
}

/*
void glTexEnviv (GLenum target, GLenum pname, const GLint *params);
void glTexGend (GLenum coord, GLenum pname, GLdouble param);
void glTexGendv (GLenum coord, GLenum pname, const GLdouble *params);
void glTexGenf (GLenum coord, GLenum pname, GLfloat param);
void glTexGenfv (GLenum coord, GLenum pname, const GLfloat *params);
void glTexGeni (GLenum coord, GLenum pname, GLint param);
void glTexGeniv (GLenum coord, GLenum pname, const GLint *params);
void glTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
*/

void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, 
                   GLenum type, const GLvoid *pixels)
{
    tex_TexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}
/*
void glTexParameterf (GLenum target, GLenum pname, GLfloat param)
void glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
*/
void glTexParameteri (GLenum target, GLenum pname, GLint param)
{
    tex_TexParameteri(target, pname, param);
}

/*
void glTexParameteriv (GLenum target, GLenum pname, const GLint *params);
void glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
*/

void glTranslated (GLdouble x, GLdouble y, GLdouble z)
{
    OpParam _params[3];

    _params[0].f = (GLfloat)x;
    _params[1].f = (GLfloat)y;
    _params[2].f = (GLfloat)z;
    glopTransform(&glcontext, _params);
}

void glTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
    OpParam _params[3];

    _params[0].f = (GLfloat)x;
    _params[1].f = (GLfloat)y;
    _params[2].f = (GLfloat)z;
    glopTransform(&glcontext, _params);
}

void glVertex2d (GLdouble x, GLdouble y)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)x;
    _params[1].f = (GLfloat)y;
    _params[2].f = 0.f;
    _params[3].f = 1.f;
    glopVertex(&glcontext, _params);
}

void glVertex2dv (const GLdouble *v)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)v[0];
    _params[1].f = (GLfloat)v[1];
    _params[2].f = 0.f;
    _params[3].f = 1.f;
    glopVertex(&glcontext, _params);
}

void glVertex2f (GLfloat x, GLfloat y)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)x;
    _params[1].f = (GLfloat)y;
    _params[2].f = 0.f;
    _params[3].f = 1.f;
    glopVertex(&glcontext, _params);
}

void glVertex2fv (const GLfloat *v)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)v[0];
    _params[1].f = (GLfloat)v[1];
    _params[2].f = 0.f;
    _params[3].f = 1.f;
    glopVertex(&glcontext, _params);
}

/*
void glVertex2i (GLint x, GLint y);
void glVertex2iv (const GLint *v);
void glVertex2s (GLshort x, GLshort y);
void glVertex2sv (const GLshort *v);
*/

void glVertex3d (GLdouble x, GLdouble y, GLdouble z)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)x;
    _params[1].f = (GLfloat)y;
    _params[2].f = (GLfloat)z;
    _params[3].f = 1.f;
    glopVertex(&glcontext, _params);
}

void glVertex3dv (const GLdouble *v)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)v[0];
    _params[1].f = (GLfloat)v[1];
    _params[2].f = (GLfloat)v[2];
    _params[3].f = 1.f;
    glopVertex(&glcontext, _params);
}

void glVertex3f (GLfloat x, GLfloat y, GLfloat z)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)x;
    _params[1].f = (GLfloat)y;
    _params[2].f = (GLfloat)z;
    _params[3].f = 1.f;
    glopVertex(&glcontext, _params);
}

void glVertex3fv (const GLfloat *v)
{
    OpParam _params[4];

    _params[0].f = (GLfloat)v[0];
    _params[1].f = (GLfloat)v[1];
    _params[2].f = (GLfloat)v[2];
    _params[3].f = 1.f;
    glopVertex(&glcontext, _params);
}

/*
void glVertex3i (GLint x, GLint y, GLint z);
void glVertex3iv (const GLint *v);
void glVertex3s (GLshort x, GLshort y, GLshort z);
void glVertex3sv (const GLshort *v);
void glVertex4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glVertex4dv (const GLdouble *v);
void glVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glVertex4fv (const GLfloat *v);
void glVertex4i (GLint x, GLint y, GLint z, GLint w);
void glVertex4iv (const GLint *v);
void glVertex4s (GLshort x, GLshort y, GLshort z, GLshort w);
void glVertex4sv (const GLshort *v);

void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
*/

void glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
    glcontext._viewport._x = x;
    glcontext._viewport._y = y;
    glcontext._viewport._width = width;
    glcontext._viewport._height= height;
    glcontext._viewport._xCenter = x + (width - 1) * 0.5f;
    glcontext._viewport._xScale  = (width - 1) * 0.5f;
    glcontext._viewport._yCenter = (y + (height - 1) * 0.5f);
    glcontext._viewport._yScale  = (height - 1) * 0.5f;
}


/* \brief
 *      初始化myGL
 *
 */
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLboolean myGLinit()
{
    WNDCLASS wndclass;

    glContext_init(&glcontext);
    /* register window class */
    hInstance = GetModuleHandle(NULL);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			"myGL", MB_ICONERROR);
		return GL_FALSE;
	}

    return GL_TRUE;
}

/* \brief
 *      退出myGL
 *
 */
GLvoid myGLexit()
{
    if (bitmapDIB) {
        DeleteObject(bitmapDIB);
        bitmapDIB = 0;
    }
    glContext_uninit(&glcontext);
}

/* \brief
 *      resize frame buffer
 */
GLvoid myGLresize(GLuint w, GLuint h)
{
    BITMAPINFOHEADER bmpHeader;
    void *pbuf = NULL;

    glcontext._buf_width = 0;
    glcontext._buf_height= 0;
    glcontext._buf_color_align = 0;
    glcontext._buf_color = NULL;
    if (bitmapDIB) {
        DeleteObject(bitmapDIB);
        bitmapDIB = 0;
    }

    if (w == 0 || h == 0) {
        return;
    }
/*  USE DIB SECTION INSTEAD OF IT!!
    if (!glcontext._buf_color) {
        glcontext._buf_color = malloc(w * h * sizeof(GLuint));
    } else {
        glcontext._buf_color = realloc(glcontext._buf_color, w * h * sizeof(GLuint));
    }
*/
    bmpHeader.biSize  = sizeof(BITMAPINFOHEADER); 
    bmpHeader.biWidth = w; 
    bmpHeader.biHeight= h; 
    bmpHeader.biPlanes= 1; 
    bmpHeader.biBitCount = 24; 
    bmpHeader.biCompression = BI_RGB; 
    bmpHeader.biSizeImage = 0; 
    bmpHeader.biXPelsPerMeter = 0; 
    bmpHeader.biYPelsPerMeter = 0; 
    bmpHeader.biClrUsed = 0; 
    bmpHeader.biClrImportant = 0; 
    bitmapDIB = CreateDIBSection(NULL, (BITMAPINFO *)&bmpHeader, DIB_RGB_COLORS, &pbuf, NULL, 0);
    /* set the colr buffer */
    glcontext._buf_width = w;
    glcontext._buf_height= h;
    glcontext._buf_color_align = (w * 3 + 3) & (~3);
    glcontext._buf_color = (GLbyte *)pbuf;
    
    if (!glcontext._buf_depth) {
        glcontext._buf_depth = malloc(w * h * sizeof(GLushort));
    } else {
        glcontext._buf_depth = realloc(glcontext._buf_depth, w * h * sizeof(GLushort));
    }
}

/*\brief
 *      require redisplay
 */
GLvoid myGLpostRedisplay()
{
    InvalidateRect(win_handle, NULL, FALSE); 
}

/* \brief
 *      swap buffer
 *
 */
GLvoid myGLswapbuffer()
{
    HDC hdc;
    GLuint w, h;

    if (!glcontext._buf_color)
        return;
#ifdef QUERY_PERFORMANCE
    begin_performance();
#endif
    w = glcontext._buf_width;
    h = glcontext._buf_height;
    hdc = GetDC(win_handle);
    
    SelectObject(hmemDC, bitmapDIB);
    BitBlt(hdc, 0, 0, w, h, hmemDC, 0, 0, SRCCOPY);
    ReleaseDC(win_handle, hdc);

#ifdef QUERY_PERFORMANCE
    end_performance("myGLswapbuffer");
#endif
}

/* \brief
 *      set window size
 */
GLvoid myGLinitWindowSize(GLushort w, GLushort h)
{
    win_w = w;
    win_h = h;
}

GLvoid myGLinitWindowPosition(GLushort x, GLushort y)
{
    win_x = x;
    win_y = y;
}

/* \brief
 *      create window
 */
GLvoid myGLcreateWindow(GLubyte *name)
{
    HWND hwnd;

	hwnd = CreateWindow(szClassName, 
		name,
		WS_OVERLAPPEDWINDOW,
		win_x,
		win_y,
		win_w,
		win_h,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);
}

GLvoid myGLsetWindowCaption(GLubyte *name)
{
    if (name) SetWindowText(win_handle, name);
}

/* \brief
 *      set reshape callback function
 */
GLvoid myGLreshapeFunc(PTRRESHAPE reshape)
{
    win_reshape = reshape;
}

/* keyboard callback */
GLvoid myGLkeyboardFunc(PTRKEYBOARD keyboard)
{
    win_keyboard = keyboard;
}

GLvoid myGLdisplayFunc(PTRDISPLAY display)
{
    win_display = display;
}

GLvoid myGLidleFunc(PTRIDLE idle)
{
    win_idle = idle;
}

GLvoid myGLmainLoop()
{
    MSG msg;
    BOOL done = FALSE;

    while (!done) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	    {
            if (msg.message == WM_QUIT) {
                done = TRUE;
            } else {
		        TranslateMessage(&msg);
		        DispatchMessage(&msg);
            }
	    }
        if (win_idle) win_idle();

    } /* end while */
}

// Callback function of window
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
	RECT rect;
    POINT pt;
    GLushort w, h;
    
	switch (message)
	{
	case WM_CREATE:
        win_handle = hwnd;
        hdc = GetDC(hwnd);
        hmemDC = CreateCompatibleDC(hdc);
        ReleaseDC(hwnd, hdc);
		return 0;

	case WM_PAINT:
		{
            if (win_display) win_display();
            GetClientRect(hwnd, &rect);
            ValidateRect(hwnd, &rect);
		}
		return 0;
    case WM_CHAR:
        {
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);
            GetClientRect(hwnd, &rect);
            if (win_keyboard) win_keyboard(wParam, pt.x, rect.bottom - 1 - pt.y);
        }
        return 0;

    case WM_SIZE:
        {
            w = LOWORD(lParam);
            h = HIWORD(lParam);
            myGLresize(w, h);
            if (win_reshape) win_reshape(w, h);
        }
        return 0;

	case WM_DESTROY:
        DeleteDC(hmemDC);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
