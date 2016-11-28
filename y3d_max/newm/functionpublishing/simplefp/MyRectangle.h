#pragma once
class MyRectangle
{
public:
	float x, y;
	float w, h;
	
	MyRectangle::MyRectangle(float width, float height) :w(width), h(height), x(-1), y(-1)
	{
	}

	~MyRectangle() {};

	void print()
	{
		printf("x = %f, y= %f, width = %f, height = %f\n", x, y, w, h);
	};
};


