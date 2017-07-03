/* \brief
 *      implementation of VertexMachine
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include "../include/GL/util.h"
#include "glvertexmachine.h"
#include "glrasterlize.h"
#include "gltexture.h"


/* set flag GL_TRUE or GL_FALSE */
static void glopSetVarious(PtrGLContext pContext, GLenum cap, GLboolean on)
{
    switch (cap)
    {
/*
    case GL_ALPHA_TEST:
    case GL_AUTO_NORMAL:
        
    case GL_BLEND:
    case GL_CLIP_PLANE0:
    case GL_CLIP_PLANE1:
    case GL_CLIP_PLANE2:
    case GL_CLIP_PLANE3:
    case GL_CLIP_PLANE4:
    case GL_CLIP_PLANE5:

    case GL_COLOR_LOGIC_OP:
    case GL_COLOR_MATERIAL:
*/

    case GL_CULL_FACE:  pContext->_cull_face_on = on; break;
    case GL_DEPTH_TEST: pContext->_depth_test_on = on; break;

/*
    case GL_DITHER:
    case GL_FOG:
    case GL_INDEX_LOGIC_OP:
*/

    case GL_LIGHT0: pContext->_lights_on[0] = on; break;
    case GL_LIGHT1: pContext->_lights_on[1] = on; break;
    case GL_LIGHT2: pContext->_lights_on[2] = on; break;
    case GL_LIGHT3: pContext->_lights_on[3] = on; break;
    case GL_LIGHT4: pContext->_lights_on[4] = on; break;
    case GL_LIGHT5: pContext->_lights_on[5] = on; break;
    case GL_LIGHT6: pContext->_lights_on[6] = on; break;
    case GL_LIGHT7: pContext->_lights_on[7] = on; break;
    case GL_LIGHTING: pContext->_lighting_on= on; break;
/*
    case GL_LINE_SMOOTH:
    case GL_LINE_STIPPLE:
    case GL_MAP1_COLOR_4:
    case GL_MAP1_INDEX:
    case GL_MAP1_NORMAL:
    case GL_MAP1_TEXTURE_COORD_1:
    case GL_MAP1_TEXTURE_COORD_2:
    case GL_MAP1_TEXTURE_COORD_3:
    case GL_MAP1_TEXTURE_COORD_4:
    case GL_MAP1_VERTEX_3:
    case GL_MAP1_VERTEX_4:
    case GL_MAP2_COLOR_4:
    case GL_MAP2_INDEX:
    case GL_MAP2_NORMAL:
    case GL_MAP2_TEXTURE_COORD_1:
    case GL_MAP2_TEXTURE_COORD_2:
    case GL_MAP2_TEXTURE_COORD_3:
    case GL_MAP2_TEXTURE_COORD_4:
    case GL_MAP2_VERTEX_3:
    case GL_MAP2_VERTEX_4: break;
*/
    case GL_NORMALIZE: pContext->_normalize_on = on; break;

/*
    case GL_POINT_SMOOTH:
    case GL_POLYGON_OFFSET_FILL:
    case GL_POLYGON_OFFSET_LINE:
    case GL_POLYGON_OFFSET_POINT:
    case GL_POLYGON_SMOOTH:
    case GL_POLYGON_STIPPLE:

    case GL_SCISSOR_TEST:
    case GL_STENCIL_TEST:
    case GL_TEXTURE_1D:
*/
    case GL_TEXTURE_2D:
        tex_TexEnable(GL_TEXTURE_2D, on); break;
/*
    case GL_TEXTURE_GEN_Q:
    case GL_TEXTURE_GEN_R:
    case GL_TEXTURE_GEN_S:
    case GL_TEXTURE_GEN_T:
*/
	default:
        printf("Error: not implemented\n");
		break;
    }
}

/*
 * _params[0] enum variable
 */
void glopEnable(PtrGLContext pContext, OpParam _params[])
{
    glopSetVarious(pContext, _params[0].e, GL_TRUE);
}

/*
 * _params[0] enum variable
 */
void glopDisable(PtrGLContext pContext, OpParam _params[])
{
    glopSetVarious(pContext, _params[0].e, GL_FALSE);
}

/*
 * _params[0] GL_TRUE or GL_FALSE
 */
