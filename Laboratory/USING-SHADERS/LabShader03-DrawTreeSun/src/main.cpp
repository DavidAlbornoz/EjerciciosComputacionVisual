//============================================================================
// Name        : Usamos los Shaders para dise�ar una figura 2D
// Professor   : Herminio Paucar
// Version     :
// Description : Utilizamos los Vertex y Fragment Shaders
//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include "Utils.h"

#define W_WIDTH 800
#define W_HEIGHT 800

GLuint renderingProgram;

GLfloat* m_Vertices;
GLuint n_Vertices;
GLuint m_VBO;
GLuint m_VAO;

using namespace std;

void init (GLFWwindow* window) {

	// Utils
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// The first 3 points are to Vertex position of Triangle
    n_Vertices = 18;
    m_Vertices = new GLfloat[18] {
			-1.0f, -1.0f, 0.0f, //Triangle 01
			-1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, //Triangle 02
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
	};

	// Cria um ID na GPU para nosso buffer
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa �rea de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			18 * sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW		// Pol�tica de acesso aos dados, para otimiza��o
		);

	// Nesse ponto, ja copiamos nossos dados pra GPU.
	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(GLvoid*) 0
		);

	glEnableVertexAttribArray(0);	// Habilita este atributo

	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    //Obtiene el valor de la variable uniforme "resize"
    GLuint resizeVal = glGetUniformLocation(renderingProgram, "resize");
    //Se va actualizar un vector de 2 valores
	int op = (int) currentTime % 3;
	if (op == 0)
		glProgramUniform2f(renderingProgram, resizeVal, 0.25 * W_WIDTH,	0.25 * W_HEIGHT);
	else if (op == 1)
		glProgramUniform2f(renderingProgram, resizeVal, 0.5 * W_WIDTH, 0.5 * W_HEIGHT);
	else
		glProgramUniform2f(renderingProgram, resizeVal, W_WIDTH, W_HEIGHT);

	// Use este VAO e suas configura��es
	glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}

int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "LabShader03: Drawing Tree with Shader", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        init(window);
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
