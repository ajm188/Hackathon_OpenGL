#include "FollowerHolder.h"

FollowerHolder::FollowerHolder()
{
	addObj(new Follower());

}

FollowerHolder::~FollowerHolder()
{
}


GLuint FollowerHolder::setupGeometry(){
	GLuint vbo;
	const int count = 62;
	float vertices[count];
	vertices[0] = 0.0f; vertices[1] = 0.0f;
	for (int i = 2; i < count; i += 2){
		vertices[i] = Dcos(i * 360 / (count-4))*((i % 4) * 50 + 50);
		vertices[i + 1] = Dsin(i * 360 / (count-4))*((i % 4)*50+50);
	}
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	return vbo;
}