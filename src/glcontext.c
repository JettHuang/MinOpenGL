/* \brief
 *      procedure on GLContext
 *
 * \author  jett huang
 * \date    2010-9-11
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "glcontext.h"
#include "gltexture.h"


/* \brief
 *      initialize the GLContext
 * \ret
 *      return 0 if succeed, otherwise return none-zero.
 */
int glContext_init(PtrGLContext pContext)
{
    int i, j;

    if (!pContext) return 1;

    /* vertex attribute */
    pContext->_vertex_edge_flag = GL_TRUE;
    pContext->_vertex_color._v[0] = 1.0;
    pContext->_vertex_color._v[1] = 1.0;
    pContext->_vertex_color._v[2] = 1.0;
    pContext->_vertex_color._v[3] = 1.0;
    pContext->_vertex_normal._v[0] = 0.0;
    pContext->_vertex_normal._v[1] = 0.0;
    pContext->_vertex_normal._v[2] = 1.0;
    for (i=0; i<MAX_TEXUNITS; ++i) {
        pContext->_vertex_texcoord[i]._v[0] = 0.0;
        pContext->_vertex_texcoord[i]._v[1] = 0.0;
        pContext->_vertex_texcoord[i]._v[2] = 0.0;
        pContext->_vertex_texcoord[i]._v[3] = 1.0;
    }

    /* lighting model && lighting */
    /* pContext->_light_model_color_ctrl = GL_SINGLE_COLOR; */
    pContext->_light_model_local_viewer = 0;
    pContext->_light_model_two_side = 0;
    pContext->_lighting_on = GL_FALSE;
    for (i=0; i<MAX_LIGHTS; ++i) {
        pContext->_lights_on[i] = GL_FALSE;
        glLightSource_init(&(pContext->_lights_source[i]));
    }
    /* set the light0 */
    pContext->_lights_source[0]._diffuse._v[0] = 1.0;
    pContext->_lights_source[0]._diffuse._v[1] = 1.0;
    pContext->_lights_source[0]._diffuse._v[2] = 1.0;
    pContext->_lights_source[0]._diffuse._v[3] = 1.0;
    pContext->_lights_source[0]._specular._v[0] = 1.0;
    pContext->_lights_source[0]._specular._v[1] = 1.0;
    pContext->_lights_source[0]._specular._v[2] = 1.0;
    pContext->_lights_source[0]._specular._v[3] = 1.0;

    glLightModel_init(&(pContext->_light_model));

    /* meterial && mode */
    pContext->_color_material_on = GL_FALSE;
    pContext->_color_material_face = GL_FRONT_AND_BACK;
    pContext->_color_material_mode = GL_AMBIENT_AND_DIFFUSE;
    glMaterialParameters_init(&(pContext->_front_material));
    glMaterialParameters_init(&(pContext->_back_material));

    /* vertex transform */
    pContext->_normalize_on = GL_FALSE;
    for (i=0; i<MAX_STACK_SIZE; ++i) {
        make_identity_matrix(&(pContext->_matrix_stacks[0][i]));
        make_identity_matrix(&(pContext->_matrix_stacks[1][i]));
        make_identity_matrix(&(pContext->_matrix_stacks[2][i]));
        for (j=0; j<MAX_TEXUNITS; ++j) {
            make_identity_matrix(&(pContext->_tex_matrix_stacks[0][i]));
        }
    }
    pContext->_matrix_top[0] = 0;     /* the top of modelView matrix */
    pContext->_matrix_top[1] = 0;     /* the top of Projection matrix */
    pContext->_matrix_top[2] = 0;     /* the top of Color matrix */
    pContext->_current_matrix = GL_MODELVIEW;    /* current active matrix, 0~3 */

    /* set vertex assamble state */
    pContext->_primitive_type = 0;
    pContext->_cull_mode = GL_BACK;
    pContext->_fontface_mode = GL_CCW;
    pContext->_cull_face_on = GL_FALSE;

    /* view-port */
    pContext->_viewport._x = 0;
    pContext->_viewport._y = 0;
    pContext->_viewport._width = 0;
    pContext->_viewport._height= 0;
    pContext->_viewport._zNear = 0.f;
    pContext->_viewport._zFar  = 1.f;
    /* internal used */
    pContext->_viewport._xScale = 0.f; pContext->_viewport._xCenter = 0.f;
    pContext->_viewport._yScale = 0.f; pContext->_viewport._yCenter = 0.f;
    pContext->_viewport._zScale = DEPTH * 0.5f, pContext->_viewport._zCenter = DEPTH * 0.5f;

    /* depth function */
    pContext->_depth_test_on = GL_FALSE;
    pContext->_depth_func = GL_LESS;

    /* frame buffers */
    pContext->_clear_r = 0;
    pContext->_clear_g = 0;
    pContext->_clear_b = 0;
    pContext->_clear_a = 255;
    pContext->_clear_depth= DEPTH;
    pContext->_buf_width = 0;
    pContext->_buf_height= 0;
    pContext->_buf_color_align = 0;
    pContext->_buf_color = NULL;
    pContext->_buf_depth = NULL;

    tex_initialize();
    return 0;
}

