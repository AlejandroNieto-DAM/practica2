#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>

void ComportamientoJugador::init()
{
	hayPlan = false;
	positionToGo.first = 3;
	positionToGo.second = 3;
	ultimaAccion = actIDLE;
	costeCosto = 0;
	costeAnchura = 0;
	activado = false;
	firstIteration = false;
}

void ComportamientoJugador::actualizarBrujulaPosicion()
{
	switch (ultimaAccion)
	{
	case actTURN_L:
		actual.orientacion = (actual.orientacion + 6) % 8;
		break;
	case actTURN_R:
		actual.orientacion = (actual.orientacion + 2) % 8;
		break;
	case actSEMITURN_L:
		actual.orientacion = (actual.orientacion + 7) % 8;
		break;
	case actSEMITURN_R:
		actual.orientacion = (actual.orientacion + 1) % 8;
		break;
	}
}

void ComportamientoJugador::actualizarPosicion()
{

	if (ultimaAccion == actFORWARD)
	{
		switch (actual.orientacion)
		{
		case 0:
			actual.fila--;
			break;
		case 1:
			actual.fila--;
			actual.columna++;
			break;
		case 2:
			actual.columna++;
			break;
		case 3:
			actual.fila++;
			actual.columna++;
			break;
		case 4:
			actual.fila++;
			break;
		case 5:
			actual.fila++;
			actual.columna--;
			break;
		case 6:
			actual.columna--;
			break;
		case 7:
			actual.fila--;
			actual.columna--;
			break;
		}
	}
}

void ComportamientoJugador::rellenarVisionCompleta(Sensores sensores)
{

	int brujula = sensores.sentido;

	if (sensores.nivel == 4)
	{
		sensores.posF = actual.fila;
		sensores.posC = actual.columna;
		brujula = actual.orientacion;
	}

	mapaResultado[sensores.posF][sensores.posC] = sensores.terreno[0];

	int contFil = 0;
	int contCol = 0;
	int util = 1;
	int valueY = 0;
	int valueX = 0;
	int partialValue = 0;
	int otherValue = 0;

	switch (brujula)
	{
	case 0:

		contFil = -1;
		contCol = -1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{
				mapaResultado[sensores.posF + contFil][sensores.posC + contCol + j] = sensores.terreno[util];
				util++;
			}

			contCol--;
			contFil--;
		}

		break;

	case 2:

		contFil = -1;
		contCol = 1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{
				mapaResultado[sensores.posF + contFil + j][sensores.posC + contCol] = sensores.terreno[util];
				util++;
			}

			contCol++;
			contFil--;
		}
		break;
	case 4:

		contFil = 1;
		contCol = 1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{
				mapaResultado[sensores.posF + contFil][sensores.posC + contCol - j] = sensores.terreno[util];
				util++;
			}

			contCol++;
			contFil++;
		}
		break;

	case 6:

		contFil = 1;
		contCol = -1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{
				mapaResultado[sensores.posF + contFil - j][sensores.posC + contCol] = sensores.terreno[util];
				util++;
			}

			contCol--;
			contFil++;
		}
		break;

	case 1:

		valueY = 1;
		valueX = 0;
		partialValue = 1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{

				if (j >= partialValue)
				{
					valueX = valueY;
				}

				mapaResultado[sensores.posF - valueY + otherValue][sensores.posC + valueX] = sensores.terreno[util];
				util++;

				if (j >= partialValue)
				{
					otherValue++;
				}

				valueX++;
			}

			otherValue = 0;
			partialValue++;
			valueY++;
			valueX = 0;
		}
		break;

	case 3:

		valueY = 0;
		valueX = 1;
		partialValue = 1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{

				if (j >= partialValue)
				{
					valueY = valueX;
				}

				mapaResultado[sensores.posF + valueY][sensores.posC + valueX - otherValue] = sensores.terreno[util];
				util++;

				if (j >= partialValue)
				{
					otherValue++;
				}

				valueY++;
			}

			otherValue = 0;
			partialValue++;
			valueY = 0;
			valueX++;
		}

		break;

	case 5:

		valueY = 1;
		valueX = 0;
		partialValue = 1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{

				if (j >= partialValue)
				{
					valueX = valueY;
				}

				mapaResultado[sensores.posF + valueY - otherValue][sensores.posC - valueX] = sensores.terreno[util];
				util++;

				if (j >= partialValue)
				{
					otherValue++;
				}

				valueX++;
			}

			otherValue = 0;
			partialValue++;
			valueY++;
			valueX = 0;
		}

		break;

	case 7:

		valueY = 0;
		valueX = 1;
		partialValue = 1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{

				if (j >= partialValue)
				{
					valueY = valueX;
				}

				mapaResultado[sensores.posF - valueY][sensores.posC - valueX + otherValue] = sensores.terreno[util];
				util++;

				if (j >= partialValue)
				{
					otherValue++;
				}

				valueY++;
			}

			otherValue = 0;
			partialValue++;
			valueY = 0;
			valueX++;
		}

		break;
	}
}

