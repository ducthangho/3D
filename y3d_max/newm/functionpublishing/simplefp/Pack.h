#pragma once

#include "MyRectangle.h"
#include <vector>
#include <algorithm>
#include "maxscript\maxscript.h"
using namespace std;

#define EPSILON 0.000001
template <class A,class B>
bool equals(A a, B b) {
	return abs(a - b) < EPSILON;
}

template <class A,class B>
bool lt(A a, B b) {
	return a<b && abs(a - b) > EPSILON;
}

class Node
{
public:
	Node* child[2];
	float top;
	float left;
	float width;
	float height;

	bool isFree;
	
	Node() :isFree{ true } {
		child[0] = nullptr;
		child[1] = nullptr;
	};

	Node(float _top = 0, float _left = 0, float _width = 0, float _height = 0) :top{ _top }, left{ _left }, width(_width), height(_height), isFree(true)
	{
		child[0] = nullptr;
		child[1] = nullptr;
	}
	~Node()
	{
		if (child[0]) delete child[0];
		if (child[1]) delete child[1];
		child[0] = nullptr;
		child[1] = nullptr;
	};

	Node* Insert(MyRectangle& rec) {
		if (child[0] != nullptr)
		{
			Node* ptr = child[0]->Insert(rec);
			if (ptr != nullptr) return ptr;
			
			return child[1]->Insert(rec);
		}
		else
		{
			if (isFree == false) return nullptr;  
			if (lt(width , rec.w) || lt(height , rec.h)) return nullptr;
			if (equals(width,rec.w) && equals(height,rec.h))
			{
				isFree = false;
				rec.x = top;
				rec.y = left;
				return this;
			}

			auto differWidth = width - rec.w;
			auto differHeight = height - rec.h;
			if (differWidth > differHeight) //seperate by width
			{
				child[0] = new Node(top,left,rec.w,height);				
				child[1] = new Node(top, left + rec.w, differWidth, height);
			}
			else  //seperate by height
			{
				child[0] = new Node(top, left, width, rec.h);
				child[1] = new Node(top + rec.h, left, width, differHeight);
			}

			return child[0]->Insert(rec);
		}
	}

	void pack(vector<MyRectangle>& vctRect,Node& enclosingRect)
	{
		std::sort(vctRect.begin(), vctRect.end(), [](MyRectangle& a, MyRectangle& b) {
			return b.h < a.h;
		});
		
		for (auto& rect : vctRect)
		{
			enclosingRect.Insert(rect);
		}
	}

	void packWithNoPreSort(vector<MyRectangle>& vctRect, Node& enclosingRect)
	{
		for (auto& rect : vctRect)
		{
			enclosingRect.Insert(rect);
		}
	}
};


class Pack
{
public:
	Pack() {};
	~Pack() {};
};