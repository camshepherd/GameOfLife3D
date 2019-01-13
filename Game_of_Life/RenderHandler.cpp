#ifndef RENDER_HANDLER_H
#define RENDER_HANDLER_H

#include "RenderHandler.h"

class Sctype : public
	std::ctype<char>
{
	mask my_table[table_size];
public:
	Sctype(size_t refs = 0)
		: std::ctype<char>(&my_table[0], false, refs)
	{
		std::copy_n(classic_table(), table_size, my_table);
		my_table['/'] = (mask)space;
	}
};

bool RenderHandler::loadOBJ(string path) {
	cubeVertices.clear();
	cubeNormals.clear();
	cubeVerticesIndexed.clear();
	cubeNormalsIndexed.clear();
	std::vector< unsigned int > vertexIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec3 > temp_normals;
	std::ifstream file(path);
	if (!file.is_open()) {
		cout << "Failed to open cube file" << endl;
		return false;
	}
	string line;
	while (getline(file, line)) {
		// read the first word of the line
		if (line.substr(0, 2) == "vn") {
			glm::vec3 normal;
			std::istringstream(line.substr(3, line.length() - 3)) >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (line[0] == 'v') {
			glm::vec3 vertex;
			std::istringstream(line.substr(2,line.length()-2)) >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (line[0] == 'f') {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], normalIndex[3];
			int count = 0;
			std::istringstream lineStream(line.substr(2, line.length() - 2));
			std::locale x(std::locale::classic(), new Sctype);
			lineStream.imbue(x);
			lineStream >> vertexIndex[0] >> normalIndex[0] >> vertexIndex[1] >> normalIndex[1] >> vertexIndex[2] >> normalIndex[2];
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}


	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		cubeVertices.push_back(vertex);
		cubeNormals.push_back(normal);
	}

	indexVBO(cubeVertices, cubeNormals, cubeIndices, cubeVerticesIndexed, cubeNormalsIndexed);

	return true;
}


GLuint RenderHandler::LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
	


	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	return ProgramID;
}


RenderHandler::RenderHandler()
{
	init();
	resetCamera();
}


RenderHandler::RenderHandler(string name, string creator, weak_ptr<queue<SEvent>> outputQueue, weak_ptr<sf::RenderWindow> theWindow) : Handler(name, creator, outputQueue), _window(shared_ptr<RenderWindow>(theWindow)){
	init();
	resetCamera();
}


RenderHandler::~RenderHandler()
{
	//Delete everything
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
}


void RenderHandler::drawCube(float x, float y, float z) {
	int shift = (int)simWidth / 2;
	// Get a handle for our "MVP" uniform
	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, -1*z)), glm::vec3(cubeScaling, cubeScaling, cubeScaling));
	// Our ModelViewProjection : multiplication of our 3 matrices
	MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

									 // Send our transformation to the currently bound shader, 
									 // in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
	glm::vec3 cubeColor = cellColours->at((int)x + shift)[(int)y + shift][(int)z + shift];
	glUniform3f(MaterialColorID, cubeColor.x,cubeColor.y,cubeColor.z);

	// Generate a buffer for the indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned short int), &cubeIndices[0], GL_STATIC_DRAW);
	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		cubeIndices.size() * sizeof(unsigned short int),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);
}


void RenderHandler::drawCubes(weak_ptr<vector<vector<vector<int>>>> frame) {
	
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, cubeVerticesIndexed.size() * sizeof(glm::vec3), &cubeVerticesIndexed[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// 2nd attribute buffer : normals
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, cubeNormalsIndexed.size() * sizeof(glm::vec3), &cubeNormalsIndexed[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned short), &cubeIndices[0], GL_STATIC_DRAW);

	glUniform3f(Light1ID, light1Pos.x, light1Pos.y, light1Pos.z);
	glUniform3f(Light2ID, light2Pos.x, light2Pos.y, light2Pos.z);
	glUniform3f(Light3ID, light3Pos.x, light3Pos.y, light3Pos.z);
	glUniform3f(Light4ID, light4Pos.x, light4Pos.y, light4Pos.z);
	glUniform3f(LightColorID, 1, 1, 1);
	glUniform1f(Light1PowerID, light1Power);
	glUniform1f(Light2PowerID, light2Power);
	glUniform1f(Light3PowerID, light3Power);
	glUniform1f(Light4PowerID, light4Power);
	glUniform1f(AmbientBrightnessID, GLfloat(0.8));
	glUniform1f(SpecularBrightnessID, GLfloat(0.3));
	//cout << "STUFF PRINTING HERE" << endl;
	int count = 0;
	int shift = int(frame.lock()->size() / 2);
	for (int x = 0 - shift; x < shift; x++) {
		for (int y = 0 - shift; y < shift; y++) {
			for (int z = 0 - shift; z < shift; z++) {
				if (frame.lock()->at(x + shift)[y + shift][z + shift] > 0) {
					drawCube(float(x), float(y), float(z));
					count++;
				}
			}
		}
	}
}


