#include "Demo.h"

Demo::Demo() {
}

Demo::~Demo() {
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Project Akhir Grafika Komputer Kelompok EternalDestiny Kelas A", 1280, 960, false, false);
}

void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildCube();
	//BuildCubeKepala();
	//BuildTangan();
	//BuildKaki();

	BuildPlane();

	BuildDindingKanan();
	BuildDindingKiri();
	BuildDindingDepan();
	BuildDindingBelakang();
	BuildDindingAtas();

	BuildTempatImam();
	BuildKaligrafiAllah();
	BuildKaligrafiMuhammad();

	//mimbar
	BuildMimbarMiddle();
	//BuildMimbarUp();
	BuildMimbarDown();

	//jendela
	BuildJendela();
	BuildJendela2();
	
	//rak
	BuildRak();
	BuildRak2();

	//kipas
	BuildTangkai();
	BuildBaling();

	BuildPintu();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		pindahY += 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		pindahY -= 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		pindahX += 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		pindahX -= 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		pindahZ += 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		pindahZ -= 10 * GetDeltaTime();
	}
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);

}

void Demo::Update(double deltaTime) {
	angle += (float) ((deltaTime * 1.5f) / 100);

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));

	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawCube();
	//DrawCubeKepala();
	//DrawTangan();
	//DrawKaki();

	DrawPlane();

	DrawDindingKanan();
	DrawDindingKiri();
	DrawDindingDepan();
	DrawDindingBelakang();
	DrawDindingAtas();

	DrawTempatImam();
	DrawKaligrafiAllah();
	DrawKaligrafiMuhammad();

	//mimbar
	DrawMimbarMiddle();
	//DrawMimbarUp();
	DrawMimbarDown();

	//jendela
	DrawJendela();
	DrawJendela2();

	//rak
	DrawRak();
	DrawRak2();

	//kipas
	DrawTangkai();
	DrawBaling();

	DrawPintu();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildPintu() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texturePintu);
	glBindTexture(GL_TEXTURE_2D, texturePintu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pintu.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
// format position, tex coords

// left
2.5, -0.1, -9.5, 1, 0, // 12
2.5, -0.1,  -8.5, 0, 0, // 13
2.5,  0.85,  -8.5,  0, 1, // 14
2.5,  0.85, -9.5, 1, 1, // 15

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &pintu);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(pintu);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawPintu()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePintu);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(pintu); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(11.7, 7, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildRak() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureRak);
	glBindTexture(GL_TEXTURE_2D, textureRak);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("amal.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
// format position, tex coords
// front
2.5, -0.5, -8.5, 0, 0,  // 0
3.5, -0.5, -8.5, 0, 1,   // 1
3.5,  0.5, -8.5, 1, 1,   // 2
2.5,  0.5, -8.5, 1, 0,  // 3

// right
3.5,  0.5,  -8.5, 0, 0,  // 4
3.5,  0.5, -9.5, 0, 1,  // 5
3.5, -0.5, -9.5,  1, 1,  // 6
3.5, -0.5,  -8.5,1, 0,  // 7

// back
2.5, -0.5, -9.5, 0, 0, // 8 
3.5,  -0.5, -9.5, 0, 1, // 9
3.5,   0.5, -9.5,  1, 1, // 10
2.5,  0.5, -9.5, 1, 0, // 11

// left
2.5, -0.5, -9.5, 0, 0, // 12
2.5, -0.5,  -8.5,0, 1, // 13
2.5,  0.5,  -8.5,  1, 1, // 14
2.5,  0.5, -9.5, 1, 0, // 15

// upper
3.5, 0.5,  -8.5, 0, 0,   // 16
2.5, 0.5,  -8.5, 0, 1,  // 17
2.5, 0.5, -9.5,  1, 1,  // 18
3.5, 0.5, -9.5, 1, 0,  // 19

// bottom
2.5, -0.5, -9.5, 0, 0, // 20
3.5, -0.5, -9.5,0, 1,  // 21
3.5, -0.5,  -8.5,  1, 1,  // 22
2.5, -0.5,  -8.5,1, 0, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &rak);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(rak);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawRak()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureRak);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(rak); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(4, 5, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildRak2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureRak2);
	glBindTexture(GL_TEXTURE_2D, textureRak2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("amal.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
// format position, tex coords
// front
-2, -0.5, -8.5, 0, 0,  // 0
-1, -0.5, -8.5, 0, 1,   // 1
-1,  0.5, -8.5, 1, 1,   // 2
-2,  0.5, -8.5, 1, 0,  // 3

// right
-1,  0.5,  -8.5, 0, 0,  // 4
-1,  0.5, -9.5, 0, 1, // 5
-1, -0.5, -9.5, 1, 1,  // 6
-1, -0.5,  -8.5,1, 0, // 7

// back
-2, -0.5, -9.5, 0, 0, // 8 
-1,  -0.5, -9.5, 0, 1, // 9
-1,   0.5, -9.5, 1, 1, // 10
-2,  0.5, -9.5, 1, 0, // 11

// left
-2, -0.5, -9.5, 0, 0, // 12
-2, -0.5,  -8.5, 0, 1, // 13
-2,  0.5,  -8.5, 1, 1, // 14
-2,  0.5, -9.5, 1, 0, // 15

// upper
-1, 0.5,  -8.5, 0, 0,   // 16
-2, 0.5,  -8.5,0, 1,  // 17
-2, 0.5, -9.5, 1, 1,  // 18
-1, 0.5, -9.5, 1, 0,   // 19

// bottom
-2, -0.5, -9.5, 0, 0, // 20
-1, -0.5, -9.5, 0, 1,  // 21
-1, -0.5,  -8.5, 1, 1,  // 22
-2, -0.5,  -8.5, 1, 0, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &rak2);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(rak2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawRak2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureRak2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(rak2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(4, 5, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildJendela() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureJD);
	glBindTexture(GL_TEXTURE_2D, textureJD);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("window3.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
// format position, tex coords
// front
-4.5, 1.5, 29.2, 0, 1,  // 0
-3.5, 1.5, 29.2, 1, 1,   // 1
-3.5,  0.5, 29.2, 1, 0,   // 2
-4.5,  0.5, 29.2, 0, 0,  // 3

	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &jendela);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(jendela);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawJendela()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureJD);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(jendela); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(6, 6, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildJendela2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureJD2);
	glBindTexture(GL_TEXTURE_2D, textureJD2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("window3.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
// format position, tex coords
// front
4.5, 1.5, 29.2, 0, 1,  // 0
3.5, 1.5, 29.2, 1, 1,   // 1
3.5,  0.5, 29.2, 1, 0,   // 2
4.5,  0.5, 29.2, 0, 0,  // 3

	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &jendela2);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(jendela2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawJendela2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureJD2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(jendela2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(6, 6, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTangkai() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureTangkai);
	glBindTexture(GL_TEXTURE_2D, textureTangkai);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woods.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
// format position, tex coords
// front
-0.1, 6, 0.1, 0, 0,  // 0
0.1, 6, 0.1, 1, 0,   // 1
0.1, 7.5, 0.1, 1, 1,   // 2
-0.1, 7.5, 0.1, 0, 1,  // 3

// right
0.1,  7,  0.1, 0, 0,  // 4
0.1,  7, -0.1, 0.3, 0.3,  // 5
0.1,  5.5, -0.1, 0.3, 0.3,  // 6
0.1,  5.5,  0.1, 0, 0.3,  // 7

// back
-0.1, 5.5, -0.1, 0, 0, // 8 
0.1, 5.5, -0.1, 0.3, 0.3, // 9
0.1, 7, -0.1, 0.3, 0.3, // 10
-0.1, 7, -0.1, 0.3, 0.3, // 11

// left
-0.1, 5.5, -0.1, 0, 0, // 12
-0.1, 5.5,  0.1, 0.3, 0.3, // 13
-0.1, 7,  0.1, 0.3, 0.3, // 14
-0.1, 7, -0.1, 0.3, 0.3, // 15

// upper
0.1, 7,  0.1, 0, 0,   // 16
-0.1, 7,  0.1, 0.3, 0.3,  // 17
-0.1, 7, -0.1, 0.3, 0.3,  // 18
0.1, 7, -0.1, 0.3, 0.3,   // 19

// bottom
-0.1, 5.5, -0.1, 0.3, 0.3, // 20
0.1, 5.5, -0.1, 0.3, 0.3,  // 21
0.1, 5.5,  0.1, 0.3, 0.3,  // 22
-0.1, 5.5,  0.1, 0.3, 0.3, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &tangkai);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(tangkai);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTangkai()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTangkai);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(tangkai); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 1.5, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildBaling() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureBaling);
	glBindTexture(GL_TEXTURE_2D, textureBaling);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("keramikputih.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
// format position, tex coords
// front
-1.5, 5.47, 0.2, 0, 0,  // 0
1.5, 5.47, 0.2, 1, 0,   // 1
1.5, 5.5, 0.2, 1, 1,   // 2
-1.5, 5.5, 0.2, 0, 1,  // 3

// right
1.5, 5.5,  0.2, 0, 0,  // 4
1.5, 5.5, -0.2, 0.3, 0.3,  // 5
1.5, 5.47, -0.2, 0.3, 0.3,  // 6
1.5, 5.47,  0.2, 0, 0.3,  // 7

// back
-1.5,  5.47, -0.2, 0, 0, // 8 
1.5, 5.47, -0.2, 0.3, 0.3, // 9
1.5,  5.5, -0.2, 0.3, 0.3, // 10
-1.5,  5.5, -0.2, 0.3, 0.3, // 11

// left
-1.5, 5.47, -0.2, 0, 0, // 12
-1.5, 5.47,  0.2, 0.3, 0.3, // 13
-1.5, 5.5,  0.2, 0.3, 0.3, // 14
-1.5, 5.5, -0.2, 0.3, 0.3, // 15

// upper
1.5, 5.5,  0.2, 0, 0,   // 16
-1.5, 5.5,  0.2, 0.3, 0.3,  // 17
-1.5, 5.5, -0.2, 0.3, 0.3,  // 18
1.5, 5.5, -0.2, 0.3, 0.3,   // 19

// bottom
-1.5, 5.47, -0.2, 0.3, 0.3, // 20
1.5, 5.47, -0.2, 0.3, 0.3,  // 21
1.5, 5.47,  0.2, 0.3, 0.3,  // 22
-1.5, 5.47,  0.2, 0.3, 0.3, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &baling);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(baling);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawBaling()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBaling);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(baling); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 1.5, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("body.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
// format position, tex coords
// front
-0.5, -0.5, -8.5, 0, 0,  // 0
0.5, -0.5, -8.5, 1, 0,   // 1
0.5,  0.5, -8.5, 1, 1,   // 2
-0.5,  0.5, -8.5, 0, 1,  // 3

// right
0.5,  0.5,  -8.5, 0, 0,  // 4
0.5,  0.5, -9, 1, 0,  // 5
0.5, -0.5, -9, 1, 1,  // 6
0.5, -0.5,  -8.5, 0, 1,  // 7

// back
-0.5, -0.5, -9, 0, 0, // 8 
0.5,  -0.5, -9, 1, 0, // 9
0.5,   0.5, -9, 1, 1, // 10
-0.5,  0.5, -9, 0, 1, // 11

// left
-0.5, -0.5, -9, 0, 0, // 12
-0.5, -0.5,  -8.5, 1, 0, // 13
-0.5,  0.5,  -8.5, 1, 1, // 14
-0.5,  0.5, -9, 0, 1, // 15

// upper
0.5, 0.5,  -8.5, 0, 0,   // 16
-0.5, 0.5,  -8.5, 1, 0,  // 17
-0.5, 0.5, -9, 1, 1,  // 18
0.5, 0.5, -9, 0, 1,   // 19

// bottom
-0.5, -0.5, -9, 0, 0, // 20
0.5, -0.5, -9, 1, 0,  // 21
0.5, -0.5,  -8.5, 1, 1,  // 22
-0.5, -0.5,  -8.5, 0, 1, // 23
	};

	unsigned int indices[] = {
		8,  9,  10, 8,  10, 11,  // back
		20, 22, 21, 20, 23, 22,   // bottom
		4,  5,  6,  4,  6,  7,   // right
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		0,  1,  2,  0,  2,  3   // front

	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//void Demo::BuildCubeKepala() {
//	// load image into texture memory
//	// ------------------------------
//	// Load and create a texture 
//	glGenTextures(1, &textureAtas);
//	glBindTexture(GL_TEXTURE_2D, textureAtas);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	int width, height;
//	unsigned char* image = SOIL_load_image("danboface.png", &width, &height, 0, SOIL_LOAD_RGBA);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//	SOIL_free_image_data(image);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	// set up vertex data (and buffer(s)) and configure vertex attributes
//	// ------------------------------------------------------------------
//	float vertices[] = {
//		// format position, tex coords
//		// front
//		-0.5, 0.5, 0.5, 0, 0,  // 0
//		0.5, 0.5, 0.5, 1, 0,   // 1
//		0.5,  1.5, 0.5, 1, 1,   // 2
//		-0.5,  1.5, 0.5, 0, 1,  // 3
//
//		// right
//		0.5,  1.5,  0.5, 0, 0,  // 4
//		0.5,  1.5, -0.5, 0.3, 0.3,  // 5
//		0.5, 0.5, -0.5, 0.3, 0.3,  // 6
//		0.5, 0.5,  0.5, 0, 0.3,  // 7
//
//		// back
//		-0.5, 0.5, -0.5, 0, 0, // 8 
//		0.5,  0.5, -0.5, 0.3, 0.3, // 9
//		0.5,   1.5, -0.5, 0.3, 0.3, // 10
//		-0.5,  1.5, -0.5, 0.3, 0.3, // 11
//
//		// left
//		-0.5, 0.5, -0.5, 0, 0, // 12
//		-0.5, 0.5,  0.5, 0.3, 0.3, // 13
//		-0.5,  1.5,  0.5, 0.3, 0.3, // 14
//		-0.5,  1.5, -0.5, 0.3, 0.3, // 15
//
//		// upper
//		0.5, 1.5,  0.5, 0, 0,   // 16
//		-0.5, 1.5,  0.5, 0.3, 0.3,  // 17
//		-0.5, 1.5, -0.5, 0.3, 0.3,  // 18
//		0.5, 1.5, -0.5, 0.3, 0.3,   // 19
//
//		// bottom
//		-0.5, 0.5, -0.5, 0.3, 0.3, // 20
//		0.5, 0.5, -0.5, 0.3, 0.3,  // 21
//		0.5, 0.5,  0.5, 0.3, 0.3,  // 22
//		-0.5, 0.5,  0.5, 0.3, 0.3, // 23
//	};
//
//	unsigned int indices[] = {
//		0,  1,  2,  0,  2,  3,   // front
//		4,  5,  6,  4,  6,  7,   // right
//		8,  9,  10, 8,  10, 11,  // back
//		12, 14, 13, 12, 15, 14,  // left
//		16, 18, 17, 16, 19, 18,  // upper
//		20, 22, 21, 20, 23, 22   // bottom
//	};
//
//	glGenVertexArrays(1, &atas);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(atas);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	// define position pointer layout 0
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(0);
//
//	// define texcoord pointer layout 1
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//
//	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//	glBindVertexArray(0);
//
//	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//}
//
//void Demo::DrawCubeKepala()
//{
//	glUseProgram(shaderProgram);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textureAtas);
//	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
//
//	glBindVertexArray(atas); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//
//	glm::mat4 model;
//	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));
//
//	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));
//
//	model = glm::scale(model, glm::vec3(2, 1.5, 2));
//
//	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glBindVertexArray(0);
//}
//
//void Demo::BuildTangan() {
//	// load image into texture memory
//	// ------------------------------
//	// Load and create a texture 
//	glGenTextures(1, &textureTangan);
//	glBindTexture(GL_TEXTURE_2D, textureTangan);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	int width, height;
//	unsigned char* image = SOIL_load_image("danboface.png", &width, &height, 0, SOIL_LOAD_RGBA);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//	SOIL_free_image_data(image);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	// set up vertex data (and buffer(s)) and configure vertex attributes
//	// ------------------------------------------------------------------
//	float vertices[] = {
//		// format position, tex coords
//		// front
//		-0.5, 0, 0.5, 0, 0,  // 0
//		0.5, 0, 0.5, 0.3, 0.3,   // 1
//		0.5,  0.5, 0.5, 0.3, 0.3,   // 2
//		-0.5,  0.5, 0.5, 0.3, 0.3,  // 3
//
//		// right
//		0.5,  0.5,  0.5, 0, 0,  // 4
//		0.5,  0.5, -0.5, 0.3, 0.3,  // 5
//		0.5, 0, -0.5, 0.3, 0.3,  // 6
//		0.5, 0,  0.5, 0.3, 0.3,  // 7
//
//		// back
//		-0.5, 0, -0.5, 0, 0, // 8 
//		0.5,  0, -0.5, 0.3, 0.3, // 9
//		0.5,   0.5, -0.5, 0.3, 0.3, // 10
//		-0.5,  0.5, -0.5, 0.3, 0.3, // 11
//
//		// left
//		-0.5, 0, -0.5, 0, 0, // 12
//		-0.5, 0,  0.5, 0.3, 0.3, // 13
//		-0.5,  0.5,  0.5, 0.3, 0.3, // 14
//		-0.5,  0.5, -0.5, 0.3, 0.3, // 15
//
//		// upper
//		0.5, 0.5,  0.5, 0, 0,   // 16
//		-0.5, 0.5,  0.5, 0.3, 0.3,  // 17
//		-0.5, 0.5, -0.5, 0.3, 0.3,  // 18
//		0.5, 0.5, -0.5, 0.3, 0.3,   // 19
//
//		// bottom
//		-0.5, 0, -0.5, 0, 0, // 20
//		0.5, 0, -0.5, 0.3, 0.3,  // 21
//		0.5, 0,  0.5, 0.3, 0.3,  // 22
//		-0.5, 0,  0.5, 0.3, 0.3, // 23
//	};
//
//	unsigned int indices[] = {
//		0,  1,  2,  0,  2,  3,   // front
//		4,  5,  6,  4,  6,  7,   // right
//		8,  9,  10, 8,  10, 11,  // back
//		12, 14, 13, 12, 15, 14,  // left
//		16, 18, 17, 16, 19, 18,  // upper
//		20, 22, 21, 20, 23, 22   // bottom
//	};
//
//	glGenVertexArrays(1, &tangan);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(tangan);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	// define position pointer layout 0
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(0);
//
//	// define texcoord pointer layout 1
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//
//	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//	glBindVertexArray(0);
//
//	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//}
//
//void Demo::DrawTangan()
//{
//	glUseProgram(shaderProgram);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textureTangan);
//	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
//
//	glBindVertexArray(tangan); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//
//	glm::mat4 model;
//	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));
//
//	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));
//
//	model = glm::scale(model, glm::vec3(3, 0.5, 0.5));
//
//	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glBindVertexArray(0);
//}
//
//void Demo::BuildKaki() {
//	// load image into texture memory
//	// ------------------------------
//	// Load and create a texture 
//	glGenTextures(1, &textureBawah);
//	glBindTexture(GL_TEXTURE_2D, textureBawah);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	int width, height;
//	unsigned char* image = SOIL_load_image("danboface.png", &width, &height, 0, SOIL_LOAD_RGBA);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//	SOIL_free_image_data(image);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	// set up vertex data (and buffer(s)) and configure vertex attributes
//	// ------------------------------------------------------------------
//	float vertices[] = {
//		// format position, tex coords
//		// front
//		-0.5, -2.5, 0.5, 0, 0,  // 0
//		0.5, -2.5, 0.5, 0.3, 0.3,   // 1
//		0.5,  -1.5, 0.5, 0.3, 0.3,   // 2
//		-0.5,  -1.5, 0.5, 0.3, 0.3,  // 3
//
//		// right
//		0.5,  -1.5,  0.5, 0, 0,  // 4
//		0.5,  -1.5, -0.5, 0.3, 0.3,  // 5
//		0.5, -2.5, -0.5, 0.3, 0.3,  // 6
//		0.5, -2.5,  0.5, 0.3, 0.3,  // 7
//
//		// back
//		-0.5, -2.5, -0.5, 0, 0, // 8 
//		0.5,  -2.5, -0.5, 0.3, 0.3, // 9
//		0.5,   -1.5, -0.5, 0.3, 0.3, // 10
//		-0.5,  -1.5, -0.5, 0.3, 0.3, // 11
//
//		// left
//		-0.5, -2.5, -0.5, 0, 0, // 12
//		-0.5, -2.5,  0.5, 0.3, 0.3, // 13
//		-0.5,  -1.5,  0.5, 0.3, 0.3, // 14
//		-0.5,  -1.5, -0.5, 0.3, 0.3, // 15
//
//		// upper
//		0.5, -1.5,  0.5, 0, 0,   // 16
//		-0.5, -1.5,  0.5, 0.3, 0.3,  // 17
//		-0.5, -1.5, -0.5, 0.3, 0.3,  // 18
//		0.5, -1.5, -0.5, 0.3, 0.3,   // 19
//
//		// bottom
//		-0.5, -2.5, -0.5, 0, 0, // 20
//		0.5, -2.5, -0.5, 0.3, 0.3,  // 21
//		0.5, -2.5,  0.5, 0.3, 0.3,  // 22
//		-0.5, -2.5,  0.5, 0.3, 0.3, // 23
//	};
//	unsigned int indices[] = {
//		0,  1,  2,  0,  2,  3,   // front
//		4,  5,  6,  4,  6,  7,   // right
//		8,  9,  10, 8,  10, 11,  // back
//		12, 14, 13, 12, 15, 14,  // left
//		16, 18, 17, 16, 19, 18,  // upper
//		20, 22, 21, 20, 23, 22   // bottom
//	};
//
//	glGenVertexArrays(1, &bawah);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(bawah);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	// define position pointer layout 0
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(0);
//
//	// define texcoord pointer layout 1
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//
//	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//	glBindVertexArray(0);
//
//	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//}
//
//void Demo::DrawKaki()
//{
//	glUseProgram(shaderProgram);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textureBawah);
//	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
//
//	glBindVertexArray(bawah); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//
//	glm::mat4 model;
//	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));
//
//	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));
//
//	model = glm::scale(model, glm::vec3(1.5, 0.5, 3));
//
//	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glBindVertexArray(0);
//}

void Demo::BuildPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("karpet.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 15,  0,
		 50.0, -0.5,  50.0, 15, 15,
		-50.0, -0.5,  50.0,  0, 15,
		//-50.0, -0.5, -50.0,  0,  0,
		//50.0, -0.5, -50.0, 1, 0,
		//50.0, -0.5,  50.0, 1, 1,
		//-50.0, -0.5,  50.0,  0, 1,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDindingAtas()
{
	// Load and create a texture 
	glGenTextures(1, &textureDA);
	glBindTexture(GL_TEXTURE_2D, textureDA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("motif.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, 10.4, -50.0,  0,  0,
		50.0, 10.4, -50.0, 0,  5,
		50.0, 10.4,  50.0, 5, 5,
		-50.0, 10.4,  50.0,  0, 5,
		//-50.0, -0.5, -50.0,  0,  0,
		//50.0, -0.5, -50.0, 1, 0,
		//50.0, -0.5,  50.0, 1, 1,
		//-50.0, -0.5,  50.0,  0, 1,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &dindingAtas);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(dindingAtas);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawDindingAtas()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureDA);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(dindingAtas); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDindingKanan() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDK);
	glBindTexture(GL_TEXTURE_2D, textureDK);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		29.5, -0.5, 0.5, 0, 5,  // 0
		30.5, -0.5, 0.5, 0, 0,   // 1
		30.5,  0.5, 0.5, 5, 0,   // 2
		29.5,  0.5, 0.5, 5, 5,  // 3

		// right
		30.5,  0.5,  0.5, 5, 5,  // 4
		30.5,  0.5, -0.5, 0, 5,  // 5
		30.5, -0.5, -0.5, 0, 0,  // 6
		30.5, -0.5,  0.5, 5, 0,  // 7

		// back
		29.5, -0.5, -0.5, 0, 5, // 8 
		30.5,  -0.5, -0.5, 0, 0, // 9
		30.5,   0.5, -0.5, 5, 0, // 10
		29.5,  0.5, -0.5, 5, 5, // 11

		// left *
		29.5, -0.5, -0.5, 2.19, 0, // 12
		29.5, -0.5,  0.5, 0, 0, // 13
		29.5,  0.5,  0.5, 0, 2.19, // 14
		29.5,  0.5, -0.5, 2.19, 2.19, // 15

		// upper
		30.5, 0.5,  0.5, 0, 5,   // 16
		29.5, 0.5,  0.5, 0, 0,  // 17
		29.5, 0.5, -0.5, 5, 0,  // 18
		30.5, 0.5, -0.5, 5, 5,   // 19

		// bottom
		29.5, -0.5, -0.5, 0, 5, // 20
		30.5, -0.5, -0.5, 0, 0,  // 21
		30.5, -0.5,  0.5, 5, 0,  // 22
		29.5, -0.5,  0.5,  5, 5, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &dindingKanan);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dindingKanan);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawDindingKanan()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDK);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(dindingKanan); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 25, 60));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDindingKiri() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDKiri);
	glBindTexture(GL_TEXTURE_2D, textureDKiri);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-29.5, -0.5, 0.5, 0, 0,  // 0
		-30.5, -0.5, 0.5, 0, 10,   // 1
		-30.5,  0.5, 0.5, 10, 10,   // 2
		-29.5,  0.5, 0.5, 10, 0,  // 3

		// right 
		-30.5,  0.5,  0.5, 2.19, 0,  // 4
		-30.5,  0.5, -0.5, 0, 0, // 5
		-30.5, -0.5, -0.5, 0, 2.19,  // 6
		-30.5, -0.5,  0.5, 2.19, 2.19,  // 7

		// back
		-29.5, -0.5, -0.5, 0, 0, // 8 
		-30.5,  -0.5, -0.5, 0, 10, // 9
		-30.5,   0.5, -0.5, 10, 10, // 10
		-29.5,  0.5, -0.5, 10, 0, // 11

		// left
		-29.5, -0.5, -0.5, 2.19, 0, // 12
		-29.5, -0.5,  0.5, 0, 0, // 13
		-29.5,  0.5,  0.5, 0, 2.19, // 14
		-29.5,  0.5, -0.5, 2.19, 2.19, // 15

		// upper
		-30.5, 0.5,  0.5, 0, 0,   // 16
		-29.5, 0.5,  0.5, 0, 10,  // 17
		-29.5, 0.5, -0.5, 10, 10,  // 18
		-30.5, 0.5, -0.5, 10, 0,  // 19

		// bottom
		-29.5, -0.5, -0.5, 0, 0, // 20
		-30.5, -0.5, -0.5, 0, 10,  // 21
		-30.5, -0.5,  0.5, 10, 10,  // 22
		-29.5, -0.5,  0.5, 10, 0, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &dindingKiri);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dindingKiri);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawDindingKiri()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDKiri);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(dindingKiri); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 25, 60));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDindingDepan() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDD);
	glBindTexture(GL_TEXTURE_2D, textureDD);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, -0.5, -30.5, 0, 0,  // 0
