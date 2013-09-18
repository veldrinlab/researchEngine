/**
 * File contains declaraion of FrameBufferObject class.
 * @file    FrameBufferObject.hpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2011-08-05
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef FRAMEBUFFEROBJECT_HPP
#define FRAMEBUFFEROBJECT_HPP

#include <GL/glew.h>
#include <iostream>

namespace AyumiEngine
{
	namespace AyumiUtils
	{
		/**
		 * Enumeration represents flags for depth and stencil buffers which will be used to configure
		 * FrameBufferObject.
		 */
		enum BufferFlag
		{
			depth16 = 1, // 16-bit depth buffer
			depth24 = 2, // 24-bit depth buffer
			depth32 = 4, // 32-bit depth buffer
			stencil1  = 16, // 1-bit stencil buffer
			stencil4  = 32, // 4-bit stencil buffer
			stencil8  = 64, // 8-bit stencil buffer
			stencil16 = 128 // 16-bit stencil buffer
		};
	
		/**
		 * Enumeration represents texture filer modes which will be used to configure FrameBufferObjects
		 */
		enum FilterFlag
		{
			filterNearest = 0,
			filterLinear  = 1,
			filterMipmap  = 2
		};

		/**
		 * Class represents FrameBufferObject which is a collection of 2D arrays or storages utilized by OpenGL;
		 * colour buffers, depth buffer, stencil buffer and accumulation buffer. By default, OpenGL uses the 
		 * framebuffer as a rendering destination that is created and managed entirely by the window system.
		 * FrameBufferObject is used to do off-screen rendering, including rendering to a texture, which will
		 * be used to manage post-process effects like shadows, blurs etc.
		 */
		class FrameBufferObject
		{
		private:
			int width;
			int height;
			int flags;

			GLuint frameBuffer;
			GLuint colorBuffer[8];
			GLuint depthBuffer;
			GLuint stencilBuffer; 
			GLint saveViewport[4];

		public:
			FrameBufferObject(const int width, const int height, const int flags = 0);
			~FrameBufferObject();

			bool create();
			bool bind();
			bool bindFace(const int face);
			bool unbind();
			bool isOk() const;
			bool attachColorTexture(GLenum target, unsigned texId, int no = 0);
			bool attachDepthTexture(GLenum target, unsigned texId);
			bool detachColorTexture(GLenum target);
			bool detachDepthTexture(GLenum target);
			unsigned createColorTexture(GLenum format = GL_RGBA, GLenum internalFormat = GL_RGBA8, GLenum clamp = GL_REPEAT, int filter = filterLinear);			 
			unsigned createColorRectTexture(GLenum format = GL_RGBA, GLenum internalFormat = GL_RGBA8);
			unsigned createCubemapTexture(GLenum format = GL_RGBA, GLenum internalFormat = GL_RGBA8, GLenum clamp = GL_CLAMP_TO_EDGE, int filter = filterLinear);
			void buildMipmaps(GLenum target = GL_TEXTURE_2D) const;
			int	getWidth () const;
			int	getHeight () const;
			bool hasStencil () const;
			bool hasDepth () const;
			unsigned getColorBuffer ( int no = 0 ) const;
			unsigned getDepthBuffer () const;
			static int maxColorAttachemnts();
			static int maxSize();
		};
	}
}
#endif
