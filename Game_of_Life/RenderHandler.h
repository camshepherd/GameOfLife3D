#ifndef RENDERHANDLER_H
#define RENDERHANDLER_H
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <boost\filesystem.hpp>
#include "vboindexer.hpp"
#include "Handler.h"
using namespace sf;
using namespace boost::filesystem;

/**
	A Handler to control the entire drawing of the 3D matrix of cells into the OpenGL context as well as the manipulation of the perspective.
	@author Cameron Shepherd
*/
class RenderHandler :
	public Handler
{
public:
	
	/** Constructor 1. Default constructor*/
	RenderHandler();

	/** Constructor 2. Set up the RenderHandler, initialise all the values, set up communication channel to the creator.
		@param name: The name for this instance
		@param creator: The name of the creator
		@param outputQueue: The communication queue for the creator
		@param theWindow: The sf::RenderWindow that contains the active OpenGL context
	*/
	RenderHandler(string name, string creator, weak_ptr<queue<SEvent>> outputQueue, weak_ptr<sf::RenderWindow> theWindow);

	/** Default destructor*/
	~RenderHandler();

	/** Draw the current state of the universe to the window.*/
	void draw();

	/** Move the camera in the drawn universe*/
	void moveCamera(sf::Vector2i rotation, sf::Vector3i translation, float zoom);

private:
	/** Instance-specific update function. This behaviour will take place after the generic update() functionality @see Handler.update().
	*/
	void updateH();

	/** Instance-specific event-handling functionality. This event-handling will take place after the generic event-handling functionality. 
		@see Handler.handleEvent().
		@param theEvent: The event to be handled.
	*/
	void handleEventH(SEvent theEvent);

	/** Initialise the OpenGL matrices, ready to draw.*/
	void init();

	/** The window to be drawn to. Assumed that it is active.*/
	shared_ptr<sf::RenderWindow> _window;

	/** The current stored frames*/
	weak_ptr<vector<vector<vector<int>>>> storedFrame;

	/** Load the specified shaders in from file for future use by the RenderHandler.
		@param vertex_file_path: Location of the vertex shader.
		@param fragment_file_path: Location of the fragment shader.
	*/
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

	/** Load in the cube object file for future use by the RenderHandler.
		@param path: The path to the .obj file.
	*/
	bool loadOBJ(string path);

	/** Draw the cube at the specified position.
		@param x: The x position of the cube
		@param y: The y position of the cube
		@param z: The z position of the cube
	*/
	void drawCube(float x, float y, float z);

	/** Draw the entire current frame.
		@param frame: The frame to be drawn.
	*/
	void drawCubes(weak_ptr<vector<vector<vector<int>>>> frame);

	/** Reset the camera to the default position.*/
	void resetCamera();
	
	/** Unbind the buffers, ready for drawing by SFML.*/
	void unbind();

	/** Store in memory all the cube colours based on position to save future computation.*/
	void genColours();

	/** Collect and send to standard output the OpenGL messages. For testing purposes.
		@param numMsgs: The number of messages to take from the message queue.
	*/
	void GetFirstNMessages(GLuint numMsgs);


	// Various identifiers
	GLuint VertexArrayID;
	GLuint vertexbuffer, programID, colorbuffer, normalbuffer, uvBuffer, elementbuffer;
	GLuint MatrixID, textureID, textureSamplerID, MaterialColorID, LightColorID;
	GLuint Light1PowerID, Light2PowerID, Light3PowerID, Light4PowerID;
	GLuint ViewMatrixID, ModelMatrixID, SpecularBrightnessID, AmbientBrightnessID;
	GLuint Light1ID, Light2ID, Light3ID, Light4ID;
	int height;
	int width;
	glm::mat4 Projection, View, preView, Model, Scale, MVP;
	sf::Vector3i frameMovement = sf::Vector3i(0,0,0);
	sf::Vector2i frameRotation = sf::Vector2i(0, 0);
	int frameZoom = 0;
	float cubeScaling = 0.4;
	std::vector<unsigned short int> cubeIndices;

	glm::vec3 up, right, direction;

	int simWidth;

	float distance;
	// position
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 light1Pos = glm::vec3(simWidth * 1.3, simWidth * 1.3, simWidth * 0.5);
	glm::vec3 light2Pos = glm::vec3(simWidth * -1 * 1.3, simWidth * 1.7, simWidth * - 1* 1.2);
	glm::vec3 light3Pos = glm::vec3(simWidth * -1 * 0.6, simWidth * -1 * 1.1, simWidth * 1.4);
	glm::vec3 light4Pos = glm::vec3(simWidth * -1 * 0.6, simWidth * -1 * 1.1, simWidth * -1 * 1.4);
	glm::vec3 cameraRotations = glm::vec3(0, 0, 0);
	float cameraDistance;

	float speedZoom = 4;
	float speedMovement = 0.00000001f; // units / second
	float mouseSpeed = 0.00005f;
	sf::Clock clock, clock2;
	int timing;

	float FoV = 45;

	glm::vec3 orientation;

	// The cube data read from the .obj file.
	std::vector< glm::vec3 > cubeVertices;
	std::vector< glm::vec2 > cubeUVs;
	std::vector< glm::vec3 > cubeNormals; 

	std::vector< glm::vec3 > cubeVerticesIndexed;
	std::vector< glm::vec3 > cubeNormalsIndexed;

	unique_ptr<vector<vector<vector<glm::vec3>>>> cellColours;
	vector<glm::vec3> rgbPos;
	

	float light1Power, light2Power, light3Power, light4Power;
};

#endif