bool inPoints(estado st, list<estado> &objetivos){
	list<estado>::iterator it;
	list<estado>::iterator pos;
	bool encontrado = false;
	for(it = objetivos.begin(); it != objetivos.end() and encontrado == false; ++it){
		if(st.fila == (*it).fila and st.columna == (*it).columna){
			encontrado = true;
			pos = it;
		}
	}

	if(encontrado == true){
		cout << "Obj borrado" << endl;
		objetivos.erase(pos);
	}
	

}

bool ComportamientoJugador::posicionNoConviene(estado actual)
{

	switch (actual.orientacion)
	{
	case 0:
		actual.fila--;
		break;
	case 1:
		actual.fila--;
		actual.columna++;
		break;
	case 2:
		actual.columna++;
		break;
	case 3:
		actual.fila++;
		actual.columna++;
		break;
	case 4:
		actual.fila++;
		break;
	case 5:
		actual.fila++;
		actual.columna--;
		break;
	case 6:
		actual.columna--;
		break;
	case 7:
		actual.fila--;
		actual.columna--;
		break;
	}

	bool noConviente = false;
	switch(mapaResultado[actual.fila][actual.columna]){
		case 'B':
		case 'A':
		case 'M':
		case 'P':
			noConviente = true;
			break;
	}

	return noConviente;
}
// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actWHEREIS;

	if (sensores.nivel == 4 and sensores.colision == true)
	{
		accion = actWHEREIS;
		plan.clear();
		subObjetivos.clear();
		activado = false;
		firstIteration = false;
	}

	if (activado and sensores.nivel == 4 and firstIteration and sensores.colision == false)
	{
		actualizarPosicion();
		actualizarBrujulaPosicion();
		cout << "Actualizacion" << endl;
	}

	if (sensores.nivel <= 3 or (sensores.nivel == 4 and !firstIteration and activado))
	{

		cout << "Entrooo" << endl;
		actual.fila = sensores.posF;
		actual.columna = sensores.posC;
		actual.orientacion = sensores.sentido;

		firstIteration = true;
	}

	cout << "Fila: " << actual.fila << endl;
	cout << "Col : " << actual.columna << endl;
	cout << "Ori : " << actual.orientacion << endl;

	if (sensores.reset == 1)
	{
		hayPlan = false;
	}

	// Capturo los destinos

	objetivos.clear();
	for (int i = 0; i < sensores.num_destinos; i++)
	{
		estado aux;
		aux.fila = sensores.destino[2 * i];
		aux.columna = sensores.destino[2 * i + 1];
		objetivos.push_back(aux);
	}

	if(subObjetivos.size() == 0){
		subObjetivos = objetivos;
	}

	if (inPoints(actual, subObjetivos))
	{
		positionToGo.first = subObjetivos.front().fila;
		positionToGo.second = subObjetivos.front().columna;
	}

	if (sensores.nivel < 3)
	{
		if (!hayPlan)
		{
			hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
		}
	}
	else if (sensores.nivel == 4 and activado)
	{
		if (!hayPlan or (plan.front() == actFORWARD and posicionNoConviene(actual)))
			hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);

	}
	else if (sensores.nivel == 3)
	{
		
		if (!hayPlan or (plan.front() == actFORWARD and posicionNoConviene(actual)))
		{
			cout << "Entramos al plan" << endl;
			hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
		}
	}

	if (hayPlan and plan.size() > 0)
	{
		accion = plan.front();
		plan.erase(plan.begin());
	}
	else
	{
		cout << "No se pudo encontrar plan" << endl;
		hayPlan = false;
	}

	// cout << "Orientacion " << sensores.sentido << endl;
	if (sensores.nivel == 3 or (sensores.nivel == 4 and activado))
	{
		rellenarVisionCompleta(sensores);
	}

	ultimaAccion = accion;

	activado = true;

	return accion;
}


