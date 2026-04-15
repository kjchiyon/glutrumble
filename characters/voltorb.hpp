//voltorb.hpp by KANG XUAN XIN

#include "../tools.hpp"

class Voltorb : public Body {

    GLUquadric* quadric; // For drawing spheres
    float radius;        // Sphere radius
    float M_PI = 3.14159;

    public:

        Texture energytex;
        Texture cooltex;

        int playinganim = 0;
        float animtime = 0;
        float bodyt = 0;

        class Energy {
        public:
            Vector3f pos{0,0,0};
            void update(float delta) {
                float speed = delta*1.5;
                pos.x = lerp(pos.x, 0, speed);
                pos.y = lerp(pos.y, 0, speed);
                pos.z = lerp(pos.z, 0, speed);
            }
        };
        std::vector<Energy*> energies;

        void initBody()
        {
            quadric = gluNewQuadric();
            gluQuadricNormals(quadric, GLU_SMOOTH);
            radius = 8.0f;
            //materials
            energytex.loadTex("materials/voltorb/energy.png");
            cooltex.loadTex("materials/voltorb/cool.png");
            //skill desc
            desc[0] = "Rollout";
            desc[1] = "Spin Block";
            desc[2] = "Charge Up";
        }

        ~Voltorb()
        {
            gluDeleteQuadric(quadric);
        }

        void drawSphere()
        {
            // Draw LOWER (white) hemisphere first
            glColor3f(0.85f, 0.85f, 0.9f); // White
            GLdouble eqBottom[4] = {0.0, -1.0, 0.0, 0.0}; // y < 0
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, eqBottom);
            gluSphere(quadric, radius, 50, 50);
            glDisable(GL_CLIP_PLANE0);

