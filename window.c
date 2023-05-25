/*!\file window.c
 * \brief géométries lumière diffuse et transformations de base en GL4Dummies
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date April 15 2016 */
#include <GL4D/gl4dg.h>
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>


/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void loadTexture(GLuint id, const char * filename);
static void keyup(int keycode);
static void keydown(int keycode);
static void resize(int w, int h);
static void draw(void);
static void quit(void);

/*!\brief dimensions de la fenêtre */
static int _wW = 1000, _wH = 1000;
/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief quelques objets géométriques */
static GLuint soleil = 0, anneau = 0;
static GLuint mercure = 0, venus = 0,terre = 0 ,mars = 0,jupiter = 0,saturne = 0, uranus= 0,neptune = 0,etoile = 0;
static GLuint ecran =0;
static GLuint textID[13] = {0};
static GLuint _pause = 0;
static GLuint _vue = 0;
static GLuint _timer = 0;
static GLuint _reset = 0;

/*!\brief La fonction principale créé la fenêtre d'affichage,
 * initialise GL et les données, affecte les fonctions d'événements et
 * lance la boucle principale d'affichage.*/
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 0, 0, _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))return 1;
  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwKeyUpFunc(keyup);
  gl4duwKeyDownFunc(keydown);
  gl4duwDisplayFunc(draw);
  gl4duwMainLoop();
  return 0;
}
/*!\brief initialise les paramètres OpenGL et les données */
static void init(void) {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    _pId  = gl4duCreateProgram("<vs>shaders/dep3d.vs", "<fs>shaders/dep3d.fs", NULL);
    gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
    gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
    resize(_wW, _wH);
    soleil = gl4dgGenSpheref(30, 30);
    mercure = gl4dgGenSpheref(30, 30);
    venus = gl4dgGenSpheref(30, 30);
    terre = gl4dgGenSpheref(30, 30);
    mars = gl4dgGenSpheref(30, 30);
    jupiter = gl4dgGenSpheref(30, 30);
    saturne = gl4dgGenSpheref(30, 30);
    uranus= gl4dgGenSpheref(30, 30);
    neptune = gl4dgGenSpheref(30, 30);
    etoile = gl4dgGenSpheref(1,1);
    anneau = gl4dgGenTorusf(3000, 300, 0.1f);
    ecran = gl4dgGenQuadf();

    glGenTextures(sizeof textID / sizeof * textID, textID);
    assert(textID[0] && textID[1] && textID[2] && textID[3] &&textID[4] && textID[5] && textID[6] && textID[7] && textID[8] && textID[9] && textID[10] && textID[11]);

    loadTexture(textID[0], "images/soleil.jpg");
    loadTexture(textID[1], "images/mercure.jpg");
    loadTexture(textID[2], "images/venus.jpg");
    loadTexture(textID[3], "images/terre.jpg");
    loadTexture(textID[4], "images/mars.jpg");
    loadTexture(textID[5], "images/jupiter.jpg");
    loadTexture(textID[6], "images/saturne.jpg");
    loadTexture(textID[7], "images/uranus.jpg");
    loadTexture(textID[8], "images/neptune.jpg");
    loadTexture(textID[9],"images/saturnring.jpg" );
    loadTexture(textID[10], "images/espace.jpg");
    loadTexture(textID[11],"images/trou_noir.jpg");
    
    glBindTexture(GL_TEXTURE_2D, textID[12]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _wW / 2, _wH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glEnable(GL_TEXTURE_2D);


}

static void loadTexture(GLuint id, const char * filename) {
  SDL_Surface * t;
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if( (t = IMG_Load(filename)) != NULL ) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif       
    glTexImage2D(GL_TEXTURE_2D, 0, mode, t->w, t->h, 0, mode, GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", filename, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
}


/*!\brief Cette fonction paramétre la vue (viewport) OpenGL en
 * fonction des dimensions de la fenêtre.*/

static void resize(int w, int h) {
  _wW  = w; _wH = h;
  glViewport(0, 0, _wW, _wH);
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * _wH / _wW, 0.5 * _wH / _wW, 1.0, 1000.0);
  //gl4duOrthof(-3.5, 3.5, -3.5 * _wH / _wW, 3.5 * _wH / _wW, 1.0, 30.0);
  gl4duBindMatrix("modelViewMatrix");
}

static void keyup(int keycode) {
  switch(keycode) {
  default:
    break;
  }
}
static void keydown(int keycode) {
  GLint v[2];
  switch(keycode) {
    case 'a' :
          _timer = !_timer;
          break;
    case 'm' :
      _vue = (_vue +1)%2;
      break;
    case 'r' :
      _reset = !_reset;
      break;
    case 'w':
      glGetIntegerv(GL_POLYGON_MODE, v);
      if(v[0] == GL_FILL)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case ' ':
      _pause = !_pause;
      break;
    case 'q':
      exit(0);
      break;
    default:
      break;
  }
}
/*!\brief dessine dans le contexte OpenGL actif. */
static void draw(void) {

    static GLfloat a = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static Uint32 t0 = 0 ,t;
    GLfloat dt = 0.0,delai = 0.5f,delai_sun = 0.02f;
    dt = ((t = SDL_GetTicks()) - t0) / 1000.0;
    t0 = t;
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();
    glUseProgram(_pId);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(_pId, "tex"), 0);

    //Taille du soleil
    static float taille_sun = 3.5f;

    /*Distance des planete*/
    static float distance_mercure = 9.0f ;
    static float distance_venus = 11.5f;
    static float distance_terre = 14.5f;
    static float distance_mars = 16.7f;
    static float distance_jupiter = 19.9f;
    static float distance_saturne = 25.5f;
    static float distance_uranus = 30.5f;
    static float distance_neptune = 33.4f ;


    /*Vitesse en orbite des planetes */
    float vit_mercure = 0.04f;
    float vit_venus = 0.02f; 
    float vit_terre = 0.01f; 
    float vit_mars = 0.008f; 
    float vit_jupiter = 0.005f;
    float vit_saturne = 0.003f;
    float vit_uranus = 0.002f;
    float vit_neptune = 0.0015f;

    /*Inclinaison des planetes par rapport à leur rotation */
    float inclinaison_mercure = 7.005f;
    float inclinaison_venus = 3.39471f;
    float inclinaison_terre = 23.43929f;
    float inclinaison_mars = 25.19f;
    float inclinaison_jupiter = 3.13f;
    float inclinaison_saturne = 26.73f;
    float inclinaison_uranus = 97.86f;
    float inclinaison_neptune = 28.32f;
    
    /*Placement des backgrounds pour l'effet espace le premier dans l'axe y*/
    gl4duPushMatrix();{
      gl4duTranslatef(0, 0.0, -100.0);
      gl4duRotatef(0, 1, 0, 0);
      gl4duRotatef(a*0.05f, 0, 0, 1);
      gl4duScalef(100.0f,100.0f,100.0f);
      gl4duSendMatrices();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textID[10]);
      gl4dgDraw(ecran);
    }gl4duPopMatrix();
    
    if(_vue == 0)
      gl4duLookAtf(0.0f, 20.0f, 60.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -2.0f);//angle camera 
    else
      gl4duLookAtf(0.0f, 70.0f, 0.0f,0.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);


    static float facteur_agrandir = 1.1f;
    static float facteur_reduire = 1.0f;
    static float taille_max = 6.0f;
    static float taille_min = 2.5f;
    static float grand = 0 ;
    //Astre Soleil
    gl4duPushMatrix(); {

      gl4duTranslatef(0, 0, 0);
      gl4duRotatef(a, 0, 0.002f, 0);
      if (_timer && !_pause) {
        if(grand == 0){
          if (taille_sun < taille_max) {
            taille_sun += (facteur_agrandir * delai_sun);
            if(taille_sun > taille_max ){
              taille_sun = taille_max;
              grand++;
            
            }
          }
        }else {
          if (taille_sun > taille_min) {
            taille_sun -= (facteur_reduire * delai_sun);
            if (taille_sun< taille_min) {
              taille_sun = taille_min;
            }
          }
        }
      }
      gl4duScalef(taille_sun, taille_sun, taille_sun);
     
      gl4duSendMatrices();
    
    } gl4duPopMatrix();
    if (taille_sun == taille_min && _timer) {
      glBindTexture(GL_TEXTURE_2D,textID[11]);
    }else {

      glBindTexture(GL_TEXTURE_2D, textID[0]);
    }
    
    gl4dgDraw(soleil);
   
    
    
    gl4duRotatef(a * vit_mercure * 2.0f, 0.0f, 1.0f,0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    // Planète Mercure
    gl4duPushMatrix();{
      if (_timer && !_pause) {
        if (distance_mercure > 0.0f) {
          distance_mercure -= dt * delai;
          gl4duTranslatef(0.0f, 0.0f, -(distance_mercure));
          if(distance_mercure < 1.5f){
            gl4dgDelete(mercure);
          }
        }
      }
      else { 
        gl4duTranslatef(0.0f, 0.0f, -(distance_mercure));
      }
        gl4duRotatef(inclinaison_mercure,1.0f,0.0f,0.0f);
        gl4duRotatef(a,0.0f,0.58f,0.0f);
        gl4duScalef(0.4f, 0.4f, 0.4f);
        gl4duSendMatrices();
        glBindTexture(GL_TEXTURE_2D, textID[1]);
        gl4dgDraw(mercure);

    }gl4duPopMatrix();

   
    gl4duRotatef(a * vit_venus * 2.0f, 0.0f, 1.0f,0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    // Planète Vénus
    gl4duPushMatrix();{
    if (_timer && !_pause) {
      if (distance_venus > 0.0f) {
        distance_venus -= dt * delai;
        gl4duTranslatef(0.0f, 0.0f, -(distance_venus));
        if(distance_venus < 1.5f){
          gl4dgDelete(venus);
        }
        
      }
    }
    else { 
      gl4duTranslatef(0.0f, 0.0f, -(distance_venus));
    }
      gl4duRotatef(inclinaison_venus,1.0f,0.0f,0.0f);
      gl4duRotatef(a,0.0f,0.243f,0.0f);
      gl4duScalef(0.7f, 0.7f, 0.7f);
      gl4duSendMatrices();
      glBindTexture(GL_TEXTURE_2D, textID[2]);
      gl4dgDraw(venus);

    }gl4duPopMatrix();
    
    
    gl4duRotatef(a * vit_terre * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
    
    // Planète Terre
    gl4duPushMatrix(); {
    if (_timer && !_pause) {
      if (distance_terre > 0.0f) {
        distance_terre -= dt * delai;
        gl4duTranslatef(0.0f, 0.0f, -(distance_terre));
        if(distance_terre < 1.5f){
          gl4dgDelete(terre);
        }
        
      }
    }
    else { 
      gl4duTranslatef(0.0f, 0.0f, -(distance_terre));
    }
      
      gl4duRotatef(inclinaison_terre,1.0f,0.0f,0.0f);
      gl4duRotatef(a,0.0f,1.0f,0.0f);
      gl4duScalef(0.7f, 0.7f, 0.7f);
      gl4duSendMatrices();
      glBindTexture(GL_TEXTURE_2D, textID[3]);
      gl4dgDraw(terre);

    } gl4duPopMatrix();
    
    
    gl4duRotatef(a * vit_mars * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
    
    // Planète Mars
    gl4duPushMatrix(); {
    if (_timer && !_pause) {
      if (distance_mars > 0.0f) {
        distance_mars -= dt * delai;
        gl4duTranslatef(0.0f, 0.0f, -(distance_mars));
        if(distance_mars < 1.5f){
          gl4dgDelete(mars);
        }
        
      }
    }
    else { 
      gl4duTranslatef(0.0f, 0.0f, -(distance_mars));
    }

    
        gl4duRotatef(inclinaison_mars,1.0f,0.0f,0.0f);
        gl4duRotatef(a,0.0f,1.025f,0.0f);
        gl4duScalef(0.5f, 0.5f, 0.5f);
        gl4duSendMatrices();
        glBindTexture(GL_TEXTURE_2D, textID[4]);
        gl4dgDraw(mars);

    } gl4duPopMatrix();

    //Planete Jupiter
    gl4duRotatef(a * vit_jupiter * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    gl4duPushMatrix(); {
    if (_timer && !_pause) {
      if (distance_jupiter > 1.5f) {
        distance_jupiter -= dt * delai;
        gl4duTranslatef(0.0f, 0.0f, -(distance_jupiter));
        if(distance_jupiter < 1.5f){
          gl4dgDelete(jupiter);
        }
        
      }
    }
    else { 
      gl4duTranslatef(0.0f, 0.0f, -(distance_jupiter));
    }
        gl4duRotatef(inclinaison_jupiter,1.0f,0.0f,0.0f);
        gl4duRotatef(a,0.0f,4.125f,0.0f);
        gl4duScalef(1.7f, 1.7f, 1.7f);
        gl4duSendMatrices();
        glBindTexture(GL_TEXTURE_2D, textID[5]);
        gl4dgDraw(jupiter);

    } gl4duPopMatrix();

    gl4duRotatef(a * vit_saturne * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
  //Planete Saturne
  gl4duPushMatrix();{
    if (_timer && !_pause) {
      if (distance_saturne > 0.0f) {
        distance_saturne -= dt * delai;
        gl4duTranslatef(0.0f, 0.0f, -(distance_saturne));
        if(distance_saturne < 1.5f){
          gl4dgDelete(saturne);
          gl4dgDelete(anneau);

        }
        
      }
    }
    else { 
      gl4duTranslatef(0.0f, 0.0f, -(distance_saturne));
    }
      gl4duRotatef(inclinaison_saturne,1.0f,0.0f,0.0f);
      gl4duRotatef(a,0.0f,4.458f,0.0f);
      gl4duScalef(1.4f, 1.4f, 1.4f);
      gl4duSendMatrices();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textID[6]);
      gl4dgDraw(saturne);
    
    gl4duPushMatrix();
    {
      gl4duRotatef(a,0.0f,4.458f,0.0f);     // Ajoutez une rotation autour de l'axe des Y pour l'anneau
      gl4duTranslatef(0.0f, 0.0f, 0.0f);   // Placez l'anneau devant Saturne
      gl4duScalef(1.9f, 0.2f, 1.9f);
      gl4duSendMatrices();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textID[9]);
      gl4dgDraw(anneau);
    }
    gl4duPopMatrix();
  }gl4duPopMatrix();



  gl4duRotatef(a * vit_uranus * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
  gl4duSendMatrices();

  //Planete Uranus
  gl4duPushMatrix(); {
    if (_timer && !_pause) {
        if (distance_uranus > 0.0f) {
          distance_uranus -= dt * delai;
          gl4duTranslatef(0.0f, 0.0f, -(distance_uranus));
          if(distance_uranus < 1.5f){
            gl4dgDelete(uranus);
          }
        
        }
    }
    else { 
      gl4duTranslatef(0.0f, 0.0f, -(distance_uranus));
    }

    
      gl4duRotatef(inclinaison_uranus,1.0f,0.0f,0.0f);
      gl4duRotatef(a,0.0f,7.167f,0.0f);

      gl4duScalef(1.0f, 1.0f, 1.0f);
      gl4duSendMatrices();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textID[7]);
      gl4dgDraw(uranus);

  } gl4duPopMatrix();

  gl4duRotatef(a * vit_neptune * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
  gl4duSendMatrices();

    //Planete Neptune
  gl4duPushMatrix(); {
    if (_timer && !_pause) {
        if (distance_neptune > 0.0f) {
          distance_neptune -= dt * delai;
          gl4duTranslatef(0.0f, 0.0f, -(distance_neptune));
          if(distance_neptune < 1.5f){
            gl4dgDelete(neptune);
          }
        
        }
    }
    else {
      gl4duTranslatef(0.0f, 0.0f, -(distance_neptune));
    }
      gl4duRotatef(inclinaison_neptune,1.0f,0.0f,0.0f);
      gl4duRotatef(a,0.0f,6.708f,0.0f);
      gl4duScalef(1.0f, 1.0f, 1.0f);
      gl4duSendMatrices();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textID[8]);
      gl4dgDraw(neptune);

  } gl4duPopMatrix();

    if(!_pause)
    a = a + 5.1f;
  
}
/*!\brief appelée au moment de sortir du programme (atexit), libère les éléments utilisés */
static void quit(void) {
  if(textID[0]) {
    glDeleteTextures(sizeof textID / sizeof *textID, textID);
    textID[0] = 0;
  }
  gl4duClean(GL4DU_ALL);
}
