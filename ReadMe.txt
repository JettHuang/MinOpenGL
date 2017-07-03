##项目描述
	为了更好的学习和理解OpenGL，吾决定实现一个简化版的OpenGL，该工程向大家展示了一个
三维引擎的工作原理，和必要的数学原理。在学习了GLSL(OpenGL Shading language)之后，我采用了
GLSL的思想，模块层次分明，尽量避免OpenGL的各个处理阶段的交织，清晰地展示OpenGL的PipeLine。

##系统架构
	请参考附图：myOpenGL Architecture.bmp

##支持的功能
	1. 2D texture
	2. Fog
	3. Lighting
	4. List
	5. Stencil
	6. Alpha

##支持的Apis (N/A is not support, A is support)
A.
	glAccum				N/A
	glActiveTexture			A
	glAlphaFunc			A
	glAreTexturesResident		N/A
	glArrayElement			A
	glAttachShader			N/A
B.	
	glBegin				A
	glBeginQuery			N/A
	glBindAttribLoaction		N/A
	glBindBuffer			N/A
	glBindTexture			A
	glBitmap			A
	glBlendColor			A
	glBlendEquation			A
	glBlendEquationSeparate		A
	glBlendFunc			A
	glBlendFuncSeparate		A
	glBufferData			N/A
	glBufferSubData			N/A
C.	
	glCallList			A
	glCallLists			A
	glClear				A
	glClearAccum			N/A
	glClearColor			A
	glClearDepth			A
	glClearIndex			N/A
	glClearStencil			A
	glClientActiveTexture		N/A
	glClipPlane			A
	glColor				A
	glColorMask			A
	glColorMaterial			A
	glColorPointer			A
	glColorSubTable			N/A
	glColorTable			N/A
	glColorTableParameter		N/A
	glCompileShader			N/A
	glCompressedTexImage1D		N/A
	glCompressedTexImage2D		N/A
	glCompressedTexImage3D		N/A
	glCompressedTexSubImage1D	N/A
	glCompressedTexSubImage2D	N/A
	glCompressedTexSubImage3D	N/A
	glConvolutionFilter1D		N/A
	glConvolutionFilter2D		N/A
	glConvolutionParameter		N/A
	glCopyColorSubTable		N/A
	glCopyColorTable		N/A
	glCopyTexImage1D		N/A
	glCopyTexImage2D		N/A
	glCopyTexSubImage1D		N/A
	glCopyTexSubImage2D		N/A
	glCopyTexSubImage3D		N/A
	glCreateProgram			N/A
	glCreateShader			N/A
	glCullFace			A
D.	
	glDeleteBuffers			N/A
	glDeleteLists			A
	glDeleteProgram			N/A
	glDeleteQueries			N/A
	glDeleteShader			N/A
	glDeleteTextures		A
	glDepthFunc			A
	glDepthMask			A
	glDepthRange			N/A
	glDetachShader			N/A
	glDisable			A
	glDisableClientState		A
	glDisableVertexAttribArray	N/A
	glDrawArrays			A
	glDrawBuffer			N/A
	glDrawBuffers			N/A
	glDrawElements			A
	glDrawPixels			A
	glDrawRangeElements		A

E.	glEdgeFlag			A
	glEdgeFlagPointer		A
	glEnable			A
	glEnableClientState		A
	glEnableVertexAttriArray	N/A
	glEnd				A
	glEndList			A
	glEndQuery			A
	glEvalCoord			N/A
	glEvalMesh			N/A
	glEvalPoint			N/A
	
F.	glFeedbackBuffer		A
	glFinish			A
	glFlush				A
	glFog				A
	glFogCoord			A
	glFogCoordPointer		
	glFrontFace			A
	glFrustum			A

