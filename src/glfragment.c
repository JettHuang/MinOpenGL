/* \brief
 *      glfragment.c
 *
 */

#include <stdio.h>
#include "../include/GL/util.h"
#include "glfragment.h"
#include "gltexture.h"


/* \brief
 *      process fragment
 *
 */
void gl_process_fragment(PtrGLContext pContext, PtrGLFragment frag)
{
    GLbyte *buf;
    GLushort *dep;
    GLuint width;
    GLfloat *rgba;

    if (!pContext->_buf_color) return;

#if 0
    if (frag->_winX < 0 || frag->_winX >= pContext->_buf_width ||
        frag->_winY < 0 || frag->_winY >= pContext->_buf_height)
    {
        printf("winx=%i, winy=%i, winz=%i\n", frag->_winX, frag->_winY, frag->_winZ);
        return;
    }
#endif
    width = pContext->_buf_width;
    if (pContext->_depth_test_on) {
        if (pContext->_buf_depth) {
            dep = (GLushort *)pContext->_buf_depth;
            dep = dep + (width * frag->_winY + frag->_winX);
            if (*dep <= frag->_winZ)
                return;
            *dep = frag->_winZ;
        }
        else {
            printf("WARNING: depth buffer is NULL\n");
        }
    }

    /* Sample texel */
#ifdef QUERY_PERFORMANCE
    begin_performance();
#endif
    tex_shade_fragment(frag);
#ifdef QUERY_PERFORMANCE
    end_performance("tex_shade_fragment");
#endif

    rgba  = frag->_color._v;
    width = pContext->_buf_color_align;
    buf = (pContext->_buf_color) + (width * frag->_winY + frag->_winX * 3);
    *buf++ = (GLbyte)(CLAMP(rgba[2], 0.f, 1.f) * 255);
    *buf++ = (GLbyte)(CLAMP(rgba[1], 0.f, 1.f) * 255);
    *buf   = (GLbyte)(CLAMP(rgba[0], 0.f, 1.f) * 255);
}