/* \brief
 *      图元组装与光栅化的实现.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/GL/util.h"
#include "glrasterlize.h"
#include "glfragment.h"


/*
** The following is a discussion of the math used to do edge clipping against
** a clipping plane.
** 
**     P1 is an end point of the edge
**     P2 is the other end point of the edge
** 
**     Q = t*P1 + (1 - t)*P2
**     That is, Q lies somewhere on the line formed by P1 and P2.
** 
**     0 <= t <= 1
**     This constrains Q to lie between P1 and P2.
** 
**     C is the plane equation for the clipping plane
** 
**     D1 = P1 dot C
**     D1 is the distance between P1 and C.  If P1 lies on the plane
**     then D1 will be zero.  The sign of D1 will determine which side
**     of the plane that P1 is on, with negative being outside.
** 
**     D2 = P2 dot C
**     D2 is the distance between P2 and C.  If P2 lies on the plane
**     then D2 will be zero.  The sign of D2 will determine which side
**     of the plane that P2 is on, with negative being outside.
** 
** Because we are trying to find the intersection of the P1 P2 line
** segment with the clipping plane we require that:
** 
**     Q dot C = 0
** 
** Therefore
** 
**     (t*P1 + (1 - t)*P2) dot C = 0
** 
**     (t*P1 + P2 - t*P2) dot C = 0
** 
**     t*P1 dot C + P2 dot C - t*P2 dot C = 0
** 
** Substituting D1 and D2 in
** 
**     t*D1 + D2 - t*D2 = 0
** 
** Solving for t
** 
**     t = -D2 / (D1 - D2)
** 
**     t = D2 / (D2 - D1)
*/


/* \breif
 *      draw point
 */
void gl_draw_point(PtrGLContext pContext, PtrGLVertex vertex)
{
    GLFragment fragment;
    GLfloat    x, y, z, wInv;
    int i;

    if (vertex->_clip_code) return;
    
    x = vertex->_pro_coord._v[0];
    y = vertex->_pro_coord._v[1];
    z = vertex->_pro_coord._v[2];
    wInv = 1.0f / vertex->_pro_coord._v[3];
    
    fragment._winX = (GLint)(x * wInv * pContext->_viewport._xScale + pContext->_viewport._xCenter);
    fragment._winY = (GLint)(y * wInv * pContext->_viewport._yScale + pContext->_viewport._yCenter);
    fragment._winZ = (GLushort)(z * wInv * pContext->_viewport._zScale + pContext->_viewport._zCenter);
    if (fragment._winX < 0 || fragment._winX >= pContext->_buf_width
        || fragment._winY < 0 || fragment._winY >= pContext->_buf_height)
    {
        printf("WARNING: gl_draw_point(): fragment is out of buffer after rasterlization\n");
        return;
    }

    copy_vector4(&fragment._color, &(vertex->_front_color));
    for (i=0; i<MAX_TEXUNITS; ++i) {
        copy_vector4(&fragment._texcoords[i], &(vertex->_texcoords[i]));
    }

    /* process fragment */
    gl_process_fragment(pContext, &fragment);
}

/* \brief
 *      interpolate
 *
 */
static void interpolate(PtrGLVertex vt, PtrGLVertex v0, PtrGLVertex v1, GLfloat t)
{
    int i;

    vt->_pro_coord._v[0] = v0->_pro_coord._v[0] + (v1->_pro_coord._v[0] - v0->_pro_coord._v[0]) * t;
    vt->_pro_coord._v[1] = v0->_pro_coord._v[1] + (v1->_pro_coord._v[1] - v0->_pro_coord._v[1]) * t;
    vt->_pro_coord._v[2] = v0->_pro_coord._v[2] + (v1->_pro_coord._v[2] - v0->_pro_coord._v[2]) * t;
    vt->_pro_coord._v[3] = v0->_pro_coord._v[3] + (v1->_pro_coord._v[3] - v0->_pro_coord._v[3]) * t;
    vt->_win_coord._v[3] = 1.0f / vt->_pro_coord._v[3]; 

    for (i=0; i<MAX_TEXUNITS; ++i) {
        vt->_texcoords[i]._v[0] = v0->_texcoords[i]._v[0] + (v1->_texcoords[i]._v[0] - v0->_texcoords[i]._v[0]) * t;
        vt->_texcoords[i]._v[1] = v0->_texcoords[i]._v[1] + (v1->_texcoords[i]._v[1] - v0->_texcoords[i]._v[1]) * t;
        vt->_texcoords[i]._v[2] = v0->_texcoords[i]._v[2] + (v1->_texcoords[i]._v[2] - v0->_texcoords[i]._v[2]) * t;
        vt->_texcoords[i]._v[3] = v0->_texcoords[i]._v[3] + (v1->_texcoords[i]._v[3] - v0->_texcoords[i]._v[3]) * t;
    }
    /* front color */
    vt->_front_color._v[0] = v0->_front_color._v[0] + (v1->_front_color._v[0] - v0->_front_color._v[0]) * t;
    vt->_front_color._v[1] = v0->_front_color._v[1] + (v1->_front_color._v[1] - v0->_front_color._v[1]) * t;
    vt->_front_color._v[2] = v0->_front_color._v[2] + (v1->_front_color._v[2] - v0->_front_color._v[2]) * t;
    vt->_front_color._v[3] = v0->_front_color._v[3] + (v1->_front_color._v[3] - v0->_front_color._v[3]) * t;
    /* back color */
    vt->_back_color._v[0] = v0->_back_color._v[0] + (v1->_back_color._v[0] - v0->_back_color._v[0]) * t;
    vt->_back_color._v[1] = v0->_back_color._v[1] + (v1->_back_color._v[1] - v0->_back_color._v[1]) * t;
    vt->_back_color._v[2] = v0->_back_color._v[2] + (v1->_back_color._v[2] - v0->_back_color._v[2]) * t;
    vt->_back_color._v[3] = v0->_back_color._v[3] + (v1->_back_color._v[3] - v0->_back_color._v[3]) * t;
}

/* \brief
 *      clip line
 * \param
 *      diff    (in)        Dist0 - Dist1
 *      dist0   (in)        Dist0
 *      tmin    (in/out)    min t
 *      tmax    (in/out)    max t
 * \ret
 */
static void clip_line(GLfloat diff, GLfloat dist0, GLfloat *tmin, GLfloat *tmax)
{
    GLfloat t;

    if (diff < 0) {
        t = dist0 / diff;
        if (t > *tmin) *tmin = t;
    } 
    else if (diff > 0) {
        t = dist0 / diff;
        if (t < *tmax) *tmax = t;
    }
}

/* define the rasterlization code */
#define __RASTER_LINE()     \
            {   \
                oneOverInvz = 1.0f / sinvz;                                     \
                for (i=0; i<MAX_TEXUNITS; ++i) {                                \
                    fragment._texcoords[i]._v[0] = st[i]._v[0] * oneOverInvz;   \
                    fragment._texcoords[i]._v[1] = st[i]._v[1] * oneOverInvz;   \
                    fragment._texcoords[i]._v[2] = st[i]._v[2] * oneOverInvz;   \
                    fragment._texcoords[i]._v[3] = st[i]._v[3] * oneOverInvz;   \
                } /* end for i*/                                                \
                fragment._color._v[0] = sc._v[0] * oneOverInvz;                 \
                fragment._color._v[1] = sc._v[1] * oneOverInvz;                 \
                fragment._color._v[2] = sc._v[2] * oneOverInvz;                 \
                fragment._color._v[3] = sc._v[3] * oneOverInvz;                 \
                fragment._winX = (GLuint)(sx + 0.5f);                           \
                fragment._winY = (GLuint)(sy + 0.5f);                           \
                fragment._winZ = (GLushort)(sz + 0.5f);                         \
                gl_process_fragment(context, &fragment);                        \
                                                                                \
                sinvz += dinvz;                                                 \
                sy += dy;                                                       \
                sz += dz;                                                       \
                sx += dx;                                                       \
                /* handle the attribute increment */                            \
                for (i=0; i<MAX_TEXUNITS; ++i) {                                \
                    st[i]._v[0] += dt[i]._v[0];                                 \
                    st[i]._v[1] += dt[i]._v[1];                                 \
                    st[i]._v[2] += dt[i]._v[2];                                 \
                    st[i]._v[3] += dt[i]._v[3];                                 \
                }                                                               \
                sc._v[0] += dc._v[0];                                           \
                sc._v[1] += dc._v[1];                                           \
                sc._v[2] += dc._v[2];                                           \
                sc._v[3] += dc._v[3];                                           \
            }