int ComportamientoJugador::costeCasilla(nodo &a, Action act)
{

	char tipoCasilla = mapaResultado[a.st.fila][a.st.columna];

	int costeCasilla = 1;

	if (mapaResultado[a.st.fila][a.st.columna] == 'K')
	{
		a.eq.bikiniOn = true;
		a.eq.zapatillasOn = false;
		// cout << "Bikini on suuuuuuuuuuuuu" << endl;
	}

	if (mapaResultado[a.st.fila][a.st.columna] == 'D')
	{
		a.eq.bikiniOn = false;
		a.eq.zapatillasOn = true;
		// cout << "Zapatillas on suuuuuuuuuuuuu" << endl;
	}

	switch (act)
	{

	case actSEMITURN_L:

		switch (tipoCasilla)
		{
		case 'A':
			if (a.eq.bikiniOn)
			{
				costeCasilla = 2;
			}
			else
			{
				costeCasilla = 300;
			}
			break;

		case 'B':
			if (a.eq.zapatillasOn)
			{
				costeCasilla = 1;
			}
			else
			{
				costeCasilla = 2;
			}
			break;

		default:
			costeCasilla = 1;
			break;
		}

		break;

	case actSEMITURN_R:

		switch (tipoCasilla)
		{
		case 'A':
			if (a.eq.bikiniOn)
			{
				costeCasilla = 2;
			}
			else
			{
				costeCasilla = 300;
			}
			break;

		case 'B':
			if (a.eq.zapatillasOn)
			{
				costeCasilla = 1;
			}
			else
			{
				costeCasilla = 2;
			}
			break;

		default:
			costeCasilla = 1;
			break;
		}

		break;

	case actTURN_L:

		switch (tipoCasilla)
		{
		case 'A':
			if (a.eq.bikiniOn)
			{
				costeCasilla = 5;
			}
			else
			{
				costeCasilla = 500;
			}
			break;

		case 'B':
			if (a.eq.zapatillasOn)
			{
				costeCasilla = 1;
			}
			else
			{
				costeCasilla = 3;
			}
			break;

		case 'T':
			costeCasilla = 2;
			break;

		default:
			costeCasilla = 1;
			break;
		}

		break;

	case actTURN_R:
		switch (tipoCasilla)
		{
		case 'A':
			if (a.eq.bikiniOn)
			{
				costeCasilla = 5;
			}
			else
			{
				costeCasilla = 500;
			}
			break;

		case 'B':
			if (a.eq.zapatillasOn)
			{
				costeCasilla = 1;
			}
			else
			{
				costeCasilla = 3;
			}
			break;

		case 'T':
			costeCasilla = 2;
			break;

		default:
			costeCasilla = 1;
			break;
		}

		break;

	case actFORWARD:

		switch (tipoCasilla)
		{
		case 'A':
			if (a.eq.bikiniOn)
			{
				costeCasilla = 10;
			}
			else
			{
				costeCasilla = 200;
			}
			break;

		case 'B':
			if (a.eq.zapatillasOn)
			{
				costeCasilla = 15;
			}
			else
			{
				costeCasilla = 100;
			}
			break;

		case 'T':
			costeCasilla = 2;
			break;

		default:
			costeCasilla = 1;
			break;
		}

		break;
	}

	return costeCasilla;
}



