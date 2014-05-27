//-------------------------------------------------------------------------------------------------
//
// TEMA 4: SLEEPING BEAUTY
//
// nume:		Bojin
// prenume:		Andrei Florian
// grupa/seria: 333 CA
//
//-------------------------------------------------------------------------------------------------

//incarcator de meshe
#include "lab_mesh_loader.hpp"
//geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"
//incarcator de shadere
#include "lab_shader_loader.hpp"
//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"
//texturi
#include "lab_texture_loader.hpp"
//time
#include <ctime>

#define PI 3.14159265358979323846


class Laborator : public lab::glut::WindowListener {


//variabile
private:

	glm::mat4 model_matrix, original_model_matrix;		//matrici modelare
	glm::mat4 view_matrix, projection_matrix;			//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int gl_program_shader;						//id-ul de opengl al obiectului de tip program shader
	unsigned int screen_width, screen_height;			//dimensiunile ferestrei

	// variabile iluminare
	glm::vec3 light_position;
	glm::vec3 eye_position;
	unsigned int material_shininess;
	float material_kd;
	float material_ks;

	// containere pentru vertices, indices
	std::vector<lab::VertexFormat> sleep_vertices, surprise_vertices, annoyed_vertices;
	std::vector<unsigned int> sleep_indices, surprise_indices, annoyed_indices;

	//meshe
	unsigned int vbo, ibo;
	unsigned int sleep_vao, sleep_num_indices;					
	unsigned int surprise_vao, surprise_num_indices;	
	unsigned int annoyed_vao, annoyed_num_indices;		

	//textura fetei
	unsigned int girl_texture;

	//atribute stare curenta
	unsigned int current_vao;
	unsigned int current_num_indices, next_num_indices;

	float start_time, lapsed_time, excess_time;		//timpii starilor
	bool change_state;								//indica necesitatea schimbarii starii
	int state_id;									//id-ul starii curente


//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator() {

		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");

		//prima stare: sleep
		lab::_loadObjFile("resurse\\girl_sleep.obj", sleep_vertices, sleep_indices);	

		//a doua stare: surprise
		lab::_loadObjFile("resurse\\girl_surprise.obj", surprise_vertices, surprise_indices);

		//a treia stare: annoyed
		lab::_loadObjFile("resurse\\girl_annoyed.obj", annoyed_vertices, annoyed_indices);

		//incarcarea in atribute a starilor
		setupFrame(sleep_vao, vbo, ibo, sleep_num_indices, sleep_vertices, sleep_indices, surprise_vertices, surprise_indices);
		setupFrame(surprise_vao, vbo, ibo, surprise_num_indices, surprise_vertices, surprise_indices, annoyed_vertices, annoyed_indices);
		setupFrame(annoyed_vao, vbo, ibo, annoyed_num_indices, annoyed_vertices, annoyed_indices, sleep_vertices, sleep_indices);

		//textura
		girl_texture = lab::loadTextureBMP("resurse\\girl_texture.bmp");

		//lumina & material
		eye_position = glm::vec3(0,6.5,3);
		light_position = glm::vec3(0,50,50);
		material_shininess = 100;
		material_kd = 0.5;
		material_ks = 0.5;

		//matrici de modelare si vizualizare
		original_model_matrix = glm::mat4(1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1);
		model_matrix = glm::rotate(original_model_matrix, 20.0f, glm::vec3(0,1,0));
		view_matrix = glm::lookAt(glm::vec3(0,6.5,3), glm::vec3(0,7.3,0), glm::vec3(0,1,0));

		//setarea starii curente
		current_vao = sleep_vao;
		current_num_indices = sleep_num_indices;
		start_time = clock();	//pornirea timerului
		excess_time = 0;
		change_state = false;
		state_id = 0;

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator() {

		//distruge shader
		glDeleteProgram(gl_program_shader);

		//distruge obiecte
		glDeleteVertexArrays(1,&sleep_vao);
		glDeleteVertexArrays(1,&surprise_vao);
		glDeleteVertexArrays(1,&annoyed_vao);

		glDeleteBuffers(1,&vbo);
		glDeleteBuffers(1,&ibo);

		glDeleteTextures(1, &girl_texture);
	
	}

