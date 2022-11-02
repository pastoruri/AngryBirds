
#include <GLES3/gl3.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include "shader_m.h"
#include "camera.h"
#include "Objeto.h"
#include "glut_ply.h"
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
float tiempoInicial = 0.0f, tiempoTranscurrido = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Model_PLY modelo;
char *archivo = (char*)("models/bunny.ply");

Esfera esfera(vec3(0),2., 100, 100);
Objeto *pModelo = new Esfera(vec3(0),2, 50, 50);
vector<Objeto*> vecObjetos;
vector<Objeto*> vecObjetosStatic;
bool proyectil_listo = false;
//Model_PLY *proyectil = new Model_PLY;

//funciones
bool collision(Esfera* pelota, Esfera* pelota2){
    float dist = glm::distance(pelota->centro, pelota2->centro);
    if (pelota->radius + pelota2->radius >= dist){
        return true;
    } 
    return false;
}

int main() {
    modelo.Load(archivo);

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)     {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    Shader lightingShader("2.2.basic_lighting.vs", "2.2.basic_lighting.fs");
    //Shader lightCubeShader("../2.2.light_cube.vs", "../2.2.light_cube.fs");

    esfera.vao = esfera.setup();
    pModelo->setup();
    modelo.setup();
    
    
    Model_PLY modelo2 = modelo;
    Model_PLY modelo3 = modelo;
    Model_PLY modelo4 = modelo;
    Esfera esfera2 = esfera;
    Esfera esfera3 = esfera;
    Esfera esfera4 = esfera;
    vecObjetosStatic.push_back(&esfera2);
    vecObjetosStatic.push_back(&esfera3);
    vecObjetosStatic.push_back(&esfera4);
   
    modelo.centro.x = 0.5;
    modelo2.centro.x = 1;
    //modelo3.centro.x = 1.5;
    esfera2.centro.x = 20;
    esfera3.centro.x = 30;
    esfera4.centro.x = 10;

    modelo.centro.y = -0.1;
    modelo2.centro.y = -0.1;
    //modelo3.centro.y = -0.1;

    esfera4.visible = false;
 
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        for (auto &obj : vecObjetos) {
            obj->tiempo_trans = currentFrame - obj->tiempo_ini;
        }
        //tiempoTranscurrido = currentFrame - tiempoInicial; //static_cast<float>(glfwGetTime());
        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        //lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("objectColor", 0.0f, 1.0f, 0.0f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

       
        esfera2.display(lightingShader);
        esfera3.display(lightingShader);
        esfera4.display(lightingShader);
        //pModelo->display(lightingShader);
        modelo.display(lightingShader);
        modelo2.display(lightingShader);
        //modelo3.display(lightingShader);
        modelo4.display(lightingShader);

        lightingShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        esfera.display(lightingShader);

        for (auto &obj : vecObjetos) {
            obj->actualizarDatos(obj->tiempo_trans);
            // calcular boundingvolume
            // calcular colisiones
            obj->display(lightingShader);
        }

         //Check collisions 
        for (auto obj : vecObjetos) {
            if (auto bola = dynamic_cast<Esfera*>(obj)){
                bola->updateCorners();
                //cout<<bola->centro.x<<' '<<bola->centro.y<<' '<<bola->centro.z<<'\n';
                if(bola->centro.y < -5){ 
                    bola->visible = false;
                    //vecObjetos.erase(vecObjetos.begin());
                    //delete bola;
                }
                //Collide(*bola, esfera2);
                for (auto obj2 : vecObjetosStatic){
                    if (auto bola2 = dynamic_cast<Esfera*>(obj2)){
                        //bola2->updateCorners();
                        if(collision(bola, bola2)){
                            bola2->visible=false;
                            if(bola2 == &esfera4){
                                modelo = modelo2;
                            }
                        }
                    }
                }
            }
        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
 //   glDeleteVertexArrays(1, &cubeVAO);
 //   glDeleteVertexArrays(1, &lightCubeVAO);
 //   glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

float vel_ini_speed = 14;
float ang_ini_glob = 45;
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * 3);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * 3);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * 3);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * 3);

    if (glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS) {
        vel_ini_speed  += 2 * deltaTime;
        cout<<vel_ini_speed<<'\n';
    }

    if (glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS) {
        vel_ini_speed  -= 2 * deltaTime;
        cout<<vel_ini_speed<<'\n';
    }

    if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS) {
        ang_ini_glob  += 2 * deltaTime;
        cout<<ang_ini_glob<<'\n';
    }

    if (glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS) {
        ang_ini_glob  -= 2 * deltaTime;
        cout<<ang_ini_glob<<'\n';
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (!proyectil_listo){
            Objeto *proyectil = new Esfera;
            // float x = rand()%10;
            // float y = rand()%10;
            // float z = rand()%10;
            proyectil->vao = esfera.vao;
            proyectil->indices_size = esfera.indices_size;
            //proyectil->radius = esfera.radius;
            //proyectil->vao = modelo.vao;
            //proyectil->indices_size = modelo.indices_size;
            //proyectil->centro = pModelo->centro;
            //proyectil->vao = pModelo->vao;
            //proyectil->indices_size = pModelo->vao;
            vecObjetos.push_back(proyectil);
            proyectil_listo = true;
            proyectil->vel_ini = vec3(vel_ini_speed ,vel_ini_speed ,0);
            proyectil->pos_ini = vec3 (0,0,0);//vec3 (x,y,z);
            proyectil->ang_ini = ang_ini_glob;
            proyectil->tiempo_ini = static_cast<float>(glfwGetTime());
            //tiempoInicial = static_cast<float>(glfwGetTime());
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE){
        proyectil_listo = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
