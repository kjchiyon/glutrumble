//game.hpp by CHEONG KAI JIN, CHONG ZHENG CHENG, KANG XUAN XIN, KAY HEAN CUN
//game ui, logic, controls

#include <cmath>
#include <iostream>

#include "tools.hpp"
#include "env.hpp"

//character headers here
#include "characters/voltorb.hpp"
#include "characters/doraemon.hpp"
#include "characters/chickenjockey.hpp"

int fms = 10; //ms per frame
float gamespeed = 1; //delta time multiplier
//game vars
unsigned int gamestate = 0; //0=spec,1=battle,2=result
float gametime = 0; //incremental gametime
float timer1 = 99;
bool isbotp2 = true; //toggle computer play as p2
bool isfreeview = false; //toggle booleans
bool ismutesound = false; //toggle sounds
bool isgodmode = false; //toggle stat change
bool isaxis = false; //toggle drawaxis
bool iswire = false; //toggle wireframe
//camera vars
float azimuth = 0.0f;
float elevation = 30.0f;
float distance = 10.0f;
float msens = 0.3f;
float panspeed = 0.2f;
Vector3f panoffset = {0.0f, 0.0f, 0.0f};
Vector3f eye = {0.0f, 0.0f, 0.0f};
int lastmx, lastmy;
bool rotating = false;

//!init characters here
ChickenJockey cjp1;
ChickenJockey cjp2;
Doraemon drp1;
Doraemon drp2;
Voltorb vtp1;
Voltorb vtp2;
//!body* array that stores models for each player side, must symmetric
Body* p1bodyArr[] = {&vtp1, &drp1, &cjp1};
Body* p2bodyArr[] = {&vtp2, &drp2, &cjp2};
std::string playernames[] = {"Voltorb", "Doraemon", "Chicken Jockey"};
int pbarrsize = 3; //player body arr size

//player vars
int inithealth = 3; //values to start with
int initmana = 2;
int p1health; //player current values
int p2health;
int p1mana;
int p2mana;
int p1idx = 0; //character selection index
int p2idx = 0;
int p1skill = 1; //which skill player selected
int p2skill = 1;
int winplayer = 0; //0= draw,1= p1 win, 2= p2 win

//delta time update
void updateGame(float delta) {
    gametime += delta;
    timer1 += delta;
    updateEnv(delta);
    p1bodyArr[p1idx]->updateBody(delta);
    p2bodyArr[p2idx]->updateBody(delta);
    if (gamestate == 2 || winplayer != 0) updateConfetti(delta);
}

//camera
void setCamera() {
    if (isfreeview) { //freeview cam
        float pi = 3.14159;
        float azirad = azimuth * pi / 180;
        float elerad = elevation * pi / 180;
        eye.x = distance * cosf(elerad) * sinf(azirad) + panoffset.x;
        eye.y = distance * sinf(elerad) + panoffset.y;
        eye.z = distance * cosf(elerad) * cosf(azirad) + panoffset.z;
        gluLookAt(eye.x, eye.y, eye.z,
                  panoffset.x, panoffset.y, panoffset.z,
                  0.0f, 1.0f, 0.0f);
    } else
    if (gamestate == 0) {
        eye.x = sin(gametime*0.5) * 20;
        eye.y = (sin(gametime*0.2)+1) * 5 + 1;
        eye.z = cos(gametime*0.5) * 20;
        gluLookAt(eye.x, eye.y, eye.z,
                  0, 2, 0,
                  0, 1, 0);
    } else
    if (gamestate == 1) {
        eye.x = sin(gametime*0.5) * 15;
        eye.y = 3;
        eye.z = cos(gametime*0.5) * 15;
        gluLookAt(eye.x, eye.y, eye.z,
                  0, 1, 0,
                  0, 1, 0);
    } else
    if (gamestate == 2) {
        float t = sinf(std::min(timer1 / 3.0f, 1.0f) * 1.570796);
        eye.x = 0;
        eye.y = lerp(10, 3, t) + sin(gametime*0.5)*0.1;
        eye.z = lerp(30, 10, t) ;
        gluLookAt(eye.x, eye.y, eye.z,
                  0, 1.5, 0,
                  0, 1, 0);
    }
}

