#include "GameObject.h"
#include "Portal.h"

using namespace std;

GameObject::GameObject(){
	dtex = NULL;
	stex = NULL;
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

struct Vec3 GameObject::getPos(){
	return pos;
}

struct Vec3 &GameObject::getVelocity(){
	return velocity;
}

void GameObject::setModel(const aiScene *model, const vector<unsigned int> *indexBuff){
	this->model = model;
	this->indexBuff = indexBuff;
}

void GameObject::setTexture(const sf::Image *dtex, const sf::Image *stex){
	this->dtex = dtex;
	this->stex = stex;
}

void GameObject::setMaterial(const aiMesh *mesh) {
	GLuint shaderID = shader->programID();
    aiMaterial* material = model->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;
    // Get a handle to the diffuse, specular, and ambient variables
    // inside the shader.  Then set them with the diffuse, specular, and
    // ambient color.
	GLint diffuse = GL_CHECK(glGetUniformLocation(shaderID, "Kd"));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    GL_CHECK(glUniform3f(diffuse, color.r, color.g, color.b));

    // Specular material
    GLint specular = GL_CHECK(glGetUniformLocation(shaderID, "Ks"));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    GL_CHECK(glUniform3f(specular, color.r, color.g, color.b));
  
    // Ambient material
    GLint ambient = GL_CHECK(glGetUniformLocation(shaderID, "Ka"));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    GL_CHECK(glUniform3f(ambient, color.r, color.g, color.b));

    // Specular power
    GLint shininess = GL_CHECK(glGetUniformLocation(shaderID, "alpha"));
    float value = 40;
    GL_CHECK(glUniform1f(shininess, value));
}

static sf::Image default_tex(1,1,sf::Color(255, 255, 255));
void GameObject::bindTexture(){
	GLuint shaderID = shader->programID();
	GLint diffuseMap = GL_CHECK(glGetUniformLocation(shaderID, "diffuseMap"));
	GL_CHECK(glActiveTexture(GL_TEXTURE0));
	if(dtex){
		dtex->Bind();
	}
	else{
		default_tex.Bind();
	}
	GL_CHECK(glUniform1i(diffuseMap, 0)); // The diffuse map will be GL_TEXTURE0

	GLint specularMap = GL_CHECK(glGetUniformLocation(shaderID, "specularMap"));
	GL_CHECK(glActiveTexture(GL_TEXTURE1));
	if(stex)
		stex->Bind();
	else
		default_tex.Bind();
	GL_CHECK(glUniform1i(specularMap, 1));	
}

void GameObject::setMeshData(const aiMesh *mesh) {
	GLuint shaderID = shader->programID();
    // Get a handle to the variables for the vertex data inside the shader.
    GLint position = GL_CHECK(glGetAttribLocation(shaderID, "positionIn"));
    GL_CHECK(glEnableVertexAttribArray(position)); 
    GL_CHECK(glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mVertices));
	
    // Texture coords.  Note the [0] at the end, very important
    GLint texcoord = GL_CHECK(glGetAttribLocation(shaderID, "texcoordIn"));
    GL_CHECK(glEnableVertexAttribArray(texcoord));
    GL_CHECK(glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTextureCoords[0]));

    // Normals
    GLint normal = GL_CHECK(glGetAttribLocation(shaderID, "normalIn"));
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

	aiMesh *mesh = model->mMeshes[child->mMeshes[0]];
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
                transformation.mat[4*i+j] = mat[4*j+i];
            }
        }
    }
    transformation.mat[15] = 1;
}