/* draw line */
static void zb_draw_line(PtrGLContext context, PtrGLVertex vertex0, PtrGLVertex vertex1)
{
    GLfloat x, y, z, wInv0, wInv1;
    GLfloat xscale, yscale, zscale, xcenter, ycenter, zcenter;
    GLfloat winx0, winy0, winz0, winx1, winy1, winz1;

    xscale = context->_viewport._xScale;
    yscale = context->_viewport._yScale;
    zscale = context->_viewport._zScale;
    xcenter= context->_viewport._xCenter;
    ycenter= context->_viewport._yCenter;
    zcenter= context->_viewport._zCenter;

    /* vertex 0 */
    x = vertex0->_pro_coord._v[0];
    y = vertex0->_pro_coord._v[1];
    z = vertex0->_pro_coord._v[2];
    wInv0 = 1.f / vertex0->_pro_coord._v[3];
    winx0 = (x * wInv0 * xscale + xcenter);
    winy0 = (y * wInv0 * yscale + ycenter);
    winz0 = (z * wInv0 * zscale + zcenter);

    /* vertex 1 */
    x = vertex1->_pro_coord._v[0];
    y = vertex1->_pro_coord._v[1];
    z = vertex1->_pro_coord._v[2];
    wInv1 = 1.f / vertex1->_pro_coord._v[3];
    winx1 = (x * wInv1 * xscale + xcenter);
    winy1 = (y * wInv1 * yscale + ycenter);
    winz1 = (z * wInv1 * zscale + zcenter);

    /*
        Attribute Bt, B0, B1.
              B0/Z0 * (1-t) + B1/Z1 * t
        Bt = ---------------------------
               1/Z0 * (1-t) +  1/Z1 * t

        Xt = X0 * (1-t) + X1 * t   ==>   t = (Xt - X0) / (X1 - X0)
        Yt = Y0 * (1-t) + Y1 * t   ==>   t = (Yt - Y0) / (Y1 - Y0)

        令 Bt = C / D
        C = B0/Z0 + (B1/Z1 - B0/Z0) * t = B0/Z0 + (B1/Z1 - B0/Z0) * [(Xt - X0) / (X1 - X0)]
        D = 1/Z0  + ( 1/Z1 -  1/Z0) * t =  1/Z0 + ( 1/Z1 -  1/Z0) * [(Xt - X0) / (X1 - X0)]

        增量形式如下: Xt + 1
        delC = (B1/Z1 - B0/Z0) / (X1 - X0) = (B1/Z1 - B0/Z0) / (Y1 - Y0)
        delD = ( 1/Z1 -  1/Z0) / (X1 - X0) = ( 1/Z1 -  1/Z0) / (Y1 - Y0)

        DelZ = (winz1 - winz0) / (X1 - X0) = (winz1 - winz0) / (Y1 - Y0)
     */
    {
        GLFragment fragment;
        Vector4 st[MAX_TEXUNITS], dt[MAX_TEXUNITS]; /* 起始texture值, 递增量 */
        Vector4 sc, dc;                             /* 起始color值, 递增量   */
        GLfloat sinvz, dinvz;                       /* 起始的D(上公式中)值, 递增量 */
        GLfloat sx, sy, sz, dx, dy, dz;             /* 起始的x,y,z和递增量   */
        GLfloat oneOverInvz;
        GLfloat xdiff = winx1 - winx0;
        GLfloat ydiff = winy1 - winy0;
        GLint i;

        if (xdiff == 0.f && ydiff == 0.f) {
            printf("INFO: zb_draw_line(), xdiff == ydiff == 0 will not draw line\n");
            return;
        }

        if (fabs(xdiff) > fabs(ydiff)) {
            GLfloat scope = 1.0f / (winx1 - winx0);
            for (i=0; i<MAX_TEXUNITS; ++i) {
                dt[i]._v[0] = (vertex1->_texcoords[i]._v[0] * wInv1 - vertex0->_texcoords[i]._v[0] * wInv0) * scope;
                dt[i]._v[1] = (vertex1->_texcoords[i]._v[1] * wInv1 - vertex0->_texcoords[i]._v[1] * wInv0) * scope;
                dt[i]._v[2] = (vertex1->_texcoords[i]._v[2] * wInv1 - vertex0->_texcoords[i]._v[2] * wInv0) * scope;
                dt[i]._v[3] = (vertex1->_texcoords[i]._v[3] * wInv1 - vertex0->_texcoords[i]._v[3] * wInv0) * scope;
            }
            dc._v[0] = (vertex1->_front_color._v[0] * wInv1 - vertex0->_front_color._v[0] * wInv0) * scope;
            dc._v[1] = (vertex1->_front_color._v[1] * wInv1 - vertex0->_front_color._v[1] * wInv0) * scope;
            dc._v[2] = (vertex1->_front_color._v[2] * wInv1 - vertex0->_front_color._v[2] * wInv0) * scope;
            dc._v[3] = (vertex1->_front_color._v[3] * wInv1 - vertex0->_front_color._v[3] * wInv0) * scope;
            dinvz = (wInv1 - wInv0) * scope;
            dz = (winz1 - winz0) * scope;
            dy = (winy1 - winy0) * scope;
            dx = 1.0f;

            /* from the min-x winx0*/
            if (winx0 < winx1) {
                for (i=0; i<MAX_TEXUNITS; ++i) {
                    st[i]._v[0] = vertex0->_texcoords[i]._v[0] * wInv0;
                    st[i]._v[1] = vertex0->_texcoords[i]._v[1] * wInv0;
                    st[i]._v[2] = vertex0->_texcoords[i]._v[2] * wInv0;
                    st[i]._v[3] = vertex0->_texcoords[i]._v[3] * wInv0;
                }
                sc._v[0] = vertex0->_front_color._v[0] * wInv0;
                sc._v[1] = vertex0->_front_color._v[1] * wInv0;
                sc._v[2] = vertex0->_front_color._v[2] * wInv0;
                sc._v[3] = vertex0->_front_color._v[3] * wInv0;
                sinvz = wInv0;
                sz = winz0;
                sy = winy0;
                sx = winx0;
                
                /* incremently to calculate */
                while (sx <= winx1) {
                    __RASTER_LINE()
                }

            /* from winx1 */
            } else {
                for (i=0; i<MAX_TEXUNITS; ++i) {
                    st[i]._v[0] = vertex1->_texcoords[i]._v[0] * wInv1;
                    st[i]._v[1] = vertex1->_texcoords[i]._v[1] * wInv1;
                    st[i]._v[2] = vertex1->_texcoords[i]._v[2] * wInv1;
                    st[i]._v[3] = vertex1->_texcoords[i]._v[3] * wInv1;
                }
                sc._v[0] = vertex1->_front_color._v[0] * wInv1;
                sc._v[1] = vertex1->_front_color._v[1] * wInv1;
                sc._v[2] = vertex1->_front_color._v[2] * wInv1;
                sc._v[3] = vertex1->_front_color._v[3] * wInv1;
                sinvz = wInv1;
                sz = winz1;
                sy = winy1;
                sx = winx1;
                
                /* incremently to calculate */
                while (sx <= winx0) {
                    __RASTER_LINE()
                }
            }
        /* diffy is long than diffx */
        } else {
            GLfloat scope = 1.0f / (winy1 - winy0);
            for (i=0; i<MAX_TEXUNITS; ++i) {
                dt[i]._v[0] = (vertex1->_texcoords[i]._v[0] * wInv1 - vertex0->_texcoords[i]._v[0] * wInv0) * scope;
                dt[i]._v[1] = (vertex1->_texcoords[i]._v[1] * wInv1 - vertex0->_texcoords[i]._v[1] * wInv0) * scope;
                dt[i]._v[2] = (vertex1->_texcoords[i]._v[2] * wInv1 - vertex0->_texcoords[i]._v[2] * wInv0) * scope;
                dt[i]._v[3] = (vertex1->_texcoords[i]._v[3] * wInv1 - vertex0->_texcoords[i]._v[3] * wInv0) * scope;
            }
            dc._v[0] = (vertex1->_front_color._v[0] * wInv1 - vertex0->_front_color._v[0] * wInv0) * scope;
            dc._v[1] = (vertex1->_front_color._v[1] * wInv1 - vertex0->_front_color._v[1] * wInv0) * scope;
            dc._v[2] = (vertex1->_front_color._v[2] * wInv1 - vertex0->_front_color._v[2] * wInv0) * scope;
            dc._v[3] = (vertex1->_front_color._v[3] * wInv1 - vertex0->_front_color._v[3] * wInv0) * scope;
            dinvz = (wInv1 - wInv0) * scope;
            dz = (winz1 - winz0) * scope;
            dx = (winx1 - winx0) * scope;
            dy = 1.0f;
            
            if (winy0 < winy1) {
                for (i=0; i<MAX_TEXUNITS; ++i) {
                    st[i]._v[0] = vertex0->_texcoords[i]._v[0] * wInv0;
                    st[i]._v[1] = vertex0->_texcoords[i]._v[1] * wInv0;
                    st[i]._v[2] = vertex0->_texcoords[i]._v[2] * wInv0;
                    st[i]._v[3] = vertex0->_texcoords[i]._v[3] * wInv0;
                }
                sc._v[0] = vertex0->_front_color._v[0] * wInv0;
                sc._v[1] = vertex0->_front_color._v[1] * wInv0;
                sc._v[2] = vertex0->_front_color._v[2] * wInv0;
                sc._v[3] = vertex0->_front_color._v[3] * wInv0;
                sinvz = wInv0;
                sz = winz0;
                sy = winy0;
                sx = winx0;
                
                /* incremently to calculate */
                while (sy <= winy1) {
                    __RASTER_LINE()
                }
            } else {
                for (i=0; i<MAX_TEXUNITS; ++i) {
                    st[i]._v[0] = vertex1->_texcoords[i]._v[0] * wInv1;
                    st[i]._v[1] = vertex1->_texcoords[i]._v[1] * wInv1;
                    st[i]._v[2] = vertex1->_texcoords[i]._v[2] * wInv1;
                    st[i]._v[3] = vertex1->_texcoords[i]._v[3] * wInv1;
                }
                sc._v[0] = vertex1->_front_color._v[0] * wInv1;
                sc._v[1] = vertex1->_front_color._v[1] * wInv1;
                sc._v[2] = vertex1->_front_color._v[2] * wInv1;
                sc._v[3] = vertex1->_front_color._v[3] * wInv1;
                sinvz = wInv1;
                sz = winz1;
                sy = winy1;
                sx = winx1;
                
                /* incremently to calculate */
                while (sy <= winy0) {
                    __RASTER_LINE()
                }
            }

        } /* end if */
    }
}

