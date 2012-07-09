/**********************************************/
/*************** INCLUDES *********************/
/**********************************************/

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**********************************************/
/*************** DEFINES **********************/
/**********************************************/

// Defines de Dimensão da Tela
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Defines Utils
#define FILE_NAME_SIZE 11
#define CUBE_PIXELS 4
#define ANIMATE_TIME 1
#define ANIMATE_TIME_SHOTS 1
#define SCALE 0.1

// Defines de Inimigos
#define ENEMIES_X_LOGIC_MAX 10
#define ENEMIES_UP_BOUND_MAX 6
#define ENEMIES_X_SPLIT_PIXELS 13
#define ENEMIES_Y_SPLIT_PIXELS 12
#define DIR_ENEMY 2
#define STEP_ENEMY_Y 5

// Defines de Blocos
#define BLOCKS_X_LOGIC_MAX 80
#define BLOCKS_UP_BOUND_MAX 2
#define BLOCKS_SETS 20
#define BLOCKS_SPLIT_X 20

// Defines de Posição Lógica de Inicio
#define X_INIT_ENEMIES 2
#define Y_INIT_ENEMIES 65
#define X_INIT_SPACESHIP 100
#define Y_INIT_SPACESHIP 5
#define X_INIT_BLOCKS 30
#define Y_INIT_BLOCKS 15
#define X_INIT_UFO 2
#define Y_INIT_UFO 135
#define X_INIT_LIVES 180
#define Y_INIT_LIVES 144

// Define de Bordas
#define LEFT_BOUND_ENEMY 2
#define RIGHT_BOUND_ENEMY 76
#define LEFT_BOUND_SPACESHIP 1
#define RIGHT_BOUND_SPACESHIP 192
#define RIGHT_BOUND_UFO 192
#define UP_BOUND_MAX 160

// Defines da Nave
#define SPACESHIP_STEP 2

// Defines de Tiro da Nave
#define SHOTS_SPACESHIP_MAX 10
#define INC_SHOT_Y 5
#define INIT_FIRST_SHOT_X 1
#define INIT_SECOND_SHOT_X 6
#define INIT_SHOT_Y 5

// Defines de Tiro dos Inimigos
#define PROB_SHOOT_ENEMY 5
#define MAX_SHOTS_ENEMIES 50
#define INIT_MAX_SHOTS_ENEMIES 2
#define LEVEL_MAX_SHOTS_ENEMIES 2

// Defines do Disco Voador
#define PROB_UFO 1
#define STEP_UFO_X 5

// Defines de Vida
#define MAX_LIVES 2

// Defines de Pontuação
#define POINTS_PER_LINE 100
#define UFO_POINT 100

// Defines de Tela
#define START_SCREEN 1
#define GAME_SCREEN 2
#define END_SCREEN 3

/**********************************************/
/*************** ESTRUTURAS *******************/
/**********************************************/

typedef struct BMPImagem {
    int   width;
    int   height;
    char *data;
}BMPImage;

typedef struct {
    int x;
    int y;
    int z;
    int time;
} TTimePosition;

typedef struct {
    int color;
    int animateFlag;
    int deathAnimateFlag;
    TTimePosition *timePositionsAnimation;
    TTimePosition *timePositionsDeathAnimation;
} TCube;

typedef struct {
    int kind; // 0: SpaceShip // 1: Enemy1 // 2: Enemy2 ... // 4: Shot // 5: Block
    TCube ***objectMatrix; // Matriz de Desenho
    TCube *animateCubes;
    int sizeAnimateCubes;
    int animateTime;
    int currentAnimateTime;
    int inAnimation;
    TCube *deathAnimateCubes;
    int sizeDeathAnimateCubes;
    int deathAnimateTime;
    int currentDeathAnimateTime;
    int inAgony;
    int xMax, yMax, zMax; // Indices Maximos ObjectMatrix
    int xLogic, yLogic, zLogic; // Posição logica
    int alive;
}TObject;

/*******************************************/
/*************** FUNÇÕES *******************/
/*******************************************/

void getBitmapImageData( char *pFileName, BMPImage *pImage );

void loadTexture(char* Filename);

void initBackgroundTexture (void);

void initIntroTexture (void);

void initGameOverTexture (void);

void initStartGlobalParameters();

void initLevelGlobalParameters();

FILE* initFile(FILE* objectFile, int kind);

TObject initObjectFile(TObject object);

TObject initObject(int kind);

TObject initLogicObject(TObject object, int xLogic, int yLogic, int zLogic);

void initEnemies();

void initSpaceShip();

void initShots();

void initBlocks();

void initUFO();

void initLives();

void initAllObjects();

void setCubeColor(int colorIndex);

void printObject(TObject object);

void printEnemies();

void printSpaceShip();

void printShots();

void printBlocks();

void renderBitmapString(float x, float y, float z, void *font, char *string);

void printPoints();

void printGameOverPoints();

void printTexture();

void printUFO();

void printLives();

void printLevel();

void printAllObjects();