/* \brief
 *      unInitialize the GLContext
 */
void glContext_uninit(PtrGLContext pContext)
{
    /* free buffers */
    /* if (pContext->_buf_color) free(pContext->_buf_color); */
    if (pContext->_buf_depth) free(pContext->_buf_depth);
}

/* \brief
 *      initialize the GLLightSource
 * \ret
 *      return 0 if succeed, otherwise return none-zero.
 */
int glLightSource_init(PtrGLLightSource pLightSrc)
{
    if (!pLightSrc) return 1;

    pLightSrc->_ambient._v[0] = 0.0;
    pLightSrc->_ambient._v[1] = 0.0;
    pLightSrc->_ambient._v[2] = 0.0;
    pLightSrc->_ambient._v[3] = 1.0;
    pLightSrc->_diffuse._v[0] = 0.0;
    pLightSrc->_diffuse._v[1] = 0.0;
    pLightSrc->_diffuse._v[2] = 0.0;
    pLightSrc->_diffuse._v[3] = 1.0;
    pLightSrc->_specular._v[0] = 0.0;
    pLightSrc->_specular._v[1] = 0.0;
    pLightSrc->_specular._v[2] = 0.0;
    pLightSrc->_specular._v[3] = 1.0;

    pLightSrc->_position._v[0] = 0.0;
    pLightSrc->_position._v[1] = 0.0;
    pLightSrc->_position._v[2] = 1.0;
    pLightSrc->_position._v[3] = 0.0;
    pLightSrc->_spotDirection._v[0] = 0.0;
    pLightSrc->_spotDirection._v[1] = 0.0;
    pLightSrc->_spotDirection._v[2] = -1.0;

    pLightSrc->_spotExponent = 0.0;
    pLightSrc->_spotCutoff   = 180.0;
    pLightSrc->_constantAttenuation = 1.0;
    pLightSrc->_linearAttenuation   = 0.0;
    pLightSrc->_quadraticAttenuation= 0.0;

    return 0;
}

/* \brief
 *      uninitialze the GLLightSource
 */
void glLightSource_uninit(PtrGLLightSource pLightSrc)
{
    /* do nothing */
}

/* \brief
 *      initialize the GLLightModel
 * \ret
 *      return 0 if succeed, otherwise return none-zero.
 */
int glLightModel_init(PtrGLLightModel pLightModel)
{
    if (!pLightModel) return 1;

    pLightModel->_ambient._v[0] = 0.2f;
    pLightModel->_ambient._v[1] = 0.2f;
    pLightModel->_ambient._v[2] = 0.2f;
    pLightModel->_ambient._v[3] = 1.0f;
	return 0;
}

/* \brief
 *      uninitialize the GLLightModel
 */
void glLightModel_uninit(PtrGLLightModel pLightModel)
{
    /* do nothing */
}

/* \brief
 *      initialize the MaterialParameters
 * \ret
 *      return 0 if succeed, otherwise return none-zero.
 */
int glMaterialParameters_init(PtrMaterialParameters pMaterial)
{
    if (!pMaterial) return 1;

    pMaterial->_emission._v[0] = 0.0f;
    pMaterial->_emission._v[1] = 0.0f;
    pMaterial->_emission._v[2] = 0.0f;
    pMaterial->_emission._v[3] = 1.0f;
    pMaterial->_ambient._v[0] = 0.2f;
    pMaterial->_ambient._v[1] = 0.2f;
    pMaterial->_ambient._v[2] = 0.2f;
    pMaterial->_ambient._v[3] = 1.0f;
    pMaterial->_diffuse._v[0] = 0.8f;
    pMaterial->_diffuse._v[1] = 0.8f;
    pMaterial->_diffuse._v[2] = 0.8f;
    pMaterial->_diffuse._v[3] = 1.0f;
    pMaterial->_specular._v[0] = 0.0f;
    pMaterial->_specular._v[1] = 0.0f;
    pMaterial->_specular._v[2] = 0.0f;
    pMaterial->_specular._v[3] = 1.0f;
    pMaterial->_shininess = 0.0f;
	return 0;
}

/* \brief
 *      uninitialize the MaterialParameters
 */
void glMaterialParameters_uninit(PtrMaterialParameters pMaterial)
{
    /* do nothing */
}