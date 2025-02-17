#include "GameObject.h"
#include "Portal.h"

using namespace std;

GameObject::GameObject():dtex(0),stex(0){
	//dtex = NULL;
	//stex = NULL;
	velocity=Vec3(0.,0.,0.);
	hide = false;
	//transformation.mat[0] = -1;
}

GameObject::~GameObject(){
	delete phyinfo;
}

void GameObject::updatePhysicalProperty(Vec3 &newPos, 
	float newMass, float newVel, float newAcc){

}

void GameObject::setPortal(int iPortal){
	this->iPortal = iPortal;
}

void GameObject::setShader(const Shader *shader){
	this->shader = shader;
}

void GameObject::setMass(int mass){
	this->mass = mass;
}

void GameObject::setPos(struct Vec3 posIn){
	pos = posIn;
	
	transformation.mat[12] = pos.x;
	transformation.mat[13] = pos.y;
	transformation.mat[14] = pos.z;
}

struct Vec3* GameObject::getPos(){
	return &pos;
}

struct Vec3 &GameObject::getVelocity(){
	return velocity;
}

void GameObject::setModel(const aiScene *model, const vector<unsigned int> *indexBuff){
	this->model = model;
	this->indexBuff = indexBuff;
}
static sf::Image default_tex(1,1,sf::Color(255, 255, 255));
//void GameObject::setTexture(const sf::Image *dtex, const sf::Image *stex){
void GameObject::setTexture(const GLuint dtex, const GLuint stex){
	this->dtex = dtex;
	this->stex = stex;
	/*
	glGenTextures( 1, &diftexture );
	glBindTexture( GL_TEXTURE_2D,diftexture );
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//if(dtex){
		gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,dtex->GetWidth(),dtex->GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,dtex->GetPixelsPtr());
	//}else{
		
		//gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,default_tex.GetWidth(),default_tex.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,default_tex.GetPixelsPtr());
	//}

	glGenTextures( 1, &spectexture );
	glBindTexture( GL_TEXTURE_2D,spectexture );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//if(stex){
		gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,stex->GetWidth(),stex->GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,stex->GetPixelsPtr());
	//}else{
		//gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,default_tex.GetWidth(),default_tex.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,default_tex.GetPixelsPtr());
	//}
	*/
}

void GameObject::setMaterial(const aiMesh *mesh) {
	GLuint shaderID = shader->programID();
    aiMaterial* material = model->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;
    // Get a handle to the diffuse, specular, and ambient variables
    // inside the shader.  Then set them with the diffuse, specular, and
    // ambient color.
	GLuint diffuse = GL_CHECK(glGetUniformLocation(shaderID, "Kd"));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    GL_CHECK(glUniform3f(diffuse, color.r, color.g, color.b));

    // Specular material
    GLuint specular = GL_CHECK(glGetUniformLocation(shaderID, "Ks"));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    GL_CHECK(glUniform3f(specular, color.r, color.g, color.b));
  
    // Ambient material
    GLuint ambient = GL_CHECK(glGetUniformLocation(shaderID, "Ka"));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    GL_CHECK(glUniform3f(ambient, color.r, color.g, color.b));

    // Specular power
    GLuint shininess = GL_CHECK(glGetUniformLocation(shaderID, "alpha"));
    float value = 4;
    GL_CHECK(glUniform1f(shininess, value));
}


void GameObject::bindTexture(){
	GLuint shaderID = shader->programID();
	GLuint diffuseMap = GL_CHECK(glGetUniformLocation(shaderID, "diffuseMap"));
	GL_CHECK(glActiveTexture(GL_TEXTURE0));
	glBindTexture( GL_TEXTURE_2D,dtex );
	GL_CHECK(glUniform1i(diffuseMap, 0)); // The diffuse map will be GL_TEXTURE0

	GLuint specularMap = GL_CHECK(glGetUniformLocation(shaderID, "specularMap"));
	GL_CHECK(glActiveTexture(GL_TEXTURE1));
	glBindTexture( GL_TEXTURE_2D,stex );
	GL_CHECK(glUniform1i(specularMap, 1));
}

void GameObject::setMeshData(const aiMesh *mesh) {
	GLuint shaderID = shader->programID();
    // Get a handle to the variables for the vertex data inside the shader.
    GLuint position = GL_CHECK(glGetAttribLocation(shaderID, "positionIn"));
    GL_CHECK(glEnableVertexAttribArray(position)); 
    GL_CHECK(glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mVertices));
	
    // Texture coords.  Note the [0] at the end, very important
    GLuint texcoord = GL_CHECK(glGetAttribLocation(shaderID, "texcoordIn"));
    GL_CHECK(glEnableVertexAttribArray(texcoord));
    GL_CHECK(glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTextureCoords[0]));

    // Normals
    GLuint normal = GL_CHECK(glGetAttribLocation(shaderID, "normalIn"));
    GL_CHECK(glEnableVertexAttribArray(normal));
    GL_CHECK(glVertexAttribPointer(normal, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mNormals));
}