//light positions, align with court lights
GLfloat lightPos0[] = {-8.6f, 6.6f, -4.8f, 0};
GLfloat lightPos1[] = {8.6f, 6.6f, -4.8f, 0};
GLfloat lightPos2[] = {-8.6f, 6.6f, 4.8f, 0};
GLfloat lightPos3[] = {8.6f, 6.6f, 4.8f, 0};

//set light position before all draw calls
void setLights() {
    //lights
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
    //material setup
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

//gui
void drawGui() {
    begin2D();
    if (gamestate == 0) {
        float bannerbob = sin(timer1+20) * 4;
        float bannersize = (sin(timer1*0.8+12.5) + 2) * 0.05 + 1.5;
        float xoffset = 400 * (1 - bannersize);
        float yoffset = 100 * (1 - bannersize);
        draw2DTexture(grlogo.texid, xoffset, 400+bannerbob+yoffset, 800*bannersize, 200*bannersize);
        drawBmGlowText("Player 1:", 5, 85, 1, 1, 1, 0.5, 0, 0);
        drawBmGlowText("Player 2:", 5, 55, 1, 1, 1, 0, 0, 0.5);
        std::string p1name = playernames[p1idx];
        std::string p2name = playernames[p2idx];
        drawBmGlowText(p1name.c_str(), 5, 70, 1, 1, 1, 0.5, 0, 0);
        drawBmGlowText(p2name.c_str(), 5, 40, 1, 1, 1, 0, 0, 0.5);
        float br = (sin(timer1*2) * 0.25) + 0.75;
        drawBmGlowText("press N or M to change player characters", 240, 120, 1*br, 1*br, 1*br, 0.5*br, 0.5*br, 0.8*br);
        drawBmGlowText("press SPACE to start", 316, 80, 1*br, 1*br, 1*br, 0.0*br, 0.8*br, 0.5*br);
    } else
    if (gamestate == 1) {
        //show player stats
        //draw bg
        glBindTexture(GL_TEXTURE_2D, 0);
        glBegin(GL_QUADS);
        glColor4f(0.5, 0, 0, 0.5);
        glVertex2f(0, 600);
        glVertex2f(0, 550);
        glVertex2f(150, 550);
        glVertex2f(200, 600);
        glColor4f(0, 0, 0.5, 0.5);
        glVertex2f(800, 600);
        glVertex2f(600, 600);
        glVertex2f(650, 550);
        glVertex2f(800, 550);
        glEnd();
        //draw info
        draw2DTexture(lifetex.texid, 30, 560, 16, 16);
        draw2DTexture(manatex.texid, 98, 560, 16, 16);
        draw2DTexture(lifetex.texid, 664, 560, 16, 16);
        draw2DTexture(manatex.texid, 730, 560, 16, 16);
        drawBmGlowText("Health", 30, 582, 1, 1, 1, 0, 0, 0);
        drawBmGlowText("Mana", 98, 582, 1, 1, 1, 0, 0, 0);
        drawBmGlowText("Health", 664, 582, 1, 1, 1, 0, 0, 0);
        drawBmGlowText("Mana", 730, 582, 1, 1, 1, 0, 0, 0);
        drawBmGlowText(std::to_string(p1health).c_str(), 51, 564, 1, 1, 1, 0, 0, 0);
        drawBmGlowText(std::to_string(p1mana).c_str(), 119, 564, 1, 1, 1, 0, 0, 0);
        drawBmGlowText(std::to_string(p2health).c_str(), 685, 564, 1, 1, 1, 0, 0, 0);
        drawBmGlowText(std::to_string(p2mana).c_str(), 753, 564, 1, 1, 1, 0, 0, 0);
        //draw player names
        std::string p1name = "(P1) " + playernames[p1idx];
        std::string p2name = "(P2) " + playernames[p2idx];
        float p2x = 795 - p2name.size() * 8;
        drawBmGlowText(p1name.c_str(), 5, 535, 1, 1, 1, 0.5, 0, 0);
        drawBmGlowText(p2name.c_str(), p2x, 535, 1, 1, 1, 0, 0, 0.5);
        //draw player skill desc
        if (timer1 > 3) {
            std::string *p1desc = p1bodyArr[p1idx]->desc;
            std::string *p2desc = p2bodyArr[p2idx]->desc;
            std::stringstream ss;
            int texthi = 165;
            drawBmGlowText("Player 1 skills:", 5, texthi, 1, 1, 1, 0.5, 0, 0);
            texthi -= 15;
            ss << "1 - " << p1desc[0];
            drawBmGlowText(ss.str().c_str(), 5, texthi, 1, 1, 1, 0.5, 0, 0);
            texthi -= 15;
            ss.str("");
            ss << "2 - " << p1desc[1];
            drawBmGlowText(ss.str().c_str(), 5, texthi, 1, 1, 1, 0.5, 0, 0);
            texthi -= 15;
            ss.str("");
            ss << "3 - " << p1desc[2];
            drawBmGlowText(ss.str().c_str(), 5, texthi, 1, 1, 1, 0.5, 0, 0);
            texthi -= 20;
            ss.str("");
            drawBmGlowText("Player 2 skills:", 5, texthi, 1, 1, 1, 0, 0, 0.5);
            texthi -= 15;
            ss << "left  - " << p2desc[0];
            drawBmGlowText(ss.str().c_str(), 5, texthi, 1, 1, 1, 0, 0, 0.5);
            texthi -= 15;
            ss.str("");
            ss << "down  - " << p2desc[1];
            drawBmGlowText(ss.str().c_str(), 5, texthi, 1, 1, 1, 0, 0, 0.5);
            texthi -= 15;
            ss.str("");
            ss << "right - " << p2desc[2];
            drawBmGlowText(ss.str().c_str(), 5, texthi, 1, 1, 1, 0, 0, 0.5);
            texthi -= 15;
            ss.str("");
            float br = (sin(timer1*2) * 0.25) + 0.75;
            drawBmGlowText("press SPACE to confirm turn", 292, 50, 1*br, 1*br, 1*br, 0.5*br, 0.8*br, 0.5*br);
            if (isbotp2) {
                //notif user bot is controlling p2
                std::string msg = "computer is controlling p2";
                drawBmText(msg.c_str(), 5, 35, 1, 1, 1);
            }
            if (isgodmode) {
                //notif user bot is controlling p2
                std::string msg = "godmode activated";
                drawBmText(msg.c_str(), 5, 20, 1, 1, 1);
            }
        }
    } else
    if (gamestate == 2) {
        float br = (sin(timer1*2) * 0.25) + 0.75;
        drawBmGlowText("press SPACE to return to title screen", 252, 50, 1*br, 1*br, 1*br, 0.5*br, 0.8*br, 0.5*br);
        float bannerbob = sin(timer1+20) * 4;
        float bannersize = (sin(timer1*2.2+12.5) + 2) * 0.05 + 1;
        float xoffset = 400 * (1 - bannersize);
        float yoffset = 100 * (1 - bannersize);
        if (winplayer == 0) {
            draw2DTexture(pdrawtex.texid, xoffset, 400+bannerbob+yoffset, 800*bannersize, 200*bannersize);
        } else
        if (winplayer == 1) {
            draw2DTexture(p1wintex.texid, xoffset, 400+bannerbob+yoffset, 800*bannersize, 200*bannersize);
        } else
        if (winplayer == 2) {
            draw2DTexture(p2wintex.texid, xoffset, 400+bannerbob+yoffset, 800*bannersize, 200*bannersize);
        }
    }
    if (isfreeview) {
        //notif user in freeview mode and info
        std::stringstream fvmsg;
        fvmsg << "freeview activated, x=" << panoffset.x << ", y=" << panoffset.y << ", z=" << panoffset.z << ", d=" << distance;
        drawBmText(fvmsg.str().c_str(), 5, 5, 1, 1, 1);
    }
    end2D();
}

//gamestate 0 = title scene
void drawGs0Scene() {
    glPushMatrix();
    glTranslatef(-4,0,0);
    p1bodyArr[p1idx]->draw();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(4,0,0);
    glRotatef(180, 0, 1, 0);
    p2bodyArr[p2idx]->draw();
    glPopMatrix();
}

//gamestate 1 = battle scene
void drawGs1Scene() {
    glPushMatrix();
    glTranslatef(-4,0,0);
    p1bodyArr[p1idx]->draw();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(4,0,0);
    glRotatef(180, 0, 1, 0);
    p2bodyArr[p2idx]->draw();
    glPopMatrix();
}

//gamestate 2 = result scene
void drawGs2Scene() {
    glPushMatrix();
    if (winplayer == 0) {
        glTranslatef(-2,0,0);
        glRotatef(-80, 0, 1, 0);
        p1bodyArr[p1idx]->draw();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(2,0,0);
        glRotatef(-100, 0, 1, 0);
        p2bodyArr[p2idx]->draw();
    } else
    if (winplayer == 1) {
        glTranslatef(-0.5,0,3);
        glRotatef(-85, 0, 1, 0);
        p1bodyArr[p1idx]->draw();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(1,0,0);
        glRotatef(-100, 0, 1, 0);
        p2bodyArr[p2idx]->draw();
    } else
    if (winplayer == 2) {
        glTranslatef(-1,0,0);
        glRotatef(-80, 0, 1, 0);
        p1bodyArr[p1idx]->draw();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.5,0,3);
        glRotatef(-95, 0, 1, 0);
        p2bodyArr[p2idx]->draw();
    }
    if (winplayer != 0) drawConfetti();
    glPopMatrix();
}

