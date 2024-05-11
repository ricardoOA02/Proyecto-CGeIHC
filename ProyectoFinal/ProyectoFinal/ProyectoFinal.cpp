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

//para iluminaci�n
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

//Personajes
Model CorajeBase_M; //Hecho
Model PataDer_M;
Model PataIzq_M;
Model ManoDer_M;
Model ManoIzq_M;

//Estructuras
Model Piso_M;		//Hecho
Model TorreTLOZ;	//Hecho	
Model CasaCoraje_M;	//Hecho
Model Igloo_M;		//Hecho
Model Isla;
Model CasaLink;		//Hecho

//Vehiculos
Model CarroCoraje_M;	//Hecho
Model LlantaDelDer_M;
Model LlantaDelIzq_M;
Model LlantaTraDer_M;
Model LlantaTraIzq_M;
Model HeliceGuardian;	//Hecho
Model CabezaGuardian;
Model CuerpoGuardian;
Model Barco_M;			//Hecho

//Objetos
Model Banca_M;
Model Kiosko_M;
Model Bote_Basura_M;
Model Fuente_M;
Model Mesa;
Model Bebedero;
Model Lampara_M;

//Flora
Model Cardo_M;
Model ArbolMuerto_M;
Model Pino_M;
Model Korok;

//Fauna
Model Caballo;
Model Zorro;
Model Pinguino_M;

//Otros
Model EspadaMaestra;
Model Slime;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//Variables de animacion
float rotLlantas;
float rotLlantasOffset;
float movCarro;
float movCarroOffset;
float rotCoche;
float rotCocheOffset;
bool carroAvanza;

float movXCardo;
float movXCardoOffset;
float movZCardo;
float movZCardoOffset;
float rotXCardo;
float rotXCardoOffset;
float rotZCardo;
float rotZCardoOffset;
bool cardoAvanza;
bool cardoRetrocede;
bool cardoDerecha;
bool cardoIzquierda;

float movPinguino;
float movPinguinoOffset;
float rotPinguino;
float rotPinguinoOffset;
float slimeSalto;
float slimeAvanza;
float slimeRotacion;
float slimeOffset;
bool pinguinoAvanza;
bool inclinacion;

float movBarco;
float movBarcoOffset;
float rotBarco;
float rotBarcoOffset;
bool barcoAvanza;
bool giroBarco;

float offsetGuardian;
float movGuardian;
float rotGuardian;
bool guardianAvanza;

float rotCoraje;
float rotCorajeOffset;
bool giroCoraje;

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


//funci�n de calculo de normales por promedio de v�rtices 
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
		// V�rtices: (x, y, z, s(x), t(y), nx, ny, nz)
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
	planoParque->CreateMesh(plano_vertices, plano_indices, 10 * 8, 8 * 3); // 13 v�rtices, 8 caras, cada cara con 3 v�rtices
	meshList.push_back(planoParque);
}

