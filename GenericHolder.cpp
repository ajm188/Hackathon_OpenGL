#include "GenericHolder.h"


GenericHolder::GenericHolder() : objs(0)
{
}


GenericHolder::~GenericHolder()
{
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);
}

void GenericHolder::addObj(GenericObject *obj){
	objs.push_back(obj);
	obj->parent = this;
	obj->updateOldPos();
}

GLuint GenericHolder::setupDrawing(){
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);//this is bound to the geometry setup in the next call
	vbo = setupGeometry();

	shaderProgram = glCreateProgram();
	vertexShader = setupVertexShader();
	glAttachShader(shaderProgram, vertexShader);
	fragmentShader = setupFragmentShader();
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);


	uniTrans = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(view));
	return shaderProgram;
}
GLuint GenericHolder::setupGeometry(){
	GLuint vbo;
	float vertices[] = {
		0.0f, 0.0f, // Vertex 1 (X, Y)
		100.0f, 0.0f, // Vertex 2 (X, Y)
		0.0f, 100.0f  // Vertex 3 (X, Y)
	};
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	return vbo;
}
GLuint GenericHolder::setupVertexShader(){
	const GLchar* vertexSource =
		"#version 330 core\n"
		"in vec2 position;"
		"uniform mat4 trans;"
		"uniform mat4 view;"
		"void main() {"
		"   gl_Position = view*trans*vec4(position, 0.0, 1.0);"//xyzw
		"}";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	checkShader(vertexShader);
	return vertexShader;
}

GLuint GenericHolder::setupFragmentShader(){
	const GLchar* fragmentSource =
		"#version 330 core\n"
		"out vec4 outColor;"
		"void main() {"
		"   outColor = vec4(1.0, 0.0, 0.0, 1.0);"//RGBA
		"}";
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	checkShader(fragmentShader);
	return fragmentShader;
}

void GenericHolder::updateAll()
{
	for (std::vector<GenericObject*>::iterator it = objs.begin(); it != objs.end(); ++it){
		(*it)->updateOldPos();
		(*it)->update();
	}
}
#include <glm/gtx/matrix_interpolation.hpp>
void GenericHolder::drawAll()
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	for (std::vector<GenericObject*>::iterator it = objs.begin(); it != objs.end(); ){
		GenericObject* itObj = *it;
		if (itObj->markedForDeath){
			delete itObj;
			it = objs.erase(it);
		}
		else
		{
			double reps = (itObj->distMoved()) + 1;
			auto rotation = (itObj)->rotation();
			auto newTrans = ((itObj)->translate())*rotation;
			auto oldTrans = ((itObj)->translateOld())*rotation;
			for (int i = 0; i <= reps; i++){
				GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");
				glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(glm::interpolate(oldTrans, newTrans, i / (float)reps)));
				draw();
			}
			++it;
		}
	}
}
void GenericHolder::drawOne(double x, double y)
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");

	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3((float)x, (float)y, 0.0));
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));
	draw();
}