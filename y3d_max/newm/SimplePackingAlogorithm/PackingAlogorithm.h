#pragma once
#include <iostream>
#include <vector>
#include "MyRectangle.h"
#include <algorithm>
#include <chrono>
#include <list>

using namespace std;

class Time {
public:
	Time() : time(std::chrono::steady_clock::now()) {};
	long long duration() {
		auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds>
			(std::chrono::steady_clock::now() - time);
		return elapsed.count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> time;
};

class AreaRect
{
public:
	float x;
	float y;
	float w;
	float h;
	bool isFree;

	AreaRect(float width, float height, float x = -1, float y = -1, bool isFree = true) :w(width), h(height), x(x), y(y), isFree(isFree)
	{
	};

	AreaRect()
	{
	};

	~AreaRect() {};

	void print() {
		cout << "x = " << x << " y = " << y << " width = " << w << " height = " << h << " isFree = " << isFree<<endl;
	}
};


class PackingAlogorithm
{
public:
	int aw;
	int ah;

	PackingAlogorithm() :ah(1), aw(1) {};
	~PackingAlogorithm() {};

	void printListArea(vector<vector<AreaRect>>& listArea)
	{
		int h = listArea.size();
		int w = listArea[0].size();
		for (size_t j = 0; j < h; j++)
			for (size_t i = 0; i < w; i++)
			{
				auto& area2 = listArea[j][i];
				area2.print();
			}
	};

	int findHeightStep(int i, int j,MyRectangle& rect, vector<vector<AreaRect>>& listArea,AreaRect& area )
	{
		if (area.h >= rect.h)
			return 0;
		else
		{
			auto needHeight = rect.h;
			auto currentHeight = area.h;
			auto t = i + 1;
			auto stepHeight = 1;

			AreaRect* areaRect = nullptr;
			if (t < ah)
			{
				areaRect = &listArea[t][j];
			}

			while (needHeight > currentHeight && areaRect!=nullptr && areaRect->isFree )
			{
				currentHeight = currentHeight + areaRect->h;
				if (currentHeight >= needHeight)
					return stepHeight;
				else
				{
					t++;
					stepHeight++;
					if (t < ah)
						areaRect = &listArea[t][j];
					else
						areaRect = nullptr;
				}
			}
		}

		return -1;
	};

	bool hasStepHeight(int i, int j, vector<vector<AreaRect>>& listArea, int heightStep)
	{
		for (size_t step = 1; step < heightStep + 1; step++)
		{
			if (listArea[i + step][j].isFree == false) 
				return false;
		}
		return true;
	}

	int findWidthStep(int i, int j, MyRectangle& rect, vector<vector<AreaRect>>& listArea, AreaRect& area,int heightStep)
	{
		if (area.w >= rect.w)
			return 0;
		else
		{
			auto needWidth = rect.w;
			auto currentWidth = area.w;
			AreaRect* areaRect = nullptr;
			auto t = j + 1;
			auto stepWidth = 1;
			if (t < aw)
				areaRect = &listArea[i][t];
			while (currentWidth < needWidth && areaRect != nullptr && areaRect->isFree == true && hasStepHeight(i,t,listArea,heightStep))
			{
				currentWidth = currentWidth + areaRect->w;
				if (currentWidth >= needWidth)
					return stepWidth;
				else
				{
					stepWidth++;
					t++;
					if (t < aw)
						areaRect = &listArea[i][t];
					else 
						areaRect = nullptr;
				}
			}
		}
		return -1;
	}

	bool findPlace(MyRectangle& rect, const MyRectangle& enclosingrect, vector<vector<AreaRect>>& listArea, float* result)
	{
		for (size_t j = 0; j < aw; j++)
		{
			for (size_t i = 0; i < ah; i++)
			{
				auto& area = listArea[i][j];
				if (area.x + rect.w > enclosingrect.w)
				{
					return false;
				}
				if (area.isFree)
				{
					auto heightStep = findHeightStep(i, j, rect, listArea, area);
					if (heightStep > -1)
					{
						auto widthStep = findWidthStep(i, j, rect, listArea, area, heightStep);
						if (widthStep > -1)
						{
							result[0] = area.x;
							result[1] = area.y;
							result[2] = i;
							result[3] = j;
							result[4] = heightStep + i;
							result[5] = widthStep + j;
							
							return true;
						};
					}
				}
			}
		}
		return false;
	};

	void updateListArea(vector<vector<AreaRect>>& listArea,const MyRectangle& rect,int brow,int bcol, int erow, int ecol,bool print = false)
	{
		auto area = listArea[0][ecol];
		auto maxWidthPos = area.x + area.w;
		auto maxRectWidthPos = rect.x + rect.w;
		auto w2 = maxWidthPos - maxRectWidthPos;
		if (w2 > 0)
		{
			auto w1 = maxRectWidthPos - area.x;
			aw++;
			for (int i = 0; i < ah; i++)
			{
				auto& preArea = listArea[i][ecol];
				preArea.w = w1;
				auto x = preArea.x + w1;
				auto y = preArea.y;
				auto h = preArea.h;
				auto isFree = preArea.isFree;
				auto& row = listArea[i];
				row.emplace(row.begin() + ecol + 1, w2, h , x , y , isFree);
			}
		}	

		area = listArea[erow][0];
		
		auto maxHeightPos = area.y + area.h;
		auto maxRectHeightPos = rect.y + rect.h;
		auto h2 = maxHeightPos - maxRectHeightPos;
		

		if (h2 > 0)
		{
			auto h1 = maxRectHeightPos - area.y;
			ah++;
			vector<AreaRect> newRow;
			for (int i = 0; i < aw; i++)
			{
				auto& aboveArea = listArea[erow][i];
				aboveArea.h = h1;
				newRow.emplace_back(aboveArea.w, h2, aboveArea.x, aboveArea.y+h1, aboveArea.isFree);
			}
			listArea.insert(listArea.begin() + erow + 1, newRow);
		}	
	
		for (size_t i = brow; i < erow + 1; i++)
		{
			for (size_t j = bcol; j < ecol + 1; j++)
			{
				listArea[i][j].isFree = false;
			}
		}
	};

	std::vector<MyRectangle> pack(std::vector<MyRectangle>& listRect,const MyRectangle& enclosingRect)
	{
		std::sort(listRect.begin(), listRect.end(), [](MyRectangle& a,MyRectangle& b) {
			return b.h < a.h;
		});

		vector<vector<AreaRect>> listArea;
		listArea.reserve(100000);
		listArea.emplace_back(1, AreaRect(enclosingRect.w, enclosingRect.h, 0, 0, true));
		//(1, std::vector<AreaRect>(1, AreaRect(enclosingRect.w, enclosingRect.h, 0, 0, true)));

		float places[6];
		for (size_t i = 0; i < listRect.size(); i++)
		{
			auto& rect = listRect[i];
			//Time tt;
			if (findPlace(rect, enclosingRect, listArea, places))
			{
				rect.x = places[0];
				rect.y = places[1];
				//if(tt.duration() >10)
				//	cout << "time find place: " << tt.duration()<<" i:"<<i<<" rect:x "<<rect.x<<" rect.y: "<<rect.y<<endl;
				//tt = Time();
				//Time t;
				updateListArea(listArea, rect, places[2], places[3], places[4], places[5]);
				//if (t.duration() > 1) {
				//	cout << "time update list area : " << t.duration() <<" i: "<<i<< " rect:x " << rect.x << " rect.y: " << rect.y << endl;
				//}
				
			}
		}

		return listRect;
	}
};
