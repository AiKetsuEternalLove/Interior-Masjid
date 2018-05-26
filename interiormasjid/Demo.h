#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2,
		texture2, textureAtas, atas, textureTangan, tangan, textureBawah, bawah,
		dindingKanan, textureDK, dindingKiri, textureDKiri, dindingDepan, textureDD,
		dindingBelakang, textureDB, dindingAtas, textureDA,
		tempatImam, textureTI, kaligrafiAllah, textureKA, kaligrafiMuhammad, textureKM, 
		textureTengah, tengah, textureMiddle, middle, textureUp, up, textureDown, down,
		rak, textureRak, rak2, textureRak2, textureTangkai, tangkai, textureBaling, baling,
		textureJD, jendela, textureJD2, jendela2, pintu, texturePintu;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY,
		posCamZ, CAMERA_SPEED, fovy;

	float angle = 0;
	double pindahX = 0, pindahY = 0, pindahZ = 0;

	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);

	void BuildPintu();
	void DrawPintu();

	void BuildRak();
	void DrawRak();
	void BuildRak2();
	void DrawRak2();

	void BuildCube();
	void BuildCube2();
	void BuildCubeKepala();
	void BuildTangan();
	void BuildKaki();
	void BuildPlane();

	void BuildDindingKanan();
	void BuildDindingKiri();
	void BuildDindingDepan();
	void BuildDindingBelakang();
	void BuildDindingAtas();

	void BuildTempatImam();
	void BuildKaligrafiAllah();
	void BuildKaligrafiMuhammad();

	void DrawCube();
	void DrawCube2();
	void DrawCubeKepala();
	void DrawTangan();
	void DrawKaki();
	void DrawPlane();

	void DrawDindingKanan();
	void DrawDindingKiri();
	void DrawDindingDepan();
	void DrawDindingBelakang();
	void DrawDindingAtas();

	void DrawTempatImam();
	void DrawKaligrafiAllah();
	void DrawKaligrafiMuhammad();

	//jendela
	void BuildJendela();
	void DrawJendela();
	void BuildJendela2();
	void DrawJendela2();

	//mimbar
	void BuildMimbarMiddle();
	void BuildMimbarUp();
	void BuildMimbarDown();

	void DrawMimbarMiddle();
	void DrawMimbarUp();
	void DrawMimbarDown();

	//kipas
	void BuildTangkai();
	void BuildBaling();

	void DrawTangkai();
	void DrawBaling();

	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