void drawGameScreen();

void drawStartScreen();

void drawEndScreen();

void display(void);

void upLevelGame();

void killAllEnemies();

void endGame();

void shootEnemies(TObject enemy);

void animationDeathSpaceship();

void animationEnemies();

void deslocateBounds(int i);

int collision(TObject shot, TObject target);

int hitAnyEnemy(int indShots);

int hitAnyBlock(TObject shot);

void animationShots();

void animateShootingSpaceship();

void animateUFO();

void shootSpaceship();

void keyboardListener(unsigned char key, int x, int y);

void keyboardSpecialListener(int key, int x, int y);

void reshape(GLsizei w, GLsizei h);

void anima();

/********************************************************/
/*************** VARIAVEIS GLOBAIS **********************/
/********************************************************/

// Variaveis de Textura
GLuint texture_id;

// Variaveis de Tela
int enableEndGame = 1;
int currentScreen = START_SCREEN;
int beginning = 1;
int beginningTextures = 1;
int vision3D = -1;

// Variaveis de Movimento dos Inimigos
int leftBoundEnemy = LEFT_BOUND_ENEMY;
int rightBoundEnemy = RIGHT_BOUND_ENEMY;
int xLogicEnemiesSet = 2;
int indBoundColumnLeft = 0;
int indBoundColumnRight = ENEMIES_X_LOGIC_MAX - 1;
int enemiesInColumn[ENEMIES_X_LOGIC_MAX];
int dirEnemy = DIR_ENEMY;

// Variaveis de Tiro
int numSpaceshipShots = 0;
int numEnemiesShots = 0;
int maxShotsEnemies = INIT_MAX_SHOTS_ENEMIES;

// Variaveis de Jogo
int points=0;
int level=0;
int currentLives = MAX_LIVES - 1;

// Variavies de Controle
int totalEnemies = ENEMIES_X_LOGIC_MAX * ENEMIES_UP_BOUND_MAX;

// Variaveis de Consulta
int enemyKindRow[ENEMIES_UP_BOUND_MAX] = {1, 1, 2, 2, 3, 3};
int pointPerLine[ENEMIES_UP_BOUND_MAX] = {10, 10, 20, 20, 30, 30};

/**********************************************/
/*************** OBJETOS **********************/
/**********************************************/

TObject enemiesSet[ENEMIES_X_LOGIC_MAX][ENEMIES_UP_BOUND_MAX]; // vetor de inimigos
TObject blocks[BLOCKS_X_LOGIC_MAX][BLOCKS_UP_BOUND_MAX];
TObject spaceShip;
TObject shotsSpaceship[SHOTS_SPACESHIP_MAX];
TObject shotsEnemies[MAX_SHOTS_ENEMIES];
TObject ufo;
TObject lives[MAX_LIVES];

/*******************************************/
/*************** MAIN **********************/
/*******************************************/

int main(int argc, char** argv){

    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Space Invaders");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(keyboardSpecialListener);
    glutReshapeFunc(reshape);
    glutTimerFunc(ANIMATE_TIME,animationEnemies,0);
    glutTimerFunc(ANIMATE_TIME_SHOTS,animationShots,0);
    glutTimerFunc(ANIMATE_TIME, animateShootingSpaceship, 0);
    glutTimerFunc(ANIMATE_TIME, animateUFO, 0);
    glutIdleFunc(anima);
    glutMainLoop();
    return 0;
}

/******************************************************/
/*************** IMPELEMENTAÇÕES **********************/
/******************************************************/

void getBitmapImageData( char *pFileName, BMPImage *pImage ) {
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
    int i;
    if( (pFile = fopen(pFileName, "rb") ) == NULL )
        printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);
        fseek( pFile, 18, SEEK_CUR );
        if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
            printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n", pFileName);
        if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
            printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n", pFileName);
        if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
            printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);
        if( nNumPlanes != 1 )
            printf("ERROR: getBitmapImageData - Plane count from %s.\n ", pFileName);
        if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
            printf( "ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ", pFileName);
        if( nNumBPP != 24 )
            printf("ERROR: getBitmapImageData - BPP from %s.\n ", pFileName);
        fseek( pFile, 24, SEEK_CUR );
        int nTotalImagesize = (pImage->width * pImage->height) * 3;
            pImage->data = (char*) malloc( nTotalImagesize );
        if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
            printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ", pFileName);
        char charTemp;
        for( i = 0; i < nTotalImagesize; i += 3 ) {
            charTemp = pImage->data[i];
            pImage->data[i] = pImage->data[i+2];
            pImage->data[i+2] = charTemp;
        }
}

void loadTexture(char* Filename) {
    BMPImage textura;
    getBitmapImageData( Filename, &textura);
    glGenTextures(1, &texture_id); /* 1 = uma textura; */
    glBindTexture ( GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, 3, textura.width, textura.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textura.data);
}

void initBackgroundTexture (void) {
    glEnable ( GL_TEXTURE_2D );
    loadTexture("textures/background.bmp");
}

