//chickenjockey.hpp by CHEONG KAI JIN

#include "../tools.hpp"

//chicken jockey character and anims
class ChickenJockey : public Body {
public:
    //chickenjockey
    Object cbody;
    Object cleg;
    Object cwing;
    Object chead;
    Object zbody;
    Object zleg;
    Object zarm;
    Object zhead;
    Texture cbodytex;
    Texture clegtex;
    Texture cwingtex;
    Texture cheadtex;
    Texture zbodytex;
    Texture zlegtex;
    Texture zarmtex;
    Texture zheadtex;
    //items
    Texture eggtex;
    Texture eggshardtex;
    Object block;
    Texture blocktex;
    Texture blockshardtex;
    Texture potiontex;
    Texture effect0; //each frame for effect animation
    Texture effect1;
    Texture effect2;
    Texture effect3;
    Texture effect4;
    Texture effect5;
    Texture effect6;
    Texture effect7;

    int playinganim = 0; //anim id
    float animtime = 0; //local animation time
    float animtimer1 = 0; //animation timer 1
    //chicken animation
    float cwingdeg = 0;
    float cbodybob = 0;
    float cheadbob = 0;
    //projectile
    class Projectile {
    public:
        int idx = 0;
        float textimer=0;
        Vector3f pos{0, 0, 0}; //position
        Vector3f vel{0, 0, 0}; //velocity
        Vector3f acl{0, -9.8, 0}; //acceleration
        void update(float delta) {
            pos = pos + vel*delta;
            vel = vel + acl*delta; //gravity
            textimer += delta;
        }
    };
    //eggs
    std::vector<Projectile*> eggs;
    std::vector<Projectile*> eggshards; //break particles
    //block wall
    int blockcount = 0;
    Vector3f blockpos[9] = { //predefined positions for block placement
        {0,1,-2},{0,1,0},{0,1,2},
        {0,3,-2},{0,3,0},{0,3,2},
        {0,5,-2},{0,5,0},{0,5,2}
    };
    std::vector<Projectile*> blockshards; //break particles
    //potion
    Projectile splashpotion;
    std::vector<Projectile*> effects;
    GLuint effecttex[8];

    void initBody() {
        //skill description
        desc[0] = "Egg Barrage";
        desc[1] = "Cobblestone Wall";
        desc[2] = "Potion Splash";
        //load model informations and textures
        cbody.loadObj("materials/chickenjockey/cbody.txt");
        cleg.loadObj("materials/chickenjockey/cleg.txt");
        cwing.loadObj("materials/chickenjockey/cwing.txt");
        chead.loadObj("materials/chickenjockey/chead.txt");
        zbody.loadObj("materials/chickenjockey/zbody.txt");
        zleg.loadObj("materials/chickenjockey/zleg.txt");
        zarm.loadObj("materials/chickenjockey/zarm.txt");
        zhead.loadObj("materials/chickenjockey/zhead.txt");
        cbodytex.loadTex("materials/chickenjockey/cbody.png");
        clegtex.loadTex("materials/chickenjockey/cleg.png");
        cwingtex.loadTex("materials/chickenjockey/cwing.png");
        cheadtex.loadTex("materials/chickenjockey/chead.png");
        zbodytex.loadTex("materials/chickenjockey/zbody.png");
        zlegtex.loadTex("materials/chickenjockey/zleg.png");
        zarmtex.loadTex("materials/chickenjockey/zarm.png");
        zheadtex.loadTex("materials/chickenjockey/zhead.png");
        //items
        block.loadObj("materials/chickenjockey/block.txt");
        blocktex.loadTex("materials/chickenjockey/cobblestone.png");
        blockshardtex.loadTex("materials/chickenjockey/cobbleshard.png");
        eggtex.loadTex("materials/chickenjockey/egg.png");
        eggshardtex.loadTex("materials/chickenjockey/eggshard.png");
        potiontex.loadTex("materials/chickenjockey/potion.png");
        effect0.loadTex("materials/chickenjockey/effect0.png");
        effect1.loadTex("materials/chickenjockey/effect1.png");
        effect2.loadTex("materials/chickenjockey/effect2.png");
        effect3.loadTex("materials/chickenjockey/effect3.png");
        effect4.loadTex("materials/chickenjockey/effect4.png");
        effect5.loadTex("materials/chickenjockey/effect5.png");
        effect6.loadTex("materials/chickenjockey/effect6.png");
        effect7.loadTex("materials/chickenjockey/effect7.png");
        effecttex[7] = effect0.texid;
        effecttex[6] = effect1.texid;
        effecttex[5] = effect2.texid;
        effecttex[4] = effect3.texid;
        effecttex[3] = effect4.texid;
        effecttex[2] = effect5.texid;
        effecttex[1] = effect6.texid;
        effecttex[0] = effect7.texid;
    }

