/**
 * File contains definition of FrameBufferObject class.
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

#include "FrameBufferObject.hpp"

namespace AyumiEngine
{
	namespace AyumiUtils
	{
		/**
		 * Constructor with initialize parameters. Create empty FrameBufferObject.
		 * @param	width is frame buffer width.
		 * @param	height is frame buffer height.
		 * @param	flags is frame buffer depth/stencil flags.
		 */
		FrameBufferObject::FrameBufferObject(const int width, const int height, const int flags)
		{
			this->width = width;
			this->height = height;
			this->flags = flags;

			frameBuffer = 0;
			depthBuffer = 0;
			stencilBuffer = 0;
	
			for (int i = 0; i < 8; ++i)
				colorBuffer[i] = 0;
		}

		/**
		 * Class destructor, free allocated memory. Delete frame and render buffers.
		 */
		FrameBufferObject::~FrameBufferObject()
		{
			if(depthBuffer != 0)
				glDeleteRenderbuffers(1,&depthBuffer);

			if(stencilBuffer != 0)
				glDeleteRenderbuffers (1,&stencilBuffer);

			if(frameBuffer != 0)
				glDeleteFramebuffers(1,&frameBuffer);
		}

		/**
		 * Method is used to create Frame Buffer Object and check creation status.
		 * @return	true if Frame Buffer was created without errors, false otherwise.
		 */
		bool FrameBufferObject::create()
		{
			if(width <= 0 || height <= 0)
			{
				std::cerr <<  "FBO create error occurred!" << std::endl;
				return false;
			}

			glGenFramebuffers(1,&frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);

			int	depthFormat = 0;
			int stencilFormat = 0;

			if(flags & depth16)
				depthFormat = GL_DEPTH_COMPONENT16;
			else if(flags & depth24)
				depthFormat = GL_DEPTH_COMPONENT24;
			else if(flags & depth32)
				depthFormat = GL_DEPTH_COMPONENT32;

			if(flags & stencil1)
				stencilFormat = GL_STENCIL_INDEX1;
			else if(flags & stencil4)
				stencilFormat = GL_STENCIL_INDEX4;
			else if(flags & stencil8)
				stencilFormat = GL_STENCIL_INDEX8;
			else if(flags & stencil16)
				stencilFormat = GL_STENCIL_INDEX16;

			if(depthFormat != 0)
			{
				glGenRenderbuffers(1,&depthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER,depthBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER,depthFormat,width,height); //Multisample wersja i to i to, wiec zmienic moze
				glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthBuffer);
			}

			if(stencilFormat != 0)
			{
				glGenRenderbuffers(1,&stencilBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER,stencilBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER,stencilFormat,width,height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,stencilBuffer);
			}

			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return status == GL_FRAMEBUFFER_COMPLETE;
		}

		/**
		 * Method is used to bind Frame Buffer Object. Each FBO need to be binded to use.
		 * @return	true if bind operation ended without errors, false otherwise.
		 */
		bool FrameBufferObject::bind()
		{
			if(frameBuffer == 0)
			{
				std::cerr <<  "FBO bind error occurred!" << std::endl;
				return false;
			}

			glFlush(); 
			glGetIntegerv(GL_VIEWPORT,saveViewport);
			glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glViewport(0,0,getWidth(),getHeight());

			return true;
		}

		/**
		 * Method is used to bind cube map face.
		 * @param	face is id of cube map face.
		 * @return	true if bind operation ended without errors, false otherwise.
		 */
		bool FrameBufferObject::bindFace (const int face)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, colorBuffer[0],0);
			return bind();
		}

		/**
		 * Method is used to unbind Frame Buffer Object. Each FBO need to be unbinded after using.
		 * @return	true if bind operation ended without errors, false otherwise.
		 */
		bool FrameBufferObject::unbind()
		{
			if(frameBuffer == 0)
			{
				std::cerr << "FBO unbind error occurred!" << std::endl;
				return false;
			}

			glFlush();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(saveViewport[0],saveViewport[1],saveViewport[2],saveViewport[3]);

			return true;
		}

		/**
		 * Method is used to check error status of Frame Buffer Object.
		 * @return	true if Frame Buffer Object is ok, false otherwise.
		 */
		bool FrameBufferObject::isOk () const
		{
			GLint currentFb;

			glGetIntegerv(GL_FRAMEBUFFER_BINDING,&currentFb);

			if(currentFb != frameBuffer)
			{
				glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
				glReadBuffer(GL_COLOR_ATTACHMENT0);
			}

			int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

			if (currentFb != frameBuffer)
				glBindFramebuffer(GL_FRAMEBUFFER,currentFb);

			return status == GL_FRAMEBUFFER_COMPLETE;
		}

		/**
		 * Method is used to attach color texture to Frame Buffer Object.
		 * @param	target is texture type.
		 * @param	texId is id of color texture.
		 * @param	no is Frame Buffer Object colorBuffer id.
		 * @return	true if color texture attached without errors, false otherwise.
		 */
		bool FrameBufferObject::attachColorTexture(GLenum target, unsigned texId, int no)
		{
			if(frameBuffer == 0)
			{
				std::cerr << "FBO attach color texture error occurred!" << std::endl;
				return false;
			}

			if(target != GL_TEXTURE_2D && target != GL_TEXTURE_RECTANGLE && (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X || target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z))
			{
				std::cerr << "FBO attach color texture wrong target error occurred!" << std::endl;
				return false;
			}

			glBindTexture(target,colorBuffer[no] = texId);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0 + no,target,texId,0);

			return true;
		}

		/**
		 * Method is used to attach depth texture to Frame Buffer Object.
		 * @param	target is texture type.
		 * @param	texId is id of depth texture.
		 * @return	true if depth texture attached without errors, false otherwise.
		 */
		bool FrameBufferObject::attachDepthTexture(GLenum target, unsigned texId)
		{
			if(frameBuffer == 0)
			{
				std::cerr << "FBO attach depth texture error occurred!" << std::endl;	
				return false;
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,target,depthBuffer = texId,0);

			return true;
		}

		/**
		 * Method is used to detach color texture from Frame Buffer Object.
		 * @param	target is texture type.
		 * @return	true if color texture dettached without errors, false otherwise.
		 */
		bool FrameBufferObject::detachColorTexture(GLenum target)
		{
			return attachColorTexture(target,0);
		}

		/**
		 * Method is used to detach depth texture from Frame Buffer Object.
		 * @param	target is texture type.
		 * @return	true if depth texture dettached without errors, false otherwise.
		 */
		bool FrameBufferObject::detachDepthTexture(GLenum target)
		{
			return attachDepthTexture(target,0);
		}

		/**
		 * Method is used to create color texture from Frame Buffer Object.
		 * @param	format is new texture format.
		 * @param	internalFormal is new  texture internal formal.
		 * @param	clamp is texture clamp type.
		 * @param	filer is texture filter type.
		 * @return	id of new color texture.
		 */
		unsigned FrameBufferObject::createColorTexture(GLenum format, GLenum internalFormat, GLenum clamp, int filter)
		{
			GLuint tex;

			glGenTextures(1,&tex);
			glBindTexture(GL_TEXTURE_2D,tex);
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			glTexImage2D(GL_TEXTURE_2D,0,internalFormat,getWidth(),getHeight(),0,format,GL_UNSIGNED_BYTE,nullptr);

			if(filter == filterNearest)
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			}
			else if(filter == filterLinear)
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			}
			else if(filter == filterMipmap)
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			}
	
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,clamp);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,clamp);

			return tex;
		}

		/**
		 * Method is used to create cube map texture from Frame Buffer Object.
		 * @param	format is new texture format.
		 * @param	internalFormal is new texture internal formal.
		 * @param	clamp is texture clamp type.
		 * @param	filer is texture filter type.
		 * @return	id of new cubemap texture.
		 */
		unsigned FrameBufferObject::createCubemapTexture(GLenum format, GLenum internalFormat, GLenum clamp, int filter)
		{
			if(getWidth()!= getHeight())
			{
				std::cerr << "FBO create cubemap texture error occurred!" << std::endl;	
				return 0;
			}
		
			GLuint tex;

			glGenTextures(1,&tex);
			glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,clamp);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,clamp);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,clamp);
	
			for ( int side = 0; side < 6; ++side)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,0,internalFormat,getWidth(),getWidth(),0,format,GL_UNSIGNED_BYTE,nullptr);
	
			if(filter == filterNearest)
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			}
			else if(filter == filterLinear)
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			}
			else if(filter == filterMipmap)
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			}
	
			return tex;
		}

		/**
		 * Method is used to create color rect texture from Frame Buffer Object.
		 * @param	format is new texture format.
		 * @param	internalFormal is new texture internal formal.
		 * @return	id of new rect texture.
		 */
		unsigned FrameBufferObject::createColorRectTexture(GLenum format, GLenum internalFormat)
		{
			GLuint tex;
			glGenTextures(1,&tex);
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			glBindTexture(GL_TEXTURE_RECTANGLE,tex);
			glTexImage2D(GL_TEXTURE_RECTANGLE,0,internalFormat,getWidth(),getHeight(),0,format,GL_UNSIGNED_BYTE,nullptr);
			glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_WRAP_T,GL_CLAMP);
			return tex;
		}

		/**
		 * Method is used to build texture mipmapis for mipmapping technique.
		 * @param	target is texture type.
		 */
		void FrameBufferObject::buildMipmaps(GLenum target) const
		{
			glBindTexture(target, getColorBuffer());
			glGenerateMipmap(target);
		}
	
		/**
		 * Accessor to Frame Buffer Object private width member.
		 * @return	width of Frame Buffer Object.
		 */
		int	FrameBufferObject::getWidth() const
		{
			return width;
		}
	
		/**
		 * Accessor to Frame Buffer Object private height member.
		 * @return	height of Frame Buffer Object.
		 */
		int	FrameBufferObject::getHeight() const
		{
			return height;
		}
		
		/**
		 * Method is used to check if Frame Buffer Object has stencil buffer.
		 * @return	true if Frame Buffer Object has stencil buffer, false otherwise.
		 */
		bool FrameBufferObject::hasStencil() const
		{
			return stencilBuffer != 0;
		}

		/**
		 * Method is used to check if Frame Buffer Object has depth buffer.
		 * @return	true if Frame Buffer Object has depth buffer, false otherwise.
		 */
		bool FrameBufferObject::hasDepth() const
		{
			return depthBuffer != 0;
		}
	
		/**
		 * Accessor to Frame Buffer Object private color buffer member.
		 * @param	no is color buffer id.
		 * @return	id of Frame Buffer Object color buffer.
		 */
		unsigned FrameBufferObject::getColorBuffer(int no) const
		{
			return colorBuffer[no];
		}

		/**
		 * Accessor to Frame Buffer Object private depth buffer member.
		 * @return	id of Frame Buffer Object depth buffer.
		 */
		unsigned FrameBufferObject::getDepthBuffer() const
		{
			return depthBuffer;
		}

		/**
		 * Static method is used to get maximum color attachments amount of Frame Buffer Object.
		 * @return	maximum amount of color attachments.
		 */
		int FrameBufferObject::maxColorAttachemnts()
		{
			GLint amount = 0;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,&amount);
			return amount;
		}

		/**
		 * Static method is used to get maximum size of Frame Buffer Object render buffer.
		 * @return	maximum size of Frame Buffer Object render buffer.
		 */
		int FrameBufferObject::maxSize()
		{
			GLint size = 0;
			glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&size);
			return size;
		}
	}
}