void glopEdgeFlag(PtrGLContext pContext, OpParam _params[])
{
    pContext->_vertex_edge_flag = _params[0].b;
}

/*
 * _params[0]  red   [0.0, 1.0] float
 * _params[1]  green [0.0, 1.0] float
 * _params[2]  blue  [0.0, 1.0] float
 * _params[3]  alpha [0.0, 1.0] float
 */
void glopColor(PtrGLContext pContext, OpParam _params[])
{
    pContext->_vertex_color._v[0] = _params[0].f;
    pContext->_vertex_color._v[1] = _params[1].f;
    pContext->_vertex_color._v[2] = _params[2].f;
    pContext->_vertex_color._v[3] = _params[3].f;
}

/* 
 * _params[0] x float
 * _params[1] y float
 * _params[2] z float
 */
void glopNormal(PtrGLContext pContext, OpParam _params[])
{
    pContext->_vertex_normal._v[0] = _params[0].f;
    pContext->_vertex_normal._v[1] = _params[1].f;
    pContext->_vertex_normal._v[2] = _params[2].f;
    normalize_vector3(&(pContext->_vertex_normal));
}

/* shade the vertex's color */
static void gl_shade_vertex(PtrGLContext pContext, PtrGLVertex pVertex)
{
    int i, face;
    float R, G, B, A;
    float attenuation;  /* 衰减系数 */
    float ldotn, vdotd, hdotn;
    Vector3 L;          /* 向量: 顶点到光源 */
    Vector3 V;          /* 向量: 光源到顶点 */
    Vector3 H;          /* 向量: Half-Vecotr */
    Vector3 negN;       /* 向量: 负的法向量 */
    PtrMaterialParameters materials[2];
    PtrVector3 normals[2];
    PtrVector4 colors[2];

    if (!pContext->_lighting_on) {
        copy_vector4(&(pVertex->_front_color), &(pContext->_vertex_color));
        copy_vector4(&(pVertex->_back_color), &(pContext->_vertex_color));
        return;
    }

    /* 设置正反面信息 */
    materials[0] = &(pContext->_front_material);
    materials[1] = &(pContext->_back_material);
    colors[0] = &(pVertex->_front_color);
    colors[1] = &(pVertex->_back_color);
    normals[0] = &(pVertex->_normal);
    normals[1] = &(pVertex->_normal);
    if (pContext->_light_model_two_side) {
        negN._v[0] = -pVertex->_normal._v[0];
        negN._v[1] = -pVertex->_normal._v[1];
        negN._v[2] = -pVertex->_normal._v[2];
        normals[1] = &negN;
    }

    /* 分别计算正反面光照 */
    for (face=0; face<2; ++face)
    {
        /* emission */
        R = materials[face]->_emission._v[0];
        G = materials[face]->_emission._v[1];
        B = materials[face]->_emission._v[2];
        A = CLAMP(materials[face]->_diffuse._v[3], 0.f, 1.f);

        /* global ambient */
        R += materials[face]->_ambient._v[0] * pContext->_light_model._ambient._v[0];
        G += materials[face]->_ambient._v[1] * pContext->_light_model._ambient._v[1];
        B += materials[face]->_ambient._v[2] * pContext->_light_model._ambient._v[2];

        /* calculate the all lights effect */
        for (i=0; i<MAX_LIGHTS; ++i)
        {
            PtrGLLightSource light = NULL;
            float lR, lG, lB, dfactor, sfactor;

            if (!pContext->_lights_on[i]) continue;

            light = &(pContext->_lights_source[i]);
            /* calculate the attenuation */
            if (light->_position._v[3] == 0.f) {
                L._v[0] = light->_position._v[0];
                L._v[1] = light->_position._v[1];
                L._v[2] = light->_position._v[2];
                attenuation = 1.f;
            } else {
                float dist;
                L._v[0] = light->_position._v[0] - pVertex->_eye_coord._v[0];
                L._v[1] = light->_position._v[1] - pVertex->_eye_coord._v[1];
                L._v[2] = light->_position._v[2] - pVertex->_eye_coord._v[2];
                dist = (float)sqrt(L._v[0] * L._v[0] + L._v[1] * L._v[1] + L._v[2] * L._v[2]);
                attenuation = 1.f / (light->_constantAttenuation + dist * light->_linearAttenuation
                    + dist * light->_quadraticAttenuation);
            }

            /* normalize */
            normalize_vector3(&L);
            V._v[0] = -L._v[0]; V._v[1] = -L._v[1]; V._v[2] = -L._v[2];
            vdotd = dot_vector3(&V, &(light->_spotDirection));
            if (light->_spotCosCutoff > vdotd) {
                continue;
            } else if (light->_spotCutoff != 180.f) {
                attenuation *= (float)pow(MAX(vdotd, 0.f), light->_spotExponent);
            }

            /* calculate the ambient */
            lR = light->_ambient._v[0] * materials[face]->_ambient._v[0];
            lG = light->_ambient._v[1] * materials[face]->_ambient._v[1];
            lB = light->_ambient._v[2] * materials[face]->_ambient._v[2];

            /* calculate the diffuse */
            ldotn = dot_vector3(&L, normals[face]);
            dfactor = MAX(ldotn, 0.f);
            lR += dfactor * light->_diffuse._v[0] * materials[face]->_diffuse._v[0];
            lG += dfactor * light->_diffuse._v[1] * materials[face]->_diffuse._v[1];
            lB += dfactor * light->_diffuse._v[2] * materials[face]->_diffuse._v[2];

            /* calculate the specular */
            if (pContext->_light_model_local_viewer) {
                H._v[0] = - pVertex->_eye_coord._v[0] + L._v[0];
                H._v[1] = - pVertex->_eye_coord._v[1] + L._v[1];
                H._v[2] = - pVertex->_eye_coord._v[2] + L._v[2];
            } else {
                H._v[0] =  L._v[0];
                H._v[1] =  L._v[1];
                H._v[2] =  1.f + L._v[2];
            }
            normalize_vector3(&H);
            hdotn = dot_vector3(&H, normals[face]);
            sfactor = (float)pow(MAX(hdotn, 0.f), materials[face]->_shininess);
            lR += sfactor * light->_specular._v[0] * materials[face]->_specular._v[0];
            lG += sfactor * light->_specular._v[1] * materials[face]->_specular._v[1];
            lB += sfactor * light->_specular._v[2] * materials[face]->_specular._v[2];

            R += attenuation * lR;
            G += attenuation * lG;
            B += attenuation * lB;
        } /* end for i */

        colors[face]->_v[0] = CLAMP(R, 0.f, 1.f);
        colors[face]->_v[1] = CLAMP(G, 0.f, 1.f);
        colors[face]->_v[2] = CLAMP(B, 0.f, 1.f);
        colors[face]->_v[3] = CLAMP(A, 0.f, 1.f);
    }
}

