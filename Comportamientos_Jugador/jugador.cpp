#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	actual.fila = sensores.posF;
	actual.columna = sensores.posC;
	actual.orientacion = sensores.sentido;

	// cout << "Fila: " << actual.fila << endl;
	// cout << "Col : " << actual.columna << endl;
	// cout << "Ori : " << actual.orientacion << endl;

	// Capturo los destinos
	cout << "sensores.num_destinos : " << sensores.num_destinos << endl;
	objetivos.clear();
	for (int i = 0; i < sensores.num_destinos; i++)
	{
		estado aux;
		aux.fila = sensores.destino[2 * i];
		aux.columna = sensores.destino[2 * i + 1];
		objetivos.push_back(aux);
	}

	if (!hayPlan)
	{
		hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
	}

	if (hayPlan and plan.size() > 0)
	{
		accion = plan.front();
		plan.erase(plan.begin());
	}
	else
	{
		cout << "No se pudo encontrar plan" << endl;
	}

	if (mapaResultado[actual.fila][actual.columna] == 'K')
	{
		bikiniOn = true;
		zapatillasOn = false;
		// cout << "Bikini on suuuuuuuuuuuuu" << endl;
	}

	if (mapaResultado[actual.fila][actual.columna] == 'D')
	{
		bikiniOn = false;
		zapatillasOn = true;
		// cout << "Zapatillas on suuuuuuuuuuuuu" << endl;
	}

	return accion;
}

int ComportamientoJugador::costeCasilla(estado a, Action act)
{

	char tipoCasilla = mapaResultado[a.fila][a.columna];
	int costeCasilla = 1;

	switch (act)
	{
	case actFORWARD:

		switch (tipoCasilla)
		{
		case 'A':
			if (bikiniOn)
			{
				costeCasilla = 10;
			}
			else
			{
				costeCasilla = 200;
			}
			break;

		case 'B':
			if (zapatillasOn)
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
		}

		break;

	case actSEMITURN_L:
	case actSEMITURN_R:

		switch (tipoCasilla)
		{
		case 'A':
			if (bikiniOn)
			{
				costeCasilla = 2;
			}
			else
			{
				costeCasilla = 300;
			}
			break;

		case 'B':
			if (zapatillasOn)
			{
				costeCasilla = 1;
			}
			else
			{
				costeCasilla = 2;
			}
			break;

		}

		break;

	case actTURN_L:
	case actTURN_R:
		switch (tipoCasilla)
		{
		case 'A':
			if (bikiniOn)
			{
				costeCasilla = 5;
			}
			else
			{
				costeCasilla = 500;
			}
			break;

		case 'B':
			if (zapatillasOn)
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
		return pathFinding_Costo(origen, un_objetivo2, plan);
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 1
		cout << "No implementado aun\n";
		break;
	case 4:
		cout << "Reto 2: Maximizar objetivos\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 2
		cout << "No implementado aun\n";
		break;
	}
	return false;
}

//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el codigo en caracter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla)
{
	if (casilla == 'P' or casilla == 'M')
		return true;
	else
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

struct nodo
{
	estado st;
	list<Action> secuencia;
	int path_cost = 0;
};

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

struct ComparaCostes
{
	bool operator()(const nodo &a, const nodo &n)
	{
		return a.path_cost > n.path_cost;
	}
};

void nodoConMayorCoste(priority_queue<nodo, vector<nodo>, ComparaCostes> &frontier, nodo a)
{
	priority_queue<nodo, vector<nodo>, ComparaCostes> aux;
	bool encontrado = false;

	while (!frontier.empty())
	{

		if (a.st.fila == frontier.top().st.fila and a.st.columna == frontier.top().st.columna)
		{
			if (encontrado == false)
			{
				if (a.path_cost < frontier.top().path_cost)
				{
					aux.push(a);
					
				}
				else
				{
					aux.push(frontier.top());
				}

				encontrado = true;
			}
		}
		else
		{
			aux.push(frontier.top());
		}

		frontier.pop();
	}

	frontier = aux;
}

bool ComportamientoJugador::pathFinding_Costo(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> explored; // Lista de Cerrados

	priority_queue<nodo, vector<nodo>, ComparaCostes> frontier; // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.path_cost = 0;
	current.secuencia.empty();

	frontier.push(current);

	while (!frontier.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		current = frontier.top();
		frontier.pop();
		explored.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		hijoTurnR.path_cost += costeCasilla(hijoTurnR.st, actTURN_R);
		if (explored.find(hijoTurnR.st) == explored.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			frontier.push(hijoTurnR);
		}

		nodoConMayorCoste(frontier, hijoTurnR);

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		hijoSEMITurnR.path_cost += costeCasilla(hijoSEMITurnR.st, actSEMITURN_R);
		if (explored.find(hijoSEMITurnR.st) == explored.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			frontier.push(hijoSEMITurnR);
		}

		nodoConMayorCoste(frontier, hijoSEMITurnR);

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		hijoTurnL.path_cost += costeCasilla(hijoTurnL.st, actTURN_L);
		if (explored.find(hijoTurnL.st) == explored.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			frontier.push(hijoTurnL);
		}

		nodoConMayorCoste(frontier, hijoTurnL);

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		hijoSEMITurnL.path_cost += costeCasilla(hijoSEMITurnL.st, actSEMITURN_L);
		if (explored.find(hijoSEMITurnL.st) == explored.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			frontier.push(hijoSEMITurnL);
		}

		nodoConMayorCoste(frontier, hijoSEMITurnL);

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			hijoForward.path_cost += costeCasilla(hijoForward.st, actFORWARD);
			if (explored.find(hijoForward.st) == explored.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				frontier.push(hijoForward);
			}

			nodoConMayorCoste(frontier, hijoForward);
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		cout << "Coste del plan = " << current.path_cost << endl;
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

bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
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
		if (explored.find(hijoTurnR.st) == explored.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			frontier.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (explored.find(hijoSEMITurnR.st) == explored.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			frontier.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		if (explored.find(hijoTurnL.st) == explored.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			frontier.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		if (explored.find(hijoSEMITurnL.st) == explored.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			frontier.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (explored.find(hijoForward.st) == explored.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				frontier.push(hijoForward);
			}
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
			cout << "I ";
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