G.	glGenBuffers			N/A
	glGenLists			A
	glGenQueries			N/A
	glGenTextures			A
	glGet				N/A
	glGetActiveAttrib		N/A
	glGetActiveUniform		N/A
	glGetAttachedShaders		N/A
	glGetAttribLocation		N/A
	glGetBufferParameteriv		N/A
	glGetBufferPointerv		N/A
	glGetBufferSubData		N/A
	glGetClipPlane			N/A
	glGetColorTable			N/A
	glGetColorTableParameter	N/A
	glGetCompressedTexImage		N/A
	glGetConvolutionFilter		N/A
	glGetConvolutionParameter	N/A
	glGetError			A
	glGetHistogram			N/A
	glGetHistogramParameter		N/A
	glGetLight			N/A
	glGetMap			N/A
	glGetMaterial			N/A
	glGetMinmax			N/A
	glGetMinmaxParameter		N/A
	glGetPixelMap			N/A
	glGetPointerv			N/A
	glGetPolygonStipple		N/A
	glGetProgram			N/A
	glGetProgramInfoLog		N/A
	glGetQueryObject		N/A
	glGetQueryiv			N/A
	glGetSeparableFilter		N/A
	glGetShader			N/A
	glGetshaderInfoLog		N/A
	glGetShaderSource		N/A
	glGetString			N/A
	glGetTexEnv			N/A
	glGetTexGen			N/A
	glGetTexImage			N/A
	glGetTexLevelParameter		N/A
	glGetTexParameter		N/A
	glGetUniform			N/A
	glGetUniformLocation		N/A
	glGetVertexAttrib		N/A
	glGetVertexAttribPointerv	N/A

H.	glHint				A
	glHistogram			N/A

I.	glIndex				N/A
	glIndexMask			N/A
	glIndexPointer			N/A
	glInitNames			A
	glInterleavedArrays		N/A
	glIsBuffer			N/A
	glIsEnabled			A
	glIsList			A
	glIsProgram			N/A
	glIsQuery			N/A
	glIsShader			N/A
	glIsTexture			A

L.	glLight				A
	glLightModel			A
	glLineStipple			A
	glLineWidth			A
	glLinkProgram			N/A
	glListBase			A
	glLoadIdentity			A	
	glLoadMatrix			A
	glLoadName			A
	glLoadTransposeMatrix		A
	glLogicOp			A
	
M.	glMap1				N/A
	glMap2				N/A
	glMapBuffer			N/A
	glMapGrid			N/A
	glMaterial			A
	glMatrixMode			A
	glMinmax			N/A
	glMultMatrix			A
	glMultTransposeMatrix		A
	glMultiDrawArrays		A
	glMultiDrawElements		A
	glMultiTexCoord			A
	
N.	glNewList			A
	glNormal			A
	glNormalPointer			A

O.	glOrtho				A
	
P.	glPassThrough			N/A
	glPixelMap			A
	glPixelStore			A
	glPixelTransfer			N/A
	glPixelZoom			A
	glPointParameter		A
	glPointSize			A
	glPolygonMode			A
	glPolygonOffset			A
	glPolygonStipple		A
	glPopAttrib			A/A
	glPopClientAttrib		N/A
	glPopMatrix			A
	glPopName			A
	glPrioritize			N/A
	glPushAttrib			N/A
	glPushClientAttrib		N/A
	glPushMatrix			A
	glPushName			A

R.	glRasterPos			A
	glReadBuffer			A
	glReadPixels			A
	glRect				A
	glRenderMode			A
	glResetHistogram		N/A
	glResetMinmax			N/A
	glRotate			A
	
S.	glSampleCoverage		N/A
	glScale				A
	glScissor			A
	glSecondaryColor		N/A
	glSecondaryColorPointer		N/A
	glSelectBuffer			A
	glSeparableFilter2D		N/A
	glShadeModel			A
	glShaderSource			N/A
	glStencilFunc			A
	glStencilFuncSeparate		A
	glStencilMask			A
	glStencilMaskSeparate		A
	glStencilOp			A
	glStencilOpSeparate		A

T.	glTexCoord			A
	glTexCoordPointer		A
	glTexEnv			A
	glTexGen			A
	glTexImage1D			N/A
	glTexImage2D			A
	glTexImage3D			N/A
	glTexParameter			A
	glTexSubImage1D			N/A
	glTexSubImage2D			N/A
	glTexSubImage3D			N/A
	glTranslate			A

U.	glUniform			N/A
	glUnmapBuffer			N/A
	glUseProgram			N/A
	
V.	glValidateProgram		N/A
	glVertex			A
	glVertexAttrib			N/A
	glVertexAttribPointer		N/A
	glVertexPointer			A
	glViewport			A

W.	glWindowPos			A


##支持Windows窗口的Apis
	wglCreateContext		A
	wglMakeCurrent			A
	wglDeleteContext		A



	








	