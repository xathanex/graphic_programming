#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

#include <ctime>

using namespace glm;
GLFWwindow* window;

int main()
{
	srand(time(0));
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
	glfwSetCursorPos(window, 1024/2, 768/2);

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
		//-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		 
		-1.0f, -1.0f, 1.0f,
		 //0.0f,  1.0f, 0.0f,
		 
		 //1.0f, -1.0f, 1.0f,
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
	
	static GLfloat g_color_buffer_data[(6*3+6*4+3*9)*3];
	for(int i = 0; i < (6*3+6*4+3*9)*3; ++i){ g_color_buffer_data[i] = (double)rand()/RAND_MAX; }
	
	static GLfloat g_uv_buffer_data[] = { 
		0.000059f, 1.0f-0.000004f, 
		0.000103f, 1.0f-0.336048f, 
		0.335973f, 1.0f-0.335903f, 
		1.000023f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.336024f, 1.0f-0.671877f, 
		0.667969f, 1.0f-0.671889f, 
		1.000023f, 1.0f-0.000013f, 
		0.668104f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.000059f, 1.0f-0.000004f, 
		0.335973f, 1.0f-0.335903f, 
		0.336098f, 1.0f-0.000071f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.336024f, 1.0f-0.671877f, 
		1.000004f, 1.0f-0.671847f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.668104f, 1.0f-0.000013f, 
		0.335973f, 1.0f-0.335903f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.668104f, 1.0f-0.000013f, 
		0.336098f, 1.0f-0.000071f, 
		0.000103f, 1.0f-0.336048f, 
		0.000004f, 1.0f-0.671870f, 
		0.336024f, 1.0f-0.671877f, 
		0.000103f, 1.0f-0.336048f, 
		0.336024f, 1.0f-0.671877f, 
		0.335973f, 1.0f-0.335903f, 
		0.667969f, 1.0f-0.671889f, 
		1.000004f, 1.0f-0.671847f, 
		0.667979f, 1.0f-0.335851f
	};
	for(int i = 0; i < 36*2; ++i){ g_uv_buffer_data[i] = g_uv_buffer_data[i]*0.1; }
	
	static const GLushort g_element_buffer_data[] = { 0, 1, 2 };
	
	GLuint Texture = loadBMP_custom("uvtemplate.bmp");
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	
	/*
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	*/

	GLuint texturebuffer;
	glGenBuffers(1, &texturebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
	do{
		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		/*glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);*/
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glm::mat4 tempMVP = MVP*glm::translate(Model, glm::vec3(0.7f, 0.0f, 3.5f));
		for(int i=0; i < 4; ++i)
		{
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(tempMVP)[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, 3);
			tempMVP = tempMVP * Rotate2;
		}
		
		tempMVP = MVP*Translate;
		for(int i=0; i < 4; ++i)
		{
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(tempMVP)[0][0]);

			glDrawArrays(GL_LINES, 3, 8);
			tempMVP = tempMVP * Rotate2;
		}
		
		tempMVP = MVP;
		tempMVP = tempMVP*Translate*Translate;
		for(int i=0; i < 8; ++i)
		{
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(tempMVP)[0][0]);

			glDrawArrays(GL_LINE_STRIP, 11, 9);
			tempMVP = tempMVP * Rotate3;
		}
		tempMVP = MVP;
		tempMVP = tempMVP*glm::rotate(Model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f))*Translate*Translate;
		for(int i=0; i < 8; ++i)
		{
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(tempMVP)[0][0]);

			glDrawArrays(GL_LINE_STRIP, 14, 9);
			tempMVP = tempMVP * Rotate3;
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

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

