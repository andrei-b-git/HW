/*
 * TEMA 2: CRAZY DRIVER
 *
 * nume:		Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 333 CA
 */
#pragma once
#include <vector>
#include "Object3D.h"
#include "Visual2D.h"
#include "Transform3D.h"
#include "DrawingWindow.h"

using namespace std;

//clasa ce contine o multime(vector) de obiecte care compun masina playerului, 
//inamicii, strada
class Ansamblu
{
public:

	vector <Object3D*> Ansamblu::objects3D;	//un Ansamblu este un vector de obiecte
	Point3D Ansamblu::center;				//centrul
	float Ansamblu::raza;					//raza
	float Ansamblu::pasx;					//pasul pe x cu care se translateaza
	float Ansamblu::pasy;					//pasul pe y cu care se translateaza
	float Ansamblu::pasz;					//pasul pe z cu care se translateaza
	float Ansamblu::unghi;					//unghiul cu care se roteste
	float Ansamblu::vit;					//viteza de translatie
	float Ansamblu::rot_vit;				//viteza de rotire
	int Ansamblu::scor;						//punctajul specific

public:

	//constructorul implicit
	Ansamblu() {}
	//constructorul particularizat
	Ansamblu(Point3D _center, float _raza, float _vit, float _rot_vit, int _scor);
	//destructorul
	~Ansamblu() {}
	//functia de adaugare a unui obiect la contextul vizual
	void addObject3D_to_Visual2D(Object3D *o, Visual2D *v);
	//functia de stergere a unui obiect din contextul vizual
	void removeObject3D_from_Visual2D(Object3D *o, Visual2D *v);
	//functia de procesare a deplasarii unui Ansamblu in fereastra
	void deplasare();
	//functia de stergere a unui Ansamblu din contextul vizual
	void removeAnsamblu_from_Visual2D(Visual2D *v);

};