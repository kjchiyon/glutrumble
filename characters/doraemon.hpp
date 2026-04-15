//doraemon.hpp by CHONG ZHENG CHENG

#include "../tools.hpp"

class Doraemon : public Body {
public:

    Object dhead;
    Object darm;
    Object dbody;
    Object dleg;
    Object dtail;
    Object air_gun;
    Object shield;
    Object dorayaki;

    Texture dheadtex;
    Texture darmtex;
    Texture dbodytex;
    Texture dlegtex;
    Texture dtailtex;
    Texture air_gun_tex;
    Texture shield_tex;
    Texture dorayaki_tex;
    Texture bullet_tex;

    // declaration animation time
    int playinganim = 0;
    float animtime = 0;

    float bodybob = 0;
    float armt = 0;
    float airt = 0;
    float foodt = 0;
    float shieldt = 0; // variable lerp


    //load model
    void initBody() {
        // model
        dhead.loadObj("materials/doraemon/dhead.txt");
        darm.loadObj("materials/doraemon/darm.txt");
        dbody.loadObj("materials/doraemon/dbody.txt");
        dleg.loadObj("materials/doraemon/dleg.txt");
        dtail.loadObj("materials/doraemon/dtail.txt");

        // texture
        dheadtex.loadTex("materials/doraemon/head.png");
        darmtex.loadTex("materials/doraemon/arm.png");
        dbodytex.loadTex("materials/doraemon//body.png");
        dlegtex.loadTex("materials/doraemon/leg.png");
        dtailtex.loadTex("materials/doraemon/tail.png");

        // skill
        air_gun.loadObj("materials/doraemon/air_gun.txt");
        shield.loadObj("materials/doraemon/shield.txt");
        dorayaki.loadObj("materials/doraemon/dorayaki_1.0.txt");

        //texture
        air_gun_tex.loadTex("materials/doraemon/air_gun.png");
        shield_tex.loadTex("materials/doraemon/xixi.png");
        dorayaki_tex.loadTex("materials/doraemon/flour.png");
        bullet_tex.loadTex("materials/doraemon/bullet.png");

        //skill desc
        desc[0] = "Air Gun";
        desc[1] = "Nobita Shield";
        desc[2] = "Dorayaki Break";
    }

