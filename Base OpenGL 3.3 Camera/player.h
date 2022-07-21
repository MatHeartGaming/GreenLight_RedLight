#pragma once
#include "model.h"
#include "skinned_mesh.h"
#include "globalData.h"
#include <glad/glad.h>
#include <irrKlang/irrKlang.h>



struct PlayerData {

	float x = 0;
	float z = 0;
	float angle = 180;
	bool userControlled = false;

	int deadState = 0;

	float velocity = 0;

	float animationTime_playerDying = 0;
	float animationTime_playerVictory_1 = 0;

	bool move = true;
	bool running = false;
	float animationTime = 0;
	bool finish = false;

	bool dead = false;
	bool victory = false;
};


class player {

public:

	player() {}

	PlayerData players[456];

	irrklang::ISoundEngine* audioEng = irrklang::createIrrKlangDevice();

	//coordinate
	float x;
	float y;
	float z;

	float collisionDist = 1;
	float acceleration = 1 / 2.34f;
	float acceleration_Run = 1 / 1.5f;

	float anglePlayer;

	bool mouseSxIsSelected = false;
	bool isUserControlled = false;

	int deadStatePlay = 0;


	// modelli 3D con scheletro e animazione
	SkinnedMesh playerRunning;
	SkinnedMesh playerWalking;
	SkinnedMesh playerStanding;
	SkinnedMesh playerDying;
	SkinnedMesh playerVictory_1;

	// tempo per le animazioni
	float animationTime_playerStanding;
	float animationTime_playerWalking;
	float animationTime_playerRunning;

	//texture
	unsigned int textureShadow;

	float startTime = 20;
	float curTime = 0;

	float volumeLevel = 0.1;

	//prototipi
	void drawPlayer(Shader animShader, Shader simpleShader, glm::vec3 mousePoint, Shader lightShader); //disegna il player

	void initPlayer(); //inizializza il player
	void resetPlayer();
	void loadMeshModel();

	float getX() {
		return x;
	}

	float getY() {
		return y;
	}

	float getZ() {
		return z;
	}

	float getAnglePlayer() {
		return anglePlayer;
	}

	void setX(float new_x) {
		x = new_x;
	}

	void setY(float new_y) {
		y = new_y;
	}

	void setZ(float new_z) {
		z = new_z;
	}

	void setAnglePlayer(float angle) {
		anglePlayer = angle;
	}

	int userPlayerIndex = 0;

	void initPlayers();
	void drawPlayers(Shader animShader);
	void animate(float dTime);

	vector<vector <glm::mat4>> tfStop;
	vector<vector <glm::mat4>> tfRun;
	vector<vector <glm::mat4>> tfDie;
	vector<vector <glm::mat4>> tfVictory_1;
	vector<vector <glm::mat4>> tfWalk;

	bool isApproaching(int a, int b, float dTime);
	void updateDynamics(float dTime);
	void adjustSpeeds(float dTime);
	void updateKinematics(float dTime);
};

void  player::resetPlayer() {
	//punto in cui nasce
	x = PLAYER_SPOW_X;
	y = PLAYER_SPOW_Y;
	z = PLAYER_SPOW_Z;

	anglePlayer = 0.0f;

	// tempo per le animazioni
	animationTime_playerStanding = INIT_ZERO;
	animationTime_playerRunning = INIT_ZERO;
	animationTime_playerWalking = INIT_ZERO;
}

void player::loadMeshModel() {
	playerRunning.loadMesh("animation/player456/Running_Slow.dae");
	playerStanding.loadMesh("animation/player456/Male Standing Pose.dae");
	playerDying.loadMesh("animation/player456/Dying.dae");
	playerWalking.loadMesh("animation/player456/Crouched_Walking_in_place.dae");

	int animRand = rand() % 2;

	if (animRand == 0) {
		playerVictory_1.loadMesh("animation/player456/Victory.dae");
		cout << "Animazione Standard" << endl;
	}
	else {
		playerVictory_1.loadMesh("animation/player456/Breakdance_Ending.dae");
		cout << "Animazione Break Dance" << endl;
	}
}

