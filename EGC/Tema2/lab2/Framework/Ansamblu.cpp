/*
 * TEMA 2: CRAZY DRIVER
 *
 * nume:		Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 333 CA
 */
#include "Ansamblu.h"

//clasa ce contine o multime(vector) de obiecte care compun masina playerului, 
//inamicii, strada

//constructor
Ansamblu::Ansamblu(Point3D _center, float _raza, float _vit, float _rot_vit, int _scor)
{
	
	Ansamblu::center = _center;
	Ansamblu::raza = _raza;
	Ansamblu::pasx = 0;
	Ansamblu::pasy = 0;
	Ansamblu::pasz = 0;
	Ansamblu::unghi = 0;
	Ansamblu::vit = _vit;
	Ansamblu::rot_vit = _rot_vit;
	Ansamblu::scor = _scor;
}

//functia de adaugare a unui obiect la contextul vizual
void Ansamblu::addObject3D_to_Visual2D(Object3D *o,Visual2D *v2d)
{
	Ansamblu::objects3D.push_back(o);
	DrawingWindow::addObject3D_to_Visual2D(o,v2d);
}

//functia de stergere a unui obiect din contextul vizual
void Ansamblu::removeObject3D_from_Visual2D(Object3D *o, Visual2D *v2d)
{
	//gaseste obiectul care trebuie sters
	int poz;
	bool gasit = false;
	for (poz = 0; poz < Ansamblu::objects3D.size(); poz++)
	{
		if (o == Ansamblu::objects3D[poz])
		{
			gasit = true;
			break;
		}
	}
	if (gasit)
		Ansamblu::objects3D.erase(Ansamblu::objects3D.begin() + poz);
	DrawingWindow::removeObject3D_from_Visual2D(o, v2d);
}

//functia de procesare a deplasarii unui Ansamblu in fereastra
void Ansamblu::deplasare()
{
	Transform3D::loadIdentityModelMatrix();
	Transform3D::loadIdentityProjectionMatrix();
	Transform3D::translateMatrix(Ansamblu::pasx,Ansamblu::pasy,Ansamblu::pasz);
	Transform3D::translateMatrix((-1)*(Ansamblu::center.x+Ansamblu::pasx),(-1)*(Ansamblu::center.y+Ansamblu::pasy),(-1)*(Ansamblu::center.z+Ansamblu::pasz));
	Transform3D::rotateMatrixOy(Ansamblu::unghi);
	Transform3D::translateMatrix(Ansamblu::center.x+Ansamblu::pasx,Ansamblu::center.y+Ansamblu::pasy,Ansamblu::center.z+Ansamblu::pasz);
	Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);

	int i;
	for(i=0; i < Ansamblu::objects3D.size(); i++)
		Transform3D::applyTransform(Ansamblu::objects3D[i]);
}

//functia de stergere a unui Ansamblu din contextul vizual
void Ansamblu::removeAnsamblu_from_Visual2D(Visual2D *v)
{
	int i;
	for(i=0; i < Ansamblu::objects3D.size(); i++)
		DrawingWindow::removeObject3D_from_Visual2D(Ansamblu::objects3D[i], v);
}