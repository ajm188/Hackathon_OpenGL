#include "badDesign.h"
#include "PlayerHolder.h"
#include "FollowerHolder.h"
#include "ZombieHolder.h"
#include "MothershipHolder.h"
#include "LineTripperHolder.h"
#include "CarrierHolder.h"
#include "MissileHolder.h"
#include "Spawner.h"
#include "TextDisplay.h"

using namespace std;
 glm::mat4 view;
 GLFWwindow* window;
 GenericHolder* holderArray[numTypes];
 bool gameOver;
 unsigned long long score;

 void checkShader(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		std::cout << buffer;
	}
}
 void checkErrors()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR){
		;
	}
}
GLFWwindow* setupDrawing(){
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(width, height, "Tale of Two Circles", NULL, NULL); // Fullscreen
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	return window;
}
int main()
{
	gameOver = false;
	score = 0;

	window = setupDrawing();

	view = glm::translate(view, glm::vec3(-1, 1, 0));
	view = glm::scale(view, glm::vec3(2.0 / width, -2.0 / height, 1.0));

	Spawner* spawnController = new Spawner();
	TextDisplay* text = new TextDisplay();

	auto maxtime = chrono::milliseconds(1000 / 60);
	holderArray[PLAYER] = new PlayerHolder();
	holderArray[FOLLOWER] = new FollowerHolder();
	holderArray[ZOMBIE] = new ZombieHolder();
	holderArray[MOTHERSHIP] = new MothershipHolder();
	holderArray[LINETRIPPER] = new LineTripperHolder();
	holderArray[CARRIER] = new CarrierHolder();
	holderArray[MISSILE] = new MissileHolder();

	for (int i = 0; i < numTypes; i++){
		holderArray[i]->setupDrawing();
	}

	while (!glfwWindowShouldClose(window) && !gameOver)
	{
		chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

		checkErrors();
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		spawnController->tick();

		for (int i = 0; i < numTypes; i++){
			holderArray[i]->updateAll();
		}

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		for (int i = 0; i < numTypes; i++){
			holderArray[i]->drawAll();
		}
		text->drawNumber(50, 50, score);
		text->drawNumber(52, 52, score);
		if (spawnController->mode == Spawner::NO_MODE)
		{
			text->drawChar(100, 500, 'N');
			holderArray[ZOMBIE]->drawOne(100, 550);
			holderArray[MOTHERSHIP]->drawOne(100, 570);
			holderArray[LINETRIPPER]->drawOne(100, 590);
			holderArray[CARRIER]->drawOne(100, 610);
			text->drawChar(300, 500, 'R');
			holderArray[CARRIER]->drawOne(300, 550);
			holderArray[MOTHERSHIP]->drawOne(300, 570);
			text->drawChar(500, 500, 'Z');
			holderArray[ZOMBIE]->drawOne(500, 530);
			holderArray[ZOMBIE]->drawOne(510, 550);
			holderArray[ZOMBIE]->drawOne(490, 560);
			holderArray[LINETRIPPER]->drawOne(500, 590);
		}
		glfwSwapBuffers(window);

		if (!gameOver && spawnController->mode != Spawner::NO_MODE){
			score++;
		}
		auto time_span = chrono::steady_clock::now() - t1;
		if ((maxtime - time_span).count() < 0){
			cout << (maxtime - time_span).count() << endl;
		}
		this_thread::sleep_for(maxtime - time_span);
	}
	//game over screen
	int counter = 0;
	while (!glfwWindowShouldClose(window))
	{	
		chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

		checkErrors();
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		for (int i = 0; i < numTypes; i++){
			holderArray[i]->drawAll();
		}
		if (counter++ < 45){
			text->drawNumber(50, 50, score);
			text->drawNumber(52, 52, score);
		}
		else if (counter >= 80){ counter = 0; }
		glfwSwapBuffers(window);
		auto time_span = chrono::steady_clock::now() - t1;
		this_thread::sleep_for(maxtime - time_span);
	}

	glfwTerminate();
}
