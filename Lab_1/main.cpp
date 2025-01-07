// Project includes
#include <random>
//GLEW & FreeGlut
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "maths_funcs.h"
#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include <vector> // STL dynamic memory.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/trolley.obj"
#define DEER_MESH_NAME "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/deer_mesh.obj"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

#pragma region SimpleTypes
typedef struct ModelData
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
};
#pragma endregion SimpleTypes

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
int gameSpeed = 200;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 25.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//camera defaults
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
//keyboard & mouse
bool keyStates[256]; // Array to hold the state of each key
bool firstMouse = true;
int mouseX, mouseY;
int currentMouseX, currentMouseY;


//SHADERS
unsigned int shaderID, deershaderID;
const char* vertexPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/vertex.txt";
const char* fragmentPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/fragment.txt";

unsigned int lsourceShader;
const char* lsourceVPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/lightsource_vertex.txt";
const char* lsourceFPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/lightsource_fragment.txt";

unsigned int scolorShader;
const char* scolorVPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/solidcolor_vertex.txt";
const char* scolorFPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/solidcolor_fragment.txt";

unsigned int texturedShader, floorShader, campShader, grassShader;
const char* texturedVPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/textured_vertex.txt";
const char* texturedFPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/textured_fragment.txt";


unsigned int skyboxShader;
const char* skyboxVPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/skybox_vertex.txt";
const char* skyboxFPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/skybox_fragment.txt";

//TEXTURES
unsigned int woodboxTexture, smileyTexture;


unsigned int woodDiffuse;
const char* woodDiffusePath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/container2.png";
unsigned int woodSpecular;
const char* woodSpecularPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/container2_specular.png";

unsigned int floorDiffuse;
const char* floorDiffusePath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/stoneFloor.jpg";
unsigned int floorSpecular;
const char* floorSpecularPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/stoneFloorSpecular.png";

unsigned int grassDiffuse;
const char* grassDiffusePath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/grassDiffuse.jpg";
unsigned int grassSpecular;
const char* grassSpecularPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/grassSpec.jpg";

unsigned int campDiffuse;
const char* campDiffusePath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/old_brick_wall_stones_Diffuse.jpg";
unsigned int campSpecular;
const char* campSpecularPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/old_brick_wall_stones_Specular.jpg";

unsigned int fleshDiffuse;
const char* fleshDiffusePath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/others_0001_color_1k.jpg";
unsigned int fleshDiffuse2;
const char* fleshDiffusePath2 = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/others_0001_color_1k - Copy.jpg";
unsigned int fleshSpecular;
const char* fleshSpecularPath = "C:/Users/Rober/College/4thYrSem1/CG/Labs/Lab_1/Lab_1/fleshSpecular.jpg";

unsigned int VBO, VAO, EBO;
unsigned int cubeVAO, lightSourceVAO, campVAO;
unsigned int floorVAO, floorVBO;
unsigned int grassVAO, grassVBO, grassEBO;
unsigned int model_vp_VBO, model_vn_VBO, modelVAO;
unsigned int deermodel_vp_VBO, deermodel_vn_VBO, deermodelVAO;

float trolleyValuesA[] = { 127.89, -58.37, 104.92, -33.21, 175.46, 89.73, -12.84 };
float trolleyValuesB[] = {45.5, 87.2, 12.3, -150.0, 33.7, -102.8, 76.1};

float deerValuesA[] = { 127.89, -58.37, 104.92, -33.21, 175.46, 89.73, 12.84, -127.89, 58.37, -104.92, 33.21, -175.46, -89.73, -12.84, 45.5, 87.2, 12.3, -150.0, 33.7, -102.8, 76.1 };
float deerValuesB[] = { 45.5, 87.2, 12.3, -150.0, 33.7, -102.8, 76.1, 127.89, -58.37, 104.92, -33.21, 175.46, 89.73, 12.84, -127.89, 58.37, -104.92, 33.21, -175.46, -89.73, -12.84 };

glm::vec3 cubePos(-3.50f, -0.50f, 0.0f);

glm::vec3 campPos(2.0f, 0.5f, 3.0f);
glm::vec3 campPos2(-2.0f,-0.50f, 3.0f);
glm::vec3 campPos3(-2.0f, 0.0f, 7.0f);
glm::vec3 campPos4(2.0f, 0.2f, 7.0f);

glm::vec3 campPosl(2.0f,   -1.5f, 3.0f);
glm::vec3 campPos2l(-2.0f, -1.5f, 3.0f);
glm::vec3 campPos3l(-2.0f, -1.5f, 7.0f);
glm::vec3 campPos4l(2.0f,  -1.5f, 7.0f);

//model imports
ModelData mesh_data;
unsigned int mesh_vao = 0;
GLuint loc1, loc2, loc3;
GLfloat rotate_y = 0.0f;

//model imports
ModelData deermesh_data;
unsigned int deermesh_vao = 0;
GLuint deerloc1, deerloc2, deerloc3;

float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float floorVertices[] = {
	// positions          // normals          // texture coords
	 100.0f,  0.0f,  100.0f,   0.0f,  1.0f, 0.0f, 100.0f, 100.0f, // top right
	 100.0f,  0.0f, -100.0f,   0.0f,  1.0f, 0.0f, 100.0f, 0.0f, // bottom right
	-100.0f,  0.0f, -100.0f,   0.0f,  1.0f, 0.0f, 0.0f, 0.0f, // bottom left
	-100.0f,  0.0f,  100.0f,   0.0f,  1.0f, 0.0f, 0.0f, 100.0f  // top left 
};

float grassVertices[] = {
	// positions          // normals          // texture coords
	 100.0f,  0.0f,  100.0f,   0.0f,  1.0f, 0.0f, 50.0f, 50.0f, // top right
	 100.0f,  0.0f, -100.0f,   0.0f,  1.0f, 0.0f, 50.0f, 0.0f, // bottom right
	-100.0f,  0.0f, -100.0f,   0.0f,  1.0f, 0.0f, 0.0f, 0.0f, // bottom left
	-100.0f,  0.0f,  100.0f,   0.0f,  1.0f, 0.0f, 0.0f, 50.0f  // top left 
};