void player::initPlayer() {

	resetPlayer();
	loadMeshModel();

	initPlayers();

	for (int i = 0; i < 350; i++) {
		vector <glm::mat4> t1;
		playerStanding.boneTransform(i/10.0f, t1);
		tfStop.emplace_back(t1);

		vector <glm::mat4> t2;
		playerRunning.boneTransform(i / 10.0f, t2);
		tfRun.emplace_back(t2);

		vector <glm::mat4> t3;
		playerWalking.boneTransform(i / 10.0f, t3);
		tfWalk.emplace_back(t3);
	}

	for (int i = 0; i < 1050; i++) {
		vector <glm::mat4> t1;
		playerDying.boneTransform(i / 10.0f, t1);
		tfDie.emplace_back(t1);
	}

	for (int i = 0; i < 1050; i++) {
		vector <glm::mat4> t1;
		playerVictory_1.boneTransform(i / 10.0f, t1);
		tfVictory_1.emplace_back(t1);
	}
}

float angleBetween(const glm::vec3 a, const glm::vec3 b) {

	float angle = atan2(b.x, a.x) - atan2(b.z, a.y);
	return angle;
}

void player::drawPlayer(Shader simpleShader, Shader animShader, glm::vec3 mousePoint,Shader lightShader) {
	
	if (moveDx == false && muoviSx == false && muoviSu == false && muoviGiu == false || isColliding) { //se non mi muovo -> meshStanding
		players[userPlayerIndex].move = false;
		players[userPlayerIndex].running = false;
	}
	else if (players[userPlayerIndex].userControlled && run) {
		players[userPlayerIndex].running = true;
	}
	else if (moveDx == true || muoviSx == true || muoviSu == true || muoviGiu == true) { //se mi muovo -> meshRunning		
		players[userPlayerIndex].move = true;
		players[userPlayerIndex].running = false;
	}

	drawPlayers(animShader);
}

void player::initPlayers() {

	int i = 0;
	float zPos = 49;
	while (i < 456) {

		float xPos = -8.5;
		while ( (xPos < 7.7) && (i < 456) ) {
			xPos += 1 + 0.3 * (rand() % 100) / 100.0f;

			players[i].x = xPos;
			players[i].z = zPos + 0.2 * (rand() % 100) / 100.0f;

			i++;
		}

		zPos -= 0.5 + 0.1 * (rand() % 100) / 100.0f;
	}
	
	//int userIndex = rand() % 456;
	int userIndex = 455;
	players[userIndex].userControlled = true;
	x = players[userIndex].x;
	z = players[userIndex].z;
	anglePlayer = players[userIndex].angle;

	userPlayerIndex = userIndex;

	audioEng = irrklang::createIrrKlangDevice();
	audioEng->setSoundVolume(0);
}

