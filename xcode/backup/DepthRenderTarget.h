#ifndef DEPTH_RENDER_TARGET_H
#define DEPTH_RENDER_TARGET_H

#include "Framework.h"

class DepthRenderTarget {
public:
    /**
     * Creates a new depth render target, for render to texture.  When this
     * target is bound using the bind() method, all OpenGL rendering is
     * directed into the texture.  The texture can be obtained by calling
     * the texture() function.
     */
    void init();
	void dispose();
    void bind();
    void unbind();

	static GLuint genColorTexutre();

private:
    GLuint frameBufferID_;
};

#endif

