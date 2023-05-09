/*!\file window.c
 * \brief géométries lumière diffuse et transformations de base en GL4Dummies
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date April 15 2016 */
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void resize(int w, int h);
static void draw(void);
static void quit(void);
/*!\brief dimensions de la fenêtre */
static int _wW = 1000, _wH = 1000;
/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief quelques objets géométriques */
static GLuint soleil = 0, anneau = 0;
static GLuint mercure = 0, venus = 0,terre = 0 ,mars = 0,jupiter = 0,saturne = 0, uranus= 0,neptune = 0;
/*!\brief La fonction principale créé la fenêtre d'affichage,
 * initialise GL et les données, affecte les fonctions d'événements et
 * lance la boucle principale d'affichage.*/
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 0, 0, 
			 _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))
    return 1;
  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwDisplayFunc(draw);
  gl4duwMainLoop();
  return 0;
}
/*!\brief initialise les paramètres OpenGL et les données */
static void init(void) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
  _pId  = gl4duCreateProgram("<vs>shaders/dep3d.vs", "<fs>shaders/dep3d.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  resize(_wW, _wH);
  soleil = gl4dgGenSpheref(100, 100);
  mercure = gl4dgGenSpheref(10, 10);
  venus = gl4dgGenSpheref(10, 10);
  terre = gl4dgGenSpheref(10, 10);
  mars = gl4dgGenSpheref(10, 10);
  jupiter = gl4dgGenSpheref(10, 10);
  saturne = gl4dgGenSpheref(10, 10);
  uranus= gl4dgGenSpheref(10, 10);
  neptune = gl4dgGenSpheref(10, 10);
  
  anneau = gl4dgGenTorusf(300, 30, 0.1f);
}
/*!\brief Cette fonction paramétre la vue (viewport) OpenGL en
 * fonction des dimensions de la fenêtre.*/
static void resize(int w, int h) {
  _wW  = w; _wH = h;
  glViewport(0, 0, _wW, _wH);
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * _wH / _wW, 0.5 * _wH / _wW, 1.0, 1000.0);
  //gl4duOrthof(-3.5, 3.5, -3.5 * _wH / _wW, 3.5 * _wH / _wW, 1.0, 1000.0);
  gl4duBindMatrix("modelViewMatrix");
}
/*!\brief dessine dans le contexte OpenGL actif. */
static void draw(void) {

static GLfloat a = 0;
GLfloat rouge[] = {1, 0, 0, 1}, vert[] = {0, 1, 0, 1}, bleu[] = {0, 0, 1, 1}, jaune[] = {1, 1, 0, 1};
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
gl4duBindMatrix("modelViewMatrix");
gl4duLoadIdentityf();
glUseProgram(_pId);
float vit_mercure = 0.04f; // vitesse orbitale de Mercure
float vit_venus = 0.02f; // vitesse orbitale de Vénus 
float vit_terre = 0.01f; // vitesse orbitale de la Terre 
float vit_mars = 0.008f; // vitesse orbitale de Mars 
float vit_jupiter = 0.005f;
float vit_saturne = 0.003f;
float vit_uranus = 0.002f;
float vit_neptune = 0.0015f;



   
gl4duPushMatrix(); {
    gl4duTranslatef(0.0f, 0.0f, -30.0f); // déplacer toutes les planètes sur l'axe central
    
    gl4duPushMatrix(); {
     gl4duTranslatef(0, 0, 0);
     gl4duRotatef(a * 2.0f, 1, 0, 0);
     gl4duScalef(3.5f, 3.5f, 3.5f);
     gl4duSendMatrices();
   } gl4duPopMatrix();
   glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, rouge);


    gl4dgDraw(soleil);
    
    // Faire tourner toutes les planètes autour de l'axe central à une vitesse différente
    gl4duRotatef(a * vit_mercure * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
    
    // Planète Mercure
    gl4duPushMatrix(); {
        gl4duTranslatef(0.0f, 0.0f, -5.0f);
        gl4duScalef(0.4f, 0.4f, 0.4f);
        gl4duSendMatrices();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, vert); // couleur rouge
        gl4dgDraw(mercure);
    } gl4duPopMatrix();
    
    // Faire tourner toutes les planètes autour de l'axe central à une vitesse différente
    gl4duRotatef(a * vit_venus * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
    
    // Planète Vénus
    gl4duPushMatrix(); {
        gl4duTranslatef(0.0f, 0.0f, -6.5f);
        gl4duScalef(0.7f, 0.7f, 0.7f);
        gl4duSendMatrices();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, vert); // couleur jaune
        gl4dgDraw(venus);
    } gl4duPopMatrix();
    
    // Faire tourner toutes les planètes autour de l'axe central à une vitesse différente
    gl4duRotatef(a * vit_terre * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
    
    // Planète Terre
    gl4duPushMatrix(); {
        gl4duTranslatef(0.0f, 0.0f, -8.5f);
        gl4duScalef(0.7f, 0.7f, 0.7f);
        gl4duSendMatrices();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu); // couleur bleue
        gl4dgDraw(terre);
    } gl4duPopMatrix();
    
    // Faire tourner toutes les planètes autour de l'axe central à une vitesse différente
    gl4duRotatef(a * vit_mars * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
    
    // Planète Mars
    gl4duPushMatrix(); {
        gl4duTranslatef(0.0f, 0.0f, -10.7f);
        gl4duScalef(0.5f, 0.5f, 0.5f);
        gl4duSendMatrices();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu); // couleur orange
        gl4dgDraw(mars);
    } gl4duPopMatrix();


    gl4duRotatef(a * vit_jupiter * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    gl4duPushMatrix(); {
        gl4duTranslatef(0.0f, 0.0f, -13.9f);
        gl4duScalef(1.7f, 1.7f, 1.7f);
        gl4duSendMatrices();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu); // couleur bleue
        gl4dgDraw(jupiter);
    } gl4duPopMatrix();

    gl4duRotatef(a * vit_saturne * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    gl4duPushMatrix(); {
        gl4duTranslatef(0.0f, 0.0f, -17.5f);
        gl4duScalef(1.4f, 1.4f, 1.4f);
        gl4duSendMatrices();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu); // couleur bleue
        gl4dgDraw(saturne);
    } gl4duPopMatrix();

    gl4duRotatef(a * vit_uranus * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    gl4duPushMatrix(); {
        gl4duTranslatef(0.0f, 0.0f, -19.5f);
        gl4duScalef(1.0f, 1.0f, 1.0f);
        gl4duSendMatrices();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu); // couleur bleue
        gl4dgDraw(uranus);
    } gl4duPopMatrix();

    gl4duRotatef(a * vit_neptune * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    gl4duPushMatrix(); {
        gl4duTranslatef(0.0f, 0.0f, -22.4f);
        gl4duScalef(1.0f, 1.0f, 1.0f);
        gl4duSendMatrices();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu); // couleur bleue
        gl4dgDraw(neptune);
    } gl4duPopMatrix();

} gl4duPopMatrix();

  
  
  /* Décommenter pour avoir un rendu sympathique pour pas cher :) */
  /*   gl4dfBlur(0, 0, 5, 1, 0, GL_FALSE); */
  /*   gl4dfSobelSetMixMode(GL4DF_SOBEL_MIX_MULT); */
  /*   gl4dfSobel(0, 0, GL_FALSE); */
  a = a + 0.1f;

  
}
/*!\brief appelée au moment de sortir du programme (atexit), libère les éléments utilisés */
static void quit(void) {
  gl4duClean(GL4DU_ALL);
}