void initIntroTexture (void) {
    glEnable ( GL_TEXTURE_2D );
    loadTexture("textures/intro.bmp");
}

void initGameOverTexture (void) {
    glEnable ( GL_TEXTURE_2D );
    loadTexture("textures/gameOver.bmp");
}

void initStartGlobalParameters() {
    currentLives = MAX_LIVES - 1;
    dirEnemy = DIR_ENEMY;
    leftBoundEnemy = LEFT_BOUND_ENEMY;
    rightBoundEnemy = RIGHT_BOUND_ENEMY;
    xLogicEnemiesSet = 2;
    numSpaceshipShots = 0;
    numEnemiesShots = 0;
    points=0;
    totalEnemies = ENEMIES_X_LOGIC_MAX * ENEMIES_UP_BOUND_MAX;
    maxShotsEnemies = INIT_MAX_SHOTS_ENEMIES;
    enemiesInColumn[ENEMIES_X_LOGIC_MAX];
    indBoundColumnLeft = 0;
    indBoundColumnRight = ENEMIES_X_LOGIC_MAX - 1;
}

void initLevelGlobalParameters() {

    currentLives = MAX_LIVES - 1;

    dirEnemy = DIR_ENEMY;
    leftBoundEnemy = LEFT_BOUND_ENEMY;
    rightBoundEnemy = RIGHT_BOUND_ENEMY;
    xLogicEnemiesSet = 2;

    numSpaceshipShots = 0;
    numEnemiesShots = 0;

    totalEnemies = ENEMIES_X_LOGIC_MAX * ENEMIES_UP_BOUND_MAX;

    if(maxShotsEnemies < MAX_SHOTS_ENEMIES) {
        maxShotsEnemies += LEVEL_MAX_SHOTS_ENEMIES;
    }

    enemiesInColumn[ENEMIES_X_LOGIC_MAX];
    indBoundColumnLeft = 0;
    indBoundColumnRight = ENEMIES_X_LOGIC_MAX - 1;
}

FILE* initFile(FILE* objectFile, int kind) {

    char fileName[30];

    switch(kind) {
        case 0:
            sprintf(fileName,"objects/%s.object", "spaceShip");
            break;
        case 1:
            sprintf(fileName,"objects/%s.object", "enemy1");
            break;
        case 2:
            sprintf(fileName,"objects/%s.object", "enemy2");
            break;
        case 3:
            sprintf(fileName,"objects/%s.object", "enemy3");
            break;
        case 4:
            sprintf(fileName,"objects/%s.object", "shot");
            break;
        case 5:
            sprintf(fileName,"objects/%s.object", "shotEnemy");
            break;
        case 6:
            sprintf(fileName,"objects/%s.object", "block");
            break;
        case 7:
            sprintf(fileName,"objects/%s.object", "ufo");
            break;
    }
    return fopen(fileName,"r");
}