	// functie de incarcare in atribute a starilor pornind de la containerele vertices si indices
	void setupFrame(unsigned int &vao, unsigned int& vbo, unsigned int &ibo, unsigned int &num_indices,
					std::vector<lab::VertexFormat> start_vertices, std::vector<unsigned int> start_indices,
					std::vector<lab::VertexFormat> end_vertices, std::vector<unsigned int> end_indices) {

		//creeaza obiectele OpenGL necesare desenarii
		unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

		//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
		glGenVertexArrays(1, &gl_vertex_array_object);
		glBindVertexArray(gl_vertex_array_object);

		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&gl_vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, start_vertices.size()*sizeof(lab::VertexFormat), &start_vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&gl_index_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, start_indices.size()*sizeof(unsigned int), &start_indices[0], GL_STATIC_DRAW);

		//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4,2,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&gl_vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, end_vertices.size()*sizeof(lab::VertexFormat), &end_vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&gl_index_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, end_indices.size()*sizeof(unsigned int), &end_indices[0], GL_STATIC_DRAW);

		//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5,2,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

		vao = gl_vertex_array_object;
		vbo = gl_vertex_buffer_object;
		ibo = gl_index_buffer_object;
		num_indices = start_indices.size();
	}


	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame() {
	
		lapsed_time = (clock()-start_time)/1000;	//timpul trecut de la starea anterioara

		if(excess_time != 0) {
			lapsed_time -= excess_time;				//se elimina timpul in excess de la trecerea dintre stari
			excess_time = 0;
		}

		if(lapsed_time >= 5) {						//se trece la urmatoarea stare daca timerul a depasit 5 sec
			change_state = true;
			start_time = clock();					//se reseteaza timerul
			excess_time = 5 - lapsed_time;			//se determina excesul de timp de la o stare la alta
			lapsed_time = 5;						//se truncheaza la 5 sec intervalul dintre starea curenta si urmatoarea
		}

	}

	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame() {
		
		//pe tot ecranul
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//foloseste shaderul
		glUseProgram(gl_program_shader);
				
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		glUniform3f(glGetUniformLocation(gl_program_shader, "light_position"),light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(gl_program_shader, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
		glUniform1i(glGetUniformLocation(gl_program_shader, "material_shininess"),material_shininess);
		glUniform1f(glGetUniformLocation(gl_program_shader, "material_kd"),material_kd);
		glUniform1f(glGetUniformLocation(gl_program_shader, "material_ks"),material_ks);

		//trimite textura la shader
		glActiveTexture(GL_TEXTURE0+1);
		glBindTexture(GL_TEXTURE_2D, girl_texture);
		glUniform1i(glGetUniformLocation(gl_program_shader, "textura"), 1);

		//trimite timerul la shader
		glUniform1f(glGetUniformLocation(gl_program_shader, "time"), (float)(lapsed_time/5));

		//desenarea fetei
		glBindVertexArray(current_vao);
		glDrawElements(GL_TRIANGLES, current_num_indices, GL_UNSIGNED_INT, 0);

	}

	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame() {
	
		//daca este necesara schimbarea starii curente
		if(change_state == true) {
			//se schimba starea curenta dupa identificarea id-ului starii anterioare
			if(state_id == 0) {
				current_vao = surprise_vao;
				current_num_indices = surprise_num_indices;
			}
			if(state_id == 1) {
				current_vao = annoyed_vao;
				current_num_indices = annoyed_num_indices;
			}
			if(state_id == 2) {
				current_vao = sleep_vao;
				current_num_indices = sleep_num_indices;
			}
			//se incrementeaza id-ul
			if(state_id != 2)
				state_id ++;
			else
				state_id = 0;
		}
		change_state = false;	

	}

	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height) {
		//reshape
		if(height==0) height=1;
		screen_width = width;
		screen_height = height;
		glViewport(0, 0, screen_width, screen_height);
		float aspect = (float)width / (float)height;
		projection_matrix = glm::perspective(75.0f, aspect, 0.1f, 10000.0f);

	}

	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y) {

		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader);
			gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		}
		if(key_pressed == 'w'){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}

	}

	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y) {}

	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {

		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();

	}

	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y) {}

	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y) {}

	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y) {}

	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y) {}

	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){}

};


int main(){

	int w, h;

	w = 1920/2;
	h = 1080/2;

	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Tema 4"),w,h,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\tw ... toggle wireframe"<<std::endl;

	//run
	lab::glut::run();

	return 0;

}