/*
 * TEMA 1: GEOMETRY WARS
 *
 * nume:		Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 333 CA
 */
#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define PI 3.14159265358979323846

using namespace std;

// Configurarea parametrilor jocului:

//Contexte vizuale, scor, level, viata:
Visual2D *titlu, *teren;	//cele doua contexte vizuale
Text *scor, *viata, *lvl;
long score = 0;				//scorul curent
int score_int = 500;		//punctajul dintre nivele
int life = 5;				//numarul curent de vieti
int level = 1;				//nivelul curent
int wave_int = 13;			//numarul de sec dintre wave-uri
int copy_wave_int = 13;

Text *lvlup = NULL;			//mesaj la lvl-up
int text_lvlup = 0;			//contor de timp pana la disparitia textului

Text *minushp = NULL;		//mesaj la scaderea vietii
int text_minushp = 0;		//contor de timp pana la disparitia textului

//Player:
Ansamblu *player;			//corpul navei jucatorului
float v = 6;				//viteza
float vr = 0.1;				//viteza de rotatie
float vit_temp = 0;			//viteza de moment
float pen = 2;				//penalizarea vitezei la activarea burghiului
float copy_pen = 2;
float acc = 0.1;			//acceleratia/deceleratia
float copy_acc = 0.1;

//Burghiu:
Polygon2D *drill;			//corpul burghiului
bool burghiu = false;		//indica daca burghiul este activat sau nu

//Gloante:
vector <Ansamblu*> gloante;//multimea gloantelor din fereastra
bool shoot = false;			//indica daca playerul trage cu gloante
float r_gl = 7;				//raza gloantelor
float copy_r_gl = 7;
float v_gl = 10;			//viteza gloantelor
float copy_v_gl = 10;
int fire_rate = 50;			//rata de tragere
int copy_fire_rate = 50;
int glont_int = 0;			//contor de timp pana la urmatorul glont

//Inamici (4 tipuri):
//wave-urile inamicilor de un anumit tip (sunt 4 tipuri in total)
vector <Ansamblu*>     inamic1,    inamic2,    inamic3,  inamic4;
//viteza tuturor inamicilor de tipul respectiv
float				 v1 =    3,  v2 = 1.25,  v3 =    1,  v4 =  3;
//viteza de rotire a tuturor inamicilor de tipul respectiv
float				vr1 = 0.25, vr2 = 0.25, vr3 = 0.25, vr4 =  0;
//punctajul tuturor inamicilor de tipul respectiv
int					 s1 =   20,  s2 =   30,  s3 =   10,  s4 = 40;
float v_inc = 0.25;			//cu cat creste viteza la fiecare level
int col_int = 0;			//contor de timp intre coliziuni

//Bonusuri (7 tipuri):
Ansamblu *bonus;			//exista un singur bonus la un moment de timp
int bid = 0;				//bonus id
int b_int = 10;				//intervalul de timp dintre doua bonusuri (sec)
int copy_b_int = 10;
int b_del = 3;				//numarul de sec pana la disparitia bonusului
int b_s2 = 100;				//punctajul bonus de tip 2
int b_s3 = 200;				//punctajul bonus de tip 3
float b_v4 = 2;				//viteza bonus de tip 4
float b_acc4 = 0.5;			//acceleratia bonus de tip 4
bool time_stop = false;		//indica daca s-a oprit animatia pentru inamici
bool invinc = false;		//indica daca playerul este invincibil
int b_time = 0;				//contor de timp pana la disparitia efectului bonusului
int b_lim = 200;			//numarul de sec pana la disparitia efectului bonusului

Text *plushp = NULL;		//mesaj pentru bonusul de tip 1
int text_plushp = 0;		//contor de timp pana la disparitia textului
Text *pluspct1 = NULL;		//mesaj pentru bonusul de tip 2
int text_pluspct1 = 0;		//contor de timp pana la disparitia textului
Text *pluspct2 = NULL;		//mesaj pentru bonusul de tip 3
int text_pluspct2 = 0;		//contor de timp pana la disparitia textului
Text *plusvit = NULL;		//mesaj pentru bonusul de tip 4
int text_plusvit = 0;		//contor de timp pana la disparitia textului
Text *plusfr = NULL;		//mesaj pentru bonusul de tip 5
int text_plusfr = 0;		//contor de timp pana la disparitia textului
Text *timestop = NULL;		//mesaj pentru bonusul de tip 6
int text_timestop = 0;		//contor de timp pana la disparitia textului
Text *invul = NULL;			//mesaj pentru bonusul de tip 7
int text_invul = 0;			//contor de timp pana la disparitia textului

int text_lim = 100;

bool full = false;			//fullscreen sau nu
bool game_over = false;		//indica faptul ca s-a terminat jocul

Ansamblu *aux = NULL;		//auxiliar

//functia de creare a corpului playerului si a corpului burghiului
void creare_player(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	player = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);

	Circle2D *cerc1 = new Circle2D(Point2D(c.x,c.y),30,Color(1,0,0),false);
	player->addObject2D_to_Visual2D(cerc1,teren);

	Circle2D *cerc2 = new Circle2D(Point2D(c.x,c.y),28,Color(1,0,0),false);
	player->addObject2D_to_Visual2D(cerc2,teren);

	Polygon2D *poly1 = new Polygon2D(Color(1,0,0),false);
	poly1->addPoint(Point2D(c.x+13,c.y-10));
	poly1->addPoint(Point2D(c.x,c.y-17));
	poly1->addPoint(Point2D(c.x-17,c.y));
	poly1->addPoint(Point2D(c.x,c.y+17));
	poly1->addPoint(Point2D(c.x+13,c.y+10));
	poly1->addPoint(Point2D(c.x,c.y+15));
	poly1->addPoint(Point2D(c.x-15,c.y));
	poly1->addPoint(Point2D(c.x,c.y-15));
	player->addObject2D_to_Visual2D(poly1,teren);

	Polygon2D *poly2 = new Polygon2D(Color(1,0,0),false);
	poly2->addPoint(Point2D(c.x+13,c.y-10));
	poly2->addPoint(Point2D(c.x,c.y-12));
	poly2->addPoint(Point2D(c.x-8,c.y));
	poly2->addPoint(Point2D(c.x,c.y+12));
	poly2->addPoint(Point2D(c.x+13,c.y+10));
	poly2->addPoint(Point2D(c.x+1,c.y+10));
	poly2->addPoint(Point2D(c.x-6,c.y));
	poly2->addPoint(Point2D(c.x+1,c.y-10));
	player->addObject2D_to_Visual2D(poly2,teren);

	//burghiu
	drill = new Polygon2D(Color(1,0,0),true);
	drill->addPoint(Point2D(c.x+35,c.y-15));
	drill->addPoint(Point2D(c.x+35,c.y+15));
	drill->addPoint(Point2D(c.x+70,c.y));
}

//functia de creare a corpului unui glont
void creare_glont(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = player->center.x;
	c.y = player->center.y;
	gloante.push_back(new Ansamblu(c,_raza,_vit,_rot_vit,_scor));

	Circle2D *cerc = new Circle2D(Point2D(c.x,c.y),_raza,Color(1,0,0),true);
	gloante[gloante.size()-1]->addObject2D_to_Visual2D(cerc,teren);
}

//functia de creare a corpului unui inamic de tip 1
void creare_inamic1(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	inamic1.push_back(new Ansamblu(c,_raza,_vit,_rot_vit,_scor));	//adaugare la wave

	Rectangle2D *enemy1a = new Rectangle2D(Point2D(c.x-15,c.y-5),20,20,Color(1,0,1),false);
	inamic1[inamic1.size()-1]->addObject2D_to_Visual2D(enemy1a,teren);

	Rectangle2D *enemy1b = new Rectangle2D(Point2D(c.x-14,c.y-4),18,18,Color(1,0,1),false);
	inamic1[inamic1.size()-1]->addObject2D_to_Visual2D(enemy1b,teren);

	Rectangle2D *enemy1c = new Rectangle2D(Point2D(c.x-5,c.y-15),20,20,Color(1,0,1),false);
	inamic1[inamic1.size()-1]->addObject2D_to_Visual2D(enemy1c,teren);

	Rectangle2D *enemy1d = new Rectangle2D(Point2D(c.x-4,c.y-14),18,18,Color(1,0,1),false);
	inamic1[inamic1.size()-1]->addObject2D_to_Visual2D(enemy1d,teren);
}