void RenderHandler::draw() {
	//IS NOT NECESSARY TO REBIND VAO EACH TIME, DON'T DO IT! 
	// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 500.0f);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
	// Use the shader
	glUseProgram(programID);
	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cout << "OpenGL error: " << err << endl;
	}
	if ((!storedFrame.expired()) && storedFrame.lock()->size() > 0) {
		drawCubes(storedFrame);
	}
	GetFirstNMessages(5);
	unbind();
}


void RenderHandler::genColours() {
	light1Power = pow(simWidth, 1.5);;
	light2Power = pow(simWidth, 1.3);
	light3Power = pow(simWidth, 1.3);
	light4Power = pow(simWidth,1.3);
	light1Pos = glm::vec3(simWidth * 0.6, simWidth * 0.6, simWidth * 1);
	light2Pos = glm::vec3(simWidth * -1 * 0.7, simWidth * 0.8, simWidth * -1 * 0.6);
	light3Pos = glm::vec3(simWidth * -1 * 0.6, simWidth * -1 * 0.55, simWidth * 0.7);
	light4Pos = glm::vec3(simWidth * -1 * 0.7, simWidth * -1 * 0.8, simWidth * -1 * 0.9);
	rgbPos.clear();
	rgbPos.push_back(glm::vec3(0,0,0));
	rgbPos.push_back(glm::vec3(simWidth/2, simWidth/2, simWidth));
	rgbPos.push_back(glm::vec3(simWidth, simWidth, 0));
	cellColours = unique_ptr < vector<vector<vector<glm::vec3>>>>(new vector<vector<vector<glm::vec3>>>(simWidth, vector<vector<glm::vec3>>(simWidth, vector<glm::vec3>(simWidth, glm::vec3(1,1,1)))));
	for (int x_ = 0; x_ < simWidth; x_++) {
		for (int y_ = 0; y_ < simWidth; y_++) {
			for (int z_ = 0; z_ < simWidth; z_++) {
				 glm::vec3 newColor = glm::normalize(glm::vec3((simWidth * 2) - glm::distance(glm::vec3(x_, y_, z_), rgbPos[0]), (simWidth * 2) - glm::distance(glm::vec3(x_, y_, z_), rgbPos[1]), (simWidth * 2) - glm::distance(glm::vec3(x_, y_, z_), rgbPos[2])));
				 cellColours->at(x_)[y_][z_] = newColor;
			}
		}
	}
}


void RenderHandler::init()
{
	genColours();
	bool result = loadOBJ("./Resources/cube_bevelled.obj");
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	cubeScaling = 0.4;
	clock2.restart();
	timing = 0;
	height = _window->getSize().y;
	width = _window->getSize().x;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles whose normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, cubeVerticesIndexed.size() & sizeof(glm::vec3), &cubeVerticesIndexed[0][0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, cubeNormalsIndexed.size() * sizeof(glm::vec3), &cubeNormalsIndexed[0][0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned short), &cubeIndices[0], GL_STATIC_DRAW);
	
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading1.vert", "StandardShading1.frag");
	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	cout << "MatrixID: " << MatrixID << endl;
	ModelMatrixID = glGetUniformLocation(programID, "M");
	cout << "ModelMatrixID: " << ModelMatrixID << endl;
	ViewMatrixID = glGetUniformLocation(programID, "V");
	cout << "ViewMatrixID: " << ViewMatrixID << endl;
	Light1ID = glGetUniformLocation(programID, "LightPosition1_worldspace");
	cout << "Light1ID: " << Light1ID << endl;
	Light2ID = glGetUniformLocation(programID, "LightPosition2_worldspace");
	cout << "Light2ID: " << Light2ID << endl;
	Light3ID = glGetUniformLocation(programID, "LightPosition3_worldspace");
	cout << "Light3ID: " << Light3ID << endl;
	Light4ID = glGetUniformLocation(programID, "LightPosition4_worldspace");
	cout << "Light4ID: " << Light4ID << endl;
	MaterialColorID = glGetUniformLocation(programID, "MaterialDiffuseColor");
	cout << "MaterialColorID: " << MaterialColorID << endl;
	LightColorID = glGetUniformLocation(programID, "LightColor");
	cout << "LightColorID: " << LightColorID << endl;
	Light1PowerID = glGetUniformLocation(programID, "Light1Power");
	cout << "Light1PowerID: " << Light1PowerID << endl;
	Light2PowerID = glGetUniformLocation(programID, "Light2Power");
	cout << "Light2PowerID: " << Light2PowerID << endl;
	Light3PowerID = glGetUniformLocation(programID, "Light3Power");
	cout << "Light3PowerID: " << Light3PowerID << endl;
	Light4PowerID = glGetUniformLocation(programID, "Light4Power");
	cout << "Light4PowerID: " << Light4PowerID << endl;
	AmbientBrightnessID = glGetUniformLocation(programID, "AmbientBrightness");
	cout << "AmbientBrightnessID: " << AmbientBrightnessID << endl;
	SpecularBrightnessID = glGetUniformLocation(programID, "SpecularBrightness");
	cout << "SpecularBrightnessID: " << SpecularBrightnessID << endl;
	resetCamera();
}


