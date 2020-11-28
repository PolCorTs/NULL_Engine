#ifndef __I_TEXTURES_H__
#define __I_TEXTURES_H__

#include <string>
#include "VarTypedefs.h"

class R_Texture;

namespace Importer
{
	namespace Textures
	{	
		void	Init();
		void	CleanUp();
		
		uint	Import(const char* path, R_Texture* r_texture);

		uint64	Save(const R_Texture* r_texture, char** buffer);
		uint64	Save(R_Texture* r_texture, char** buffer);

		bool	Load(const char* buffer, const uint size, R_Texture* r_texture);
		bool	Load(R_Texture* r_texture);

		namespace Utilities
		{	
			uint CreateTexture(const void* data, uint width,
												 uint height,
												 uint target = 0x0DE1, 				// 0x0DE1 = GL_TEXTURE_2D
												 int filter = 0x2600,				// 0x2600 = GL_NEAREST
												 int wrapping = 0x2901,				// 0x2901 = GL_REPEAT
												 int internal_format = 0x1908,		// 0x1908 = GL_RGBA
												 uint format = 0x1908); 			// 0x1908 = GL_RGBA

			//uint CreateTexture(const void* data, TextureParameters tex_param);
		}
	}
}

#endif // !__I_TEXTURES_H__