TObject initObjectFile(TObject object) {

    FILE* objectFile;
    TCube*** objectMatrix;
    objectFile = initFile(objectFile, object.kind);

    int kindAnimation; // PAra o switch se é animação normal ou de morte
    int amountAnimateCubes;
    int xAnimateCube, yAnimateCube, zAnimateCube;
    int amountTimePositions;

    int i, j, k, printCubo;

    fscanf(objectFile, "%d", &object.xMax);
    fscanf(objectFile, "%d", &object.yMax);
    fscanf(objectFile, "%d", &object.zMax);

    objectMatrix = (TCube***) malloc(sizeof(TCube**) * object.xMax);

    for(i=0; i<object.xMax; i++) {

        objectMatrix[i] = (TCube**) malloc(sizeof(TCube*) * object.yMax);

        for(j=0;j<object.yMax; j++) {
            objectMatrix[i][j] = (TCube*) malloc(sizeof(TCube) * object.zMax);
        }
    }

    // COMECA A LER AS INFORMAÇÕES DE DESENHO
    if(objectFile != NULL) {
        for(i = 0; i<object.yMax; i++) {
            for(j=0; j<object.zMax; j++) {
                for(k=0; k<object.xMax; k++) {
                    fscanf(objectFile, "%d", &printCubo);
                    objectMatrix[k][i][j].color = printCubo;
                    objectMatrix[k][i][j].animateFlag = 0;
                }
            }
        }

    }
    else {
        printf("File not found\n");
    }

    // COMECA A LER ANIMACAO
    object.objectMatrix = objectMatrix;

    fscanf(objectFile, "%d", &kindAnimation);

        fscanf(objectFile, "%d", &amountAnimateCubes);
        object.sizeAnimateCubes = amountAnimateCubes;

        fscanf(objectFile, "%d", &object.animateTime);
        object.currentAnimateTime = 0;

        object.animateCubes = (TCube*) malloc(sizeof(TCube)*amountAnimateCubes);

        for(i=0; i<amountAnimateCubes; i++){

            fscanf(objectFile, "%d", &xAnimateCube);
            fscanf(objectFile, "%d", &yAnimateCube);
            fscanf(objectFile, "%d", &zAnimateCube);
            fscanf(objectFile, "%d", &amountTimePositions);

            object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].animateFlag = 1;
            object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsAnimation = (TTimePosition*) malloc(sizeof(TTimePosition)*amountTimePositions);

            for(j=0; j<amountTimePositions; j++) {
                fscanf(objectFile,"%d", &object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsAnimation[j].x);
                fscanf(objectFile,"%d", &object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsAnimation[j].y);
                fscanf(objectFile,"%d", &object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsAnimation[j].z);
                fscanf(objectFile,"%d", &object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsAnimation[j].time);

            }

            // INSERI NA LISTA DE ANIMAÇÂO
            object.animateCubes[i] = object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube];
        }

    // COMECA A LER ANIMACAO DE MORTE
    fscanf(objectFile, "%d", &kindAnimation);

    fscanf(objectFile, "%d", &amountAnimateCubes);
    object.sizeDeathAnimateCubes = amountAnimateCubes;

    fscanf(objectFile, "%d", &object.deathAnimateTime);
    object.currentDeathAnimateTime = 0;

    object.deathAnimateCubes = (TCube*) malloc(sizeof(TCube)*amountAnimateCubes);

    for(i=0; i<amountAnimateCubes; i++){

        fscanf(objectFile, "%d", &xAnimateCube);
        fscanf(objectFile, "%d", &yAnimateCube);
        fscanf(objectFile, "%d", &zAnimateCube);
        fscanf(objectFile, "%d", &amountTimePositions);

        object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].deathAnimateFlag = 1;
        object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsDeathAnimation = (TTimePosition*) malloc(sizeof(TTimePosition)*amountTimePositions);

        for(j=0; j<amountTimePositions; j++) {
            fscanf(objectFile,"%d", &object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsDeathAnimation[j].x);
            fscanf(objectFile,"%d", &object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsDeathAnimation[j].y);
            fscanf(objectFile,"%d", &object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsDeathAnimation[j].z);
            fscanf(objectFile,"%d", &object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube].timePositionsDeathAnimation[j].time);

        }

        // INSERI NA LISTA DE ANIMAÇÂO DE MORTE
        object.deathAnimateCubes[i] = object.objectMatrix[xAnimateCube][yAnimateCube][zAnimateCube];
    }
    fclose(objectFile);

    return object;
}

TObject initObject(int kind) {

    TObject object;
    object.kind = kind;
    object.alive = 1;
    object = initObjectFile(object);
    return object;
}

TObject initLogicObject(TObject object, int xLogic, int yLogic, int zLogic) {

    // INFORMACAO LOGICA
    object.xLogic = xLogic;
    object.yLogic = yLogic;
    object.zLogic = zLogic;
    return object;
}

void initEnemies() {
    int i, j;
    for(i=0; i<ENEMIES_X_LOGIC_MAX; i++) {
        for(j=0; j<ENEMIES_UP_BOUND_MAX; j++) {

            enemiesSet[i][j] = initObject(enemyKindRow[j]);
            enemiesSet[i][j] = initLogicObject(enemiesSet[i][j], X_INIT_ENEMIES + (i*ENEMIES_X_SPLIT_PIXELS), Y_INIT_ENEMIES + (j*ENEMIES_Y_SPLIT_PIXELS), 0);
            enemiesSet[i][j].inAnimation = 1;

        }
    }

    for(i=0; i<ENEMIES_X_LOGIC_MAX; i++) {
        enemiesInColumn[i] = ENEMIES_UP_BOUND_MAX;
    }

}

void initSpaceShip() {
    spaceShip = initObject(0);
    spaceShip = initLogicObject(spaceShip, X_INIT_SPACESHIP, Y_INIT_SPACESHIP,0);
    spaceShip.inAnimation = 0;
    spaceShip.inAgony = 0;
}

void initShots() {
    int i;
    for(i=0; i<SHOTS_SPACESHIP_MAX; i++) {
        shotsSpaceship[i] = initObject(4);
        shotsSpaceship[i].alive = 0;
    }
    for(i=0; i<maxShotsEnemies; i++) {
        shotsEnemies[i] = initObject(5);
        shotsEnemies[i].alive = 0;
    }
}

void initBlocks() {
    int i, j;
    for(i=0; i<BLOCKS_X_LOGIC_MAX; i++) {
        for(j=0; j<BLOCKS_UP_BOUND_MAX; j++) {
            blocks[i][j] = initObject(6);
            blocks[i][j] = initLogicObject(blocks[i][j], X_INIT_BLOCKS + (i*blocks[i][j].xMax) + ((i/BLOCKS_SETS)*BLOCKS_SPLIT_X*blocks[i][j].xMax), Y_INIT_BLOCKS + (j*blocks[i][j].yMax), 0);
        }
    }

}

void initUFO() {
    ufo = initObject(7);
    ufo.alive = 0;
    ufo.inAgony = 0;
}