void drawAxis() {
    glPushMatrix();
    glTranslatef(panoffset.x, panoffset.y, panoffset.z);
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_LINES);
    glColor3f(1,0,0); //x
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glColor3f(0,1,0); //y
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    glColor3f(0,0,1); //z
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

//draw scene
void drawScene() {
    drawEnv();
    if (gamestate == 0) {
        drawGs0Scene();
    } else
    if (gamestate == 1) {
        drawGs1Scene();
    } else
    if (gamestate == 2) {
        drawGs2Scene();
    }
    if (isaxis) {
        drawAxis();
    }
}

//draw which gamestate
void drawGame() {
    setCamera();
    setLights();
    drawScene();
    drawGui();
}

//enter gamestate 0
void enterGs0() {
    gamestate = 0;
    p1bodyArr[p1idx]->playAnim(0);
    p2bodyArr[p2idx]->playAnim(0);
    playBgm("materials/game/carnival.wav");
}

//enter gamestate 1
void enterGs1() {
    gamestate = 1;
    timer1 = 99;
    p1health = inithealth;
    p1mana = initmana;
    p2health = inithealth;
    p2mana = initmana;
    p1bodyArr[p1idx]->playAnim(0);
    p2bodyArr[p2idx]->playAnim(0);
    playBgm("materials/game/jungle.wav");
}