0.5, -0.5, -30.5, 0, 1,   // 1
0.5,  0.5, -30.5, 1, 1,   // 2
-0.5,  0.5, -30.5, 0, 1,  // 3

// right
0.5,  0.5,  -30.5, 0, 0,  // 4
0.5,  0.5, -29.5, 0, 1, // 5
0.5, -0.5, -29.5, 1, 1,  // 6
0.5, -0.5,  -30.5, 0, 1,  // 7

// back *
-0.5, -0.5, -29.5, 2.19, 0, // 8 
0.5,  -0.5, -29.5,  0, 0, // 9
0.5,   0.5, -29.5, 0, 2.19, // 10
-0.5,  0.5, -29.5, 2.19, 2.19, // 11

// left
-0.5, -0.5, -29.5, 0, 0, // 12
-0.5, -0.5,  -30.5, 0, 1, // 13
-0.5,  0.5,  -30.5, 1, 1, // 14
-0.5,  0.5, -29.5, 0, 1, // 15

// upper
0.5, 0.5,  -30.5, 0, 0,   // 16
-0.5, 0.5,  -30.5, 0, 1,  // 17
-0.5, 0.5, -29.5, 1, 1,  // 18
0.5, 0.5, -29.5, 0, 1,   // 19

// bottom
-0.5, -0.5, -29.5, 0, 0, // 20
0.5, -0.5, -29.5, 0, 1, // 21
0.5, -0.5,  -30.5, 1, 1,  // 22
-0.5, -0.5,  -30.5, 0, 1, // 23
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &dindingDepan);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dindingDepan);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawDindingDepan()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDD);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(dindingDepan); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(72, 25, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDindingBelakang() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDB);
	glBindTexture(GL_TEXTURE_2D, textureDB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 30.5, 2.19, 0,  // 0
		0.5, -0.5, 30.5, 0, 0,   // 1
		0.5,  0.5, 30.5, 0, 2.19,   // 2
		-0.5,  0.5, 30.5, 2.19, 2.19,  // 3

		// right
		0.5,  0.5,  30.5, 0, 0,  // 4
		0.5,  0.5, 29.5, 0, 5,  // 5
		0.5, -0.5, 29.5, 5, 5, // 6
		0.5, -0.5,  30.5, 5, 0, // 7

		// back *
		-0.5, -0.5, 29.5, 2.19, 0, // 8 
		0.5,  -0.5, 29.5, 0, 0, // 9
		0.5,   0.5, 29.5, 0, 2.19, // 10
		-0.5,  0.5, 29.5, 2.19, 2.19, // 11

		// left
		-0.5, -0.5, 29.5, 0, 0, // 12
		-0.5, -0.5,  30.5, 0, 5, // 13
		-0.5,  0.5,  30.5,5, 5, // 14
		-0.5,  0.5, 29.5, 5, 0, // 15

		// upper
		0.5, 0.5,  30.5, 0, 0,   // 16
		-0.5, 0.5,  30.5, 0, 5,  // 17
		-0.5, 0.5, 29.5, 5, 5, // 18
		0.5, 0.5, 29.5, 5, 0,   // 19

		// bottom
		-0.5, -0.5, 29.5, 0, 0, // 20
		0.5, -0.5, 29.5, 0, 5,  // 21
		0.5, -0.5,  30.5, 5, 5,  // 22
		-0.5, -0.5,  30.5, 5, 0, // 23
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &dindingBelakang);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dindingBelakang);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawDindingBelakang()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDB);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(dindingBelakang); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(72, 25, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTempatImam() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureTI);
	glBindTexture(GL_TEXTURE_2D, textureTI);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("depan.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.1, 29.5, 1, 0,  // 0
		0.5, -0.1, 29.5, 0, 0,   // 1
		0.5,  0.9, 29.5, 1, 1,   // 2
		-0.5,  0.9, 29.5, 1, 0,  // 3

		// right
		0.5,  0.9,  29.5, 1, 0,  // 4
		0.5,  0.9, 28.5, 0, 0,  // 5
		0.5, -0.1, 28.5, 0, 1, // 6
		0.5, -0.1,  29.5, 1, 1, // 7

		// back 
		-0.5, -0.1, 28.5, 1, 0, // 8 
		0.5,  -0.1, 28.5, 0, 0, // 9
		0.5,   0.9, 28.5, 0, 1, // 10
		-0.5,  0.9, 28.5, 1, 1, // 11

		// left
		-0.5, -0.1, 28.5,1, 0, // 12
		-0.5, -0.1,  29.5, 0, 0, // 13
		-0.5,  0.9,  29.5,0, 1, // 14
		-0.5,  0.9, 28.5, 1, 1, // 15

		// upper
		0.5, 0.9,  29.5, 1, 0,   // 16
		-0.5, 0.9,  29.5,0, 0,  // 17
		-0.5, 0.9, 28.5, 0, 1, // 18
		0.5, 0.9, 28.5, 1, 1,   // 19

		// bottom
		-0.5, -0.1, 28.5, 1, 0, // 20
		0.5, -0.1, 28.5, 0, 0,  // 21
		0.5, -0.1,  29.5,0, 1,  // 22
		-0.5, -0.1,  29.5, 1, 1, // 23
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &tempatImam);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(tempatImam);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTempatImam()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTI);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(tempatImam); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(10, 10, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKaligrafiAllah() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKA);
	glBindTexture(GL_TEXTURE_2D, textureKA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Allah.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-3, 1.5, 29.5, 0, 0,  // 0
		-2, 1.5, 28.5, 0, 0,   // 1
		-2,  0.5, 28.5, 0, 0,   // 2
		-3,  0.5, 29.5, 0, 0,  // 3

		// right
		-2, 1.5,  29.5, 0, 0,  // 4
		-3, 1.5, 28.5, 0, 0,  // 5
		-3,  0.5, 28.5, 0, 0, // 6
		-2,  0.5,  29.5, 0, 0, // 7

		// back 
		-3, 0.5, 28.5, 1, 0, // 8 
		-2,  0.5, 28.5,0, 0, // 9
		-2,   1.5, 28.5, 0, 1, // 10
		-3,  1.5, 28.5, 1, 1, // 11

		// left
		-2, 0.5, 28.5, 0, 0, // 12
		-2, 0.5,  29.5, 0, 0, // 13
		-2,  1.5,  29.5,0, 0, // 14
		-2,  1.5, 28.5, 0, 0, // 15

		// upper
		-3, 1.5,  29.5, 0, 0,   // 16
		-2, 1.5,  29.5,0, 0,  // 17
		-2, 1.5, 28.5, 0, 0, // 18
		-3, 1.5, 28.5, 0, 0,   // 19

		// bottom
		-2, 0.5, 28.5, 0, 0, // 20
		-3, 0.5, 28.5, 0, 0,  // 21
		-3,  0.5,  29.5, 0, 0,  // 22
		-2,  0.5,  29.5, 0, 0, // 23

	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &kaligrafiAllah);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(kaligrafiAllah);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKaligrafiAllah()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKA);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(kaligrafiAllah); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(6, 6, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKaligrafiMuhammad() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKM);
	glBindTexture(GL_TEXTURE_2D, textureKM);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Muhammad.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		2, 1.5, 29.5, 0, 0,  // 0
		3, 1.5, 28.5, 0, 0,   // 1
		3,  0.5, 28.5, 0, 0,   // 2
		2,  0.5, 29.5, 0, 0,  // 3

		// right
		2, 1.5,  29.5, 0, 0,  // 4
		3, 1.5, 28.5, 0, 0,  // 5
		3,  0.5, 28.5, 0, 0, // 6
		2,  0.5,  29.5, 0, 0, // 7

		// back 
		2, 0.5, 28.5, 1, 0, // 8 
		3,  0.5, 28.5,0, 0, // 9
		3,   1.5, 28.5, 0, 1, // 10
		2,  1.5, 28.5, 1, 1, // 11

		// left
		2, 0.5, 28.5, 0, 0, // 12
		2, 0.5,  29.5, 0, 0, // 13
		2,  1.5,  29.5,0, 0, // 14
		2,  1.5, 28.5, 0, 0, // 15

		// upper
		3, 1.5,  29.5, 0, 0,   // 16
		2, 1.5,  29.5,0, 0,  // 17
		2, 1.5, 28.5, 0, 0, // 18
		3, 1.5, 28.5, 0, 0,   // 19

		// bottom
		2, 0.5, 28.5, 0, 0, // 20
		3, 0.5, 28.5, 0, 0,  // 21
		3,  0.5,  29.5, 0, 0,  // 22
		2,  0.5,  29.5, 0, 0, // 23
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &kaligrafiMuhammad);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(kaligrafiMuhammad);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKaligrafiMuhammad()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKM);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(kaligrafiMuhammad); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(6, 6, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildMimbarMiddle() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureMiddle);
	glBindTexture(GL_TEXTURE_2D, textureMiddle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("allah4.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-4.5,  0, 12.5, 0, 0,  // 0
		-2.5,  0, 12.5, 0.3, 0.3,   // 1
		-2.5,  2.25, 12.5, 0.3, 0.3,   // 2
		-4.5,  2.25, 12.5, 0.3, 0.3,  // 3

		// right
		-2.5,  3.25,  12.5, 0, 0,  // 4
		-2.5,  3.25, 11.5, 0.3, 0.3,  // 5
		-2.5,  0, 11.5, 0.3, 0.3,  // 6
		-2.5,  0,  12.5, 0.3, 0.3,  // 7

		// back
		-4.5,   0, 11.5, 1, 0, // 8 
		-2.5,   0, 11.5, 0, 0, // 9
		-2.5,   3.25, 11.5, 0, 1, // 10
		-4.5,   3.25, 11.5, 1, 1, // 11

		// left
		-4.5,  0, 11.5, 0, 0, // 12
		-4.5,  0,  12.5, 0.3, 0.3, // 13
		-4.5,  3.25,  12.5, 0.3, 0.3, // 14
		-4.5,  3.25, 11.5, 0.3, 0.3, // 15

		// upper
		-2.5, 3.75,  12.5, 0, 0,   // 16
		-4.5, 3.75,  12.5, 0.3, 0.3,  // 17
		-4.5, 3.25, 11.5, 0.3, 0.3,  // 18
		-2.5, 3.25, 11.5, 0.3, 0.3,   // 19

		// bottom
		-4.5,  0, 11.5, 0.3, 0.3, // 20
		-2.5,  0, 11.5, 0.3, 0.3,  // 21
		-2.5,  0,  12.5, 0.3, 0.3,  // 22
		-4.5,  0,  12.5, 0.3, 0.3, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 21, 22, 20, 22, 23   // bottom
	};

	glGenVertexArrays(1, &middle);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(middle);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawMimbarMiddle()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureMiddle);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(middle); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 1.5, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//void Demo::BuildMimbarUp() {