void initLives() {
    int i;
    for(i=0; i<MAX_LIVES; i++) {
        lives[i] = initObject(0);
        lives[i] = initLogicObject(lives[i], X_INIT_LIVES + i*10, Y_INIT_LIVES, 0);
    }
}

void initAllObjects() {

    initEnemies();
    initSpaceShip();
    initShots();
    initBlocks();
    initUFO();
    initLives();
    initBackgroundTexture();
}

void setCubeColor(int colorIndex) {
    switch(colorIndex) {
        case 1:
            glColor3ub(255,0,0);
            break;
        case 2:
            glColor3ub(190,190,190);
            break;
        case 3:
            glColor3ub(0,0, 255);
            break;
        case 4:
            glColor3ub(255,255, 0);
            break;
        case 5:
            glColor3ub(130,0, 255);
            break;
        case 6:
            glColor3ub(255,165,0);
            break;
        case 7:
            glColor3ub(0,165,0);
            break;
        case 8:
            glColor3ub(225, 225,225);
            break;
        case 9:
            glColor3ub(139, 69, 19);
            break;
    }
}

void printObject(TObject object) {
    int i,j, k;

    glPushMatrix();

        glTranslatef(object.xLogic*CUBE_PIXELS, object.yLogic*CUBE_PIXELS, 0);
        for(i=0; i<object.yMax; i++) { // Y
            for(j=0; j<object.zMax; j++) { // Z
                for(k=0; k<object.xMax; k++) { // X
                    if(object.objectMatrix[k][i][j].color > 0) {

                        glPushMatrix();

                                if(object.inAgony == 1 && object.objectMatrix[k][i][j].deathAnimateFlag == 1) {
                                     glTranslatef(object.objectMatrix[k][i][j].timePositionsDeathAnimation[0].x*CUBE_PIXELS,
                                                 object.objectMatrix[k][i][j].timePositionsDeathAnimation[0].y*CUBE_PIXELS,
                                                 -object.objectMatrix[k][i][j].timePositionsDeathAnimation[0].z*CUBE_PIXELS);
                                }
                                else {
                                    if(object.objectMatrix[k][i][j].animateFlag == 1 && object.inAnimation == 1) {
                                        glTranslatef(object.objectMatrix[k][i][j].timePositionsAnimation[object.currentAnimateTime-1].x*CUBE_PIXELS,
                                                     object.objectMatrix[k][i][j].timePositionsAnimation[object.currentAnimateTime-1].y*CUBE_PIXELS,
                                                     -object.objectMatrix[k][i][j].timePositionsAnimation[object.currentAnimateTime-1].z*CUBE_PIXELS);

                                    }
                                    else {
                                        glTranslatef(k*CUBE_PIXELS,i*CUBE_PIXELS,j*CUBE_PIXELS);
                                    }
                                }

                            glColor3f(0,0,0);
                            glutWireCube(CUBE_PIXELS);

                            setCubeColor(object.objectMatrix[k][i][j].color);

                            glutSolidCube(CUBE_PIXELS);
                        glPopMatrix();
                    }
                }
            }
        }
    glPopMatrix();
}

void printEnemies() {
    int i,j;
    for(i=0; i<ENEMIES_X_LOGIC_MAX; i++) {
        for(j=0; j<ENEMIES_UP_BOUND_MAX; j++) {
            if(enemiesSet[i][j].alive == 1) {
                printObject(enemiesSet[i][j]);
            }
        }
    }
}

void printSpaceShip() {
    printObject(spaceShip);
}

void printShots() {
    int i;
    for(i=0; i<SHOTS_SPACESHIP_MAX; i++) {
        if(shotsSpaceship[i].alive == 1) {
            printObject(shotsSpaceship[i]);
        }
    }
    for(i=0; i<maxShotsEnemies; i++) {
        if(shotsEnemies[i].alive == 1) {
            printObject(shotsEnemies[i]);
        }
    }
}

void printBlocks() {

    int i, j;
    for(i=0; i<BLOCKS_X_LOGIC_MAX; i++) {
        for(j=0; j<BLOCKS_UP_BOUND_MAX; j++) {
            if(blocks[i][j].alive == 1) {
                printObject(blocks[i][j]);
            }
        }
    }

}

void renderBitmapString(float x, float y, float z, void *font, char *string) {
    char *c;
    glRasterPos3f(x, y,z);

    for(c = string; *c != '\0'; c++)
    glutBitmapCharacter(font, *c);
}

void printPoints() {
    char pointsString[30], pointsStringNumeric[30];
    sprintf(pointsString, "Pontos:");
    glColor3ub(255,255,0);
    renderBitmapString(10, 580, -10, GLUT_BITMAP_HELVETICA_18, pointsString);
    sprintf(pointsStringNumeric, "%d", points);
    glColor3ub(255,255,255);
    renderBitmapString(80, 580, -10, GLUT_BITMAP_HELVETICA_18, pointsStringNumeric);
}