    void drawBody() {
        //hierarchical modelling
        glPushMatrix();
            glScalef(0.4, 0.4, 0.4);

            //clegs
            glPushMatrix();
                glTranslatef(0, 0.9, 0.2);
                cleg.drawObj(clegtex.texid);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0, 0.9, -0.2);
                cleg.drawObj(clegtex.texid);
            glPopMatrix();

            //cbody
            glPushMatrix();
                float cbodybob = sin(animtime*2) * 0.1;
                glTranslatef(0, 0.6 + cbodybob, 0);
                cbody.drawObj(cbodytex.texid);

                //chead
                glPushMatrix();
                    float cheadbob = sin(animtime*2+1.5) * 0.1;
                    glTranslatef(0.5, 0.6 + cheadbob, 0);
                    chead.drawObj(cheadtex.texid);
                glPopMatrix();

                //cwings
                float cwingdeg = (sin(animtime*30) + 1) * 45;
                glPushMatrix();
                    glTranslatef(0, 1.2, 0.6);
                    glRotatef(-cwingdeg, 1, 0, 0);
                    cwing.drawObj(cwingtex.texid);
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(0, 1.2, -0.6);
                    glRotatef(cwingdeg, 1, 0, 0);
                    cwing.drawObj(cwingtex.texid);
                glPopMatrix();
                glPushMatrix();

                    //zbody
                    glTranslatef(-0.2, 1.4, 0);
                    glScalef(0.7, 0.7, 0.7);

                    if (playinganim == 5) {
                        float zbodytilt = sin(animtime*3.14159/1.7392) * 15;
                        glRotatef(zbodytilt, 1, 0, 0);
                    }

                    zbody.drawObj(zbodytex.texid);

                    //zlegs
                    glPushMatrix();
                        glTranslatef(0, 0, 0.3);
                        glRotatef(75, 0, 0, 1);
                        glRotatef(-30, 1, 0, 0);
                        zleg.drawObj(zlegtex.texid);
                    glPopMatrix();
                    glPushMatrix();
                        glTranslatef(0, 0, -0.3);
                        glRotatef(75, 0, 0, 1);
                        glRotatef(30, 1, 0, 0);
                        zleg.drawObj(zlegtex.texid);
                    glPopMatrix();

                    //zarms
                    float zarmbobx = sin(animtime*0.33) *1; //bob degree
                    float zarmbobz = sin(animtime) *3;
                    glPushMatrix(); //right
                        glTranslatef(0, 1.3, 0.8);
                        glRotatef(90 + zarmbobz, 0, 0, 1);
                        glRotatef(zarmbobx, 1, 0, 0);

                        if (playinganim == 1) { //attack: hold egg and shake
                            if (animtime < 2) {
                                float zrarmshake = cos(animtime*32) * 30 + 30;
                                glRotatef(zrarmshake, 0, 0, 1);
                            }
                            drawParticle(0, -1.5, 0, 5, eggtex.texid); //hold egg
                        }

