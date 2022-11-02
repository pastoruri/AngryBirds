#ifndef GLUT_PLY_H
#define GLUT_PLY_H
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Objeto.h"
#include "shader_m.h"
using namespace std;
using namespace glm;

class Model_PLY: public Objeto {
public:
	Model_PLY();
	vec3 centro;
    //vector<vec3> positions;
    //vector<vec3> normals;
    //vector<GLuint> indices;
    //GLuint indices_size;
    //GLuint vao;
    //GLint POSITION_ATTRIBUTE=0, NORMAL_ATTRIBUTE=1;
    int		Load(char *filename);
    void    imprimir();
    int     enviar_GPU();
    GLuint  setup() { return enviar_GPU(); }
    void    display(Shader &sh);
    void    actualizarDatos(float t);
	// Position min and max for AABB
    //	Vector3f posMin, posMax;
};

#endif