/* \brief
 *      draw line
 */
void gl_draw_line(PtrGLContext pContext, PtrGLVertex vertex0, PtrGLVertex vertex1)
{
    int cc0, cc1;
    
    cc0 = vertex0->_clip_code;
    cc1 = vertex1->_clip_code;
    /* test if v0, v1 both in the volumn */
    if ((cc0 | cc1) == 0) {
        zb_draw_line(pContext, vertex0, vertex1);
    } 
    /* test if v0, v1 both out of the volumn */
    else if ((cc0 & cc1) != 0) {
        return;
    } 
    /* only one in the volumn */
    else {
        /* we calculate the distance between vertex to planes: 
         1. < 1,  0,  0, 1>    left
         2. <-1,  0,  0, 1>    right
         3. < 0,  0,  1, 1>    front
         4. < 0,  0, -1, 1>    back
         5. < 0, -1,  0, 1>    top
         6. < 0,  1,  0, 1>    bottom

         Dist(v0, left)  =  x0 + w0, Dist(v1, left)  =  x1 + w1;
         Dist(v0, right) = -x0 + w0, Dist(v1, right) = -x1 + w1;
         Dist(v0, front) =  z0 + w0, Dist(v1, front) =  z1 + w1;
         Dist(v0, back)  = -z0 + w0, Dist(v1, back)  = -z1 + w1;
         Dist(v0, top)   = -y0 + w0, Dist(v1, top)   = -y1 + w1;
         Dist(v0, bottom)=  y0 + w0, Dist(v1, bottom)=  y1 + w1;
        */
        GLVertex vt0, vt1;
        GLfloat tmin = 0.f, tmax = 1.f;

        GLfloat dx = vertex0->_pro_coord._v[0] - vertex1->_pro_coord._v[0];
        GLfloat dy = vertex0->_pro_coord._v[1] - vertex1->_pro_coord._v[1];
        GLfloat dz = vertex0->_pro_coord._v[2] - vertex1->_pro_coord._v[2];
        GLfloat dw = vertex0->_pro_coord._v[3] - vertex1->_pro_coord._v[3];
        GLfloat x0 = vertex0->_pro_coord._v[0];
        GLfloat y0 = vertex0->_pro_coord._v[1];
        GLfloat z0 = vertex0->_pro_coord._v[2];
        GLfloat w0 = vertex0->_pro_coord._v[3];
        
        clip_line(dx+dw, x0+w0, &tmin, &tmax);
        clip_line(-dx+dw, -x0+w0, &tmin, &tmax);
        clip_line(dz+dw, z0+w0, &tmin, &tmax);
        clip_line(-dz+dw, -z0+w0, &tmin, &tmax);
        clip_line(-dy+dw, -y0+w0, &tmin, &tmax);
        clip_line(dy+dw, y0+w0, &tmin, &tmax);

        /* do interpolate */
        interpolate(&vt0, vertex0, vertex1, tmin);
        interpolate(&vt1, vertex0, vertex1, tmax);
        /* rasterlization */
        zb_draw_line(pContext, &vt0, &vt1);
    }
}

/* \brief 
 *      clip the polygon to the plane
 */
static GLint clip_to_plane(PtrGLContext pContext, GLVertex **iv, GLint niv, GLVertex **ov, PtrVector4 plane)
{
    GLint i, nout, generated;
    GLVertex *s, *p, *newVertex, *temp;
    GLfloat pDist, sDist, t;

    nout = 0;
    generated = 0;
    temp = pContext->_next_clip_temp;
    s = iv[niv - 1];
    sDist = (s->_pro_coord._v[0] * plane->_v[0]) + (s->_pro_coord._v[1] * plane->_v[1]) +
        (s->_pro_coord._v[2] * plane->_v[2]) + (s->_pro_coord._v[3] * plane->_v[3]);
    for (i=0; i<niv; ++i) {
        p = iv[i];
        pDist = (p->_pro_coord._v[0] * plane->_v[0]) + (p->_pro_coord._v[1] * plane->_v[1]) +
            (p->_pro_coord._v[2] * plane->_v[2]) + (p->_pro_coord._v[3] * plane->_v[3]);

        if (pDist >= 0.f) {
            /* p is inside the clipping plane */
            if (pDist == 0.f || sDist >= 0.f) {
            /* s is inside the clipping plane */
                *ov++ = p;
                nout++;
            } else {
                /* s is outside the clipping plane */
                t = pDist / (pDist - sDist);
                newVertex = temp++;
                interpolate(newVertex, p, s, t);
                *ov++ = newVertex;
                *ov++ = p;
                nout += 2;
                if (++generated >= 3) {
                    /* Toss the non-convex polygon */
                    return 0;
                }
            }
        } /* p is outside of the plane */
        else {
            if (sDist > 0.f) {
                t = sDist / (sDist - pDist);
                newVertex = temp++;
                interpolate(newVertex, s, p, t);
                *ov++ = newVertex;
                nout++;
                if (++generated >= 3) {
                    /* Toss the non-convex polygon */
                    return 0;                    
                }
            }
            /* else { discard s & p } */
        }

        s = p;
        sDist = pDist;
    } /* end for i*/

    pContext->_next_clip_temp = temp;
    return nout;
}

/* the six planes of volumns */
static Vector4 frustum_planes[6] = {
    { 1.f,  0.f,  0.f, 1.f },   /* left  */
    {-1.f,  0.f,  0.f, 1.f },   /* right */
    { 0.f,  0.f,  1.f, 1.f },   /* front */
    { 0.f,  0.f, -1.f, 1.f },   /* back  */
    { 0.f, -1.f,  0.f, 1.f },   /* top   */
    { 0.f,  1.f,  0.f, 1.f }    /* bottom*/
};

static void zb_draw_triangle(PtrGLContext pContext, PtrGLVertex v0, PtrGLVertex v1, PtrGLVertex v2, GLboolean front);

/* \brief
 *      draw triangle
 */
