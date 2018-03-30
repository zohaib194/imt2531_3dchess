#include "../header/globalVar.hpp"
#include "../header/gl_setup.hpp"
#include "../header/errorHandler.hpp"
#include "../class/model.hpp"
#include "../header/global_function.hpp"
#include "../header/piece.hpp"
#include "../header/glfw_setup.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <stdio.h>

int main(int argc, char const *argv[])
{	
	getMoves();

	// OpenGL setup
	GLFWwindow* window = glfw_setup();

	Model* chessBoard = new Model(("../asset/modell_chessBoard.obj"));
	
	Shader* shaderProgram = new Shader("../shader/vertex.vert", "../shader/fragment.frag");
	
	while(!glfwWindowShouldClose(window)) {

		glClearColor(0.0,0.0,0.0,1);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram->bind();

		static float time = 0.0;
		time += .01;
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(3.14f / 3.0f, (GLfloat)1024 / (GLfloat)768, 0.1f, -10.0f);

		std::map<std::string, GLuint> uniforms = shaderProgram->getUniform(	std::map<std::string, GLchar*>({
			{"viewID", "view"},
			{"projectionID", "projection"},
			{"modelID", "model"},
			{"normalMatrixID", "normalMatrix"},
		}));

		glUniformMatrix4fv(uniforms["viewID"], 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniforms["projectionID"], 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 modelm;
		modelm = glm::translate(modelm, glm::vec3(0.0f, -1.75f, -1.0f)); // translate it down so it's at the center of the scene
		modelm = glm::scale(modelm, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
																//glm::mat4 model = glm::rotate(glm::mat4(), time, glm::vec3(0, 1, 0));
		//model = glm::rotate(model, time, glm::vec3(0, 1, 0));														//ourShader.setMat4("model", model);
		glUniformMatrix4fv(uniforms["modelID"], 1, GL_FALSE, glm::value_ptr(modelm));
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view*modelm)));
		glUniformMatrix3fv(uniforms["normalMatrixID"], 1, GL_FALSE, glm::value_ptr(normalMatrix));
		
		chessBoard->Draw(*shaderProgram); 
		 //printf("hello\t");

		glfwSwapBuffers(window);    //<-- SWAP BUFFERS
        glfwPollEvents();           //<-- LISTEN FOR WINDOW EVENTS
	}
	glfwDestroyWindow(window);

	return 0;
}
 
