/*!\file window.c
 * \brief syteme solaire avec différent option (credit,déplacement,texture ) en GL4Dummies
 * \author MATHANARUBAN Jonny, jonnymthdev@gmail.com
 * \date May 29 2023 */

#include <GL4D/gl4dg.h>
#include <assert.h>
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

//Definition des mask car non detecté dans le fichier 
#define R_MASK 0xff000000
#define G_MASK 0x00ff0000
#define B_MASK 0x0000ff00
#define A_MASK 0x000000ff





/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void loadTexture(GLuint id, const char * filename);
static void initText(GLuint * ptId, const char * text);
static void drawTextCreditdebut(GLuint _tId, GLuint _textTexId,GLuint objet);
static void ahInitAudio(const char * file);
static void keydown(int keycode);
static void resize(int w, int h);
static void draw(void);
static void quit(void);



/*!\brief dimensions de la fenêtre */
static int _wW = 1000, _wH = 1000;
/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0,_tId = 0;
/*!\brief quelques objets géométriques */
static GLuint soleil = 0, anneau = 0,mercure = 0, venus = 0,terre = 0 ,mars = 0,jupiter = 0,saturne = 0, uranus= 0,neptune = 0;
static GLuint ecran = 0,ecrancredit = 0,_textTexId = 0;
static GLuint textID[28] = {0};
static GLuint _pause = 0,_vue = 0, _timer = 0,_reset = 0,_eclairmode = 0,_credit = 0;

static GLfloat yvue = 20.0f;
static GLfloat zvue = 60.0f;
static Mix_Music * _mmusic = NULL;



/*!\brief La fonction principale créé la fenêtre d'affichage,
 * initialise GL et les données, affecte les fonctions d'événements et
 * lance la boucle principale d'affichage.*/
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "Systeme Solaire", 0, 0, _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))return 1;
  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
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
  _tId = gl4duCreateProgram("<vs>shaders/credits.vs", "<fs>shaders/credits.fs", NULL);
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
  anneau = gl4dgGenTorusf(3000, 300, 0.1f);
  ecran = gl4dgGenQuadf();
  ecrancredit = gl4dgGenQuadf();
  const char* text = "Ceci était la création \n"
    " d'un trou noir au milieu  "
    " du système solaire , dans ce "
    " programme la distance des "
    " planètes sont à une échelle " 
    " réduite par rapport au "
    " soleil et sa taille aussi  .\n\n\n "
    " En effet lorsque le soleil "
    " consomme toutes ses ressources "
    " il devient une géante rouge "
    " qui augmente dans un temps "
    " puis rétrecit dans un autre " 
    " pour former un trou noir\n\n\n "
    " Ce programme a pour but de " 
    " reproduire ce phénomène  "
    " \n\n\n"
    " Merci \n\n"
    " Créer par MATHANARUBAN Jonny\n\n\n\n"
    " Pour quitter le programme appuyer"
    " sur la touche 'q' ou Patienter ";
  
  initText(&_textTexId, text);
  
    
    
    

  glGenTextures(sizeof textID / sizeof * textID, textID);
  for (int i = 0;i<27; i++) {
    assert(textID[i]);
  }

  //texture général 
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
  loadTexture(textID[25],"images/soleilrouge.jpg");

  //texture pour la normal map
  loadTexture(textID[12], "images/soleil_normal.jpg");
  loadTexture(textID[13], "images/mercure_normal.jpg");
  loadTexture(textID[14], "images/venus_normal.jpg");
  loadTexture(textID[15], "images/terre_normal.jpg");
  loadTexture(textID[16], "images/mars_normal.jpg");
  loadTexture(textID[17], "images/jupiter_normal.jpg");
  loadTexture(textID[18], "images/saturne_normal.jpg");
  loadTexture(textID[19], "images/uranus_normal.jpg");
  loadTexture(textID[20], "images/neptune_normal.jpg");
  loadTexture(textID[21], "images/saturnring_normal.jpg" );
  loadTexture(textID[22], "images/espace_normal.jpg");
  loadTexture(textID[23], "images/trou_noir_normal.jpg");
  loadTexture(textID[26], "images/soleilrouge_normal.jpg");

  //texture mouvement
  loadTexture(textID[24],"images/element.jpg");

   
    
  glBindTexture(GL_TEXTURE_2D, textID[27]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _wW / 2, _wH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glEnable(GL_TEXTURE_2D);

  //initialisation de la musique 

    
  ahInitAudio("musique/Nomyn-Fragments.mp3");


}



