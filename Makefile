make:
	g++ -std=c++2a -framework OpenGL -lGL -lglfw -lGLESv2 main.cpp Objeto.cpp glut_ply.cpp