void printGameOverPoints() {
    char pointsString[30], pointsStringNumeric[30];
    sprintf(pointsStringNumeric, "%d", points);
    glPushMatrix();
        glColor3ub(255,255,255);
        renderBitmapString(362, 250, -10, GLUT_BITMAP_TIMES_ROMAN_24, pointsStringNumeric);
    glPopMatrix();
}

void printTexture() {
    glPushMatrix();
        glTranslatef(0,0,-300);
        glEnable(GL_TEXTURE_2D);
        glBindTexture ( GL_TEXTURE_2D, texture_id );
        glBegin ( GL_QUADS );
            glTexCoord2f(0, 0); glVertex3f(0, 0,  -1);
            glTexCoord2f(1, 0); glVertex3f( 800, 0,  -1);
            glTexCoord2f(1, 1); glVertex3f( 800,  600,  -1);
            glTexCoord2f(0, 1); glVertex3f(0,  600,  -1);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void printUFO() {
    if(ufo.alive == 1) {
        printObject(ufo);
    }
}

void printLives() {
    int i;
    char lifeString[30];

    sprintf(lifeString, "Vidas: ");
    glColor3ub(255,255,0);
    renderBitmapString(650, 580, -10, GLUT_BITMAP_HELVETICA_18, lifeString);

    for(i=0; i<MAX_LIVES; i++) {
        if(lives[i].alive == 1) {
            printObject(lives[i]);
        }
    }
}

void printLevel() {
    char levelString[30], levelStringNumeric[30];
    sprintf(levelString, "Nivel:");
    glColor3ub(255,255,0);
    renderBitmapString(350, 580, -10, GLUT_BITMAP_HELVETICA_18, levelString);
    sprintf(levelStringNumeric, "%d", level + 1);
    glColor3ub(255,255,255);
    renderBitmapString(400, 580, -10, GLUT_BITMAP_HELVETICA_18, levelStringNumeric);
}

//DESENHA O OBJETO
void printAllObjects() {

    printEnemies();
    printSpaceShip();
    printShots();
    printBlocks();
    printPoints();
    printUFO();
    printLives();
    printLevel();
    printTexture();

}

void drawGameScreen(){
    if(beginning == 1){
        if(level == 0) {
            initStartGlobalParameters();
        }
        else {
            initLevelGlobalParameters();
        }
        initAllObjects();
        beginning = 0;
        enableEndGame = 1;
    }
    printAllObjects();
}

void drawStartScreen(){
    if(beginningTextures == 1) {
        initIntroTexture();
        beginningTextures = 0;
    }
    printTexture();
}

void drawEndScreen(){

    if(beginningTextures == 1) {
        initGameOverTexture();
        beginningTextures = 0;
    }
    printTexture();
    printGameOverPoints();
}

void display(void){
    glClearColor (0,0,0,0.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    switch(currentScreen){

        case START_SCREEN:
            drawStartScreen();
            break;
        case GAME_SCREEN:
            drawGameScreen();
            break;
        case END_SCREEN:
            drawEndScreen();
    }
    glutSwapBuffers();
}

void upLevelGame() {
    beginning = 1;
    level++;
    points+=level*POINTS_PER_LINE;
    enableEndGame = 0;
}

void killAllEnemies() {
    int i, j;
    for(i=0; i<ENEMIES_X_LOGIC_MAX; i++) {
        for(j=0; j<ENEMIES_UP_BOUND_MAX; j++) {
            enemiesSet[i][j].alive = 0;
        }
    }
}

void endGame() {
    currentScreen = END_SCREEN;
    beginningTextures = 1;
    level = 0;
    beginning = 1;
    enableEndGame = 0;
    killAllEnemies();
}

void shootEnemies(TObject enemy) {
    int i;
    int probShootEnemy = (rand()%100) + 1;
    if(probShootEnemy < PROB_SHOOT_ENEMY) {
        for(i=0; i<maxShotsEnemies; i++) {

            if(shotsEnemies[i].alive == 0) {
                shotsEnemies[i] = initLogicObject(shotsEnemies[i], enemy.xLogic, enemy.yLogic, 0);
                shotsEnemies[i].alive = 1;
                numEnemiesShots++;
                break;
            }
        }
    }

}

void animationDeathSpaceship() {
    spaceShip.currentDeathAnimateTime++;
    if(spaceShip.currentDeathAnimateTime == spaceShip.deathAnimateTime) {
        spaceShip.currentDeathAnimateTime = 0;
        spaceShip.inAgony = 0;
    }
    else {
        glutTimerFunc(ANIMATE_TIME,animationDeathSpaceship,0);
    }
}

void animationEnemies() {

    int i, j, hitBound = 0;
    int probShootEnemy;

    xLogicEnemiesSet += dirEnemy;

    if(xLogicEnemiesSet >= rightBoundEnemy || xLogicEnemiesSet <= leftBoundEnemy) {
        dirEnemy = dirEnemy * -1;
        hitBound = -STEP_ENEMY_Y;
    }

    for(i=0; i<ENEMIES_X_LOGIC_MAX; i++){
        for(j=0; j<ENEMIES_UP_BOUND_MAX; j++) {

            if(enemiesSet[i][j].alive == 1) {
                if(enemiesSet[i][j].yLogic <= spaceShip.yLogic) {
                    if(enableEndGame == 1) {
                        endGame();
                    }
                }

                //Tiro do Inimigo
                shootEnemies(enemiesSet[i][j]);

                if(enemiesSet[i][j].inAgony == 0) {
                    // Animação do Inimigo
                    if(enemiesSet[i][j].currentAnimateTime == enemiesSet[i][j].animateTime) {
                        enemiesSet[i][j].currentAnimateTime = 1;
                    }
                    else {
                        enemiesSet[i][j].currentAnimateTime++;
                    }
                }
                else {
                    enemiesSet[i][j].currentDeathAnimateTime++;
                    if(enemiesSet[i][j].currentDeathAnimateTime == enemiesSet[i][j].deathAnimateTime) {
                        enemiesSet[i][j].alive = 0;
                    }
                }

                enemiesSet[i][j].xLogic += dirEnemy;
                enemiesSet[i][j].yLogic += hitBound;
            }
        }
    }
    glutTimerFunc(ANIMATE_TIME,animationEnemies,0);
}

void deslocateBounds(int i) {

    if(i == indBoundColumnLeft) {
        if(enemiesInColumn[i] == 0) {
            leftBoundEnemy -= ENEMIES_X_SPLIT_PIXELS;
            indBoundColumnLeft++;
            deslocateBounds(indBoundColumnLeft);
        }
    }

    if(i == indBoundColumnRight) {

       if(enemiesInColumn[i] == 0) {
            rightBoundEnemy += ENEMIES_X_SPLIT_PIXELS;
            indBoundColumnRight--;
            deslocateBounds(indBoundColumnRight);
       }
    }

}

int collision(TObject shot, TObject target) {

    if(shot.kind == 5) { // DESCENDO
        if(shot.xLogic >= target.xLogic && shot.xLogic <= (target.xLogic + target.xMax) &&
            shot.yLogic <= target.yLogic && shot.yLogic <= (target.yLogic + target.yMax) &&
            shot.zLogic >= target.zLogic && shot.zLogic <= (target.zLogic + target.zMax)) {
                return 1;
        }
    }
    else {
        if(shot.xLogic >= target.xLogic && shot.xLogic <= (target.xLogic + target.xMax) &&
            shot.yLogic + shot.yMax >= target.yLogic && shot.yLogic + shot.yMax >= (target.yLogic + target.yMax) &&
            shot.zLogic >= target.zLogic && shot.zLogic <= (target.zLogic + target.zMax)) {
                return 1;
        }
    }
    return 0;
}

int hitAnyEnemy(int indShots) {
    int i, j;
    for(i=0; i<ENEMIES_X_LOGIC_MAX; i++){
        for(j=0; j<ENEMIES_UP_BOUND_MAX; j++) {

            if(enemiesSet[i][j].alive == 1 && enemiesSet[i][j].inAgony == 0) {

                if(collision(shotsSpaceship[indShots], enemiesSet[i][j]) == 1) {
                    enemiesSet[i][j].inAgony = 1;

                    enemiesInColumn[i]--;

                    totalEnemies--;
                    points += pointPerLine[j];

                    if(totalEnemies == 0) {
                        upLevelGame();
                    }

                    deslocateBounds(i);

                    return 1;
                }
            }
        }
    }
    return 0;

}

int hitAnyBlock(TObject shot) {
    int i, j;

    if(shot.kind == 5) {
        for(j=BLOCKS_UP_BOUND_MAX-1; j>=0; j--) {
            for(i=0; i<BLOCKS_X_LOGIC_MAX; i++) {

                if(blocks[i][j].alive == 1) {
                    if(collision(shot, blocks[i][j])) {
                        blocks[i][j].alive = 0;
                        return 1;
                    }
                }
            }
        }
    }
    else {
        for(j=0; j<BLOCKS_UP_BOUND_MAX; j++) {
            for(i=0; i<BLOCKS_X_LOGIC_MAX; i++) {

                if(blocks[i][j].alive == 1) {
                    if(collision(shot, blocks[i][j])) {
                        blocks[i][j].alive = 0;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;

}

void animationShots() {
    int i;
    // Tiros da Nave
    if(numSpaceshipShots > 0) {
        for(i=0; i<SHOTS_SPACESHIP_MAX; i++) {

            if(shotsSpaceship[i].alive == 1) {

                shotsSpaceship[i].yLogic += INC_SHOT_Y;

                if(hitAnyBlock(shotsSpaceship[i])) { // COLISAO BLOCO
                    shotsSpaceship[i].alive = 0;
                    numSpaceshipShots--;
                }
                else {

                    if(collision(shotsSpaceship[i], ufo) && ufo.inAgony == 0) { // COLISAO UFO
                        ufo.inAgony = 1;
                    }
                    else {
                        // FINAL DA TELA || COLISA TIRO E ENEMY
                        if(shotsSpaceship[i].yLogic > UP_BOUND_MAX  || hitAnyEnemy(i) == 1) {
                            shotsSpaceship[i].alive = 0;
                            numSpaceshipShots--;
                        }
                    }
                }
            }
        }
    }

    // Tiros do inimigos
    if(numEnemiesShots > 0) {
        for(i=0; i<maxShotsEnemies; i++) {

            if(shotsEnemies[i].alive == 1) {
                shotsEnemies[i].yLogic -= INC_SHOT_Y;

                if(shotsEnemies[i].yLogic < 0) { // FINAL DA TELA
                    shotsEnemies[i].alive = 0;
                    numEnemiesShots--;
                }
                else {

                    // COLISAO TIRO INIMIGO E NAVE
                    if(collision(shotsEnemies[i], spaceShip) && spaceShip.inAgony == 0) {

                        if(currentLives == -1) {
                            if(enableEndGame == 1) {
                                endGame();
                            }
                        }
                        else {
                            spaceShip.inAgony = 1;
                            lives[currentLives].alive = 0;
                            currentLives--;
                            shotsEnemies[i].alive = 0;
                            glutTimerFunc(ANIMATE_TIME,animationDeathSpaceship,0);
                        }
                    }
                    else {
                        if(hitAnyBlock(shotsEnemies[i])) {
                            shotsEnemies[i].alive = 0;
                            numEnemiesShots--;
                        }
                    }
                }
            }
        }
    }

    glutTimerFunc(ANIMATE_TIME_SHOTS,animationShots,0);
}


void animateShootingSpaceship() {

    if(spaceShip.inAnimation == 1) {
        spaceShip.currentAnimateTime++;

        if(spaceShip.currentAnimateTime == spaceShip.animateTime) {
            spaceShip.inAnimation = 0;
            spaceShip.currentAnimateTime = 0;
        }
    }

    glutTimerFunc(ANIMATE_TIME, animateShootingSpaceship, 0);
}

void animateUFO() {
    int probUFOAlive;
    if(ufo.alive == 0) {
        probUFOAlive = (rand()%100) + 1;
        if(probUFOAlive <= PROB_UFO) {
            ufo.alive = 1;
            ufo = initLogicObject(ufo, X_INIT_UFO, Y_INIT_UFO, 0);
        }
    }
    else {
        if(ufo.inAgony == 0) {
            ufo.xLogic += STEP_UFO_X;
            if(ufo.xLogic >= RIGHT_BOUND_UFO) {
                ufo.alive = 0;
            }
        }
        else {
            ufo.currentDeathAnimateTime++;
            if(ufo.currentDeathAnimateTime == ufo.deathAnimateTime) {
                ufo.alive = 0;
                ufo.inAgony = 0;
                ufo.currentDeathAnimateTime = 0;
                points+= UFO_POINT;
            }
        }
    }
    glutTimerFunc(ANIMATE_TIME, animateUFO, 0);
}


void shootSpaceship() {
    int i;
    for(i=0; i<SHOTS_SPACESHIP_MAX; i++) {
        if(shotsSpaceship[i].alive == 0) {

            if(shotsSpaceship[i].kind == 4 && shotsSpaceship[i+1].kind == 4) {
                shotsSpaceship[i] = initLogicObject(shotsSpaceship[i], spaceShip.xLogic + INIT_FIRST_SHOT_X, INIT_SHOT_Y, 0);
                shotsSpaceship[i].alive = 1;
                shotsSpaceship[i+1] = initLogicObject(shotsSpaceship[i+1], spaceShip.xLogic + INIT_SECOND_SHOT_X, INIT_SHOT_Y, 0);
                shotsSpaceship[i+1].alive = 1;
                numSpaceshipShots+=2;
                spaceShip.inAnimation = 1;
                break;
            }

        }
    }

}

void keyboardListener(unsigned char key, int x, int y) {

    switch(key) {
        case 32:
        case GLUT_KEY_UP:
        case 'w':
        case 'W':
            shootSpaceship();
            break;
        case 13:
            currentScreen = GAME_SCREEN;
            break;

        // Case para apresentação - Passar de Level
        case 'l':
            totalEnemies = 1;
            break;

        // Case para apresentação - Morrer rapidamente
        case 'm':
            currentLives = -1;
            break;

        // Case para apresentação - Visão 3D
        case 'v':
            vision3D *= -1;
            break;
    }
}

void keyboardSpecialListener(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_LEFT:
            if (spaceShip.xLogic > LEFT_BOUND_SPACESHIP) {
                spaceShip.xLogic -= SPACESHIP_STEP;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (spaceShip.xLogic < RIGHT_BOUND_SPACESHIP) {
                spaceShip.xLogic += SPACESHIP_STEP;
            }
            break;
    }
}

void reshape(GLsizei w, GLsizei h) {
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1000, 1000);

	if(vision3D == 1) {
        gluLookAt(0,100,200, 0,10,10, 0,1,0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void anima() {
    glutPostRedisplay();
}