/*
 * _params[0] x float
 * _params[1] y float
 * _params[2] z float
 * _params[3] w float
 */
static void  set_clip_code(PtrGLVertex v)
{
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
    GLfloat w = v->_pro_coord._v[3];
    
    v->_clip_code = 0;
    v->_clip_code |= v->_pro_coord._v[0] < -w ? VERTEX_CLIP_LEFT  : 0;
    v->_clip_code |= v->_pro_coord._v[0] >  w ? VERTEX_CLIP_RIGHT : 0;
    v->_clip_code |= v->_pro_coord._v[1] < -w ? VERTEX_CLIP_BOTTOM  : 0;
    v->_clip_code |= v->_pro_coord._v[1] >  w ? VERTEX_CLIP_TOP : 0;
    v->_clip_code |= v->_pro_coord._v[2] >  w ? VERTEX_CLIP_BACK : 0;
    v->_clip_code |= v->_pro_coord._v[2] < -w ? VERTEX_CLIP_FRONT : 0;
}

void glopVertex(PtrGLContext pContext, OpParam _params[])
{
    /* a. vertex transform
     * b. shader color
     */
    PtrMatrix4 modelview, projection;
    Matrix4 inverse, inverse_trans;
    GLVertex vertex;
    Vector4 pos;    /* position of vertex */

#ifdef QUERY_PERFORMANCE
    begin_performance();
#endif
    /* set the flag */
    vertex._edge_flag = pContext->_vertex_edge_flag;
    memcpy(vertex._texcoords, pContext->_vertex_texcoord, sizeof(vertex._texcoords));

    /* calculate the eye-positon && volume-position */
    pos._v[0] = _params[0].f; pos._v[1] = _params[1].f; pos._v[2] = _params[2].f; pos._v[3] = _params[3].f;
    modelview = &(pContext->_matrix_stacks[0][pContext->_matrix_top[0]]);
    projection= &(pContext->_matrix_stacks[1][pContext->_matrix_top[1]]);
    invert_matrix(modelview, &inverse);
    transpose_matrix(&inverse, &inverse_trans);
    multiply_matrix_by_vector3(&inverse_trans, &pContext->_vertex_normal, &vertex._normal);
    multiply_matrix_by_vector4(modelview, &pos, &vertex._eye_coord);
    multiply_matrix_by_vector4(projection,&vertex._eye_coord, &vertex._pro_coord);
	/* set the clip flag */
    set_clip_code(&vertex);

    /* color shader */
    if (pContext->_normalize_on) {
        normalize_vector3(&vertex._normal);
    }
    gl_shade_vertex(pContext, &vertex);

#ifdef QUERY_PERFORMANCE
    end_performance("operate vertex ");
#endif

    /* send it to rasterlization machine */
    gl_receive_vertex(pContext, &vertex);
}

