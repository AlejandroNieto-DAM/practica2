#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila;
  int columna;
  int orientacion;
  int equip = 0;
};

struct nodo
{
	estado st;
	list<Action> secuencia;
	double path_cost = 0;
  double heuristica = 0;
  int funcion = 0;
};


class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      init();
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      init();
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    estado actual;
    list<estado> objetivos;
    list<Action> plan;
    bool hayPlan;
    pair<int, int> positionToGo;
    Action ultimaAccion;
    list<estado> subObjetivos;
    int costeCosto;
    int costeAnchura;
    int activado;
    int brujula;
    int fil;
    int col;
    bool firstIteration;
  

    // Métodos privados de la clase
    void init();

    bool pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Costo(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_A(const estado &origen, const estado &destino, list<Action> &plan);

    bool descubrirMapa(const estado &origen, list<Action> &plan);
    void pruebaMapa();
    bool posicionNoConviene(estado actual);
    int distanceToGoal(nodo current, estado goal);

    void busquedaPuntoLejano(estado current, int nivel);

    int costeCasilla(nodo &a, Action act);
    void rellenarVisionCompleta(Sensores sensores);
    void actualizarBrujulaPosicion();
    void actualizarPosicion();

    pair<int, int> findCasilla(char type);

    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);


};

#endif