void CrearToroide(GLfloat radioToro, GLfloat radioTubo, int vertRes, int horRes) {
	int i, j, k;

	GLfloat dtVer = 2 * PI / vertRes, dtHor = 2 * PI / horRes, x, z, y, theta, phi; // Se calculan el tama�o del angulo para la resolucion vertical y horizontal

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

	//Inicializaci�n del sonido
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	if (!engine)
		printf("No se puede inicializar el audio\n");

	CreateObjects();
	CrearPlano();
	CreateShaders();
	CrearToroide(10.0f, 3.0f, 20, 20);
	camera = Camera(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.75f, 0.5f, false);
	camera2 = Camera(glm::vec3(0.0f, 80.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -90.0f, 1.0f, 0.5f, true);
	
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
	Piso_M = Model();
	Piso_M.LoadModel("Models/piso.obj");
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
	CarroCoraje_M.LoadModel("Models/Coraje/carroBase.obj");
	LlantaDelDer_M = Model();
	LlantaDelDer_M.LoadModel("Models/Coraje/llantaDelDer.obj");
	LlantaDelIzq_M = Model();
	LlantaDelIzq_M.LoadModel("Models/Coraje/llantaDelIzq.obj");
	LlantaTraDer_M = Model();
	LlantaTraDer_M.LoadModel("Models/Coraje/llantaTraDer.obj");
	LlantaTraIzq_M = Model();
	LlantaTraIzq_M.LoadModel("Models/Coraje/llantaTraIzq.obj");
	CorajeBase_M = Model();
	CorajeBase_M.LoadModel("Models/Coraje/corajeBase.obj");
	PataDer_M = Model();
	PataDer_M.LoadModel("Models/Coraje/pataDer.obj");
	PataIzq_M = Model();
	PataIzq_M.LoadModel("Models/Coraje/pataIzq.obj");
	ManoDer_M = Model();
	ManoDer_M.LoadModel("Models/Coraje/manoDer.obj");
	ManoIzq_M = Model();
	ManoIzq_M.LoadModel("Models/Coraje/manoIzq.obj");
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
	Slime = Model();
	Slime.LoadModel("Models/TLOZ/Slime.obj");
	CasaLink = Model();
	CasaLink.LoadModel("Models/TLOZ/CasaVentanas.obj");
	Cardo_M = Model();
	Cardo_M.LoadModel("Models/Coraje/cardo.obj");
	Barco_M = Model();
	Barco_M.LoadModel("Models/horadeaven/barco.obj");

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


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
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
	//Ejecuci�n del sonido de fondo
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

	//Animaciones
	carroAvanza = true;
	rotLlantas = 0.0f;
	rotLlantasOffset = 5.0f;
	movCarro = 1.0f;
	movCarroOffset = 0.5f;

	cardoAvanza = true;
	cardoRetrocede = cardoIzquierda = cardoDerecha = false;
	movXCardo = movZCardo = 1.0f;
	movXCardoOffset = movZCardoOffset = 0.3f;
	rotXCardo = 0.0f;
	rotXCardoOffset = 4.0f;
	rotZCardo = 0.0f;
	rotZCardoOffset = 4.0f;

	pinguinoAvanza = true;
	inclinacion = true;
	movPinguino = 1.0f;
	movPinguinoOffset = 0.1f;
	rotPinguino = -15.0f;
	rotPinguinoOffset = 3.0f;

	slimeAvanza = 0.0f;
	slimeRotacion = 0.0f;
	slimeSalto = 0.0f;
	slimeOffset = 1.0f;

	barcoAvanza = true;
	giroBarco = true;
	movBarcoOffset = 0.3f;
	movBarco = 0.0f;
	rotBarco = -1.0f;
	rotBarcoOffset = 1.0f;

	offsetGuardian = 0.5f;
	movGuardian = 0.0f;
	rotGuardian = 0.0f;
	guardianAvanza = true;

	rotCorajeOffset = 0.7f;
	rotCoraje = 0.0f;
	giroCoraje = true;

	lastTime = glfwGetTime();

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//printf("%f\n", deltaTime);
		//Animaciones

		//Animacion de carro
		if (carroAvanza)
		{
			if (movCarro > -200.0f)
			{
				movCarro -= movCarroOffset * deltaTime;
				rotLlantas -= rotLlantasOffset * deltaTime;
			}
			else
				carroAvanza = false;
		}
		else
		{
			if (movCarro < 200.0f)
			{
				movCarro += movCarroOffset * deltaTime;
				rotLlantas += rotLlantasOffset * deltaTime;
			}
			else
				carroAvanza = true;
		}
		if (rotLlantas > 360.0f || rotLlantas < -360.0f)
			rotLlantas = 0.0f;

		//Animacion b�sica del cardo
		if (cardoAvanza)
		{
			if (movZCardo > -100.0f)
			{
				movZCardo -= movZCardoOffset * deltaTime;
				rotXCardo -= rotXCardoOffset * deltaTime;
				rotZCardo = 0.0f;
			}
			else
			{
				cardoAvanza = false;
				cardoIzquierda = true;
			}
		}
		if (cardoRetrocede)
		{
			if (movZCardo < 100.0f)
			{
				movZCardo += movZCardoOffset * deltaTime;
				rotXCardo += rotXCardoOffset * deltaTime;
				rotZCardo = 0.0f;
			}
			else
			{
				cardoRetrocede = false;
				cardoDerecha = true;
			}
		}
		if (cardoIzquierda)
		{
			if (movXCardo > -100.0f)
			{
				movXCardo -= movXCardoOffset * deltaTime;
				rotZCardo += rotZCardoOffset * deltaTime;
				rotXCardo = 0.0f;
			}
			else
			{
				cardoRetrocede = true;
				cardoIzquierda = false;
			}
		}
		if (cardoDerecha)
		{
			if (movXCardo < 100.0f)
			{
				movXCardo += movXCardoOffset * deltaTime;
				rotZCardo -= rotZCardoOffset * deltaTime;
				rotXCardo = 0.0f;
			}
			else
			{
				cardoAvanza = true;
				cardoDerecha = false;
			}
		}
		if (rotXCardo > 360.0f || rotXCardo < -360.0f)
			rotXCardo = 0.0f;
		if (rotZCardo > 360.0f || rotZCardo < -360.0f)
			rotZCardo = 0.0f;
		
		// Animacion vehiculo aereo
		if (movGuardian > 100.0f)
		{
			guardianAvanza = false;
		}
		else if (movGuardian < -100.0f)
		{
			guardianAvanza = true;
		}

		if (guardianAvanza)
		{
			movGuardian += offsetGuardian * deltaTime;
			rotGuardian += 15 * offsetGuardian * deltaTime;
		}
		else
		{
			movGuardian -= offsetGuardian * deltaTime;
			rotGuardian += 15 * offsetGuardian * deltaTime;
		}

		//Animaci�n b�sica del Pinguino
		if (pinguinoAvanza)
		{
			if (movPinguino > -80.0f)
				movPinguino -= movPinguinoOffset * deltaTime;
			else
				pinguinoAvanza = false;
		}
		else
		{
			if (movPinguino < 80.0f)
				movPinguino += movPinguinoOffset * deltaTime;
			else
				pinguinoAvanza = true;
		}
		if (inclinacion)
		{
			if (rotPinguino > -15.0f)
				rotPinguino -= rotPinguinoOffset * deltaTime;
			else
				inclinacion = false;
		}
		else
		{
			if (rotPinguino < 15.0f)
				rotPinguino += rotPinguinoOffset * deltaTime;
			else
				inclinacion = true;
		}

		// Animacion compleja slime
		if (slimeSalto == 180.0f)
		{
			slimeSalto = 0.0f;
		}
		else
		{
			slimeSalto += 0.005f;
		}

		if (slimeAvanza == 360.0f)
		{
			slimeAvanza = 0.0f;
		}
		else
		{
			slimeAvanza += 0.0025f;
		}

		if (slimeRotacion == 360.0f)
		{
			slimeRotacion = 0.0f;
		}
		else
		{
			slimeRotacion += slimeOffset * deltaTime;
		}

		// Animacion barco
		if (barcoAvanza)
		{
			if (movBarco > -500.0f)
				movBarco -= movBarcoOffset * deltaTime;
			else
				barcoAvanza = false;
		}
		else
		{
			if (movBarco < 500.0f)
				movBarco += movBarcoOffset * deltaTime;
			else
				barcoAvanza = true;
		}
		if (giroBarco)
		{
			if (rotBarco > -15.0f)
				rotBarco -= rotBarcoOffset * deltaTime;
			else
				giroBarco = false;
		}
		else
		{
			if (rotBarco < 15.0f)
				rotBarco += rotBarcoOffset * deltaTime;
			else
				giroBarco = true;
		}

		//Animacion personaje
		if (mainWindow.getMovimientoEdo() && mainWindow.getCamaraEdo())
		{
			if (giroCoraje)
			{
				if (rotCoraje > -15.0f)
					rotCoraje -= rotCorajeOffset * deltaTime;
				else
					giroCoraje = false;
			}
			else
			{
				if (rotCoraje < 15.0f)
					rotCoraje += rotCorajeOffset * deltaTime;
				else
					giroCoraje = true;
			}
		}

		
		//Recibir eventos del usuario
		glfwPollEvents();

		
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
		
		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		if (mainWindow.getCamaraEdo())
		{
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		}
		else
		{
			camera2.keyControl(mainWindow.getsKeys(), deltaTime);
			//camera2.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera2.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera2.getCameraPosition().x, camera2.getCameraPosition().y, camera2.getCameraPosition().z);
		}
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		//

		// Luz ligada a la c�mara de tipo flash
		// sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelAuxCarro(1.0);
		glm::mat4 modelAuxGuardian(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[2]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(500.0f, 1.0f, 500.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//CespesTexture.UseTexture();
		//meshList[4]->RenderMesh();
		Piso_M.RenderModel();

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
		model = glm::translate(model, glm::vec3(-578.0f, -0.3f, -62.8f));
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
		//model = glm::rotate(model, glm::radians(rotCoche), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-113.2f, -1.0f, 248.7f + movCarro));
		modelAuxCarro = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarroCoraje_M.RenderModel();
		
		//Llanta Del Der
		model = modelAuxCarro;
		model = glm::translate(model, glm::vec3(-3.0f, 1.987f, 7.6f));
		model = glm::rotate(model, glm::radians(rotLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDelDer_M.RenderModel();

		//Llanta Del Izq
		model = modelAuxCarro;
		model = glm::translate(model, glm::vec3(3.0f, 1.987f, 7.6f));
		model = glm::rotate(model, glm::radians(rotLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDelIzq_M.RenderModel();

		//Llanta Tra Der
		model = modelAuxCarro;
		model = glm::translate(model, glm::vec3(-2.0f, 1.987f, -6.5f));
		model = glm::rotate(model, glm::radians(rotLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTraDer_M.RenderModel();

		//Llanta Tra Izq
		model = modelAuxCarro;
		model = glm::translate(model, glm::vec3(2.0f, 1.987f, -6.5f));
		model = glm::rotate(model, glm::radians(rotLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTraIzq_M.RenderModel();

		//Instancia Cuerpo Coraje
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(camera.getCameraPosition().x + glm::cos(glm::radians(camera.getYaw())) * 8.0, camera.getCameraPosition().y - 11.0f, camera.getCameraPosition().z + glm::sin(glm::radians(camera.getYaw())) * 8.0));
		model = glm::rotate(model, glm::radians(180.0f - camera.getYaw()), glm::vec3(0.0f, 1.0, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CorajeBase_M.RenderModel();

		//Pata Der Coraje
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.15f, 0.5f));
		model = glm::rotate(model, glm::radians(rotCoraje), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataDer_M.RenderModel();

		//Pata Izq Coraje
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.131f, -0.5f));
		model = glm::rotate(model, glm::radians(-rotCoraje), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataIzq_M.RenderModel();

		//Mano Der Coraje
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.889f));
		model = glm::rotate(model, glm::radians(-rotCoraje), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManoIzq_M.RenderModel();

		//Mano Izq Coraje
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.51f, -0.89f));
		model = glm::rotate(model, glm::radians(rotCoraje), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManoDer_M.RenderModel();
		
		//Instancia Casa de Coraje
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(356.65f, -1.0f, -188.7f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaCoraje_M.RenderModel();

		// Instancia del guardian (TLOZ)
		// cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-329.7f + movGuardian, 30.0f, -42.06f));
		modelAuxGuardian = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoGuardian.RenderModel();
		// helice 1
		model = modelAuxGuardian;
		model = glm::translate(model, glm::vec3(0.0f, 12.0f, 10.2f));
		model = glm::rotate(model, glm::radians(110 + rotGuardian), glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HeliceGuardian.RenderModel();
		// helice 2
		model = modelAuxGuardian;
		model = glm::translate(model, glm::vec3(-9.0f, 12.0f, -5.2f));
		model = glm::rotate(model, glm::radians(rotGuardian), glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HeliceGuardian.RenderModel();
		// helice 3
		model = modelAuxGuardian;
		model = glm::translate(model, glm::vec3(9.0f, 12.0f, -5.2f));
		model = glm::rotate(model, glm::radians(70 + rotGuardian), glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HeliceGuardian.RenderModel();
		// cabeza
		model = modelAuxGuardian;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabezaGuardian.RenderModel();

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
		model = glm::translate(model, glm::vec3(641.5f, -1.0f, -193.15f));
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
		model = glm::translate(model, glm::vec3(-217.0f, 0.0f, 395.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Caballo.RenderModel();

		//Instancia zorro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-437.3f, 0.0f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Zorro.RenderModel();

		//Instancia pinguino
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(538.8f , -1.0f, -219.9f + movPinguino));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, glm::radians(-rotPinguino), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

		//Instancia Casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.2f, 1.0f, -174.9f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CasaLink.RenderModel();
		glDisable(GL_BLEND);

		//Instancia Slime
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-570.3 + 10 * cos(slimeAvanza), 1.0f + 5 * abs(sin(slimeSalto)), 53.85f + 10 * sin(slimeAvanza)));
		model = glm::rotate(model, glm::radians(90-slimeRotacion), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Slime.RenderModel();
		glDisable(GL_BLEND);


		//Instancia Cardo
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(378.4 + movXCardo, 0.0f, -245 + movZCardo));
		model = glm::rotate(model, glm::radians(rotXCardo), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotZCardo), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cardo_M.RenderModel();

		//Instancia Barco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-31.45 + movBarco, 0.0f, -447.85f));
		model = glm::rotate(model, glm::radians(rotBarco), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Barco_M.RenderModel();

		//Instancia isla
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-326.4f, 40.0f, -178.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Isla.RenderModel();

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
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