            // Draw UPPER (red) hemisphere second
            glColor3f(0.85f, 0.08f, 0.13f); // Red
            GLdouble eqTop[4] = {0.0, 1.0, 0.0, 0.0}; // y > 0
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, eqTop);
            gluSphere(quadric, radius, 50, 50);
            glDisable(GL_CLIP_PLANE0);
        }

        void drawEyes()
        {
            glColor3f(1, 1, 1);

            // Small offset to avoid z-fighting with sphere
            float offset = 0.135f;

            // Left eye
            float lx1 = -3.3f, ly1 = 2.8f;
            float lx2 = -3.2f, ly2 = 1.3f;
            float lx3 = -1.3f, ly3 = 1.1f;
            float lx4 = -1.3f, ly4 = 1.3f;

            float lz1 = sqrtf(radius * radius - lx1 * lx1 - ly1 * ly1) + offset;
            float lz2 = sqrtf(radius * radius - lx2 * lx2 - ly2 * ly2) + offset;
            float lz3 = sqrtf(radius * radius - lx3 * lx3 - ly3 * ly3) + offset;
            float lz4 = sqrtf(radius * radius - lx4 * lx4 - ly4 * ly4) + offset;

            glBegin(GL_QUADS);
                glVertex3f(lx1, ly1, lz1); // top-left (outer)
                glVertex3f(lx2, ly2, lz2); // bottom-right (inner)
                glVertex3f(lx3, ly3, lz3); // bottom-left (outer)
                glVertex3f(lx4, ly4, lz4);
            glEnd();

            // Right eye (mirror X values)
            float rx4 =  3.3f, ry4 = 2.8f;
            float rx3 =  3.2f, ry3 = 1.3f;
            float rx2 =  1.3f, ry2 = 1.1f;
            float rx1 =  1.3f, ry1 = 1.3f;

            float rz1 = sqrtf(radius * radius - rx1 * rx1 - ry1 * ry1) + offset;
            float rz2 = sqrtf(radius * radius - rx2 * rx2 - ry2 * ry2) + offset;
            float rz3 = sqrtf(radius * radius - rx3 * rx3 - ry3 * ry3) + offset;
            float rz4 = sqrtf(radius * radius - rx4 * rx4 - ry4 * ry4) + offset;

            glBegin(GL_QUADS);
                glVertex3f(rx1, ry1, rz1); // top-right (outer)
                glVertex3f(rx2, ry2, rz2); // bottom-left (inner)
                glVertex3f(rx3, ry3, rz3); // bottom-right (outer)
                glVertex3f(rx4, ry4, rz4);
            glEnd();

            // Left pupil (black ellipse)
            glColor3f(0, 0, 0);
            float plx = -2.3f;        // X position for left pupil
            float ply = 1.9f;         // Y position for both pupils
            //float plz = sqrtf(radius * radius - plx * plx - ply * ply) + offset + 0.01f;

            glBegin(GL_POLYGON);
            for (int i = 0; i < 36; ++i) {
                float angle = 2.0f * M_PI * i / 36;
                float x = plx + cosf(angle) * 0.10f; // X radius
                float y = ply + sinf(angle) * 0.25f; // Y radius
                float z = sqrtf(radius * radius - x * x - y * y) + offset + 0.01f;
                glVertex3f(x, y, z);
            }
            glEnd();

            // Right pupil (black ellipse)
            float prx = 2.3f;        // X position for right pupil
            float pry = 1.9f;
            //float prz = sqrtf(radius * radius - prx * prx - pry * pry) + offset + 0.01f;

            glBegin(GL_POLYGON);
            for (int i = 0; i < 36; ++i) {
                float angle = 2.0f * M_PI * i / 36;
                float x = prx + cosf(angle) * 0.10f;
                float y = pry + sinf(angle) * 0.25f;
                float z = sqrtf(radius * radius - x * x - y * y) + offset + 0.01f;
                glVertex3f(x, y, z);
            }
            glEnd();
        }

        void drawBody()
        {
            glTranslatef(0, 0.8, 0);

            float bottomoffset = -0.8;

            if (playinganim == 0) { //idle
                float rotx = sin(animtime*1.2) * 15;
                float rotz = cos(animtime*1.2) * 15;
                pivotRotatef(rotx, 1, 0, 0, 0, bottomoffset, 0);
                pivotRotatef(rotz, 0, 0, 1, 0, bottomoffset, 0);
            }

            if (playinganim == 1) { //attack
                float bodyx = lerp(0, 8, bodyt); //lerp body towards enemy
                float rotz = animtime*-1000; //roll
                glTranslatef(bodyx, 0, 0);
                glRotatef(rotz, 0, 0, 1);
            }

            if (playinganim == 2) { //defend
                float roty = animtime*1000; //spin fast
                glRotatef(roty, 0, 1, 0);
                glRotatef(10, 0, 0, 1);
            }

            if (playinganim == 3) { //regenerate
                for (Energy* e : energies) { //create energy particles
                    drawParticle(e->pos.x, e->pos.y, e->pos.z, 1, energytex.texid);
                }
                //same as idle
                float rotx = sin(animtime*1.2) * 15;
                float rotz = cos(animtime*1.2) * 15;
                pivotRotatef(rotx, 1, 0, 0, 0, bottomoffset, 0);
                pivotRotatef(rotz, 0, 0, 1, 0, bottomoffset, 0);
            }

            if (playinganim == 4 || playinganim == 6) { //defeat
                float roty = sin(animtime*1.6)*20;
                glRotatef(-40, 0, 0, 1); //look down
                glRotatef(roty, 0, 1, 0); //shake head
            }

            if (playinganim == 5) { //victory
                glRotatef(10, 0, 0, 1);
                float coolsize = sin(animtime*1.6+23)*0.1+0.5;
                float coolbob = sin(animtime*0.6)*0.1;
                drawParticle(0.8, 0.2+coolbob, 0.5, coolsize, cooltex.texid);
            }

            glScalef(0.1, 0.1, 0.1);
            glRotatef(90, 0, 1, 0);
            glBindTexture(GL_TEXTURE_2D, 0); //unbind texture
            drawSphere();
            drawEyes();
        }

        void updateBody(float delta) {

            if (playinganim == 1) {
                if (animtime < 0.8) {
                    bodyt += delta;
                } else
                if (animtime < 2 && bodyt > 0) {
                    bodyt -= delta;
                } else {
                    playAnim(0);
                }
            }

            if (playinganim == 2 && animtime > 3) {
                playAnim(0);
            }

            if (playinganim == 3) {
                for (Energy* e : energies) {
                    e->update(delta);
                }
            }

            animtime += delta;
        }

        void playAnim(int num) {

            if (num == 1) {
                bodyt = 0;
                playSfx("materials/voltorb/attack.wav");
            }

            if (num == 2) {
                playSfx("materials/voltorb/defend.wav");
            }

            if (num == 3) {
                for (Energy* e : energies) {
                    delete e;
                    e = nullptr;
                }
                energies.clear();
                float side = 3;
                for (int i = 0; i < 16; i++) {
                    Energy* newenergy = new Energy();
                    newenergy->pos.x = randf(-side,side);
                    newenergy->pos.y = randf(0,side);
                    newenergy->pos.z = randf(-side,side);
                    energies.push_back(newenergy);
                }
                playSfx("materials/voltorb/heal.wav");
            }

            animtime = 0;
            playinganim = num;
        }

};
