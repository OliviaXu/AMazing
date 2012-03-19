#include "DepthRenderTarget.h"

DepthRenderTarget::DepthRenderTarget(unsigned int width, unsigned int height) {
    width_ = width;
    height_ = height;

    // Initialize the texture, including filtering options
	glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &depth_textureID_);
    glBindTexture(GL_TEXTURE_2D, depth_textureID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_DEPTH_COMPONENT, 
        width_, 
        height_, 
        0, 
        GL_DEPTH_COMPONENT, 
        GL_UNSIGNED_BYTE, 
        0);

    // Generate a framebuffer
    glGenFramebuffersEXT(1, &frameBufferID_);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_);

    // Attach the texture to the frame buffer
    glFramebufferTexture2DEXT(
        GL_FRAMEBUFFER_EXT,
        GL_DEPTH_ATTACHMENT_EXT,
        GL_TEXTURE_2D,
        depth_textureID_,
        0);

    // Check the status of the FBO
    //glDrawBuffer(GL_BACK);
	//glReadBuffer(GL_BACK);
    /*if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) {
        throw std::runtime_error("Invalid framebuffer configuration");
    }*/
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
}

DepthRenderTarget::~DepthRenderTarget(){
	glDeleteFramebuffersEXT(1, &frameBufferID_);
    glDeleteTextures(1, &depth_textureID_);
}

GLuint DepthRenderTarget::genColorTexutre(){
	GLuint color_textureID;
	//Generate color texture
	glGenTextures(1, &color_textureID);
    glBindTexture(GL_TEXTURE_2D, color_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RGB8, 
        width_, 
        height_, 
        0, 
        GL_RGB, 
        GL_UNSIGNED_BYTE, 
        0);

	return color_textureID;
}

GLuint DepthRenderTarget::depthTextureID() {
    return depth_textureID_;
}

void DepthRenderTarget::bind() {
    //glPushAttrib(GL_VIEWPORT_BIT);
    GL_CHECK(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID_));
    //glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
    //glViewport(0, 0, width_, height_);
}

void DepthRenderTarget::unbind() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
    //glPopAttrib();
}