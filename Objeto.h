//
// Created by hgallegos on 5/10/2022.
//

#ifndef LEARNOPENGL_OBJETO_H
#define LEARNOPENGL_OBJETO_H
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader_m.h"
using namespace std;
using namespace glm;

class Objeto {
public:
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    vector<GLuint> indices;
    GLuint indices_size;
    GLuint vao;
    mat4 model;
    bool visible=true;
    GLint POSITION_ATTRIBUTE=0, NORMAL_ATTRIBUTE=1, TEXCOORD0_ATTRIBUTE=8;
    vec3 vel_ini, pos_ini, dir;
    float ang_ini;
    float tiempo_ini, tiempo_trans;
    virtual GLuint setup()=0;
    virtual void display(Shader &sh)=0;
    virtual void actualizarDatos(float t)=0;
};

class Esfera:public Objeto{
public:
    vec3 centro;
    float radius;
    int slices, stacks;
    vec3 topRight, bottomLeft;
    Esfera() {
        centro = vec3(0.0);
        pos_ini = centro;
        vel_ini = vec3(0);
    }
    Esfera(vec3 _centro) {
        centro = _centro;
        pos_ini = centro;
        vel_ini = vec3(0);
    }
    Esfera(vec3 _centro, float _radius, int _slices, int _stacks) {
        centro = _centro;
        pos_ini = centro;
        vel_ini = vec3(0);
        radius = _radius;
        slices = _slices;
        stacks = _stacks;
        topRight = vec3(centro.x + radius, centro.y + radius, centro.z + radius);
        bottomLeft = vec3(centro.x - radius, centro.y - radius, centro.z - radius);
    }
    GLuint setup();

    void display(Shader &sh);
    void actualizarDatos(float t);
    void updateCorners(){
        topRight = vec3(centro.x + radius, centro.y + radius, centro.z + radius);
        bottomLeft = vec3(centro.x - radius, centro.y - radius, centro.z - radius);
    }
};

#endif //LEARNOPENGL_OBJETO_H
