//-------------------------------------------------------------------------------------------------
//
// TEMA 3: ATLETISM
//
// nume:		Bojin
// prenume:		Andrei Florian
// grupa/seria: 333 CA
//
//-------------------------------------------------------------------------------------------------

// incarcator de meshe
#include "lab_mesh_loader.hpp"

// geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"

// incarcator de shadere
#include "lab_shader_loader.hpp"

// interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"

// camera
#include "lab_camera.hpp"

// time
#include <ctime>

#include <iostream>
#include <fstream>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define PI 3.14159265358979323846

using namespace std;


class Laborator : public lab::glut::WindowListener{

// variabile
private:

	glm::mat4 original_model_matrix;			// matricea identitate
	// matrici 4x4 pt modelare vizualizare proiectie
	glm::mat4 model_matrix, view_matrix, projection_matrix;

	// locatii la nivel de shader a variabilelor asociate
	unsigned int location_model_matrix, location_view_matrix, location_projection_matrix;
	unsigned int location_light_position, location_eye_position;
	unsigned int location_material_shininess, location_material_kd, location_material_ks;
	unsigned int location_object_color;

	unsigned int gl_program_shader_gouraud;		// id shader gouraud
	unsigned int gl_program_shader_phong;		// id shader phong
	unsigned int gl_program_shader_steag;		// id shader steag
	unsigned int gl_program_shader_curent;		// id shader curent

	glm::vec3 light_position;					// pozitia luminii		
	glm::vec3 eye_position;						// pozitia observatorului

	// variabile ce tin de iluminare
	unsigned int material_shininess;
	float material_kd;
	float material_ks;

	unsigned int screen_width, screen_height;	// dimensiunile ecranului
	bool view;				// true daca se vrea impartirea viewportului in 4

	bool finish;	// true daca una dintre masini a ajuns la linia de finish
	bool lap;		// true daca jucatorul principal a parcurs o tura
	int winner;		// id-ul castigatorului

	unsigned int vbo, ibo;
	// variabile afiliate circuitului
	vector<unsigned int> vao_circuit, num_indices_circuit;
	int nr_objects_circuit;		// nr de obiecte din care este compus circuitul
	int nr_triang;				// nr de triunghiuri ale unui semicerc
	float bandlength;			// lungimea pistei drepte a circuitului
	float bandwidth;			// latimea pistei circuitului
	float h;		// diferenta de inaltime a partii interioare a circuitului
	float pash;		// cu cat se micsoreaza h fata de nivelul intregului circuit
	float outerband;			// latimea benzii albe exterioare
	float whiteband;			// latimea unei benzi albe interioare
	float greyband;				// latimea unei benzi gri a pistei
	float yellowband;			// latimea benzii galbene interioare
	float innerband;			// latimea benzii gri deschis interioare

	// variabile afiliate masinilor
	vector<unsigned int> vao_masini, num_indices_masini;
	vector<glm::vec4> color_masini;		// culoarea fiecarei masini
	vector<float> raza;			// distanta fata de origine a fiecarei masini
	vector<float> vit;			// viteza fiecarei masini
	vector<glm::vec3> pas;		// pasul fiecarei masini
	vector<float> rot;			// unghiul de rotatie fata de centrul propriu
	vector<glm::mat4> model_matrix_masini;	// matricea de modelare

	// variabile afiliate camerelor de vizualizare
	vector<lab::Camera> cam;	// camerele de vizualizare
	int curent_cam;				// id-ul camerei curente
	float rot_cam;				// unghiul de rotatie fata de centrul propriu
	float u_cam;				// unghiul de rotatie al camerei de filmat
	// camera de filmat
	vector<unsigned int> vao_camera, num_indices_camera;
	int nr_objects_camera;		// nr de obiecte care compun camera de filmat
	glm::mat4 model_matrix_cam, model_matrix_camera;	// matricile

	// variabile afiliate steagului
	unsigned int vao_objsteag, num_indices_objsteag;	// obiectul obj steag
	glm::mat4 model_matrix_objsteag;	// matricea de modelare a acestuia
	unsigned int vao_steag, num_indices_steag;	// obiectul creat steag
	glm::mat4 model_matrix_steag, view_matrix_steag;	// matricile
	float offset;	// variabila care controleaza viteza/gradul de fluturare
	unsigned int vao_stick, num_indices_stick;	// obiectul flagpole (batz :p)


// metode
public:

	// constructor .. e apelat cand e instantiata clasa
	Laborator(){
		// setari pentru desenare, clear color seteaza culoarea de clear pentru
		// ecran (format R,G,B,A)
		glClearColor(0,0.7,0.7,1);	// culoarea cerului
		glClearDepth(1);			// clear depth si depth test (nu le studiem
		// momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	// sunt folosite pentru a determina obiectele
		//cele mai apropiate de camera (la curs: algoritmul pictorului, 
		// algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ
		// la programul creat
		gl_program_shader_gouraud = lab::loadShader("shadere\\shader_gouraud_vertex.glsl", 
												"shadere\\shader_gouraud_fragment.glsl");
		gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", 
													"shadere\\shader_phong_fragment.glsl");
		gl_program_shader_steag = lab::loadShader("shadere\\shader_vertex_steag.glsl", 
													"shadere\\shader_fragment_steag.glsl");
		gl_program_shader_curent = gl_program_shader_gouraud;

		// gasirea locatiilor variabilelor din shader
		location_model_matrix = glGetUniformLocation(gl_program_shader_curent, "model_matrix");
		location_view_matrix = glGetUniformLocation(gl_program_shader_curent, "view_matrix");
		location_projection_matrix = glGetUniformLocation(gl_program_shader_curent, "projection_matrix");
		location_light_position = glGetUniformLocation(gl_program_shader_curent, "light_position");
		location_eye_position = glGetUniformLocation(gl_program_shader_curent, "eye_position");
		location_material_shininess = glGetUniformLocation(gl_program_shader_curent, "material_shininess");
		location_material_kd = glGetUniformLocation(gl_program_shader_curent, "material_kd");
		location_material_ks = glGetUniformLocation(gl_program_shader_curent, "material_ks");
		location_object_color = glGetUniformLocation(gl_program_shader_curent, "object_color");

		// matricea originala de modelare este matricea identitate
		original_model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

		// variabile specifice luminii & materialului
		eye_position = glm::vec3(0,50,70);
		light_position = glm::vec3(100,30,0);
		material_shininess = 100;
		material_kd = 0.5;
		material_ks = 0.5;

		// setarea parametrilor circuitului
		nr_triang = 30;
		bandlength = 60;
		bandwidth = 40;
		h = -3;
		pash = 0.001;
		outerband = 1;
		whiteband = 0.2;
		greyband = 3;
		yellowband = 0.3;
		innerband = 6;

		// initializarea celor 6 camere de vizualizare
		for(int i=0; i<6; i++)
			cam.push_back(lab::Camera());

		curent_cam = 1;	// camera de vizualizare curenta este a masinii 2(verzi)

		view = false;		// viewportul initial nu este impartit in 4
		finish = false;		// cursa nu s-a sfarsit inca
		winner = 0;			// initializarea id-ului castigatorului
		offset = 0;			// initializarea offsetului steagului

		// incarcarea si crearea obiectelor
		createCircuit();
		createMasini();
		createCamera();
		createObjSteag();
		createSteag(100,60,2);
		createStick(8,180);

