#pragma once
#include "model.h"
#include "skinned_mesh.h"
#include "globalData.h"
#include <irrKlang/irrKlang.h>

class GameArena {

public:

	GameArena() {}

	int botType;

	Model* dollHead = new Model();
	Model* dollBody = new Model();
	Model* Arena = new Model();

	int animState = 0;
	float animTime = 0;

	float headRotationTime = 2;

	float rotationAngle;  
	float volumeLevel = 0.5;
	bool start = false;

	//Texture barra vita
	 unsigned int textureShadow;

	 float startTime = 20;
	 float curTime = 0;

	//Prototipi
	void drawVillain(Shader animShader , Shader simpleShader, Shader lightShader);
	void animate(float dTime);

	void initVillain();

	void resetVillan();
	void loadMeshModel();

	irrklang::ISoundEngine* SoundEngine_effect;

};

void  GameArena::resetVillan() {

	rotationAngle = 0;
	animTime = 1;

}
void GameArena::loadMeshModel() {
	dollHead->loadModel("animation/bambola/Doll_Head.obj");
	dollBody->loadModel("animation/bambola/Doll.obj");
	Arena->loadModel("animation/wall2.obj");
	Arena->loadModel("animation/Floor2.obj");
	Arena->loadModel("animation/sky2.obj");
	Arena->loadModel("animation/tree.obj");
}

void GameArena::initVillain() {

	resetVillan();
	loadMeshModel();

	SoundEngine_effect = irrklang::createIrrKlangDevice();
	SoundEngine_effect->setSoundVolume(0);
}

void GameArena::drawVillain(Shader animShader, Shader simpleShader, Shader lightShader){

	lightShader.use();

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	lightProjection = glm::ortho(-10.0f, 10.0f, -50.0f, 50.0f, 10.0f, 27.0f);
	
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 1.0));
	
	lightSpaceMatrix = lightProjection * lightView;
	lightShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	lightShader.setVec3("lightPos", lightPos);



	glm::mat4 model = glm::mat4(UNIT);
	model = glm::translate(model, glm::vec3(-0.11, 0.06, -43.6)); 
	model = glm::rotate(model, glm::radians(rotationAngle-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	model = glm::scale(model, glm::vec3(UNIT * 0.5, UNIT * 0.5, UNIT * 0.5));
	lightShader.setMat4("model", model);
	dollHead->Draw(lightShader);

	model = glm::mat4(UNIT);
	model = glm::translate(model, glm::vec3(0, 0, -43.44));
	model = glm::rotate(model, glm::radians(170.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(UNIT * 0.5, UNIT * 0.5, UNIT * 0.5));
	lightShader.setMat4("model", model);
	dollBody->Draw(lightShader);

	model = glm::mat4(UNIT);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	lightShader.setMat4("model", model);
	Arena->Draw(lightShader);
}

void GameArena::animate(float dTime) {

	curTime += dTime;
	if (curTime < startTime) {
		return;
	}

	if (!start) {
		SoundEngine_effect->setSoundVolume(volumeLevel);
		SoundEngine_effect->play2D("audio/RedLight.mp3", false);
	}

	start = true;

	animTime = animTime - dTime;

	if (animState == 0) {
		rotationAngle = 205.0f * animTime / headRotationTime;

		if (animTime < 0) {
			animState = 1;

			int time = rand() % 2000;
			animTime = (time / 100.0f)+5.0f;
		}
	}
	else if (animState == 1) {

		if (animTime < 0) {
			animState = 2;
			animTime = headRotationTime;
			SoundEngine_effect->setSoundVolume(volumeLevel);
			SoundEngine_effect->play2D("audio/GreenLight.mp3", false);
		}
	}
	else if (animState == 2) {
		rotationAngle = 205.0f - 205.0f * animTime / headRotationTime;

		if (animTime < 0) {
			animState = 3;
			int time = rand() % 1000;
			animTime = (time / 100.0f) + 2.0f;
		}
	}
	else if (animState == 3) {

		if (animTime < 0) {
			animState = 0;
			animTime = headRotationTime;
			SoundEngine_effect->setSoundVolume(volumeLevel);
			SoundEngine_effect->play2D("audio/RedLight.mp3", false);
		}
	}

}




