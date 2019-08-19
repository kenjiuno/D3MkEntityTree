
#pragma once

// http://oss.sgi.com/projects/ogl-sample/ABI/glext.h
// http://oss.sgi.com/projects/ogl-sample/registry/ARB/fragment_program.txt

// http://www.delphi3d.net/hardware/allexts.php

#include <gl/gl.h>

namespace GLExtz_c_ARB_fragment_program
{

enum cv_ARB_fragment_program {
GL_FRAGMENT_PROGRAM_ARB = 0x8804,
GL_PROGRAM_FORMAT_ASCII_ARB = 0x8875,
GL_PROGRAM_LENGTH_ARB = 0x8627,
GL_PROGRAM_FORMAT_ARB = 0x8876,
GL_PROGRAM_BINDING_ARB = 0x8677,
GL_PROGRAM_INSTRUCTIONS_ARB = 0x88A0,
GL_MAX_PROGRAM_INSTRUCTIONS_ARB = 0x88A1,
GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB = 0x88A2,
GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB = 0x88A3,
GL_PROGRAM_TEMPORARIES_ARB = 0x88A4,
GL_MAX_PROGRAM_TEMPORARIES_ARB = 0x88A5,
GL_PROGRAM_NATIVE_TEMPORARIES_ARB = 0x88A6,
GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB = 0x88A7,
GL_PROGRAM_PARAMETERS_ARB = 0x88A8,
GL_MAX_PROGRAM_PARAMETERS_ARB = 0x88A9,
GL_PROGRAM_NATIVE_PARAMETERS_ARB = 0x88AA,
GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB = 0x88AB,
GL_PROGRAM_ATTRIBS_ARB = 0x88AC,
GL_MAX_PROGRAM_ATTRIBS_ARB = 0x88AD,
GL_PROGRAM_NATIVE_ATTRIBS_ARB = 0x88AE,
GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB = 0x88AF,
GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB = 0x88B4,
GL_MAX_PROGRAM_ENV_PARAMETERS_ARB = 0x88B5,
GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB = 0x88B6,
GL_PROGRAM_ALU_INSTRUCTIONS_ARB = 0x8805,
GL_PROGRAM_TEX_INSTRUCTIONS_ARB = 0x8806,
GL_PROGRAM_TEX_INDIRECTIONS_ARB = 0x8807,
GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB = 0x8808,
GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB = 0x8809,
GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB = 0x880A,
GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB = 0x880B,
GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB = 0x880C,
GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB = 0x880D,
GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB = 0x880E,
GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB = 0x880F,
GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB = 0x8810,
GL_PROGRAM_STRING_ARB = 0x8628,
GL_PROGRAM_ERROR_POSITION_ARB = 0x864B,
GL_CURRENT_MATRIX_ARB = 0x8641,
GL_TRANSPOSE_CURRENT_MATRIX_ARB = 0x88B7,
GL_CURRENT_MATRIX_STACK_DEPTH_ARB = 0x8640,
GL_MAX_PROGRAM_MATRICES_ARB = 0x862F,
GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB = 0x862E,
GL_MAX_TEXTURE_COORDS_ARB = 0x8871,
GL_MAX_TEXTURE_IMAGE_UNITS_ARB = 0x8872,
GL_PROGRAM_ERROR_STRING_ARB = 0x8874,
GL_MATRIX0_ARB = 0x88C0,
GL_MATRIX1_ARB = 0x88C1,
GL_MATRIX2_ARB = 0x88C2,
GL_MATRIX3_ARB = 0x88C3,
GL_MATRIX4_ARB = 0x88C4,
GL_MATRIX5_ARB = 0x88C5,
GL_MATRIX6_ARB = 0x88C6,
GL_MATRIX7_ARB = 0x88C7,
GL_MATRIX8_ARB = 0x88C8,
GL_MATRIX9_ARB = 0x88C9,
GL_MATRIX10_ARB = 0x88CA,
GL_MATRIX11_ARB = 0x88CB,
GL_MATRIX12_ARB = 0x88CC,
GL_MATRIX13_ARB = 0x88CD,
GL_MATRIX14_ARB = 0x88CE,
GL_MATRIX15_ARB = 0x88CF,
GL_MATRIX16_ARB = 0x88D0,
GL_MATRIX17_ARB = 0x88D1,
GL_MATRIX18_ARB = 0x88D2,
GL_MATRIX19_ARB = 0x88D3,
GL_MATRIX20_ARB = 0x88D4,
GL_MATRIX21_ARB = 0x88D5,
GL_MATRIX22_ARB = 0x88D6,
GL_MATRIX23_ARB = 0x88D7,
GL_MATRIX24_ARB = 0x88D8,
GL_MATRIX25_ARB = 0x88D9,
GL_MATRIX26_ARB = 0x88DA,
GL_MATRIX27_ARB = 0x88DB,
GL_MATRIX28_ARB = 0x88DC,
GL_MATRIX29_ARB = 0x88DD,
GL_MATRIX30_ARB = 0x88DE,
GL_MATRIX31_ARB = 0x88DF,
};

};