/* _params[0] texture unit id
 * _params[1] s float
 * _params[2] t float
 * _params[3] r float
 * _params[4] q float
 */
void glopTexCoord(PtrGLContext pContext, OpParam _params[])
{
    int tex_unit = _params[0].i;
    Vector4 coord;
    PtrMatrix4 mat;

    if (tex_unit >= GL_TEXTURE0 &&
        tex_unit <= GL_TEXTURE7)
    {
        tex_unit -= GL_TEXTURE0;
        coord._v[0] = _params[1].f;
        coord._v[1] = _params[2].f;
        coord._v[2] = _params[3].f;
        coord._v[3] = _params[4].f;
        mat = &(pContext->_tex_matrix_stacks[tex_unit][pContext->_tex_matrix_top[tex_unit]]);
        multiply_matrix_by_vector4(mat, &coord, &coord);
        pContext->_vertex_texcoord[tex_unit] = coord;
    }
}

/*
 * _params[0] paramName enum
 * _params[1,2,...] ...
 *
 */
void glopLightModel(PtrGLContext pContext, OpParam _params[])
{
    switch (_params[0].e)
    {
    case GL_LIGHT_MODEL_LOCAL_VIEWER:
        pContext->_light_model_local_viewer = _params[1].b; break;
/*    case GL_LIGHT_MODEL_COLOR_CONTROL:
        pContext->_light_model_color_ctrl = _params[1].e; break;
*/
    case GL_LIGHT_MODEL_TWO_SIDE:
        pContext->_light_model_two_side = _params[1].b; break;
    case GL_LIGHT_MODEL_AMBIENT:
        {
        pContext->_light_model._ambient._v[0] = _params[1].f;
        pContext->_light_model._ambient._v[1] = _params[2].f;
        pContext->_light_model._ambient._v[2] = _params[3].f;
        pContext->_light_model._ambient._v[3] = _params[4].f;
        }
        break;
    default:
        printf("invalid pname in glLightModel\n"); break;
    }
}

/*
 * _params[0] light[i]
 * _params[1] pname
 * _params[2,...] param
 */