//enter gamestate 2
void enterGs2() {
    gamestate = 2;
    timer1 = 0;
    if (winplayer == 0) {
        p1bodyArr[p1idx]->playAnim(6);
        p2bodyArr[p2idx]->playAnim(6);
    } else
    if (winplayer == 1) {
        p1bodyArr[p1idx]->playAnim(5);
        p2bodyArr[p2idx]->playAnim(6);
    } else
    if (winplayer == 2) {
        p1bodyArr[p1idx]->playAnim(6);
        p2bodyArr[p2idx]->playAnim(5);
    }
    if (winplayer == 0) playBgm("materials/game/fade.wav"); //draw
    else playBgm("materials/game/party.wav"); //win
    clearConfetti();
}

void checkWin(int) {
    if (gamestate == 2) return;
    if (p1health <= 0 && p2health <= 0) {
        winplayer = 0;
        enterGs2();
    } else
    if (p2health <= 0) {
        winplayer = 1;
        enterGs2();
    } else
    if (p1health <= 0) {
        winplayer = 2;
        enterGs2();
    }
}

//bot p2 logic
void botP2() {
    //select skill based on self and opp stats
    if (p2mana <= 0) {
        if (p1mana > 0) {
            p2skill = randi(2, 3);
        } else {
            p2skill = 3;
        }
    } else {
        if (p1mana <= 0) {
            if (randi(0, 1)) {
                p2skill = 1;
            } else {
                p2skill = 3;
            }
        } else {
            p2skill = randi(1, 3);
        }
    }
}