namespace GLExtz_c_ARB_multitexture
{

enum cv_ARB_multitexture {
GL_TEXTURE0_ARB = 0x84C0,
GL_TEXTURE1_ARB = 0x84C1,
GL_TEXTURE2_ARB = 0x84C2,
GL_TEXTURE3_ARB = 0x84C3,
GL_TEXTURE4_ARB = 0x84C4,
GL_TEXTURE5_ARB = 0x84C5,
GL_TEXTURE6_ARB = 0x84C6,
GL_TEXTURE7_ARB = 0x84C7,
GL_TEXTURE8_ARB = 0x84C8,
GL_TEXTURE9_ARB = 0x84C9,
GL_TEXTURE10_ARB = 0x84CA,
GL_TEXTURE11_ARB = 0x84CB,
GL_TEXTURE12_ARB = 0x84CC,
GL_TEXTURE13_ARB = 0x84CD,
GL_TEXTURE14_ARB = 0x84CE,
GL_TEXTURE15_ARB = 0x84CF,
GL_TEXTURE16_ARB = 0x84D0,
GL_TEXTURE17_ARB = 0x84D1,
GL_TEXTURE18_ARB = 0x84D2,
GL_TEXTURE19_ARB = 0x84D3,
GL_TEXTURE20_ARB = 0x84D4,
GL_TEXTURE21_ARB = 0x84D5,
GL_TEXTURE22_ARB = 0x84D6,
GL_TEXTURE23_ARB = 0x84D7,
GL_TEXTURE24_ARB = 0x84D8,
GL_TEXTURE25_ARB = 0x84D9,
GL_TEXTURE26_ARB = 0x84DA,
GL_TEXTURE27_ARB = 0x84DB,
GL_TEXTURE28_ARB = 0x84DC,
GL_TEXTURE29_ARB = 0x84DD,
GL_TEXTURE30_ARB = 0x84DE,
GL_TEXTURE31_ARB = 0x84DF,
GL_ACTIVE_TEXTURE_ARB = 0x84E0,
GL_CLIENT_ACTIVE_TEXTURE_ARB = 0x84E1,
GL_MAX_TEXTURE_UNITS_ARB = 0x84E2,
};

};

namespace GLExtz_c_ARB_texture_env_combine
{

enum cv_ARB_texture_env_combine {
GL_COMBINE_ARB = 0x8570,
GL_COMBINE_RGB_ARB = 0x8571,
GL_COMBINE_ALPHA_ARB = 0x8572,
GL_SOURCE0_RGB_ARB = 0x8580,
GL_SOURCE1_RGB_ARB = 0x8581,
GL_SOURCE2_RGB_ARB = 0x8582,
GL_SOURCE0_ALPHA_ARB = 0x8588,
GL_SOURCE1_ALPHA_ARB = 0x8589,
GL_SOURCE2_ALPHA_ARB = 0x858A,
GL_OPERAND0_RGB_ARB = 0x8590,
GL_OPERAND1_RGB_ARB = 0x8591,
GL_OPERAND2_RGB_ARB = 0x8592,
GL_OPERAND0_ALPHA_ARB = 0x8598,
GL_OPERAND1_ALPHA_ARB = 0x8599,
GL_OPERAND2_ALPHA_ARB = 0x859A,
GL_RGB_SCALE_ARB = 0x8573,
GL_ADD_SIGNED_ARB = 0x8574,
GL_INTERPOLATE_ARB = 0x8575,
GL_SUBTRACT_ARB = 0x84E7,
GL_CONSTANT_ARB = 0x8576,
GL_PRIMARY_COLOR_ARB = 0x8577,
GL_PREVIOUS_ARB = 0x8578,
};

};

namespace GLExtz_c_ARB_texture_env_dot3
{

enum cv_ARB_texture_env_dot3 {
GL_DOT3_RGB_ARB = 0x86AE,
GL_DOT3_RGBA_ARB = 0x86AF,
};

};

namespace GLExtz_c_WGL_EXT_pixel_format
{

enum cv_WGL_EXT_pixel_format {
WGL_NUMBER_PIXEL_FORMATS_EXT = 0x2000,
WGL_DRAW_TO_WINDOW_EXT = 0x2001,
WGL_DRAW_TO_BITMAP_EXT = 0x2002,
WGL_ACCELERATION_EXT = 0x2003,
WGL_NEED_PALETTE_EXT = 0x2004,
WGL_NEED_SYSTEM_PALETTE_EXT = 0x2005,
WGL_SWAP_LAYER_BUFFERS_EXT = 0x2006,
WGL_SWAP_METHOD_EXT = 0x2007,
WGL_NUMBER_OVERLAYS_EXT = 0x2008,
WGL_NUMBER_UNDERLAYS_EXT = 0x2009,
WGL_TRANSPARENT_EXT = 0x200A,
WGL_TRANSPARENT_VALUE_EXT = 0x200B,
WGL_SHARE_DEPTH_EXT = 0x200C,
WGL_SHARE_STENCIL_EXT = 0x200D,
WGL_SHARE_ACCUM_EXT = 0x200E,
WGL_SUPPORT_GDI_EXT = 0x200F,
WGL_SUPPORT_OPENGL_EXT = 0x2010,
WGL_DOUBLE_BUFFER_EXT = 0x2011,
WGL_STEREO_EXT = 0x2012,
WGL_PIXEL_TYPE_EXT = 0x2013,
WGL_COLOR_BITS_EXT = 0x2014,
WGL_RED_BITS_EXT = 0x2015,
WGL_RED_SHIFT_EXT = 0x2016,
WGL_GREEN_BITS_EXT = 0x2017,
WGL_GREEN_SHIFT_EXT = 0x2018,
WGL_BLUE_BITS_EXT = 0x2019,
WGL_BLUE_SHIFT_EXT = 0x201A,
WGL_ALPHA_BITS_EXT = 0x201B,
WGL_ALPHA_SHIFT_EXT = 0x201C,
WGL_ACCUM_BITS_EXT = 0x201D,
WGL_ACCUM_RED_BITS_EXT = 0x201E,
WGL_ACCUM_GREEN_BITS_EXT = 0x201F,
WGL_ACCUM_BLUE_BITS_EXT = 0x2020,
WGL_ACCUM_ALPHA_BITS_EXT = 0x2021,
WGL_DEPTH_BITS_EXT = 0x2022,
WGL_STENCIL_BITS_EXT = 0x2023,
WGL_AUX_BUFFERS_EXT = 0x2024,
WGL_NO_ACCELERATION_EXT = 0x2025,
WGL_GENERIC_ACCELERATION_EXT = 0x2026,
WGL_FULL_ACCELERATION_EXT = 0x2027,
WGL_SWAP_EXCHANGE_EXT = 0x2028,
WGL_SWAP_COPY_EXT = 0x2029,
WGL_SWAP_UNDEFINED_EXT = 0x202A,
WGL_TYPE_RGBA_EXT = 0x202B,
WGL_TYPE_COLORINDEX_EXT = 0x202C,
};

};

