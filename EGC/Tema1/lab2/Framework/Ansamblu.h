/*
 * TEMA 1: GEOMETRY WARS
 *
 * nume:		Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 333 CA
 */
#pragma once
#include <vector>
#include "Object2D.h"
#include "Visual2D.h"
#include "Transform2D.h"
#include "DrawingWindow.h"

using namespace std;

//clasa ce contine o multime(vector) de obiecte care compun nava playerului, 
//inamicii, gloantele si bonusurile
class Ansamblu
{
public:

	vector <Object2D*> Ansamblu::objects2D;	//un Ansamblu este un vector de obiecte
	Point2D Ansamblu::center;				//centrul
	float Ansamblu::raza;					//raza
	float Ansamblu::pasx;					//pasul pe x cu care se translateaza
	float Ansamblu::pasy;					//pasul pe y cu care se translateaza
	float Ansamblu::unghi;					//unghiul cu care se roteste
	float Ansamblu::vit;					//viteza de translatie
	float Ansamblu::rot_vit;				//viteza de rotire
	int Ansamblu::scor;						//punctajul specific

public:

	//constructorul implicit
	Ansamblu() {}
	//constructorul particularizat
	Ansamblu(Point2D _center, float _raza, float _vit, float _rot_vit, int _scor);
	//destructorul
	~Ansamblu() {}
	//functia de adaugare a unui obiect la contextul vizual
	void addObject2D_to_Visual2D(Object2D *o, Visual2D *v);
	//functia de stergere a unui obiect din contextul vizual
	void removeObject2D_from_Visual2D(Object2D *o, Visual2D *v);
	//functia de testare a apartenentei Ansamblului la interiorul ferestrei
	bool interior(int a, int b);
	//functia de procesare a deplasarii unui Ansamblu in fereastra
	void deplasare();
	//functia de stergere a unui Ansamblu din contextul vizual
	void removeAnsamblu_from_Visual2D(Visual2D *v);

};