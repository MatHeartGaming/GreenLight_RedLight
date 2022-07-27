#pragma once
#include "model.h"
#include "path.h"
#include "skinned_mesh.h"
#include "globalData.h"
#include "player.h"

class Participant {

	public:
	Participant() {}

	//coordinate
	float x;
	float y;
	float z;

	float rotationAngle; //angolo di rotazione corrente

	// modelli 3D con scheletro e animazione
	SkinnedMesh meshWalking;
	
	// booleani per le azioni del bot
	bool animation_botWalking;
	bool animation_botDead;
	bool isDead;
	bool isColliding_vsPlayer;

	player* p;


	//Prototipi
	void drawParticipant(Shader animShader, Shader simpleShader);
	void drawShadowParticipant(Shader shaderShadow);//disegna ombra
	void animate(Shader animShader);
	void initParticipant();
};