namespace GLExtz_c_WGL_EXT_pbuffer
{

enum cv_WGL_EXT_pbuffer {
WGL_DRAW_TO_PBUFFER_EXT = 0x202D,
WGL_MAX_PBUFFER_PIXELS_EXT = 0x202E,
WGL_MAX_PBUFFER_WIDTH_EXT = 0x202F,
WGL_MAX_PBUFFER_HEIGHT_EXT = 0x2030,
WGL_OPTIMAL_PBUFFER_WIDTH_EXT = 0x2031,
WGL_OPTIMAL_PBUFFER_HEIGHT_EXT = 0x2032,
WGL_PBUFFER_LARGEST_EXT = 0x2033,
WGL_PBUFFER_WIDTH_EXT = 0x2034,
WGL_PBUFFER_HEIGHT_EXT = 0x2035,
};

};

namespace GLExtz_c_WGL_ARB_pixel_format
{

enum cv_WGL_ARB_pixel_format {
WGL_NUMBER_PIXEL_FORMATS_ARB = 0x2000,
WGL_DRAW_TO_WINDOW_ARB = 0x2001,
WGL_DRAW_TO_BITMAP_ARB = 0x2002,
WGL_ACCELERATION_ARB = 0x2003,
WGL_NEED_PALETTE_ARB = 0x2004,
WGL_NEED_SYSTEM_PALETTE_ARB = 0x2005,
WGL_SWAP_LAYER_BUFFERS_ARB = 0x2006,
WGL_SWAP_METHOD_ARB = 0x2007,
WGL_NUMBER_OVERLAYS_ARB = 0x2008,
WGL_NUMBER_UNDERLAYS_ARB = 0x2009,
WGL_TRANSPARENT_ARB = 0x200A,
WGL_TRANSPARENT_RED_VALUE_ARB = 0x2037,
WGL_TRANSPARENT_GREEN_VALUE_ARB = 0x2038,
WGL_TRANSPARENT_BLUE_VALUE_ARB = 0x2039,
WGL_TRANSPARENT_ALPHA_VALUE_ARB = 0x203A,
WGL_TRANSPARENT_INDEX_VALUE_ARB = 0x203B,
WGL_SHARE_DEPTH_ARB = 0x200C,
WGL_SHARE_STENCIL_ARB = 0x200D,
WGL_SHARE_ACCUM_ARB = 0x200E,
WGL_SUPPORT_GDI_ARB = 0x200F,
WGL_SUPPORT_OPENGL_ARB = 0x2010,
WGL_DOUBLE_BUFFER_ARB = 0x2011,
WGL_STEREO_ARB = 0x2012,
WGL_PIXEL_TYPE_ARB = 0x2013,
WGL_COLOR_BITS_ARB = 0x2014,
WGL_RED_BITS_ARB = 0x2015,
WGL_RED_SHIFT_ARB = 0x2016,
WGL_GREEN_BITS_ARB = 0x2017,
WGL_GREEN_SHIFT_ARB = 0x2018,
WGL_BLUE_BITS_ARB = 0x2019,
WGL_BLUE_SHIFT_ARB = 0x201A,
WGL_ALPHA_BITS_ARB = 0x201B,
WGL_ALPHA_SHIFT_ARB = 0x201C,
WGL_ACCUM_BITS_ARB = 0x201D,
WGL_ACCUM_RED_BITS_ARB = 0x201E,
WGL_ACCUM_GREEN_BITS_ARB = 0x201F,
WGL_ACCUM_BLUE_BITS_ARB = 0x2020,
WGL_ACCUM_ALPHA_BITS_ARB = 0x2021,
WGL_DEPTH_BITS_ARB = 0x2022,
WGL_STENCIL_BITS_ARB = 0x2023,
WGL_AUX_BUFFERS_ARB = 0x2024,
WGL_NO_ACCELERATION_ARB = 0x2025,
WGL_GENERIC_ACCELERATION_ARB = 0x2026,
WGL_FULL_ACCELERATION_ARB = 0x2027,
WGL_SWAP_EXCHANGE_ARB = 0x2028,
WGL_SWAP_COPY_ARB = 0x2029,
WGL_SWAP_UNDEFINED_ARB = 0x202A,
WGL_TYPE_RGBA_ARB = 0x202B,
WGL_TYPE_COLORINDEX_ARB = 0x202C,
};

};

namespace GLExtz_c_WGL_ARB_pbuffer
{

enum cv_WGL_ARB_pbuffer {
WGL_DRAW_TO_PBUFFER_ARB = 0x202D,
WGL_MAX_PBUFFER_PIXELS_ARB = 0x202E,
WGL_MAX_PBUFFER_WIDTH_ARB = 0x202F,
WGL_MAX_PBUFFER_HEIGHT_ARB = 0x2030,
WGL_PBUFFER_LARGEST_ARB = 0x2033,
WGL_PBUFFER_WIDTH_ARB = 0x2034,
WGL_PBUFFER_HEIGHT_ARB = 0x2035,
WGL_PBUFFER_LOST_ARB = 0x2036,
};

};