		lab::glut::enterFullscreen();		// FullScreen implicit 
		glutSetCursor(GLUT_CURSOR_NONE);	// ascunderea cursorului mouseului

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// desenare wireframe

	}

	// destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		// distruge shadere
		glDeleteProgram(gl_program_shader_gouraud);
		glDeleteProgram(gl_program_shader_phong);
		glDeleteProgram(gl_program_shader_steag);

		// distruge mesh incarcat
		glDeleteBuffers(1,&vbo);
		glDeleteBuffers(1,&ibo);
		int i;
		// distruge circuitul
		for(i=0; i<vao_circuit.size(); i++)
			glDeleteVertexArrays(1,&vao_circuit[i]);
		// distruge masinile
		for(i=0; i<vao_masini.size(); i++)
			glDeleteVertexArrays(1,&vao_masini[i]);
		// distruge camera de filmat
		for(i=0; i<vao_camera.size(); i++)
			glDeleteVertexArrays(1,&vao_camera[i]);
	}

	// structura interna a unui punct
	struct myVerticesStruct {
		glm::vec3 pos;				// pozitia punctului
		glm::vec3 color;			// culoarea punctului
	};

	// functie de creare a unui semicerc
	void createSemiCerc(int nr_triang, float x_center, float y_center, 
						float z_center, float h, float radius, 
						glm::vec3 color, float offset) {

		vector<myVerticesStruct> vertices;
		vector<glm::uvec3> indices;
		myVerticesStruct point;

		// centrul semicercului
		point.pos = glm::vec3(x_center, y_center+h, z_center);
		point.color = color;
		vertices.push_back(point);

		// crearea punctelor de pe conturul semicercului
		for (float u = offset; u <= offset+PI+0.1; u += (float)PI/nr_triang)
		{
			point.pos = glm::vec3(x_center+radius * cos(u), y_center, z_center+radius * sin(u));
			point.color = color;
			vertices.push_back(point);
		}
		// crearea fetelor
		for(int i = 1; i <= nr_triang; i++)
			indices.push_back(glm::uvec3(0,i,i+1));

		// adaugarea la vao-ul circuitului
		nr_objects_circuit ++;
		vao_circuit.push_back(nr_objects_circuit);

		// creeare VAO
		glGenVertexArrays(1, &vao_circuit[vao_circuit.size()-1]);
		// bind VAO
		glBindVertexArray(vao_circuit[vao_circuit.size()-1]);

		// creez VBO
		glDeleteBuffers(1, &vbo);
		glGenBuffers(1, &vbo);
		// bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(myVerticesStruct)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// creez IBO
		glDeleteBuffers(1, &ibo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(glm::uvec3)*indices.size(),&indices[0], GL_STATIC_DRAW);

		// legare atribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)0);
	
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)sizeof(glm::vec3));

		num_indices_circuit.push_back(nr_triang * 3);

	}

	// functie de creare a unui semicerc intrerupt(pentru benzile albe intrerupte)
	void createSemiCercInt(int nr_triang, float x_center, float y_center,
						float z_center, float h, float radius,
						glm::vec3 color, float offset) {

		vector<myVerticesStruct> vertices;
		vector<glm::uvec3> indices;
		myVerticesStruct point;

		// centrul semicercului
		point.pos = glm::vec3(x_center, y_center+h, z_center);
		point.color = color;
		vertices.push_back(point);

		// crearea punctelor de pe conturul semicercului
		for (float u = offset; u <= offset+PI+0.1; u += (float)PI/nr_triang)
		{
			point.pos = glm::vec3(x_center+radius * cos(u), y_center, z_center+radius * sin(u));
			point.color = color;
			vertices.push_back(point);
		}
		// crearea fetelor
		for(int i = 1; i <= nr_triang; i+=2)
			indices.push_back(glm::uvec3(0,i,i+1));

		// adaugarea la vao-ul circuitului
		nr_objects_circuit ++;
		vao_circuit.push_back(nr_objects_circuit);

		// creeare VAO
		glGenVertexArrays(1, &vao_circuit[vao_circuit.size()-1]);
		// bind VAO
		glBindVertexArray(vao_circuit[vao_circuit.size()-1]);

		// creez VBO
		glDeleteBuffers(1, &vbo);
		glGenBuffers(1, &vbo);
		// bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(myVerticesStruct)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// creez IBO
		glDeleteBuffers(1, &ibo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(glm::uvec3)*indices.size(),&indices[0], GL_STATIC_DRAW);

		// legare atribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)0);
	
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)sizeof(glm::vec3));

		num_indices_circuit.push_back(nr_triang * 3);

	}

	// functie de creare a unui dreptunghi impartit in doua
	void createDreptunghi(float x_center, float y_center, float z_center,
						float h, float lungime, float latime, glm::vec3 color) {

		vector<myVerticesStruct> vertices;
		vector<glm::uvec3> indices;
		myVerticesStruct point;

		// crearea punctelor
		point.pos = glm::vec3(x_center-latime/2, y_center, z_center-lungime/2);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center-latime/2, y_center, z_center+lungime/2);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center, y_center+h, z_center+lungime/2);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center+latime/2, y_center, z_center+lungime/2);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center+latime/2, y_center, z_center-lungime/2);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center, y_center+h, z_center-lungime/2);
		point.color = color;
		vertices.push_back(point);

		// crearea fetelor
		indices.push_back(glm::uvec3(0,1,2));
		indices.push_back(glm::uvec3(2,5,0));
		indices.push_back(glm::uvec3(5,2,3));
		indices.push_back(glm::uvec3(3,4,5));

		// adaugarea la vao-ul circuitului
		nr_objects_circuit ++;
		vao_circuit.push_back(nr_objects_circuit);

		// creeare VAO
		glGenVertexArrays(1, &vao_circuit[vao_circuit.size()-1]);
		// bind VAO
		glBindVertexArray(vao_circuit[vao_circuit.size()-1]);

		// creez VBO
		glDeleteBuffers(1, &vbo);
		glGenBuffers(1, &vbo);
		// bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(myVerticesStruct)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// creez IBO
		glDeleteBuffers(1, &ibo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(glm::uvec3)*indices.size(),&indices[0], GL_STATIC_DRAW);

		// legare atribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)0);
	
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)sizeof(glm::vec3));

		num_indices_circuit.push_back(4 * 3);

	}

	// functie de creare a circuitului cu functiile de mai sus
	void createCircuit() {
		// setarea camerei de vizualizare de deasupra stadionului
		cam[5].set(glm::vec3(0,100,0), glm::vec3(0,0,0), glm::vec3(0,0,-1));

		nr_objects_circuit = 0;
		// culori
		glm::vec3 white = glm::vec3(0.7, 0.7, 0.7);
		glm::vec3 darkgrey = glm::vec3(0.1, 0.1, 0.1);
		glm::vec3 lightgrey = glm::vec3(0.3, 0.3, 0.3);
		glm::vec3 yellow = glm::vec3(0.7, 0.7, 0);
		glm::vec3 green = glm::vec3(0, 0.5, 0);

		// suprafata verde pe care se afla circuitul
		createDreptunghi(0, 0, 0, h, 1000, 1000, green);

		// banda exterioara alba
		createSemiCerc(nr_triang, 0, 0, bandlength/2, h, bandwidth, white, 0);
		createDreptunghi(0, 0, 0, h, bandlength, 2*bandwidth, white);
		createSemiCerc(nr_triang, 0, 0, -(bandlength/2), h, bandwidth,  white, PI);

		int i;
		float j = 0.1;
		// crearea a 3 secvente de banda gri, banda alba
		for(i=1; i<=3; i++, j+=pash) {
			// banda gri-inchis
			createSemiCerc(nr_triang, 0, 0, bandlength/2, h	+ j, bandwidth - outerband - greyband*(i-1) - whiteband*(i-1), darkgrey, 0);
			createDreptunghi(0, 0, 0, h	+ j, bandlength, 2*(bandwidth - outerband - greyband*(i-1) - whiteband*(i-1)), darkgrey);
			createSemiCerc(nr_triang, 0, 0, -(bandlength/2), h + j, bandwidth - outerband - greyband*(i-1) - whiteband*(i-1),  darkgrey, PI);

			// banda alba
			createSemiCercInt(nr_triang, 0, 0, bandlength/2, h + j, bandwidth - outerband - greyband*i - whiteband*(i-1), white, 0);
			createDreptunghi(0, 0, 0, h + j, bandlength, 2*(bandwidth - outerband - greyband*i - whiteband*(i-1)), white);
			createSemiCercInt(nr_triang, 0, 0, -(bandlength/2), h + j, bandwidth - outerband - greyband*i - whiteband*(i-1),  white, PI);
		}

		// banda gri-inchis
		createSemiCerc(nr_triang, 0, 0, bandlength/2, h + j, bandwidth - outerband - greyband*3 - whiteband*3, darkgrey, 0);
		createDreptunghi(0, 0, 0, h + j, bandlength, 2*(bandwidth - outerband - greyband*3 - whiteband*3), darkgrey);
		createSemiCerc(nr_triang, 0, 0, -(bandlength/2), h + j, bandwidth - outerband - greyband*3 - whiteband*3,  darkgrey, PI);

		j += pash;
		// banda galbena
		createSemiCerc(nr_triang, 0, 0, bandlength/2, h + j, bandwidth - outerband - greyband*4 - whiteband*3, yellow, 0);
		createDreptunghi(0, 0, 0, h + j, bandlength, 2*(bandwidth - outerband - greyband*4 - whiteband*3), yellow);
		createSemiCerc(nr_triang, 0, 0, -(bandlength/2), h + j, bandwidth - outerband - greyband*4 - whiteband*3,  yellow, PI);

		j += pash;
		// banda gri-deschis
		createSemiCerc(nr_triang, 0, 0, bandlength/2, h + j, bandwidth - outerband - greyband*4 - whiteband*3 - yellowband, lightgrey, 0);
		createDreptunghi(0, 0, 0, h + j, bandlength, 2*(bandwidth - outerband - greyband*4 - whiteband*3 - yellowband), lightgrey);
		createSemiCerc(nr_triang, 0, 0, -(bandlength/2), h + j, bandwidth - outerband - greyband*4 - whiteband*3 - yellowband,  lightgrey, PI);

		j += pash;
		// banda alba
		createSemiCerc(nr_triang, 0, 0, bandlength/2, h + j, bandwidth - outerband - greyband*4 - whiteband*3 - yellowband - innerband, white, 0);
		createDreptunghi(0, 0, 0, h + j, bandlength, 2*(bandwidth - outerband - greyband*4 - whiteband*3 - yellowband - innerband), white);
		createSemiCerc(nr_triang, 0, 0, -(bandlength/2), h + j, bandwidth - outerband - greyband*4 - whiteband*3 - yellowband - innerband,  white, PI);

		// teren verde
		createSemiCerc(nr_triang, 0, 0, bandlength/2, 0, bandwidth - outerband*2 - greyband*4 - whiteband*3 - yellowband - innerband, green, 0);
		createDreptunghi(0, 0, 0, 0, bandlength, 2*(bandwidth - outerband*2 - greyband*4 - whiteband*3 - yellowband - innerband), green);
		createSemiCerc(nr_triang, 0, 0, -(bandlength/2), 0, bandwidth - outerband*2 - greyband*4 - whiteband*3 - yellowband - innerband,  green, PI);

		// linie start/finish
		createDreptunghi(bandwidth*5/4 - outerband*2 - greyband*4 - whiteband*3 - yellowband - innerband, 0, -2.5, 0, 0.5, bandwidth/2, white);

	}

	// functie de loadare a unui obj masina si setarea parametrilor acesteia
	void createMasini(){

		//incarca vertecsii si indecsii din fisier
		std::vector<lab::VertexFormat> vertices;
		std::vector<unsigned int> indices;
		lab::_loadObjFile("resurse\\Porsche_911_GT2.obj", vertices, indices);

		std::cout<<"Mesh Loader : am incarcat fisierul "<< "resurse\\Porsche_911_GT2.obj" <<std::endl;

		//creeaza obiectele OpenGL necesare desenarii
		unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

		//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
		glGenVertexArrays(1, &gl_vertex_array_object);
		glBindVertexArray(gl_vertex_array_object);

		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&gl_vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(lab::VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&gl_index_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

		ifstream in ("resurse\\viteze.txt");   // citirea din fisier a vitezelor
		string vit_tmp;
		// crearea celor 4 masini, fiecare cu parametrii ei
		for(int i=0; i<4; i++) {
			vao_masini.push_back(i);
			num_indices_masini.push_back(i);

			vao_masini[i] = gl_vertex_array_object;
			vbo = gl_vertex_buffer_object;
			ibo = gl_index_buffer_object;
			num_indices_masini[i] = indices.size();
			// parametrii
			raza.push_back(bandwidth - outerband - (3-i)*greyband - (3-i)*whiteband - greyband/2);
			getline(in, vit_tmp);
			vit.push_back(::atof(vit_tmp.c_str()));
			pas.push_back(glm::vec3(raza[i],0.5,0));
			rot.push_back(0);
			model_matrix_masini.push_back(original_model_matrix);
			// setarea camerelor de vizualizare din perspectiva soferilor
			if(i == 2)
				cam[i].set(glm::vec3(pas[i][0],pas[i][1]+0.5,pas[i][2]-0.5), glm::vec3(pas[i][0],pas[i][1]+0.5,pas[i][2]-1), glm::vec3(0,1,0));
			else
				cam[i].set(glm::vec3(pas[i][0],pas[i][1]+0.5,pas[i][2]), glm::vec3(0,pas[i][1]+0.5,0), glm::vec3(0,1,0));
		}
		// culorile masinilor
		color_masini.push_back(glm::vec4(0.5,0,0.5,0));
		color_masini.push_back(glm::vec4(0,0.5,0,0));
		color_masini.push_back(glm::vec4(0.5,0,0,0));
		color_masini.push_back(glm::vec4(0,0,0.5,0));
		// inchidere fisierului de citire
		in.close();

	}

	// functie de creare a suportului camerei de filmat
	void createSuport(float x_center, float y_center, float z_center, glm::vec3 color){

		vector<myVerticesStruct> vertices;
		vector<glm::uvec3> indices;
		myVerticesStruct point;

		// crearea punctelor
		point.pos = glm::vec3(x_center-outerband/2, y_center, z_center-0.5);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center-outerband/2, y_center, z_center+0.5);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center+outerband/2, y_center, z_center+0.5);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center+outerband/2, y_center, z_center-0.5);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center+outerband*3.5-outerband/2, y_center+outerband*4, z_center-0.25);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center+outerband*3.5-outerband/2, y_center+outerband*4, z_center+0.25);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center+outerband*3.5-outerband/4, y_center+outerband*4, z_center+0.25);
		point.color = color;
		vertices.push_back(point);

		point.pos = glm::vec3(x_center+outerband*3.5-outerband/4, y_center+outerband*4, z_center-0.25);
		point.color = color;
		vertices.push_back(point);

		// crearea fetelor
		indices.push_back(glm::uvec3(0,1,2));
		indices.push_back(glm::uvec3(2,3,0));
		indices.push_back(glm::uvec3(4,5,6));
		indices.push_back(glm::uvec3(6,7,4));
		indices.push_back(glm::uvec3(0,1,5));
		indices.push_back(glm::uvec3(5,4,0));
		indices.push_back(glm::uvec3(1,2,6));
		indices.push_back(glm::uvec3(6,5,1));
		indices.push_back(glm::uvec3(2,3,7));
		indices.push_back(glm::uvec3(7,6,2));
		indices.push_back(glm::uvec3(3,0,4));
		indices.push_back(glm::uvec3(4,7,3));

		// adaugarea acestora la vao-ul camerei de filmat
		nr_objects_camera ++;
		vao_camera.push_back(nr_objects_camera);

		// creeare VAO
		glGenVertexArrays(1, &vao_camera[vao_camera.size()-1]);
		// bind VAO
		glBindVertexArray(vao_camera[vao_camera.size()-1]);

		// creez VBO
		glDeleteBuffers(1, &vbo);
		glGenBuffers(1, &vbo);
		// bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(myVerticesStruct)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// creez IBO
		glDeleteBuffers(1, &ibo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(glm::uvec3)*indices.size(),&indices[0], GL_STATIC_DRAW);

		// legare atribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)0);
	
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)sizeof(glm::vec3));

		num_indices_camera.push_back(12 * 3);

	}

	// functie de creare a camerei de filmat
	void createCamera(){

		nr_objects_camera = 0;
		// crearea suportului camerei de filmat
		createSuport(bandwidth - outerband/2,0,0,glm::vec3(0,0,0.1));
		nr_objects_camera ++;
		// adaugarea la vao-ul camerei de filmat
		vao_camera.push_back(nr_objects_camera);
		num_indices_camera.push_back(nr_objects_camera);
		// loadarea obj-ului camera de filmat
		lab::loadObj("resurse\\cameraTV.obj", vao_camera[nr_objects_camera-1],
					 vbo, ibo, num_indices_camera[nr_objects_camera-1]);
		// setarea camerei de vizualizare FPS a camerei de filmat
		cam[4].set(glm::vec3(bandwidth+outerband*3-outerband/2+0.2,
					outerband*3.9+0.7,0), glm::vec3(0,outerband*3.9+0.7,0),
					glm::vec3(0,1,0));
		float d = bandwidth+outerband*3-outerband/2+0.2 - pas[2][0];
		u_cam = atan((outerband*3.9+0.7-pas[2][1])/d);
		cam[4].rotateFPSoX(-u_cam);
		cam[4].rotateTPSoY(-0.01,((bandwidth+outerband*3-outerband/2+0.2-raza[2])*(bandwidth+outerband*3-outerband/2+0.2-raza[2]) + 
									(outerband*3.9+0.7-pas[2][1])*(outerband*3.9+0.7-pas[2][1])));
		// potrivirea obj-ului camera de filmat
		rot_cam = 0;
		model_matrix_camera = original_model_matrix;
		model_matrix_cam = glm::translate(model_matrix_camera, glm::vec3(bandwidth+outerband*3-outerband/2,outerband*3.9,0));
		model_matrix_cam = glm::rotate(model_matrix_cam,-90.0f+u_cam*(float)(180/PI),glm::vec3(0,0,1));
		model_matrix_cam = glm::rotate(model_matrix_cam,-90.0f,glm::vec3(0,1,0));
		model_matrix_cam = glm::scale(model_matrix_cam,glm::vec3(0.5,0.5,0.5));

	}

	// functie de creare a obj-ului steag
	void createObjSteag(){

		lab::loadObj("resurse\\flag.obj", vao_objsteag, vbo, ibo, num_indices_objsteag);
		model_matrix_objsteag = glm::rotate(original_model_matrix,90.0f,glm::vec3(0,1,0));
		model_matrix_objsteag = glm::scale(model_matrix_objsteag,glm::vec3(0.05,0.05,0.05));

	}

	// functie de creare a meshei steagului care flutura
	void createSteag(float latime, float inaltime, float unitate) {

		vector<myVerticesStruct> vertices;
		vector<glm::uvec4> indices;
		myVerticesStruct point;
		int x, y;
		// crearea punctelor
		for(y=0; y<=inaltime; y+=unitate) {
			for(x=0; x<=latime; x+=unitate) {

				point.pos = glm::vec3(x, y, 0);
				point.color = glm::vec3(0, 0, 0);
				vertices.push_back(point);
			}
		}
		int i, j;
		// crearea fetelor
		for(i=0; i<inaltime/unitate; i++) {
			for(j=0; j<latime/unitate; j++) {
	
				indices.push_back(glm::uvec4(j+i*(latime/unitate+1), j+i*(latime/unitate+1)+1, 
											 j+(i+1)*(latime/unitate+1)+1, j+(i+1)*(latime/unitate+1)));
			}
		}

		// creeare VAO
		glGenVertexArrays(1, &vao_steag);
		// bind VAO
		glBindVertexArray(vao_steag);

		// creez VBO
		glDeleteBuffers(1, &vbo);
		glGenBuffers(1, &vbo);
		// bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(myVerticesStruct)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// creez IBO
		glDeleteBuffers(1, &ibo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(glm::uvec4)*indices.size(),&indices[0], GL_STATIC_DRAW);

		// legare atribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)0);
	
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)sizeof(glm::vec3));

		num_indices_steag = inaltime/unitate * latime/unitate * 4;

	}

	// functie de creare a suportului steagului
	void createStick(float latime, float inaltime) {

		vector<myVerticesStruct> vertices;
		vector<glm::uvec4> indices;
		myVerticesStruct point;
		// crearea punctelor
		point.pos = glm::vec3(-latime, inaltime/3, 0);
		point.color = glm::vec3(0, 0, 0);
		vertices.push_back(point);

		point.pos = glm::vec3(-latime, -inaltime*2/3, 0);
		point.color = glm::vec3(0, 0, 0);
		vertices.push_back(point);

		point.pos = glm::vec3(0, -inaltime*2/3, 0);
		point.color = glm::vec3(0, 0, 0);
		vertices.push_back(point);

		point.pos = glm::vec3(0, inaltime/3, 0);
		point.color = glm::vec3(0, 0, 0);
		vertices.push_back(point);
		// crearea fetelor
		indices.push_back(glm::uvec4(0,1,2,3));

		// creeare VAO
		glGenVertexArrays(1, &vao_stick);
		// bind VAO
		glBindVertexArray(vao_stick);

		// creez VBO
		glDeleteBuffers(1, &vbo);
		glGenBuffers(1, &vbo);
		// bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(myVerticesStruct)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// creez IBO
		glDeleteBuffers(1, &ibo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(glm::uvec4)*indices.size(),&indices[0], GL_STATIC_DRAW);

		// legare atribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)0);
	
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(myVerticesStruct),(void*)sizeof(glm::vec3));

		num_indices_stick = 1 * 4;

	}

	// functie de detectare a apartenentei masinii la interiorul circuitului
	bool interior(float x, float z){
		// daca masina este pe banda dreapta
		if(z >= -(bandlength/2) && z <= bandlength/2) {
			if((x-1 <= bandwidth - outerband - greyband*4 - whiteband*3 && 
				x+1 >= -(bandwidth - outerband - greyband*4 - whiteband*3)) ||
				x+1 >= bandwidth - outerband || x-1 <= -(bandwidth - outerband))

				return false;
		}
		else {	// daca masina este in curba
			float d;
			if(z < -(bandlength/2))	// curba de jos
				d = sqrt( (x-0)*(x-0)+(z+bandlength/2)*(z+bandlength/2) );
			else	// curba de sus
				d = sqrt( (x-0)*(x-0)+(z-bandlength/2)*(z-bandlength/2) );
			if(d-1 <= bandwidth - outerband - greyband*4 - whiteband*3 || d+1 >= bandwidth - outerband)
				return false;
		}
		return true;
	}

	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa 
	// updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){

		// miscarea tuturor obiectelor 
		float u, d;

		// miscarea fiecarei masini mai putin cea a jucatorului principal
		for(int i=0; i<vao_masini.size() && finish == false; i++){
			if(i != 2){
				// miscarea pe banda dreapta dreapta
				if(pas[i][2] > -(bandlength/2) && pas[i][0] == raza[i]){
					if(pas[i][2] - vit[i] >= -(bandlength/2))
						pas[i][2] -= vit[i];
					else {
						pas[i][2] = -(bandlength/2);
						rot[i] = 0;
					}
					model_matrix_masini[i] = glm::translate(original_model_matrix, pas[i]);
					// setarea camerei de vizualizare FPS
					cam[i].set(glm::vec3(raza[i],pas[i][1]+0.5,0), glm::vec3(0,pas[i][1]+0.5,0), glm::vec3(0,1,0));
					cam[i].translateRight(-pas[i][2]);
					cam[i].rotateFPSoY(PI/2);
					cam[i].translateForward(0.5);
				}
				// miscarea pe curba de sus
				if(pas[i][2] == -(bandlength/2) && rot[i] < 180){
					model_matrix_masini[i] = glm::translate(original_model_matrix, pas[i]);
					model_matrix_masini[i] = glm::translate(model_matrix_masini[i], glm::vec3(-raza[i],0,0));
					if(180 - rot[i] >= vit[i]/raza[i]*(float)(180/PI))
						rot[i] += vit[i]/raza[i]*(float)(180/PI);
					else {
						rot[i] = 180;
						pas[i][0] = -raza[i];
					}
					model_matrix_masini[i] = glm::rotate(model_matrix_masini[i],rot[i],glm::vec3(0,1,0));			
					model_matrix_masini[i] = glm::translate(model_matrix_masini[i], glm::vec3(raza[i],0,0));
					// setarea camerei de vizualizare FPS
					cam[i].set(glm::vec3(raza[i],pas[i][1]+0.5,-(bandlength/2)), glm::vec3(0,pas[i][1]+0.5,-(bandlength/2)), glm::vec3(0,1,0));
					cam[i].rotateTPSoY(-rot[i]*(float)(PI/180),raza[i]);
					cam[i].rotateFPSoY(PI/2);
					cam[i].translateForward(0.5);
				}
				// miscarea pe banda dreapta stanga
				if(pas[i][2] < bandlength/2 && pas[i][0] == -raza[i]){
					if(pas[i][2] + vit[i] <= bandlength/2)
						pas[i][2] += vit[i];
					else {
						pas[i][2] = bandlength/2;
						rot[i] = 0;
					}
					model_matrix_masini[i] = glm::translate(original_model_matrix, pas[i]);
					model_matrix_masini[i] = glm::rotate(model_matrix_masini[i],180.0f,glm::vec3(0,1,0));
					// setarea camerei de vizualizare FPS
					cam[i].set(glm::vec3(-raza[i],pas[i][1]+0.5,0), glm::vec3(0,pas[i][1]+0.5,0), glm::vec3(0,1,0));
					cam[i].translateRight(pas[i][2]);
					cam[i].rotateFPSoY(PI/2);
					cam[i].translateForward(0.5);
				}
				//miscarea pe curba de jos
				if(pas[i][2] == bandlength/2 && rot[i] < 180){
					model_matrix_masini[i] = glm::translate(original_model_matrix, pas[i]);
					model_matrix_masini[i] = glm::rotate(model_matrix_masini[i],180.0f,glm::vec3(0,1,0));
					model_matrix_masini[i] = glm::translate(model_matrix_masini[i], glm::vec3(-raza[i],0,0));
					if(180 - rot[i] >= vit[i]/raza[i]*(float)(180/PI))
						rot[i] += vit[i]/raza[i]*(float)(180/PI);
					else {
						rot[i] = 180;
						pas[i][0] = raza[i];
					}
					model_matrix_masini[i] = glm::rotate(model_matrix_masini[i],rot[i],glm::vec3(0,1,0));				
					model_matrix_masini[i] = glm::translate(model_matrix_masini[i], glm::vec3(raza[i],0,0));
					// setarea camerei de vizualizare FPS
					cam[i].set(glm::vec3(-raza[i],pas[i][1]+0.5,bandlength/2), glm::vec3(0,pas[i][1]+0.5,bandlength/2), glm::vec3(0,1,0));
					cam[i].rotateTPSoY(-rot[i]*(float)(PI/180),raza[i]);
					cam[i].rotateFPSoY(PI/2);
					cam[i].translateForward(0.5);
				}
				// daca una dintre masini a ajuns la linia de finish, jocul se incheie si se retine castigatorul
				if(floor(pas[i][2]) == 0 && pas[i][0] > 0) {
					finish = true;
					winner = i;
				}
			}
			else{	// miscarea jucatorului principal
				// daca tasta UP a fost apasata si masina este in interior
				if(lab::glut::keySpecialStates[GLUT_KEY_UP] == true && 
					interior(pas[i][0]-vit[i]*sin(rot[i]*(float)(PI/180)), 
					pas[i][2]-vit[i]*cos(rot[i]*(float)(PI/180))) == true){

					pas[i][2] -= vit[i]*cos(rot[i]*(float)(PI/180));
					pas[i][0] -= vit[i]*sin(rot[i]*(float)(PI/180));
					// setarea camerei de vizualizare FPS
					cam[i].translateForward(vit[i]);
					// setarea camerei de vizualizare a camerei de filmat
					// banda dreapta
					if(pas[i][2] < bandlength/2 && pas[i][2] > -(bandlength/2)) {
						if(pas[i][0] > 0) {		// dreapta
							cam[4].set(glm::vec3(bandwidth+outerband*3-outerband/2+0.2,outerband*3.9+0.7,0), glm::vec3(0,outerband*3.9+0.7,0), glm::vec3(0,1,0));
							cam[4].translateRight(-pas[i][2]);
							d = bandwidth+outerband*3-outerband/2+0.2 - pas[i][0];
						}
						else {	// stanga
							cam[4].set(glm::vec3(-(bandwidth+outerband*3-outerband/2+0.2),outerband*3.9+0.7,0), glm::vec3(0,outerband*3.9+0.7,0), glm::vec3(0,1,0));
							cam[4].translateRight(pas[i][2]);
							d = pas[i][0] + bandwidth+outerband*3-outerband/2+0.2;
						}
					}
					// curba sus
					if(pas[i][2] < -(bandlength/2)) {
						cam[4].set(glm::vec3(bandwidth+outerband*3-outerband/2+0.2,outerband*3.9+0.7,-(bandlength/2)), glm::vec3(0,outerband*3.9+0.7,-(bandlength/2)), glm::vec3(0,1,0));
						if(pas[i][0] > 0) {		// dreapta
							u = atan((-(bandlength/2)-pas[i][2])/pas[i][0]);
							d = bandwidth+outerband*3-outerband/2+0.2 - pas[i][0]/cos(u);
						}
						else {	// stanga
							u = PI - atan((-(bandlength/2)-pas[i][2])/-pas[i][0]);
							d = bandwidth+outerband*3-outerband/2+0.2 + pas[i][0]/cos(PI-u);
						}
						cam[4].rotateTPSoY(-u,bandwidth+outerband*3-outerband/2+0.2);
					}
					// curba jos
					if(pas[i][2] > bandlength/2) {
						cam[4].set(glm::vec3(-(bandwidth+outerband*3-outerband/2+0.2),outerband*3.9+0.7,bandlength/2), glm::vec3(0,outerband*3.9+0.7,bandlength/2), glm::vec3(0,1,0));
						if(pas[i][0] < 0) {		// stanga
							u = atan((pas[i][2]-bandlength/2)/-pas[i][0]);
							d = bandwidth+outerband*3-outerband/2+0.2 + pas[i][0]/cos(u);
						}
						else {	// dreapta
							u = PI - atan((pas[i][2]-bandlength/2)/pas[i][0]);
							d = bandwidth+outerband*3-outerband/2+0.2 - pas[i][0]/cos(PI-u);
							lap = true;
						}
						cam[4].rotateTPSoY(-u,bandwidth+outerband*3-outerband/2+0.2);
					}
					u_cam = atan((outerband*3.9+0.7-pas[i][1])/d);
					cam[4].rotateFPSoX(-u_cam);
					cam[4].rotateTPSoY(-0.01,((bandwidth+outerband*3-outerband/2+0.2-raza[i])*(bandwidth+outerband*3-outerband/2+0.2-raza[i]) + 
												(outerband*3.9+0.7-pas[i][1])*(outerband*3.9+0.7-pas[i][1])));
				}
				// daca tasta DOWN a fost apasata si masina este in interior
				if(lab::glut::keySpecialStates[GLUT_KEY_DOWN] == true && 
					interior(pas[i][0]+vit[i]*sin(rot[i]*(float)(PI/180)), 
					pas[i][2]+vit[i]*cos(rot[i]*(float)(PI/180))) == true){

					pas[i][2] += vit[i]*cos(rot[i]*(float)(PI/180));
					pas[i][0] += vit[i]*sin(rot[i]*(float)(PI/180));
					// setarea camerei de vizualizare FPS
					cam[i].translateForward(-vit[i]);
					// setarea camerei de vizualizare a camerei de filmat
					// banda dreapta
					if(pas[i][2] < bandlength/2 && pas[i][2] > -(bandlength/2)) {
						if(pas[i][0] > 0) {		// dreapta
							cam[4].set(glm::vec3(bandwidth+outerband*3-outerband/2+0.2,outerband*3.9+0.7,0), glm::vec3(0,outerband*3.9+0.7,0), glm::vec3(0,1,0));
							cam[4].translateRight(-pas[i][2]);
							d = bandwidth+outerband*3-outerband/2+0.2 - pas[i][0];
						}
						else {	// stanga
							cam[4].set(glm::vec3(-(bandwidth+outerband*3-outerband/2+0.2),outerband*3.9+0.7,0), glm::vec3(0,outerband*3.9+0.7,0), glm::vec3(0,1,0));
							cam[4].translateRight(pas[i][2]);
							d = pas[i][0] + bandwidth+outerband*3-outerband/2+0.2;
						}
					}
					// curba de sus
					if(pas[i][2] < -(bandlength/2)) {
						cam[4].set(glm::vec3(bandwidth+outerband*3-outerband/2+0.2,outerband*3.9+0.7,-(bandlength/2)), glm::vec3(0,outerband*3.9+0.7,-(bandlength/2)), glm::vec3(0,1,0));
						if(pas[i][0] > 0) {		// dreapta
							u = atan((-(bandlength/2)-pas[i][2])/pas[i][0]);
							d = bandwidth+outerband*3-outerband/2+0.2 - pas[i][0]/cos(u);
						}
						else {	// stanga
							u = PI - atan((-(bandlength/2)-pas[i][2])/-pas[i][0]);
							d = bandwidth+outerband*3-outerband/2+0.2 + pas[i][0]/cos(PI-u);
						}
						cam[4].rotateTPSoY(-u,bandwidth+outerband*3-outerband/2+0.2);
					}
					// curba de jos
					if(pas[i][2] > bandlength/2) {
						cam[4].set(glm::vec3(-(bandwidth+outerband*3-outerband/2+0.2),outerband*3.9+0.7,bandlength/2), glm::vec3(0,outerband*3.9+0.7,bandlength/2), glm::vec3(0,1,0));
						if(pas[i][0] < 0) {		// stanga
							u = atan((pas[i][2]-bandlength/2)/-pas[i][0]);
							d = bandwidth+outerband*3-outerband/2+0.2 + pas[i][0]/cos(u);
						}
						else {	// dreapta
							u = PI - atan((pas[i][2]-bandlength/2)/pas[i][0]);
							d = bandwidth+outerband*3-outerband/2+0.2 - pas[i][0]/cos(PI-u);
						}
						cam[4].rotateTPSoY(-u,bandwidth+outerband*3-outerband/2+0.2);
					}
					u_cam = atan((outerband*3.9+0.7-pas[i][1])/d);
					cam[4].rotateFPSoX(-u_cam);
					cam[4].rotateTPSoY(-0.01,((bandwidth+outerband*3-outerband/2+0.2-raza[i])*(bandwidth+outerband*3-outerband/2+0.2-raza[i]) + 
												(outerband*3.9+0.7-pas[i][1])*(outerband*3.9+0.7-pas[i][1])));
				}
				// daca tasta LEFT a fost apasata
				if(lab::glut::keySpecialStates[GLUT_KEY_LEFT] == true){
					// daca tasta DOWN este apasata in acelasi timp
					if(lab::glut::keySpecialStates[GLUT_KEY_DOWN] == true) {
						rot[i] -= vit[i]*3;
						cam[i].rotateTPSoY(vit[i]*3*(float)(PI/180), -0.5);
					}
					// daca tasta UP este apasata in acelasi timp
					else {
						rot[i] += vit[i]*3;
						cam[i].rotateTPSoY(-(vit[i]*3*(float)(PI/180)), -0.5);
					}
				}
				// daca tasta RIGHT a fost apasata
				if(lab::glut::keySpecialStates[GLUT_KEY_RIGHT] == true){
					// daca tasta DOWN este apasata in acelasi timp
					if(lab::glut::keySpecialStates[GLUT_KEY_DOWN] == true) {
						rot[i] += vit[i]*3;
						cam[i].rotateTPSoY(-(vit[i]*3*(float)(PI/180)), -0.5);
					}
					// daca tasta UP este apasata in acelasi timp
					else {
						rot[i] -= vit[i]*3;
						cam[i].rotateTPSoY(vit[i]*3*(float)(PI/180), -0.5);
					}
				}
				// miscarea camerei de filmat care urmareste jucatorul principal
				// banda dreapta
				if(pas[i][2] > -(bandlength/2) && pas[i][2] < bandlength/2){
					if(pas[i][0] > 0) {		// dreapta
						model_matrix_camera = glm::translate(original_model_matrix, glm::vec3(0,0,pas[i][2]));
					}
					else{	// stanga
						model_matrix_camera = glm::rotate(original_model_matrix,180.0f,glm::vec3(0,1,0));
						model_matrix_camera = glm::translate(model_matrix_camera, glm::vec3(0,0,-pas[i][2]));
					}
				}
				else {
					// curba de sus
					if(pas[i][2] <= -(bandlength/2)){
						model_matrix_camera = glm::translate(original_model_matrix, glm::vec3(0,0,-(bandlength/2)));
						if(pas[i][0] > 0)	// dreapta
							rot_cam = atan((-(bandlength/2)-pas[i][2])/pas[i][0])*(float)(180/PI);
						else	// stanga
							rot_cam = 180 - atan((-(bandlength/2)-pas[i][2])/-pas[i][0])*(float)(180/PI);
					}
					// curba de jos
					if(pas[i][2] >= bandlength/2) {
						model_matrix_camera = glm::rotate(original_model_matrix,180.0f,glm::vec3(0,1,0));
						model_matrix_camera = glm::translate(model_matrix_camera, glm::vec3(0,0,-(bandlength/2)));
						if(pas[i][0] < 0)	// stanga
							rot_cam = atan((pas[i][2]-bandlength/2)/-pas[i][0])*(float)(180/PI);
						else	// dreapta
							rot_cam = 180 - atan((pas[i][2]-bandlength/2)/pas[i][0])*(float)(180/PI);
					}
					model_matrix_camera = glm::rotate(model_matrix_camera,rot_cam,glm::vec3(0,1,0));
				}
				// se executa miscarea jucatorului principal
				model_matrix_masini[i] = glm::translate(original_model_matrix, pas[i]);
				model_matrix_masini[i] = glm::rotate(model_matrix_masini[i],rot[i],glm::vec3(0,1,0));

				// daca jucatorul principal a parcurs o tura si a trecut linia de finish
				if(floor(pas[i][2]) == 0 && pas[i][0] > 0 && lap == true) {
					finish = true;	// jocul se sfarseste
					winner = i;		// se tine minte castigatorul
				}
			}
		}
	}

	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		
		//pe tot ecranul
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		int i=0, c_cam;
		
		if(finish == false) {	// daca cursa nu s-a terminat
			do {

				if(view == false) {		// daca impartirea viewportului in 4 nu e activata
					glViewport(0, 0, screen_width, screen_height);
					c_cam = curent_cam; // camera de vizualizare selectata este cea curenta
				}
				else {	// daca impartirea viewportului in 4 e activata
					// se imparte viewportul, fiecare cu cate o camera de vizualizare specifica
					switch(i) {
						case 0: {
							glViewport(0, screen_height/2, screen_width/2, screen_height/2);
							c_cam = 2;
							break;
						}
						case 1: {
							glViewport(screen_width/2, screen_height/2, screen_width/2, screen_height/2);
							c_cam = curent_cam;
							break;
						}
						case 2: {
							glViewport(0, 0, screen_width/2, screen_height/2);
							c_cam = 4;
							break;
						}
						case 3: {
							glViewport(screen_width/2, 0, screen_width/2, screen_height/2);
							c_cam = 5;
							break;
						}
					}
				}

				//foloseste shaderul
				glUseProgram(gl_program_shader_curent);
							
				//trimite variabile uniforme la shader
				model_matrix = original_model_matrix;
				glUniformMatrix4fv(location_model_matrix,1,false,glm::value_ptr(model_matrix));
				glUniformMatrix4fv(location_view_matrix,1,false,glm::value_ptr(cam[c_cam].getViewMatrix()));
				glUniformMatrix4fv(location_projection_matrix,1,false,glm::value_ptr(projection_matrix));
				glUniform3f(location_light_position,light_position.x, light_position.y, light_position.z);
				glUniform3f(location_eye_position,eye_position.x, eye_position.y, eye_position.z);
				glUniform1i(location_material_shininess,material_shininess);
				glUniform1f(location_material_kd,material_kd);
				glUniform1f(location_material_ks,material_ks);
				glUniform4f(location_object_color, 0,0,0,0);

				//bind circuit
				for(int i = 0; i < vao_circuit.size(); i++) {
					glBindVertexArray(vao_circuit[i]);
					glDrawElements(GL_TRIANGLES, num_indices_circuit[i], GL_UNSIGNED_INT, 0);
				}
				// bind obj steag
				glUniformMatrix4fv(location_model_matrix,1,false,glm::value_ptr(model_matrix_objsteag));
				glUniform4f(location_object_color, 0.7,0.7,0.7,0);
				glBindVertexArray(vao_objsteag);
				glDrawElements(GL_TRIANGLES, num_indices_objsteag, GL_UNSIGNED_INT, 0);

				// bind camera de filmat
				for(int i = 0; i < vao_camera.size(); i++) {
					glUniformMatrix4fv(location_model_matrix,1,false,glm::value_ptr(model_matrix_camera));
					glUniform4f(location_object_color, 0,0,0,0);
					if(i == 1) {
						model_matrix_cam = glm::translate(model_matrix_camera, glm::vec3(bandwidth+outerband*3-outerband/2,outerband*3.9,0));
						model_matrix_cam = glm::rotate(model_matrix_cam,-90.0f+u_cam*(float)(180/PI),glm::vec3(0,0,1));
						model_matrix_cam = glm::rotate(model_matrix_cam,-90.0f,glm::vec3(0,1,0));
						model_matrix_cam = glm::scale(model_matrix_cam,glm::vec3(0.5,0.5,0.5));
						glUniformMatrix4fv(location_model_matrix,1,false,glm::value_ptr(model_matrix_cam));
						glUniform4f(location_object_color, 0,0.7,0.7,0);
					}
					glBindVertexArray(vao_camera[i]);
					glDrawElements(GL_TRIANGLES, num_indices_camera[i], GL_UNSIGNED_INT, 0);
				}
				// bind masini
				for(int i = 0; i < vao_masini.size(); i++) {
					glUniformMatrix4fv(location_model_matrix,1,false,glm::value_ptr(model_matrix_masini[i]));
					glUniform4f(location_object_color, color_masini[i][0],color_masini[i][1],color_masini[i][2],color_masini[i][3]);

					glBindVertexArray(vao_masini[i]);
					glDrawElements(GL_TRIANGLES, num_indices_masini[i], GL_UNSIGNED_INT, 0);
				}

				i ++; // se trece la viewportul urmator

			} while(i < 4 && view == true);
		}
		else {		// daca cursa s-a terminat, se afiseaza steagul fluturand cu culoarea castigatorului

			//foloseste shaderul
			glUseProgram(gl_program_shader_steag);

			glViewport(0, 0, screen_width, screen_height);
							
			//trimite variabile uniforme la shader
			model_matrix_steag = original_model_matrix;
			model_matrix_steag = glm::translate(model_matrix_steag, glm::vec3(-50,-30,0));
			view_matrix_steag = glm::lookAt(glm::vec3(0,0,100), glm::vec3(0,0,0), glm::vec3(0,1,0));
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_steag, "model_matrix"),1,false,glm::value_ptr(model_matrix_steag));
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_steag, "view_matrix"),1,false,glm::value_ptr(view_matrix_steag));
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_steag, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
			// bind suport steag
			glUniform1f(glGetUniformLocation(gl_program_shader_steag, "offset"), 0);
			glUniform4f(glGetUniformLocation(gl_program_shader_steag, "object_color"), 0.95,0.95,0.95,0);
			glBindVertexArray(vao_stick);
			glDrawElements(GL_QUADS, num_indices_stick, GL_UNSIGNED_INT, 0);
			// actualizarea si trimiterea offsetului la shader
			offset += 0.25;
			glUniform1f(glGetUniformLocation(gl_program_shader_steag, "offset"), offset);
			// setarea culorii castigatorului
			if(winner == 0)
				glUniform4f(glGetUniformLocation(gl_program_shader_steag, "object_color"), 0.85,0,0.85,0);
			if(winner == 1)
				glUniform4f(glGetUniformLocation(gl_program_shader_steag, "object_color"), 0.3,1,0,0);
			if(winner == 2)
				glUniform4f(glGetUniformLocation(gl_program_shader_steag, "object_color"), 1,0,0,0);
			if(winner == 3)
				glUniform4f(glGetUniformLocation(gl_program_shader_steag, "object_color"), 0.3,0,1,0);
			// bind steag
			glBindVertexArray(vao_steag);
			glDrawElements(GL_QUADS, num_indices_steag, GL_UNSIGNED_INT, 0);

		}

	}

	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){
		// gasirea locatiilor variabilelor din shader
		location_model_matrix = glGetUniformLocation(gl_program_shader_curent, "model_matrix");
		location_view_matrix = glGetUniformLocation(gl_program_shader_curent, "view_matrix");
		location_projection_matrix = glGetUniformLocation(gl_program_shader_curent, "projection_matrix");
		location_light_position = glGetUniformLocation(gl_program_shader_curent, "light_position");
		location_eye_position = glGetUniformLocation(gl_program_shader_curent, "eye_position");
		location_material_shininess = glGetUniformLocation(gl_program_shader_curent, "material_shininess");
		location_material_kd = glGetUniformLocation(gl_program_shader_curent, "material_kd");
		location_material_ks = glGetUniformLocation(gl_program_shader_curent, "material_ks");
		location_object_color = glGetUniformLocation(gl_program_shader_curent, "object_color");

	}

	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		screen_width = width;
		screen_height = height;
		float aspect = (float)width / (float)height;

		projection_matrix = glm::perspective(75.0f, aspect,0.1f, 10000.0f);
	}

	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader_gouraud);
			glDeleteProgram(gl_program_shader_phong);
			gl_program_shader_gouraud = lab::loadShader("shadere\\shader_gouraud_vertex.glsl", "shadere\\shader_gouraud_fragment.glsl");
			gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
			gl_program_shader_curent = gl_program_shader_gouraud;
			location_model_matrix = glGetUniformLocation(gl_program_shader_curent, "model_matrix");
			location_view_matrix = glGetUniformLocation(gl_program_shader_curent, "view_matrix");
			location_projection_matrix = glGetUniformLocation(gl_program_shader_curent, "projection_matrix");
			location_light_position = glGetUniformLocation(gl_program_shader_curent, "light_position");
			location_eye_position = glGetUniformLocation(gl_program_shader_curent, "eye_position");
			location_material_shininess = glGetUniformLocation(gl_program_shader_curent, "material_shininess");
			location_material_kd = glGetUniformLocation(gl_program_shader_curent, "material_kd");
			location_material_ks = glGetUniformLocation(gl_program_shader_curent, "material_ks");
			location_object_color = glGetUniformLocation(gl_program_shader_curent, "object_color");	
		}
		// tasta ENTER umple sau nu obiectele cu culoare
		if(key_pressed == 13){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_FILL:GL_LINE));
		}
		// tasta 1 schimba la camera anterioara
		if(key_pressed == '1') {
			if(curent_cam == 0)
				curent_cam = 5;
			else
				curent_cam --;
		}
		// tasta 2 schimba la camera urmatoare
		if(key_pressed == '2') {
			if(curent_cam == 5)
				curent_cam = 0;
			else
				curent_cam ++;
		}
		// tasta 3 imparte viewportul in 4
		if(key_pressed == '3') {
			view = !view;
		}
		// pentru camera de deasupra circuitului avem mai multe controale
		if(curent_cam == 5) {
			float v = 0.1f;
			if(key_pressed == 'w') { cam[curent_cam].translateForward(v*5); }
			if(key_pressed == 'a') { cam[curent_cam].translateRight(-v*5); }
			if(key_pressed == 's') { cam[curent_cam].translateForward(-v*5); }
			if(key_pressed == 'd') { cam[curent_cam].translateRight(v*5); }
			if(key_pressed == 'r') { cam[curent_cam].translateUpword(v*5); }
			if(key_pressed == 'f') { cam[curent_cam].translateUpword(-v*5); }
			if(key_pressed == 'e') { cam[curent_cam].rotateFPSoY(v); }
			if(key_pressed == 'q') { cam[curent_cam].rotateFPSoY(-v); }
			if(key_pressed == 'c') { cam[curent_cam].rotateFPSoZ(-v); }
			if(key_pressed == 'z') { cam[curent_cam].rotateFPSoZ(v); }
			if(key_pressed == 't') { cam[curent_cam].rotateFPSoX(v); }
			if(key_pressed == 'g') { cam[curent_cam].rotateFPSoX(-v); }
			if(key_pressed == 'o') { cam[curent_cam].set(glm::vec3(0,100,0), glm::vec3(0,0,0), glm::vec3(0,0,-1)); }
		}

	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) {
			lab::glut::enterFullscreen();		// activarea FullScreen
			glutSetCursor(GLUT_CURSOR_NONE);	// ascunderea cursorului mouseului
		}
		if(key_pressed == GLUT_KEY_F2) {
			lab::glut::exitFullscreen();			// iesirea din FullScreen
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);	// aparitia cursorului mouseului
		}
		// tasta F3 activeaza shaderul Gouraud
		if(key_pressed == GLUT_KEY_F3){
			gl_program_shader_curent = gl_program_shader_gouraud;
		}
		// tasta F4 activeaza shaderul Phong
		if(key_pressed == GLUT_KEY_F4){
			gl_program_shader_curent = gl_program_shader_phong;
		}
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout<<"Mouse scroll"<<std::endl;}

};