    void drawBody() {
        //hiearchical modelling
        glPushMatrix();
            glTranslatef(0, 0.2, 0);
            glScalef(0.5, 0.5, 0.5);
            //body
            glPushMatrix();
                glTranslatef(0, bodybob, 0);

                // win, jump
                if (playinganim == 5) {
                    float jumpy = fabs(sin(animtime*3)); //jump y
                    glTranslatef(0, jumpy, 0); //jumping
                }

                dbody.drawObj(dbodytex.texid);

                //head
                glPushMatrix();
                    glTranslatef(0, 1.4, 0);

                    // lose animation, head shaking
                    if(playinganim == 6){
                        glTranslatef(-0.2, 0.2, 0);
                        glRotatef(-30, 0, 0, 1); // head rotate to down 30 degree
                        float headdeg = sin(animtime * 2) * 45; //head degree
                        glRotatef(headdeg, 0, 1, 0); // shaking
                    }
                    dhead.drawObj(dheadtex.texid);
                glPopMatrix();

                //right arm
                glPushMatrix();
                    glTranslatef(0, 1.5, 0.7);

                    if (playinganim == 1) {
                        float armdeg = lerp(0, 90, armt);
                        glRotatef(armdeg, 0, 0, 1);
                    }

                    // skill 3 and win animation, arm's movement
                    if (playinganim == 3 || playinganim == 5) {
                        float armt2 = 1 - fabs(armt);
                        float armdeg = lerp(0, 135, armt2); // arm degree from 0 to 135
                        glRotatef(armdeg, 0, 0, 1);
                    }

                    glRotatef(-45, 1, 0, 0);
                    darm.drawObj(darmtex.texid);

                    // skill 1
                    if (playinganim == 1){
                        //draw air gun
                        glPushMatrix();
                            glTranslatef(0.5, -1.6, 0);
                            glRotatef(-90, 0, 1, 0);
                            glRotatef(135, 0, 0, 1);
                            air_gun.drawObj(air_gun_tex.texid);
                        glPopMatrix();
                    }
                glPopMatrix();

                //left arm
                glPushMatrix();
                    glTranslatef(0, 1.5, -0.7);

                    // skill 3 and win animation, arm's movement
                    if (playinganim == 3 || playinganim == 5) {
                        float armt2 = 1 - fabs(armt);
                        float armdeg = lerp(0, 135, armt2); // arm degree from 0 to 135
                        glRotatef(armdeg, 0, 0, 1);
                    }

                    glRotatef(45, 1, 0, 0);
                    darm.drawObj(darmtex.texid);
                glPopMatrix();

                //left leg
                glPushMatrix();
                    glTranslatef(0, 0.25 - bodybob, 0.45);
                    dleg.drawObj(dlegtex.texid);
                glPopMatrix();

                //right leg
                glPushMatrix();
                    glTranslatef(0, 0.25 - bodybob, -0.45);
                    dleg.drawObj(dlegtex.texid);
                glPopMatrix();

                //tail
                glPushMatrix();
                    glTranslatef(-0.6, 0.3, 0);
                    glRotatef(-90, 0, 0, 1);
                    dtail.drawObj(dtailtex.texid);
                glPopMatrix();

            glPopMatrix();
        glPopMatrix();

        // skill 1, bullet animation time start on 0.5 sec, delete after 2.5 sec
        if (playinganim == 1 && animtime > 0.5 && animtime < 1.5) {
            float airx = lerp(2, 8, airt); // x from 2 to 8
            drawParticle(airx, 1, 0, 1, bullet_tex.texid); // bullet
        }

        // skill 2, delete after 3 sec
        if (playinganim == 2 && animtime < 3) {
            //draw shield
            glPushMatrix();
                float shieldx = lerp(0.5, 2, shieldt); //x movement
                float shieldy = lerp(0.5, 1.2, shieldt); // y movement
                float shieldscale = lerp(0, 1.2, shieldt); // scale small to big

                glTranslatef(shieldx, shieldy, 0);
                glScalef(shieldscale, shieldscale, shieldscale);
                shield.drawObj(shield_tex.texid); //shield front
                glTranslatef(0.25, 0, 0);
                glScalef(0.2, 1, 1);
                glRotatef(180, 0, 1, 0);
                shield.drawObj(shield_tex.texid); //shield back
            glPopMatrix();
        }

        // skill 3, delete after 1 sec
        if (playinganim == 3 && animtime < 1) {
            glPushMatrix();
                float foodx = lerp(2, 0.2, foodt); // x from 2 to 0.2
                float foody = lerp(0.5, 1.5, foodt); // y from 0.5 to 1.5
                float foodscale = lerp(1, 0.3, foodt); // scale big to small

                glTranslatef(foodx, foody, 0);
                glScalef(foodscale, foodscale, foodscale);

                dorayaki.drawObj(dorayaki_tex.texid);
            glPopMatrix();
        }
    }

    void updateBody(float delta) { //time equation
        bodybob = sin(animtime*3.8) * 0.065; // body wave

        // skill 1
        if (playinganim == 1) { // skill 1
            if (armt < 1) { //
                armt += delta / 0.5; // arm's move speed
            } else {
                armt = 1;
            }
            if (animtime > 0.5) { // air bullet appear after 0.5 sec
                if (airt < 1) {
                    airt += delta / 1;
                } else {
                    airt = 1;
                }
            }
        }

        // skill 2
        if (playinganim == 2) { // skill 2
            if (shieldt < 1) { //
                shieldt += delta / 0.5; // shield appear speed
            } else {
                shieldt = 1;
            }
        }

        // skill 3
        if (playinganim == 3) { // skill 3
            if (armt < 1) { //
                armt += delta*2 * (3.14159 /3); // arm move speed
            } else {
                armt = 1;
            }
            if (foodt < 1) { //
                foodt += delta / 0.5;
            } else {
                foodt = 1;
            }
        }


        // win animation
        if (playinganim == 5) {
             armt = fabs(sin(animtime*3));
        }

        animtime += delta;
    }

    void playAnim(int num) {

        // skill 1
        if (num == 1) {
            armt = 0;
            airt = 0;
            playSfx("materials/doraemon/gun.wav");
        }

        //skill 2
        if (num == 2) {
            shieldt = 0;
            playSfx("materials/doraemon/shield.wav");
        }

        //skill 3
        if (num == 3) {
            armt = -1;
            foodt = 0;
            playSfx("materials/doraemon/eat.wav");
        }

        // win animation
        if (num == 5) {
            armt = -1;
        }

        animtime = 0;
        playinganim = num;
    }
};