unsigned int floorIndices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3,    // second triangle
};
glm::vec3 floorPos(4.2f, -1.0f, 7.0f);
glm::vec3 grassPos(4.2f, -.990f, 22.80f);

// world space positions of our cubes
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

float ambientChannels[] = { 0.8f, 0.7f, 0.5f };

glm::vec3 ambientColor(ambientChannels[0], ambientChannels[1], ambientChannels[2]);

glm::vec3 sunlightPos(40.0f, 40.0f, 70.0f);
glm::vec3 sunlightAmbient(ambientChannels[0] / 2, ambientChannels[1] / 2, ambientChannels[2] / 2);
glm::vec3 sunlightDiffuse(0.8f, 0.8f, 0.8f);
glm::vec3 sunlightSpecular(1.0f, 1.0f, 1.0f);

glm::vec3 spotLightPos1(-5.0f, 2.0f, 0.0f);
glm::vec3 spotLightDirection1(0.0f, -2.0f, 0.0f);

glm::vec3 spotLightPos2(5.0f, 1.0f, 0.0f);
glm::vec3 spotLightDirection2(0.0f, -1.0f, 0.0f);

glm::vec3 spotLightAmbient(ambientChannels[0] / 2, ambientChannels[1] / 2, ambientChannels[2] / 2);
glm::vec3 spotLightDiffuse(4.0f, 2.0f, 1.0f);
glm::vec3 spotLightSpecular(1.0f, 1.0f, 1.0f);
float lightCutOff = glm::cos(glm::radians(20.5f));
float lightCutOffOuter = glm::cos(glm::radians(35.5f));

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData load_mesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
	return modelData;
}

#pragma endregion MESH LOADING

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}

#pragma region VBO_FUNCTIONS
void generateObjectBufferMesh() {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	mesh_data = load_mesh(MESH_NAME);
	unsigned int vp_vbo = 0;
	loc1 = glGetAttribLocation(shaderID, "vertex_position");
	loc2 = glGetAttribLocation(shaderID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderID, "vertex_texture");

	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
	unsigned int vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	unsigned int vt_vbo = 0;
	//	glGenBuffers (1, &vt_vbo);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glBufferData (GL_ARRAY_BUFFER, monkey_head_data.mTextureCoords * sizeof (vec2), &monkey_head_data.mTextureCoords[0], GL_STATIC_DRAW);

	unsigned int vao = 0;
	glBindVertexArray(vao);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	glEnableVertexAttribArray (loc3);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}
#pragma endregion VBO_FUNCTIONS

//------------------------------------------------------//

