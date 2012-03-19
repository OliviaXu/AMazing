#include "DepthRenderTarget.h"

//GLuint DepthRenderTarget::frameBufferID_ = 0;

void DepthRenderTarget::init() {
    // Generate a framebuffer
    glGenFramebuffersEXT(1, &frameBufferID_);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glDrawBuffer(GL_BACK);
	//glReadBuffer(GL_BACK);
}

void DepthRenderTarget::dispose(){
	glDeleteFramebuffersEXT(1, &frameBufferID_);
}

GLuint DepthRenderTarget::genColorTexutre(){
	return 0;
}

void DepthRenderTarget::bind() {
    GL_CHECK(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_));
}

void DepthRenderTarget::unbind() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    //glDrawBuffer(GL_BACK);
	//glReadBuffer(GL_BACK);
}