//functia de creare a unui wave de 4 inamici de tip 1 din cele 4 colturi ale ferestrei
void creare_inamici1()
{
	float raza = 15*sqrt(2);	//raza inamicului de tip 1
	int j;
	creare_inamic1(DrawingWindow::width-50, DrawingWindow::height/11+50, raza, v1, vr1, s1);
	creare_inamic1(DrawingWindow::width-50, DrawingWindow::height-50, raza, v1, vr1, s1);
	//rotire catre centrul ferestrei
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*inamic1[inamic1.size()-1]->center.x,(-1)*inamic1[inamic1.size()-1]->center.y);
	Transform2D::rotateMatrix(PI/2);
	Transform2D::translateMatrix(inamic1[inamic1.size()-1]->center.x,inamic1[inamic1.size()-1]->center.y);
	for(j=0; j < inamic1[inamic1.size()-1]->objects2D.size(); j++)
		Transform2D::applyTransform(inamic1[inamic1.size()-1]->objects2D[j]);
	creare_inamic1(50, DrawingWindow::height-50, raza, v1, vr1, s1);
	creare_inamic1(50, DrawingWindow::height/11+50, raza, v1, vr1, s1);
	//rotire catre centrul ferestrei
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*inamic1[inamic1.size()-1]->center.x,(-1)*inamic1[inamic1.size()-1]->center.y);
	Transform2D::rotateMatrix(PI/2);
	Transform2D::translateMatrix(inamic1[inamic1.size()-1]->center.x,inamic1[inamic1.size()-1]->center.y);
	for(j=0; j < inamic1[inamic1.size()-1]->objects2D.size(); j++)
		Transform2D::applyTransform(inamic1[inamic1.size()-1]->objects2D[j]);
}

//functia de creare a corpului unui inamic de tip 2
void creare_inamic2(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	inamic2.push_back(new Ansamblu(c,_raza,_vit,_rot_vit,_scor));	//adaugare la wave

	Circle2D *enemy2a = new Circle2D(Point2D(c.x,c.y),3,Color(1,1,0),false);
	inamic2[inamic2.size()-1]->addObject2D_to_Visual2D(enemy2a,teren);

	Polygon2D *enemy2b = new Polygon2D(Color(1,1,0),false);
	enemy2b->addPoint(Point2D(c.x-3,c.y));
	enemy2b->addPoint(Point2D(c.x-23,c.y));
	enemy2b->addPoint(Point2D(c.x-23,c.y+20));
	inamic2[inamic2.size()-1]->addObject2D_to_Visual2D(enemy2b,teren);

	Polygon2D *enemy2c = new Polygon2D(Color(1,1,0),false);
	enemy2c->addPoint(Point2D(c.x+3,c.y));
	enemy2c->addPoint(Point2D(c.x+23,c.y));
	enemy2c->addPoint(Point2D(c.x+23,c.y-20));
	inamic2[inamic2.size()-1]->addObject2D_to_Visual2D(enemy2c,teren);
}

//functia de creare a unui wave de 4 inamici de tip 2 din cele 4 centre de laturi ale ferestrei
void creare_inamici2()
{
	float raza = 20*sqrt(2)+3;	//raza inamicului de tip 2
	creare_inamic2(DrawingWindow::width/2, DrawingWindow::height/11+60, raza, v2, vr2, s2);
	creare_inamic2(DrawingWindow::width-60, DrawingWindow::height*12/22, raza, v2, vr2, s2);
	creare_inamic2(DrawingWindow::width/2, DrawingWindow::height-60, raza, v2, vr2, s2);
	creare_inamic2(60, DrawingWindow::height*12/22, raza, v2, vr2, s2);
}

//functia de creare a corpului unui inamic de tip 3
void creare_inamic3(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	inamic3.push_back(new Ansamblu(c,_raza,_vit,_rot_vit,_scor));	//adaugare la wave

	Polygon2D *enemy3a = new Polygon2D(Color(0,1,1),true);
	enemy3a->addPoint(Point2D(c.x-2,c.y+15));
	enemy3a->addPoint(Point2D(c.x-2,c.y-2));
	enemy3a->addPoint(Point2D(c.x-15,c.y-9));
	inamic3[inamic3.size()-1]->addObject2D_to_Visual2D(enemy3a,teren);

	Polygon2D *enemy3b = new Polygon2D(Color(0,1,1),true);
	enemy3b->addPoint(Point2D(c.x+2,c.y+15));
	enemy3b->addPoint(Point2D(c.x+2,c.y-2));
	enemy3b->addPoint(Point2D(c.x+15,c.y-9));
	inamic3[inamic3.size()-1]->addObject2D_to_Visual2D(enemy3b,teren);
}

//functia de creare a unui wave de 4 inamici de tip 3 din cele 4 colturi ale ferestrei
void creare_inamici3()
{
	float raza = 10*sqrt(3);	//raza inamicului de tip 3
	int j;
	creare_inamic3(DrawingWindow::width-50, DrawingWindow::height/11+50, raza, v3, vr3, s3);
	//rotire catre centrul ferestrei
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*inamic3[inamic3.size()-1]->center.x,(-1)*inamic3[inamic3.size()-1]->center.y);
	Transform2D::rotateMatrix(PI/4);
	Transform2D::translateMatrix(inamic3[inamic3.size()-1]->center.x,inamic3[inamic3.size()-1]->center.y);
	for(j=0; j < inamic3[inamic3.size()-1]->objects2D.size(); j++)
		Transform2D::applyTransform(inamic3[inamic3.size()-1]->objects2D[j]);
	creare_inamic3(DrawingWindow::width-50, DrawingWindow::height-50, raza, v3, vr3, s3);
	//rotire catre centrul ferestrei
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*inamic3[inamic3.size()-1]->center.x,(-1)*inamic3[inamic3.size()-1]->center.y);
	Transform2D::rotateMatrix(PI*3/4);
	Transform2D::translateMatrix(inamic3[inamic3.size()-1]->center.x,inamic3[inamic3.size()-1]->center.y);
	for(j=0; j < inamic3[inamic3.size()-1]->objects2D.size(); j++)
		Transform2D::applyTransform(inamic3[inamic3.size()-1]->objects2D[j]);
	creare_inamic3(50, DrawingWindow::height-50, raza, v3, vr3, s3);
	//rotire catre centrul ferestrei
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*inamic3[inamic3.size()-1]->center.x,(-1)*inamic3[inamic3.size()-1]->center.y);
	Transform2D::rotateMatrix(-PI*3/4);
	Transform2D::translateMatrix(inamic3[inamic3.size()-1]->center.x,inamic3[inamic3.size()-1]->center.y);
	for(j=0; j < inamic3[inamic3.size()-1]->objects2D.size(); j++)
		Transform2D::applyTransform(inamic3[inamic3.size()-1]->objects2D[j]);
	creare_inamic3(50, DrawingWindow::height/11+50, raza, v3, vr3, s3);
	//rotire catre centrul ferestrei
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*inamic3[inamic3.size()-1]->center.x,(-1)*inamic3[inamic3.size()-1]->center.y);
	Transform2D::rotateMatrix(-PI/4);
	Transform2D::translateMatrix(inamic3[inamic3.size()-1]->center.x,inamic3[inamic3.size()-1]->center.y);
	for(j=0; j < inamic3[inamic3.size()-1]->objects2D.size(); j++)
		Transform2D::applyTransform(inamic3[inamic3.size()-1]->objects2D[j]);
}