                        if (playinganim == 2) { //defend: hold block and shake
                            if (animtime < 0.9) {
                                float zrarmshake = cos(animtime*32) * 30 + 30;
                                glRotatef(zrarmshake, 0, 0, 1);
                            }
                            glPushMatrix();
                            glTranslatef(0, -1.5, 0);
                            glScalef(0.5,0.5,0.5);
                            block.drawObj(blocktex.texid); //hold block
                            glPopMatrix();
                        }

                        if (playinganim == 4 || playinganim == 6) { //defeat: look down and shake head
                            glRotatef(-30, 0, 0, 1);
                        }


                        if (playinganim == 5) { //defeat: look down and shake head
                            float zrarmwave = sin(animtime*3.14159/1.7392) *20;
                            glRotatef(zrarmwave, 0, 0, 1);
                        }

                        zarm.drawObj(zarmtex.texid);

                    glPopMatrix();
                    glPushMatrix(); //left
                        glTranslatef(0, 1.3, -0.8);
                        glRotatef(90 - zarmbobz, 0, 0, 1);
                        glRotatef(-zarmbobx, 1, 0, 0);

                        if (playinganim == 4 || playinganim == 6) {
                            glRotatef(-30, 0, 0, 1);
                        }

                        if (playinganim == 5) { //defeat: look down and shake head
                            float zlarmwave = sin(animtime*3.14159/1.7392) *-20;
                            glRotatef(zlarmwave, 0, 0, 1);
                        }

                        zarm.drawObj(zarmtex.texid);
                    glPopMatrix();

                    //zhead
                    glPushMatrix();
                        glTranslatef(0, 1.6, 0);
                        glScalef(1.4, 1.4, 1.4);

                        if (playinganim == 4 || playinganim == 6) { //lose, look down shake head
                            float zheadshake = sin(animtime*2) *15;
                            glRotatef(-40, 0, 0, 1);
                            glRotatef(zheadshake, 0, 1, 0);
                        }