//This shall be a abstract method later on
void GameObject::passShaderParam(const aiMesh *mesh){
	setMaterial(mesh);
	bindTexture();
	setMeshData(mesh);
}

void GameObject::draw(const std::vector<Portal *> *portals){
	GLuint shaderID = shader->programID();
	GL_CHECK(glUseProgram(shaderID));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//Translate to the position as specified in map file
	glMultMatrixf(transformation.mat);

	aiNode *root = model->mRootNode;
	aiNode *child = root->mChildren[0];
	/*aiMatrix4x4 &transformMat = 
		root->mTransformation;// * child->mTransformation);
	GLfloat mat1[] = {
		transformMat.a1, transformMat.b1, transformMat.c1, transformMat.d1,
		transformMat.a2, transformMat.b2, transformMat.c2, transformMat.d2,
		transformMat.a3, transformMat.b3, transformMat.c3, transformMat.d3,
		transformMat.a4, transformMat.b4, transformMat.c4, transformMat.d4
	};
	aiMatrix4x4 &transformMat2 = 
		child->mTransformation;
	GLfloat mat2[] = {
		transformMat2.a1, transformMat2.b1, transformMat2.c1, transformMat2.d1,
		transformMat2.a2, transformMat2.b2, transformMat2.c2, transformMat2.d2,
		transformMat2.a3, transformMat2.b3, transformMat2.c3, transformMat2.d3,
		transformMat2.a4, transformMat2.b4, transformMat2.c4, transformMat2.d4
	};
	glMultMatrixf(mat1);
	glMultMatrixf(mat2);*/

	//aiMatrix4x4 &atransformMat = root->mTransformation;
	//aiMatrix4x4 &btransformMat = child->mTransformation;

	aiMatrix4x4 transformMat =	root->mTransformation * child->mTransformation;
	GLfloat mat[] = {
		transformMat.a1, transformMat.b1, transformMat.c1, transformMat.d1,
		transformMat.a2, transformMat.b2, transformMat.c2, transformMat.d2,
		transformMat.a3, transformMat.b3, transformMat.c3, transformMat.d3,
		transformMat.a4, transformMat.b4, transformMat.c4, transformMat.d4
	};
	glMultMatrixf(mat);

	aiMesh *mesh = model->mMeshes[child->mMeshes[child->mNumMeshes-1]];
	passShaderParam(mesh);
	GL_CHECK(glDrawElements(GL_TRIANGLES, 3*mesh->mNumFaces, 
					GL_UNSIGNED_INT, &(*indexBuff)[0]));
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

int GameObject::getPortal(){
	return iPortal;
}

void GameObject::setHide(bool flag){
	hide = flag;
}

bool GameObject::isHidden(){
	return hide;
}

void GameObject::setTrans(float* mat)
{
    for(int i = 0;i < 4;++i)
    {
        for(int j = 0;j < 4;++j)
        {
            if(i != 3 || j != 3)
            {
                //transformation.mat[4*i+j] = mat[4*j+i];
                transformation.mat[4*i+j] = mat[4*i+j];
            }
        }
    }
    transformation.mat[15] = 1;
}

void GameObject::setPos(float x, float y, float z)
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

void GameObject::respondToCollision(GameObject *obj, std::queue<MAZEevent> *eq){
	return;
}

void GameObject::setRigidBody(btRigidBody *rb){
	rigidBody = rb;
}

void GameObject::moveTo(float x, float y, float z){
	float deltaX = x-pos.x;
	float deltaY = y-pos.y;
	float deltaZ = z-pos.z;

	rigidBody->translate(btVector3(deltaX, deltaY, deltaZ));
	//rigidBody->translate(btVector3(x, y, z));
	//btTransform trans;
	//btScalar m[16];
	//rigidBody->getMotionState()->getWorldTransform(trans);
	//trans.getOpenGLMatrix(m);
	//setTrans(m);

	//btVector3 o = trans.getOrigin();
	//setPos(o.getX(), o.getY(), o.getZ());
	//cout << x << "," << y << "," << z << "----" << pos.x <<"," << pos.y << "," << pos.z << endl;
}