namespace GLExtz_c_WGL_ATI_pixel_format_float
{

enum cv_WGL_ATI_pixel_format_float {
RGBA_FLOAT_MODE_ATI = 0x8820,
COLOR_CLEAR_UNCLAMPED_VALUE_ATI = 0x8835,
WGL_TYPE_RGBA_FLOAT_ATI = 0x21A0,
};

};

namespace GLExtz_c_WGL_ARB_render_texture
{

enum cv_WGL_ARB_render_texture {
WGL_BIND_TO_TEXTURE_RGB_ARB = 0x2070,
WGL_BIND_TO_TEXTURE_RGBA_ARB = 0x2071,
WGL_TEXTURE_FORMAT_ARB = 0x2072,
WGL_TEXTURE_TARGET_ARB = 0x2073,
WGL_MIPMAP_TEXTURE_ARB = 0x2074,
WGL_TEXTURE_RGB_ARB = 0x2075,
WGL_TEXTURE_RGBA_ARB = 0x2076,
WGL_NO_TEXTURE_ARB = 0x2077,
WGL_TEXTURE_CUBE_MAP_ARB = 0x2078,
WGL_TEXTURE_1D_ARB = 0x2079,
WGL_TEXTURE_2D_ARB = 0x207A,
WGL_MIPMAP_LEVEL_ARB = 0x207B,
WGL_CUBE_MAP_FACE_ARB = 0x207C,
WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB = 0x207D,
WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB = 0x207E,
WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB = 0x207F,
WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB = 0x2080,
WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB = 0x2081,
WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB = 0x2082,
WGL_FRONT_LEFT_ARB = 0x2083,
WGL_FRONT_RIGHT_ARB = 0x2084,
WGL_BACK_LEFT_ARB = 0x2085,
WGL_BACK_RIGHT_ARB = 0x2086,
WGL_AUX0_ARB = 0x2087,
WGL_AUX1_ARB = 0x2088,
WGL_AUX2_ARB = 0x2089,
WGL_AUX3_ARB = 0x208A,
WGL_AUX4_ARB = 0x208B,
WGL_AUX5_ARB = 0x208C,
WGL_AUX6_ARB = 0x208D,
WGL_AUX7_ARB = 0x208E,
WGL_AUX8_ARB = 0x208F,
WGL_AUX9_ARB = 0x2090,
};

};

namespace GLExtz_c_WGL_ARB_multisample
{

enum cv_WGL_ARB_multisample {
GLX_SAMPLE_BUFFERS_ARB = 100000,
GLX_SAMPLES_ARB = 100001,
WGL_SAMPLE_BUFFERS_ARB = 0x2041,
WGL_SAMPLES_ARB = 0x2042,
MULTISAMPLE_ARB = 0x809D,
SAMPLE_ALPHA_TO_COVERAGE_ARB = 0x809E,
SAMPLE_ALPHA_TO_ONE_ARB = 0x809F,
SAMPLE_COVERAGE_ARB = 0x80A0,
MULTISAMPLE_BIT_ARB = 0x20000000,
SAMPLE_BUFFERS_ARB = 0x80A8,
SAMPLES_ARB = 0x80A9,
SAMPLE_COVERAGE_VALUE_ARB = 0x80AA,
SAMPLE_COVERAGE_INVERT_ARB = 0x80AB,
};

};