void gl_draw_triangle(PtrGLContext pContext, PtrGLVertex vertex0, PtrGLVertex vertex1, PtrGLVertex vertex2)
{
    Vector3 normal, A, B, Z;
    GLfloat dot;
    GLboolean front;
    int cc0, cc1, cc2;

#ifdef QUERY_PERFORMANCE
    begin_performance();
#endif

    cc0 = vertex0->_clip_code;
    cc1 = vertex1->_clip_code;
    cc2 = vertex2->_clip_code;

    if ((cc0 & cc1 & cc2) != 0) {
        /* 3 vertex all clip out */
        return;
    }

    /* compute the face of triangle */
    A._v[0] = vertex1->_eye_coord._v[0] - vertex0->_eye_coord._v[0];
    A._v[1] = vertex1->_eye_coord._v[1] - vertex0->_eye_coord._v[1];
    A._v[2] = vertex1->_eye_coord._v[2] - vertex0->_eye_coord._v[2];
    B._v[0] = vertex2->_eye_coord._v[0] - vertex1->_eye_coord._v[0];
    B._v[1] = vertex2->_eye_coord._v[1] - vertex1->_eye_coord._v[1];
    B._v[2] = vertex2->_eye_coord._v[2] - vertex1->_eye_coord._v[2];
    Z._v[0] = 0.f; Z._v[1] = 0.f; Z._v[2] = 1.0f;
    cross_vector3(&A, &B, &normal);
    dot = dot_vector3(&normal, &Z);

    if (dot == 0.f) {
        /* the projection of three vertexes is one the line */
        return;
    }

    front = GL_FALSE;
    if (pContext->_fontface_mode == GL_CCW && dot > 0.f) {
        front = GL_TRUE;
    }
    if (pContext->_fontface_mode == GL_CW && dot < 0.f) {
        front = GL_TRUE;
    }

    /* test cull face */
    if (pContext->_cull_face_on) {
        switch (pContext->_cull_mode) {
        case GL_FRONT:
            if (front == GL_TRUE) return;
            break;
        case GL_BACK:
            if (front == GL_FALSE) return;
            break;
        default: /* GL_FRONT_BACK */
            return;
        }
    }

    /* clip the triangle && rasterlization */
    {
        GLfloat xscale, yscale, zscale, xcenter, ycenter, zcenter, minx, maxx, miny, maxy, winv;
        GLVertex *triangle[3] = {vertex0, vertex1, vertex2};
        GLVertex **iv, *ov[6][9] = { 0 };
        GLint i, niv;
        
        niv = 3;
        iv = triangle;
        pContext->_next_clip_temp = pContext->_vertex_clip_temp;
        for (i=0; i<6; i++) {
            niv = clip_to_plane(pContext, iv, niv, &ov[i][0], &frustum_planes[i]);
            if (niv < 3) return;
            iv = &ov[i][0];
        }

        /* iv 中存放多边形的点 */
        xscale = pContext->_viewport._xScale;
        yscale = pContext->_viewport._yScale;
        zscale = pContext->_viewport._zScale;
        xcenter= pContext->_viewport._xCenter;
        ycenter= pContext->_viewport._yCenter;
        zcenter= pContext->_viewport._zCenter;
        minx = xcenter - xscale;
        maxx = xcenter + xscale;
        miny = ycenter - yscale;
        maxy = ycenter + yscale;
        for (i=0; i<niv; i++) {
            winv = 1.f / iv[i]->_pro_coord._v[3];
            iv[i]->_win_coord._v[0] = iv[i]->_pro_coord._v[0] * winv * xscale + xcenter;
            iv[i]->_win_coord._v[1] = iv[i]->_pro_coord._v[1] * winv * yscale + ycenter;
            iv[i]->_win_coord._v[2] = iv[i]->_pro_coord._v[2] * winv * zscale + zcenter;
            iv[i]->_win_coord._v[3] = winv;
            /* is x,y out of the range? */
            if (iv[i]->_win_coord._v[0] < minx) 
                iv[i]->_win_coord._v[0] = minx;
            else if (iv[i]->_win_coord._v[0] > maxx) 
                iv[i]->_win_coord._v[0] = maxx;
            if (iv[i]->_win_coord._v[1] < miny) 
                iv[i]->_win_coord._v[1] = miny;
            else if (iv[i]->_win_coord._v[1] > maxy) 
                iv[i]->_win_coord._v[1] = maxy;
        }

        /* 将多边形分割成多个三角形后分别光栅化 */
        {
            GLVertex *v0, *v1, *v2;

            v0 = iv[0]; v1 = iv[1]; v2 = iv[2];
            for (i=2; i<niv; i++) {
                v2 = iv[i];
                zb_draw_triangle(pContext, v0, v1, v2, front);
                v1 = v2;
            } /* end for i */
        }

    }

#ifdef QUERY_PERFORMANCE
    end_performance("gl_draw_triangle");
#endif
}

/* =====================绘制三角形(左上填充规则)======================================== */
#define SWAP(x, y, tmp)  { tmp = x; x = y; y = tmp; }
#define FCMP(x, y)  (fabs((x)-(y)) < 1e-3 ? 1 : 0)

/*
        Attribute Bt, B0, B1.
              B0/Z0 * (1-t) + B1/Z1 * t
        Bt = ---------------------------
               1/Z0 * (1-t) +  1/Z1 * t

        Xt = X0 * (1-t) + X1 * t   ==>   t = (Xt - X0) / (X1 - X0)
        Yt = Y0 * (1-t) + Y1 * t   ==>   t = (Yt - Y0) / (Y1 - Y0)

        令 Bt = C / D
        C = B0/Z0 + (B1/Z1 - B0/Z0) * t = B0/Z0 + (B1/Z1 - B0/Z0) * [(Xt - X0) / (X1 - X0)]
        D = 1/Z0  + ( 1/Z1 -  1/Z0) * t =  1/Z0 + ( 1/Z1 -  1/Z0) * [(Xt - X0) / (X1 - X0)]

        增量形式如下: Xt + 1
        delC = (B1/Z1 - B0/Z0) / (X1 - X0) = (B1/Z1 - B0/Z0) / (Y1 - Y0)
        delD = ( 1/Z1 -  1/Z0) / (X1 - X0) = ( 1/Z1 -  1/Z0) / (Y1 - Y0)

        DelZ = (winz1 - winz0) / (X1 - X0) = (winz1 - winz0) / (Y1 - Y0)
*/

/* 绘制线段 */
static void zb_span_line(PtrGLContext context, const PtrVector4 vl, const PtrVector4 col_l, const Vector4 tex_l[MAX_TEXUNITS], const PtrVector4 vr,
                         const PtrVector4 col_r, const Vector4 tex_r[MAX_TEXUNITS])
{
    GLFragment fragment;
    GLfloat delX, sX, eX, eyez;
    Vector4 tex_s[MAX_TEXUNITS], tex_dx[MAX_TEXUNITS];
    Vector4 col_s, col_dx;
    Vector4 win_s, win_dx;
    int i;

    delX = vr->_v[0] - vl->_v[0];
    delX = 1.f / delX;
    copy_vector4(&win_s, vl);
    win_dx._v[0] = 1.f;
    win_dx._v[1] = 0.f;
    win_dx._v[2] = (vr->_v[2] - vl->_v[2]) * delX;
    win_dx._v[3] = (vr->_v[3] - vl->_v[3]) * delX;
    copy_vector4(&col_s, col_l);
    col_dx._v[0] = (col_r->_v[0] - col_l->_v[0]) * delX;
    col_dx._v[1] = (col_r->_v[1] - col_l->_v[1]) * delX;
    col_dx._v[2] = (col_r->_v[2] - col_l->_v[2]) * delX;
    col_dx._v[3] = (col_r->_v[3] - col_l->_v[3]) * delX;
    for (i=0; i<MAX_TEXUNITS; ++i) {
        copy_vector4(&tex_s[i], &tex_l[i]);
        tex_dx[i]._v[0] = (tex_r[i]._v[0] - tex_l[i]._v[0]) * delX;
        tex_dx[i]._v[1] = (tex_r[i]._v[1] - tex_l[i]._v[1]) * delX;
        tex_dx[i]._v[2] = (tex_r[i]._v[2] - tex_l[i]._v[2]) * delX;
        tex_dx[i]._v[3] = (tex_r[i]._v[3] - tex_l[i]._v[3]) * delX;
    }

    sX = ceilf(vl->_v[0]);
    eX = ceilf(vr->_v[0]) - 1.f;
    win_s._v[0] = sX;
/* {{@ 加入由ceilf造成的误差校正 2010-11-15 */
    {
        GLfloat err = sX - vl->_v[0];

        win_s._v[2] += win_dx._v[2] * err;
        win_s._v[3] += win_dx._v[3] * err;
        col_s._v[0] += col_dx._v[0] * err;
        col_s._v[1] += col_dx._v[1] * err;
        col_s._v[2] += col_dx._v[2] * err;
        col_s._v[3] += col_dx._v[3] * err;
        for (i=0; i<MAX_TEXUNITS; ++i) {
            tex_s[i]._v[0] += tex_dx[i]._v[0] * err;
            tex_s[i]._v[1] += tex_dx[i]._v[1] * err;
            tex_s[i]._v[2] += tex_dx[i]._v[2] * err;
            tex_s[i]._v[3] += tex_dx[i]._v[3] * err;
        }
    }
/* }}@ */

    for (; sX<=eX; sX+=1.f) {
        fragment._winX = (GLint)(win_s._v[0]);     win_s._v[0] += win_dx._v[0];
        fragment._winY = (GLint)(win_s._v[1]);     win_s._v[1] += win_dx._v[1];
        fragment._winZ = (GLushort)(win_s._v[2] + 0.5f);  win_s._v[2] += win_dx._v[2];
        eyez = 1.f / win_s._v[3];                         win_s._v[3] += win_dx._v[3];
        fragment._color._v[0] = col_s._v[0] * eyez;  col_s._v[0] += col_dx._v[0];
        fragment._color._v[1] = col_s._v[1] * eyez;  col_s._v[1] += col_dx._v[1];
        fragment._color._v[2] = col_s._v[2] * eyez;  col_s._v[2] += col_dx._v[2];
        fragment._color._v[3] = col_s._v[3] * eyez;  col_s._v[3] += col_dx._v[3];
        for (i=0; i<MAX_TEXUNITS; ++i) {
            fragment._texcoords[i]._v[0] = tex_s[i]._v[0] * eyez;  tex_s[i]._v[0] += tex_dx[i]._v[0];
            fragment._texcoords[i]._v[1] = tex_s[i]._v[1] * eyez;  tex_s[i]._v[1] += tex_dx[i]._v[1]; 
            fragment._texcoords[i]._v[2] = tex_s[i]._v[2] * eyez;  tex_s[i]._v[2] += tex_dx[i]._v[2];
            fragment._texcoords[i]._v[3] = tex_s[i]._v[3] * eyez;  tex_s[i]._v[3] += tex_dx[i]._v[3];
        }

        gl_process_fragment(context, &fragment);
    } /* end for */
}