                        if (playinganim == 5) { //win
                            glRotatef(20, 0, 0, 1);
                        }
                        zhead.drawObj(zheadtex.texid);

                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
        //items
        if (playinganim == 1) { //draw eggs and eggshards if any
            glPushMatrix();
            for (Projectile* e : eggs) {
               drawParticle(e->pos.x, e->pos.y, e->pos.z, 0.5, eggtex.texid);
            }
            for (Projectile* es : eggshards) {
               drawParticle(es->pos.x, es->pos.y, es->pos.z, 1, eggshardtex.texid);
            }
            glPopMatrix();
        } else
        if (playinganim == 2) { //draw wall blocks and shards if any
            glPushMatrix();
            glScalef(0.5, 0.5, 0.5);
            glTranslatef(3,0,0);
            for (int i = 0; i < blockcount; i++) {
                glPushMatrix();
                glTranslatef(0, blockpos[i].y, blockpos[i].z);
                block.drawObj(blocktex.texid);
                glPopMatrix();
            }
            for (Projectile* bs : blockshards) {
               drawParticle(bs->pos.x, bs->pos.y, bs->pos.z, 4, blockshardtex.texid);
            }
            glPopMatrix();
        } else
        if (playinganim == 3) { //draw potion and effect if any
            if (splashpotion.pos.y > 1.5) {
               drawParticle(splashpotion.pos.x, splashpotion.pos.y, splashpotion.pos.z, 1, potiontex.texid);
            }
            for (Projectile* ef : effects) {
               drawParticle(ef->pos.x, ef->pos.y, ef->pos.z, 0.8, effecttex[ef->idx]);
            }
        }
    }

    //delta time update
    void updateBody(float delta) {

        if (playinganim == 1) { //throw eggs
            for (int i = 0; i < (int)eggs.size(); i++) {
                Projectile* e = eggs[i];
                e->update(delta);
                if (e->pos.x > 8.0f) {
                    for (int i = 0; i < 4; i++) {
                        Projectile* eggshard = new Projectile();
                        eggshard->pos = e->pos;
                        eggshard->vel = {randf(-1,1), randf(-1, 3), randf(-1,1)};
                        eggshards.push_back(eggshard);
                    }
                    delete e;
                    e = nullptr;
                    eggs.erase(eggs.begin() + i);
                }
            }
            for (int i = 0; i < (int)eggshards.size(); i++) {
                Projectile* es = eggshards[i];
                es->update(delta);
                if (es->pos.y <= 0.0f) {
                    delete es;
                    es = nullptr;
                    eggshards.erase(eggshards.begin() + i);
                }
            }
            if (animtime < 2 && animtimer1 > 0.1) {
                animtimer1 = 0;
                Projectile* newegg = new Projectile();
                newegg->pos = {0.3, 1.2, 0};
                newegg->vel.x = 10;
                newegg->vel.z = randf(-1, 1);
                newegg->vel.y = randf(-1, 1) + 4;
                eggs.push_back(newegg);
                playSfx("materials/chickenjockey/eggthrow.wav");
            }
            animtimer1 += delta;
        } else

        if (playinganim == 2) { //build wall and break after
            for (int i = 0; i < (int)blockshards.size(); i++) {
                Projectile* bs = blockshards[i];
                bs->update(delta);
                if (bs->pos.y <= 0.0f) {
                    delete bs;
                    bs = nullptr;
                    blockshards.erase(blockshards.begin() + i);
                }
            }
            if (blockcount < 9 && animtimer1 > 0.1 && animtime < 3) {
                animtimer1 = 0;
                blockcount++;
                playSfx("materials/chickenjockey/stoneplace.wav");
            } else
            if (animtime >= 3 && blockcount == 9) {
                blockcount = 0;
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 4; j++) {
                        Projectile* blockshard = new Projectile();
                        blockshard->pos = blockpos[i];
                        blockshard->vel = {randf(-2,2), randf(-1, 4), randf(-2,2)};
                        blockshards.push_back(blockshard);
                    }
                }
                playSfx("materials/chickenjockey/stonebreak.wav");
            }
            animtimer1 += delta;
        } else

        if (playinganim == 3) { //throw potion and splash effect
            for (int i = 0; i < (int)effects.size(); i++) {
                Projectile* ef = effects[i];
                ef->update(delta);
                //if potion particle finish animation, delete
                if (ef->textimer >= 0.2f) {
                    ef->textimer = 0;
                    if (ef->idx++ == 7) {
                        delete ef;
                        ef = nullptr;
                        effects.erase(effects.begin() + i);
                    }
                }
            }
            if (splashpotion.pos.y < 1.5 && splashpotion.pos.y != 0) {
                splashpotion.pos.y = 0;
                //generate potion particles
                for (int i = 0; i < 6; i++) {
                    Projectile* effect = new Projectile();
                    effect->textimer = randf(0, 0.15);
                    effect->pos = {0, 1.5, 0};
                    effect->vel = {randf(-4,4), randf(-1, 1), randf(-4,4)};
                    effect->acl = {0, 3, 0}; //float upwards
                    effects.push_back(effect);
                }
                playSfx("materials/chickenjockey/potionbreak.wav");
            }
            if (splashpotion.pos.y != 0) splashpotion.update(delta);
        }

        //animtime update
        animtime += delta;
    }

    void playAnim(int num) {
        //clean ups
        if (playinganim == 1) { //clean up eggs
            animtimer1 = 99;
            for (Projectile* e : eggs) {
                delete e;
                e = nullptr;
            }
            for (Projectile* es : eggshards) {
                delete es;
                es = nullptr;
            }
            eggs.clear();
            eggshards.clear();
        } else
        if (playinganim == 2) { //clean up blocks
            animtimer1 = 99;
            for (Projectile* bs : blockshards) {
                delete bs;
                bs = nullptr;
            }
            blockshards.clear();
        }
        //setups
        if (num == 3) {
            splashpotion.pos = {0,2,0};
            splashpotion.vel = {0,5,0};
            playSfx("materials/chickenjockey/potionthrow.wav");
        }
        playinganim = num;
        animtime = 0;
    }

};
