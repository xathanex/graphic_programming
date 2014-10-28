#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>

using namespace glm;
GLFWwindow* window;

int main()
{
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1024, 768, "Tutorial 03 - Matrices", NULL, NULL);
	if( window == NULL ){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if(glewInit() != GLEW_OK){
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders("SimpleTransform.vertexshader", "SingleColor.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	glm::mat4 View       = glm::lookAt(
								glm::vec3(0,0,10),
								glm::vec3(0,0,0),
								glm::vec3(0,1,0)
						   );

	glm::mat4 Model      = glm::mat4(1.0f);
	glm::mat4 Rotate0     = glm::rotate(Model, 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Rotate1     = glm::rotate(Model, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Rotate2     = glm::rotate(Model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Rotate3     = glm::rotate(Model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Translate		= glm::translate(Model, glm::vec3(3.0f, 0.0f, 0.0f));
	glm::mat4 MVP        = Projection * View * Model * Rotate0;
	
	static const GLfloat g_vertex_buffer_data[] = { 
		//pyramid
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		 
		-1.0f, -1.0f, 1.0f,
		 0.0f,  1.0f, 0.0f,
		 
		 1.0f, -1.0f, 1.0f,
		 0.0f,  1.0f, 0.0f,
		 
		//box
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,

		-1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		 
		-1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,
		
		 1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		
		//sphere
		 0.0f,  1.0f, 0.0f,
		 1.41f/2, 1.41f/2, 0.0f,
		 1.0f,  0.0f, 0.0f,
		 1.41f/2, -1.41f/2, 0.0f,
		 0.0f, -1.0f, 0.0f,
		-1.41f/2, -1.41f/2, 0.0f,
		-1.0f,  0.0f, 0.0f,
		-1.41f/2, 1.41f/2, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};
	static const GLushort g_element_buffer_data[] = { 0, 1, 2 };

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);

		glm::mat4 tempMVP = MVP;
		for(int i=0; i < 4; ++i)
		{
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(tempMVP)[0][0]);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_LINES, 0, 6);
			tempMVP = tempMVP * Rotate2;
		}
		
		tempMVP = MVP*Translate;
		for(int i=0; i < 4; ++i)
		{
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(tempMVP)[0][0]);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_LINES, 6, 8);
			tempMVP = tempMVP * Rotate2;
		}
		
		tempMVP = MVP;
		tempMVP = tempMVP*Translate*Translate;
		for(int i=0; i < 8; ++i)
		{
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(tempMVP)[0][0]);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_LINE_STRIP, 14, 9);
			tempMVP = tempMVP * Rotate3;
		}
		tempMVP = MVP;
		tempMVP = tempMVP*glm::rotate(Model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f))*Translate*Translate;
		for(int i=0; i < 8; ++i)
		{
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(tempMVP)[0][0]);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_LINE_STRIP, 14, 9);
			tempMVP = tempMVP * Rotate3;
		}

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();

	return 0;
}