//	// load image into texture memory
//	// ------------------------------
//	// Load and create a texture 
//	glGenTextures(1, &textureUp);
//	glBindTexture(GL_TEXTURE_2D, textureUp);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	int width, height;
//	unsigned char* image = SOIL_load_image("allah4.png", &width, &height, 0, SOIL_LOAD_RGBA);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//	SOIL_free_image_data(image);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	// set up vertex data (and buffer(s)) and configure vertex attributes
//	// ------------------------------------------------------------------
//	float vertices[] = {
//		// format position, tex coords
//		// front
//		-6.0,  4,  12.75, 0, 0,  // 0
//		-3.0,  4,  12.75, 0, 0,   // 1
//		-3.0,  4, 12.75, 0, 0,   // 2
//		-6.0,  4, 12.75, 0, 0,  // 3
//
//		// right
//		-3.0,  4,  12.75, 0, 0,  // 4
//		-3.0,  3.5, 11.5, 0.3, 0.3,  // 5
//		-3.0,  3.5,  11.5, 0.3, 0.3,  // 6
//		-3.0,  4,   12.75, 0, 0.3,  // 7
//
//		// back
//		-6.0,  3.5,  11.5, 0, 0, // 8 
//		-3.0,  3.5,  11.5, 0.3, 0.3, // 9
//		-3.0,  3.5, 11.5, 0.3, 0.3, // 10
//		-6.0,  3.5, 11.5, 0.3, 0.3, // 11
//
//		// left
//		-6.0,  3.5,  11.5, 0, 0, // 12
//		-6.0,  4,   12.75, 0.3, 0.3, // 13
//		-6.0,  4,  12.75, 0.3, 0.3, // 14
//		-6.0,  3.5, 11.5, 0.3, 0.3, // 15
//
//		// upper
//		-3.0, 4,  12.75, 0, 0,   // 16
//		-6.0, 4,  12.75, 0.3, 0.3,  // 17
//		-6.0, 3.5, 11.5, 0.3, 0.3,  // 18
//		-3.0, 3.5, 11.5, 0.3, 0.3,   // 19
//
//		// bottom
//		-6.0, 3.5, 11.5, 0.3, 0.3, // 20
//		-3.0, 3.5, 11.5, 0.3, 0.3,  // 21
//		-3.0, 4.0,  12.75, 0.3, 0.3,  // 22
//		-6.0, 4.0,  12.75, 0.3, 0.3, // 23
//
//
//	};
//
//	unsigned int indices[] = {
//		0,  1,  2,  0,  2,  3,   // front
//		4,  5,  6,  4,  6,  7,   // right
//		8,  9,  10, 8,  10, 11,  // back
//		12, 14, 13, 12, 15, 14,  // left
//		16, 18, 17, 16, 19, 18,  // upper
//		20, 21, 22, 20, 22, 23   // bottom
//	};
//
//	glGenVertexArrays(1, &up);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(up);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	// define position pointer layout 0
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(0);
//
//	// define texcoord pointer layout 1
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//
//	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//	glBindVertexArray(0);
//
//	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//}
////mimbar
//void Demo::DrawMimbarUp()
//{
//	glUseProgram(shaderProgram);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textureUp);
//	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
//
//	glBindVertexArray(up); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//
//	glm::mat4 model;
//	model = glm::translate(model, glm::vec3(0, 0, 0));
//
//	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));
//
//	model = glm::scale(model, glm::vec3(1.5, 1.5, 2));
//
//	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glBindVertexArray(0);
//}