//functia de creare a corpului unui inamic de tip 4
void creare_inamic4(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	inamic4.push_back(new Ansamblu(c,_raza,_vit,_rot_vit,_scor));	//adaugare la wave

	Rectangle2D *enemy4a = new Rectangle2D(Point2D(c.x-17,c.y-17),34,34,Color(0,1,0),false);
	inamic4[inamic4.size()-1]->addObject2D_to_Visual2D(enemy4a,teren);

	Rectangle2D *enemy4b = new Rectangle2D(Point2D(c.x-15,c.y-15),30,30,Color(0,1,0),false);
	inamic4[inamic4.size()-1]->addObject2D_to_Visual2D(enemy4b,teren);

	Polygon2D *enemy4c = new Polygon2D(Color(0,1,0),false);
	enemy4c->addPoint(Point2D(c.x,c.y-15));
	enemy4c->addPoint(Point2D(c.x+15,c.y));
	enemy4c->addPoint(Point2D(c.x,c.y+15));
	enemy4c->addPoint(Point2D(c.x-15,c.y));
	inamic4[inamic4.size()-1]->addObject2D_to_Visual2D(enemy4c,teren);

	Polygon2D *enemy4d = new Polygon2D(Color(0,1,0),false);
	enemy4d->addPoint(Point2D(c.x,c.y-13));
	enemy4d->addPoint(Point2D(c.x+13,c.y));
	enemy4d->addPoint(Point2D(c.x,c.y+13));
	enemy4d->addPoint(Point2D(c.x-13,c.y));
	inamic4[inamic4.size()-1]->addObject2D_to_Visual2D(enemy4d,teren);
}

//functia de creare a unui wave de 4 inamici de tip 4 din cele 4 centre de laturi ale ferestrei
void creare_inamici4()
{
	float raza = 17*sqrt(2);	//raza inamicului de tip 4
	creare_inamic4(DrawingWindow::width/2, DrawingWindow::height/11+50, raza, v4, vr4, s4);
	inamic4[inamic4.size()-1]->unghi = PI/4;
	creare_inamic4(DrawingWindow::width-50, DrawingWindow::height*12/22, raza, v4, vr4, s4);
	inamic4[inamic4.size()-1]->unghi = PI/4+PI/2;
	creare_inamic4(DrawingWindow::width/2, DrawingWindow::height-50, raza, v4, vr4, s4);
	inamic4[inamic4.size()-1]->unghi = PI/4+PI;
	creare_inamic4(50, DrawingWindow::height*12/22, raza, v4, vr4, s4);
	inamic4[inamic4.size()-1]->unghi = PI/4+PI*3/2;
}

//functia de creare a unui bonus de tip 1
void creare_bonus1(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	
	
	Polygon2D *poly = new Polygon2D(Color(1,0,0),true);
	poly->addPoint(Point2D(c.x,c.y-15));
	poly->addPoint(Point2D(c.x-10,c.y-5));
	poly->addPoint(Point2D(c.x-14,c.y+5));
	poly->addPoint(Point2D(c.x-10,c.y+10));
	poly->addPoint(Point2D(c.x-2.5,c.y+10));
	poly->addPoint(Point2D(c.x,c.y+5));
	poly->addPoint(Point2D(c.x+2.5,c.y+10));
	poly->addPoint(Point2D(c.x+10,c.y+10));
	poly->addPoint(Point2D(c.x+14,c.y+5));
	poly->addPoint(Point2D(c.x+10,c.y-5));
	bonus->addObject2D_to_Visual2D(poly,teren);
}

//functia de creare a unui bonus de tip 2
void creare_bonus2(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	
	
	Circle2D *cerc = new Circle2D(Point2D(c.x,c.y),10,Color(1,0.75,0),true);
	bonus->addObject2D_to_Visual2D(cerc,teren);
}

//functia de creare a unui bonus de tip 3
void creare_bonus3(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	
	
	Polygon2D *poly = new Polygon2D(Color(0,0.5,0),true);
	poly->addPoint(Point2D(c.x-12.5,c.y-7.5));
	poly->addPoint(Point2D(c.x-12.5,c.y+7.5));
	poly->addPoint(Point2D(c.x+12.5,c.y+7.5));
	poly->addPoint(Point2D(c.x+12.5,c.y-7.5));
	bonus->addObject2D_to_Visual2D(poly,teren);
}

//functia de creare a unui bonus de tip 4
void creare_bonus4(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	
	
	Polygon2D *poly = new Polygon2D(Color(0,0.5,1),true);
	poly->addPoint(Point2D(c.x-12.5,c.y-7.5));
	poly->addPoint(Point2D(c.x-12.5,c.y+7.5));
	poly->addPoint(Point2D(c.x+2.5,c.y+7.5));
	poly->addPoint(Point2D(c.x+15,c.y));
	poly->addPoint(Point2D(c.x+2.5,c.y-7.5));
	bonus->addObject2D_to_Visual2D(poly,teren);
}

//functia de creare a unui bonus de tip 5
void creare_bonus5(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	
	
	Polygon2D *poly = new Polygon2D(Color(1,0.5,0),true);
	poly->addPoint(Point2D(c.x-10,c.y-12.5));
	poly->addPoint(Point2D(c.x-10,c.y+12.5));
	poly->addPoint(Point2D(c.x+15,c.y));
	bonus->addObject2D_to_Visual2D(poly,teren);
}

//functia de creare a unui bonus de tip 6
void creare_bonus6(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	
	
	Polygon2D *poly = new Polygon2D(Color(1,0,0.5),true);
	poly->addPoint(Point2D(c.x-7.5,c.y-10));
	poly->addPoint(Point2D(c.x-7.5,c.y+10));
	poly->addPoint(Point2D(c.x+7.5,c.y+10));
	poly->addPoint(Point2D(c.x+7.5,c.y-10));
	bonus->addObject2D_to_Visual2D(poly,teren);
}

//functia de creare a unui bonus de tip 7
void creare_bonus7(float x, float y, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point2D c;
	c.x = x;
	c.y = y;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	
	
	Polygon2D *poly = new Polygon2D(Color(0,1,1),true);
	poly->addPoint(Point2D(c.x-10,c.y-5));
	poly->addPoint(Point2D(c.x-10,c.y+10));
	poly->addPoint(Point2D(c.x+10,c.y+10));
	poly->addPoint(Point2D(c.x+10,c.y-5));
	poly->addPoint(Point2D(c.x,c.y-15));
	bonus->addObject2D_to_Visual2D(poly,teren);
}

//functia de creare a unui bonus de un anumit tip la anumite coordonate
void creare_bonus(int b, int x, int y)
{
	switch(b) {
		case 1: {
			creare_bonus1(x, y, 15, 0, 0, 0);
		} break;
		case 2: {
			creare_bonus2(x, y, 10, 0, 0, 0);
		} break;
		case 3: {
			creare_bonus3(x, y, 15, 0, 0, 0);
		} break;
		case 4: {
			creare_bonus4(x, y, 15, 0, 0, 0);
		} break;
		case 5: {
			creare_bonus5(x, y, 15, 0, 0, 0);
		} break;
		case 6: {
			creare_bonus6(x, y, 12.5, 0, 0, 0);
		} break;
		case 7: {
			creare_bonus7(x, y, 15, 0, 0, 0);
		} break;
	}
}

//functia de stergere a unui bonus de pe teren
void sterge_bonus(int value)
{
	if(game_over == false) {
		bonus->removeAnsamblu_from_Visual2D(teren);
		bid = 0;	//nu am nici un bonus pe teren
		glutPostRedisplay();
		glutTimerFunc(b_int*1000, sterge_bonus, 0);	//se apeleaza o data la b_int sec
	}
}

