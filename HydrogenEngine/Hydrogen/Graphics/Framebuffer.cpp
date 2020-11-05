#include "Framebuffer.h"

#define Framebuffer Handle

namespace Hydrogen {

	void HydrogenFramebuffer::CreateIdentifier(void) {
		glCall(glGenFramebuffers(1, &Framebuffer));
	}

	void HydrogenFramebuffer::DeleteIdentifier(void) {
		glCall(glDeleteFramebuffers(1, &Framebuffer));
	}

	void HydrogenFramebuffer::AttachTexture(HydrogenTextureTarget target, HydrogenFramebufferTextureAttachment attachment, HydrogenTexture* texture) {
		HydrogenFramebufferAttachment AttachmentInfo;
		AttachmentInfo.Target = target;
		AttachmentInfo.Attachment = attachment;
		AttachmentInfo.Texture = texture;
		AttachTexture(AttachmentInfo);
	}

	void HydrogenFramebuffer::AttachTexture(HydrogenFramebufferAttachment attachment) {
		Bind();
		switch (attachment.Target) {
		case HYDROGEN_TEXTURE_TARGET_TEXTURE_2D:
			glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.Attachment, HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, attachment.Texture->GetIdentifier(), 0));
			break;
		case HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP:
			glCall(glFramebufferTexture(GL_FRAMEBUFFER, attachment.Attachment, attachment.Texture->GetIdentifier(), 0));
			break;
		default:
			break;
		}
		int Status = glCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			exit(-1);
		}
		Unbind();
		FramebufferInfo.Attachments.push_back(attachment);
	}

	void HydrogenFramebuffer::Bind() {
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer));
	}
	
	void HydrogenFramebuffer::Unbind(void) {
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	HydrogenFramebufferInfo* HydrogenFramebuffer::GetFramebufferInfo(void) {
		return &FramebufferInfo;
	}

}