void Demo::BuildMimbarDown() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDown);
	glBindTexture(GL_TEXTURE_2D, textureDown);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("allah4.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-4.75,  -0.25,  12.0, 0, 0, // 8 
		-2.25,  -0.25,  12.0, 0.3, 0.3, // 9
		-2.25,  0, 12.0, 0.3, 0.3, // 10
		-4.75,  0, 12.0, 0.3, 0.3, // 11

		// right
		-2.25,  0,  13.0, 0, 0,  // 4
		-2.25,  0, 12.0, 0.3, 0.3,  // 5
		-2.25,  -0.25,  12.0, 0.3, 0.3,  // 6
		-2.25,  -0.25,   13.0, 0, 0.3,  // 7

		// back
		-4.75,  -0.25,  13.0, 0, 0,  // 0
		-2.25,  -0.25,  13.0, 0, 0,   // 1
		-2.25,  0, 13.0, 0, 0,   // 2
		-4.75,  0, 13.0, 0, 0,  // 3


		// left
		-4.75,  -0.25,  12.0, 0, 0, // 12
		-4.75,  -0.25,   13.0, 0.3, 0.3, // 13
		-4.75,  0,  13.0, 0.3, 0.3, // 14
		-4.75,  0, 12.0, 0.3, 0.3, // 15

		// upper
		-2.25, 0,  13.0, 0, 0,   // 16
		-4.75, 0,  13.0, 0.3, 0.3,  // 17
		-4.75, 0, 12.0, 0.3, 0.3,  // 18
		-2.25, 0, 12.0, 0.3, 0.3,   // 19

		// bottom
		-4.75,  -0.25, 12.0, 0.3, 0.3, // 20
		-2.25,  -0.25, 12.0, 0.3, 0.3,  // 21
		-2.25,  -0.25,  13.0, 0.3, 0.3,  // 22
		-4.75,  -0.25,  13.0, 0.3, 0.3, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 21, 22, 20, 22, 23   // bottom
	};

	glGenVertexArrays(1, &down);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(down);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawMimbarDown()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDown);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(down); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 2, 1.92));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}



void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.003f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}