//functia de creare a unui bonus random la o locatie random
void updateb(int value)
{
	if(game_over == false) {
		bid = 1+rand()%7;
		int x = 60+rand()%(int)(DrawingWindow::width-2*60+1);
		int y = DrawingWindow::height/11+100+rand()%(int)(DrawingWindow::height-2*100+1);

		creare_bonus(bid, x, y);

		glutPostRedisplay();
		glutTimerFunc(b_int*1000, updateb, 0);	//se apeleaza o data la b_int sec
	}
}

//functia de creare a unui wave de inamici de tip 1
void update1(int value)
{
	if(game_over == false) {
		creare_inamici1();
		glutPostRedisplay();
		glutTimerFunc(wave_int*4000, update1, 0);	//se apeleaza o data la wave_int sec
	}
}

//functia de creare a unui wave de inamici de tip 2
void update2(int value)
{
	if(game_over == false) {
		creare_inamici2();
		glutPostRedisplay();
		glutTimerFunc(wave_int*4000, update2, 0);	//se apeleaza o data la wave_int sec
	}
}

//functia de creare a unui wave de inamici de tip 3
void update3(int value)
{
	if(game_over == false) {
		creare_inamici3();
		glutPostRedisplay();
		glutTimerFunc(wave_int*4000, update3, 0);	//se apeleaza o data la wave_int sec
	}
}

