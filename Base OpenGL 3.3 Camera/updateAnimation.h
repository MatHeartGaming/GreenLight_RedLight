#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "GameArena.h"
#include <vector>
#include "mainMenu.h"

class updateAnimation
{
public:	

	//richiamta tutto l'update dei tempi
	void updateAllAnimations(player* player);

	//player animation time
	void playerAnimation(player* player);

	void increasePlayer_Run(player* player);
	void increasePlayer_Stand(player* player);
	void increasePlayer_Walk(player* player);

};

void updateAnimation::updateAllAnimations(player* player) {
	playerAnimation(player);
}

void updateAnimation::playerAnimation(player* player) {
	increasePlayer_Run(player);
	increasePlayer_Stand(player);
	increasePlayer_Walk(player);
}

void updateAnimation::increasePlayer_Stand(player* player) {
	player->animationTime_playerStanding = player->animationTime_playerStanding + 0.06f;   //incremento l'animazione
	if (player->animationTime_playerStanding > 20.0f) {  //quando l'animazione supera la soglia
		player->animationTime_playerStanding = 0.0f;     //resetto il tempo di animazione all'inizio -> ricomincia da capo (per evitare bug)
	}
}

void updateAnimation::increasePlayer_Run(player* player) {

	if (run) { //se il player corre
		player->animationTime_playerRunning = player->animationTime_playerRunning + 0.1f;  //incremento l'animazione

		if (player->animationTime_playerRunning > 20.0f) {   //quando il player fa il secondo passo
			player->animationTime_playerRunning = 0.0f;     //resetto il tempo di animazione all'inizio -> ricomincia da
		}
	}
}

void updateAnimation::increasePlayer_Walk(player* player) {

	if (moveDx == true || muoviSx == true || muoviSu == true || muoviGiu == true) { //se il player cammina
		player->animationTime_playerWalking = player->animationTime_playerWalking + 0.1f;  //incremento l'animazione
		if (player->animationTime_playerWalking > 20.0f) {   //quando il player fa il secondo passo
			player->animationTime_playerWalking = 0.0f;     //resetto il tempo di animazione all'inizio -> ricomincia da
		}
	}
}
