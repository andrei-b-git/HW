/*
 * TEMA 1: GEOMETRY WARS
 *
 * nume:		Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 333 CA
 */
#include "Ansamblu.h"

//clasa ce contine o multime(vector) de obiecte care compun nava playerului, 
//inamicii, gloantele si bonusurile

//constructor
Ansamblu::Ansamblu(Point2D _center, float _raza, float _vit, float _rot_vit, int _scor)
{
	
	Ansamblu::center = _center;
	Ansamblu::raza = _raza;
	Ansamblu::pasx = 0;
	Ansamblu::pasy = 0;
	Ansamblu::unghi = 0;
	Ansamblu::vit = _vit;
	Ansamblu::rot_vit = _rot_vit;
	Ansamblu::scor = _scor;
}

//functia de adaugare a unui obiect la contextul vizual
void Ansamblu::addObject2D_to_Visual2D(Object2D *o,Visual2D *v2d)
{
	Ansamblu::objects2D.push_back(o);
	DrawingWindow::addObject2D_to_Visual2D(o,v2d);
}

//functia de stergere a unui obiect din contextul vizual
void Ansamblu::removeObject2D_from_Visual2D(Object2D *o, Visual2D *v2d)
{
	//gaseste obiectul care trebuie sters
	int poz;
	bool gasit = false;
	for (poz = 0; poz < Ansamblu::objects2D.size(); poz++)
	{
		if (o == Ansamblu::objects2D[poz])
		{
			gasit = true;
			break;
		}
	}
	if (gasit)
		Ansamblu::objects2D.erase(Ansamblu::objects2D.begin() + poz);
	DrawingWindow::removeObject2D_from_Visual2D(o, v2d);
}

//functia de testare a apartenentei Ansamblului la interiorul ferestrei
bool Ansamblu::interior(int a, int b)
{
	if((pasx+a*vit*cos(unghi)) >= -center.x+raza && 
	   (pasx+a*vit*cos(unghi)) <= DrawingWindow::width-center.x-raza && 
	   (pasy+b*vit*sin(unghi)) >= DrawingWindow::height/11-center.y+raza &&
	   (pasy+b*vit*sin(unghi)) <= DrawingWindow::height-center.y-raza)

	   return true;

	return false;
}

//functia de procesare a deplasarii unui Ansamblu in fereastra
void Ansamblu::deplasare()
{
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(Ansamblu::pasx,Ansamblu::pasy);
	Transform2D::translateMatrix((-1)*(Ansamblu::center.x+Ansamblu::pasx),(-1)*(Ansamblu::center.y+Ansamblu::pasy));
	Transform2D::rotateMatrix(Ansamblu::unghi);
	Transform2D::translateMatrix(Ansamblu::center.x+Ansamblu::pasx,Ansamblu::center.y+Ansamblu::pasy);

	int i;
	for(i=0; i < Ansamblu::objects2D.size(); i++)
		Transform2D::applyTransform(Ansamblu::objects2D[i]);
}

//functia de stergere a unui Ansamblu din contextul vizual
void Ansamblu::removeAnsamblu_from_Visual2D(Visual2D *v)
{
	int i;
	for(i=0; i < Ansamblu::objects2D.size(); i++)
		DrawingWindow::removeObject2D_from_Visual2D(Ansamblu::objects2D[i], v);
}