// Llama al algoritmo de busqueda que se usara en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan)
{
	switch (level)
	{
	case 0:
		cout << "Demo\n";
		estado un_objetivo;
		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Profundidad(origen, un_objetivo, plan);
		break;

	case 1:
		cout << "Optimo numero de acciones\n";
		estado un_objetivo1;
		un_objetivo1 = objetivos.front();
		cout << "fila: " << un_objetivo1.fila << " col:" << un_objetivo1.columna << endl;
		return pathFinding_Anchura(origen, un_objetivo1, plan);
		break;
	case 2:
		cout << "Optimo en coste\n";
		estado un_objetivo2;
		un_objetivo2 = objetivos.front();
		cout << "fila: " << un_objetivo2.fila << " col:" << un_objetivo2.columna << endl;
		return pathFinding_A(origen, un_objetivo2, plan);
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";
		if (mapaResultado[positionToGo.first][positionToGo.second] != '?')
		{
			positionToGo.first = -1;
			positionToGo.second = -1;
			busquedaPuntoLejano(origen, level);
		}
		return descubrirMapa(origen, plan);
		break;

	case 4:
		cout << "Reto 2: Maximizar objetivos\n";
		

		return descubrirMapa(origen, plan);
		break;
	}
	return false;
}



//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el codigo en caracter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla)
{
	if (casilla == 'P' or casilla == 'M' or casilla == 'a' or casilla == 'l')
		return true;
	else
		return false;
}

void ComportamientoJugador::busquedaPuntoLejano(estado current, int nivel)
{

	bool encontrado = false;
	estado aux;
	

	if (nivel == 4)
	{
		positionToGo.first = objetivos.front().fila;
		positionToGo.second = objetivos.front().columna;
	}

	switch (current.orientacion)
	{

	case 0:
	case 1:
	case 7:

		if (nivel == 3)
		{
			for (int i = 3; i < mapaResultado.size() - 4 and encontrado == false; i++)
			{
				for (int j = 3; j < mapaResultado.size() - 4 and encontrado == false; j++)
				{
					if (mapaResultado[i][j] == '?')
					{
						encontrado = true;
						positionToGo.first = i;
						positionToGo.second = j;
					}
				}
			}
		}

		break;

	case 3:
	case 4:
	case 5:

		if (nivel == 3)
		{
			for (int i = mapaResultado.size() - 4; i >= 3 and encontrado == false; i--)
			{
				for (int j = mapaResultado.size() - 4; j >= 3 and encontrado == false; j--)
				{
					if (mapaResultado[i][j] == '?')
					{
						encontrado = true;
						positionToGo.first = i;
						positionToGo.second = j;
					}
				}
			}
		}

		break;

	case 2:
		if (nivel == 3)
		{
			for (int i = mapaResultado.size() - 4; i >= 3 and encontrado == false; i--)
			{
				for (int j = 3; j < mapaResultado.size() - 4 and encontrado == false; j++)
				{
					if (mapaResultado[j][i] == '?')
					{
						encontrado = true;
						positionToGo.first = j;
						positionToGo.second = i;
					}
				}
			}
		}

		break;

	case 6:

		if (nivel == 3)
		{
			for (int i = 3; i < mapaResultado.size() - 4 and encontrado == false; i++)
			{
				for (int j = mapaResultado.size() - 4; j >= 3 and encontrado == false; j--)
				{
					if (mapaResultado[j][i] == '?')
					{
						encontrado = true;
						positionToGo.first = j;
						positionToGo.second = i;
					}
				}
			}
		}

		break;
	}
}