//Shader Functions//
void use(unsigned int ID)
{
	glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void setBool(unsigned int ID, const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void setInt(unsigned int ID, const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void setFloat(unsigned int ID, const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void setVec2(unsigned int ID, const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void setVec2(unsigned int ID, const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void setVec3(unsigned int ID, const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void setVec3(unsigned int ID, const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void setVec4(unsigned int ID, const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void setVec4(unsigned int ID, const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void setMat2(unsigned int ID, const std::string& name, const glm::mat2& mat)
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void setMat3(unsigned int ID, const std::string& name, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void setMat4(unsigned int ID, const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}




//Input Functions
void keyboardDown(unsigned char key, int x, int y) {
	keyStates[key] = true;
}
void keyboardUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}
void mouseMotion(int x, int y) {
	currentMouseX = x;
	currentMouseY = y;
}
void window_reshape_callback(int width, int height) {
	cout << "......USER INPUT: Resizing Window......";
	glViewport(0, 0, width, height);
}


//Camera Controls
void cameraMouseControl() {

	if (firstMouse)
	{
		mouseX = currentMouseX;
		mouseY = currentMouseY;
		firstMouse = false;
	}

	// Calculate the change in mouse position
	int deltaX = currentMouseX - mouseX;
	int deltaY = currentMouseY - mouseY;
	mouseX = currentMouseX;
	mouseY = currentMouseY;

	float sensitivity = 0.5f; // change this value to your liking
	deltaX *= sensitivity;
	deltaY *= sensitivity;

	yaw += deltaX;
	pitch += deltaY;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void cameraViewKeyControl() {
	float deltaX = 0;
	float deltaY = 0;
	float sensitivity = 0.2f; // change this value to your liking
	if (keyStates['i'])
		deltaY += sensitivity;
	if (keyStates['k'])
		deltaY -= sensitivity;
	if (keyStates['j'])
		deltaX -= sensitivity;
	if (keyStates['l'])
		deltaX += sensitivity;
	yaw += deltaX;
	pitch += deltaY;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void cameraPosKeyControl() {
	float cameraSpeed = (static_cast<float>(deltaTime));
	if (keyStates['w']) //forward
		cameraPos += cameraSpeed * cameraFront;
	if (keyStates['s']) //back
		cameraPos -= cameraSpeed * cameraFront;
	if (keyStates['a']) //left
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyStates['d']) //right
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyStates['q']) //up
		cameraPos += cameraSpeed * cameraUp;
	if (keyStates['e']) //up
		cameraPos -= cameraSpeed * cameraUp;
}


//Update & Display
void update() {
	// Get the elapsed time in milliseconds
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME);

	cameraPosKeyControl();
	cameraViewKeyControl();
	cameraMouseControl();

	// Convert milliseconds to seconds if needed
	float elapsedSeconds = static_cast<float>(elapsedTime) / 1000.0f;

	// Schedule the display function to be called again
	glutPostRedisplay();
}
void display() {
	glEnable(GL_DEPTH_TEST);
	float currentFrame = (float)glutGet(GLUT_ELAPSED_TIME) / gameSpeed;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glClearColor(ambientChannels[0], ambientChannels[1], ambientChannels[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


	// be sure to activate shader when setting uniforms/drawing objects
	glUseProgram(texturedShader);
	setVec3(texturedShader, "viewPos", cameraPos);
	setVec3(texturedShader, "ambientColor", ambientColor);
	setFloat(texturedShader, "material.shininess", 62.0f);
	setVec3(texturedShader, "pointlight.position", sunlightPos);
	setVec3(texturedShader, "pointlight.ambient", sunlightAmbient);
	setVec3(texturedShader, "pointlight.diffuse", sunlightDiffuse);
	setVec3(texturedShader, "pointlight.specular", sunlightSpecular);


	setVec3(texturedShader, "spotLights[0].position", spotLightPos1);
	setVec3(texturedShader, "spotLights[0].direction", spotLightDirection1);

	setVec3(texturedShader, "spotLights[0].ambient", spotLightAmbient);
	setVec3(texturedShader, "spotLights[0].diffuse", spotLightDiffuse);
	setVec3(texturedShader, "spotLights[0].specular", spotLightSpecular);
	setFloat(texturedShader, "spotLights[0].cutOff", lightCutOff);
	setFloat(texturedShader, "spotLights[0].outerCutOff", lightCutOffOuter);
	setFloat(texturedShader, "spotLights[0].constant", 1.0f);
	setFloat(texturedShader, "spotLights[0].linear", 0.09f);
	setFloat(texturedShader, "spotLights[0].quadratic", 0.032f);

	setVec3(texturedShader, "spotLights[1].position", spotLightPos2);
	setVec3(texturedShader, "spotLights[1].direction", spotLightDirection2);

	setVec3(texturedShader, "spotLights[1].ambient", spotLightAmbient);
	setVec3(texturedShader, "spotLights[1].diffuse", spotLightDiffuse);
	setVec3(texturedShader, "spotLights[1].specular", spotLightSpecular);
	setFloat(texturedShader, "spotLights[1].cutOff", lightCutOff);
	setFloat(texturedShader, "spotLights[1].outerCutOff", lightCutOffOuter);
	setFloat(texturedShader, "spotLights[1].constant", 1.0f);
	setFloat(texturedShader, "spotLights[1].linear", 0.09f);

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.10f, 1000.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	setMat4(texturedShader, "projection", projection);
	setMat4(texturedShader, "view", view);

	// world transformation
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cubePos);
	setMat4(texturedShader, "model", model);
	

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodDiffuse);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, woodSpecular);

	// render the cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//////////////////////////////////////////////////////////////////
	//CAMPANILE LEGS
	glUseProgram(campShader);
	setVec3(campShader, "viewPos", cameraPos);
	setVec3(campShader, "ambientColor", ambientColor);
	setFloat(campShader, "material.shininess", 62.0f);
	setVec3(campShader, "pointlight.position", sunlightPos);
	setVec3(campShader, "pointlight.ambient", sunlightAmbient);
	setVec3(campShader, "pointlight.diffuse", sunlightDiffuse);
	setVec3(campShader, "pointlight.specular", sunlightSpecular);

	setVec3(campShader, "spotLights[0].position", spotLightPos1);
	setVec3(campShader, "spotLights[0].direction", spotLightDirection1);

	setVec3(campShader, "spotLights[0].ambient", spotLightAmbient);
	setVec3(campShader, "spotLights[0].diffuse", spotLightDiffuse);
	setVec3(campShader, "spotLights[0].specular", spotLightSpecular);
	setFloat(campShader, "spotLights[0].cutOff", lightCutOff);
	setFloat(campShader, "spotLights[0].outerCutOff", lightCutOffOuter);
	setFloat(campShader, "spotLights[0].constant", 1.0f);
	setFloat(campShader, "spotLights[0].linear", 0.09f);
	setFloat(campShader, "spotLights[0].quadratic", 0.032f);

	setVec3(campShader, "spotLights[1].position", spotLightPos2);
	setVec3(campShader, "spotLights[1].direction", spotLightDirection2);

	setVec3(campShader, "spotLights[1].ambient", spotLightAmbient);
	setVec3(campShader, "spotLights[1].diffuse", spotLightDiffuse);
	setVec3(campShader, "spotLights[1].specular", spotLightSpecular);
	setFloat(campShader, "spotLights[1].cutOff", lightCutOff);
	setFloat(campShader, "spotLights[1].outerCutOff", lightCutOffOuter);
	setFloat(campShader, "spotLights[1].constant", 1.0f);
	setFloat(campShader, "spotLights[1].linear", 0.09f);

	setMat4(campShader, "projection", projection);
	setMat4(campShader, "view", view);

	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, campPos);
	model = glm::scale(model, glm::vec3(1.20f, 3.0f, 1.20f)); // a smaller cube
	setMat4(campShader, "model", model);


	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, campDiffuse);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, campSpecular);

	// render the cube
	glBindVertexArray(campVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, campPos2);
	model = glm::scale(model, glm::vec3(1.20f, 3.0f, 1.20f)); // a smaller cube
	setMat4(campShader, "model", model);
	// render the cube
	glBindVertexArray(campVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, campPos3);
	model = glm::scale(model, glm::vec3(1.20f, 3.0f, 1.20f)); // a smaller cube
	setMat4(campShader, "model", model);
	// render the cube
	glBindVertexArray(campVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, campPos4);
	model = glm::scale(model, glm::vec3(1.20f, 3.0f, 1.20f)); // a smaller cube
	setMat4(campShader, "model", model);
	// render the cube
	glBindVertexArray(campVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, campPosl);
	model = glm::scale(model, glm::vec3(1.40f, 3.0f, 1.40f)); // a smaller cube
	setMat4(campShader, "model", model);
	// render the cube
	glBindVertexArray(campVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, campPos2l);
	model = glm::scale(model, glm::vec3(1.40f, 3.0f, 1.40f)); // a smaller cube
	setMat4(campShader, "model", model);
	// render the cube
	glBindVertexArray(campVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, campPos3l);
	model = glm::scale(model, glm::vec3(1.40f, 3.0f, 1.40f)); // a smaller cube
	setMat4(campShader, "model", model);
	// render the cube
	glBindVertexArray(campVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, campPos4l);
	model = glm::scale(model, glm::vec3(1.40f, 3.0f, 1.40f)); // a smaller cube
	setMat4(campShader, "model", model);
	// render the cube
	glBindVertexArray(campVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//////////////////////////////////////////////////////////////////

	////////////////

	//MODEL
	// Switch back to the main shader for other objects
	glUseProgram(shaderID);

	setVec3(shaderID, "viewPos", cameraPos);
	setVec3(shaderID, "ambientColor", ambientColor);

	setVec3(shaderID, "material.ambient", 0.329412f, 0.1f, 0.027451f);
	setVec3(shaderID, "material.diffuse", 0.2f, 0.05f, 0.05f);
	setVec3(shaderID, "material.specular", 0.992157f, 0.941176f, 0.807843f);
	setFloat(shaderID, "material.shininess", 5.0f);

	setVec3(shaderID, "light.position", sunlightPos);
	setVec3(shaderID, "light.ambient", sunlightAmbient);
	setVec3(shaderID, "light.diffuse", sunlightDiffuse);
	setVec3(shaderID, "light.specular", sunlightSpecular);


	setVec3(shaderID, "pointlight.position", sunlightPos);
	setVec3(shaderID, "pointlight.ambient", sunlightAmbient);
	setVec3(shaderID, "pointlight.diffuse", sunlightDiffuse);
	setVec3(shaderID, "pointlight.specular", sunlightSpecular);
	
	
	setVec3(shaderID, "spotLights[0].position", spotLightPos1);
	setVec3(shaderID, "spotLights[0].direction", spotLightDirection1);
	
	setVec3(shaderID, "spotLights[0].ambient", spotLightAmbient);
	setVec3(shaderID, "spotLights[0].diffuse", spotLightDiffuse);
	setVec3(shaderID, "spotLights[0].specular", spotLightSpecular);
	setFloat(shaderID, "spotLights[0].cutOff", lightCutOff);
	setFloat(shaderID, "spotLights[0].outerCutOff", lightCutOffOuter);
	setFloat(shaderID, "spotLights[0].constant", 1.0f);
	setFloat(shaderID, "spotLights[0].linear", 0.09f);
	setFloat(shaderID, "spotLights[0].quadratic", 0.032f);
	
	setVec3(shaderID, "spotLights[1].position", spotLightPos2);
	setVec3(shaderID, "spotLights[1].direction", spotLightDirection2);
	
	setVec3(shaderID, "spotLights[1].ambient", spotLightAmbient);
	setVec3(shaderID, "spotLights[1].diffuse", spotLightDiffuse);
	setVec3(shaderID, "spotLights[1].specular", spotLightSpecular);
	setFloat(shaderID, "spotLights[1].cutOff", lightCutOff);
	setFloat(shaderID, "spotLights[1].outerCutOff", lightCutOffOuter);
	setFloat(shaderID, "spotLights[1].constant", 1.0f);
	setFloat(shaderID, "spotLights[1].linear", 0.09f);




	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderID, "model");
	int view_mat_location = glGetUniformLocation(shaderID, "view");
	int proj_mat_location = glGetUniformLocation(shaderID, "proj");


	// Root of the Hierarchy
	mat4 persp_proj = perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	mat4 model1 = identity_mat4();
	model1 = rotate_z_deg(model1, rotate_y);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glBindVertexArray(modelVAO);
	// bind diffuse map

	for (int i = 0; i < 7; i++) {

		// Set up the child matrix
		vec3 trolleyPos = vec3(1.0f + trolleyValuesA[i] / 5, -4.90f, -10.0f - trolleyValuesB[i] / 5);
		
		mat4 modelChild = identity_mat4();
		modelChild = rotate_z_deg(modelChild, 30);
		modelChild = rotate_y_deg(modelChild, -7);
		modelChild = rotate_x_deg(modelChild, 95);
		modelChild = rotate_y_deg(modelChild, trolleyValuesA[i]);
		modelChild = translate(modelChild, trolleyPos);
		modelChild = scale(modelChild, vec3(0.2f, 0.2f, 0.2f));
		// Apply the root matrix to the child matrix
		modelChild = model1 * modelChild;
		
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);
	}

	//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	int projectionLoc = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	int viewLoc = glGetUniformLocation(shaderID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//DEER MODEL
	// Switch back to the main shader for other objects
	glUseProgram(deershaderID);
	
	
	setVec3(deershaderID, "ambientColor", ambientColor);
	
	setVec3(deershaderID, "material.ambient", 0.329412f, 0.1f, 0.027451f);
	setVec3(deershaderID, "material.diffuse", 0.4f, 0.4f, 0.1f);
	setVec3(deershaderID, "material.specular", 0.992157f, 0.941176f, 0.807843f);
	setFloat(deershaderID, "material.shininess", 256.0f);
	
	setVec3(deershaderID, "light.position", sunlightPos);
	setVec3(deershaderID, "light.ambient", sunlightAmbient);
	setVec3(deershaderID, "light.diffuse", sunlightDiffuse);
	setVec3(deershaderID, "light.specular", sunlightSpecular);
	
	
	setVec3(deershaderID, "pointlight.position", sunlightPos);
	setVec3(deershaderID, "pointlight.ambient", sunlightAmbient);
	setVec3(deershaderID, "pointlight.diffuse", sunlightDiffuse);
	setVec3(deershaderID, "pointlight.specular", sunlightSpecular);
	
	
	setVec3(deershaderID, "spotLights[0].position", spotLightPos1);
	setVec3(deershaderID, "spotLights[0].direction", spotLightDirection1);
	
	setVec3(deershaderID, "spotLights[0].ambient", spotLightAmbient);
	setVec3(deershaderID, "spotLights[0].diffuse", spotLightDiffuse);
	setVec3(deershaderID, "spotLights[0].specular", spotLightSpecular);
	setFloat(deershaderID, "spotLights[0].cutOff", lightCutOff);
	setFloat(deershaderID, "spotLights[0].outerCutOff", lightCutOffOuter);
	setFloat(deershaderID, "spotLights[0].constant", 1.0f);
	setFloat(deershaderID, "spotLights[0].linear", 0.09f);
	setFloat(deershaderID, "spotLights[0].quadratic", 0.032f);
	
	setVec3(deershaderID, "spotLights[1].position", spotLightPos2);
	setVec3(deershaderID, "spotLights[1].direction", spotLightDirection2);
	
	setVec3(deershaderID, "spotLights[1].ambient", spotLightAmbient);
	setVec3(deershaderID, "spotLights[1].diffuse", spotLightDiffuse);
	setVec3(deershaderID, "spotLights[1].specular", spotLightSpecular);
	setFloat(deershaderID, "spotLights[1].cutOff", lightCutOff);
	setFloat(deershaderID, "spotLights[1].outerCutOff", lightCutOffOuter);
	setFloat(deershaderID, "spotLights[1].constant", 1.0f);
	setFloat(deershaderID, "spotLights[1].linear", 0.09f);
	
	
	
	
	//Declare your uniform variables that will be used in your shader
	matrix_location = glGetUniformLocation(deershaderID, "model");
	view_mat_location = glGetUniformLocation(deershaderID, "view");
	proj_mat_location = glGetUniformLocation(deershaderID, "proj");
	
	
	// Root of the Hierarchy
	persp_proj = perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	model1 = identity_mat4();
	model1 = rotate_z_deg(model1, rotate_y);
	
	// update uniforms & draw
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fleshDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fleshSpecular);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fleshDiffuse2);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glBindVertexArray(deermodelVAO);
	
	
	for (int i = 0; i < 20; i++) {
		// Set up the child matrix
		vec3 deerPos = vec3(10.0f + (deerValuesA[i] / 15) + abs(cos(deerValuesA[i]) * sin((float)glutGet(GLUT_ELAPSED_TIME) / 10000) * 10), -0.93f, -20.0f - (deerValuesB[i] / 15) + abs(sin(deerValuesB[i]) * sin((float)glutGet(GLUT_ELAPSED_TIME) / 10000) * 10));
		setVec3(deershaderID, "viewPos", cameraPos - glm::vec3(10.0f + (deerValuesA[i] / 15) + abs(cos(deerValuesA[i]) * sin((float)glutGet(GLUT_ELAPSED_TIME) / 10000) * 10), -0.93f, -20.0f - (deerValuesB[i] / 15) + abs(sin(deerValuesB[i]) * sin((float)glutGet(GLUT_ELAPSED_TIME) / 10000) * 10)));
		mat4 modelChild = identity_mat4();
		modelChild = rotate_y_deg(modelChild, deerValuesA[i]/5);
		modelChild = translate(modelChild, deerPos);
		modelChild = scale(modelChild, vec3(1.1f, 1.1f, 1.1f));
	
		// Apply the root matrix to the child matrix
		modelChild = model1 * modelChild;
		// Update the appropriate uniform and draw the mesh again
	
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild.m);
		glDrawArrays(GL_TRIANGLES, 0, deermesh_data.mPointCount);
	}
	
	//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projectionLoc = glGetUniformLocation(deershaderID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	viewLoc = glGetUniformLocation(deershaderID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	/////////////////////////////////////////////////////////////////

	//////DRAW FLOOR
	glUseProgram(floorShader);
	setVec3(floorShader, "viewPos", cameraPos);
	setVec3(floorShader, "ambientColor", ambientColor);
	setFloat(floorShader, "material.shininess", 62.0f);

	setVec3(floorShader, "pointlight.position", sunlightPos);
	setVec3(floorShader, "pointlight.ambient", sunlightAmbient);
	setVec3(floorShader, "pointlight.diffuse", sunlightDiffuse);
	setVec3(floorShader, "pointlight.specular", sunlightSpecular);


	setVec3(floorShader, "spotLights[0].position", spotLightPos1);
	setVec3(floorShader, "spotLights[0].direction", spotLightDirection1);

	setVec3(floorShader, "spotLights[0].ambient", spotLightAmbient);
	setVec3(floorShader, "spotLights[0].diffuse", spotLightDiffuse);
	setVec3(floorShader, "spotLights[0].specular", spotLightSpecular);
	setFloat(floorShader, "spotLights[0].cutOff", lightCutOff);
	setFloat(floorShader, "spotLights[0].outerCutOff", lightCutOffOuter);
	setFloat(floorShader, "spotLights[0].constant", 1.0f);
	setFloat(floorShader, "spotLights[0].linear", 0.09f);
	setFloat(floorShader, "spotLights[0].quadratic", 0.032f);

	setVec3(floorShader, "spotLights[1].position", spotLightPos2);
	setVec3(floorShader, "spotLights[1].direction", spotLightDirection2);

	setVec3(floorShader, "spotLights[1].ambient", spotLightAmbient);
	setVec3(floorShader, "spotLights[1].diffuse", spotLightDiffuse);
	setVec3(floorShader, "spotLights[1].specular", spotLightSpecular);
	setFloat(floorShader, "spotLights[1].cutOff", lightCutOff);
	setFloat(floorShader, "spotLights[1].outerCutOff", lightCutOffOuter);
	setFloat(floorShader, "spotLights[1].constant", 1.0f);
	setFloat(floorShader, "spotLights[1].linear", 0.09f);

	setMat4(floorShader, "projection", projection);
	setMat4(floorShader, "view", view);
	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, floorPos);
	//model = glm::scale(model, glm::vec3(40.0f)); // a larger plane
	setMat4(floorShader, "model", model);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorDiffuse);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, floorSpecular);
	//
	//// render the cube
	glBindVertexArray(floorVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	/////////////////////////////////////////////////////////////////

	/////
	//////DRAW FLOOR
	glUseProgram(grassShader);
	setVec3(grassShader, "viewPos", cameraPos);
	setVec3(grassShader, "ambientColor", ambientColor);
	//setVec3(texturedShader, "material.ambient", 1.0f, 0.5f, 0.31f);
	//setVec3(texturedShader, "material.diffuse", 1.0f, 0.5f, 0.31f);
	//setVec3(texturedShader, "material.specular", 0.5f, 0.5f, 0.5f);
	setFloat(grassShader, "material.shininess", 30.0f);

	//setVec3(scolorShader, "objectColor", 1.0f, 0.5f, 0.31f);
	//setVec3(scolorShader, "lightColor", 1.0f, 1.0f, 1.0f);
	setVec3(grassShader, "pointlight.position", sunlightPos);
	setVec3(grassShader, "pointlight.ambient", sunlightAmbient);
	setVec3(grassShader, "pointlight.diffuse", sunlightDiffuse);
	setVec3(grassShader, "pointlight.specular", sunlightSpecular);


	setVec3(grassShader, "spotLights[0].position", spotLightPos1);
	setVec3(grassShader, "spotLights[0].direction", spotLightDirection1);

	setVec3(grassShader, "spotLights[0].ambient", spotLightAmbient);
	setVec3(grassShader, "spotLights[0].diffuse", spotLightDiffuse);
	setVec3(grassShader, "spotLights[0].specular", spotLightSpecular);
	setFloat(grassShader, "spotLights[0].cutOff", lightCutOff);
	setFloat(grassShader, "spotLights[0].outerCutOff", lightCutOffOuter);
	setFloat(grassShader, "spotLights[0].constant", 1.0f);
	setFloat(grassShader, "spotLights[0].linear", 0.09f);
	setFloat(grassShader, "spotLights[0].quadratic", 0.032f);

	setVec3(grassShader, "spotLights[1].position", spotLightPos2);
	setVec3(grassShader, "spotLights[1].direction", spotLightDirection2);

	setVec3(grassShader, "spotLights[1].ambient", spotLightAmbient);
	setVec3(grassShader, "spotLights[1].diffuse", spotLightDiffuse);
	setVec3(grassShader, "spotLights[1].specular", spotLightSpecular);
	setFloat(grassShader, "spotLights[1].cutOff", lightCutOff);
	setFloat(grassShader, "spotLights[1].outerCutOff", lightCutOffOuter);
	setFloat(grassShader, "spotLights[1].constant", 1.0f);
	setFloat(grassShader, "spotLights[1].linear", 0.09f);

	setMat4(grassShader, "projection", projection);
	setMat4(grassShader, "view", view);
	// world transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, grassPos);
	model = glm::scale(model, glm::vec3(0.150f)); // a smaller cube
	//model = glm::scale(model, glm::vec3(40.0f)); // a larger plane
	setMat4(grassShader, "model", model);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassDiffuse);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grassSpecular);
	//
	//// render the cube
	glBindVertexArray(grassVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	/////

	// also draw the lamp object
	glUseProgram(lsourceShader);
	setVec3(lsourceShader, "color", sunlightSpecular);

	setMat4(lsourceShader, "projection", projection);
	setMat4(lsourceShader, "view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, sunlightPos);
	model = glm::scale(model, glm::vec3(2.0f)); // a smaller cube
	setMat4(lsourceShader, "model", model);

	glBindVertexArray(lightSourceVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glutSwapBuffers();
}


//Initialising
void checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
void shaderInit() {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;

	std::string sColorVCode;
	std::string sColorFCode;

	std::string lSourceVCode;
	std::string lSourceFCode;

	std::string texturedVCode;
	std::string texturedFCode;

	//std::string skyboxVCode;
	//std::string skyboxFCode;

	//-----------------------------------------//
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	std::ifstream sColorVFile;
	std::ifstream sColorFFile;

	std::ifstream lSourceVFile;
	std::ifstream lSourceFFile;

	std::ifstream texturedVFile;
	std::ifstream texturedFFile;

	//std::ifstream skyboxVFile;
	//std::ifstream skyboxFFile;

	//-----------------------------------------//
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	sColorVFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	sColorFFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	lSourceVFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	lSourceFFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	texturedVFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	texturedFFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//skyboxVFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	//skyboxFFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		sColorVFile.open(scolorVPath);
		sColorFFile.open(scolorFPath);

		lSourceVFile.open(lsourceVPath);
		lSourceFFile.open(lsourceFPath);

		texturedVFile.open(texturedVPath);
		texturedFFile.open(texturedFPath);

		//skyboxVFile.open(skyboxVPath);
		//skyboxFFile.open(skyboxFPath);

		//-----------------------------------------//
		std::stringstream vShaderStream, fShaderStream;
		std::stringstream sColorVStream, sColorFStream;
		std::stringstream lSourceVStream, lSourceFStream;
		std::stringstream texturedVStream, texturedFStream;

		//std::stringstream skyboxVStream, skyboxFStream;

		//-----------------------------------------//
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		sColorVStream << sColorVFile.rdbuf();
		sColorFStream << sColorFFile.rdbuf();

		lSourceVStream << lSourceVFile.rdbuf();
		lSourceFStream << lSourceFFile.rdbuf();

		texturedVStream << texturedVFile.rdbuf();
		texturedFStream << texturedFFile.rdbuf();

		//skyboxVStream << skyboxVFile.rdbuf();
		//skyboxFStream << skyboxFFile.rdbuf();

		//-----------------------------------------//
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		sColorVFile.close();
		sColorFFile.close();

		lSourceVFile.close();
		lSourceFFile.close();

		texturedVFile.close();
		texturedFFile.close();

		//skyboxVFile.close();
		//skyboxFFile.close();

		//-----------------------------------------//
		//// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		sColorVCode = sColorVStream.str();
		sColorFCode = sColorFStream.str();

		lSourceVCode = lSourceVStream.str();
		lSourceFCode = lSourceFStream.str();

		texturedVCode = texturedVStream.str();
		texturedFCode = texturedFStream.str();

		//skyboxVCode = skyboxVStream.str();
		//skyboxFCode = skyboxFStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}

	//DEFAULTSHADER
	//-----------------------------------------//
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");


	// shader Program
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertex);
	glAttachShader(shaderID, fragment);
	glLinkProgram(shaderID);
	checkCompileErrors(shaderID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary

	// shader Program
	deershaderID = glCreateProgram();
	glAttachShader(deershaderID, vertex);
	glAttachShader(deershaderID, fragment);
	glLinkProgram(deershaderID);
	checkCompileErrors(deershaderID, "DEER PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	//-----------------------------------------//




	//SOLIDCOLOR
	//-----------------------------------------//
	const char* sColorVShaderCode = sColorVCode.c_str();
	const char* sColorFShaderCode = sColorFCode.c_str();
	// 2. compile shaders
	unsigned int sColorVertex, sColorFragment;
	// vertex shader
	sColorVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sColorVertex, 1, &sColorVShaderCode, NULL);
	glCompileShader(sColorVertex);
	checkCompileErrors(sColorVertex, "SCOLOR VERTEX");
	// fragment Shader
	sColorFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sColorFragment, 1, &sColorFShaderCode, NULL);
	glCompileShader(sColorFragment);
	checkCompileErrors(sColorFragment, "SCOLOR FRAGMENT");
	// shader Program
	scolorShader = glCreateProgram();
	glAttachShader(scolorShader, sColorVertex);
	glAttachShader(scolorShader, sColorFragment);
	glLinkProgram(scolorShader);
	checkCompileErrors(scolorShader, "SCOLOR PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(sColorVertex);
	glDeleteShader(sColorFragment);
	//-----------------------------------------//



	//LIGHTSOURCE
	//-----------------------------------------//
	const char* lSourceVShaderCode = lSourceVCode.c_str();
	const char* lSourceFShaderCode = lSourceFCode.c_str();
	// 2. compile shaders
	unsigned int lSourceVertex, lSourceFragment;
	// vertex shader
	lSourceVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(lSourceVertex, 1, &lSourceVShaderCode, NULL);
	glCompileShader(lSourceVertex);
	checkCompileErrors(lSourceVertex, "LSOURCE VERTEX");
	// fragment Shader
	lSourceFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(lSourceFragment, 1, &lSourceFShaderCode, NULL);
	glCompileShader(lSourceFragment);
	checkCompileErrors(lSourceFragment, "LSOURCE FRAGMENT");
	// shader Program
	lsourceShader = glCreateProgram();
	glAttachShader(lsourceShader, lSourceVertex);
	glAttachShader(lsourceShader, lSourceFragment);
	glLinkProgram(lsourceShader);
	checkCompileErrors(lsourceShader, "LSOURCE PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(lSourceVertex);
	glDeleteShader(lSourceFragment);
	//-----------------------------------------//



	//TEXTURED CUBE & FLOOR
	//-----------------------------------------//
	const char* texturedVShaderCode = texturedVCode.c_str();
	const char* texturedFShaderCode = texturedFCode.c_str();
	// 2. compile shaders
	unsigned int texturedVertex, texturedFragment;
	// vertex shader
	texturedVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(texturedVertex, 1, &texturedVShaderCode, NULL);
	glCompileShader(texturedVertex);
	checkCompileErrors(texturedVertex, "TEXTURED VERTEX");
	// fragment Shader
	texturedFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(texturedFragment, 1, &texturedFShaderCode, NULL);
	glCompileShader(texturedFragment);
	checkCompileErrors(texturedFragment, "TEXTURED FRAGMENT");


	// CUBE shader Program
	texturedShader = glCreateProgram();
	glAttachShader(texturedShader, texturedVertex);
	glAttachShader(texturedShader, texturedFragment);
	glLinkProgram(texturedShader);
	checkCompileErrors(texturedShader, "TEXTURED PROGRAM");

	// CAMPLEG shader Program
	campShader = glCreateProgram();
	glAttachShader(campShader, texturedVertex);
	glAttachShader(campShader, texturedFragment);
	glLinkProgram(campShader);
	checkCompileErrors(campShader, "TEXTURED PROGRAM");


	// Floor shader Program
	floorShader = glCreateProgram();
	glAttachShader(floorShader, texturedVertex);
	glAttachShader(floorShader, texturedFragment);
	glLinkProgram(floorShader);
	checkCompileErrors(floorShader, "FLOOR PROGRAM");

	// Grass shader Program
	grassShader = glCreateProgram();
	glAttachShader(grassShader, texturedVertex);
	glAttachShader(grassShader, texturedFragment);
	glLinkProgram(grassShader);
	checkCompileErrors(grassShader, "GRASS PROGRAM");


	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(texturedVertex);
	glDeleteShader(texturedFragment);
	//-----------------------------------------//
}
void textureInit() {

	//WOODBOX DIFFUSE & SPEC
	glGenTextures(1, &woodDiffuse);
	glBindTexture(GL_TEXTURE_2D, woodDiffuse);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(woodDiffusePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// specular

	glGenTextures(1, &woodSpecular);
	glBindTexture(GL_TEXTURE_2D, woodSpecular);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	data = stbi_load(woodSpecularPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	/////
	glUseProgram(texturedShader); // don't forget to activate the shader before setting uniforms!  
	setInt(texturedShader, "material.diffuse", 0);
	setInt(texturedShader, "material.specular", 1);


	//CAMPLEG DIFFUSE & SPEC
	glGenTextures(1, &campDiffuse);
	glBindTexture(GL_TEXTURE_2D, campDiffuse);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load(campDiffusePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// specular

	glGenTextures(1, &campSpecular);
	glBindTexture(GL_TEXTURE_2D, campSpecular);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	data = stbi_load(campSpecularPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	/////
	glUseProgram(campShader); // don't forget to activate the shader before setting uniforms!  
	setInt(campShader, "material.diffuse", 0);
	setInt(campShader, "material.specular", 1);

	////////FLESH D & S
	//CAMPLEG DIFFUSE & SPEC
	glGenTextures(1, &fleshDiffuse);
	glBindTexture(GL_TEXTURE_2D, fleshDiffuse);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load(fleshDiffusePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// specular

	glGenTextures(1, &fleshSpecular);
	glBindTexture(GL_TEXTURE_2D, fleshSpecular);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	data = stbi_load(fleshSpecularPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &fleshDiffuse2);
	glBindTexture(GL_TEXTURE_2D, fleshDiffuse2);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load(fleshDiffusePath2, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	/////
	glUseProgram(deershaderID); // don't forget to activate the shader before setting uniforms!
	setInt(deershaderID, "diffuseTex", 0);
	setInt(deershaderID, "specularTex", 1);
	setInt(deershaderID, "diffuseTex2", 2);
	//setInt(shaderID, "diffuseTex", 0);


	//FLOOR DIFFUSE & SPEC
	glGenTextures(1, &floorDiffuse);
	glBindTexture(GL_TEXTURE_2D, floorDiffuse);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load(floorDiffusePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// specular

	glGenTextures(1, &floorSpecular);
	glBindTexture(GL_TEXTURE_2D, floorSpecular);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	data = stbi_load(floorSpecularPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	/////
	glUseProgram(floorShader); // don't forget to activate the shader before setting uniforms!  
	setInt(floorShader, "material.diffuse", 0);
	setInt(floorShader, "material.specular", 1);



	//FLOOR DIFFUSE & SPEC
	glGenTextures(1, &grassDiffuse);
	glBindTexture(GL_TEXTURE_2D, grassDiffuse);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load(grassDiffusePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// specular

	glGenTextures(1, &grassSpecular);
	glBindTexture(GL_TEXTURE_2D, grassSpecular);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	data = stbi_load(grassSpecularPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	/////
	glUseProgram(grassShader); // don't forget to activate the shader before setting uniforms!  
	setInt(grassShader, "material.diffuse", 0);
	setInt(grassShader, "material.specular", 1);

}
void cubesInit() {
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	glGenVertexArrays(1, &campVAO);
	glBindVertexArray(campVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);




	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	glGenVertexArrays(1, &lightSourceVAO);
	glBindVertexArray(lightSourceVAO);
	
	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
void floorInit() {
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(floorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);


	glBindVertexArray(floorVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glGenBuffers(1, &grassEBO);

	glBindVertexArray(grassVAO);

	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grassEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);


	glBindVertexArray(grassVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
void trolleyInit() {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	mesh_data = load_mesh(MESH_NAME);
	loc1 = glGetAttribLocation(shaderID, "vertex_position");
	loc2 = glGetAttribLocation(shaderID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderID, "vertex_texture");


	glGenVertexArrays(1, &modelVAO);
	glGenBuffers(1, &model_vp_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model_vp_VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &model_vn_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model_vn_VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

	glBindVertexArray(modelVAO);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, model_vp_VBO);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, model_vn_VBO);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
void deerInit() {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.


	deermesh_data = load_mesh(DEER_MESH_NAME);
	deerloc1 = glGetAttribLocation(deershaderID, "vertex_position");
	deerloc2 = glGetAttribLocation(deershaderID, "vertex_normal");
	deerloc3 = glGetAttribLocation(deershaderID, "vertex_texture");

	glGenVertexArrays(1, &deermodelVAO);

	glGenBuffers(1, &deermodel_vp_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, deermodel_vp_VBO);
	glBufferData(GL_ARRAY_BUFFER, deermesh_data.mPointCount * sizeof(vec3), &deermesh_data.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &deermodel_vn_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, deermodel_vn_VBO);
	glBufferData(GL_ARRAY_BUFFER, deermesh_data.mPointCount * sizeof(vec3), &deermesh_data.mNormals[0], GL_STATIC_DRAW);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	unsigned int vt_vbo = 0;
	glGenBuffers (1, &vt_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glBufferData (GL_ARRAY_BUFFER, deermesh_data.mPointCount * sizeof(vec2), &deermesh_data.mTextureCoords[0], GL_STATIC_DRAW);

	glBindVertexArray(deermodelVAO);

	glEnableVertexAttribArray(deerloc1);
	glBindBuffer(GL_ARRAY_BUFFER, deermodel_vp_VBO);
	glVertexAttribPointer(deerloc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(deerloc2);
	glBindBuffer(GL_ARRAY_BUFFER, deermodel_vn_VBO);
	glVertexAttribPointer(deerloc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	glEnableVertexAttribArray (loc3);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}
void init() {
	glEnable(GL_DEPTH_TEST);
	shaderInit();
	cubesInit();
	floorInit();
	trolleyInit();
	deerInit();
	textureInit();
}


//Main
int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("Hello Triangle");

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(mouseMotion);
	glutIdleFunc(update);  // Set the update function as the idle function
	glutReshapeFunc(window_reshape_callback);
	glutMouseFunc(NULL);

	// Set up your objects and shaders
	init();

	// Begin infinite event loop
	glutMainLoop();
	return 0;
}