bool player::isApproaching(int a, int b, float dTime) {

	float presentDistance = glm::length(glm::vec2(players[a].x - players[b].x, players[a].z - players[b].z));

	float p1X = players[a].x + players[a].velocity * dTime * sin(glm::radians(players[a].angle));
	float p1Z = players[a].z + players[a].velocity * dTime * cos(glm::radians(players[a].angle));

	float p2X = players[b].x + players[b].velocity * dTime * sin(glm::radians(players[b].angle));
	float p2Z = players[b].z + players[b].velocity * dTime * cos(glm::radians(players[b].angle));

	float projectedDistance = glm::length(glm::vec2(p1X - p2X, p1Z - p2Z));

	if (projectedDistance < presentDistance) {
		return true;
	}
	else {
		return false;
	}
}
void player::updateDynamics(float dTime) {

	if (!players[userPlayerIndex].dead) {
		if (moveDx == false && muoviSx == false && muoviSu == false && muoviGiu == false || isColliding) { //se non mi muovo -> meshStanding
			players[userPlayerIndex].move = false;
		}
		else if (run) {
			players[userPlayerIndex].running = true;
		}
		else if (moveDx == true || muoviSx == true || muoviSu == true || muoviGiu == true) { //se mi muovo -> meshRunning		
			players[userPlayerIndex].move = true;
		}
	}

	for (int i = 0; i < 456; i++) {
		if (!players[i].dead) {
			if (players[i].running) {
				float vLim = (0.6 + 0.4 * (rand() % 100) / 10.0f);
				if (players[i].velocity < vLim) {
					players[i].velocity += acceleration_Run * dTime * (0.75 + 0.5 * (rand() % 100) / 10.0f);
				}
			} else if (players[i].move) {
				float vLim = (0.6 + 0.4 * (rand() % 100) / 100.0f);
				if (players[i].velocity < vLim && !players[i].userControlled) {
					players[i].velocity += acceleration * dTime * (0.75 + 0.5 * (rand() % 100) / 100.0f);
				}
				else if (players[i].velocity < vLim && players[i].userControlled) {
					players[i].velocity += acceleration_Run * dTime* (0.75 + 0.5 * (rand() % 100) / 10.0f);
				}
				else {
					players[i].velocity -= acceleration * dTime * (0.75 + 0.5 * (rand() % 100) / 100.0f);
				}
			}
			else if (players[i].velocity > 0 && !players[i].userControlled) {
				players[i].velocity -= acceleration * dTime * (0.75 + 0.5 * (rand() % 100) / 100.0f);
			}
			else if (players[i].velocity > 0 && players[i].userControlled) {
				players[i].velocity = 0;
			}
		}
	}
}
void player::adjustSpeeds(float dTime) {

	size_t numUpdates = 0;
	float xDis, yDis, tmp;

	int numUpdatePrev = 0;
	int retries = 0;

	do {

		numUpdates = 0;

		for (int i = 0; i < 456; i++) {

			if (players[i].dead) {
				continue;
			}

			for (int j = i + 1; j < 456; j++) {

				if (players[j].dead) {
					continue;
				}

				xDis = abs(players[i].x - players[j].x);
				yDis = abs(players[i].z - players[j].z);

				if ((xDis < collisionDist) && (yDis < 0.5 * collisionDist)) {
					if (isApproaching(i, j, dTime)) {

						if (players[j].finish) {
							players[i].move = false;
							players[i].velocity = 0;
							players[i].finish = true;
						}
						else if (players[i].finish) {
							players[j].move = false;
							players[j].velocity = 0;
							players[j].finish = true;
						}
						else {

							tmp = players[i].angle;
							players[i].angle = players[j].angle;
							players[j].angle = tmp;

							tmp = players[i].velocity;
							players[i].velocity = players[j].velocity;
							players[j].velocity = tmp;

							numUpdates++;
						}
					}
				}
			}
		}

		if (numUpdatePrev == numUpdates) {
			retries++;
		}

		if (retries > 40) {
			break;
		}

		numUpdatePrev = numUpdates;
	} while (numUpdates > 0);
}
void player::updateKinematics(float dTime) {

	for (int i = 0; i < 456; i++) {
		if (!players[i].dead) {
			if (players[i].velocity > 0) {

				players[i].x = players[i].x + players[i].velocity * dTime * sin(glm::radians(players[i].angle));
				players[i].z = players[i].z + players[i].velocity * dTime * cos(glm::radians(players[i].angle));
				players[i].animationTime = players[i].animationTime + players[i].velocity * dTime;

				if (players[i].userControlled) {
					//cout << "Z player: " << players[i].z << ", X: " << players[i].x << endl;
				}
			}

			if (players[i].z < -42) {
				players[i].move = false;
				players[i].finish = true;
				players[i].velocity = 0;
				players[i].z = -42;
			}
		}
	}
}

void player::animate(float dTime) {

	curTime += dTime;
	if (curTime < startTime) {
		return;
	}

	if (!players[userPlayerIndex].dead) {
		players[userPlayerIndex].angle = anglePlayer;
	}

	for (int i = 0; i < 456; i++) {
		if (players[i].dead) {

			if (players[i].deadState == 0) {

				players[i].deadState = 1;

				if (deadStatePlay == 0) {
					audioEng->setSoundVolume(0.8);
					audioEng->play2D("audio/Shot.mp3", false);

					deadStatePlay = 1;
				}
			}
			else if (players[i].deadState == 1) {

				if (players[i].animationTime_playerDying > 0.8) {
					players[i].deadState = 2;
					deadStatePlay = 0;

					if ((i % 3) == 0) {
						audioEng->setSoundVolume(volumeLevel);
						audioEng->play2D("audio/Scream.mp3", false);
					}
					
				}
			}

			players[i].animationTime_playerDying += dTime;

			if (players[i].animationTime_playerDying > 4) {
				players[i].animationTime_playerDying = 4;
			}
		}

		if (players[i].finish) {

			players[i].animationTime_playerVictory_1 += dTime;

			if (players[i].animationTime_playerVictory_1 > 4) {
				players[i].animationTime_playerVictory_1 = 4;
				players[i].victory = true;
			}
		}

	}

	updateDynamics(dTime);
	adjustSpeeds(dTime);
	updateKinematics(dTime);

	anglePlayer = players[userPlayerIndex].angle;

}

