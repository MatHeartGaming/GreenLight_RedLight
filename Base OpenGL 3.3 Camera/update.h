#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "mainMenu.h"
#include "pauseMenu.h"

/* classe Update in cui vengono gestiti tutti
gli aggiornamenti relativi allo stato degli 
oggetti in game e dei menù:
	a) movimento e collisioni del player
	b) mira e shot
	c) movimento e collisioni dei bot
	d) attacco dei bot
	e) click dei bottoni del main menù e pause menù 
*/

class update {
public:

	update() {}

	// ---- Movimento e collisioni del PLAYER ---- //
	void moveRight(player* p, Shader animShader);
	void moveLeft(player* p, Shader animShader);
	void moveUp(player* p, Shader animShader);
	void moveDown(player* p, Shader animShader);
	void calculateAnglePlayer(player* p); 
	
	// ---- MENU ---- //
	void cursorMainMenu(mainMenu* main_menu);
	void cursorPauseMenu(pauseMenu* pause_menu, game* gameuno);

};

void update::moveRight(player* p, Shader animShader) {
	for (int i = 0; i < 456; i++) {
		if (p->players[i].x < (ROOM_DIM_X1)) {
			isColliding = false;
		}
		else {
			isColliding = true;
		}
	}
}

void update::moveLeft(player* p, Shader animShader) {
	for (int i = 0; i < 456; i++) {
		if (p->players[i].x > (ROOM_DIM_X2)) {
			isColliding = false;
		}
		else {
			isColliding = true;
		}
	}
}

void update::moveUp(player* p, Shader animShader) {
	
	for (int i = 0; i < 456; i++) {
		
		if (p->players[i].z > (ROOM_DIM_Z1) && p->players[i].x > (ROOM_DIM_X2) && p->players[i].x < (ROOM_DIM_X1)) {
			isColliding = false;
		}
		else {
			isColliding = true;
		}
	}
}

void update::moveDown(player* p, Shader animShader) {
	for (int i = 0; i < 456; i++) {
		if (p->players[i].z < (ROOM_DIM_Z2) && p->players[i].x >(ROOM_DIM_X2) && p->players[i].x < (ROOM_DIM_X1)) {
			isColliding = false;
			//p->setZ(p->getZ() + MOVE_STEP);
		}
		else {
			isColliding = true;
		}
	}
}

void update::calculateAnglePlayer(player* p) {

	float anglePlayerR = 0.0f;
	float anglePlayer = p->getAnglePlayer();

	if (moveDx == true) {
		anglePlayerR = ANGLE_90;
		if (muoviSu == true) {
			anglePlayerR = ANGLE_135;
		}
		if (muoviGiu == true) {
			anglePlayerR = ANGLE_45;
		}
	}
	if (muoviSx == true) {
		anglePlayerR = ANGLE_270;
		if (muoviSu == true) {
			anglePlayerR = ANGLE_225;
		}
		if (muoviGiu == true) {
			anglePlayerR = ANGLE_315;
		}
	}

	if (muoviGiu == true) {
		if (moveDx == false && muoviSx == false) {
			anglePlayerR = 0.0f;
		}
	}
	if (muoviSu == true) {
		if (moveDx == false && muoviSx == false) {
			anglePlayerR = ANGLE_180;
		}
	}
	if (moveDx == false && muoviSx == false && muoviSu == false && muoviGiu == false) {
		anglePlayerR = anglePlayer;
	}

	if (anglePlayer - anglePlayerR > 0.0f ) {
		if (anglePlayer - anglePlayerR < ANGLE_180) {
			anglePlayer = anglePlayer - ANGLE_OFFSET;
		}
		else {
			anglePlayer = anglePlayer + ANGLE_OFFSET;

		}
	}
	if (anglePlayer - anglePlayerR < 0.0f) {
		if (anglePlayer - anglePlayerR > -ANGLE_180) {
			anglePlayer = anglePlayer + ANGLE_OFFSET;
		}
		else {
			anglePlayer = anglePlayer - ANGLE_OFFSET;
		}
	}
	
	if(anglePlayerR == anglePlayer){
		anglePlayer = anglePlayerR;
	}
	if (anglePlayer == ANGLE_360) {
		anglePlayer = 0.0f;
	}
	if (anglePlayer < 0.0f) {
		anglePlayer = ANGLE_360 - ANGLE_OFFSET;
	}


	p->setAnglePlayer(anglePlayer);

}

