#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// GLEW must be included first, if we need it.
#ifdef _WIN32
#define GLEW_STATIC
#define FRAMEWORK_USE_GLEW
#include <GL/glew.h>
#endif
#ifdef __linux__
#define FRAMEWORK_USE_GLEW
#include <GL/glew.h>
#endif

// SFML automatically includes SDL headers
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Open Asset Import Library
#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>

#include <memory>
#include <iostream>
#include <fstream>

#include <vector>
#include <queue>

#include <string>
#include <assert.h>
#include <cmath>
#include <map>
#include <climits>
#include <set>

#include <time.h>

#include "Util.h"

#define DEBUG_OUTPUT 0

#define GL_CHECK(x)		(x);\
						{GLenum __MY_GL_ERROR = glGetError();\
						if (GL_NO_ERROR != __MY_GL_ERROR) {\
							printf("%s", gluErrorString(__MY_GL_ERROR));\
						}}

#pragma unmanaged

#endif