void player::drawPlayers(Shader animShader) {

	animShader.use();

	//material properties
	animShader.setVec3("material.ambient", 0.9f, 0.9f, 0.9f);
	animShader.setVec3("material.diffuse", 0.8f, 0.8f, 0.8f);
	animShader.setVec3("material.specular", 0.1f, 0.1f, 0.1f);
	animShader.setFloat("material.shininess", 76.8f);

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	lightProjection = glm::ortho(-10.0f, 10.0f, -50.0f, 50.0f, 10.0f, 27.0f);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 1.0));
	lightSpaceMatrix = lightProjection * lightView;
	animShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	animShader.setVec3("lightPos", lightPos);


	glm::mat4 model = glm::mat4(UNIT);

	for (int i = 0; i <	456; i++) {

		model = glm::mat4(UNIT);
		if (!players[i].userControlled) {
			model = glm::translate(model, glm::vec3(players[i].x, 0.0f, players[i].z));
			model = glm::rotate(model, glm::radians(players[i].angle), glm::vec3(0, 1, 0));
		}
		else {

			model = glm::translate(model, glm::vec3(players[i].x, 0.0f, players[i].z));
			model = glm::rotate(model, glm::radians(players[i].angle), glm::vec3(0, 1, 0));
			if (players[i].move) {
				players[i].animationTime = animationTime_playerWalking;
			}
			else {
				players[i].animationTime = animationTime_playerStanding;
			}

		}

		animShader.setMat4("model", model);

		if (players[i].dead) {
			int animSlot = int(players[i].animationTime_playerDying * 10) % 1050;

			glUniformMatrix4fv(glGetUniformLocation(animShader.ID, "bones"),
				tfDie[animSlot].size(),
				GL_FALSE,
				glm::value_ptr(tfDie[animSlot][0]));

			playerDying.render();
		}
		else if (players[i].running && players[i].velocity > 0) {
			int animSlot = int(players[i].animationTime * 10) % 350;

			glUniformMatrix4fv(glGetUniformLocation(animShader.ID, "bones"),
				tfRun[animSlot].size(),
				GL_FALSE,
				glm::value_ptr(tfRun[animSlot][0]));

			playerRunning.render();
		}
		else if (players[i].velocity > 0) {
			int animSlot = int(players[i].animationTime * 10) % 350;

			glUniformMatrix4fv(glGetUniformLocation(animShader.ID, "bones"),
				tfWalk[animSlot].size(),
				GL_FALSE,
				glm::value_ptr(tfWalk[animSlot][0]));

			playerWalking.render();
		}
		else if (players[i].victory) {
				int animSlot2 = int(players[i].animationTime * 10) % 350;

				glUniformMatrix4fv(glGetUniformLocation(animShader.ID, "bones"),
					tfStop[animSlot2].size(),
					GL_FALSE,
					glm::value_ptr(tfStop[animSlot2][0]));

				playerStanding.render();
		}
		else if (players[i].finish) {
			int animSlot = int(players[i].animationTime_playerVictory_1 * 10) % 1050;

			glUniformMatrix4fv(glGetUniformLocation(animShader.ID, "bones"),
				tfVictory_1[animSlot].size(),
				GL_FALSE,
				glm::value_ptr(tfVictory_1[animSlot][0]));

			playerVictory_1.render();
		}
		else {
			int animSlot = int(players[i].animationTime * 10) % 350;

			glUniformMatrix4fv(glGetUniformLocation(animShader.ID, "bones"),
				tfStop[animSlot].size(),
				GL_FALSE,
				glm::value_ptr(tfStop[animSlot][0]));

			playerStanding.render();
		}
	}
}