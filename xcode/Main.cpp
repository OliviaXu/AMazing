#include "Framework.h"
#include "Shader.h"
#include "GameEngine.h"

// Note: See the SMFL documentation for info on setting up fullscreen mode
// and using rendering settings
// http://www.sfml-dev.org/tutorials/1.6/window-window.php
sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(800, 600), "CS248 Rules!", sf::Style::Close, settings);
 
// This is a clock you can use to control animation.  For more info, see:
// http://www.sfml-dev.org/tutorials/1.6/window-time.php
sf::Clock clck;

#define MAP_FILE "maps/map.txt"
#define CONFIG_FILE "config_file"	

GameEngine *gameEngine;


#include "assimp.h"
#include <vector>
#include <math.h>
using namespace std;
Assimp::Importer importer;
Shader* regShader;


void initOpenGL();
void initWorld();


int main(int argc, char** argv) {
    // Put your game loop here (i.e., render with OpenGL, update animation)
    initOpenGL();
    
    /*regShader = new Shader("shaders/phong");
    if(!regShader->loaded())
    {
        std::cerr << "Shader failed to load" << std::endl;
		std::cerr << regShader->errors() << std::endl;
		exit(-1);
    }
    const aiScene* scene;
    aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
    scene = importer.ReadFile("models/testBox.3DS",  
                                aiProcess_CalcTangentSpace |
                                aiProcess_Triangulate |
                                aiProcess_JoinIdenticalVertices |
                                aiProcessPreset_TargetRealtime_Quality |
                                aiProcess_FindDegenerates |
                                aiProcess_SortByPType);
    
    if (!scene || scene->mNumMeshes <= 0) {
        std::cerr << importer.GetErrorString() << std::endl;
        exit(-1);
    }*/
    
    
    initWorld();
    
    gameEngine->run();

    return 0;
}



void initOpenGL() {
    // Initialize GLEW on Windows, to make sure that OpenGL 2.0 is loaded
#ifdef FRAMEWORK_USE_GLEW
    GLint error = glewInit();
    if (GLEW_OK != error) {
        std::cerr << glewGetErrorString(error) << std::endl;
        exit(-1);
    }
    if (!GLEW_VERSION_2_0 || !GL_EXT_framebuffer_object) {
        std::cerr << "This program requires OpenGL 2.0 and FBOs" << std::endl;
        exit(-1);
    }
#endif

    // This initializes OpenGL with some common defaults.  More info here:
    // http://www.sfml-dev.org/tutorials/1.6/window-opengl.php
    glClearDepth(1.0f);
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
}

void initWorld()
{
    gameEngine = new GameEngine(MAP_FILE, CONFIG_FILE);
    gameEngine->init(&window);
}