void glopLight(PtrGLContext pContext, OpParam _params[])
{
    PtrGLLightSource ptrLight = NULL;

    switch (_params[0].e)
    {
    case GL_LIGHT0:
        ptrLight = &(pContext->_lights_source[0]); break;
    case GL_LIGHT1:
        ptrLight = &(pContext->_lights_source[1]); break;
    case GL_LIGHT2:
        ptrLight = &(pContext->_lights_source[2]); break;
    case GL_LIGHT3:
        ptrLight = &(pContext->_lights_source[3]); break;
    case GL_LIGHT4:
        ptrLight = &(pContext->_lights_source[4]); break;
    case GL_LIGHT5:
        ptrLight = &(pContext->_lights_source[5]); break;
    case GL_LIGHT6:
        ptrLight = &(pContext->_lights_source[6]); break;
    case GL_LIGHT7:
        ptrLight = &(pContext->_lights_source[7]); break;
    default:
        printf("ERROR: invalid Light id\n");
        break;

    }

    /* check the pointer */
    if (!ptrLight) return;
    switch (_params[1].e)
    {
    case GL_AMBIENT:
        {
            ptrLight->_ambient._v[0] = _params[2].f;
            ptrLight->_ambient._v[1] = _params[3].f;
            ptrLight->_ambient._v[2] = _params[4].f;
            ptrLight->_ambient._v[3] = _params[5].f;
        }
        break;
    case GL_DIFFUSE:
        {
            ptrLight->_diffuse._v[0] = _params[2].f;
            ptrLight->_diffuse._v[1] = _params[3].f;
            ptrLight->_diffuse._v[2] = _params[4].f;
            ptrLight->_diffuse._v[3] = _params[5].f;
        }
        break;
    case GL_SPECULAR:
        {
            ptrLight->_specular._v[0] = _params[2].f;
            ptrLight->_specular._v[1] = _params[3].f;
            ptrLight->_specular._v[2] = _params[4].f;
            ptrLight->_specular._v[3] = _params[5].f;
        }
        break;
    case GL_POSITION:
        {
            PtrMatrix4 modelview;
            ptrLight->_position._v[0] = _params[2].f;
            ptrLight->_position._v[1] = _params[3].f;
            ptrLight->_position._v[2] = _params[4].f;
            ptrLight->_position._v[3] = _params[5].f;
            modelview = &(pContext->_matrix_stacks[0][pContext->_matrix_top[0]]);
            multiply_matrix_by_vector4(modelview, &ptrLight->_position, &ptrLight->_position);
        }   
        break;
    case GL_SPOT_DIRECTION:
        {
            PtrMatrix4 modelview;
            ptrLight->_spotDirection._v[0] = _params[2].f;
            ptrLight->_spotDirection._v[1] = _params[3].f;
            ptrLight->_spotDirection._v[2] = _params[4].f;
            normalize_vector3(&(ptrLight->_spotDirection));
            modelview = &(pContext->_matrix_stacks[0][pContext->_matrix_top[0]]);
            multiply_matrix_by_vector3(modelview, &ptrLight->_spotDirection, &ptrLight->_spotDirection);
        }
        break;
    case GL_SPOT_EXPONENT:
        ptrLight->_spotExponent = _params[2].f; break;
    case GL_SPOT_CUTOFF:
        ptrLight->_spotCutoff = _params[2].f; break;
    case GL_CONSTANT_ATTENUATION:
        ptrLight->_constantAttenuation = _params[2].f; break;
    case GL_LINEAR_ATTENUATION:
        ptrLight->_linearAttenuation =  _params[2].f; break;
    case GL_QUADRATIC_ATTENUATION:
        ptrLight->_quadraticAttenuation = _params[2].f; break;
    default:
        printf("ERROR: invalid pname in glLight\n");
    }
}

/*
 * _params[0] face
 * _params[1] mode
 */
void glopColorMaterial(PtrGLContext pContext, OpParam _params[])
{
    pContext->_color_material_face = _params[0].e;
    pContext->_color_material_mode = _params[1].e;
}

/*
 * _params[0] face
 * _params[1] pname
 * _params[2,..] param
 */
