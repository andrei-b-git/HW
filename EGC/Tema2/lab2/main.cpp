/*
 * TEMA 2: CRAZY DRIVER
 *
 * nume:		Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 333 CA
 */
#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Transform3D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <math.h>

#define PI 3.14159265358979323846

using namespace std;


// Configurarea parametrilor jocului:

//Contexte vizuale, scor, timp, distanta, viata:
Visual2D *titlu, *teren;	//cele doua contexte vizuale
Text *scor, *viata, *timp;
long score = 0;				//scorul curent
int life = 9;				//numarul curent de vieti
int startime = 0;			//timpul de start
Circle2D *minicar;			//simbol al masinii pe scala distantei
float distanta = 0;			//contor al distantei parcurse

Text *minushp = NULL;		//mesaj la scaderea vietii
int text_minushp = 0;		//contor de timp pana la disparitia textului

//Drum si benzi:
Ansamblu *drum;				//drumul
Ansamblu *benzi1, *benzi2;	//benzile intermitente
int ind = 1;

//Player:
Ansamblu *player;			//corpul masinii jucatorului
float v = 15;				//viteza
float vr = 0.1;				//viteza de rotatie
float vit_temp = 0;			//viteza de moment
float acc = 0.1;			//acceleratia/deceleratia
float pas = 0;				//pasul de rotatie al rotilor

//Inamici (3 tipuri):
//corpurile inamicilor de un anumit tip (sunt 3 tipuri in total)
vector <Ansamblu*>	 inamic1,  inamic2,  inamic3;
//viteza tuturor inamicilor de tipul respectiv
float				v1  =  0, v2  =	 0, v3  =  5;
//viteza de rotire a tuturor inamicilor de tipul respectiv
float				vr1 =  0, vr2 =  0, vr3 = 15;
//punctajul tuturor inamicilor de tipul respectiv
int					s1  = 10, s2  = 20, s3  = 30;
int col_int = 0;			//contor de timp intre coliziuni
int contor = 0;				//intervalul dintre 2 inamici consecutivi

//Bonusuri (3 tipuri):
Ansamblu *bonus;
int bid = 0;				//bonus id
int b_s = 1000;				//punctajul bonus de tip 2
bool invinc = false;		//indica daca playerul este invincibil
int b_time = 0;				//contor de timp pana la disparitia efectului bonusului
int b_lim = 200;			//numarul de sec pana la disparitia efectului bonusului
int bcontor = 0;			//intervalul dintre 2 bonusuri consecutive

Text *plushp = NULL;		//mesaj pentru bonusul de tip 1
int text_plushp = 0;		//contor de timp pana la disparitia textului
Text *pluspct = NULL;		//mesaj pentru bonusul de tip 2
int text_pluspct = 0;		//contor de timp pana la disparitia textului
Text *invul = NULL;			//mesaj pentru bonusul de tip 3
int text_invul = 0;			//contor de timp pana la disparitia textului

int text_lim = 20;			//timpul dupa care dispare textul

bool full = false;			//fullscreen sau nu
bool game_over = false;		//indica faptul ca s-a terminat jocul

Ansamblu *aux = NULL;		//auxiliar


//functia de creare a drumului ce contine strada si mediul inconjurator
void creare_drum(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;
	drum = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	

	//banda galbena din stanga, din mijlocul strazii
	vertices.push_back(new Point3D(c.x-15, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-5, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-5, c.y, c.z+DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-15, c.y, c.z+DrawingWindow::width));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *banda1 = new Object3D(vertices,faces,Color(0.6,0.6,0),false);
	drum->addObject3D_to_Visual2D(banda1,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//banda galbena din dreapta, din mijlocul strazii
	vertices.push_back(new Point3D(c.x+5, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+15, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+15, c.y, c.z+DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+5, c.y, c.z+DrawingWindow::width));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *banda2 = new Object3D(vertices,faces,Color(0.6,0.6,0),false);
	drum->addObject3D_to_Visual2D(banda2,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//banda alba din stanga, de pe marginea drumului
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/4+15, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/4+25, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/4+25, c.y, c.z+DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/4+15, c.y, c.z+DrawingWindow::width));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *banda3 = new Object3D(vertices,faces,Color(0.6,0.6,0.6),false);
	drum->addObject3D_to_Visual2D(banda3,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//banda alba din dreapta, de pe marginea drumului
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/4-15, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/4-25, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/4-25, c.y, c.z+DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/4-15, c.y, c.z+DrawingWindow::width));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *banda4 = new Object3D(vertices,faces,Color(0.6,0.6,0.6),false);
	drum->addObject3D_to_Visual2D(banda4,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//strada
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/4, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/4, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/4, c.y, c.z+DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/4, c.y, c.z+DrawingWindow::width));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *strada = new Object3D(vertices,faces,Color(0.329412, 0.329412, 0.329412),false);
	drum->addObject3D_to_Visual2D(strada,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//marginea strazii
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/3.5, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/3.5, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/3.5, c.y, c.z+DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/3.5, c.y, c.z+DrawingWindow::width));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *margine = new Object3D(vertices,faces,Color(0.309804, 0.309804, 0.184314),false);
	drum->addObject3D_to_Visual2D(margine,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//mediul inconjurator
	vertices.push_back(new Point3D(c.x-DrawingWindow::width*3.5, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width*3.5, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width*3.5, c.y, c.z+DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width*3.5, c.y, c.z+DrawingWindow::width));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *mediu = new Object3D(vertices,faces,Color(0.184314, 0.309804, 0.184314),false);
	drum->addObject3D_to_Visual2D(mediu,teren);

	//aplic transformarea de proiectie la aparitie
	drum->deplasare();
}

//functia de creare a unei benzi intermitente pe partea stanga a strazii
void creare_benzi1(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;

	vertices.push_back(new Point3D(c.x-DrawingWindow::width/8, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/8+10, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/8+10, c.y, c.z-DrawingWindow::width+70));
	vertices.push_back(new Point3D(c.x-DrawingWindow::width/8, c.y, c.z-DrawingWindow::width+70));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *banda1 = new Object3D(vertices,faces,Color(0.6,0.6,0.6),false);
	benzi1->addObject3D_to_Visual2D(banda1,teren);
}

//functia de creare a unei benzi intermitente pe partea dreapta a strazii
void creare_benzi2(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;

	vertices.push_back(new Point3D(c.x+DrawingWindow::width/8-10, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/8, c.y, c.z-DrawingWindow::width));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/8, c.y, c.z-DrawingWindow::width+70));
	vertices.push_back(new Point3D(c.x+DrawingWindow::width/8-10, c.y, c.z-DrawingWindow::width+70));
	
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *banda2 = new Object3D(vertices,faces,Color(0.6,0.6,0.6),false);
	benzi2->addObject3D_to_Visual2D(banda2,teren);
}