/* 绘制平顶的三角形
                V0        V1
 
                     V2
 */
static void zb_draw_top_triangle(PtrGLContext pContext, PtrGLVertex v0, PtrGLVertex v1, PtrGLVertex v2, GLboolean front)
{
    PtrGLVertex tmp;
    GLfloat delY_l, delY_r, startY, endY, errY;
    Vector4 vex_sl, vex_sr, vex_dyl, vex_dyr;
    Vector4 tex_sl[MAX_TEXUNITS], tex_sr[MAX_TEXUNITS], tex_dyr[MAX_TEXUNITS], tex_dyl[MAX_TEXUNITS];
    Vector4 col_sl, col_sr, col_dyr, col_dyl;
    int i;

    /* 对v0, v1按x从小到大排序 */
    if (v0->_win_coord._v[0] > v1->_win_coord._v[0])
        SWAP(v0, v1, tmp);

    startY = floor(v2->_win_coord._v[1]) + 1.f;
    endY   = floor(v0->_win_coord._v[1]);
    delY_l = v0->_win_coord._v[1] - v2->_win_coord._v[1];
    delY_r = v1->_win_coord._v[1] - v2->_win_coord._v[1];
    delY_l = 1.f / delY_l;
    delY_r = 1.f / delY_r;
    vex_dyl._v[0] = (v0->_win_coord._v[0] - v2->_win_coord._v[0]) * delY_l;
    vex_dyr._v[0] = (v1->_win_coord._v[0] - v2->_win_coord._v[0]) * delY_r;
    vex_dyl._v[1] = 1.f;
    vex_dyr._v[1] = 1.f;
    vex_dyl._v[2] = (v0->_win_coord._v[2] - v2->_win_coord._v[2]) * delY_l;
    vex_dyr._v[2] = (v1->_win_coord._v[2] - v2->_win_coord._v[2]) * delY_r;
    vex_dyl._v[3] = (v0->_win_coord._v[3] - v2->_win_coord._v[3]) * delY_l;
    vex_dyr._v[3] = (v1->_win_coord._v[3] - v2->_win_coord._v[3]) * delY_r;

    errY = (startY - v2->_win_coord._v[1]);
    vex_sl._v[0] = v2->_win_coord._v[0] + vex_dyl._v[0] * errY;
    vex_sr._v[0] = v2->_win_coord._v[0] + vex_dyr._v[0] * errY;
    vex_sl._v[1] = startY;
    vex_sr._v[1] = startY;
    vex_sl._v[2] = v2->_win_coord._v[2] + vex_dyl._v[2] * errY;
    vex_sr._v[2] = v2->_win_coord._v[2] + vex_dyr._v[2] * errY; 
    vex_sl._v[3] = v2->_win_coord._v[3] + vex_dyl._v[3] * errY;
    vex_sr._v[3] = v2->_win_coord._v[3] + vex_dyr._v[3] * errY;

    /* color */
    if (front == GL_TRUE) {
        col_sl._v[0] = v2->_front_color._v[0] * vex_sl._v[3];
        col_sl._v[1] = v2->_front_color._v[1] * vex_sl._v[3];
        col_sl._v[2] = v2->_front_color._v[2] * vex_sl._v[3];
        col_sl._v[3] = v2->_front_color._v[3] * vex_sl._v[3];
        copy_vector4(&col_sr, &col_sl);
        col_dyl._v[0] = (v0->_front_color._v[0] * v0->_win_coord._v[3] - col_sl._v[0]) * delY_l;
        col_dyl._v[1] = (v0->_front_color._v[1] * v0->_win_coord._v[3] - col_sl._v[1]) * delY_l;
        col_dyl._v[2] = (v0->_front_color._v[2] * v0->_win_coord._v[3] - col_sl._v[2]) * delY_l;
        col_dyl._v[3] = (v0->_front_color._v[3] * v0->_win_coord._v[3] - col_sl._v[3]) * delY_l;
        col_dyr._v[0] = (v1->_front_color._v[0] * v1->_win_coord._v[3] - col_sr._v[0]) * delY_r;
        col_dyr._v[1] = (v1->_front_color._v[1] * v1->_win_coord._v[3] - col_sr._v[1]) * delY_r;
        col_dyr._v[2] = (v1->_front_color._v[2] * v1->_win_coord._v[3] - col_sr._v[2]) * delY_r;
        col_dyr._v[3] = (v1->_front_color._v[3] * v1->_win_coord._v[3] - col_sr._v[3]) * delY_r;
    } else {
        col_sl._v[0] = v2->_back_color._v[0] * vex_sl._v[3];
        col_sl._v[1] = v2->_back_color._v[1] * vex_sl._v[3];
        col_sl._v[2] = v2->_back_color._v[2] * vex_sl._v[3];
        col_sl._v[3] = v2->_back_color._v[3] * vex_sl._v[3];
        copy_vector4(&col_sr, &col_sl);
        col_dyl._v[0] = (v0->_back_color._v[0] * v0->_win_coord._v[3] - col_sl._v[0]) * delY_l;
        col_dyl._v[1] = (v0->_back_color._v[1] * v0->_win_coord._v[3] - col_sl._v[1]) * delY_l;
        col_dyl._v[2] = (v0->_back_color._v[2] * v0->_win_coord._v[3] - col_sl._v[2]) * delY_l;
        col_dyl._v[3] = (v0->_back_color._v[3] * v0->_win_coord._v[3] - col_sl._v[3]) * delY_l;
        col_dyr._v[0] = (v1->_back_color._v[0] * v1->_win_coord._v[3] - col_sr._v[0]) * delY_r;
        col_dyr._v[1] = (v1->_back_color._v[1] * v1->_win_coord._v[3] - col_sr._v[1]) * delY_r;
        col_dyr._v[2] = (v1->_back_color._v[2] * v1->_win_coord._v[3] - col_sr._v[2]) * delY_r;
        col_dyr._v[3] = (v1->_back_color._v[3] * v1->_win_coord._v[3] - col_sr._v[3]) * delY_r;
    }

    /* texture coords */
    for (i=0; i<MAX_TEXUNITS; ++i) {
        tex_sl[i]._v[0] = v2->_texcoords[i]._v[0] * vex_sl._v[3];
        tex_sl[i]._v[1] = v2->_texcoords[i]._v[1] * vex_sl._v[3];
        tex_sl[i]._v[2] = v2->_texcoords[i]._v[2] * vex_sl._v[3];
        tex_sl[i]._v[3] = v2->_texcoords[i]._v[3] * vex_sl._v[3];
        copy_vector4(&tex_sr[i], &tex_sl[i]);
        tex_dyl[i]._v[0] = (v0->_texcoords[i]._v[0] * v0->_win_coord._v[3] - tex_sl[i]._v[0]) * delY_l;
        tex_dyl[i]._v[1] = (v0->_texcoords[i]._v[1] * v0->_win_coord._v[3] - tex_sl[i]._v[1]) * delY_l;
        tex_dyl[i]._v[2] = (v0->_texcoords[i]._v[2] * v0->_win_coord._v[3] - tex_sl[i]._v[2]) * delY_l;
        tex_dyl[i]._v[3] = (v0->_texcoords[i]._v[3] * v0->_win_coord._v[3] - tex_sl[i]._v[3]) * delY_l;

        tex_dyr[i]._v[0] = (v1->_texcoords[i]._v[0] * v1->_win_coord._v[3] - tex_sr[i]._v[0]) * delY_r;
        tex_dyr[i]._v[1] = (v1->_texcoords[i]._v[1] * v1->_win_coord._v[3] - tex_sr[i]._v[1]) * delY_r;
        tex_dyr[i]._v[2] = (v1->_texcoords[i]._v[2] * v1->_win_coord._v[3] - tex_sr[i]._v[2]) * delY_r;
        tex_dyr[i]._v[3] = (v1->_texcoords[i]._v[3] * v1->_win_coord._v[3] - tex_sr[i]._v[3]) * delY_r;
    }
    
    /*
    FIXED: 添加floor造成的属性偏差. Jett huang, 2010-11-10
    {{@
    */
    /* color */
    col_sl._v[0] += col_dyl._v[0] * errY;
    col_sl._v[1] += col_dyl._v[1] * errY;
    col_sl._v[2] += col_dyl._v[2] * errY;
    col_sl._v[3] += col_dyl._v[3] * errY;
    col_sr._v[0] += col_dyr._v[0] * errY;
    col_sr._v[1] += col_dyr._v[1] * errY;
    col_sr._v[2] += col_dyr._v[2] * errY;
    col_sr._v[3] += col_dyr._v[3] * errY;
    
    /* texture */
    for (i=0; i<MAX_TEXUNITS; ++i) {
        tex_sl[i]._v[0] += tex_dyl[i]._v[0] * errY;
        tex_sl[i]._v[1] += tex_dyl[i]._v[1] * errY;
        tex_sl[i]._v[2] += tex_dyl[i]._v[2] * errY;
        tex_sl[i]._v[3] += tex_dyl[i]._v[3] * errY;
        tex_sr[i]._v[0] += tex_dyr[i]._v[0] * errY;
        tex_sr[i]._v[1] += tex_dyr[i]._v[1] * errY;
        tex_sr[i]._v[2] += tex_dyr[i]._v[2] * errY;
        tex_sr[i]._v[3] += tex_dyr[i]._v[3] * errY;
    }
    /*
    }}@
    */
    for (; startY <= endY; startY += 1.f) 
    {
        zb_span_line(pContext, &vex_sl, &col_sl, tex_sl, &vex_sr, &col_sr, tex_sr);
        vex_sl._v[0] += vex_dyl._v[0];
        vex_sl._v[1] += 1.f;
        vex_sl._v[2] += vex_dyl._v[2];
        vex_sl._v[3] += vex_dyl._v[3];
        vex_sr._v[0] += vex_dyr._v[0];
        vex_sr._v[1] += vex_dyr._v[1];
        vex_sr._v[2] += vex_dyr._v[2];
        vex_sr._v[3] += vex_dyr._v[3];
        /* color */
        col_sl._v[0] += col_dyl._v[0]; col_sr._v[0] += col_dyr._v[0];
        col_sl._v[1] += col_dyl._v[1]; col_sr._v[1] += col_dyr._v[1];
        col_sl._v[2] += col_dyl._v[2]; col_sr._v[2] += col_dyr._v[2];
        col_sl._v[3] += col_dyl._v[3]; col_sr._v[3] += col_dyr._v[3];
        /* texture coords */
        for (i=0; i<MAX_TEXUNITS; ++i) {
            tex_sl[i]._v[0] += tex_dyl[i]._v[0]; tex_sr[i]._v[0] += tex_dyr[i]._v[0];
            tex_sl[i]._v[1] += tex_dyl[i]._v[1]; tex_sr[i]._v[1] += tex_dyr[i]._v[1];
            tex_sl[i]._v[2] += tex_dyl[i]._v[2]; tex_sr[i]._v[2] += tex_dyr[i]._v[2];
            tex_sl[i]._v[3] += tex_dyl[i]._v[3]; tex_sr[i]._v[3] += tex_dyr[i]._v[3];
        }
    } /* end for Y */
}