void glopMaterial(PtrGLContext pContext, OpParam _params[])
{
    PtrMaterialParameters faces[2] = {NULL, NULL};
    int i;

    switch (_params[0].e)
    {
    case GL_FRONT:
        faces[0] = &(pContext->_front_material); break;
    case GL_BACK:
        faces[1] = &(pContext->_back_material); break;
    case GL_FRONT_AND_BACK:
        faces[0] = &(pContext->_front_material);
        faces[1] = &(pContext->_back_material); break;
    default:
        printf("ERROR: invalid face in glMaterial\n");
        break;
    }

    for (i=0; i<2; ++i) {
        if (!faces[i]) continue;
        switch (_params[1].e)
        {
        case GL_AMBIENT:
            {
                faces[i]->_ambient._v[0] = _params[2].f;
                faces[i]->_ambient._v[1] = _params[3].f;
                faces[i]->_ambient._v[2] = _params[4].f;
                faces[i]->_ambient._v[3] = _params[5].f;
            }
            break;
        case GL_DIFFUSE:
            {
                faces[i]->_diffuse._v[0] = _params[2].f;
                faces[i]->_diffuse._v[1] = _params[3].f;
                faces[i]->_diffuse._v[2] = _params[4].f;
                faces[i]->_diffuse._v[3] = _params[5].f;
            }
            break;
        case GL_SPECULAR:
            {
                faces[i]->_specular._v[0] = _params[2].f;
                faces[i]->_specular._v[1] = _params[3].f;
                faces[i]->_specular._v[2] = _params[4].f;
                faces[i]->_specular._v[3] = _params[5].f;
            }
            break;
        case GL_EMISSION:
            {
                faces[i]->_emission._v[0] = _params[2].f;
                faces[i]->_emission._v[1] = _params[3].f;
                faces[i]->_emission._v[2] = _params[4].f;
                faces[i]->_emission._v[3] = _params[5].f;
            }
            break;
        case GL_SHININESS:
            faces[i]->_shininess = _params[2].f; break;
        case GL_AMBIENT_AND_DIFFUSE:
            {
                faces[i]->_ambient._v[0] = _params[2].f;
                faces[i]->_ambient._v[1] = _params[3].f;
                faces[i]->_ambient._v[2] = _params[4].f;
                faces[i]->_ambient._v[3] = _params[5].f;
                faces[i]->_diffuse._v[0] = _params[2].f;
                faces[i]->_diffuse._v[1] = _params[3].f;
                faces[i]->_diffuse._v[2] = _params[4].f;
                faces[i]->_diffuse._v[3] = _params[5].f;
            }
            break;
        default:
            printf("invalid pname in glMaterial\n"); break;
        }
    } /* end for i */
}

/*
 * _params[0] mode
 */
void glopMatrixMode(PtrGLContext pContext, OpParam _params[])
{
    pContext->_current_matrix = _params[0].e;
}

/* get current active stack 
   1 false, 0 true	
*/
static int get_current_stack_matrix(PtrGLContext pContext, PtrMatrix4 *ptrStack, int **top)
{
    int idx;

    *ptrStack = NULL; *top = NULL;
    switch (pContext->_current_matrix)
    {
    case GL_MODELVIEW:
        *ptrStack = &(pContext->_matrix_stacks[0][0]); 
        *top = &(pContext->_matrix_top[0]);
        break;
    case GL_PROJECTION:
        *ptrStack = &(pContext->_matrix_stacks[1][0]); 
        *top = &(pContext->_matrix_top[1]);
        break;
    case GL_COLOR:
        *ptrStack = &(pContext->_matrix_stacks[2][0]);
        *top = &(pContext->_matrix_top[2]);
        break;
    case GL_TEXTURE:
        {
            idx = (pContext->_active_tex_unit - GL_TEXTURE0);
            if (pContext->_active_tex_unit >= GL_TEXTURE0 &&
                pContext->_active_tex_unit <= GL_TEXTURE7)
            {
                *ptrStack = &(pContext->_tex_matrix_stacks[idx][0]);
                *top = &(pContext->_tex_matrix_top[idx]);
            }
        }
        break;
    default:
        break;
    }

    if (!ptrStack) {
        printf("ERROR: invalid stack in glPushMatrix\n");
        return 1;
    }
    return 0;
}

/*
 * none
 */
void glopPushMatrix(PtrGLContext pContext, OpParam _params[])
{
    PtrMatrix4 ptrStack = NULL;
    int *top = NULL;
    
    if (!get_current_stack_matrix(pContext, &ptrStack, &top) &&
        *top < (MAX_STACK_SIZE-1))
    {
        (*top)++;
        copy_matrix4(ptrStack + *top, ptrStack + *top - 1);
    }
}

/*
 * none
 */
void glopPopMatrix(PtrGLContext pContext, OpParam _params[])
{
    PtrMatrix4 ptrStack = NULL;
    int *top = NULL;

    if (!get_current_stack_matrix(pContext, &ptrStack, &top) &&
        *top > 0)
    {
        (*top)--;
    }
}

/*
 * _params[0]       degree float
 * _params[1,2,3]   x,y,z axies
 *
 */
