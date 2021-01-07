#pragma once
class vec3f
{
public:
	vec3f();
	~vec3f();

	float x, y, z;

	vec3f(float component1, float component2, float component3);


	vec3f sum(vec3f other);
	vec3f scaledBy(float number);
	void scaleBy(float number);
	float magnitude();
	void normalize();
	vec3f normalized();

	static vec3f randUnitVector();
};