//functia de creare a unui wave de inamici de tip 4
void update4(int value)
{
	if(game_over == false) {
		creare_inamici4();
		glutPostRedisplay();
		glutTimerFunc(wave_int*4000, update4, 0);	//se apeleaza o data la wave_int sec
	}
}

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	if(full == true)
		glutFullScreen();	//trecerea in fullscreen a ferestrei (RECOMAND folosirea fullscreen)

	//crearea contextului vizual titlu, a scorului si a vietii
	titlu = new Visual2D(0,0,DrawingWindow::width,DrawingWindow::height/11,
						 0,0,DrawingWindow::width,DrawingWindow::height/11);  
	titlu->cadruFereastra(Color(0,1,0));
	titlu->tipTran(true);
	addVisual2D(titlu);

	Text *titlu1 = new Text(" - GEOMETRY WARS - 	Copyright : Bojin Andrei", 
							Point2D(DrawingWindow::width/13, DrawingWindow::height/25), 
							Color(0,1,1), BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(titlu1, titlu);
	Text *scor_text = new Text("-< SCORE >-", Point2D(DrawingWindow::width/2.14, DrawingWindow::height/20), 
							Color(1,0.5,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(scor_text, titlu);
	scor = new Text("00000", Point2D(DrawingWindow::width/2.063, DrawingWindow::height/35), 
							Color(0,1,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(scor, titlu);
	Text *Level = new Text("Level", Point2D(DrawingWindow::width*1.95/3, DrawingWindow::height/26), 
							Color(1,0.5,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(Level, titlu);
	lvl = new Text(std::to_string(level), Point2D(DrawingWindow::width*2.035/3, DrawingWindow::height/26), 
							Color(0,1,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(lvl, titlu);

	//crearea obiectelor atasate numarului de vieti din dreapta sus
	Point2D c;
	c.x = DrawingWindow::width*5/6;
	c.y = DrawingWindow::height/22;

	Polygon2D *poly3 = new Polygon2D(Color(1,0.5,0),false);
	poly3->addPoint(Point2D(c.x+13,c.y-10));
	poly3->addPoint(Point2D(c.x,c.y-17));
	poly3->addPoint(Point2D(c.x-17,c.y));
	poly3->addPoint(Point2D(c.x,c.y+17));
	poly3->addPoint(Point2D(c.x+13,c.y+10));
	poly3->addPoint(Point2D(c.x,c.y+15));
	poly3->addPoint(Point2D(c.x-15,c.y));
	poly3->addPoint(Point2D(c.x,c.y-15));
	addObject2D_to_Visual2D(poly3,titlu);

	Polygon2D *poly4 = new Polygon2D(Color(1,0.5,0),false);
	poly4->addPoint(Point2D(c.x+13,c.y-10));
	poly4->addPoint(Point2D(c.x,c.y-12));
	poly4->addPoint(Point2D(c.x-8,c.y));
	poly4->addPoint(Point2D(c.x,c.y+12));
	poly4->addPoint(Point2D(c.x+13,c.y+10));
	poly4->addPoint(Point2D(c.x+1,c.y+10));
	poly4->addPoint(Point2D(c.x-6,c.y));
	poly4->addPoint(Point2D(c.x+1,c.y-10));
	addObject2D_to_Visual2D(poly4,titlu);

	Polygon2D *poly5 = new Polygon2D(Color(1,0.5,0),false);
	poly5->addPoint(Point2D(c.x-36,c.y));
	poly5->addPoint(Point2D(c.x-28,c.y+10));
	poly5->addPoint(Point2D(c.x-38,c.y+2));
	poly5->addPoint(Point2D(c.x-48,c.y+10));
	poly5->addPoint(Point2D(c.x-40,c.y));
	poly5->addPoint(Point2D(c.x-48,c.y-10));
	poly5->addPoint(Point2D(c.x-38,c.y-2));
	poly5->addPoint(Point2D(c.x-28,c.y-10));
	addObject2D_to_Visual2D(poly5,titlu);

	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*c.x,(-1)*c.y);
	Transform2D::scaleMatrix(0.5,0.5);
	Transform2D::translateMatrix(c.x,c.y);
	Transform2D::applyTransform(poly3);
	Transform2D::applyTransform(poly4);
	Transform2D::applyTransform(poly5);

	viata = new Text(std::to_string(life), Point2D(c.x-40, c.y-8), Color(0,1,0), BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(viata, titlu);

	//crearea contextului vizual teren
	teren = new Visual2D(0,DrawingWindow::height/11,DrawingWindow::width,DrawingWindow::height,
						 0,DrawingWindow::height/11,DrawingWindow::width,DrawingWindow::height);   
	teren->cadruFereastra(Color(1,0,0));
	teren->tipTran(true);
	addVisual2D(teren);

	//crearea corpului playerului si a corpului burghiului
	creare_player(DrawingWindow::width/2, DrawingWindow::height*12/22, 30, v, vr, 0);
	burghiu = false;	//burghiul este dezactivat
	shoot = false;		//tragerea cu gloante este dezactivata

	//crearea bonusurilor
	glutTimerFunc(b_int/2*1000, updateb, 0);
	//stergerea acestora cu un delay de b_del sec
	glutTimerFunc((b_int/2+b_del)*1000, sterge_bonus, 0);

	//crearea wave-urilor de inamici cu un delay de wave_int sec intre ele
	update1(0);
	glutTimerFunc(wave_int*1000, update2, 0);
	glutTimerFunc(wave_int*2000, update3, 0);
	glutTimerFunc(wave_int*3000, update4, 0);

}

//functia de testare a faptului ca burghiul este in interiorul ferestrei
bool burghiu_in(float pasx, float pasy, float unghi)
{
	if((pasx+70*cos(unghi)) >= (-1)*DrawingWindow::width/2 && 
	   (pasx+70*cos(unghi)) <= DrawingWindow::width/2 &&
	   (pasy+70*sin(unghi)) >= (-1)*DrawingWindow::height*10/22 && 
	   (pasy+70*sin(unghi)) <= DrawingWindow::height*10/22 &&
	   (pasx+35*cos(unghi)) >= (-1)*DrawingWindow::width/2 && 
	   (pasx+35*cos(unghi)) <= DrawingWindow::width/2 &&
	   (pasy+15*sin(unghi)) >= (-1)*DrawingWindow::height*10/22 && 
	   (pasy+15*sin(unghi)) <= DrawingWindow::height*10/22 &&
	   (pasx+35*cos(unghi)) >= (-1)*DrawingWindow::width/2 && 
	   (pasx+35*cos(unghi)) <= DrawingWindow::width/2 && 
	   (pasy-15*sin(unghi)) >= (-1)*DrawingWindow::height*10/22 && 
	   (pasy-15*sin(unghi)) <= DrawingWindow::height*10/22)

		return true;

	return false;
}

//functia de eliminare a unui Ansamblu dintr-un vector de Ansambluri
int removeAnsamblu(Ansamblu *ans, vector <Ansamblu*> inamic)
{
	//gaseste ansamblul care trebuie sters
	int poz;
	bool gasit = false;
	for (poz = 0; poz< inamic.size(); poz++)
	{
		if (ans == inamic[poz])
		{
			gasit = true;
			break;
		}
	}
	if (gasit)
		return poz;	//intoarce pozitia elementului de eliminat din vector
	return -1;
}

//functia de miscare a tuturor gloantelor din fereastra
void move_gloante()
{
	int i, j;
	for(i=0; i<gloante.size(); i++) {
		if(gloante[i]->interior(1,1) == false) { //sterge glontul daca a parasit fereastra
			gloante[i]->removeAnsamblu_from_Visual2D(teren);
			gloante.erase(gloante.begin() + removeAnsamblu(gloante[i], gloante));
		}
		else {
			Transform2D::loadIdentityMatrix();
			gloante[i]->pasx += gloante[i]->vit*cos(gloante[i]->unghi);
			gloante[i]->pasy += gloante[i]->vit*sin(gloante[i]->unghi);
			Transform2D::translateMatrix(gloante[i]->pasx,gloante[i]->pasy);
			for(j=0; j < gloante[i]->objects2D.size(); j++)
				Transform2D::applyTransform(gloante[i]->objects2D[j]);
		}
	}
}

//functia de miscare random a unui wave de inamici de un anumit tip (1 sau 3)
void move_inamici(vector <Ansamblu*> inamic)
{
	int i, r, a, b;
	for(i=0; i<inamic.size(); i++) {
		switch(i%4)	//alegerea directiei de deplasare a unui inamic din wave
		{
			case 0: {
				a = -1;
				b = 1;
			} break;
			case 1: {
				a = -1;
				b = -1;
			} break;
			case 2: {
				a = 1;
				b = -1;
			} break;
			case 3: {
				a = 1;
				b = 1;
			} break;
		}
		r = pow(-1,rand());
		if(inamic[i]->interior(a,b) == true) {
		    inamic[i]->pasx += a*inamic[i]->vit*cos(inamic[i]->unghi);
			inamic[i]->pasy += b*inamic[i]->vit*sin(inamic[i]->unghi);
			inamic[i]->deplasare();
			inamic[i]->unghi += inamic[i]->rot_vit*r;
		}
		else {	//daca s-a "ciocnit" de marginea ferestrei se misca in directia opusa
		    inamic[i]->pasx -= a*inamic[i]->vit*cos(inamic[i]->unghi);
			inamic[i]->pasy -= b*inamic[i]->vit*sin(inamic[i]->unghi);
			inamic[i]->deplasare();	
			inamic[i]->unghi += inamic[i]->rot_vit*r;	
		}
	}
}

//functia de miscare a unui wave de inamici de tip 2:
//presupune ca in timp ce se rotesc pe loc, inamicii sa vina catre player
void move_inamici2()
{
	int i, j;
	for(i=0; i<inamic2.size(); i++) {
		Transform2D::loadIdentityMatrix();

		if(inamic2[i]->center.x+inamic2[i]->pasx < player->center.x+player->pasx)
		    inamic2[i]->pasx += inamic2[i]->vit;
		if(inamic2[i]->center.x+inamic2[i]->pasx > player->center.x+player->pasx)
			inamic2[i]->pasx -= inamic2[i]->vit;
		if(inamic2[i]->center.y+inamic2[i]->pasy < player->center.y+player->pasy)
			inamic2[i]->pasy += inamic2[i]->vit;
		if(inamic2[i]->center.y+inamic2[i]->pasy > player->center.y+player->pasy)
			inamic2[i]->pasy -= inamic2[i]->vit;

		Transform2D::translateMatrix(inamic2[i]->pasx,inamic2[i]->pasy);
		inamic2[i]->unghi += inamic2[i]->rot_vit;
		Transform2D::translateMatrix((-1)*(inamic2[i]->center.x+inamic2[i]->pasx),
									 (-1)*(inamic2[i]->center.y+inamic2[i]->pasy));
		Transform2D::rotateMatrix(inamic2[i]->unghi);
		Transform2D::translateMatrix(inamic2[i]->center.x+inamic2[i]->pasx,
									 inamic2[i]->center.y+inamic2[i]->pasy);

		for(j=0; j<inamic2[i]->objects2D.size(); j++)
			Transform2D::applyTransform(inamic2[i]->objects2D[j]);
	}
}

//functia de miscare a unui wave de inamici de tip 4:
//presupune inamicii sa se reflecte de peretii ferestrei
void move_inamici4()
{
	int i, j;
	for(i=0; i<inamic4.size(); i++) {
		Transform2D::loadIdentityMatrix();
		if(inamic4[i]->interior(1,1) == false) {
			inamic4[i]->unghi += PI/2;
			if(inamic4[i]->interior(1,1) == false)
				inamic4[i]->unghi -= PI;
		}
		inamic4[i]->pasx += inamic4[i]->vit*cos(inamic4[i]->unghi);
		inamic4[i]->pasy += inamic4[i]->vit*sin(inamic4[i]->unghi);

		Transform2D::translateMatrix(inamic4[i]->pasx,inamic4[i]->pasy);
		for(j=0; j<inamic4[i]->objects2D.size(); j++)
			Transform2D::applyTransform(inamic4[i]->objects2D[j]);
	}
}

//functia de testare a coliziunii unui glont cu inamicii de un anumit tip
Ansamblu* coliziune_glont(Ansamblu* glont, vector <Ansamblu*> inamic)
{
	int i;
	float d, x2, x1, y2, y1;
	x2 = glont->center.x+glont->pasx;
	y2 = glont->center.y+glont->pasy;
	for(i=0; i<inamic.size(); i++) {
		x1 = inamic[i]->center.x+inamic[i]->pasx;
		y1 = inamic[i]->center.y+inamic[i]->pasy;
		d = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
		if(d <= glont->raza+inamic[i]->raza)
			return inamic[i];	//intoarce inamicul cu care s-a ciocnit
	}
	return NULL;
}

//functia de testare a coliziunii corpului playerului cu inamicii de un anumit tip
Ansamblu* coliziune_corp(vector <Ansamblu*> inamic)
{
	int i;
	float d, x2, x1, y2, y1;
	x2 = player->center.x+player->pasx;
	y2 = player->center.y+player->pasy;
	for(i=0; i<inamic.size(); i++) {
		x1 = inamic[i]->center.x+inamic[i]->pasx;
		y1 = inamic[i]->center.y+inamic[i]->pasy;
		d = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
		if(d <= player->raza+inamic[i]->raza)
			return inamic[i];	//intoarce inamicul cu care s-a ciocnit
	}
	return NULL;
}

//functia de testare a coliziunii burghiului cu inamicii de un anumit tip
Ansamblu* coliziune_burghiu(vector <Ansamblu*> inamic)
{
	int i;
	float d3, d2, d1, d, x3, x2, x1, xc, xi, y3, y2, y1, yc, yi;
	x3 = player->center.x+player->pasx+70*cos(player->unghi);
	y3 = player->center.y+player->pasy+70*sin(player->unghi);
	x2 = player->center.x+player->pasx+35*cos(player->unghi);
	y2 = player->center.y+player->pasy+15*sin(player->unghi);
	x1 = player->center.x+player->pasx+35*cos(player->unghi);
	y1 = player->center.y+player->pasy-15*sin(player->unghi);
	xc = (x1+x2+x3)/3;
	yc = (y1+y2+y3)/3;
	for(i=0; i<inamic.size(); i++) {
		xi = inamic[i]->center.x+inamic[i]->pasx;
		yi = inamic[i]->center.y+inamic[i]->pasy;
		d3 = sqrt((x3-xi)*(x3-xi)+(y3-yi)*(y3-yi));
		d2 = sqrt((x2-xi)*(x2-xi)+(y2-yi)*(y2-yi));
		d1 = sqrt((x1-xi)*(x1-xi)+(y1-yi)*(y1-yi));
		d = sqrt((xc-xi)*(xc-xi)+(yc-yi)*(yc-yi));
		if(d3 <= inamic[i]->raza || d2 <= inamic[i]->raza || 
		   d1 <= inamic[i]->raza || d <= 11.66+inamic[i]->raza)
			return inamic[i];	//intoarce inamicul cu care s-a ciocnit
	}
	return NULL;
}

//functia de testare a coliziunii corpului playerului cu un bonus
bool coliziune_bonus()
{
	float d, x2, x1, y2, y1;
	x2 = player->center.x+player->pasx;
	y2 = player->center.y+player->pasy;
	x1 = bonus->center.x;
	y1 = bonus->center.y;
	d = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	if(d <= player->raza+bonus->raza)
		return true;
	return false;
}

//functia de incrementare a vitezei tuturor inamicilor de un anumit tip
void vit_inc(vector <Ansamblu*> inamic)
{
	int i;
	for(i=0; i<inamic.size(); i++) {
		inamic[i]->vit += v_inc;	//incrementeaza cu v_inc
	}
}

//functia de adaugare punctaj(value) la scorul curent si testarea consecintelor
void add_score(int value)
{
	score += value;
	if(score <= 99999) {
		//se reafiseaza scorul
		DrawingWindow::removeText_from_Visual2D(scor, titlu);
		int i;
		string s="";
		for(i=0; i < 5-std::to_string(score).size(); i++)
			s += "0";
		scor = new Text(s+std::to_string(score), Point2D(DrawingWindow::width/2.063, 
						DrawingWindow::height/35), Color(0,1,0), BITMAP_HELVETICA_18);
		DrawingWindow::addText_to_Visual2D(scor, titlu);
		//se testeaza trecerea la urmatorul nivel, din score_int in score_int puncte
		if(score/score_int == level) {
			level ++;
			//se reafiseaza levelul
			DrawingWindow::removeText_from_Visual2D(lvl, titlu);
			lvl = new Text(std::to_string(level), Point2D(DrawingWindow::width*2.035/3, 
							DrawingWindow::height/26), Color(0,1,0), BITMAP_HELVETICA_18);
			DrawingWindow::addText_to_Visual2D(lvl, titlu);
			//se afiseaza un mesaj pe ecran cu levelul la care s-a ajuns
			lvlup = new Text("LEVEL "+std::to_string(level), Point2D(DrawingWindow::width/2.075, 
							 DrawingWindow::height*13/22), Color(0,1,0), BITMAP_HELVETICA_18);
			DrawingWindow::addText_to_Visual2D(lvlup, teren);

			text_lvlup = 0;
			//scade intervalul de timp dintre 2 wave-uri
			wave_int --;
			if(wave_int == 0)	//daca se ajunge la 0, se reseteaza
				wave_int = copy_wave_int;
			//se incrementeaza vitezele inamicilor prezenti pe teren
			vit_inc(inamic1);
			vit_inc(inamic2);
			vit_inc(inamic3);
			vit_inc(inamic4);
			//se incrementeaza vitezele de creare a inamicilor
			v1 += v_inc;
			v2 += v_inc;
			v3 += v_inc;
			v4 += v_inc;
		}
	}
	else {	//afisarea unor mesaje in caz ca s-a realizat imposibilul: ajungerea la punctajul limita (99999) ! :D
		Text *end1 = new Text("CONGRATULATIONS!", Point2D(DrawingWindow::width/2.225, 
							  DrawingWindow::height*12/22), Color(1,0,0), BITMAP_HELVETICA_18);
		DrawingWindow::addText_to_Visual2D(end1, teren);
		Text *end2 = new Text("You have beaten", Point2D(DrawingWindow::width/2.167, 
							  DrawingWindow::height*11/22), Color(1,0.5,0), BITMAP_HELVETICA_18);
		DrawingWindow::addText_to_Visual2D(end2, teren);
		Text *end3 = new Text("- GEOMETRY WARS -", Point2D(DrawingWindow::width/2.23, 
							  DrawingWindow::height*10.5/22), Color(0,1,1), BITMAP_HELVETICA_18);
		DrawingWindow::addText_to_Visual2D(end3, teren);

		game_over = true;	//s-a terminat jocul
	}
}

//functia care realizeaza procesarea coliziunilor dintre obiectele de pe teren si 
//a consecintelor acestora
void coliziune()
{
	//procesarea coliziunilor gloantelor cu inamicii
	int i;
	Ansamblu *a1, *a2, *a3, *a4;
	for(i=0; i<gloante.size(); i++) {
		a1 = coliziune_glont(gloante[i], inamic1);
		a2 = coliziune_glont(gloante[i], inamic2);
		a3 = coliziune_glont(gloante[i], inamic3);
		a4 = coliziune_glont(gloante[i], inamic4);
		if(a1 != NULL || a2 != NULL || a3 != NULL || a4 != NULL) {
			if(a1 != NULL) {
				//sterge atat glontu cat si inamicul care s-au ciocnit
				gloante[i]->removeAnsamblu_from_Visual2D(teren);
				gloante.erase(gloante.begin() + removeAnsamblu(gloante[i], gloante));
				a1->removeAnsamblu_from_Visual2D(teren);
				inamic1.erase(inamic1.begin() + removeAnsamblu(a1, inamic1));
				add_score(a1->scor/2);	//creste scorul cu jumate din punctaj
			}
			if(a2 != NULL) {
				//sterge atat glontu cat si inamicul care s-au ciocnit
				gloante[i]->removeAnsamblu_from_Visual2D(teren);
				gloante.erase(gloante.begin() + removeAnsamblu(gloante[i], gloante));
				a2->removeAnsamblu_from_Visual2D(teren);
				inamic2.erase(inamic2.begin() + removeAnsamblu(a2, inamic2));
				add_score(a2->scor/2);	//creste scorul cu jumate din punctaj
			}
			if(a3 != NULL) {
				//sterge atat glontu cat si inamicul care s-au ciocnit
				gloante[i]->removeAnsamblu_from_Visual2D(teren);
				gloante.erase(gloante.begin() + removeAnsamblu(gloante[i], gloante));
				a3->removeAnsamblu_from_Visual2D(teren);
				inamic3.erase(inamic3.begin() + removeAnsamblu(a3, inamic3));
				add_score(a3->scor/2);	//creste scorul cu jumate din punctaj
			}
			if(a4 != NULL) {
				//sterge atat glontu cat si inamicul care s-au ciocnit
				gloante[i]->removeAnsamblu_from_Visual2D(teren);
				gloante.erase(gloante.begin() + removeAnsamblu(gloante[i], gloante));
				a4->removeAnsamblu_from_Visual2D(teren);
				inamic4.erase(inamic4.begin() + removeAnsamblu(a4, inamic4));
				add_score(a4->scor/2);	//creste scorul cu jumate din punctaj
			}
		}
	}

	//procesarea coliziunilor corpului playerului cu inamicii
	//in cazul coliziunii consecutive cu acelasi inamic, acest lucru se testeaza o data la 1 sec
	//pentru a se putea actualiza corect numarul de vietii
	if(invinc == false) {	//daca playerul nu a luat un bonus de invincibilitate
		a1 = coliziune_corp(inamic1);
		a2 = coliziune_corp(inamic2);
		a3 = coliziune_corp(inamic3);
		a4 = coliziune_corp(inamic4);
		if(a1 != NULL || a2 != NULL || a3 != NULL || a4 != NULL) {
			if(aux == NULL || (aux != a1 && aux != a2 && aux != a3 && aux != a4) || (col_int > 50)) {
				life--;	//viata scade
				//reafisarea vietii
				DrawingWindow::removeText_from_Visual2D(viata, titlu);
				Point2D c;
				c.x = DrawingWindow::width*5/6;
				c.y = DrawingWindow::height/22;
				viata = new Text(std::to_string(life), Point2D(c.x-40, c.y-8), 
								 Color(0,1,0), BITMAP_TIMES_ROMAN_24);
				DrawingWindow::addText_to_Visual2D(viata, titlu);
				//afisarea unui mesaj corespunzator pentru a anunta playerul ca s-a ciocnit
				minushp = new Text("-1 life", Point2D(player->center.x+player->pasx, 
								   player->center.y+player->pasy), Color(1,0,0), BITMAP_HELVETICA_18);
				DrawingWindow::addText_to_Visual2D(minushp, teren);

				text_minushp = 0;

				if(life == 0) {	 //daca s-a ajuns la 0 vieti
					//se afiseaza un mesaj corespunzator impreuna cu scorul final
					Text *end = new Text("GAME OVER", Point2D(DrawingWindow::width/2.14, 
										 DrawingWindow::height*12/22), Color(1,0,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(end, teren);
					Text *final_scor = new Text("FINAL SCORE:", Point2D(DrawingWindow::width/2.25, 
												DrawingWindow::height*11/22), Color(1,0.5,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(final_scor, teren);
					int i;
					string s="";
					for(i=0; i < 5-std::to_string(score).size(); i++)
						s += "0";
					Text *sc = new Text(s+std::to_string(score), Point2D(DrawingWindow::width/1.92, 
										DrawingWindow::height*11/22), Color(0,1,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(sc, teren);

					game_over = true;	//s-a terminat jocul
				}
				if(a1 != NULL)	aux = a1;
				if(a2 != NULL)	aux = a2;
				if(a3 != NULL)	aux = a3;
				if(a4 != NULL)	aux = a4;
			}
			col_int = 0;
		}
	}

	col_int ++;
	//se sterge mesajul pierderii unei vieti
	if(minushp != NULL && text_minushp >= text_lim)
		DrawingWindow::removeText_from_Visual2D(minushp, teren);
	text_minushp ++;

	//procesarea coliziunilor burghiului cu inamicii
	if(burghiu == true) {
		a1 = coliziune_burghiu(inamic1);
		a2 = coliziune_burghiu(inamic2);
		a3 = coliziune_burghiu(inamic3);
		a4 = coliziune_burghiu(inamic4);
		if(a1 != NULL || a2 != NULL || a3 != NULL || a4 != NULL) {
			if(a1 != NULL) {
				//sterge inamicul ciocnit
				a1->removeAnsamblu_from_Visual2D(teren);
				inamic1.erase(inamic1.begin() + removeAnsamblu(a1, inamic1));
				add_score(a1->scor);	//creste scorul cu punctajul integral
			}
			if(a2 != NULL) {
				//sterge inamicul ciocnit
				a2->removeAnsamblu_from_Visual2D(teren);
				inamic2.erase(inamic2.begin() + removeAnsamblu(a2, inamic2));
				add_score(a2->scor);	//creste scorul cu punctajul integral
			}
			if(a3 != NULL) {
				//sterge inamicul ciocnit
				a3->removeAnsamblu_from_Visual2D(teren);
				inamic3.erase(inamic3.begin() + removeAnsamblu(a3, inamic3));
				add_score(a3->scor);	//creste scorul cu punctajul integral
			}
			if(a4 != NULL) {
				//sterge inamicul ciocnit
				a4->removeAnsamblu_from_Visual2D(teren);
				inamic4.erase(inamic4.begin() + removeAnsamblu(a4, inamic4));
				add_score(a4->scor);	//creste scorul cu punctajul integral
			}
		}
	}

	//procesarea coliziunilor corpului playerului cu un anumit bonus
	if(bid != 0)	//daca este prezent un bonus pe teren
		if(coliziune_bonus() == true) {
			switch(bid) {
				//bonus de tip 1: cresterea numarului de vieti cu 1
				case 1: {
					life++;
					//reafisarea numarului de vieti
					DrawingWindow::removeText_from_Visual2D(viata, titlu);
					Point2D c;
					c.x = DrawingWindow::width*5/6;
					c.y = DrawingWindow::height/22;
					if(life >= 10) {
						viata = new Text(std::to_string(life), Point2D(c.x-55, c.y-8), 
										 Color(0,1,0), BITMAP_TIMES_ROMAN_24);
						DrawingWindow::addText_to_Visual2D(viata, titlu);
					}
					else {
						viata = new Text(std::to_string(life), Point2D(c.x-40, c.y-8), 
										 Color(0,1,0), BITMAP_TIMES_ROMAN_24);
						DrawingWindow::addText_to_Visual2D(viata, titlu);
					}
					//afisarea unui mesaj corespunzator
					plushp = new Text("+1 life", Point2D(player->center.x+player->pasx, 
									  player->center.y+player->pasy), Color(0,1,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(plushp, teren);

					text_plushp = 0;
				} break;
				//bonus de tip 2: cresterea scorului cu valoarea b_s2
				case 2: {
					add_score(b_s2);
					//afisarea unui mesaj corespunzator
					pluspct1 = new Text("+ "+std::to_string(b_s2)+" pct", Point2D(player->center.x+player->pasx, 
										player->center.y+player->pasy), Color(1,0.75,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(pluspct1, teren);

					text_pluspct1 = 0;
				} break;
				//bonus de tip 3: cresterea scorului cu valoarea b_s3
				case 3: {
					add_score(b_s3);
					//afisarea unui mesaj corespunzator
					pluspct2 = new Text("+ "+std::to_string(b_s3)+" pct", Point2D(player->center.x+player->pasx, 
										player->center.y+player->pasy), Color(0,0.5,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(pluspct2, teren);

					text_pluspct2 = 0;
				} break;
				//bonus de tip 4: cresterea vitezei playerului pentru b_lim sec
				case 4: {
					player->vit += b_v4;	//creste viteza
					pen = 0;				//penalizare 0 la activarea burghiului
					acc = b_acc4;			//acceleratia mai mare
					//afisarea unui mesaj corespunzator
					plusvit = new Text("+ speed ("+std::to_string(b_lim/50)+"sec)", Point2D(player->center.x+player->pasx, 
										player->center.y+player->pasy), Color(0,0.5,1), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(plusvit, teren);

					text_plusvit = 0;
					b_time = 0;
				} break;
				//bonus de tip 5: cresterea ratei de tragere cu gloante a playerului pentru b_lim sec
				case 5: {
					fire_rate /= 4;
					r_gl *= 2;
					v_gl *= 2;
					//afisarea unui mesaj corespunzator
					plusfr = new Text("+ fire power ("+std::to_string(b_lim/50)+"sec)", Point2D(player->center.x+player->pasx, 
										player->center.y+player->pasy), Color(1,0.5,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(plusfr, teren);

					text_plusfr = 0;
					b_time = 0;
				} break;
				//bonus de tip 6: oprirea animatiei inamicilor pentru b_lim sec
				case 6: {
					time_stop = true;
					//afisarea unui mesaj corespunzator
					timestop = new Text("time stop ("+std::to_string(b_lim/50)+"sec)", Point2D(player->center.x+player->pasx, 
										player->center.y+player->pasy), Color(1,0,0.5), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(timestop, teren);

					text_timestop = 0;
					b_time = 0;
				} break;
				//bonus de tip 7: invincibilitatea pentru player pentru b_lim sec
				case 7: {
					invinc = true;
					//afisarea unui mesaj corespunzator
					invul = new Text("invincible ("+std::to_string(b_lim/50)+"sec)", Point2D(player->center.x+player->pasx, 
										player->center.y+player->pasy), Color(0,1,1), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(invul, teren);

					text_invul = 0;
					b_time = 0;
				} break;
			}
			//stergerea bonusului de pe teren
			bonus->removeAnsamblu_from_Visual2D(teren);
			bid = 0;
		}
	//revenirea la parametrii normali dupa expirarea efectului bonusului
	if(b_time >= b_lim) {
		player->vit = v;
		pen = copy_pen;
		acc = copy_acc;
		fire_rate = copy_fire_rate;
		r_gl = copy_r_gl;
		v_gl = copy_v_gl;
		time_stop = false;
		invinc = false;
	}

	b_time ++;
	//se sterge mesajul cresterii cu un level
	if(lvlup != NULL && text_lvlup >= text_lim)
		DrawingWindow::removeText_from_Visual2D(lvlup, teren);
	text_lvlup ++;
	//se sterge mesajul corespunzator bonusului de tip 1
	if(plushp != NULL && text_plushp >= text_lim)
		DrawingWindow::removeText_from_Visual2D(plushp, teren);
	text_plushp ++;
	//se sterge mesajul corespunzator bonusului de tip 2
	if(pluspct1 != NULL && text_pluspct1 >= text_lim)
		DrawingWindow::removeText_from_Visual2D(pluspct1, teren);
	text_pluspct1 ++;
	//se sterge mesajul corespunzator bonusului de tip 3
	if(pluspct2 != NULL && text_pluspct2 >= text_lim)
		DrawingWindow::removeText_from_Visual2D(pluspct2, teren);
	text_pluspct2 ++;
	//se sterge mesajul corespunzator bonusului de tip 4
	if(plusvit != NULL && text_plusvit >= text_lim)
		DrawingWindow::removeText_from_Visual2D(plusvit, teren);
	text_plusvit ++;
	//se sterge mesajul corespunzator bonusului de tip 5
	if(plusfr != NULL && text_plusfr >= text_lim)
		DrawingWindow::removeText_from_Visual2D(plusfr, teren);
	text_plusfr ++;
	//se sterge mesajul corespunzator bonusului de tip 6
	if(timestop != NULL && text_timestop >= text_lim)
		DrawingWindow::removeText_from_Visual2D(timestop, teren);
	text_timestop ++;
	//se sterge mesajul corespunzator bonusului de tip 7
	if(invul != NULL && text_invul >= text_lim)
		DrawingWindow::removeText_from_Visual2D(invul, teren);
	text_invul ++;

}

//functia care permite animatia
void DrawingWindow::onIdle()
{
	if(game_over == false)
	{
		coliziune();	//se proceseaza coliziunile
		//procesarea apasarii tastei UP
		if(keySpecialStates[KEY_UP] == true) {
			if(player->interior(1,1) == true) {
				if(burghiu == false) {
					if(vit_temp <= player->vit)
						vit_temp += acc;
					player->pasx += vit_temp*cos(player->unghi);
					player->pasy += vit_temp*sin(player->unghi);
					player->deplasare();
					Transform2D::applyTransform(drill);
				} else {
					if(vit_temp <= player->vit-pen)
						vit_temp += acc;
					else
						vit_temp -= acc;
					if(burghiu_in(player->pasx+vit_temp*cos(player->unghi), 
									player->pasy+vit_temp*sin(player->unghi), 
									player->unghi) == true) {
						player->pasx += vit_temp*cos(player->unghi);
						player->pasy += vit_temp*sin(player->unghi);
						player->deplasare();
						Transform2D::applyTransform(drill);
					}
				}
			}
		}
		else { //procesarea eliberarii tastei RIGHT (pentru deceleratie)
			if(vit_temp != 0 && player->interior(1,1) == true) {
				if(vit_temp >= 0)
					vit_temp -= acc;
				if(vit_temp >= 0) {
					if(burghiu == false) {
						player->pasx += vit_temp*cos(player->unghi);
						player->pasy += vit_temp*sin(player->unghi);
						player->deplasare();
						Transform2D::applyTransform(drill);
					} else {
						if(burghiu_in(player->pasx+vit_temp*cos(player->unghi), 
										player->pasy+vit_temp*sin(player->unghi), 
										player->unghi) == true) {
							player->pasx += vit_temp*cos(player->unghi);
							player->pasy += vit_temp*sin(player->unghi);
							player->deplasare();
							Transform2D::applyTransform(drill);
						}
					}
				}
			}
		}
		//procesarea apasarii tastei LEFT
		if(keySpecialStates[KEY_LEFT] == true) {
			if(burghiu == false) {
				player->unghi += player->rot_vit;
				player->deplasare();
				Transform2D::applyTransform(drill);
			} else
				if(burghiu_in(player->pasx, player->pasy, 
								player->unghi+player->rot_vit) == true) {
					player->unghi += player->rot_vit;
					player->deplasare();
					Transform2D::applyTransform(drill);
				}
		}
		//procesarea apasarii tastei RIGHT
		if(keySpecialStates[KEY_RIGHT] == true) {
			if(burghiu == false) {
				player->unghi -= player->rot_vit;
				player->deplasare();
				Transform2D::applyTransform(drill);
			} else
				if(burghiu_in(player->pasx, player->pasy, 
								player->unghi-player->rot_vit) == true) {
					player->unghi -= player->rot_vit;
					player->deplasare();
					Transform2D::applyTransform(drill);
				}
		}
		//se creaza cate un glont
		if(burghiu == true && shoot == true && glont_int >= fire_rate) {
			creare_glont(0,0,r_gl,v_gl,0,0);
			gloante[gloante.size()-1]->pasx = player->pasx;
			gloante[gloante.size()-1]->pasy = player->pasy;
			gloante[gloante.size()-1]->unghi = player->unghi;
			glont_int = 0;
		}

		glont_int ++;
		//se realizeaza miscarea gloantelor
		move_gloante();
		//se realizeaza miscarea inamicilor
		if(time_stop == false) {
			move_inamici(inamic1);
			move_inamici2();
			move_inamici(inamic3);
			move_inamici4();
		}
	}
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{
	titlu->poarta(0,0,width,height/11);
	teren->poarta(0,height/11,width,height);
}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
	switch(key)
	{
		case KEY_ESC : exit(0); break;
		case KEY_SPACE: {		//se activeaza/dezactiveaza burghiul la apasarea SPACE
			if(game_over == false && keyStates[KEY_SPACE] == true) {
				if(burghiu == false && burghiu_in(player->pasx, player->pasy, 
												  player->unghi) == true) {
					player->addObject2D_to_Visual2D(drill,teren);
					burghiu = true;
				} else {
					player->removeObject2D_from_Visual2D(drill,teren);
					burghiu = false;
				}
			}
		} break;
		case 'b': {	//se activeaza/dezactiveaza tragerea cu gloante la apasarea tastei 'b'
			if(game_over == false && keyStates['b'] == true)
				if(shoot == false)
					shoot = true;
				else
					shoot = false;
		} break;
	}
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{

}

//functia main
int main(int argc, char** argv)
{
	int w, h;
	char rasp;
	cout << "  ----- Mentionez ca jocul a fost testat pe rezolutia de 1920x1080. ----- " << endl;
	cout << " -- Rularea pe o rezolutie mai mica de atat va duce la efecte nedorite, --" << endl;
	cout << "-- gen text decalat. Acest lucru se datoreaza nescalabilitatii fontului. --" << endl << endl;
	cout << "	Introduceti rezolutia ecranului dumneavoastra:" << endl << endl;
	cout << "	width (1920 recomandat): "; cin >> w;
	cout << "	height (1080 recomandat): "; cin >> h; cout << endl;
	cout << "	Doriti FullScreen? y/n" << endl << endl;
	cout << "	"; cin >> rasp;
	if(rasp == 'y')
		full = true;
	else
		full = false;
	//creare fereastra
	DrawingWindow dw(argc, argv, w, h, 200, 100, "Laborator EGC");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, 
	//animatia si procesarea evenimentelor
	dw.run();
	return 0;
}