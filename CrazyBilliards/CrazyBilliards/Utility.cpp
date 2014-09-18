#include "Utility.h"

AUX_RGBImageRec* Utility::LoadBmpTexture(char* filename)
{
	FILE* pFile = NULL;
	if (!filename) {
		return NULL;
	}
	pFile = fopen(filename, "r");
	if(pFile) {
		fclose(pFile);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}

void Utility::GetPngTextureInfo(int color_type, glTextureT *texinfo)
{
	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		texinfo->format = GL_LUMINANCE;
		texinfo->internalFormat = 1;
		break;

	case PNG_COLOR_TYPE_GRAY_ALPHA:
		texinfo->format = GL_LUMINANCE_ALPHA;
		texinfo->internalFormat = 2;
		break;

	case PNG_COLOR_TYPE_RGB:
		texinfo->format = GL_RGB;
		texinfo->internalFormat = 3;
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		texinfo->format = GL_RGBA;
		texinfo->internalFormat = 4;
		break;

	default:
		/* Badness */
		break;
	}
}

glTextureT* Utility::LoadPngTexture(const char *filename)
{
	glTextureT*		texinfo;
	png_byte		magic[8];
	png_structp		png_ptr;
	png_infop		info_ptr;
	int				bit_depth, color_type;
	FILE*			fp = NULL;
	png_bytep*		row_pointers = NULL;
	int				i;

	/* open image file */
	fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "error: couldn't open \"%s\"!\n", filename);
		return NULL;
	}

	/* read magic number */
	fread(magic, 1, sizeof (magic), fp);

	/* check for valid magic number */
	if (!png_check_sig (magic, sizeof (magic))) {
		fprintf(stderr, "error: \"%s\" is not a valid PNG image!\n", filename);
		fclose(fp);
		return NULL;
	}

	/* create a png read struct */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		return NULL;
	}

	/* create a png info struct */
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return NULL;
    }

	/* create our OpenGL texture object */
	texinfo = (glTextureT *)malloc(sizeof (glTextureT));

	/* initialize the setjmp for returning properly after a libpng error occured */
	if (setjmp (png_jmpbuf(png_ptr))) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		if (row_pointers)
			free(row_pointers);

		if (texinfo) {
			if (texinfo->texels)
				free (texinfo->texels);

			free (texinfo);
		}

		return NULL;
	}

	/* setup libpng for using standard C fread() function with our FILE pointer */
	png_init_io(png_ptr, fp);

	/* tell libpng that we have already read the magic number */
	png_set_sig_bytes(png_ptr, sizeof (magic));

	/* read png info */
	png_read_info(png_ptr, info_ptr);

	/* get some usefull information from header */
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	/* convert index color images to RGB images */
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	/* convert 1-2-4 bits grayscale images to 8 bits grayscale. */
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	if (bit_depth == 16)
		png_set_strip_16(png_ptr);
	else if (bit_depth < 8)
		png_set_packing(png_ptr);

	/* update info structure to apply transformations */
	png_read_update_info(png_ptr, info_ptr);

	/* retrieve updated information */
	png_get_IHDR(png_ptr, info_ptr,
		(png_uint_32*)(&texinfo->width),
		(png_uint_32*)(&texinfo->height),
		&bit_depth, &color_type, NULL, NULL, NULL);

	/* get image format and components per pixel */
	GetPngTextureInfo(color_type, texinfo);

	/* we can now allocate memory for storing pixel data */
	texinfo->texels = (GLubyte *)malloc(sizeof (GLubyte) * texinfo->width * texinfo->height * texinfo->internalFormat);

	/* setup a pointer array.  Each one points at the begening of a row. */
	row_pointers = (png_bytep *)malloc(sizeof (png_bytep) * texinfo->height);

	for (i = 0; i < texinfo->height; ++i) {
		row_pointers[i] = (png_bytep)(texinfo->texels + ((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
	}

	/* read pixel data using row pointers */
	png_read_image(png_ptr, row_pointers);

	/* finish decompression and release memory */
	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	/* we don't need row pointers anymore */
	free(row_pointers);

	fclose(fp);
	return texinfo;
}

bool Utility::LoadTexture(const char* filename, GLuint& texture)
{
	int n = strlen(filename);
	if (n < 5) {
		return false;
	}
	if (filename[n - 4] == '.') {
		if ((filename[n - 3] == 'b' || filename[n - 3] == 'B') && 
			(filename[n - 2] == 'm' || filename[n - 2] == 'M') && 
			(filename[n - 1] == 'p' || filename[n - 1] == 'P')) {
				// TODO: load BMP;
				return false;
		}
		if ((filename[n - 3] == 'p' || filename[n - 3] == 'P') && 
			(filename[n - 2] == 'n' || filename[n - 2] == 'N') && 
			(filename[n - 1] == 'g' || filename[n - 1] == 'G')) {
				glTextureT* pngTex = NULL;
				pngTex = Utility::LoadPngTexture(filename);
				if (pngTex && pngTex->texels) {
					glGenTextures(1, &pngTex->id);
					glBindTexture(GL_TEXTURE_2D, pngTex->id);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glTexImage2D(GL_TEXTURE_2D, 0, pngTex->internalFormat,
						pngTex->width, pngTex->height, 0, pngTex->format, 
						GL_UNSIGNED_BYTE, pngTex->texels);

					texture = pngTex->id;

					free(pngTex->texels);
					free(pngTex);

					return true;
				}
		}
	}
	
	return false;
}

bool Utility::FloatCompare(float a, float b, float eps /*= COLLISION_EPS*/)
{
	return TCompare(a, b, eps);
}

bool Utility::IsCollide(float ax, float ay, float bx, float by, float len)
{
	float tmp1 = (ax - bx) * (ax - bx) + (ay - by) * (ay - by);
	float tmp2 = len * len;
	return tmp1 < tmp2 * 0.85f;
}

float Utility::Degree2Radian(float deg)
{
	return (deg * PI / 180.0f);
}

float Utility::Radian2Degree(float rad)
{
	return (rad * 180.0f / PI);
}