#define GLEXTz_DECLARE(T,F) typedef T (* F##_t)
#define GLEXTz_ENTRY(F) F##_t F
#define GLEXTz_BIND(F) \
	F = NULL; \
	if (fAssign && (F = (F##_t)wglGetProcAddress("gl"#F)) == NULL) break; \

#define GLEXTz_WGL_BIND(F) \
	F = NULL; \
	if (fAssign && (F = (F##_t)wglGetProcAddress(#F)) == NULL) break; \

DECLARE_HANDLE(HPBUFFEREXT);
DECLARE_HANDLE(HPBUFFERARB);

struct CGLExtz
{
	// 
	bool fARB_fragment_program;
	bool fARB_multitexture;
	bool fARB_texture_env_combine;
	bool fARB_texture_env_dot3;
	bool fWGL_EXT_extensions_string;
	bool fWGL_EXT_pixel_format;
	bool fWGL_EXT_pbuffer;
	bool fWGL_ARB_extensions_string;
	bool fWGL_ARB_pbuffer;
	bool fWGL_ARB_pixel_format;
	bool fWGL_ARB_render_texture;

	CString strExts, strWGLExts, strWGLARBs;

	// 
	CGLExtz()
	{

	}
	// 
	~CGLExtz()
	{
		Close();
	}
	// 
	void Close()
	{
		Bind(false);
	}
	// 
	bool Load(HDC hDCAssoc = NULL)
	{
		Close();

		strExts = " " + CString((LPCSTR)glGetString(GL_EXTENSIONS)) + " ";

		if (Bind(true, hDCAssoc))
			return true;
		return false;
	}
	// 
	bool IsExtAvail(LPCSTR psz)
	{
		if (strExts.Find(" " + CString(psz) + " ") < 0)
			return false;
		return true;
	}
	// 
	bool IsWGLExtAvail(LPCSTR psz)
	{
		if (strWGLExts.Find(" " + CString(psz) + " ") < 0)
			return false;
		return true;
	}
	// 
	bool IsWGLARBAvail(LPCSTR psz)
	{
		if (strWGLARBs.Find(" " + CString(psz) + " ") < 0)
			return false;
		return true;
	}
	// 
	bool Is2ExtsAvail(LPCSTR psz)
	{
		if (IsWGLARBAvail(psz))
			return true;
		if (IsWGLExtAvail(psz))
			return true;

		return false;
	}
	// 
	bool Bind(bool fAssign, HDC hDCAssoc = NULL)
	{
		for (fARB_fragment_program = false; ; ) {
			GLEXTz_BIND(ProgramStringARB);
			GLEXTz_BIND(BindProgramARB);
			GLEXTz_BIND(DeleteProgramsARB);
			GLEXTz_BIND(GenProgramsARB);
			GLEXTz_BIND(ProgramEnvParameter4dARB);
			GLEXTz_BIND(ProgramEnvParameter4dvARB);
			GLEXTz_BIND(ProgramEnvParameter4fARB);
			GLEXTz_BIND(ProgramEnvParameter4fvARB);
			GLEXTz_BIND(ProgramLocalParameter4dARB);
			GLEXTz_BIND(ProgramLocalParameter4dvARB);
			GLEXTz_BIND(ProgramLocalParameter4fARB);
			GLEXTz_BIND(ProgramLocalParameter4fvARB);
			GLEXTz_BIND(GetProgramEnvParameterdvARB);
			GLEXTz_BIND(GetProgramEnvParameterfvARB);
			GLEXTz_BIND(GetProgramLocalParameterdvARB);
			GLEXTz_BIND(GetProgramLocalParameterfvARB);
			GLEXTz_BIND(GetProgramivARB);
			GLEXTz_BIND(GetProgramStringARB);
			GLEXTz_BIND(IsProgramARB);

			fARB_fragment_program = fAssign && IsExtAvail("GL_ARB_fragment_program");
			break;
		}

		for (fARB_multitexture = false; ; ) {
			GLEXTz_BIND(ActiveTextureARB);
			GLEXTz_BIND(ClientActiveTextureARB);
			GLEXTz_BIND(MultiTexCoord1dARB);
			GLEXTz_BIND(MultiTexCoord1dvARB);
			GLEXTz_BIND(MultiTexCoord1fARB);
			GLEXTz_BIND(MultiTexCoord1fvARB);
			GLEXTz_BIND(MultiTexCoord1iARB);
			GLEXTz_BIND(MultiTexCoord1ivARB);
			GLEXTz_BIND(MultiTexCoord1sARB);
			GLEXTz_BIND(MultiTexCoord1svARB);
			GLEXTz_BIND(MultiTexCoord2dARB);
			GLEXTz_BIND(MultiTexCoord2dvARB);
			GLEXTz_BIND(MultiTexCoord2fARB);
			GLEXTz_BIND(MultiTexCoord2fvARB);
			GLEXTz_BIND(MultiTexCoord2iARB);
			GLEXTz_BIND(MultiTexCoord2ivARB);
			GLEXTz_BIND(MultiTexCoord2sARB);
			GLEXTz_BIND(MultiTexCoord2svARB);
			GLEXTz_BIND(MultiTexCoord3dARB);
			GLEXTz_BIND(MultiTexCoord3dvARB);
			GLEXTz_BIND(MultiTexCoord3fARB);
			GLEXTz_BIND(MultiTexCoord3fvARB);
			GLEXTz_BIND(MultiTexCoord3iARB);
			GLEXTz_BIND(MultiTexCoord3ivARB);
			GLEXTz_BIND(MultiTexCoord3sARB);
			GLEXTz_BIND(MultiTexCoord3svARB);
			GLEXTz_BIND(MultiTexCoord4dARB);
			GLEXTz_BIND(MultiTexCoord4dvARB);
			GLEXTz_BIND(MultiTexCoord4fARB);
			GLEXTz_BIND(MultiTexCoord4fvARB);
			GLEXTz_BIND(MultiTexCoord4iARB);
			GLEXTz_BIND(MultiTexCoord4ivARB);
			GLEXTz_BIND(MultiTexCoord4sARB);
			GLEXTz_BIND(MultiTexCoord4svARB);

			fARB_multitexture = fAssign && IsExtAvail("GL_ARB_multitexture");
			break;
		}

		for (fARB_texture_env_combine = false; ; ) {

			fARB_texture_env_combine = fAssign && IsExtAvail("GL_ARB_texture_env_combine");
			break;
		}

		for (fARB_texture_env_dot3 = false; ; ) {

			fARB_texture_env_dot3 = fAssign && IsExtAvail("GL_ARB_texture_env_dot3");
			break;
		}

		// ->#<->#<->#<- WGL_EXT_extensions_string ->#<->#<->#<-
		for (fWGL_EXT_extensions_string = false; ; ) {

			GLEXTz_WGL_BIND(wglGetExtensionsStringEXT);

			fWGL_EXT_extensions_string = fAssign; // && IsExtAvail("WGL_EXT_extensions_string");
			break;
		}

		if (fWGL_EXT_extensions_string) {
			strWGLExts = " " + CString(wglGetExtensionsStringEXT()) + " ";
		} else {
			strWGLExts = "";
		}

		// ->#<->#<->#<- WGL_ARB_extensions_string ->#<->#<->#<-
		for (fWGL_ARB_extensions_string = false; ; ) {

			GLEXTz_WGL_BIND(wglGetExtensionsStringARB);

			fWGL_ARB_extensions_string = fAssign;
			break;
		}

		if (fWGL_ARB_extensions_string && hDCAssoc != NULL) {
			strWGLARBs = " " + CString(wglGetExtensionsStringARB(hDCAssoc)) + " ";
		} else {
			strWGLARBs = "";
		}

        // ->#<->#<->#<- WGL Extz. ->#<->#<->#<-
		for (fWGL_EXT_pixel_format = false; ; ) {

			GLEXTz_WGL_BIND(wglGetPixelFormatAttribivEXT);
			GLEXTz_WGL_BIND(wglGetPixelFormatAttribfvEXT);
			GLEXTz_WGL_BIND(wglChoosePixelFormatEXT);

			fWGL_EXT_pixel_format = fAssign && Is2ExtsAvail("WGL_EXT_pixel_format");
			break;
		}

		for (fWGL_EXT_pbuffer = false; ; ) {

			GLEXTz_WGL_BIND(wglCreatePbufferEXT);
			GLEXTz_WGL_BIND(wglGetPbufferDCEXT);
			GLEXTz_WGL_BIND(wglReleasePbufferDCEXT);
			GLEXTz_WGL_BIND(wglDestroyPbufferEXT);
			GLEXTz_WGL_BIND(wglQueryPbufferEXT);

			fWGL_EXT_pbuffer = fAssign && Is2ExtsAvail("WGL_EXT_pbuffer");
			break;
		}

		for (fWGL_ARB_pbuffer = false; ; ) {

			GLEXTz_WGL_BIND(wglCreatePbufferARB);
			GLEXTz_WGL_BIND(wglGetPbufferDCARB);
			GLEXTz_WGL_BIND(wglReleasePbufferDCARB);
			GLEXTz_WGL_BIND(wglDestroyPbufferARB);
			GLEXTz_WGL_BIND(wglQueryPbufferARB);

			fWGL_ARB_pbuffer = fAssign && Is2ExtsAvail("WGL_ARB_pbuffer");
			break;
		}

		for (fWGL_ARB_pixel_format = false; ; ) {

			GLEXTz_WGL_BIND(wglGetPixelFormatAttribivARB);
			GLEXTz_WGL_BIND(wglGetPixelFormatAttribfvARB);
			GLEXTz_WGL_BIND(wglChoosePixelFormatARB);

			fWGL_ARB_pixel_format = fAssign && Is2ExtsAvail("WGL_ARB_pixel_format");
			break;
		}

		for (fWGL_ARB_render_texture = false; ; ) {

			GLEXTz_WGL_BIND(wglBindTexImageARB);
			GLEXTz_WGL_BIND(wglReleaseTexImageARB);
			GLEXTz_WGL_BIND(wglSetPbufferAttribARB);

			fWGL_ARB_render_texture = fAssign && Is2ExtsAvail("WGL_ARB_render_texture");
			break;
		}

		return true;
	}

	// 
	typedef void (APIENTRY * ProgramStringARB_t)(GLenum target, GLenum format, GLsizei len, const void *string);
	typedef void (APIENTRY * BindProgramARB_t)(GLenum target, GLuint program);
	typedef void (APIENTRY * DeleteProgramsARB_t)(GLsizei n, const GLuint *programs);
	typedef void (APIENTRY * GenProgramsARB_t)(GLsizei n, GLuint *programs);
	typedef void (APIENTRY * ProgramEnvParameter4dARB_t)(GLenum target, GLuint index, double x, double y, double z, double w);
	typedef void (APIENTRY * ProgramEnvParameter4dvARB_t)(GLenum target, GLuint index, const double *params);
	typedef void (APIENTRY * ProgramEnvParameter4fARB_t)(GLenum target, GLuint index, float x, float y, float z, float w);
	typedef void (APIENTRY * ProgramEnvParameter4fvARB_t)(GLenum target, GLuint index, const float *params);
	typedef void (APIENTRY * ProgramLocalParameter4dARB_t)(GLenum target, GLuint index, double x, double y, double z, double w);
	typedef void (APIENTRY * ProgramLocalParameter4dvARB_t)(GLenum target, GLuint index, const double *params);
	typedef void (APIENTRY * ProgramLocalParameter4fARB_t)(GLenum target, GLuint index, float x, float y, float z, float w);
	typedef void (APIENTRY * ProgramLocalParameter4fvARB_t)(GLenum target, GLuint index, const float *params);
	typedef void (APIENTRY * GetProgramEnvParameterdvARB_t)(GLenum target, GLuint index, double *params);
	typedef void (APIENTRY * GetProgramEnvParameterfvARB_t)(GLenum target, GLuint index, float *params);
	typedef void (APIENTRY * GetProgramLocalParameterdvARB_t)(GLenum target, GLuint index, double *params);
	typedef void (APIENTRY * GetProgramLocalParameterfvARB_t)(GLenum target, GLuint index, float *params);
	typedef void (APIENTRY * GetProgramivARB_t)(GLenum target, GLenum pname, int *params);
	typedef void (APIENTRY * GetProgramStringARB_t)(GLenum target, GLenum pname, void *string);
	typedef GLboolean (APIENTRY * IsProgramARB_t)(GLuint program);

	typedef void (APIENTRY * ActiveTextureARB_t)(GLenum);
	typedef void (APIENTRY * ClientActiveTextureARB_t)(GLenum);
	typedef void (APIENTRY * MultiTexCoord1dARB_t)(GLenum, GLdouble);
	typedef void (APIENTRY * MultiTexCoord1dvARB_t)(GLenum, const GLdouble *);
	typedef void (APIENTRY * MultiTexCoord1fARB_t)(GLenum, GLfloat);
	typedef void (APIENTRY * MultiTexCoord1fvARB_t)(GLenum, const GLfloat *);
	typedef void (APIENTRY * MultiTexCoord1iARB_t)(GLenum, GLint);
	typedef void (APIENTRY * MultiTexCoord1ivARB_t)(GLenum, const GLint *);
	typedef void (APIENTRY * MultiTexCoord1sARB_t)(GLenum, GLshort);
	typedef void (APIENTRY * MultiTexCoord1svARB_t)(GLenum, const GLshort *);
	typedef void (APIENTRY * MultiTexCoord2dARB_t)(GLenum, GLdouble, GLdouble);
	typedef void (APIENTRY * MultiTexCoord2dvARB_t)(GLenum, const GLdouble *);
	typedef void (APIENTRY * MultiTexCoord2fARB_t)(GLenum, GLfloat, GLfloat);
	typedef void (APIENTRY * MultiTexCoord2fvARB_t)(GLenum, const GLfloat *);
	typedef void (APIENTRY * MultiTexCoord2iARB_t)(GLenum, GLint, GLint);
	typedef void (APIENTRY * MultiTexCoord2ivARB_t)(GLenum, const GLint *);
	typedef void (APIENTRY * MultiTexCoord2sARB_t)(GLenum, GLshort, GLshort);
	typedef void (APIENTRY * MultiTexCoord2svARB_t)(GLenum, const GLshort *);
	typedef void (APIENTRY * MultiTexCoord3dARB_t)(GLenum, GLdouble, GLdouble, GLdouble);
	typedef void (APIENTRY * MultiTexCoord3dvARB_t)(GLenum, const GLdouble *);
	typedef void (APIENTRY * MultiTexCoord3fARB_t)(GLenum, GLfloat, GLfloat, GLfloat);
	typedef void (APIENTRY * MultiTexCoord3fvARB_t)(GLenum, const GLfloat *);
	typedef void (APIENTRY * MultiTexCoord3iARB_t)(GLenum, GLint, GLint, GLint);
	typedef void (APIENTRY * MultiTexCoord3ivARB_t)(GLenum, const GLint *);
	typedef void (APIENTRY * MultiTexCoord3sARB_t)(GLenum, GLshort, GLshort, GLshort);
	typedef void (APIENTRY * MultiTexCoord3svARB_t)(GLenum, const GLshort *);
	typedef void (APIENTRY * MultiTexCoord4dARB_t)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
	typedef void (APIENTRY * MultiTexCoord4dvARB_t)(GLenum, const GLdouble *);
	typedef void (APIENTRY * MultiTexCoord4fARB_t)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
	typedef void (APIENTRY * MultiTexCoord4fvARB_t)(GLenum, const GLfloat *);
	typedef void (APIENTRY * MultiTexCoord4iARB_t)(GLenum, GLint, GLint, GLint, GLint);
	typedef void (APIENTRY * MultiTexCoord4ivARB_t)(GLenum, const GLint *);
	typedef void (APIENTRY * MultiTexCoord4sARB_t)(GLenum, GLshort, GLshort, GLshort, GLshort);
	typedef void (APIENTRY * MultiTexCoord4svARB_t)(GLenum, const GLshort *);

	typedef const char * (APIENTRY * wglGetExtensionsStringEXT_t)();

	typedef BOOL (APIENTRY * wglGetPixelFormatAttribivEXT_t)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, int *piValues);
	typedef BOOL (APIENTRY * wglGetPixelFormatAttribfvEXT_t)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, FLOAT *pfValues);
	typedef BOOL (APIENTRY * wglChoosePixelFormatEXT_t)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

	typedef HPBUFFEREXT (APIENTRY * wglCreatePbufferEXT_t)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
	typedef HDC (APIENTRY * wglGetPbufferDCEXT_t)(HPBUFFEREXT hPbuffer);
	typedef int (APIENTRY * wglReleasePbufferDCEXT_t)(HPBUFFEREXT hPbuffer, HDC hDC);
	typedef BOOL (APIENTRY * wglDestroyPbufferEXT_t)(HPBUFFEREXT hPbuffer);
	typedef BOOL (APIENTRY * wglQueryPbufferEXT_t)(HPBUFFEREXT hPbuffer, int iAttribute, int *piValue);

	// typedef: WGL_ARB_extensions_string
	typedef const char * (APIENTRY * wglGetExtensionsStringARB_t)(HDC hdc);

	// typedef: WGL_ARB_pbuffer
	typedef HPBUFFERARB (APIENTRY * wglCreatePbufferARB_t)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
	typedef HDC (APIENTRY * wglGetPbufferDCARB_t)(HPBUFFERARB hPbuffer);
	typedef int (APIENTRY * wglReleasePbufferDCARB_t)(HPBUFFERARB hPbuffer, HDC hDC);
	typedef BOOL (APIENTRY * wglDestroyPbufferARB_t)(HPBUFFERARB hPbuffer);
	typedef BOOL (APIENTRY * wglQueryPbufferARB_t)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);

	// typedef WGL_ARB_pixel_format
	typedef BOOL (APIENTRY * wglGetPixelFormatAttribivARB_t)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
	typedef BOOL (APIENTRY * wglGetPixelFormatAttribfvARB_t)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
	typedef BOOL (APIENTRY * wglChoosePixelFormatARB_t)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

	// typedef: WGL_ARB_render_texture
	typedef BOOL (APIENTRY * wglBindTexImageARB_t)(HPBUFFERARB hPbuffer, int iBuffer);
	typedef BOOL (APIENTRY * wglReleaseTexImageARB_t)(HPBUFFERARB hPbuffer, int iBuffer);
	typedef BOOL (APIENTRY * wglSetPbufferAttribARB_t)(HPBUFFERARB hPbuffer, const int *piAttribList);

	// 
	ProgramStringARB_t ProgramStringARB;
	BindProgramARB_t BindProgramARB;
	DeleteProgramsARB_t DeleteProgramsARB;
	GenProgramsARB_t GenProgramsARB;
	ProgramEnvParameter4dARB_t ProgramEnvParameter4dARB;
	ProgramEnvParameter4dvARB_t ProgramEnvParameter4dvARB;
	ProgramEnvParameter4fARB_t ProgramEnvParameter4fARB;
	ProgramEnvParameter4fvARB_t ProgramEnvParameter4fvARB;
	ProgramLocalParameter4dARB_t ProgramLocalParameter4dARB;
	ProgramLocalParameter4dvARB_t ProgramLocalParameter4dvARB;
	ProgramLocalParameter4fARB_t ProgramLocalParameter4fARB;
	ProgramLocalParameter4fvARB_t ProgramLocalParameter4fvARB;
	GetProgramEnvParameterdvARB_t GetProgramEnvParameterdvARB;
	GetProgramEnvParameterfvARB_t GetProgramEnvParameterfvARB;
	GetProgramLocalParameterdvARB_t GetProgramLocalParameterdvARB;
	GetProgramLocalParameterfvARB_t GetProgramLocalParameterfvARB;
	GetProgramivARB_t GetProgramivARB;
	GetProgramStringARB_t GetProgramStringARB;
	IsProgramARB_t IsProgramARB;

	ActiveTextureARB_t ActiveTextureARB;
	ClientActiveTextureARB_t ClientActiveTextureARB;
	MultiTexCoord1dARB_t MultiTexCoord1dARB;
	MultiTexCoord1dvARB_t MultiTexCoord1dvARB;
	MultiTexCoord1fARB_t MultiTexCoord1fARB;
	MultiTexCoord1fvARB_t MultiTexCoord1fvARB;
	MultiTexCoord1iARB_t MultiTexCoord1iARB;
	MultiTexCoord1ivARB_t MultiTexCoord1ivARB;
	MultiTexCoord1sARB_t MultiTexCoord1sARB;
	MultiTexCoord1svARB_t MultiTexCoord1svARB;
	MultiTexCoord2dARB_t MultiTexCoord2dARB;
	MultiTexCoord2dvARB_t MultiTexCoord2dvARB;
	MultiTexCoord2fARB_t MultiTexCoord2fARB;
	MultiTexCoord2fvARB_t MultiTexCoord2fvARB;
	MultiTexCoord2iARB_t MultiTexCoord2iARB;
	MultiTexCoord2ivARB_t MultiTexCoord2ivARB;
	MultiTexCoord2sARB_t MultiTexCoord2sARB;
	MultiTexCoord2svARB_t MultiTexCoord2svARB;
	MultiTexCoord3dARB_t MultiTexCoord3dARB;
	MultiTexCoord3dvARB_t MultiTexCoord3dvARB;
	MultiTexCoord3fARB_t MultiTexCoord3fARB;
	MultiTexCoord3fvARB_t MultiTexCoord3fvARB;
	MultiTexCoord3iARB_t MultiTexCoord3iARB;
	MultiTexCoord3ivARB_t MultiTexCoord3ivARB;
	MultiTexCoord3sARB_t MultiTexCoord3sARB;
	MultiTexCoord3svARB_t MultiTexCoord3svARB;
	MultiTexCoord4dARB_t MultiTexCoord4dARB;
	MultiTexCoord4dvARB_t MultiTexCoord4dvARB;
	MultiTexCoord4fARB_t MultiTexCoord4fARB;
	MultiTexCoord4fvARB_t MultiTexCoord4fvARB;
	MultiTexCoord4iARB_t MultiTexCoord4iARB;
	MultiTexCoord4ivARB_t MultiTexCoord4ivARB;
	MultiTexCoord4sARB_t MultiTexCoord4sARB;
	MultiTexCoord4svARB_t MultiTexCoord4svARB;

	wglGetExtensionsStringEXT_t wglGetExtensionsStringEXT;

	wglGetPixelFormatAttribivEXT_t wglGetPixelFormatAttribivEXT;
	wglGetPixelFormatAttribfvEXT_t wglGetPixelFormatAttribfvEXT;
	wglChoosePixelFormatEXT_t wglChoosePixelFormatEXT;

	wglCreatePbufferEXT_t wglCreatePbufferEXT;
	wglGetPbufferDCEXT_t wglGetPbufferDCEXT;
	wglReleasePbufferDCEXT_t wglReleasePbufferDCEXT;
	wglDestroyPbufferEXT_t wglDestroyPbufferEXT;
	wglQueryPbufferEXT_t wglQueryPbufferEXT;

	// pfn: WGL_ARB_extensions_string
	wglGetExtensionsStringARB_t wglGetExtensionsStringARB;

	// pfn: WGL_ARB_pbuffer
	wglCreatePbufferARB_t wglCreatePbufferARB;
	wglGetPbufferDCARB_t wglGetPbufferDCARB;
	wglReleasePbufferDCARB_t wglReleasePbufferDCARB;
	wglDestroyPbufferARB_t wglDestroyPbufferARB;
	wglQueryPbufferARB_t wglQueryPbufferARB;

	// pfn: WGL_ARB_pixel_format
	wglGetPixelFormatAttribivARB_t wglGetPixelFormatAttribivARB;
	wglGetPixelFormatAttribfvARB_t wglGetPixelFormatAttribfvARB;
	wglChoosePixelFormatARB_t wglChoosePixelFormatARB;

	// pfn: WGL_ARB_render_texture
	wglBindTexImageARB_t wglBindTexImageARB;
	wglReleaseTexImageARB_t wglReleaseTexImageARB;
	wglSetPbufferAttribARB_t wglSetPbufferAttribARB;

};