//!game turn logic here !!!
void playTurn() {
    timer1 = 0;
    p1bodyArr[p1idx]->playAnim(0);
    p2bodyArr[p2idx]->playAnim(0);
    //bot choose skill
    if (isbotp2) botP2();
    //update turn
    switch (p1skill) {
    case 1:
        if (p1mana > 0) {
            p1bodyArr[p1idx]->playAnim(1);
            if (!isgodmode) {
                p1mana--;
                if (p2skill == 1) p2health--;
                else if (p2skill == 3) p2health -= 2;
            }
        } else {
            p1bodyArr[p1idx]->playAnim(4);
        }
        break;
    case 2:
        p1bodyArr[p1idx]->playAnim(2);
        break;
    case 3:
        p1bodyArr[p1idx]->playAnim(3);
        if (!isgodmode) p1mana++;
        break;
    }
    switch (p2skill) {
    case 1:
        if (p2mana > 0) {
            p2bodyArr[p2idx]->playAnim(1);
            if (!isgodmode) {
                p2mana--;
                if (p1skill == 1) p1health--;
                else if (p1skill == 3) p1health -= 2;
            }
        } else {
            p2bodyArr[p2idx]->playAnim(4);
        }
        break;
    case 2:
        p2bodyArr[p2idx]->playAnim(2);
        break;
    case 3:
        p2bodyArr[p2idx]->playAnim(3);
        if (!isgodmode) p2mana++;
        break;
    }
    if (p1health < 0) p1health = 0;
    if (p2health < 0) p2health = 0;
    //check win conditions
    glutTimerFunc(3000, checkWin, 0);
}

void moveFreeview(unsigned char key) { //freeview movement
    // camera right vector (perpendicular to forward and up)
    float azirad = azimuth * 3.14159f / 180.0f;
    float rightX = cosf(azirad);
    float rightZ = -sinf(azirad);
    // camera forward vector on XZ plane
    float forwardX = sinf(azirad);
    float forwardZ = cosf(azirad);
    switch (key) {
        case 'a':
            panoffset.x -= rightX * panspeed;
            panoffset.z -= rightZ * panspeed;
            break;
        case 'd':
            panoffset.x += rightX * panspeed;
            panoffset.z += rightZ * panspeed;
            break;
        case 's':
            panoffset.x += forwardX * panspeed;
            panoffset.z += forwardZ * panspeed;
            break;
        case 'w':
            panoffset.x -= forwardX * panspeed;
            panoffset.z -= forwardZ * panspeed;
            break;
        case 'q':
            panoffset.y += panspeed;
            break;
        case 'e':
            panoffset.y -= panspeed;
            break;
        case 'z':
            distance -= 0.5f;
            break;
        case 'x':
            distance += 0.5f;
            break;
    }
}