void glopRotate(PtrGLContext pContext, OpParam _params[])
{
    Matrix4 mat;
    Vector3 vec;
    PtrMatrix4 ptrStack = NULL;
    int *top = NULL;

    if (!get_current_stack_matrix(pContext, &ptrStack, &top))
    {
        vec._v[0] = _params[1].f; vec._v[1] = _params[2].f; vec._v[2] = _params[3].f;
		make_rotate_matrix(&mat, _params[0].f, &vec);
        multiply_matrix_by_matrix(&ptrStack[*top], &mat, &ptrStack[*top]);
    }
}

/* 
 * _params[0]   x
 * _params[1]   y
 * _params[2]   z
 */
void glopTransform(PtrGLContext pContext, OpParam _params[])
{
    Matrix4 mat;
    Vector3 vec;
    PtrMatrix4 ptrStack = NULL;
    int *top = NULL;

    if (!get_current_stack_matrix(pContext, &ptrStack, &top))
    {
        vec._v[0] = _params[0].f; vec._v[1] = _params[1].f; vec._v[2] = _params[2].f;
        make_transform_matrix(&mat, &vec);
        multiply_matrix_by_matrix(&ptrStack[*top], &mat, &ptrStack[*top]);
    }
}

/*
 * _params[0]   x
 * _params[1]   y
 * _params[2]   z
 */
void glopScale(PtrGLContext pContext, OpParam _params[])
{
    Matrix4 mat;
    Vector3 vec;
    PtrMatrix4 ptrStack = NULL;
    int *top = NULL;

    if (!get_current_stack_matrix(pContext, &ptrStack, &top))
    {
        vec._v[0] = _params[0].f; vec._v[1] = _params[1].f; vec._v[2] = _params[2].f;
        make_scale_matrix(&mat, &vec);
        multiply_matrix_by_matrix(&ptrStack[*top], &mat, &ptrStack[*top]);
    }
}

/*
 * _params[0..15]
 *
 */
void glopLoadMatrix(PtrGLContext pContext, OpParam _params[])
{
    PtrMatrix4 ptrStack = NULL, mat = NULL;
    int *top = NULL;

    if (!get_current_stack_matrix(pContext, &ptrStack, &top))
    {
        mat = &ptrStack[*top];
        mat->_m[0][0] = _params[0].f; mat->_m[0][1] = _params[4].f; mat->_m[0][2] = _params[8].f;  mat->_m[0][3] = _params[12].f; 
        mat->_m[1][0] = _params[1].f; mat->_m[1][1] = _params[5].f; mat->_m[1][2] = _params[9].f;  mat->_m[1][3] = _params[13].f; 
        mat->_m[2][0] = _params[2].f; mat->_m[2][1] = _params[6].f; mat->_m[2][2] = _params[10].f; mat->_m[2][3] = _params[14].f; 
        mat->_m[3][0] = _params[3].f; mat->_m[3][1] = _params[7].f; mat->_m[3][2] = _params[11].f; mat->_m[3][3] = _params[15].f; 
    }
}

void glopLoadIdentity(PtrGLContext pContext, OpParam _params[])
{
    PtrMatrix4 ptrStack = NULL;
    int *top = NULL;

    if (!get_current_stack_matrix(pContext, &ptrStack, &top))
    {
        make_identity_matrix(&ptrStack[*top]);
    }
}

/*
 * _params[0..15]
 *
 */
void glopMultMatrix(PtrGLContext pContext, OpParam _params[])
{
    PtrMatrix4 ptrStack = NULL;
    Matrix4 mat;
    int *top = NULL;

    mat._m[0][0] = _params[0].f; mat._m[0][1] = _params[4].f; mat._m[0][2] = _params[8].f;  mat._m[0][3] = _params[12].f; 
    mat._m[1][0] = _params[1].f; mat._m[1][1] = _params[5].f; mat._m[1][2] = _params[9].f;  mat._m[1][3] = _params[13].f; 
    mat._m[2][0] = _params[2].f; mat._m[2][1] = _params[6].f; mat._m[2][2] = _params[10].f; mat._m[2][3] = _params[14].f; 
    mat._m[3][0] = _params[3].f; mat._m[3][1] = _params[7].f; mat._m[3][2] = _params[11].f; mat._m[3][3] = _params[15].f; 
    if (!get_current_stack_matrix(pContext, &ptrStack, &top))
    {
        multiply_matrix_by_matrix(&ptrStack[*top], &mat, &ptrStack[*top]);
    }
}
