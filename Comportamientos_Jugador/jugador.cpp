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

bool inPoints(estado st, list<estado> &objetivos)
{
	list<estado>::iterator it;
	list<estado>::iterator pos;
	bool encontrado = false;
	for (it = objetivos.begin(); it != objetivos.end() and encontrado == false; ++it)
	{
		if (st.fila == (*it).fila and st.columna == (*it).columna)
		{
			encontrado = true;
			pos = it;
		}
	}

	if (encontrado == true)
	{
		//cout << "Obj borrado" << endl;
		objetivos.erase(pos);
	}

	return encontrado;
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

	bool noConviene = false;
	switch (mapaResultado[actual.fila][actual.columna])
	{
	case 'B':
		if (actual.equip != 1)
		{
			noConviene = true;
		}
		break;
	case 'A':
		if (actual.equip != 2)
		{
			noConviene = true;
		}
		break;
	case 'M':
	case 'P':
	case 'a':
	case 'l':
		noConviene = true;
		break;
	}

	return noConviene;
}

pair<int, int> ComportamientoJugador::findCasilla(char type)
{
	for (int i = 3; i < mapaResultado.size() - 4; i++)
	{
		for (int j = 3; j < mapaResultado.size() - 4; j++)
		{
			if (mapaResultado[i][j] == type)
			{
				return pair<int, int>(i, j);
			}
		}
	}

	return pair<int, int>(-1, -1);
}

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actWHEREIS;

	if (sensores.nivel == 4 and sensores.colision == true or sensores.reset == 1)
	{
		accion = actWHEREIS;
		plan.clear();
		activado = true;
		firstIteration = false;
		return accion;
		//cout << "Reset " << endl;
	}

	if (activado and sensores.nivel == 4 and firstIteration and sensores.colision == false)
	{
		actualizarPosicion();
		actualizarBrujulaPosicion();
		//cout << "Actualizacion" << endl;
	}

	if (sensores.nivel <= 3 or (sensores.nivel == 4 and !firstIteration and activado))
	{

		actual.fila = sensores.posF;
		actual.columna = sensores.posC;
		actual.orientacion = sensores.sentido;

		firstIteration = true;
	}

	if (sensores.nivel <= 3 or (sensores.nivel == 4 and activado))
	{
		if (mapaResultado[actual.fila][actual.columna] == 'D')
		{
			actual.equip = 1;
		}

		if (mapaResultado[actual.fila][actual.columna] == 'K')
		{
			actual.equip = 2;
		}
	}

	//cout << "Fila: " << actual.fila << endl;
	//cout << "Col : " << actual.columna << endl;
	//cout << "Ori : " << actual.orientacion << endl;

	// Capturo los destinos

	objetivos.clear();
	for (int i = 0; i < sensores.num_destinos; i++)
	{
		estado aux;
		aux.fila = sensores.destino[2 * i];
		aux.columna = sensores.destino[2 * i + 1];
		objetivos.push_back(aux);
	}

	//cout << "Objetivo primero = " << objetivos.front().fila << " " << objetivos.front().columna << endl;
	//cout << "Subobjetivos size = " << subObjetivos.size() << endl;

	if (sensores.nivel == 4 and subObjetivos.size() == 0)
	{
		subObjetivos = objetivos;
	}

	if ((sensores.nivel == 4 and inPoints(actual, subObjetivos)) or (sensores.nivel == 4 and positionToGo.first > -1))
	{
		positionToGo.first = subObjetivos.front().fila;
		positionToGo.second = subObjetivos.front().columna;
	}

	if (hayPlan and mapaResultado[actual.fila][actual.columna] == 'B' and actual.equip != 1 and sensores.nivel >= 3)
	{
		pair<int, int> posicionZapatillas = findCasilla('D');
		if (posicionZapatillas.first != -1)
		{
			estado goal;
			goal.fila = posicionZapatillas.first;
			goal.columna = posicionZapatillas.second;

			hayPlan = pathFinding_A(actual, goal, plan);
		}
	}

	if (hayPlan and mapaResultado[actual.fila][actual.columna] == 'A' and actual.equip != 2 and sensores.nivel >= 3)
	{
		pair<int, int> posicionBikini = findCasilla('K');
		if (posicionBikini.first != -1)
		{
			estado goal;
			goal.fila = posicionBikini.first;
			goal.columna = posicionBikini.second;

			hayPlan = pathFinding_A(actual, goal, plan);
		}
	}

	if (hayPlan and sensores.bateria < 1000 and sensores.nivel >= 3)
	{
		pair<int, int> posicionRecarga = findCasilla('X');
		if (posicionRecarga.first != -1)
		{
			estado goal;
			goal.fila = posicionRecarga.first;
			goal.columna = posicionRecarga.second;

			hayPlan = pathFinding_A(actual, goal, plan);

			for (int i = 0; i < 100; i++)
			{
				plan.push_back(actIDLE);
			}
		}
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
		if (!hayPlan or (plan.front() == actFORWARD and posicionNoConviene(actual) and (ultimaAccion != actTURN_R or ultimaAccion != actTURN_L or ultimaAccion != actSEMITURN_L or ultimaAccion != actSEMITURN_R)))
		{
			hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
		}
	}
	else if (sensores.nivel == 3)
	{
		if (!hayPlan or (plan.front() == actFORWARD and posicionNoConviene(actual) and (ultimaAccion != actTURN_R or ultimaAccion != actTURN_L)))
		{
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
	anterior = actual;
	activado = true;

	return accion;
}

int ComportamientoJugador::costeCasilla(nodo &a, Action act)
{

	char tipoCasilla = mapaResultado[a.st.fila][a.st.columna];

	int costeCasilla = 1;

	switch (act)
	{

	case actSEMITURN_L:
	case actSEMITURN_R:

		switch (tipoCasilla)
		{
		case 'A':
			if (a.st.equip == 2)
			{
				costeCasilla = 2;
			}
			else
			{
				costeCasilla = 300;
			}
			break;

		case 'B':
			if (a.st.equip == 1)
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
	case actTURN_R:
		switch (tipoCasilla)
		{
		case 'A':
			if (a.st.equip == 2)
			{
				costeCasilla = 5;
			}
			else
			{
				costeCasilla = 500;
			}
			break;

		case 'B':
			if (a.st.equip == 1)
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
			if (a.st.equip == 2)
			{
				costeCasilla = 10;
			}
			else
			{
				costeCasilla = 200;
			}
			break;

		case 'B':
			if (a.st.equip == 1)
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

bool ComportamientoJugador::inTablero(estado position)
{

	if (position.fila >= 3 and position.fila < mapaResultado.size() - 3 and position.columna >= 3 and position.columna < mapaResultado.size() - 3)
	{
		return true;
	}

	return false;
}
// Llama al algoritmo de busqueda que se usara en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan)
{
	estado un_objetivo;

	switch (level)
	{
	case 0:
		cout << "Demo\n";

		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Profundidad(origen, un_objetivo, plan);
		break;

	case 1:
		cout << "Optimo numero de acciones\n";
		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Anchura(origen, un_objetivo, plan);
		break;
	case 2:
		cout << "Optimo en coste\n";
		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_A(origen, un_objetivo, plan);
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";
		if (positionToGo.first != -1 and positionToGo.second != -1)
		{

			if ((inTablero(subObjetivos.front()) and subObjetivos.front().fila == origen.fila and subObjetivos.front().columna == origen.columna) or
			(mapaResultado[subObjetivos.front().fila][subObjetivos.front().columna] == 'P') or
			(mapaResultado[subObjetivos.front().fila][subObjetivos.front().columna] == 'A') or
			(mapaResultado[subObjetivos.front().fila][subObjetivos.front().columna] == 'B'))
			{
				subObjetivos.pop_front();
			}

			if (subObjetivos.size() == 0)
			{
				busquedaPuntoLejano(origen, level);
			}

			if(subObjetivos.size() > 0)
				return descubrirMapaNvl3(origen, plan);
		}

		break;

	case 4:
		cout << "Reto 2: Maximizar objetivos\n";
		if (positionToGo.first > 0 and positionToGo.second > 0)
		{
			return descubrirMapa(origen, plan);
		}
		return false;
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
	
	cout << "Buscamos puntito" << endl;
	switch (current.orientacion)
	{

	case 0:
	case 1:
	case 7:

		for (int i = 3; i < mapaResultado.size() - 3 and encontrado == false; i++)
		{
			for (int j = 3; j < mapaResultado.size() - 3; j++)
			{
				if (mapaResultado[i][j] == '?')
				{
					encontrado = true;
					positionToGo.first = i;
					positionToGo.second = j;
				}
			}
		}

		if (encontrado == true)
		{

			int x_value = abs(positionToGo.first - current.fila) / 2;
			int y_value = abs(positionToGo.second - current.columna) / 2; 
			aux.fila = current.fila - x_value;

			if(positionToGo.second > current.columna)
				aux.columna = current.columna + y_value;
			else
				aux.columna = current.columna - y_value;

			cout << " " << aux.fila << " " << aux.columna << endl;
			if(inTablero(aux))
				if(mapaResultado[aux.fila][aux.columna] != 'A' and mapaResultado[aux.fila][aux.columna] != 'B')
					subObjetivos.push_back(aux);

			aux.fila = positionToGo.first;
			aux.columna = positionToGo.second;
			subObjetivos.push_back(aux);
		}

		break;

	case 3:
	case 4:
	case 5:

		for (int i = mapaResultado.size() - 4; i >= 3 and encontrado == false; i--)
		{
			for (int j = mapaResultado.size() - 4; j >= 3; j--)
			{
				if (mapaResultado[i][j] == '?')
				{
					encontrado = true;
					positionToGo.first = i;
					positionToGo.second = j;
				}
			}
		}

		if (encontrado == true)
		{
			int x_value = abs(positionToGo.first - current.fila) / 2;
			int y_value = abs(positionToGo.second - current.columna) / 2; 
			aux.fila = current.fila + x_value;
			
			if(positionToGo.second > current.columna)
				aux.columna = current.columna - y_value;
			else 
				aux.columna = current.columna + y_value;

			cout << " " << aux.fila << " " << aux.columna << endl;
			if(inTablero(aux))
				if(mapaResultado[aux.fila][aux.columna] != 'A' and mapaResultado[aux.fila][aux.columna] != 'B')
					subObjetivos.push_back(aux);

			aux.fila = positionToGo.first;
			aux.columna = positionToGo.second;
			subObjetivos.push_back(aux);
		}

		break;

	case 2:

		for (int i = mapaResultado.size() - 4; i >= 3 and encontrado == false; i--)
		{
			for (int j = 3; j < mapaResultado.size() - 3; j++)
			{
				if (mapaResultado[j][i] == '?')
				{
					encontrado = true;
					positionToGo.first = j;
					positionToGo.second = i;
				}
			}
		}

		if (encontrado == true)
		{
			int x_value = abs(positionToGo.first - current.fila) / 2;
			int y_value = abs(positionToGo.second - current.columna) / 2; 
			
			if(positionToGo.first > current.fila)
				aux.fila = current.fila + x_value;
			else
				aux.fila = current.fila - x_value;

			aux.columna = current.columna + y_value;

			cout << " " << aux.fila << " " << aux.columna << endl;
			if(inTablero(aux))
				if(mapaResultado[aux.fila][aux.columna] != 'A' and mapaResultado[aux.fila][aux.columna] != 'B')
					subObjetivos.push_back(aux);

			aux.fila = positionToGo.first;
			aux.columna = positionToGo.second;
			subObjetivos.push_back(aux);
		}

		break;

	case 6:

		for (int i = 3; i < mapaResultado.size() - 3 and encontrado == false; i++)
		{
			for (int j = mapaResultado.size() - 4; j >= 3; j--)
			{
				if (mapaResultado[j][i] == '?')
				{
					encontrado = true;
					positionToGo.first = j;
					positionToGo.second = i;
				}
			}
		}

		if (encontrado == true)
		{
			int x_value = abs(positionToGo.first - current.fila) / 2;
			int y_value = abs(positionToGo.second - current.columna) / 2; 
			
			if(positionToGo.first > current.fila)
				aux.fila = current.fila - x_value;
			else
				aux.fila = current.fila + x_value;

			aux.columna = current.columna - y_value;
			
						cout << " " << aux.fila << " " << aux.columna << endl;
			if(inTablero(aux))
				if(mapaResultado[aux.fila][aux.columna] != 'A' and mapaResultado[aux.fila][aux.columna] != 'B')
					subObjetivos.push_back(aux);

			aux.fila = positionToGo.first;
			aux.columna = positionToGo.second;
			subObjetivos.push_back(aux);
		}

		break;
	}

	/*if (encontrado == false)
	{
		cout << "Vamos pa las de fuera" << endl;
		for (int i = 0; i < mapaResultado.size() and encontrado == false; i++)
		{
			for (int j = 0; j < mapaResultado.size(); j++)
			{
				if (mapaResultado[i][j] == '?')
				{
					encontrado = true;
					positionToGo.first = i;
					positionToGo.second = j;
				}
			}
		}

		
		aux.fila = positionToGo.first;
		aux.columna = positionToGo.second;

		cout << "Antes conversion " << aux.fila << " " << aux.columna << endl;
		if(aux.fila < 3){
			aux.fila += (3 - aux.fila);
			cout << "Entro al mas de fila " << endl;
		} else if(aux.fila > (mapaResultado.size() - 3)){
			aux.fila -= (mapaResultado.size() - aux.fila);
			cout << "Entro al mas de fila por arriba " << endl;
		}
			
		
		
		if(aux.columna < 3){
			cout << "Entro al mas de col " << endl;
			aux.columna += (3 - aux.columna);
		} else if(aux.columna > mapaResultado.size() - 3){
			aux.columna -= (mapaResultado.size() - aux.columna);
			cout << "Entro al mas de col por arriba " << endl;
		}
			

		cout << "Despues conversion " << aux.fila << " " << aux.columna << endl;

		subObjetivos.push_back(aux);

		
	}*/
}

bool ComportamientoJugador::descubrirMapa(const estado &origen, list<Action> &plan)
{

	estado destino;
	destino.fila = positionToGo.first;
	destino.columna = positionToGo.second;

	cout << "Position to go " << destino.fila << " " << destino.columna << endl;
	if (destino.fila == -1 and destino.columna == -1)
	{
		return false;
	}

	pathFinding_A(origen, destino, plan);

	if (plan.size() > 0)
	{
		return true;
	}
	return false;
}

bool ComportamientoJugador::descubrirMapaNvl3(const estado &origen, list<Action> &plan)
{

	estado destino;
	destino.fila = subObjetivos.front().fila;
	destino.columna = subObjetivos.front().columna;

	cout << "Position to go nvl3 what happens " << destino.fila << " " << destino.columna << endl;
	if (!inTablero(destino))
	{
		if(subObjetivos.size() > 0)
			subObjetivos.pop_front();

		return false;
	}

	pathFinding_A(origen, destino, plan);
	plan.push_back(actIDLE);

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

struct ComparaEstadosNivel2
{
	bool operator()(const estado &a, const estado &n) const
	{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.equip > n.equip))
			return true;
		else
			return false;
	}
};

bool ComparaCostes(const nodo &a, const nodo &n)
{
	return a.path_cost < n.path_cost;
}

int ComportamientoJugador::distanceToGoal(nodo current, estado goal)
{
	int minSteps = 0;
	int rowDifference = 0;
	int colDifference = 0;

	if (goal.fila < current.st.fila)
	{
		rowDifference = current.st.fila - goal.fila;
	}
	else
	{
		rowDifference = goal.fila - current.st.fila;
	}

	if (goal.columna < current.st.columna)
	{
		colDifference = current.st.columna - goal.columna;
	}
	else
	{
		colDifference = goal.columna - current.st.columna;
	}

	minSteps = rowDifference;
	if (colDifference > rowDifference)
	{
		minSteps = colDifference;
	}

	switch (current.st.orientacion)
	{
	case 0:
		if (goal.fila > current.st.fila)
		{
			minSteps += 1;
		}
		break;
	case 1:
		if (goal.fila > current.st.fila and goal.columna < current.st.columna)
		{
			minSteps += 1;
		}
		break;
	case 2:
		if (current.st.columna > goal.columna)
		{
			minSteps += 1;
		}
		break;
	case 3:
		if (goal.fila < current.st.fila and goal.columna < current.st.columna)
		{
			minSteps += 1;
		}
		break;
	case 4:
		if (goal.fila < current.st.fila)
		{
			minSteps += 1;
		}
		break;
	case 5:
		if (goal.fila < current.st.fila and goal.columna > current.st.columna)
		{
			minSteps += 1;
		}
		break;
	case 6:
		if (goal.columna > current.st.columna)
		{
			minSteps += 1;
		}
		break;
	case 7:
		if (goal.fila > current.st.fila and goal.columna > current.st.columna)
		{
			minSteps += 1;
		}
		break;
	}
	return minSteps;
}

struct ComparaFuncionCoste
{
	bool operator()(const nodo a, const nodo n)
	{
		return a.funcion > n.funcion;
	}
};

bool ComportamientoJugador::pathFinding_A(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	//cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstadosNivel2> explored; // Lista de Cerrados
	explored.clear();

	priority_queue<nodo, vector<nodo>, ComparaFuncionCoste> frontier; // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.heuristica = distanceToGoal(current, destino);
	current.path_cost = 0;
	current.funcion = current.path_cost + current.heuristica;
	current.secuencia.empty();

	frontier.push(current);

	while (!frontier.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{
		// cout << "Abiertos size: " << frontier.size() << endl;
		// cout << "Cerrados size: " << explored.size() << endl;

		if (!frontier.empty())
		{
			current = frontier.top();
			while (explored.find(current.st) != explored.end())
			{
				frontier.pop();
				current = frontier.top();
			}
		}

		frontier.pop();
		explored.insert(current.st);

		if (mapaResultado[current.st.fila][current.st.columna] == 'K')
		{
			current.st.equip = 2;
		}
		else if (mapaResultado[current.st.fila][current.st.columna] == 'D')
		{
			current.st.equip = 1;
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			hijoForward.path_cost += costeCasilla(hijoForward, actFORWARD);
			hijoForward.heuristica = distanceToGoal(hijoForward, destino);
			hijoForward.funcion = hijoForward.path_cost + hijoForward.heuristica;

			if (explored.find(hijoForward.st) == explored.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				frontier.push(hijoForward);
			}
		}

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		hijoTurnR.path_cost += costeCasilla(hijoTurnR, actTURN_R);
		hijoTurnR.heuristica = distanceToGoal(hijoTurnR, destino);
		hijoTurnR.funcion = hijoTurnR.path_cost + hijoTurnR.heuristica;

		if (explored.find(hijoTurnR.st) == explored.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			frontier.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		hijoTurnL.path_cost += costeCasilla(hijoTurnL, actTURN_L);
		hijoTurnL.heuristica = distanceToGoal(hijoTurnL, destino);
		hijoTurnL.funcion = hijoTurnL.path_cost + hijoTurnL.heuristica;

		if (explored.find(hijoTurnL.st) == explored.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			frontier.push(hijoTurnL);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		hijoSEMITurnR.path_cost += costeCasilla(hijoSEMITurnR, actSEMITURN_R);
		hijoSEMITurnR.heuristica = distanceToGoal(hijoSEMITurnR, destino);
		hijoSEMITurnR.funcion = hijoSEMITurnR.path_cost + hijoSEMITurnR.heuristica;

		if (explored.find(hijoSEMITurnR.st) == explored.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			frontier.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		hijoSEMITurnL.path_cost += costeCasilla(hijoSEMITurnL, actSEMITURN_L);
		hijoSEMITurnL.heuristica = distanceToGoal(hijoSEMITurnL, destino);
		hijoSEMITurnL.funcion = hijoSEMITurnL.path_cost + hijoSEMITurnL.heuristica;

		if (explored.find(hijoSEMITurnR.st) == explored.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			frontier.push(hijoSEMITurnL);
		}
	}

	//cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		//cout << "Cargando el plan\n";
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
		//cout << "No encontrado plan\n";
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