//keyboard
void keyboard(unsigned char key, int x, int y) {
    if (gamestate==0) {
        //title scene interaction
        if (key=='n') {
            if (++p1idx >= pbarrsize) p1idx = 0; //switch p1
        } else
        if (key=='m') {
            if (++p2idx >= pbarrsize) p2idx = 0; //switch p2
        } else
        if (key==' ') {
            enterGs1(); //start battle
        }
    } else
    if (gamestate==1) {
        //battle scene interaction
        //p1 skill selection
        if (key=='1') {
            p1skill = 1;
        } else
        if (key=='2') {
            p1skill = 2;
        } else
        if (key=='3') {
            p1skill = 3;
        } else
        if (key==' ' && timer1 > 3) { //play turn
            playTurn();
            std::cout << "\np1 used skill " << p1skill << "\np2 used skill " << p2skill << "\n";
        }
        if (key=='i') { //toggle isbotp2
            isbotp2 = !isbotp2;
        }
        if (key=='g') { //toggle godmode
            isgodmode = !isgodmode;
        }
    } else
    if (gamestate==2) {
        if (key==' ') { //back to menu title
            enterGs0();
        }
    }

    if (key=='o') { //toggle audio
        if (ismutesound) {
            ismutesound = false;
            ma_engine_set_volume(&soundengine, 0.5);
        } else {
            ismutesound = true;
            ma_engine_set_volume(&soundengine, 0);
        }
    }
    if (key==']') { //increase gamespeed
        gamespeed += 0.1;
        std::cout << "gamespeed: " << gamespeed << '\n';
    }
    if (key=='[') { //decrease gamespeed
        gamespeed -= 0.1;
        if (gamespeed < 0) gamespeed = 0;
        std::cout << "gamespeed: " << gamespeed << '\n';
    }
    if (key=='p') { //toggle pause
        if (gamespeed == 0) {
            gamespeed = 1;
        } else {
            gamespeed = 0;
        }
        std::cout << "gamespeed: " << gamespeed << '\n';
    }
    if (key=='f') { //toggle freeview
        isfreeview = !isfreeview;
    }
    if (key=='r') { //toggle axis
        isaxis = !isaxis;
    }
    if (isfreeview) { //freeview movement
        moveFreeview(key);
    }
}

//special keys
void special(int key, int x, int y) {
    if (gamestate==1) {
        //p2 skill selection
        if (key == GLUT_KEY_LEFT) {
            p2skill = 1;
        } else
        if (key == GLUT_KEY_DOWN) {
            p2skill = 2;
        } else
        if (key == GLUT_KEY_RIGHT) {
            p2skill = 3;
        }
    }
    if (key == GLUT_KEY_F1) {
        iswire = !iswire;
        if (iswire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

//mouse button
void mouse(int button, int state, int x, int y) {
    if (isfreeview) {
        if (button == GLUT_LEFT_BUTTON) {
            //mouse is dragging, move cam
            rotating = (state == GLUT_DOWN);
            lastmx = x;
            lastmy = y;
        }
    }
}

//mouse motion
void motion(int x, int y) {
    if (rotating) {
        //calculate drag distance and cam movement
        int dx = x - lastmx;
        int dy = y - lastmy;
        lastmx = x;
        lastmy = y;
        azimuth -= dx * msens;
        elevation += dy * msens;
        if (elevation > 89.0f) elevation = 89.0f;
        if (elevation < -89.0f) elevation = -89.0f;
    }
}

//init game
void initGame() {
    initEnv();
    //init characters
    for (Body* a : p1bodyArr) {
        a->initBody();
    }
    for (Body* a : p2bodyArr) {
        a->initBody();
    }
    //init sound engine from tools
    ma_engine_init(NULL, &soundengine);
    ma_engine_set_volume(&soundengine, 0.5);
    enterGs0();
}