//functia de creare corpului masinii playerului, impreuna cu rotile acesteia
void creare_player(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;
	player = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	

	//spoiler-ul masinii
	vertices.push_back(new Point3D(c.x-70, c.y+65, c.z+115));
	vertices.push_back(new Point3D(c.x+70, c.y+65, c.z+115));
	vertices.push_back(new Point3D(c.x+70, c.y+65, c.z+105));
	vertices.push_back(new Point3D(c.x-70, c.y+65, c.z+105));

	vertices.push_back(new Point3D(c.x-70, c.y+70, c.z+115));
	vertices.push_back(new Point3D(c.x+70, c.y+70, c.z+115));
	vertices.push_back(new Point3D(c.x+70, c.y+70, c.z+105));
	vertices.push_back(new Point3D(c.x-70, c.y+70, c.z+105));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	Object3D *spoiler = new Object3D(vertices,faces,Color(1,0,0),false);
	player->addObject3D_to_Visual2D(spoiler,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//suportul spoiler-ului masinii
	vertices.push_back(new Point3D(c.x-15, c.y+50, c.z+100));
	vertices.push_back(new Point3D(c.x+15, c.y+50, c.z+100));
	vertices.push_back(new Point3D(c.x+15, c.y+50, c.z+80));
	vertices.push_back(new Point3D(c.x-15, c.y+50, c.z+80));

	vertices.push_back(new Point3D(c.x-15, c.y+65, c.z+110));
	vertices.push_back(new Point3D(c.x+15, c.y+65, c.z+110));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	Object3D *suport = new Object3D(vertices,faces,Color(0,0,1),false);
	player->addObject3D_to_Visual2D(suport,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//acoperisul masinii
	vertices.push_back(new Point3D(c.x-30, c.y+66, c.z-30));
	vertices.push_back(new Point3D(c.x+30, c.y+66, c.z-30));
	vertices.push_back(new Point3D(c.x+30, c.y+66, c.z+50));
	vertices.push_back(new Point3D(c.x-30, c.y+66, c.z+50));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	Object3D *acoperis = new Object3D(vertices,faces,Color(1,0,0),false);
	player->addObject3D_to_Visual2D(acoperis,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//geamurile masinii
	vertices.push_back(new Point3D(c.x-50, c.y+50, c.z-50));
	vertices.push_back(new Point3D(c.x+50, c.y+50, c.z-50));
	vertices.push_back(new Point3D(c.x+50, c.y+50, c.z+70));
	vertices.push_back(new Point3D(c.x-50, c.y+50, c.z+70));

	vertices.push_back(new Point3D(c.x-30, c.y+65, c.z-30));
	vertices.push_back(new Point3D(c.x+30, c.y+65, c.z-30));
	vertices.push_back(new Point3D(c.x+30, c.y+65, c.z+50));
	vertices.push_back(new Point3D(c.x-30, c.y+65, c.z+50));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	Object3D *geamuri = new Object3D(vertices,faces,Color(0,0,1),false);
	player->addObject3D_to_Visual2D(geamuri,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//caroseria masinii
	vertices.push_back(new Point3D(c.x-70, c.y+25, c.z-120));
	vertices.push_back(new Point3D(c.x+70, c.y+25, c.z-120));
	vertices.push_back(new Point3D(c.x+70, c.y+25, c.z+120));
	vertices.push_back(new Point3D(c.x-70, c.y+25, c.z+120));

	vertices.push_back(new Point3D(c.x-50, c.y+50, c.z-70));
	vertices.push_back(new Point3D(c.x+50, c.y+50, c.z-70));
	vertices.push_back(new Point3D(c.x+50, c.y+50, c.z+100));
	vertices.push_back(new Point3D(c.x-50, c.y+50, c.z+100));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	Object3D *caroserie = new Object3D(vertices,faces,Color(1,0,0),false);
	player->addObject3D_to_Visual2D(caroserie,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//sasiul masinii
	vertices.push_back(new Point3D(c.x-60, c.y+10, c.z-90));
	vertices.push_back(new Point3D(c.x+60, c.y+10, c.z-90));
	vertices.push_back(new Point3D(c.x+60, c.y+10, c.z+90));
	vertices.push_back(new Point3D(c.x-60, c.y+10, c.z+90));

	vertices.push_back(new Point3D(c.x-70, c.y+25, c.z-120));
	vertices.push_back(new Point3D(c.x+70, c.y+25, c.z-120));
	vertices.push_back(new Point3D(c.x+70, c.y+25, c.z+120));
	vertices.push_back(new Point3D(c.x-70, c.y+25, c.z+120));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	Object3D *sasiu = new Object3D(vertices,faces,Color(1,0,0),false);
	player->addObject3D_to_Visual2D(sasiu,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//roata fata stanga
	vertices.push_back(new Point3D(c.x-60, c.y, c.z-75-15/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y, c.z-75+15/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+15, c.z-75+30/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+30, c.z-75+15/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+30, c.z-75-15/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+15, c.z-75-30/sqrt(3)));

	vertices.push_back(new Point3D(c.x-70, c.y, c.z-75-15/sqrt(3)));
	vertices.push_back(new Point3D(c.x-70, c.y, c.z-75+15/sqrt(3)));
	vertices.push_back(new Point3D(c.x-70, c.y+15, c.z-75+30/sqrt(3)));
	vertices.push_back(new Point3D(c.x-70, c.y+30, c.z-75+15/sqrt(3)));
	vertices.push_back(new Point3D(c.x-70, c.y+30, c.z-75-15/sqrt(3)));
	vertices.push_back(new Point3D(c.x-70, c.y+15, c.z-75-30/sqrt(3)));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(6);
	contour.push_back(7);
	contour.push_back(8);
	contour.push_back(9);
	contour.push_back(10);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(8);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(9);
	contour.push_back(8);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(10);
	contour.push_back(9);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(11);
	contour.push_back(10);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(5);
	contour.push_back(0);
	contour.push_back(6);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	Object3D *roata1 = new Object3D(vertices,faces,Color(0,1,0),false);
	player->addObject3D_to_Visual2D(roata1,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//roata fata dreapta
	vertices.push_back(new Point3D(c.x+60, c.y, c.z-75-15/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y, c.z-75+15/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+15, c.z-75+30/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+30, c.z-75+15/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+30, c.z-75-15/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+15, c.z-75-30/sqrt(3)));

	vertices.push_back(new Point3D(c.x+70, c.y, c.z-75-15/sqrt(3)));
	vertices.push_back(new Point3D(c.x+70, c.y, c.z-75+15/sqrt(3)));
	vertices.push_back(new Point3D(c.x+70, c.y+15, c.z-75+30/sqrt(3)));
	vertices.push_back(new Point3D(c.x+70, c.y+30, c.z-75+15/sqrt(3)));
	vertices.push_back(new Point3D(c.x+70, c.y+30, c.z-75-15/sqrt(3)));
	vertices.push_back(new Point3D(c.x+70, c.y+15, c.z-75-30/sqrt(3)));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(6);
	contour.push_back(7);
	contour.push_back(8);
	contour.push_back(9);
	contour.push_back(10);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(8);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(9);
	contour.push_back(8);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(10);
	contour.push_back(9);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(11);
	contour.push_back(10);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(5);
	contour.push_back(0);
	contour.push_back(6);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	Object3D *roata2 = new Object3D(vertices,faces,Color(0,1,0),false);
	player->addObject3D_to_Visual2D(roata2,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//roata spate dreapta
	vertices.push_back(new Point3D(c.x+60, c.y, c.z+75-20/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y, c.z+75+20/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+20, c.z+75+40/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+40, c.z+75+20/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+40, c.z+75-20/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+20, c.z+75-40/sqrt(3)));

	vertices.push_back(new Point3D(c.x+75, c.y, c.z+75-20/sqrt(3)));
	vertices.push_back(new Point3D(c.x+75, c.y, c.z+75+20/sqrt(3)));
	vertices.push_back(new Point3D(c.x+75, c.y+20, c.z+75+40/sqrt(3)));
	vertices.push_back(new Point3D(c.x+75, c.y+40, c.z+75+20/sqrt(3)));
	vertices.push_back(new Point3D(c.x+75, c.y+40, c.z+75-20/sqrt(3)));
	vertices.push_back(new Point3D(c.x+75, c.y+20, c.z+75-40/sqrt(3)));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(6);
	contour.push_back(7);
	contour.push_back(8);
	contour.push_back(9);
	contour.push_back(10);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(8);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(9);
	contour.push_back(8);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(10);
	contour.push_back(9);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(11);
	contour.push_back(10);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(5);
	contour.push_back(0);
	contour.push_back(6);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	Object3D *roata3 = new Object3D(vertices,faces,Color(0,1,0),false);
	player->addObject3D_to_Visual2D(roata3,teren);

	vertices.clear();
	contour.clear();
	faces.clear();

	//roata spate stanga
	vertices.push_back(new Point3D(c.x-60, c.y, c.z+75-20/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y, c.z+75+20/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+20, c.z+75+40/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+40, c.z+75+20/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+40, c.z+75-20/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+20, c.z+75-40/sqrt(3)));

	vertices.push_back(new Point3D(c.x-75, c.y, c.z+75-20/sqrt(3)));
	vertices.push_back(new Point3D(c.x-75, c.y, c.z+75+20/sqrt(3)));
	vertices.push_back(new Point3D(c.x-75, c.y+20, c.z+75+40/sqrt(3)));
	vertices.push_back(new Point3D(c.x-75, c.y+40, c.z+75+20/sqrt(3)));
	vertices.push_back(new Point3D(c.x-75, c.y+40, c.z+75-20/sqrt(3)));
	vertices.push_back(new Point3D(c.x-75, c.y+20, c.z+75-40/sqrt(3)));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(6);
	contour.push_back(7);
	contour.push_back(8);
	contour.push_back(9);
	contour.push_back(10);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(8);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(9);
	contour.push_back(8);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(10);
	contour.push_back(9);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(11);
	contour.push_back(10);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(5);
	contour.push_back(0);
	contour.push_back(6);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	Object3D *roata4 = new Object3D(vertices,faces,Color(0,1,0),false);
	player->addObject3D_to_Visual2D(roata4,teren);

	//aplic transformarea de proiectie la aparitie
	player->deplasare();
}

//functia de creare a inamicului de tip 1 (o piramida)
void creare_inamic1(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;
	inamic1.push_back(new Ansamblu(c,_raza,_vit,_rot_vit,_scor));	

	vertices.push_back(new Point3D(c.x-60, c.y, c.z-60));
	vertices.push_back(new Point3D(c.x+60, c.y, c.z-60));
	vertices.push_back(new Point3D(c.x+60, c.y, c.z+60));
	vertices.push_back(new Point3D(c.x-60, c.y, c.z+60));

	vertices.push_back(new Point3D(c.x, c.y+80, c.z));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	Object3D *piramida = new Object3D(vertices,faces,Color(1,0,1),false);
	inamic1[inamic1.size()-1]->addObject3D_to_Visual2D(piramida,teren);

	//aplic transformarea de proiectie la aparitie
	inamic1[inamic1.size()-1]->deplasare();
}

//functia de creare a inamicului de tip 2 (un cub)
void creare_inamic2(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;
	inamic2.push_back(new Ansamblu(c,_raza,_vit,_rot_vit,_scor));	

	vertices.push_back(new Point3D(c.x-60, c.y, c.z-60));
	vertices.push_back(new Point3D(c.x+60, c.y, c.z-60));
	vertices.push_back(new Point3D(c.x+60, c.y, c.z+60));
	vertices.push_back(new Point3D(c.x-60, c.y, c.z+60));

	vertices.push_back(new Point3D(c.x-60, c.y+120, c.z-60));
	vertices.push_back(new Point3D(c.x+60, c.y+120, c.z-60));
	vertices.push_back(new Point3D(c.x+60, c.y+120, c.z+60));
	vertices.push_back(new Point3D(c.x-60, c.y+120, c.z+60));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	Object3D *paralelipiped = new Object3D(vertices,faces,Color(0,1,1),false);
	inamic2[inamic2.size()-1]->addObject3D_to_Visual2D(paralelipiped,teren);

	//aplic transformarea de proiectie la aparitie
	inamic2[inamic2.size()-1]->deplasare();
}

//functia de creare a inamicului de tip 3 (un octogon 3d)
void creare_inamic3(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;
	inamic3.push_back(new Ansamblu(c,_raza,_vit,_rot_vit,_scor));	

	vertices.push_back(new Point3D(c.x+60, c.y-60, c.z-60/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y-60, c.z+60/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y, c.z+120/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+60, c.z+60/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y+60, c.z-60/sqrt(3)));
	vertices.push_back(new Point3D(c.x+60, c.y, c.z-120/sqrt(3)));

	vertices.push_back(new Point3D(c.x-60, c.y-60, c.z-60/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y-60, c.z+60/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y, c.z+120/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+60, c.z+60/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y+60, c.z-60/sqrt(3)));
	vertices.push_back(new Point3D(c.x-60, c.y, c.z-120/sqrt(3)));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(6);
	contour.push_back(7);
	contour.push_back(8);
	contour.push_back(9);
	contour.push_back(10);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(8);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(9);
	contour.push_back(8);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(10);
	contour.push_back(9);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(11);
	contour.push_back(10);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(5);
	contour.push_back(0);
	contour.push_back(6);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	Object3D *roata = new Object3D(vertices,faces,Color(1,1,0),false);
	inamic3[inamic3.size()-1]->addObject3D_to_Visual2D(roata,teren);

	//aplic transformarea de proiectie la aparitie
	inamic3[inamic3.size()-1]->deplasare();
}

//functia de creare a unui bonus de tip 1 (o inima)
void creare_bonus1(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	

	vertices.push_back(new Point3D(c.x,c.y-45,c.z));
	vertices.push_back(new Point3D(c.x-30,c.y-15,c.z-10));
	vertices.push_back(new Point3D(c.x-42,c.y+15,c.z-15));
	vertices.push_back(new Point3D(c.x-30,c.y+30,c.z-10));
	vertices.push_back(new Point3D(c.x-7.5,c.y+30,c.z-10));
	vertices.push_back(new Point3D(c.x,c.y+5,c.z-10));
	vertices.push_back(new Point3D(c.x+7.5,c.y+30,c.z-10));
	vertices.push_back(new Point3D(c.x+30,c.y+30,c.z-10));
	vertices.push_back(new Point3D(c.x+42,c.y+15,c.z-15));
	vertices.push_back(new Point3D(c.x+30,c.y-15,c.z-10));

	vertices.push_back(new Point3D(c.x-30,c.y-15,c.z+10));
	vertices.push_back(new Point3D(c.x-42,c.y+15,c.z+15));
	vertices.push_back(new Point3D(c.x-30,c.y+30,c.z+10));
	vertices.push_back(new Point3D(c.x-7.5,c.y+30,c.z+10));
	vertices.push_back(new Point3D(c.x,c.y+5,c.z+10));
	vertices.push_back(new Point3D(c.x+7.5,c.y+30,c.z+10));
	vertices.push_back(new Point3D(c.x+30,c.y+30,c.z+10));
	vertices.push_back(new Point3D(c.x+42,c.y+15,c.z+15));
	vertices.push_back(new Point3D(c.x+30,c.y-15,c.z+10));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(10);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(11);
	contour.push_back(10);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(12);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(13);
	contour.push_back(12);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(14);
	contour.push_back(13);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(15);
	contour.push_back(14);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(6);
	contour.push_back(7);
	contour.push_back(16);
	contour.push_back(15);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(7);
	contour.push_back(8);
	contour.push_back(17);
	contour.push_back(16);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(8);
	contour.push_back(9);
	contour.push_back(18);
	contour.push_back(17);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(9);
	contour.push_back(0);
	contour.push_back(18);
	faces.push_back(new Face(contour));

	Object3D *inima = new Object3D(vertices,faces,Color(1,0,0),true);
	bonus->addObject3D_to_Visual2D(inima,teren);

	//aplic transformarea de proiectie la aparitie
	bonus->deplasare();
}

//functia de creare a unui bonus de tip 2 (un paralelipiped)
void creare_bonus2(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	

	vertices.push_back(new Point3D(c.x-37.5,c.y-22.5,c.z-10));
	vertices.push_back(new Point3D(c.x-37.5,c.y+22.5,c.z-10));
	vertices.push_back(new Point3D(c.x+37.5,c.y+22.5,c.z-10));
	vertices.push_back(new Point3D(c.x+37.5,c.y-22.5,c.z-10));

	vertices.push_back(new Point3D(c.x-37.5,c.y-22.5,c.z+10));
	vertices.push_back(new Point3D(c.x-37.5,c.y+22.5,c.z+10));
	vertices.push_back(new Point3D(c.x+37.5,c.y+22.5,c.z+10));
	vertices.push_back(new Point3D(c.x+37.5,c.y-22.5,c.z+10));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	Object3D *para = new Object3D(vertices,faces,Color(0,1,0),true);
	bonus->addObject3D_to_Visual2D(para,teren);

	//aplic transformarea de proiectie la aparitie
	bonus->deplasare();
}

//functia de creare a unui bonus de tip 3 (un scut)
void creare_bonus3(float x, float y, float z, float _raza, float _vit, float _rot_vit, int _scor)
{
	Point3D c;
	c.x = x;
	c.y = y;
	c.z = z;
	vector <Point3D*> vertices;
	vector <int> contour;
	vector <Face*> faces;
	bonus = new Ansamblu(c,_raza,_vit,_rot_vit,_scor);	

	vertices.push_back(new Point3D(c.x,c.y-45,c.z));
	vertices.push_back(new Point3D(c.x+30,c.y-15,c.z-10));
	vertices.push_back(new Point3D(c.x+30,c.y+30,c.z-10));
	vertices.push_back(new Point3D(c.x-30,c.y+30,c.z-10));
	vertices.push_back(new Point3D(c.x-30,c.y-15,c.z-10));

	vertices.push_back(new Point3D(c.x+30,c.y-15,c.z+10));
	vertices.push_back(new Point3D(c.x+30,c.y+30,c.z+10));
	vertices.push_back(new Point3D(c.x-30,c.y+30,c.z+10));
	vertices.push_back(new Point3D(c.x-30,c.y-15,c.z+10));

	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(8);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(8);
	contour.push_back(0);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	Object3D *scut = new Object3D(vertices,faces,Color(0,0,1),true);
	bonus->addObject3D_to_Visual2D(scut,teren);

	//aplic transformarea de proiectie la aparitie
	bonus->deplasare();
}

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	if(full == true)
		glutFullScreen();	//trecerea in fullscreen a ferestrei (RECOMAND folosirea fullscreen)

	//crearea contextului vizual titlu, a scorului, a timpului, a distantei si a vietii
	titlu = new Visual2D(0,0,DrawingWindow::width,DrawingWindow::height/11,
						 0,0,DrawingWindow::width,DrawingWindow::height/11);  
	titlu->cadruFereastra(Color(0,1,0));
	titlu->tipTran(true);
	addVisual2D(titlu);

	Text *titlu1 = new Text(" - CRAZY DRIVER -       Copyright : Bojin Andrei", 
							Point2D(DrawingWindow::width/9, DrawingWindow::height/27), 
							Color(0,1,1), BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(titlu1, titlu);
	Text *scor_text = new Text("-< SCORE >-", Point2D(DrawingWindow::width/2.14, DrawingWindow::height/20), 
							Color(1,0.5,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(scor_text, titlu);
	scor = new Text("00000", Point2D(DrawingWindow::width/2.063, DrawingWindow::height/35), 
							Color(0,1,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(scor, titlu);
	Text *distance = new Text("-< DISTANCE >-", Point2D(DrawingWindow::width*1.87/3, DrawingWindow::height/18), 
							Color(1,0.5,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(distance, titlu);

	//crearea obiectelor atasate distantei
	Point2D c;
	c.x = DrawingWindow::width*1.985/3;
	c.y = DrawingWindow::height/33;

	//punctul rosu care reprezinta masina pe parcursul drumului
	minicar = new Circle2D(Point2D(c.x-140,c.y),10,Color(0.7,0,0),true);
	addObject2D_to_Visual2D(minicar,titlu);

	//drumul reprezentat sub forma unui segment gradat
	Polygon2D *poly = new Polygon2D(Color(0,0.5,0),false);
	poly->addPoint(Point2D(c.x-2,c.y-2));
	poly->addPoint(Point2D(c.x-2,c.y-10));
	poly->addPoint(Point2D(c.x+2,c.y-10));
	poly->addPoint(Point2D(c.x+2,c.y-2));
	poly->addPoint(Point2D(c.x+72,c.y-2));
	poly->addPoint(Point2D(c.x+72,c.y-7));
	poly->addPoint(Point2D(c.x+76,c.y-7));
	poly->addPoint(Point2D(c.x+76,c.y-2));
	poly->addPoint(Point2D(c.x+146,c.y-2));
	poly->addPoint(Point2D(c.x+146,c.y-10));
	poly->addPoint(Point2D(c.x+150,c.y-10));
	poly->addPoint(Point2D(c.x+150,c.y+10));
	poly->addPoint(Point2D(c.x+146,c.y+10));
	poly->addPoint(Point2D(c.x+146,c.y+2));
	poly->addPoint(Point2D(c.x+76,c.y+2));
	poly->addPoint(Point2D(c.x+76,c.y+7));
	poly->addPoint(Point2D(c.x+72,c.y+7));
	poly->addPoint(Point2D(c.x+72,c.y+2));
	poly->addPoint(Point2D(c.x+2,c.y+2));
	poly->addPoint(Point2D(c.x+2,c.y+10));
	poly->addPoint(Point2D(c.x-2,c.y+10));
	poly->addPoint(Point2D(c.x-2,c.y+2));
	poly->addPoint(Point2D(c.x-72,c.y+2));
	poly->addPoint(Point2D(c.x-72,c.y+7));
	poly->addPoint(Point2D(c.x-76,c.y+7));
	poly->addPoint(Point2D(c.x-76,c.y+2));
	poly->addPoint(Point2D(c.x-146,c.y+2));
	poly->addPoint(Point2D(c.x-146,c.y+10));
	poly->addPoint(Point2D(c.x-150,c.y+10));
	poly->addPoint(Point2D(c.x-150,c.y-10));
	poly->addPoint(Point2D(c.x-146,c.y-10));
	poly->addPoint(Point2D(c.x-146,c.y-2));
	poly->addPoint(Point2D(c.x-76,c.y-2));
	poly->addPoint(Point2D(c.x-76,c.y-7));
	poly->addPoint(Point2D(c.x-72,c.y-7));
	poly->addPoint(Point2D(c.x-72,c.y-2));
	addObject2D_to_Visual2D(poly,titlu);

	Text *Timp = new Text("Time:", Point2D(DrawingWindow::width*2.38/3, DrawingWindow::height/26), 
							Color(1,0.5,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(Timp, titlu);
	timp = new Text("00:00", Point2D(DrawingWindow::width*2.465/3, DrawingWindow::height/26), 
							Color(0,1,0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(timp, titlu);

	//timpul de start al jocului
	startime = clock();

	//crearea obiectelor atasate numarului de vieti din dreapta sus
	c.x = DrawingWindow::width*5.5/6;
	c.y = DrawingWindow::height/22;

	//x-ul
	Polygon2D *poly1 = new Polygon2D(Color(1,0.5,0),false);
	poly1->addPoint(Point2D(c.x-36,c.y));
	poly1->addPoint(Point2D(c.x-28,c.y+10));
	poly1->addPoint(Point2D(c.x-38,c.y+2));
	poly1->addPoint(Point2D(c.x-48,c.y+10));
	poly1->addPoint(Point2D(c.x-40,c.y));
	poly1->addPoint(Point2D(c.x-48,c.y-10));
	poly1->addPoint(Point2D(c.x-38,c.y-2));
	poly1->addPoint(Point2D(c.x-28,c.y-10));
	addObject2D_to_Visual2D(poly1,titlu);

	//inima
	Polygon2D *poly2 = new Polygon2D(Color(1,0,0),true);
	poly2->addPoint(Point2D(c.x,c.y-15));
	poly2->addPoint(Point2D(c.x-10,c.y-5));
	poly2->addPoint(Point2D(c.x-14,c.y+5));
	poly2->addPoint(Point2D(c.x-10,c.y+10));
	poly2->addPoint(Point2D(c.x-2.5,c.y+10));
	poly2->addPoint(Point2D(c.x,c.y+5));
	poly2->addPoint(Point2D(c.x+2.5,c.y+10));
	poly2->addPoint(Point2D(c.x+10,c.y+10));
	poly2->addPoint(Point2D(c.x+14,c.y+5));
	poly2->addPoint(Point2D(c.x+10,c.y-5));
	addObject2D_to_Visual2D(poly2,titlu);

	//aranjarea acestora in fereastra
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*c.x,(-1)*c.y);
	Transform2D::scaleMatrix(0.5,0.5);
	Transform2D::translateMatrix(c.x,c.y);
	Transform2D::applyTransform(poly1);
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix((-1)*c.x,(-1)*c.y);
	Transform2D::scaleMatrix(0.7,0.7);
	Transform2D::translateMatrix(c.x,c.y+1);
	Transform2D::applyTransform(poly2);

	viata = new Text(std::to_string(life), Point2D(c.x-40, c.y-8), Color(0,1,0), BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(viata, titlu);

	//crearea contextului vizual teren
	teren = new Visual2D(-DrawingWindow::width/2,-DrawingWindow::height*10/11,DrawingWindow::width/2,0,
						 0,DrawingWindow::height/11,DrawingWindow::width,DrawingWindow::height); 
	teren->cadruFereastra(Color(1,0,0));
	teren->tipTran(true);
	addVisual2D(teren);

	//crearea masinii playerului
	creare_player(120, -DrawingWindow::width/3, 100, 75, v, vr, 0);

	//crearea benzilor drumului
	Point3D c1;
	c1.x = 0;
	c1.y = -DrawingWindow::width/3;
	c1.z = -DrawingWindow::width/2*sqrt(2);
	benzi1 = new Ansamblu(c1,0,0,0,0);
	benzi2 = new Ansamblu(c1,0,0,0,0);
	int i;
	for(i=0; i<DrawingWindow::width*2-140; i=i+140) {
		creare_benzi1(c1.x,c1.y,c1.z+i,0,0,0,0);
		creare_benzi2(c1.x,c1.y,c1.z+i,0,0,0,0);
	}
	//aplic transformarea de proiectie la aparitie
	benzi1->deplasare();
	benzi2->deplasare();

	//crearea drumului
	creare_drum(0,-DrawingWindow::width/3,-DrawingWindow::width/2*sqrt(2),0,0,0,0);

}

//functia de creare a unui inamic de un anumit tip pe o anumita banda
void creare_inamic(int i, int x)
{
	switch(i) {
		case 1: {
			creare_inamic1( x, -DrawingWindow::width/3, -DrawingWindow::width/2*sqrt(2)-DrawingWindow::width, 65, v1, vr1, s1);
		} break;
		case 2: {
			creare_inamic2( x, -DrawingWindow::width/3, -DrawingWindow::width/2*sqrt(2)-DrawingWindow::width, 65*sqrt(2), v2, vr2, s2);
		} break;
		case 3: {
			creare_inamic3( x, -DrawingWindow::width/3+60, -DrawingWindow::width/2*sqrt(2)-DrawingWindow::width, 65, v3, vr3, s3);
		} break;
	}
}

//functia de creare a unui inamic random la o locatie random
void updatei()
{
	int id = 1+rand()%3;					//id-ul random al tipului inamicului
	int v[4] = {-350,-120,120,350};		//un vector cu posibilele pozitii pe Ox
	int x = 0+rand()%(4);				//alegerea unui indice random din vector

	creare_inamic(id,v[x]);				//crearea inamicului cu parametrii calculati
}

//functia de creare a unui bonus de un anumit tip la anumite coordonate
void creare_bonus(int b, int x)
{
	switch(b) {
		case 1: {
			creare_bonus1(x, -DrawingWindow::width/3, -DrawingWindow::width/2*sqrt(2)-DrawingWindow::width, 45, 0, 0.05, 0);
		} break;
		case 2: {
			creare_bonus2(x, -DrawingWindow::width/3, -DrawingWindow::width/2*sqrt(2)-DrawingWindow::width, 45, 0, 0.05, 0);
		} break;
		case 3: {
			creare_bonus3(x, -DrawingWindow::width/3, -DrawingWindow::width/2*sqrt(2)-DrawingWindow::width, 45, 0, 0.05, 0);
		} break;
	}
}

//functia de creare a unui bonus random la o locatie random
void updateb()
{
	bid = 1+rand()%3;					//id-ul random al tipului bonusului
	int v[4] = {-350,-120,120,350};		//un vector cu posibilele pozitii pe Ox
	int x = 0+rand()%(4);				//alegerea unui indice random din vector

	creare_bonus(bid,v[x]);				//crearea bonusului cu parametrii calculati
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
	}
	else {	//afisarea unor mesaje in caz ca s-a realizat imposibilul: ajungerea la punctajul limita (99999) ! :D
		Text *end1 = new Text("CONGRATULATIONS!", Point2D(-100, 
								-DrawingWindow::height/17), Color(0,1,0), BITMAP_HELVETICA_18);
		DrawingWindow::addText_to_Visual2D(end1, teren);
		Text *end2 = new Text("You have beaten", Point2D(-73, 
								-DrawingWindow::height/11), Color(1,0.5,0), BITMAP_HELVETICA_18);
		DrawingWindow::addText_to_Visual2D(end2, teren);
		Text *end3 = new Text("- CRAZY DRIVER -", Point2D(-87, 
								-DrawingWindow::height/8), Color(0,1,1), BITMAP_HELVETICA_18);
		DrawingWindow::addText_to_Visual2D(end3, teren);

		game_over = true;	//s-a terminat jocul
	}
}

//functia de miscare a inamicilor de un anumit tip in fereastra
void move_inamici(vector <Ansamblu*> inamic)
{
	int i;
	for(i=0; i<inamic.size(); i++) {
		//sterge inamicul daca a parasit fereastra (drumul)
		if((int)inamic[i]->pasz+100-2*DrawingWindow::width >= 0 || 
		  ((int)inamic[i]->pasz < 0 && (int)inamic[i]->pasy == 0)) { 
			if((int)inamic[i]->pasz+100-2*DrawingWindow::width >= 0)
				//daca inamicul e pe cale sa iasa din fereastra, i se adauga scorul
				add_score(inamic[i]->scor);
			inamic[i]->removeAnsamblu_from_Visual2D(teren);
			if(inamic == inamic1)
				inamic1.erase(inamic1.begin() + removeAnsamblu(inamic1[i], inamic1));
			if(inamic == inamic2)
				inamic2.erase(inamic2.begin() + removeAnsamblu(inamic2[i], inamic2));
			if(inamic == inamic3)
				inamic3.erase(inamic3.begin() + removeAnsamblu(inamic3[i], inamic3));
		}
		else {
			//pentru inamici de tipul 3
			if(inamic == inamic3) {
				//viteza inamicului este una relativa la viteza playerului
				inamic[i]->pasz += vit_temp-inamic[i]->vit;
				//se executa o miscare de tip "bouncing ball"
				if(inamic[i]->pasx == 0) {
					inamic[i]->pasy += inamic[i]->rot_vit;
					inamic[i]->rot_vit -= 0.5;
				}
				else {
					inamic[i]->pasy -= inamic[i]->rot_vit;
					inamic[i]->rot_vit += 0.5;
				}
				if(inamic[i]->rot_vit == 0)
					inamic[i]->pasx = 0.01;
				if(inamic[i]->pasy == 0)
					inamic[i]->pasx = 0;					
			}
			else
			//viteza celorlalti este aceeasi cu a playerului(sunt statici)
				if(vit_temp >= 0)
					inamic[i]->pasz += vit_temp;

			//singurul inamic care nu se roteste este cel de tip 1
			if(inamic != inamic1)
				inamic[i]->unghi += 0.1;
			//inamicii de tip 3 se rotesc in jurul lui Ox
			if(inamic == inamic3) {
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(inamic[i]->pasx,inamic[i]->pasy,inamic[i]->pasz);
				Transform3D::translateMatrix((-1)*(inamic[i]->center.x+inamic[i]->pasx),
					(-1)*(inamic[i]->center.y+inamic[i]->pasy),(-1)*(inamic[i]->center.z+inamic[i]->pasz));
				Transform3D::rotateMatrixOx(-inamic[i]->unghi);
				Transform3D::translateMatrix(inamic[i]->center.x+inamic[i]->pasx,
					inamic[i]->center.y+inamic[i]->pasy,inamic[i]->center.z+inamic[i]->pasz);
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				int j;
				for(j=0; j < inamic[i]->objects3D.size(); j++)
					Transform3D::applyTransform(inamic[i]->objects3D[j]);
			}
			//restul inamicilor se misca normal, cu rotatii in jurul lui Oy
			else
				inamic[i]->deplasare();			
		}
	}
}

//functia de miscare a bonusului
void move_bonus()
{
	if(bid != 0) {
		//sterge bonusul daca a parasit fereastra (drumul)
		if((int)bonus->pasz+100-2*DrawingWindow::width >= 0) {
			bonus->removeAnsamblu_from_Visual2D(teren);
			bid = 0;
		}
		else {
			//viteza bonusului este aceeasi cu a playerului(sunt statici)
			if(vit_temp >= 0)
				bonus->pasz += vit_temp;
			bonus->unghi += 0.1;
			bonus->deplasare();			
		}
	}
}

//functia de testare a coliziunii corpului playerului cu inamicii de tipul 1 si 2
Ansamblu* coliziune_corp12(vector <Ansamblu*> inamic)
{
	int i;
	float d, x2, x1, z2, z1;
	x2 = player->center.x+player->pasx-50*sin(player->unghi);
	z2 = player->center.z+player->pasz-50*cos(player->unghi);
	for(i=0; i<inamic.size(); i++) {
		x1 = inamic[i]->center.x+inamic[i]->pasx;
		z1 = inamic[i]->center.z+inamic[i]->pasz;
		d = sqrt((x2-x1)*(x2-x1)+(z2-z1)*(z2-z1));
		if(d <= player->raza+inamic[i]->raza) {
			inamic[i]->scor = 0;
			return inamic[i];	//intoarce inamicul cu care s-a ciocnit
		}
	}
	return NULL;
}

//functia de testare a coliziunii corpului playerului cu inamicii de tipul 3
Ansamblu* coliziune_corp3(vector <Ansamblu*> inamic)
{
	int i;
	float d2, d1, x3, x2, x1, y2, y1, z3, z2, z1;
	x3 = player->center.x+player->pasx+50*sin(player->unghi);
	x2 = player->center.x+player->pasx-50*sin(player->unghi);
	y2 = player->center.y+player->pasy;
	z3 = player->center.z+player->pasz+50*cos(player->unghi);
	z2 = player->center.z+player->pasz-50*cos(player->unghi);
	for(i=0; i<inamic.size(); i++) {
		x1 = inamic[i]->center.x+inamic[i]->pasx;
		y1 = inamic[i]->center.y+inamic[i]->pasy;
		z1 = inamic[i]->center.z+inamic[i]->pasz;
		d2 = sqrt((x3-x1)*(x3-x1)+(y2-y1)*(y2-y1)+(z3-z1)*(z3-z1));
		d1 = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
		if(d2 <= player->raza+inamic[i]->raza || d1 <= player->raza+inamic[i]->raza) {
			inamic[i]->scor = 0;
			return inamic[i];	//intoarce inamicul cu care s-a ciocnit
		}
	}
	return NULL;
}

//functia de testare a coliziunii corpului playerului cu un bonus
bool coliziune_bonus()
{
	float d, x2, x1, z2, z1;
	x2 = player->center.x+player->pasx-50*sin(player->unghi);
	z2 = player->center.z+player->pasz-50*cos(player->unghi);
	x1 = bonus->center.x+bonus->pasx;
	z1 = bonus->center.z+bonus->pasz;
	d = sqrt((x2-x1)*(x2-x1)+(z2-z1)*(z2-z1));
	if(d <= player->raza+bonus->raza)
		return true;
	return false;
}

//functia care realizeaza procesarea coliziunilor dintre obiectele de pe teren si 
//a consecintelor acestora
void coliziune()
{
	//procesarea coliziunilor corpului playerului cu inamicii
	//in cazul coliziunii consecutive cu acelasi inamic, acest lucru se testeaza o data la 0.5 sec
	//pentru a se putea actualiza corect numarul de vietii
	if(invinc == false) {	//daca playerul nu a luat un bonus de invincibilitate
		Ansamblu *a1, *a2, *a3;
		a1 = coliziune_corp12(inamic1);
		a2 = coliziune_corp12(inamic2);
		a3 = coliziune_corp3(inamic3);
		if(a1 != NULL || a2 != NULL || a3 != NULL) {
			if(aux == NULL || (aux != a1 && aux != a2 && aux != a3) || (col_int > 20)) {
				life--;	//viata scade
				//reafisarea vietii
				DrawingWindow::removeText_from_Visual2D(viata, titlu);
				Point2D c;
				c.x = DrawingWindow::width*5.5/6;
				c.y = DrawingWindow::height/22;
				viata = new Text(std::to_string(life), Point2D(c.x-40, c.y-8), 
								 Color(0,1,0), BITMAP_TIMES_ROMAN_24);
				DrawingWindow::addText_to_Visual2D(viata, titlu);
				//afisarea unui mesaj corespunzator pentru a anunta playerul ca s-a ciocnit
				minushp = new Text("!!! CRASH !!!", Point2D(-60, 
										 -DrawingWindow::height/8), Color(1,0,0), BITMAP_HELVETICA_18);
				DrawingWindow::addText_to_Visual2D(minushp, teren);

				text_minushp = 0;

				if(life == 0) {	 //daca s-a ajuns la 0 vieti
					//se afiseaza un mesaj corespunzator impreuna cu scorul final
					Text *end = new Text("GAME OVER", Point2D(-60, 
										 -DrawingWindow::height/17), Color(1,0,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(end, teren);
					Text *final_scor = new Text("FINAL SCORE:", Point2D(-100, 
												-DrawingWindow::height/11), Color(1,0.5,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(final_scor, teren);
					int i;
					string s="";
					for(i=0; i < 5-std::to_string(score).size(); i++)
						s += "0";
					Text *sc = new Text(s+std::to_string(score), Point2D(40, 
										-DrawingWindow::height/11), Color(0,1,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(sc, teren);

					game_over = true;	//s-a terminat jocul
				}
				if(a1 != NULL)	aux = a1;
				if(a2 != NULL)	aux = a2;
				if(a3 != NULL)	aux = a3;
			}
			col_int = 0;
		}
	}
	col_int ++;

	//se sterge mesajul pierderii unei vieti (crash)
	if(minushp != NULL && text_minushp >= text_lim)
		DrawingWindow::removeText_from_Visual2D(minushp, teren);
	text_minushp ++;

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
					c.x = DrawingWindow::width*5.5/6;
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
					plushp = new Text("+1 life", Point2D(-30, 
									  -DrawingWindow::height/7), Color(0,1,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(plushp, teren);

					text_plushp = 0;
				} break;
				//bonus de tip 2: cresterea scorului cu valoarea b_s
				case 2: {
					add_score(b_s);
					//afisarea unui mesaj corespunzator
					pluspct = new Text("+ "+std::to_string(b_s)+" pct", Point2D(-40, 
										-DrawingWindow::height/7), Color(1,0.75,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(pluspct, teren);

					text_pluspct = 0;
				} break;
				//bonus de tip 3: invincibilitatea pentru player pentru b_lim sec
				case 3: {
					invinc = true;
					//afisarea unui mesaj corespunzator
					invul = new Text("invincible ("+std::to_string(b_lim/50)+" sec)", Point2D(-60, 
										-DrawingWindow::height/7), Color(0,1,1), BITMAP_HELVETICA_18);
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
		invinc = false;
	}
	b_time ++;

	//se sterge mesajul corespunzator bonusului de tip 1
	if(plushp != NULL && text_plushp >= text_lim)
		DrawingWindow::removeText_from_Visual2D(plushp, teren);
	text_plushp ++;
	//se sterge mesajul corespunzator bonusului de tip 2
	if(pluspct != NULL && text_pluspct >= text_lim)
		DrawingWindow::removeText_from_Visual2D(pluspct, teren);
	text_pluspct ++;
	//se sterge mesajul corespunzator bonusului de tip 3
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
			//acceleratia
			if(vit_temp <= player->vit)
				vit_temp += acc;
			//miscarea benzii intermitente stangi
			benzi1->pasz += vit_temp;
			Transform3D::loadIdentityModelMatrix();
			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::translateMatrix(benzi1->pasx,benzi1->pasy,benzi1->pasz);
			Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
			int i;
			for(i=0; i <= benzi1->objects3D.size()-ind; i++)
				Transform3D::applyTransform(benzi1->objects3D[i]);
			for(i=benzi1->objects3D.size()-ind+1; i <= benzi1->objects3D.size()-1; i++) {
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(benzi1->pasx,benzi1->pasy,
											 benzi1->pasz-140*(benzi1->objects3D.size()-i));
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				Transform3D::applyTransform(benzi1->objects3D[i]);
			}
			//miscarea benzii intermitente drepti
			benzi2->pasz += vit_temp;
			Transform3D::loadIdentityModelMatrix();
			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::translateMatrix(benzi2->pasx,benzi2->pasy,benzi2->pasz);
			Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
			for(i=0; i <= benzi2->objects3D.size()-ind; i++)
				Transform3D::applyTransform(benzi2->objects3D[i]);
			for(i=benzi2->objects3D.size()-ind+1; i <= benzi2->objects3D.size()-1; i++) {
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(benzi2->pasx,benzi2->pasy,
											 benzi2->pasz-140*(benzi2->objects3D.size()-i));
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				Transform3D::applyTransform(benzi2->objects3D[i]);
			}
			//daca benzile au iesit din fereastra (drum)
			if((int)benzi1->pasz+70-140*ind >= 0)
			{
				//sterg benzile iesite 
				benzi1->removeObject3D_from_Visual2D(benzi1->objects3D[benzi1->objects3D.size()-ind],teren);
				benzi2->removeObject3D_from_Visual2D(benzi2->objects3D[benzi2->objects3D.size()-ind],teren);
				//adaug noi benzi la orizont
				Point3D c1;
				c1.x = 0;
				c1.y = -DrawingWindow::width/3;
				c1.z = -DrawingWindow::width/2*sqrt(2);
				creare_benzi1(c1.x,c1.y,c1.z,0,0,0,0);
				creare_benzi2(c1.x,c1.y,c1.z,0,0,0,0);
				ind ++;
				//daca s-au epuizat toate benzile vechi se creaza noi benzi
				if(ind > benzi1->objects3D.size()) {
					ind = 1;
					benzi1->pasz = 0;
					benzi2->pasz = 0;
					benzi1->removeAnsamblu_from_Visual2D(teren);
					benzi2->removeAnsamblu_from_Visual2D(teren);
					Point3D c1;
					c1.x = 0;
					c1.y = -DrawingWindow::width/3;
					c1.z = -DrawingWindow::width/2*sqrt(2);
					benzi1 = new Ansamblu(c1,0,0,0,0);
					benzi2 = new Ansamblu(c1,0,0,0,0);
					int i;
					for(i=0; i<DrawingWindow::width*2-140; i=i+140) {
						creare_benzi1(c1.x,c1.y,c1.z+i,0,0,0,0);
						creare_benzi2(c1.x,c1.y,c1.z+i,0,0,0,0);
					}
				}
			}
			//miscarea masinii playerului
			player->pasx -= (vit_temp+3*(vit_temp>0))*sin(player->unghi);
			Transform3D::loadIdentityModelMatrix();
			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
			Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),
				(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
			Transform3D::rotateMatrixOy(-player->unghi);
			Transform3D::translateMatrix(player->center.x+player->pasx,
				player->center.y+player->pasy+15,player->center.z+player->pasz);
			Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
			for(i=0; i < player->objects3D.size(); i++) {
				Transform3D::applyTransform(player->objects3D[i]);
			}
		}
		else { //procesarea eliberarii tastei UP (pentru deceleratie)
			//deceleratia
			if(vit_temp >= 0)
				vit_temp -= acc;
			//miscarea benzii intermitente stangi
			if(vit_temp >= 0) {
				benzi1->pasz += vit_temp;
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(benzi1->pasx,benzi1->pasy,benzi1->pasz);
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				int i;
				for(i=0; i <= benzi1->objects3D.size()-ind; i++)
					Transform3D::applyTransform(benzi1->objects3D[i]);
				for(i=benzi1->objects3D.size()-ind+1; i <= benzi1->objects3D.size()-1; i++) {
					Transform3D::loadIdentityModelMatrix();
					Transform3D::loadIdentityProjectionMatrix();
					Transform3D::translateMatrix(benzi1->pasx,benzi1->pasy,
												 benzi1->pasz-140*(benzi1->objects3D.size()-i));
					Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
					Transform3D::applyTransform(benzi1->objects3D[i]);
				}
				//miscarea benzii intermitente drepti
				benzi2->pasz += vit_temp;
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(benzi2->pasx,benzi2->pasy,benzi2->pasz);
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				for(i=0; i <= benzi2->objects3D.size()-ind; i++)
					Transform3D::applyTransform(benzi2->objects3D[i]);
				for(i=benzi2->objects3D.size()-ind+1; i <= benzi2->objects3D.size()-1; i++) {
					Transform3D::loadIdentityModelMatrix();
					Transform3D::loadIdentityProjectionMatrix();
					Transform3D::translateMatrix(benzi2->pasx,benzi2->pasy,
												 benzi2->pasz-140*(benzi2->objects3D.size()-i));
					Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
					Transform3D::applyTransform(benzi2->objects3D[i]);
				}
				//daca benzile au iesit din fereastra (drum)
				if((int)benzi1->pasz+70-140*ind >= 0)
				{
					//sterg benzile iesite 
					benzi1->removeObject3D_from_Visual2D(benzi1->objects3D[benzi1->objects3D.size()-ind],teren);
					benzi2->removeObject3D_from_Visual2D(benzi2->objects3D[benzi2->objects3D.size()-ind],teren);
					//adaug noi benzi la orizont
					Point3D c1;
					c1.x = 0;
					c1.y = -DrawingWindow::width/3;
					c1.z = -DrawingWindow::width/2*sqrt(2);
					creare_benzi1(c1.x,c1.y,c1.z,0,0,0,0);
					creare_benzi2(c1.x,c1.y,c1.z,0,0,0,0);
					ind ++;
					//daca s-au epuizat toate benzile vechi se creaza noi benzi
					if(ind > benzi1->objects3D.size()) {
						ind = 1;
						benzi1->pasz = 0;
						benzi2->pasz = 0;
						benzi1->removeAnsamblu_from_Visual2D(teren);
						benzi2->removeAnsamblu_from_Visual2D(teren);
						Point3D c1;
						c1.x = 0;
						c1.y = -DrawingWindow::width/3;
						c1.z = -DrawingWindow::width/2*sqrt(2);
						benzi1 = new Ansamblu(c1,0,0,0,0);
						benzi2 = new Ansamblu(c1,0,0,0,0);
						int i;
						for(i=0; i<DrawingWindow::width*2-140; i=i+140) {
							creare_benzi1(c1.x,c1.y,c1.z+i,0,0,0,0);
							creare_benzi2(c1.x,c1.y,c1.z+i,0,0,0,0);
						}
					}
							
				}
			}
			//miscarea masinii playerului
			player->pasx -= (vit_temp+3*(vit_temp>0))*sin(player->unghi);
			Transform3D::loadIdentityModelMatrix();
			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
			Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),
				(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
			Transform3D::rotateMatrixOy(-player->unghi);
			Transform3D::translateMatrix(player->center.x+player->pasx,
				player->center.y+player->pasy+15,player->center.z+player->pasz);
			Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
			int i;
			for(i=0; i < player->objects3D.size(); i++) {
				Transform3D::applyTransform(player->objects3D[i]);
			}
		}
		//procesarea apasarii tastei DOWN (frana)
		if(keySpecialStates[KEY_DOWN] == true) {
			vit_temp = 0;
		}
		//procesarea apasarii tastei LEFT
		if(keySpecialStates[KEY_LEFT] == true) {
			if(player->unghi <= 0.6) {
				player->unghi += player->rot_vit;
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
				Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),
					(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
				Transform3D::rotateMatrixOy(-player->unghi);
				Transform3D::translateMatrix(player->center.x+player->pasx,
					player->center.y+player->pasy+15,player->center.z+player->pasz);
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				int i;
				for(i=0; i < player->objects3D.size(); i++) {
					Transform3D::applyTransform(player->objects3D[i]);
				}
			}
		}
		//procesarea eliberarii tastei LEFT (indreptare automata)
		else {
			if(player->unghi >= 0) {
				player->unghi -= player->rot_vit;
				if(player->unghi < 0)
					player->unghi = 0;
			}
			if(player->unghi >= 0) {
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
				Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),
					(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
				Transform3D::rotateMatrixOy(-player->unghi);
				Transform3D::translateMatrix(player->center.x+player->pasx,
					player->center.y+player->pasy+15,player->center.z+player->pasz);
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				int i;
				for(i=0; i < player->objects3D.size(); i++) {
					Transform3D::applyTransform(player->objects3D[i]);
				}
			}
		}
		//procesarea apasarii tastei RIGHT
		if(keySpecialStates[KEY_RIGHT] == true) {
			if(player->unghi >= -0.6) {
				player->unghi -= player->rot_vit;
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
				Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),
					(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
				Transform3D::rotateMatrixOy(-player->unghi);
				Transform3D::translateMatrix(player->center.x+player->pasx,
					player->center.y+player->pasy+15,player->center.z+player->pasz);
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				int i;
				for(i=0; i < player->objects3D.size(); i++) {
					Transform3D::applyTransform(player->objects3D[i]);
				}
			}
		}
		//procesarea eliberarii tastei RIGHT (indreptare automata)
		else {
			if(player->unghi <= 0) {
				player->unghi += player->rot_vit;
				if(player->unghi > 0)
					player->unghi = 0;
			}
			if(player->unghi <= 0) {
				Transform3D::loadIdentityModelMatrix();
				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
				Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),
					(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
				Transform3D::rotateMatrixOy(-player->unghi);
				Transform3D::translateMatrix(player->center.x+player->pasx,
					player->center.y+player->pasy+15,player->center.z+player->pasz);
				Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
				int i;
				for(i=0; i < player->objects3D.size(); i++) {
					Transform3D::applyTransform(player->objects3D[i]);
				}
			}
		}
		//miscarea rotii fata stanga
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
		Transform3D::translateMatrix((-1)*(player->center.x+player->pasx-65),
			(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz-75));
		Transform3D::rotateMatrixOx(-pas*(vit_temp > 0));
		if(keySpecialStates[KEY_LEFT] == true)
			Transform3D::rotateMatrixOy(-0.4);
		else
			Transform3D::rotateMatrixOy(0.4);
		if(keySpecialStates[KEY_RIGHT] == true)
			Transform3D::rotateMatrixOy(0.4);
		else
			Transform3D::rotateMatrixOy(-0.4);
		Transform3D::translateMatrix(player->center.x+player->pasx-65,player->center.y+player->pasy+15,player->center.z+player->pasz-75);
		Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
		Transform3D::rotateMatrixOy(-player->unghi);
		Transform3D::translateMatrix(player->center.x+player->pasx,player->center.y+player->pasy+15,player->center.z+player->pasz);
		Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
		Transform3D::applyTransform(player->objects3D[6]);

		//miscarea rotii fata dreapta
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
		Transform3D::translateMatrix((-1)*(player->center.x+player->pasx+65),(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz-75));
		Transform3D::rotateMatrixOx(-pas*(vit_temp > 0));
		if(keySpecialStates[KEY_LEFT] == true)
			Transform3D::rotateMatrixOy(-0.4);
		else
			Transform3D::rotateMatrixOy(0.4);
		if(keySpecialStates[KEY_RIGHT] == true)
			Transform3D::rotateMatrixOy(0.4);
		else
			Transform3D::rotateMatrixOy(-0.4);
		Transform3D::translateMatrix(player->center.x+player->pasx+65,player->center.y+player->pasy+15,player->center.z+player->pasz-75);
		Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
		Transform3D::rotateMatrixOy(-player->unghi);
		Transform3D::translateMatrix(player->center.x+player->pasx,player->center.y+player->pasy+15,player->center.z+player->pasz);
		Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
		Transform3D::applyTransform(player->objects3D[7]);

		//miscarea rotii spate dreapta
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
		Transform3D::translateMatrix((-1)*(player->center.x+player->pasx+67.5),(-1)*(player->center.y+player->pasy+20),(-1)*(player->center.z+player->pasz+75));
		Transform3D::rotateMatrixOx(-pas*(vit_temp > 0));
		Transform3D::translateMatrix(player->center.x+player->pasx+67.5,player->center.y+player->pasy+20,player->center.z+player->pasz+75);
		Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
		Transform3D::rotateMatrixOy(-player->unghi);
		Transform3D::translateMatrix(player->center.x+player->pasx,player->center.y+player->pasy+15,player->center.z+player->pasz);
		Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
		Transform3D::applyTransform(player->objects3D[8]);

		//miscarea rotii spate stanga
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(player->pasx,player->pasy,player->pasz);
		Transform3D::translateMatrix((-1)*(player->center.x+player->pasx-67.5),(-1)*(player->center.y+player->pasy+20),(-1)*(player->center.z+player->pasz+75));
		Transform3D::rotateMatrixOx(-pas*(vit_temp > 0));
		Transform3D::translateMatrix(player->center.x+player->pasx-67.5,player->center.y+player->pasy+20,player->center.z+player->pasz+75);
		Transform3D::translateMatrix((-1)*(player->center.x+player->pasx),(-1)*(player->center.y+player->pasy+15),(-1)*(player->center.z+player->pasz));
		Transform3D::rotateMatrixOy(-player->unghi);
		Transform3D::translateMatrix(player->center.x+player->pasx,player->center.y+player->pasy+15,player->center.z+player->pasz);
		Transform3D::perspectiveProjectionMatrix(0,-200,DrawingWindow::width/3);
		Transform3D::applyTransform(player->objects3D[9]);

		//pasul de rotatie al rotilor
		pas += 0.3;

		//crearea inamicilor si a bonusurilor pe masura ce se misca playerul
		//la apasarea tastei UP
		if(keySpecialStates[KEY_UP] == true) {
			int r = 20+rand()%41;
			if(contor >= r) {
				updatei();		//crearea inamicilor
				contor = 0;
			}
			if(bcontor >= r+400 && bid == 0) {
				updateb();		//crearea bonusului
				bcontor = 0;
			}
			contor ++;
			bcontor ++;

			//se actualizeaza distanta parcursa
			if(distanta <= 280) {
				Transform2D::loadIdentityMatrix();
				Transform2D::translateMatrix(distanta,0);
				Transform2D::applyTransform(minicar);
			}
			else { //s-a ajuns la sfarsitul distantei de parcurs
				Text *end1 = new Text("CONGRATULATIONS!", Point2D(-100, 
									  -DrawingWindow::height/17), Color(0,1,0), BITMAP_HELVETICA_18);
				DrawingWindow::addText_to_Visual2D(end1, teren);
				Text *end2 = new Text("You have beaten", Point2D(-73, 
									  -DrawingWindow::height/11), Color(1,0.5,0), BITMAP_HELVETICA_18);
				DrawingWindow::addText_to_Visual2D(end2, teren);
				Text *end3 = new Text("- CRAZY DRIVER -", Point2D(-87, 
									  -DrawingWindow::height/8), Color(0,1,1), BITMAP_HELVETICA_18);
				DrawingWindow::addText_to_Visual2D(end3, teren);

				game_over = true;	//s-a terminat jocul
			}
			distanta += 0.025;
		//la eliberarea tastei UP
		} else {
			if(vit_temp >= 0) {
				int r = 20+rand()%41;
				if(contor >= r) {
					updatei();		//crearea inamicilor
					contor = 0;
				}
				if(bcontor >= r+400 && bid == 0) {
					updateb();		//crearea bonusurilor
					bcontor = 0;
				}
				contor ++;
				bcontor ++;

				//se actualizeaza distanta parcursa
				if(distanta <= 280) {
					Transform2D::loadIdentityMatrix();
					Transform2D::translateMatrix(distanta,0);
					Transform2D::applyTransform(minicar);
				}
				else { //s-a ajuns la sfarsitul distantei de parcurs
					Text *end1 = new Text("CONGRATULATIONS!", Point2D(-100, 
										  -DrawingWindow::height/17), Color(0,1,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(end1, teren);
					Text *end2 = new Text("You have beaten", Point2D(-73, 
										  -DrawingWindow::height/11), Color(1,0.5,0), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(end2, teren);
					Text *end3 = new Text("- CRAZY DRIVER -", Point2D(-87, 
										  -DrawingWindow::height/8), Color(0,1,1), BITMAP_HELVETICA_18);
					DrawingWindow::addText_to_Visual2D(end3, teren);

					game_over = true;	//s-a terminat jocul
				}
				distanta += 0.025;
			}
		}
		//miscarea inamicilor
		move_inamici(inamic1);
		move_inamici(inamic2);
		move_inamici(inamic3);

		//miscarea bonusului
		move_bonus();

		//actualizarea timpului trecut
		int t = (clock()-startime)/1000;
		int minute = t/60;
		int sec = t%60;
		DrawingWindow::removeText_from_Visual2D(timp, titlu);
		int i;
		string m="", s="";
		for(i=0; i < 2-std::to_string(minute).size(); i++)
			m += "0";
		for(i=0; i < 2-std::to_string(sec).size(); i++)
			s += "0";
		timp = new Text(m+std::to_string(minute)+":"+s+std::to_string(sec), 
						Point2D(DrawingWindow::width*2.465/3, DrawingWindow::height/26), 
						Color(0,1,0), BITMAP_HELVETICA_18);
		DrawingWindow::addText_to_Visual2D(timp, titlu);
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
	}
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
	
}

//functia main
int main(int argc, char** argv)
{
	//setarea dimensiunilor ferestrei
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