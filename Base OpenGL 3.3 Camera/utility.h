#pragma once
#include <vector>

/* Contiene delle funzioni di
   utility utilizzate a livello
   globale dalle altre classi */

//rand int tra: M(incluso) ed N(escluso) 
int randMtoN(int M, int N) {
	return M + rand() % N;
}

//rand float tra: M(incluso) ed N(escluso) 
float frandMToN(float M, float N) {
	return M + (rand() / (RAND_MAX / (N - M)));
}

//Controlla se un numero int è già presente (return true) o no (return false) all'interno di un <vector>
bool numeroGiaPresente(int numero, vector<int> vettore) {
	for (int i = 0; i < vettore.size(); i++) {
		if (numero == vettore[i]) {
			return true; //numero presente
		}
	}
	return false; //numero non presete
}

//Funzione di confronto tra due float: restituisce true se a = b a meno di un valore epsilon (molto piccolo)
bool isEqual(float a, float b, float epsilon)
{
	return fabs(a - b) < epsilon;
}




