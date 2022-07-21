#pragma once
#include "player.h"
#include "shader_s.h"
#include "GameArena.h"
#include "loading.h"
#include <vector>

/*classe game qui vengono gestiti tutte le azioni relative
alla partita e inizializzati tutti gli oggetti*/
class game {

public:
	
	//Da renderizzare:
	player* p;					 //PLAYER
	GameArena* doll;

	bool gameInitialized;
	bool inGame;
	bool gamePause;

	loading* loadingGame;

	glm::vec3 mousePoint;				 //Coordinate del mouse

	unsigned int textureLifeBar;
	unsigned int textureShadow;

	bool startGameSoundtrack;
	bool gameOver = false;

	unsigned int num_texture[10]; //texture Numeri

	//Costruttore
	game() {
		p = new player();

		doll = new GameArena();

		loadingGame = new loading();
		startGameSoundtrack = false;
	};

	// -- Prototipi -- //
	void init(); //inizializza il game
	void setShadersProperties(Shader simpleShader, Shader lightShader, Shader animShader, glm::mat4 view); //setta le proprietà degli shader
	void draw(Shader simpleShader, Shader lightShader, Shader animShader, glm::mat4 view);

	player* getPlayer() {
		return p;
	}

	/*
	vector <villain*> getSpawnedBotList() {
		return spawnedBotList;
	}
	*/

	glm::vec3 getMousePoint() {
		return mousePoint;
	}

	void getPlayer(player* player) {
		p = player;
	}

	void setMousePoint(glm::vec3 w) {
		mousePoint = w;
	}

};

void game::init() {

	//Se il game non è stato ancora inizializzato
	if (gameInitialized == false) {

		if (loadingGame->statusLoading == STATUS_LOADING_0) {
			// init MAP
		}

		if (loadingGame->statusLoading == STATUS_LOADING_1) {
			// init PLAYER
			p->initPlayer();
			cout << "*** Players: Loaded -> Initialized" << endl;
			doll->initVillain();
		}

		if (loadingGame->statusLoading >= STATUS_LOADING_2 && loadingGame->statusLoading < STATUS_LOADING_5) {
			cout << "*** Bot Models: Loaded" << endl;
		}

		if (loadingGame->statusLoading == STATUS_LOADING_5) {
			// init PATH
			//initPathList();

			gameInitialized = true;	//segnalo che l'init del game è stato completato completato
			cout << "*** INIT GAME: COMPLETED" << endl;
		}
	}

	//Se il game è già stato inizializzato. Resetto animazioni e posizioni del player e dei bot
	else if (gameInitialized == true) {

		if (loadingGame->statusLoading == UNIT) {
			//resetto il player
			
			for (int i = 0; i < 456; i++) {
				p->players[i].move = false;
				p->players[i].running = false;
				p->players[i].dead = false;
				p->players[i].victory = false;
				p->players[i].finish = false;
				p->players[i].velocity = 0;
				p->players[i].x = PLAYER_SPOW_X;
				p->players[i].z = PLAYER_SPOW_Y;
				p->players[i].animationTime_playerDying = 0;
				p->players[i].animationTime_playerVictory_1 = 0;
			}

			int i = 0;
			float zPos = 49;
			while (i < 456) {

				float xPos = -8.5;
				while ((xPos < 7.7) && (i < 456)) {
					xPos += 1 + 0.3 * (rand() % 100) / 100.0f;

					p->players[i].x = xPos;
					p->players[i].z = zPos + 0.2 * (rand() % 100) / 100.0f;

					i++;
				}

				zPos -= 0.5 + 0.1 * (rand() % 100) / 100.0f;
			}

			//int userIndex = rand() % 456;
			int userIndex = 455;
			p->players[userIndex].userControlled = true;
			p->players[userIndex].x = p->players[userIndex].x;
			p->players[userIndex].z = p->players[userIndex].z;

			doll->resetVillan();
		}
	}


}

void game::setShadersProperties(Shader simpleShader, Shader lightShader, Shader animShader, glm::mat4 view) {
	
	// ---- ANIMATION Shader ---- //
	animShader.use();

	//projection
	glm::mat4 projection_animation = glm::mat4(UNIT);	//identity matrix
	projection_animation = glm::perspective(glm::radians(PROJECTION_ANGLE_45), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 50000.0f);
	animShader.setMat4("projection", projection_animation);

	//view
	animShader.setMat4("view", view);

	//light properties
	animShader.setVec3("light.position", lightPos);
	animShader.setVec3("light.ambient", UNIT, UNIT, UNIT);
	animShader.setVec3("light.diffuse", UNIT, UNIT, UNIT);
	animShader.setVec3("light.specular", UNIT, UNIT, UNIT);


	// ---- SIMPLE Shader ---- //
	simpleShader.use();

	//view
	simpleShader.setMat4("view", view);

	//projection
	glm::mat4 projection2 = glm::mat4(UNIT);	//identity matrix
	projection2 = glm::perspective(glm::radians(PROJECTION_ANGLE_45), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 50000.0f);
	simpleShader.setMat4("projection", projection2);
	simpleShader.setVec4("color", UNIT, UNIT, UNIT, UNIT);

	// ---- LIGHT Shader ---- //
	lightShader.use();

	//projection
	glm::mat4 projection3 = glm::mat4(UNIT);	//identity matrix
	projection3 = glm::perspective(glm::radians(PROJECTION_ANGLE_45), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 50000.0f);
	lightShader.setMat4("projection", projection3);

	//view
	lightShader.setMat4("view", view);

	//light per la mappa
	lightShader.setVec3("light.position", lightPos);
	lightShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
	lightShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
	lightShader.setVec3("light.specular", UNIT, UNIT, UNIT);
	lightShader.setVec3("colormodel", UNIT, UNIT, UNIT);

}

void game::draw(Shader simpleShader, Shader lightShader, Shader animShader, glm::mat4 view) {
	//Setto le proprietà view, projection degli shaders
	setShadersProperties(simpleShader, lightShader, animShader, view); 

	//DRAW PLAYER
	p->drawPlayer(simpleShader, animShader, getMousePoint(), lightShader);

	doll->drawVillain(animShader, simpleShader, lightShader);
}