/* 绘制平底的三角形
                 V0

         V1             V2
 */
static void zb_draw_bottom_triangle(PtrGLContext pContext, PtrGLVertex v0, PtrGLVertex v1, PtrGLVertex v2, GLboolean front)
{
    PtrGLVertex tmp;
    GLfloat delY_l, delY_r, startY, endY, errY;
    Vector4 vex_sl, vex_sr, vex_dyl, vex_dyr;
    Vector4 tex_sl[MAX_TEXUNITS], tex_sr[MAX_TEXUNITS], tex_dyr[MAX_TEXUNITS], tex_dyl[MAX_TEXUNITS];
    Vector4 col_sl, col_sr, col_dyr, col_dyl;
    int i;

    /* 对v1, v2按x从小到大排序 */
    if (v1->_win_coord._v[0] > v2->_win_coord._v[0])
        SWAP(v1, v2, tmp);

    startY = floor(v1->_win_coord._v[1]) + 1.f;
    endY   = floor(v0->_win_coord._v[1]);
    delY_l = v0->_win_coord._v[1] - v1->_win_coord._v[1];
    delY_r = v0->_win_coord._v[1] - v2->_win_coord._v[1];
    delY_l = 1.f / delY_l;
    delY_r = 1.f / delY_r;
    vex_dyl._v[0] = (v0->_win_coord._v[0] - v1->_win_coord._v[0]) * delY_l;
    vex_dyr._v[0] = (v0->_win_coord._v[0] - v2->_win_coord._v[0]) * delY_r;
    vex_dyl._v[1] = 1.f;
    vex_dyr._v[1] = 1.f;
    vex_dyl._v[2] = (v0->_win_coord._v[2] - v1->_win_coord._v[2]) * delY_l;
    vex_dyr._v[2] = (v0->_win_coord._v[2] - v2->_win_coord._v[2]) * delY_r;
    vex_dyl._v[3] = (v0->_win_coord._v[3] - v1->_win_coord._v[3]) * delY_l;
    vex_dyr._v[3] = (v0->_win_coord._v[3] - v2->_win_coord._v[3]) * delY_r;

    errY = (startY - v1->_win_coord._v[1]);
    vex_sl._v[0] = v1->_win_coord._v[0] + vex_dyl._v[0] * errY;
    vex_sr._v[0] = v2->_win_coord._v[0] + vex_dyr._v[0] * errY;
    vex_sl._v[1] = startY;
    vex_sr._v[1] = startY;
    vex_sl._v[2] = v1->_win_coord._v[2] + vex_dyl._v[2] * errY;
    vex_sr._v[2] = v2->_win_coord._v[2] + vex_dyr._v[2] * errY;
    vex_sl._v[3] = v1->_win_coord._v[3] + vex_dyl._v[3] * errY;
    vex_sr._v[3] = v2->_win_coord._v[3] + vex_dyr._v[3] * errY;

    /* color */
    if (front == GL_TRUE) {
        col_sl._v[0] = v1->_front_color._v[0] * vex_sl._v[3];
        col_sl._v[1] = v1->_front_color._v[1] * vex_sl._v[3];
        col_sl._v[2] = v1->_front_color._v[2] * vex_sl._v[3];
        col_sl._v[3] = v1->_front_color._v[3] * vex_sl._v[3];
        col_sr._v[0] = v2->_front_color._v[0] * vex_sr._v[3];
        col_sr._v[1] = v2->_front_color._v[1] * vex_sr._v[3];
        col_sr._v[2] = v2->_front_color._v[2] * vex_sr._v[3];
        col_sr._v[3] = v2->_front_color._v[3] * vex_sr._v[3];

        col_dyl._v[0] = (v0->_front_color._v[0] * v0->_win_coord._v[3] - col_sl._v[0]) * delY_l;
        col_dyl._v[1] = (v0->_front_color._v[1] * v0->_win_coord._v[3] - col_sl._v[1]) * delY_l;
        col_dyl._v[2] = (v0->_front_color._v[2] * v0->_win_coord._v[3] - col_sl._v[2]) * delY_l;
        col_dyl._v[3] = (v0->_front_color._v[3] * v0->_win_coord._v[3] - col_sl._v[3]) * delY_l;
        col_dyr._v[0] = (v0->_front_color._v[0] * v0->_win_coord._v[3] - col_sr._v[0]) * delY_r;
        col_dyr._v[1] = (v0->_front_color._v[1] * v0->_win_coord._v[3] - col_sr._v[1]) * delY_r;
        col_dyr._v[2] = (v0->_front_color._v[2] * v0->_win_coord._v[3] - col_sr._v[2]) * delY_r;
        col_dyr._v[3] = (v0->_front_color._v[3] * v0->_win_coord._v[3] - col_sr._v[3]) * delY_r;
    } else {
        col_sl._v[0] = v1->_back_color._v[0] * vex_sl._v[3];
        col_sl._v[1] = v1->_back_color._v[1] * vex_sl._v[3];
        col_sl._v[2] = v1->_back_color._v[2] * vex_sl._v[3];
        col_sl._v[3] = v1->_back_color._v[3] * vex_sl._v[3];
        col_sr._v[0] = v2->_back_color._v[0] * vex_sr._v[3];
        col_sr._v[1] = v2->_back_color._v[1] * vex_sr._v[3];
        col_sr._v[2] = v2->_back_color._v[2] * vex_sr._v[3];
        col_sr._v[3] = v2->_back_color._v[3] * vex_sr._v[3];

        col_dyl._v[0] = (v0->_back_color._v[0] * v0->_win_coord._v[3] - col_sl._v[0]) * delY_l;
        col_dyl._v[1] = (v0->_back_color._v[1] * v0->_win_coord._v[3] - col_sl._v[1]) * delY_l;
        col_dyl._v[2] = (v0->_back_color._v[2] * v0->_win_coord._v[3] - col_sl._v[2]) * delY_l;
        col_dyl._v[3] = (v0->_back_color._v[3] * v0->_win_coord._v[3] - col_sl._v[3]) * delY_l;
        col_dyr._v[0] = (v0->_back_color._v[0] * v0->_win_coord._v[3] - col_sr._v[0]) * delY_r;
        col_dyr._v[1] = (v0->_back_color._v[1] * v0->_win_coord._v[3] - col_sr._v[1]) * delY_r;
        col_dyr._v[2] = (v0->_back_color._v[2] * v0->_win_coord._v[3] - col_sr._v[2]) * delY_r;
        col_dyr._v[3] = (v0->_back_color._v[3] * v0->_win_coord._v[3] - col_sr._v[3]) * delY_r;
    }

    /* texture coords */
    for (i=0; i<MAX_TEXUNITS; ++i) {
        tex_sl[i]._v[0] = v1->_texcoords[i]._v[0] * vex_sl._v[3];
        tex_sl[i]._v[1] = v1->_texcoords[i]._v[1] * vex_sl._v[3];
        tex_sl[i]._v[2] = v1->_texcoords[i]._v[2] * vex_sl._v[3];
        tex_sl[i]._v[3] = v1->_texcoords[i]._v[3] * vex_sl._v[3];
        tex_sr[i]._v[0] = v2->_texcoords[i]._v[0] * vex_sr._v[3];
        tex_sr[i]._v[1] = v2->_texcoords[i]._v[1] * vex_sr._v[3];
        tex_sr[i]._v[2] = v2->_texcoords[i]._v[2] * vex_sr._v[3];
        tex_sr[i]._v[3] = v2->_texcoords[i]._v[3] * vex_sr._v[3];

        tex_dyl[i]._v[0] = (v0->_texcoords[i]._v[0] * v0->_win_coord._v[3] - tex_sl[i]._v[0]) * delY_l;
        tex_dyl[i]._v[1] = (v0->_texcoords[i]._v[1] * v0->_win_coord._v[3] - tex_sl[i]._v[1]) * delY_l;
        tex_dyl[i]._v[2] = (v0->_texcoords[i]._v[2] * v0->_win_coord._v[3] - tex_sl[i]._v[2]) * delY_l;
        tex_dyl[i]._v[3] = (v0->_texcoords[i]._v[3] * v0->_win_coord._v[3] - tex_sl[i]._v[3]) * delY_l;
        tex_dyr[i]._v[0] = (v0->_texcoords[i]._v[0] * v0->_win_coord._v[3] - tex_sr[i]._v[0]) * delY_r;
        tex_dyr[i]._v[1] = (v0->_texcoords[i]._v[1] * v0->_win_coord._v[3] - tex_sr[i]._v[1]) * delY_r;
        tex_dyr[i]._v[2] = (v0->_texcoords[i]._v[2] * v0->_win_coord._v[3] - tex_sr[i]._v[2]) * delY_r;
        tex_dyr[i]._v[3] = (v0->_texcoords[i]._v[3] * v0->_win_coord._v[3] - tex_sr[i]._v[3]) * delY_r;
    }
    
    /*
    FIXED: 添加floor造成的属性偏差. Jett huang, 2010-11-10
    {{@
    */
    /* color */
    col_sl._v[0] += col_dyl._v[0] * errY;
    col_sl._v[1] += col_dyl._v[1] * errY;
    col_sl._v[2] += col_dyl._v[2] * errY;
    col_sl._v[3] += col_dyl._v[3] * errY;
    col_sr._v[0] += col_dyr._v[0] * errY;
    col_sr._v[1] += col_dyr._v[1] * errY;
    col_sr._v[2] += col_dyr._v[2] * errY;
    col_sr._v[3] += col_dyr._v[3] * errY;
    
    /* texture */
    for (i=0; i<MAX_TEXUNITS; ++i) {
        tex_sl[i]._v[0] += tex_dyl[i]._v[0] * errY;
        tex_sl[i]._v[1] += tex_dyl[i]._v[1] * errY;
        tex_sl[i]._v[2] += tex_dyl[i]._v[2] * errY;
        tex_sl[i]._v[3] += tex_dyl[i]._v[3] * errY;
        tex_sr[i]._v[0] += tex_dyr[i]._v[0] * errY;
        tex_sr[i]._v[1] += tex_dyr[i]._v[1] * errY;
        tex_sr[i]._v[2] += tex_dyr[i]._v[2] * errY;
        tex_sr[i]._v[3] += tex_dyr[i]._v[3] * errY;
    }
    /*
    }}@
    */

    for (; startY <= endY; startY += 1.f) 
    {
        zb_span_line(pContext, &vex_sl, &col_sl, tex_sl, &vex_sr, &col_sr, tex_sr);
        vex_sl._v[0] += vex_dyl._v[0];
        vex_sl._v[1] += vex_dyl._v[1];
        vex_sl._v[2] += vex_dyl._v[2];
        vex_sl._v[3] += vex_dyl._v[3];
        vex_sr._v[0] += vex_dyr._v[0];
        vex_sr._v[1] += vex_dyr._v[1];
        vex_sr._v[2] += vex_dyr._v[2];
        vex_sr._v[3] += vex_dyr._v[3];
        /* color */
        col_sl._v[0] += col_dyl._v[0]; col_sr._v[0] += col_dyr._v[0];
        col_sl._v[1] += col_dyl._v[1]; col_sr._v[1] += col_dyr._v[1];
        col_sl._v[2] += col_dyl._v[2]; col_sr._v[2] += col_dyr._v[2];
        col_sl._v[3] += col_dyl._v[3]; col_sr._v[3] += col_dyr._v[3];
        /* texture coords */
        for (i=0; i<MAX_TEXUNITS; ++i) {
            tex_sl[i]._v[0] += tex_dyl[i]._v[0]; tex_sr[i]._v[0] += tex_dyr[i]._v[0];
            tex_sl[i]._v[1] += tex_dyl[i]._v[1]; tex_sr[i]._v[1] += tex_dyr[i]._v[1];
            tex_sl[i]._v[2] += tex_dyl[i]._v[2]; tex_sr[i]._v[2] += tex_dyr[i]._v[2];
            tex_sl[i]._v[3] += tex_dyl[i]._v[3]; tex_sr[i]._v[3] += tex_dyr[i]._v[3];
        }
    } /* end for Y */
}