int main(){

	//setarea dimensiunilor ferestrei

	int w, h;

	cout << "  ----- Mentionez ca jocul a fost testat pe rezolutia de 1920x1080. ----- " << endl;
	cout << " -- Rularea pe o rezolutie mai mica de atat va duce la efecte nedorite. --" << endl;
	cout << " --		Rularea se va face cu setarile de 1920x1080 implicite.		--" << endl << endl;

	// se pot schimba acesti parametrii daca se doreste rularea cu alte dimensiuni a programului
	w = 1920;
	h = 1080;

	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Tema 3"),w,h,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window, context, framebuffer);

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl;
	std::cout<<"\tENTER ... toggle wireframe"<<std::endl<<"\to ... reseteaza camera"<<std::endl;
	std::cout<<"\tw ... translatie camera in fata"<<std::endl<<"\ts ... translatie camera inapoi"<<std::endl;
	std::cout<<"\ta ... translatie camera in stanga"<<std::endl<<"\td ... translatie camera in dreapta"<<std::endl;
	std::cout<<"\tr ... translatie camera in sus"<<std::endl<<"\tf ... translatie camera jos"<<std::endl;
	std::cout<<"\tq ... rotatie camera FPS pe Oy, plus"<<std::endl<<"\te ... rotatie camera FPS pe Oy, minus"<<std::endl;
	std::cout<<"\tz ... rotatie camera FPS pe Oz, plus"<<std::endl<<"\tc ... rotatie camera FPS pe Oz, minus"<<std::endl;
	std::cout<<"\tt ... rotatie camera FPS pe Ox, plus"<<std::endl<<"\tg ... rotatie camera FPS pe Ox, minus"<<std::endl;
	std::cout<<"\tUP, DOWN, LEFT, RIGHT ... controlul jucatorului principal"<<std::endl;
	std::cout<<"\tF1 ... activare FullScreen"<<std::endl<<"\tF2 ... dezactivarea FullScreen"<<std::endl;
	std::cout<<"\tF3 ... activare shader Gouraud"<<std::endl<<"\tF4 ... activare shader Phong"<<std::endl;
	std::cout<<"\t1 ... schimbarea camera anterioara"<<std::endl<<"\t2 ... schimbarea camera urmatoare"<<std::endl;
	std::cout<<"\t3 ... impartirea viewportului in 4"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}