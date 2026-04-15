//env.hpp by KAY HEAN CUN, CHEONG KAI JIN
//environment stuff

//lighting vars
GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specular[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat shininess = 50.0f;
//environment objects
Object court;
Texture courttex;
Object skysphere;
Texture skytex;
//gui textures
Texture grlogo;
Texture p1wintex;
Texture p2wintex;
Texture pdrawtex;
Texture lifetex;
Texture manatex;

float skytime = 0;

float confettitimer = 0; //confetti spawn cooldown
class Confetti {
public:
    float r, g, b;
    Vector3f rot{0,0,0};
    Vector3f rotv{0,0,0}; //rotate speed
    Vector3f pos{0,0,0};
    Vector3f vel{0,0,0};
    void update(float delta) {
        rot = rot + rotv*delta;
        pos = pos + vel*delta;
    }
};
std::vector<Confetti*> confettivec;
float colorlist[][3] ={ // color list for confetti
    {168./255, 100./255, 253./255},
    {41./255, 205./255, 255./255},
    {120./255, 255./255, 68./255},
    {255./255, 113./255, 141./255},
    {253./255, 255./255, 106./255},
};

void initEnv() {
    //light setup
    float lstr = 0.3; //light strength (diffuse)
    GLfloat light0_diffuse[] = { lstr, lstr, lstr, 1 };
    GLfloat light1_diffuse[] = { lstr, lstr, lstr, 1 };
    GLfloat light2_diffuse[] = { lstr, lstr, lstr, 1 };
    GLfloat light3_diffuse[] = { lstr, lstr, lstr, 1 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
    //env materials
    court.loadObj("materials/game/court.obj");
    courttex.loadTex("materials/game/court.png");
    grlogo.loadTex("materials/game/grlogo.png");
    lifetex.loadTex("materials/game/lifeheart.png");
    manatex.loadTex("materials/game/mana.png");
    p1wintex.loadTex("materials/game/p1win.png");
    p2wintex.loadTex("materials/game/p2win.png");
    pdrawtex.loadTex("materials/game/draw.png");
    skysphere.loadObj("materials/game/skysphere.obj");
    skytex.loadTex("materials/game/skybox.png");
}

void updateEnv(float delta) {
    skytime += delta;
}

void drawEnv() {
    glDisable(GL_LIGHTING); //draw without lighting because light baked
    glPushMatrix();
        court.drawObj(courttex.texid);

        float sbrotx = skytime*0.081;
        float sbroty = skytime*0.063+33;
        glPushMatrix();
        glScalef(5,5,5);
        glRotatef(sbrotx, 1, 0, 0);
        glRotatef(sbroty, 0, 1, 0);
        skysphere.drawObj(skytex.texid);
        glPopMatrix();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

//confetti for result scene
void updateConfetti(float delta) {
    //spawn confetti
    float confetticd = 0.05;
    confettitimer += delta;
    while (confettitimer > confetticd) {
        confettitimer -= confetticd;
        Confetti* newconfetti = new Confetti();
        int i = randi(0, 4);
        newconfetti->r = colorlist[i][0];
        newconfetti->g = colorlist[i][1];
        newconfetti->b = colorlist[i][2];
        newconfetti->rot = {randf(0, 360), randf(0, 360), randf(0, 360)};
        newconfetti->rotv = {randf(-90, 90), randf(-10, 10), randf(-90, 90)};
        newconfetti->pos = {randf(-4, 4), 8, randf(-10, 10)};
        newconfetti->vel = {randf(-1, 1), randf(-1, -3), randf(-1, 1)};
        confettivec.push_back(newconfetti);
    }
    //update
    for (int i = 0; i < (int)confettivec.size(); i++) {
        confettivec[i]->update(delta);
        if (confettivec[i]->pos.y < 0) {
            delete confettivec[i];
            confettivec[i] = nullptr;
            confettivec.erase(confettivec.begin() + i);
        }
    }
}

void drawConfetti() {
    float s = 0.08;
    glBindTexture(GL_TEXTURE_2D, 0);
    //draw
    for (Confetti* c : confettivec) {
        glPushMatrix();

        glTranslatef(c->pos.x, c->pos.y, c->pos.z);
        glRotatef(c->rot.x, 1, 0, 0);
        glRotatef(c->rot.y, 0, 1, 0);
        glRotatef(c->rot.z, 0, 0, 1);

        glColor3f(c->r, c->g, c->b);
        glBegin(GL_QUADS);
        //frontface
        glVertex3f(-s, -s, 0);
        glVertex3f(s, -s, 0);
        glVertex3f(s, s, 0);
        glVertex3f(-s, s, 0);
        //backface
        glVertex3f(-s, -s, 0);
        glVertex3f(-s, s, 0);
        glVertex3f(s, s, 0);
        glVertex3f(s, -s, 0);
        glEnd();

        glPopMatrix();
    }
}

void clearConfetti() {
    //delete all confetti instances
    for (Confetti* c : confettivec) {
        delete c;
        c = nullptr;
    }
    confettivec.clear();
}
