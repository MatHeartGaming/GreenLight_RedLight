/*

		Copyright 2011 Etay Meiri

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "texture.h"
#include <assert.h>
#include <iostream>

#include "stb_image.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
}

bool Texture::Load()
{
	// try
	// {
	//   m_image.read(m_fileName);
	//   m_image.write(&m_blob, "RGBA");
	// }
	// catch (Magick::Error &Error)
	// {
	//   std::cout << "Error loading texture '" << m_fileName
	//             << "': " << Error.what() << std::endl;
	//   return false;
	// }

	int            width, height, numberComponents;
	unsigned char* data =
		stbi_load(m_fileName.c_str(), &width, &height, &numberComponents, 0);
	if (data)
	{
		GLenum format;
		if (numberComponents == 1)
			format = GL_RED;
		else if (numberComponents == 3)
			format = GL_RGB;
		else if (numberComponents == 4)
			format = GL_RGBA;
		else
			assert(0);

		glGenTextures(1, &m_textureObj);
		glBindTexture(GL_TEXTURE_2D, m_textureObj);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			format,
			width,
			height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// glGenTextures(1, &m_textureObj);
		// glBindTexture(m_textureTarget, m_textureObj);
		// glTexImage2D(m_textureTarget,
		//              0,
		//              GL_RGBA,
		//              m_image.columns(),
		//              m_image.rows(),
		//              0,
		//              GL_RGBA,
		//              GL_UNSIGNED_BYTE,
		//              m_blob.data());
		// glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// glBindTexture(m_textureTarget, 0);

		return true;
	}
	return false;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}