/* 绘制通用的三角形 */
/* \brief
    interlolate with t on viewport
*/
static void zb_interpolate(PtrGLVertex vt, PtrGLVertex v0, PtrGLVertex v1, GLfloat t)
{
    int i;
    GLfloat zt, factor0, factor1;

    vt->_win_coord._v[0] = v0->_win_coord._v[0] + (v1->_win_coord._v[0] - v0->_win_coord._v[0]) * t;
    vt->_win_coord._v[1] = v0->_win_coord._v[1] + (v1->_win_coord._v[1] - v0->_win_coord._v[1]) * t;
    vt->_win_coord._v[2] = v0->_win_coord._v[2] + (v1->_win_coord._v[2] - v0->_win_coord._v[2]) * t;
    vt->_win_coord._v[3] = v0->_win_coord._v[3] + (v1->_win_coord._v[3] - v0->_win_coord._v[3]) * t;
    zt = 1.f / vt->_win_coord._v[3];
    factor0 = v0->_win_coord._v[3] * (1 - t);
    factor1 = v1->_win_coord._v[3] * t;

    for (i=0; i<MAX_TEXUNITS; ++i) {
        vt->_texcoords[i]._v[0] = (v0->_texcoords[i]._v[0] * factor0 + v1->_texcoords[i]._v[0] * factor1) * zt;
        vt->_texcoords[i]._v[1] = (v0->_texcoords[i]._v[1] * factor0 + v1->_texcoords[i]._v[1] * factor1) * zt;
        vt->_texcoords[i]._v[2] = (v0->_texcoords[i]._v[2] * factor0 + v1->_texcoords[i]._v[2] * factor1) * zt;
        vt->_texcoords[i]._v[3] = (v0->_texcoords[i]._v[3] * factor0 + v1->_texcoords[i]._v[3] * factor1) * zt;
    }
    /* front color */
    vt->_front_color._v[0] = (v0->_front_color._v[0] * factor0 + v1->_front_color._v[0] * factor1) * zt;
    vt->_front_color._v[1] = (v0->_front_color._v[1] * factor0 + v1->_front_color._v[1] * factor1) * zt;
    vt->_front_color._v[2] = (v0->_front_color._v[2] * factor0 + v1->_front_color._v[2] * factor1) * zt;
    vt->_front_color._v[3] = (v0->_front_color._v[3] * factor0 + v1->_front_color._v[3] * factor1) * zt;
    /* back color */
    vt->_back_color._v[0] = (v0->_back_color._v[0] * factor0 + v1->_back_color._v[0] * factor1) * zt;
    vt->_back_color._v[1] = (v0->_back_color._v[1] * factor0 + v1->_back_color._v[1] * factor1) * zt;
    vt->_back_color._v[2] = (v0->_back_color._v[2] * factor0 + v1->_back_color._v[2] * factor1) * zt;
    vt->_back_color._v[3] = (v0->_back_color._v[3] * factor0 + v1->_back_color._v[3] * factor1) * zt;
}

