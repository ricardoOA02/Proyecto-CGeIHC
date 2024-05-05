/*
Proyecto Final
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

//Biblioteca de audio
#include <irrklang/irrKlang.h>

//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

using namespace irrklang;

const float PI = 3.14159265f;
const float toRadians = 3.14159265f / 180.0f;
using std::vector;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera camera2;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture CespesTexture;

//Estructuras
Model TorreTLOZ;
Model CasaCoraje_M;
Model Igloo_M;
Model Isla;

//Vehiculos
Model CarroCoraje_M;
Model HeliceGuardian;
Model CabezaGuardian;
Model CuerpoGuardian;

//Objetos
Model Banca_M;
Model Kiosko_M;
Model Bote_Basura_M;
Model Fuente_M;
Model Mesa;
Model Bebedero;
Model Coraje_M;
Model Lampara_M;
Model EspadaMaestra;

//Flora
Model ArbolMuerto_M;
Model Pino_M;
Model Korok;

//Faun
Model Caballo;
Model Zorro;
Model Pinguino_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void CrearPlano()
{
	unsigned int plano_indices[] = {
		// Indices
		9,0,2,
		2,0,1,
		9,2,8,
		8,2,5,
		5,2,4,
		4,2,3,
		8,5,6,
		7,8,6,
	};

	GLfloat plano_vertices[] = {
		// Vértices: (x, y, z, s(x), t(y), nx, ny, nz)
		-0.234f,0.0f,	0.27f,      0.0f,   0.0f,   0.0f,-1.0f,0.0f, // C 0
		1.8f,	0.0f,	0.27f,      10.0f,   0.0f,   0.0f,-1.0f,0.0f, // D 1
		1.8f,	0.0f,	-1.642f,    10.0f,   10.0f,   0.0f,-1.0f,0.0f, // E 2
		5.42f,	0.0f,	-1.64f,     30.0f,   10.0f,   0.0f,-1.0f,0.0f, // F 3
		5.42f,	0.0f,	-2.1f,      30.0f,   13.0f,   0.0f,-1.0f,0.0f, // G 4
		6.95f,	0.0f,	-3.3f,      37.0f,   20.0f,   0.0f,-1.0f,0.0f, // H 5
		7.33f,	0.0f,	-3.3f,      40.0f,   20.0f,   0.0f,-1.0f,0.0f, // I 6
		7.33f,	0.0f,	-3.94f,     40.0f,   23.0f,   0.0f,-1.0f,0.0f, // J 7
		4.568f,	0.0f,	-3.94f,     22.0f,   23.0f,   0.0f,-1.0f,0.0f, // K 8
		-1.27f,	0.0f,	-3.94f,     -5.0f,	23.0f,   0.0f,-1.0f,0.0f, // L 9
	};

	Mesh* planoParque = new Mesh();
	planoParque->CreateMesh(plano_vertices, plano_indices, 10 * 8, 8 * 3); // 13 vértices, 8 caras, cada cara con 3 vértices
	meshList.push_back(planoParque);
}

void CrearToroide(GLfloat radioToro, GLfloat radioTubo, int vertRes, int horRes) {
	int i, j, k;

	GLfloat dtVer = 2 * PI / vertRes, dtHor = 2 * PI / horRes, x, z, y, theta, phi; // Se calculan el tamaño del angulo para la resolucion vertical y horizontal

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	for (j = 0; j <= vertRes; j++)
	{
		theta = dtVer * j;
		for (k = 0; k <= horRes; k++)
		{
			phi = dtHor * k;
			x = (radioToro + radioTubo * cos(phi)) * cos(theta); // Calculo de las coordenadas de los puntos en base a la ecuacion del toroide
			z = (radioToro + radioTubo * cos(phi)) * sin(theta); // tomando en cuenta resolucion vertical y horizontal
			y = radioTubo * sin(phi);
			for (i = 0; i < 3; i++)
			{
				switch (i) { // Se agregan los vertices al vector
				case 0:
					vertices.push_back(x);
					break;
				case 1:
					vertices.push_back(y);
					break;
				case 2:
					vertices.push_back(z);
					break;
				}
			}
		}
	}

	int control = 0, index1 = 0; // Variables de control para crear los indices
	int index2 = index1 + horRes;

	while (control < (vertRes * horRes * 6) + vertRes * 6)
	{
		indices.push_back(index1); // Se agregan los indices para formar triangulos
		indices.push_back(index2);
		if (control % 2 == 0)
		{
			index1++;
			indices.push_back(index1);
		}
		else
		{
			index2++;
			indices.push_back(index2);
		}
		control++;
	}

	Mesh* toro = new Mesh(); // Se creal el mesh para renderizar la figura
	toro->CreateMeshGeometry(vertices, indices, vertices.size(), (vertRes * horRes * 6) + vertRes * 6);
	meshList.push_back(toro);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	//Inicialización del sonido
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	if (!engine)
		printf("No se puede inicializar el audio\n");

	CreateObjects();
	CrearPlano();
	CreateShaders();
	CrearToroide(10.0f, 3.0f, 20, 20);
	camera = Camera(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);
	camera2 = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);
	
	//brickTexture = Texture("Textures/brick.png");
	//brickTexture.LoadTextureA();
	//dirtTexture = Texture("Textures/dirt.png");
	//dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	CespesTexture = Texture("Textures/cesped.tga");
	CespesTexture.LoadTextureA();
	
	// Texturas

	//Modelos

	Banca_M = Model();
	Banca_M.LoadModel("Models/banca.obj");
	Kiosko_M = Model();
	Kiosko_M.LoadModel("Models/kiosko.obj");
	Bote_Basura_M = Model();
	Bote_Basura_M.LoadModel("Models/botebasura.obj");
	Fuente_M = Model();
	Fuente_M.LoadModel("Models/fuente.obj");
	Mesa = Model();
	Mesa.LoadModel("Models/Mesa.obj");
	Bebedero = Model();
	Bebedero.LoadModel("Models/Bebedero.obj");
	TorreTLOZ = Model();
	TorreTLOZ.LoadModel("Models/TLOZ/TorreDeMapeo.obj");
	ArbolMuerto_M = Model();
	ArbolMuerto_M.LoadModel("Models/arbolmuerto.obj");
	CarroCoraje_M = Model();
	CarroCoraje_M.LoadModel("Models/Coraje/carro.obj");
	Coraje_M = Model();
	Coraje_M.LoadModel("Models/Coraje/coraje.obj");
	CasaCoraje_M = Model();
	CasaCoraje_M.LoadModel("Models/Coraje/casaCoraje.obj");
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lampara.obj");
	Pino_M = Model();
	Pino_M.LoadModel("Models/pino.obj");
	Igloo_M = Model();
	Igloo_M.LoadModel("Models/igloo.obj");
	CabezaGuardian = Model();
	CabezaGuardian.LoadModel("Models/TLOZ/Guardian/CabezaGuardian.obj");
	CuerpoGuardian = Model();
	CuerpoGuardian.LoadModel("Models/TLOZ/Guardian/CuerpoGuardian.obj");
	HeliceGuardian = Model();
	HeliceGuardian.LoadModel("Models/TLOZ/Guardian/HeliceGuardian.obj");
	Isla = Model();
	Isla.LoadModel("Models/TLOZ/Isla.obj");
	Korok = Model();
	Korok.LoadModel("Models/TLOZ/Korok.obj");
	Caballo = Model();
	Caballo.LoadModel("Models/Caballo.obj");
	Zorro = Model();
	Zorro.LoadModel("Models/Zorro.obj");
	Pinguino_M = Model();
	Pinguino_M.LoadModel("Models/horadeaven/gunter.obj");
	EspadaMaestra = Model();
	EspadaMaestra.LoadModel("Models/TLOZ/EspadaMaestra.obj");

	// Skybox
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/skybox_mountains_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/skybox_mountains_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/skybox_mountains_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/skybox_mountains_up.tga");
	skyboxFaces.push_back("Textures/Skybox/skybox_mountains_ft.tga");
	skyboxFaces.push_back("Textures/Skybox/skybox_mountains_bk.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	//Ejecución del sonido de fondo
	//engine->play2D("audios/parque.mp3", true);
	irrklang::ISound* AudioFondo = engine->play2D("audios/parque.mp3", true, false, true);
	AudioFondo->setVolume(0.1f);

	//Inicia el audio 3D
	irrklang::vec3df posicionOyente(0, 0, 0);
	irrklang::vec3df direccionOyente(0, -1, 0);
	irrklang::vec3df velPerSecond(0, 0, 0);
	irrklang::vec3df upVector(0, 1, 0);

	//Sonidos de objetos

	//Audio del carro
	irrklang::vec3df posicion(20.0f, -1.0f, -20.0f);
	irrklang::ISound* AudioCarro = engine->play3D("audios/carro.flac", posicion, true, true);

	AudioCarro->setMinDistance(15.0f);
	AudioCarro->setIsPaused(false);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//camera.mouseControl(mainWindow.getXChange(), 0.0f);
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		//

		// Luz ligada a la cámara de tipo flash
		// sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[2]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(200.0f, 1.0f, 200.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		CespesTexture.UseTexture();
		meshList[4]->RenderMesh();


		//Actualizar la posicion del audio
		posicionOyente.X = camera.getCameraPosition().x;
		posicionOyente.Y = camera.getCameraPosition().y;
		posicionOyente.Z = camera.getCameraPosition().z;
		direccionOyente.X = -camera.getCameraDirection().x;
		direccionOyente.Y = -camera.getCameraDirection().y;
		direccionOyente.Z = -camera.getCameraDirection().z;

		engine->setListenerPosition(posicionOyente, direccionOyente, velPerSecond, upVector);

		//Instancia de la banca 
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(1.0f, -1.6f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Instancia Kiosko
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kiosko_M.RenderModel();

		//Instancia Bote de Basura
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, -0.7f, -4.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_Basura_M.RenderModel();

		//Instancia fuente
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, -0.3f, -18.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fuente_M.RenderModel();
		
		//Instancia mesa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, -0.3f, -8.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();

		//Instancia Bebedero
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-15.0f, -0.3f, 8.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bebedero.RenderModel();

		//Instancia torre
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-25.0f, -0.3f, 20.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TorreTLOZ.RenderModel();

		//Instancia Arbol Muerto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArbolMuerto_M.RenderModel();

		//Instancia Carro de Coraje
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -1.0f, -20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarroCoraje_M.RenderModel();
		//Sonido del carro


		//Instancia Corage
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(camera.getCameraPosition().x, camera.getCameraPosition().y - 11.0f, camera.getCameraPosition().z - 8.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coraje_M.RenderModel();
		
		//Instancia Casa de Coraje
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -1.0f, -60.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaCoraje_M.RenderModel();

		//Instancia Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -1.0f, -60.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();

		//Instancia Pinos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -1.0f, -60.0f));
		//model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pino_M.RenderModel();

		//Instancia Igloo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -1.0f, -60.0f));
		//model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Igloo_M.RenderModel();

		//Instancia korok
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 4.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Korok.RenderModel();

		//Instancia caballo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 5.0f, 50.0f));
		//model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Caballo.RenderModel();

		//Instancia zorro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 5.0f, 50.0f));
		//model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Zorro.RenderModel();

		//Instancia pinguino
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -1.0f, 50.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pinguino_M.RenderModel();

		//Instancia espada maestra
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -1.0f, 50.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EspadaMaestra.RenderModel();

		//Instancia toroide
		//model = glm::mat4(1.0);
		////model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		////model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[5]->RenderMesh();

		

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	AudioCarro->drop();
	return 0;
}