void ahInitAudio(const char * file) {
  int mixFlags = MIX_INIT_OGG | MIX_INIT_MP3, res;
  res = Mix_Init(mixFlags);
  if( (res & mixFlags) != mixFlags ) {
    fprintf(stderr, "Mix_Init: Erreur lors de l'initialisation de la bibliotheque SDL_Mixer\n");
    fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
    exit(-3);
  }
  if(Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 1024) < 0)
    exit(-4);
  if(!(_mmusic = Mix_LoadMUS(file))) {
    fprintf(stderr, "Erreur lors du Mix_LoadMUS: %s\n", Mix_GetError());
    exit(-5);
  }
  if(!Mix_PlayingMusic())
    Mix_PlayMusic(_mmusic, -1);
}



static void loadTexture(GLuint id, const char * filename) {
  SDL_Surface * t;
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if( (t = IMG_Load(filename)) != NULL ) {
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;      
    glTexImage2D(GL_TEXTURE_2D, 0, mode, t->w, t->h, 0, mode, GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", filename, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
}



static void initText(GLuint * ptId, const char * text) {
  static int firstTime = 1;
  SDL_Color c = {255, 255, 0, 255};
  SDL_Surface * d, * s;
  TTF_Font * font = NULL;
  if(firstTime) {
    /* initialisation de la bibliothèque SDL2 ttf */
    if(TTF_Init() == -1) {
      fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
      exit(2);
    }
    firstTime = 0;
  }
  if(*ptId == 0) {
    /* initialisation de la texture côté OpenGL */
    glGenTextures(1, ptId);
    glBindTexture(GL_TEXTURE_2D, *ptId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  /* chargement de la font */
  if( !(font = TTF_OpenFont("DejaVuSans-Bold.ttf", 128)) ) {
    fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
    return;
  }
  /* création d'une surface SDL avec le texte */
  d = TTF_RenderUTF8_Blended_Wrapped(font, text, c, 2048);
  if(d == NULL) {
    TTF_CloseFont(font);
    fprintf(stderr, "Erreur lors du TTF_RenderText\n");
    return;
  }
  /* copie de la surface SDL vers une seconde aux spécifications qui correspondent au format OpenGL */
  s = SDL_CreateRGBSurface(0, d->w, d->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);
  assert(s);
  SDL_BlitSurface(d, NULL, s, NULL);
  SDL_FreeSurface(d);
  /* transfert vers la texture OpenGL */
  glBindTexture(GL_TEXTURE_2D, *ptId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
  fprintf(stderr, "Dimensions de la texture : %d %d\n", s->w, s->h);
  SDL_FreeSurface(s);
  TTF_CloseFont(font);
  glBindTexture(GL_TEXTURE_2D, 0);
}
static void drawTextCreditdebut(GLuint _tId, GLuint _textTexId,GLuint objet) {
  const GLfloat inclinaison = -60.0;
  static GLfloat t0 = -1;
  GLfloat t, d;

  if(t0 < 0.0f)
    t0 = SDL_GetTicks();
  t = (SDL_GetTicks() - t0) / 1000.0f;
  d = -1.4f /* du retard pour commencer en bas */ + 0.1f /* vitesse */ * t;

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glUseProgram(_tId);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textTexId);

  glUniform1i(glGetUniformLocation(_tId, "inv"), 1);
  glUniform1i(glGetUniformLocation(_tId, "tex"), 0);

  gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();

  gl4duScalef(1, 2, 1);
  gl4duTranslatef(0.2f, d * cos(inclinaison * M_PI / 180.0f), -2 + d * sin(inclinaison * M_PI / 180.0f));
  gl4duRotatef(inclinaison, 1, 0, 0);

  gl4duSendMatrices();
  gl4dgDraw(objet);

  glUseProgram(0);

  if (d >= 1.4f) {
    gl4duClean(GL4DU_ALL);
    exit(0);
  
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

static void keydown(int keycode) {
  GLint v[2];
  switch(keycode) {
    case 'a' :
          _timer = !_timer;
          _eclairmode = !_eclairmode;
        break;
    case 'e' :
          _eclairmode = !_eclairmode;
        break;
    case 'c':
          _credit = !_credit;
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
    case SDLK_UP:
          zvue -= 0.2f;
        break;
    case SDLK_DOWN:
          zvue += 0.2f;
        break;
    case SDLK_RIGHT:
          yvue +=0.2f;
        break;
    case SDLK_LEFT:
          yvue -=0.2f;
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
  if (_credit) {
    drawTextCreditdebut(_tId,_textTexId,ecrancredit);
  }else {
  
    static GLfloat a = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static Uint32 t0 = 0 ,t;
    GLfloat dt = 0.0,delai = 0.9f,delai_sun = 0.02f;
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
    static float vit_mercure = 0.04f;
    static float vit_venus = 0.02f; 
    static float vit_terre = 0.01f; 
    static float vit_mars = 0.008f; 
    static float vit_jupiter = 0.005f;
    static float vit_saturne = 0.003f;
    static float vit_uranus = 0.002f;
    static float vit_neptune = 0.0015f;

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
      gl4duTranslatef(0, 0.0, -500.0);
      gl4duRotatef(0, 1, 0, 0);
      gl4duRotatef(a*0.05f, 0, 0, 1);
      gl4duScalef(500.0f,500.0f,500.0f);
      gl4duSendMatrices();
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[22]);//texture espace normal map
       glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[10]);//texture espace
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
      gl4dgDraw(ecran);
    }gl4duPopMatrix();
    if (_reset) {
      zvue = 60.0f;
      yvue = 20.0f;
      _reset = !_reset;
    }
    static GLfloat dezoomey = 70.0f;

    if(_vue == 0)
      gl4duLookAtf(0.0f, yvue, zvue, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -2.0f);//angle camera 
    else
      gl4duLookAtf(0.0f, dezoomey, 0.0f,0.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);


    /*if (distance_terre < 2.5f) {
      _vue = !_vue;
    }*/


    static float facteur_agrandir = 1.1f;
    static float facteur_reduire = 1.0f;
    static float taille_max = 6.0f;
    static float taille_min = 2.5f;
    static float grand = 0 ;
    //Astre Soleil
    gl4duPushMatrix(); {

      gl4duTranslatef(0, 0, 0);
      gl4duRotatef(a, 0, 0.002f, 0);
      if (!_timer && !_pause) {
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
    if (taille_sun > taille_min && !_timer) {
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[26]);//texture etoile rouge normal map
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
      
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[25]); //texture etoile rouge
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
    }
    else if (taille_sun == taille_min && !_timer) {
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[23]);//texture trou noir normal map
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
      
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[11]);//textire trou noir  
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);


    }else {
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[12]);//texture soleil normal map
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[0]);//texture soleil 
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
  
    }
    if (!_eclairmode && taille_sun == taille_max) {
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, textID[24]);//texture superposé 
      glUniform1i(glGetUniformLocation(_pId, "tex2"), 2);
    }

    gl4dgDraw(soleil);
   
    
    
    gl4duRotatef(a * vit_mercure * 2.0f, 0.0f, 1.0f,0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    // Planète Mercure
    gl4duPushMatrix();{
      if (!_timer && !_pause) {
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
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textID[13]);
        glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textID[1]);
        glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
        glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
        gl4dgDraw(mercure);

    }gl4duPopMatrix();

   
    gl4duRotatef(a * vit_venus * 2.0f, 0.0f, 1.0f,0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    // Planète Vénus
    gl4duPushMatrix();{
    if (!_timer && !_pause) {
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
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[14]);
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[2]);
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
      gl4dgDraw(venus);

    }gl4duPopMatrix();
    
    
    gl4duRotatef(a * vit_terre * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
    
    // Planète Terre
    gl4duPushMatrix(); {
    if (!_timer && !_pause) {
      if (distance_terre > 0.0f) {
        distance_terre -= dt * delai;
        gl4duTranslatef(0.0f, 0.0f, -(distance_terre));

        if(distance_terre < 1.5f){
          gl4dgDelete(terre);
          _vue = 1;  
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
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[15]);
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[3]);
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
      gl4dgDraw(terre);

    } gl4duPopMatrix();
    
    
    gl4duRotatef(a * vit_mars * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
    
    // Planète Mars
    gl4duPushMatrix(); {
    if (!_timer && !_pause) {
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
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textID[16]);
        glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textID[4]);
        glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
        glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
        gl4dgDraw(mars);

    } gl4duPopMatrix();

    //Planete Jupiter
    gl4duRotatef(a * vit_jupiter * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();

    gl4duPushMatrix(); {
    if (!_timer && !_pause) {
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
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textID[17]);
        glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textID[5]);
        glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
        glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
        gl4dgDraw(jupiter);

    } gl4duPopMatrix();

    gl4duRotatef(a * vit_saturne * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
    gl4duSendMatrices();
  //Planete Saturne
  gl4duPushMatrix();{
    if (!_timer && !_pause) {
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
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[18]);
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[6]);
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
      gl4dgDraw(saturne);
    
    gl4duPushMatrix();
    {
      gl4duRotatef(a,0.0f,4.458f,0.0f);     // Ajoutez une rotation autour de l'axe des Y pour l'anneau
      gl4duTranslatef(0.0f, 0.0f, 0.0f);   // Placez l'anneau devant Saturne
      gl4duScalef(1.9f, 0.2f, 1.9f);
      gl4duSendMatrices();
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[21]);
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[9]);
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0); 
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
      gl4dgDraw(anneau);
    }
    gl4duPopMatrix();
  }gl4duPopMatrix();



  gl4duRotatef(a * vit_uranus * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
  gl4duSendMatrices();

  //Planete Uranus
  gl4duPushMatrix(); {
    if (!_timer && !_pause) {
        if (distance_uranus > 0.0f) {
          distance_uranus -= dt * delai;
          gl4duTranslatef(0.0f, 0.0f, -(distance_uranus));
          if(distance_uranus < 1.5f){
            gl4dgDelete(uranus);
            dezoomey += 0.55f;
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
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[19]);
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[7]);
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
      gl4dgDraw(uranus);

  } gl4duPopMatrix();

  gl4duRotatef(a * vit_neptune * 2.0f, 0.0f, 1.0f, 0.0f); // rotation en fonction du temps et de la vitesse orbitale
  gl4duSendMatrices();

    //Planete Neptune
  gl4duPushMatrix(); {
    if (!_timer && !_pause) {
        if (distance_neptune > 0.0f) {
          distance_neptune -= dt * delai;
          gl4duTranslatef(0.0f, 0.0f, -(distance_neptune));
          if(distance_neptune < 1.5f){
            gl4dgDelete(neptune);
            _credit = !_credit;// des que neptune disparait lancement du credit de fin 
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
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textID[20]);
      glUniform1i(glGetUniformLocation(_pId, "nm"), 1);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,textID[8]);
      glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
      glUniform1i(glGetUniformLocation(_pId, "use_nm"), 1);
      gl4dgDraw(neptune);

  } gl4duPopMatrix();
  #ifdef __APPLE__
    if (!_pause)
      a = a + 2.1f;
    #else
      if (!_pause)
        a += 5.1f;
  #endif

  }
}
/*!\brief appelée au moment de sortir du programme (atexit), libère les éléments utilisés */
static void quit(void) {
  for (int i = 0;i < 25;++i) {
    if(textID[i]) {
      glDeleteTextures(sizeof textID / sizeof *textID, textID);
      textID[i] = 0;
    }
  }
  if (_mmusic != NULL) {
    Mix_FreeMusic(_mmusic);
    _mmusic = NULL;
  }
  if(_textTexId) {
    glDeleteTextures(1, &_textTexId);
    _textTexId = 0;
  }
  gl4duClean(GL4DU_ALL);
}