bool ComportamientoJugador::descubrirMapa(const estado &origen, list<Action> &plan)
{

	estado destino;
	destino.fila = positionToGo.first;
	destino.columna = positionToGo.second;

	cout << "Position to go " << destino.fila << " " << destino.columna << endl;

	pathFinding_A(origen, destino, plan);

	if (plan.size() > 0)
	{
		return true;
	}
	return false;
}

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st)
{
	int fil = st.fila, col = st.columna;

	// calculo cual es la casilla de delante del agente
	switch (st.orientacion)
	{
	case 0:
		fil--;
		break;
	case 1:
		fil--;
		col++;
		break;
	case 2:
		col++;
		break;
	case 3:
		fil++;
		col++;
		break;
	case 4:
		fil++;
		break;
	case 5:
		fil++;
		col--;
		break;
	case 6:
		col--;
		break;
	case 7:
		fil--;
		col--;
		break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil < 0 or fil >= mapaResultado.size())
		return true;
	if (col < 0 or col >= mapaResultado[0].size())
		return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col]))
	{
		// No hay obstaculo, actualizo el parametro st poniendo la casilla de delante.
		st.fila = fil;
		st.columna = col;
		return false;
	}
	else
	{
		return true;
	}
}

struct ComparaEstados
{
	bool operator()(const estado &a, const estado &n) const
	{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};

bool ComparaCostes(const nodo &a, const nodo &n)
{
	return a.path_cost < n.path_cost;
}

bool ComparaFuncion(const nodo &a, const nodo &n)
{
	return a.funcion < n.funcion;
}

void nodoConMayorCoste(list<nodo> &frontier, nodo a)
{

	frontier.sort(ComparaCostes);

	vector<list<nodo>::iterator> toDelete;
	list<nodo>::iterator it;
	bool firstMinor = false;

	for (it = frontier.begin(); it != frontier.end(); ++it)
	{
		if (a.st.fila == it->st.fila and a.st.columna == it->st.columna and a.st.orientacion == it->st.orientacion)
		{

			if (a.path_cost < it->path_cost)
			{
				*it = a;
			}

			if (firstMinor)
			{
				toDelete.push_back(it);
			}

			firstMinor = true;
		}
	}

	if (toDelete.size() > 0)
	{
		for (int i = 0; i < toDelete.size(); i++)
		{
			frontier.erase(toDelete[i]);
		}
	}
}

void nodoConMenorCoste(list<nodo> &frontier, nodo a)
{

	frontier.sort(ComparaFuncion);

	vector<list<nodo>::iterator> toDelete;
	list<nodo>::iterator it;
	bool firstMinor = false;

	for (it = frontier.begin(); it != frontier.end(); ++it)
	{
		if (a.st.fila == it->st.fila and a.st.columna == it->st.columna and a.st.orientacion == it->st.orientacion)
		{

			if (a.funcion < it->funcion)
			{
				*it = a;
			}

			if (firstMinor)
			{
				toDelete.push_back(it);
			}

			firstMinor = true;
		}
	}

	if (toDelete.size() > 0)
	{
		for (int i = 0; i < toDelete.size(); i++)
		{
			frontier.erase(toDelete[i]);
		}
	}
}

int distanceToGoal(estado current, estado goal)
{
	int minSteps = 0;
	int rowDifference = 0;
	int colDifference = 0;

	if (goal.fila < current.fila)
	{
		rowDifference = current.fila - goal.fila;
	}
	else
	{
		rowDifference = goal.fila - current.fila;
	}

	if (goal.columna < current.columna)
	{
		colDifference = current.columna - goal.columna;
	}
	else
	{
		colDifference = goal.columna - current.columna;
	}

	minSteps = rowDifference;
	if (colDifference > rowDifference)
	{
		minSteps = colDifference;
	}

	return minSteps;
}

bool ComportamientoJugador::pathFinding_A(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> explored; // Lista de Cerrados
	explored.clear();

	list<nodo> frontier; // Lista de Abiertos
	frontier.clear();

	nodo current;
	current.st = origen;
	current.path_cost = 0;
	current.funcion = current.path_cost + distanceToGoal(current.st, destino);
	current.secuencia.empty();

	frontier.push_back(current);

	while (!frontier.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{
		// cout << "List size: " << frontier.size() << " " << frontier.front().path_cost << " " << frontier.back().path_cost << endl;

		current = frontier.front();
		frontier.erase(frontier.begin());
		explored.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		hijoTurnR.path_cost += costeCasilla(hijoTurnR, actTURN_R);
		hijoTurnR.funcion = hijoTurnR.path_cost + distanceToGoal(hijoTurnR.st, destino);

		if (explored.find(hijoTurnR.st) == explored.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			frontier.push_back(hijoTurnR);
			nodoConMenorCoste(frontier, hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		hijoTurnL.path_cost += costeCasilla(hijoTurnL, actTURN_L);
		hijoTurnL.funcion = hijoTurnL.path_cost + distanceToGoal(hijoTurnL.st, destino);

		if (explored.find(hijoTurnL.st) == explored.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			frontier.push_back(hijoTurnL);
			nodoConMenorCoste(frontier, hijoTurnL);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		hijoSEMITurnR.path_cost += costeCasilla(hijoSEMITurnR, actSEMITURN_R);
		hijoSEMITurnR.funcion = hijoSEMITurnR.path_cost + distanceToGoal(hijoSEMITurnR.st, destino);

		if (explored.find(hijoSEMITurnR.st) == explored.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			frontier.push_back(hijoSEMITurnR);
			nodoConMenorCoste(frontier, hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		hijoSEMITurnL.path_cost += costeCasilla(hijoSEMITurnL, actSEMITURN_L);
		hijoSEMITurnL.funcion = hijoSEMITurnL.path_cost + distanceToGoal(hijoSEMITurnL.st, destino);

		if (explored.find(hijoSEMITurnL.st) == explored.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			frontier.push_back(hijoSEMITurnL);
			nodoConMenorCoste(frontier, hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			hijoForward.path_cost += costeCasilla(hijoForward, actFORWARD);
			hijoForward.funcion = hijoForward.path_cost + distanceToGoal(hijoForward.st, destino);

			if (explored.find(hijoForward.st) == explored.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				frontier.push_back(hijoForward);
				nodoConMenorCoste(frontier, hijoForward);
			}
		}

		frontier.sort(ComparaFuncion);
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		// cout << "Longitud del plan: " << plan.size() << endl;
		// cout << "Coste del plan = " << current.path_cost << endl;
		PintaPlan(plan);
		//  ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

bool ComportamientoJugador::pathFinding_Costo(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> explored; // Lista de Cerrados

	list<nodo> frontier; // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.path_cost = 0;
	current.secuencia.empty();

	frontier.push_back(current);

	while (!frontier.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{
		// cout << "List size: " << frontier.size() << " " << frontier.front().path_cost << " " << frontier.back().path_cost << endl;

		current = frontier.front();
		frontier.erase(frontier.begin());
		explored.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		hijoTurnR.path_cost += costeCasilla(hijoTurnR, actTURN_R);
		if (explored.find(hijoTurnR.st) == explored.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			frontier.push_back(hijoTurnR);
			nodoConMayorCoste(frontier, hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		hijoTurnL.path_cost += costeCasilla(hijoTurnL, actTURN_L);
		if (explored.find(hijoTurnL.st) == explored.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			frontier.push_back(hijoTurnL);
			nodoConMayorCoste(frontier, hijoTurnL);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		hijoSEMITurnR.path_cost += costeCasilla(hijoSEMITurnR, actSEMITURN_R);
		if (explored.find(hijoSEMITurnR.st) == explored.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			frontier.push_back(hijoSEMITurnR);
			nodoConMayorCoste(frontier, hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		hijoSEMITurnL.path_cost += costeCasilla(hijoSEMITurnL, actSEMITURN_L);
		if (explored.find(hijoSEMITurnL.st) == explored.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			frontier.push_back(hijoSEMITurnL);
			nodoConMayorCoste(frontier, hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			hijoForward.path_cost += costeCasilla(hijoForward, actFORWARD);
			if (explored.find(hijoForward.st) == explored.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				frontier.push_back(hijoForward);
				nodoConMayorCoste(frontier, hijoForward);
			}
		}

		frontier.sort(ComparaCostes);
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		// cout << "Longitud del plan: " << plan.size() << endl;
		// cout << "Coste del plan = " << current.path_cost << endl;
		PintaPlan(plan);
		//  ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	// cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> explored; // Lista de Cerrados
	queue<nodo> frontier;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	frontier.push(current);

	while (!frontier.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		current = frontier.front();
		frontier.pop();
		explored.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		hijoTurnR.path_cost += costeCasilla(hijoTurnR, actTURN_R);
		if (explored.find(hijoTurnR.st) == explored.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			frontier.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		hijoSEMITurnR.path_cost += costeCasilla(hijoSEMITurnR, actSEMITURN_R);
		if (explored.find(hijoSEMITurnR.st) == explored.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			frontier.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		hijoTurnL.path_cost += costeCasilla(hijoTurnL, actTURN_L);
		if (explored.find(hijoTurnL.st) == explored.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			frontier.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		hijoSEMITurnL.path_cost += costeCasilla(hijoSEMITurnL, actSEMITURN_L);
		if (explored.find(hijoSEMITurnL.st) == explored.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			frontier.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{

			hijoForward.path_cost += costeCasilla(hijoForward, actFORWARD);
			if (explored.find(hijoForward.st) == explored.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				frontier.push(hijoForward);
			}
		}
	}

	// cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		// cout << "Cargando el plan\n";
		plan = current.secuencia;
		// cout << "Longitud del plan: " << plan.size() << endl;
		costeAnchura = current.path_cost;
		// PintaPlan(plan);
		//  ver el plan en el mapa
		// VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Implementación de la busqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	stack<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Sacar por la consola la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			cout << "A ";
		}
		else if (*it == actTURN_R)
		{
			cout << "D ";
		}
		else if (*it == actSEMITURN_R)
		{
			cout << "d ";
		}
		else if (*it == actTURN_L)
		{
			cout << "I ";
		}
		else if (*it == actSEMITURN_L)
		{
			cout << "i ";
		}
		else
		{
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

// Funcion auxiliar para poner a 0 todas las casillas de una matriz
void AnularMatriz(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			switch (cst.orientacion)
			{
			case 0:
				cst.fila--;
				break;
			case 1:
				cst.fila--;
				cst.columna++;
				break;
			case 2:
				cst.columna++;
				break;
			case 3:
				cst.fila++;
				cst.columna++;
				break;
			case 4:
				cst.fila++;
				break;
			case 5:
				cst.fila++;
				cst.columna--;
				break;
			case 6:
				cst.columna--;
				break;
			case 7:
				cst.fila--;
				cst.columna--;
				break;
			}
			mapaConPlan[cst.fila][cst.columna] = 1;
		}
		else if (*it == actTURN_R)
		{
			cst.orientacion = (cst.orientacion + 2) % 8;
		}
		else if (*it == actSEMITURN_R)
		{
			cst.orientacion = (cst.orientacion + 1) % 8;
		}
		else if (*it == actTURN_L)
		{
			cst.orientacion = (cst.orientacion + 6) % 8;
		}
		else if (*it == actSEMITURN_L)
		{
			cst.orientacion = (cst.orientacion + 7) % 8;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