void update::cursorMainMenu(mainMenu* main_menu) {

	float mouseX = main_menu->getMousePoint().x;
	float mouseY = main_menu->getMousePoint().y;

	float aspectRatio = (float)SCR_HEIGHT / (float)SCR_WIDTH;

	float ymin, ymax, xmin, xmax;

	if (aspectRatio > 1) {
		ymin = -0.464;
		ymax = -0.238;

		xmin = -0.1945 * aspectRatio - 0.0097;
		xmax = 0.2481 * aspectRatio + 0.0019;
	}
	else {
		xmin = -0.166;
		xmax = 0.21;

		ymin = -1.4106 * aspectRatio * aspectRatio + 2.8403 * aspectRatio - 1.846;
		ymax = -0.8009 * aspectRatio * aspectRatio + 1.5633 * aspectRatio - 0.985;
	}

	
	if (mouseX >= xmin && mouseX <= xmax && mouseY >= ymin && mouseY <= ymax) {
		main_menu->button_NewGame->cursorIsAbove = true;
		if (mouseSx) {
			main_menu->button_NewGame->isSelected = true;
		}
		if (main_menu->buttonClicked) {
			cout << "START NEW GAME" << endl;
			main_menu->buttonClicked = false;
			main_menu->startNewGame = true;
			main_menu->button_NewGame->isSelected = false;
		}
	}
	else {
		main_menu->button_NewGame->cursorIsAbove = false;
	}
	main_menu->buttonClicked = false;
}

void update::cursorPauseMenu(pauseMenu* pause_menu, game* gameuno) {
	float mouseX = pause_menu->getMousePoint().x;
	float mouseZ = pause_menu->getMousePoint().y;

	if (gameuno->gameOver == false) {
		if (mouseX >= -0.12f && mouseX <= 0.12f && mouseZ >= 0.34f && mouseZ <= 0.55f) {
			pause_menu->returnGame->cursorIsAbove = true;
			if (mouseSx) {
				pause_menu->returnGame->isSelected = true;
			}
			if (pause_menu->buttonClicked) {
				cout << "RETURN" << endl;
				pause_menu->buttonClicked = false;
				gameuno->gamePause = false;
				pause_menu->returnGame->isSelected = false;
			}
		}
		else {
			pause_menu->returnGame->cursorIsAbove = false;
		}
	}
	else {
		pause_menu->returnGame->cursorIsAbove = false;
	}



	if (mouseX >= -0.12f && mouseX <= 0.12f && mouseZ >= -0.08 && mouseZ <= 0.12f) {
		pause_menu->goToMainMenu->cursorIsAbove = true;
		if (mouseSx) {
			pause_menu->goToMainMenu->isSelected = true;
		}
		if (pause_menu->buttonClicked) {
			cout << "MAIN MENU" << endl;
			pause_menu->buttonClicked = false;

			//back to main menu
			gameuno->inGame = false;
			gameuno->gamePause = false;

			pause_menu->goToMainMenu->isSelected = false;
		}
	}
	else {
		pause_menu->goToMainMenu->cursorIsAbove = false;
	}


	if (mouseX >= -0.12f && mouseX <= 0.12f && mouseZ >= -0.53f && mouseZ <= -0.31f) {
		pause_menu->quit->cursorIsAbove = true;
		if (mouseSx) {
			pause_menu->quit->isSelected = true;
		}
		if (pause_menu->buttonClicked) {
			cout << "QUIT" << endl;
			pause_menu->buttonClicked = false;
			quit = true;
			pause_menu->quit->isSelected = false;
		}
	}
	else {
		pause_menu->quit->cursorIsAbove = false;
	}


	pause_menu->buttonClicked = false;
	

}