void RenderHandler::unbind() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VertexArrayID);
}


void RenderHandler::updateH() {
	moveCamera(frameRotation, frameMovement, frameZoom * simWidth);
	frameRotation = sf::Vector2i(0, 0);
	frameMovement = sf::Vector3i(0, 0, 0);
	frameZoom = 0;
	if (closing && running) {
		running = false;
	}
}

void RenderHandler::GetFirstNMessages(GLuint numMsgs)
{
	GLint maxMsgLen = 300;

	std::vector<GLchar> msgData(numMsgs * maxMsgLen * 2);
	std::vector<GLenum> sources(numMsgs * 2);
	std::vector<GLenum> types(numMsgs * 2);
	std::vector<GLenum> severities(numMsgs * 2);
	std::vector<GLuint> ids(numMsgs * 2);
	std::vector<GLsizei> lengths(numMsgs * 2);
	GLuint numFound = glGetDebugMessageLog(numMsgs, msgData.size(), &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);
	sources.resize(numFound);
	types.resize(numFound);
	severities.resize(numFound);
	ids.resize(numFound);
	lengths.resize(numFound);

	std::vector<std::string> messages;
	messages.reserve(numFound);

	std::vector<GLchar>::iterator currPos = msgData.begin();
	for (size_t msg = 0; msg < lengths.size(); ++msg)
	{
		messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
		currPos = currPos + lengths[msg];
	}
	for (auto message : messages) {
		cout << "OpenGL message: " << message << endl;
	}
}

void RenderHandler::handleEventH(SEvent theEvent) {
	if (theEvent._sType == WINDOW_RESIZE) {
		//handle the resizing of the window
	}
	else if (theEvent._sType == WORLD_RESET) {
		resetCamera();
	}
	else if (theEvent._sType == WORLD_MOVE) {
		frameMovement = theEvent._vector3iVal;
	}
	else if (theEvent._sType == WORLD_ZOOM) {
		frameZoom += theEvent._floatVal;
	}
	else if (theEvent._sType == WORLD_ROTATE) {
		frameRotation = theEvent._vectoriVal;
	}
	else if (theEvent._sType == FRAME) {
		if (!theEvent._frame.expired()) {
			storedFrame = theEvent._frame;
			try {
				if (simWidth != storedFrame.lock()->size()) {
					simWidth = int(storedFrame.lock()->size());
					resetCamera();
					genColours();
				}
			}
			catch (exception e) {
				cout << "caught exception before it broke the whole programme" << endl;
			}
		}
	}
}


void RenderHandler::moveCamera(sf::Vector2i rotation, sf::Vector3i translation, float zoom) {
	int deltaTime = clock.getElapsedTime().asMicroseconds();
	clock.restart();
	// As an example, rotate around the vertical axis at 180°/sec
	orientation.x += - 1 * rotation.y * speedMovement * float(deltaTime);
	orientation.y += rotation.x * speedMovement * float(deltaTime);	

	orientation.z = 0;
	// Build the model matrix
	glm::mat4 theRotation = glm::eulerAngleYXZ(orientation.y, orientation.x, orientation.z);


	glm::mat4 forwards = glm::translate(glm::vec3(0, 0, distance));
	glm::mat4 backwards = glm::translate(glm::vec3(0, 0, 0 - distance));
	View = preView * theRotation;

	// Up vector : perpendicular to both direction and right
	up = glm::cross(right, direction);
	FoV -= (speedZoom * zoom * float(deltaTime));
	if (FoV > 180) {
		FoV = 180;
	}
	else if (FoV < 0) {
		FoV = 0;
	}
}


void RenderHandler::resetCamera() {
	if (simWidth > 0) {
		distance = simWidth * 2;
	}
	else {
		distance = 80;
	}
	// Initial Field of View
	FoV = 45.0f;
	// Camera matrix
	preView = glm::lookAt(
		glm::vec3(0, 0, distance),           // Camera is here
		glm::vec3(0, 0, distance) + glm::vec3(0, 0, 0) - glm::vec3(0, 0, distance), // and looks here : at the same position, plus "direction"
		glm::vec3(0, 1, 0)                  // Head is up (set to 0,-1,0 to look upside-down)
	);
	View = preView;
	orientation = glm::vec3(0, 0, 0);
	speedZoom = 0.000022;
	speedMovement = 0.0000006f; // units / second
	mouseSpeed = 0.0017f;
}
#endif
