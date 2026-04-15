//tools.hpp by CHEONG KAI JIN
//some classes and functions

#ifndef TOOLS
#define TOOLS

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //for textures, simple single header
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h" //for audio, simple single header

//!useful data structs
struct Vector3f {
    float x, y, z;
    Vector3f operator+(const Vector3f& b) const { return {x + b.x, y + b.y, z + b.z}; }
    Vector3f operator-(const Vector3f& b) const { return {x - b.x, y - b.y, z - b.z}; }
    Vector3f operator*(float s) const { return {x * s, y * s, z * s}; }
};

//!this is a base/superclass for characters
//!code your character model and animations in a subclass of this
class Body {
public:
    std::string desc[3]; //skill description

    //!these functions can be overridden in your character subclass
    virtual void initBody() {}
    virtual void drawBody() {}
    virtual void updateBody(float delta) {}
    virtual void playAnim(int num) {}
    //num: 0=idle 1=attack 2=defend 3=regen 4=no_mana 5=victory 6=defeat

    //proofing
    void draw() {
        glPushMatrix();
        drawBody();
        glPopMatrix();
    }

    virtual ~Body() = default; //deconstructor
};

//!class for obj loading and drawing, obj must be triangulated!!!
class Object {
public:
    //data structs
    struct Vertex { float x, y, z; };
    struct TexCoord { float u, v; };
    struct Normal { float x, y, z; };
    struct Face {int v[3], t[3], n[3]; };

    //model data stored in vectors
    std::vector<Vertex> vertices;
    std::vector<TexCoord> texCoords;
    std::vector<Normal> normals;
    std::vector<Face> faces;

    //load model using relative path
    void loadObj(const std::string& path) {
        //load file
        std::ifstream file(path.c_str());
        if (!file.is_open()) {
            std::cout << "object load fail: " << path << "\n";
            return;
        }
        //clear data
        vertices.clear();
        texCoords.clear();
        normals.clear();
        faces.clear();
        //read data
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;
            if (prefix == "v") {
                Vertex v; ss >> v.x >> v.y >> v.z;
                vertices.push_back(v);
            } else if (prefix == "vt") {
                TexCoord t; ss >> t.u >> t.v;
                texCoords.push_back(t);
            } else if (prefix == "vn") {
                Normal n; ss >> n.x >> n.y >> n.z;
                normals.push_back(n);
            } else if (prefix == "f") {
                Face f;
                for (int i = 0; i < 3; ++i) {
                    std::string token; ss >> token;
                    sscanf(token.c_str(), "%d/%d/%d", &f.v[i], &f.t[i], &f.n[i]);
                    f.v[i]--; f.t[i]--; f.n[i]--;
                }
                faces.push_back(f);
            }
        }
    }

    //draw
    void drawObj(GLuint texid) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texid);
        glColor3f(1,1,1);
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < faces.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                const Normal& n = normals[faces[i].n[j]];
                glNormal3f(n.x, n.y, n.z);
                const TexCoord& t = texCoords[faces[i].t[j]];
                glTexCoord2f(t.u, t.v);
                const Vertex& v = vertices[faces[i].v[j]];
                glVertex3f(v.x, v.y, v.z);
            }
        }
        glEnd();
        glPopMatrix();
    }
};

//!class to load texture, use png!!!
class Texture {
public:
    GLuint texid;

    //load texture using path
    void loadTex(const std::string& path) {
        int w, h, n; //n is color format
        stbi_set_flip_vertically_on_load(true); //flip image
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &n, 0);
        if (!data) {
            std::cout << "texture load fail: " << path << "\n";
            return;
        }
        //generate texture
        glGenTextures(1, &texid); //get generated texture id
        glBindTexture(GL_TEXTURE_2D, texid);

        GLenum format = (n == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }

    //deconstruct, delete texture
    ~Texture() {
        glDeleteTextures(1, &texid);
    }
};

//enter 2d projection for gui
void begin2D() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glAlphaFunc(GL_GREATER, 0);
}

//exit 2d projection
void end2D() {
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glAlphaFunc(GL_GREATER, 0.99);
    glColor4f(1,1,1,1);
}

//draw a 2d flat texture on screen
void draw2DTexture(GLuint texid, float x, float y, float w, float h, float tr=1, float tg=1, float tb=1) {
    glBindTexture(GL_TEXTURE_2D, texid);
    glColor3f(tr, tg, tb);  //texture tint, white to show as-is
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + w, y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + w, y + h);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + h);
    glEnd();
}

//batch draw particles
void drawParticle(float x, float y, float z, float texsize, GLuint texid) {
    //get modelview matrix
    float mv[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    Vector3f up    = { mv[1], mv[5], mv[9] }; // Up vector
    Vector3f right = { mv[0], mv[4], mv[8] }; // Right vector
    right = right * (texsize * 0.5f);
    up    = up * (texsize * 0.5f);
    //build vertices
    Vector3f pos = {x, y, z};
    Vector3f quad[4];
    quad[0] = pos - right - up; // Bottom-left
    quad[1] = pos + right - up; // Bottom-right
    quad[2] = pos + right + up; // Top-right
    quad[3] = pos - right + up; // Top-left
    //draw
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texid);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(quad[0].x, quad[0].y, quad[0].z);
    glTexCoord2f(1, 0); glVertex3f(quad[1].x, quad[1].y, quad[1].z);
    glTexCoord2f(1, 1); glVertex3f(quad[2].x, quad[2].y, quad[2].z);
    glTexCoord2f(0, 1); glVertex3f(quad[3].x, quad[3].y, quad[3].z);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

//draw 2d bitmap text to screen
void drawBmText(const char* text, float x, float y, float red, float green, float blue, void* font = GLUT_BITMAP_8_BY_13) {
    //draw text
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(red, green, blue);
    glRasterPos2f(x, y);
    while (*text) glutBitmapCharacter(font, *text++);
}

//draw 2d bitmap text with a outer glow
void drawBmGlowText(const char* text, float x, float y, float red, float green, float blue, float gred, float ggreen, float gblue, void* font = GLUT_BITMAP_8_BY_13) {
    drawBmText(text, x+1, y, gred, ggreen, gblue, font);
    drawBmText(text, x-1, y, gred, ggreen, gblue, font);
    drawBmText(text, x, y+1, gred, ggreen, gblue, font);
    drawBmText(text, x, y-1, gred, ggreen, gblue, font);
    drawBmText(text, x, y, red, green, blue, font);
}

//audio
ma_engine soundengine;
ma_sound bgm;

//play background music loop
void playBgm(std::string path) {
    ma_sound_uninit(&bgm);
    ma_sound_init_from_file(&soundengine, path.c_str(), 0, NULL, NULL, &bgm);
    ma_sound_set_looping(&bgm, MA_TRUE);
    ma_sound_start(&bgm);
}

//!play one-shot sound effects
void playSfx(std::string path) {
    ma_engine_play_sound(&soundengine, path.c_str(), NULL);
}

//!rotate about relative pivot, you can use this
void pivotRotatef(float deg, float rx, float ry, float rz, float px, float py, float pz) {
    glTranslatef(px, py, pz);
    glRotatef(deg, rx, ry, rz);
    glTranslatef(-px, -py, -pz);
}

//!useful math funcs
//linear interpolation
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float randf(float mn=0, float mx=1) {
    return (float)rand() / RAND_MAX * (mx - mn) + mn;
}

int randi(int mn, int mx) {
    return mn + rand() % (mx - mn + 1);
}

#endif // TOOLS
