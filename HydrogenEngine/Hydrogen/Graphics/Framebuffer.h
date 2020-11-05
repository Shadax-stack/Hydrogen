#ifndef HYDROGEN_FRAMEBUFFER_H
#define HYDROGEN_FRAMEBUFFER_H
#include "../Core/HydrogenEngine.h"
#include "Handle.h"
#include "Texture.h"
#include "Texture2D.h"
#include "TextureCubemap.h"
#include <vector>

namespace Hydrogen {

	enum HydrogenFramebufferTextureAttachment {
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_0 = GL_COLOR_ATTACHMENT0,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_1 = GL_COLOR_ATTACHMENT1,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_2 = GL_COLOR_ATTACHMENT2,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_3 = GL_COLOR_ATTACHMENT3,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_4 = GL_COLOR_ATTACHMENT4,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_5 = GL_COLOR_ATTACHMENT5,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_6 = GL_COLOR_ATTACHMENT6,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_7 = GL_COLOR_ATTACHMENT7,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_8 = GL_COLOR_ATTACHMENT8,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_9 = GL_COLOR_ATTACHMENT9,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_10 = GL_COLOR_ATTACHMENT10,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_11 = GL_COLOR_ATTACHMENT11,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_12 = GL_COLOR_ATTACHMENT12,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_13 = GL_COLOR_ATTACHMENT13,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_14 = GL_COLOR_ATTACHMENT14,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_15 = GL_COLOR_ATTACHMENT15,
		HYDROGEN_FRAMEBUFFER_ATTACHMENT_DEPTH = GL_DEPTH_ATTACHMENT
	};

	struct HYDROGEN_API HydrogenFramebufferAttachment {
		HydrogenTextureTarget Target; //We find out what type the texture is from this parameter
		HydrogenFramebufferTextureAttachment Attachment; //what attachment this is
		HydrogenTexture* Texture; //An arbirarty texture
	};

	//In this rendering engine I simply do not care about the stencil buffer so basically the only real types of attachments are color and depth, no depth/stencil

	struct HYDROGEN_API HydrogenFramebufferInfo : public HydrogenHandleInfo {
		std::vector<HydrogenFramebufferAttachment> Attachments;
		int Width, Height; 
	};

	class HYDROGEN_API HydrogenFramebuffer : public HydrogenHandle {
	private:
		HydrogenFramebufferInfo FramebufferInfo;
	public:
		void CreateIdentifier(void);
		void DeleteIdentifier(void);
		void SetIdentifier(unsigned int id) = delete;
		void AttachTexture(HydrogenTextureTarget target, HydrogenFramebufferTextureAttachment attachment, HydrogenTexture* texture);
		void AttachTexture(HydrogenFramebufferAttachment attachment);
		void Bind();
		static void Unbind(void);
		HydrogenFramebufferInfo* GetFramebufferInfo(void);
	};

}

#endif