static void zb_draw_triangle(PtrGLContext pContext, PtrGLVertex v0, PtrGLVertex v1, PtrGLVertex v2, GLboolean front)
{
    PtrGLVertex tmp;
    GLfloat t, x0, y0, x1, y1, x2, y2;

    x0 = v0->_win_coord._v[0]; y0 = v0->_win_coord._v[1];
    x1 = v1->_win_coord._v[0]; y1 = v1->_win_coord._v[1];
    x2 = v2->_win_coord._v[0]; y2 = v2->_win_coord._v[1];

    /* 检查三角形的合法性 */
    if ((FCMP(x0, x1) && FCMP(x1, x2)) || (FCMP(y0, y1) && FCMP(y1, y2)))
        return;

    /* 对三个顶点按y的从大到小排序,  v0 -- v1 -- v2 */
    if (y0 < y1) {
        SWAP(x0, x1, t);
        SWAP(y0, y1, t);
        SWAP(v0, v1, tmp);
    }
    if (y0 < y2) {
        SWAP(x0, x2, t);
        SWAP(y0, y2, t);
        SWAP(v0, v2, tmp);
    } 
    if (y1 < y2) {
        SWAP(x1, x2, t);
        SWAP(y1, y2, t);
        SWAP(v1, v2, tmp);
    }

    /* 判断是否为平底、平顶、普通 */
    if (FCMP(y0, y1)) {
        zb_draw_top_triangle(pContext, v0, v1, v2, front);
    } else if (FCMP(y1, y2)) {
        zb_draw_bottom_triangle(pContext, v0, v1, v2, front);
    } else {
        /* 拆成平底+平顶 
                    V0
                    /\
                   /  \
               vt /----V1
                 / 
                /
                V2
        */
        GLfloat t;
        GLVertex vt;

        t = (y1 - y0) / (y2 - y0);
        zb_interpolate(&vt, v0, v2, t);
        zb_draw_bottom_triangle(pContext, v0, v1, &vt, front);
        zb_draw_top_triangle(pContext, v1, &vt, v2, front);
    }
}

/* \brief
 *      receive a vertex from vertex-machine.
 *
 */
void gl_receive_vertex(PtrGLContext pContext, PtrGLVertex vertex)
{
    int *count = &(pContext->_vertex_count);

    switch (pContext->_primitive_type)
    {
    case GL_POINT:
        {
            gl_draw_point(pContext, vertex);
            (*count) = 0;
        }
        return;

    case GL_LINES:
        {
            if (*count == 0) {
                pContext->_vertex_buf[0] = *vertex;
                (*count)++;
                return;
            }
            if (*count == 1) {
                gl_draw_line(pContext, &(pContext->_vertex_buf[0]), vertex);
                (*count) = 0;
            }
        }
        return;

    case GL_LINE_STRIP:
         {
            if (*count == 0) {
                pContext->_vertex_buf[0] = *vertex;
                (*count)++;
                return;
            }
            if (*count == 1) {
                gl_draw_line(pContext, &(pContext->_vertex_buf[0]), vertex);
                pContext->_vertex_buf[0] = *vertex;
            }
        }
        return;

    case GL_TRIANGLES:
        {
            if (*count < 2) {
                pContext->_vertex_buf[(*count)++] = *vertex;
                return;
            }
            if (*count == 2) {
                gl_draw_triangle(pContext, &(pContext->_vertex_buf[0]), &(pContext->_vertex_buf[1]), vertex);
                (*count) = 0;
            }
        }
        return;

    case GL_TRIANGLE_STRIP:
        {
            if (*count < 2) {
                pContext->_vertex_buf[(*count)++] = *vertex;
                return;
            }
            if (*count == 2) {
                gl_draw_triangle(pContext, &(pContext->_vertex_buf[0]), &(pContext->_vertex_buf[1]), vertex);
                pContext->_vertex_buf[0] = pContext->_vertex_buf[1];
                pContext->_vertex_buf[1] = *vertex;
            }
        }
        return;

    case GL_TRIANGLE_FAN:
        {
            if (*count < 2) {
                pContext->_vertex_buf[(*count)++] = *vertex;
                return;
            }
            if (*count == 2) {
                gl_draw_triangle(pContext, &(pContext->_vertex_buf[0]), &(pContext->_vertex_buf[1]), vertex);
                pContext->_vertex_buf[1] = *vertex;
            }
        }
        return;
    
    case GL_QUADS:
        {
            if (*count < 3) {
                pContext->_vertex_buf[(*count)++] = *vertex;
                return;
            }
            if (*count == 3) {
                gl_draw_triangle(pContext, &(pContext->_vertex_buf[0]), &(pContext->_vertex_buf[1]), &(pContext->_vertex_buf[2]));
                gl_draw_triangle(pContext, &(pContext->_vertex_buf[0]), &(pContext->_vertex_buf[2]), vertex);
                (*count) = 0;
            }
        }
        return;

    case GL_QUAD_STRIP:
        {
            if (*count < 3) {
                pContext->_vertex_buf[(*count)++] = *vertex;
                return;
            }
            if (*count == 3) {
                gl_draw_triangle(pContext, &(pContext->_vertex_buf[0]), &(pContext->_vertex_buf[1]), vertex);
                gl_draw_triangle(pContext, &(pContext->_vertex_buf[0]), vertex, &(pContext->_vertex_buf[2]));
                pContext->_vertex_buf[0] = pContext->_vertex_buf[2];
                pContext->_vertex_buf[1] = *vertex;
                (*count) = 2;
            }
        }
        return;

    default:
        printf("error: gl_receive_vertex()\n");
        return;
    }
}