//#pragma once
//#include <iostream>
//#include <vector>
//#include "myrectangle.h"
//#include <algorithm>
//#include <chrono>
//#include <list>
//
//using namespace std;
//
//
//class time {
//public:
//	time() : time(std::chrono::steady_clock::now()) {};
//	long long duration() {
//		auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds>
//			(std::chrono::steady_clock::now() - time);
//		return elapsed.count();
//	}
//private:
//	std::chrono::time_point<std::chrono::steady_clock> time;
//};
//
//class arearect
//{
//public:
//	float x;
//	float y;
//	float w;
//	float h;
//	bool isfree;
//
//	arearect(float width, float height, float x = -1, float y = -1, bool isfree = true) :w(width), h(height), x(x), y(y), isfree(isfree)
//	{
//	};
//
//	arearect()
//	{
//	};
//
//	~arearect() {};
//
//	void print() {
//		cout << "x = " << x << " y = " << y << " width = " << w << " height = " << h << " isfree = " << isfree << endl;
//	}
//};
//
//
//class packingrectangleuselist
//{
//public:
//	int aw;
//	int ah;
//
//	packingrectangleuselist() :ah(1), aw(1) {};
//	~packingrectangleuselist() {};
//
//	void printlistarea(list<list<arearect>>& listarea)
//	{
//		int h = listarea.size();
//		int w = listarea.front().size();
//		for (std::list<list<arearect>>::iterator itx = listarea.begin();itx!=listarea.end();itx++)
//			for (std::list<arearect>::iterator ity = (*itx).begin();ity!=(*itx).end();ity++)
//			{
//				ity->print();
//			}
//	};
//
//	int findheightstep(int i, int j, myrectangle& rect, vector<vector<arearect>>& listarea, arearect& area)
//	{
//		if (area.h >= rect.h)
//			return 0;
//		else
//		{
//			auto needheight = rect.h;
//			auto currentheight = area.h;
//			auto t = i + 1;
//			auto stepheight = 1;
//
//			arearect* arearect = nullptr;
//			if (t < ah)
//			{
//				arearect = &listarea[t][j];
//			}
//
//			while (needheight > currentheight && arearect != nullptr && arearect->isfree)
//			{
//				currentheight = currentheight + arearect->h;
//				if (currentheight >= needheight)
//					return stepheight;
//				else
//				{
//					t++;
//					stepheight++;
//					if (t < ah)
//						arearect = &listarea[t][j];
//					else
//						arearect = nullptr;
//				}
//			}
//		}
//
//		return -1;
//	};
//
//	bool hasstepheight(int i, int j, vector<vector<arearect>>& listarea, int heightstep)
//	{
//		for (size_t step = 1; step < heightstep + 1; step++)
//		{
//			if (listarea[i + step][j].isfree == false)
//				return false;
//		}
//		return true;
//	}
//
//	int findwidthstep(int i, int j, myrectangle& rect, vector<vector<arearect>>& listarea, arearect& area, int heightstep)
//	{
//		if (area.w >= rect.w)
//			return 0;
//		else
//		{
//			auto needwidth = rect.w;
//			auto currentwidth = area.w;
//			arearect* arearect = nullptr;
//			auto t = j + 1;
//			auto stepwidth = 1;
//			if (t < aw)
//				arearect = &listarea[i][t];
//			while (currentwidth < needwidth && arearect != nullptr && arearect->isfree == true && hasstepheight(i, t, listarea, heightstep))
//			{
//				currentwidth = currentwidth + arearect->w;
//				if (currentwidth >= needwidth)
//					return stepwidth;
//				else
//				{
//					stepwidth++;
//					t++;
//					if (t < aw)
//						arearect = &listarea[i][t];
//					else
//						arearect = nullptr;
//				}
//			}
//		}
//		return -1;
//	}
//
//	/*bool findplace(myrectangle& rect, const myrectangle& enclosingrect, vector<vector<arearect>>& listarea, float* result)
//	{
//		for (size_t j = 0; j < aw; j++)
//		{
//			for (size_t i = 0; i < ah; i++)
//			{
//				auto& area = listarea[i][j];
//				if (area.x + rect.w > enclosingrect.w)
//				{
//					return false;
//				}
//				if (area.isfree)
//				{
//					auto heightstep = findheightstep(i, j, rect, listarea, area);
//					if (heightstep > -1)
//					{
//						auto widthstep = findwidthstep(i, j, rect, listarea, area, heightstep);
//						if (widthstep > -1)
//						{
//							result[0] = area.x;
//							result[1] = area.y;
//							result[2] = i;
//							result[3] = j;
//							result[4] = heightstep + i;
//							result[5] = widthstep + j;
//
//							return true;
//						};
//					}
//				}
//			}
//		}
//		return false;
//	};*/
//
//	bool findplace(myrectangle& rect, const myrectangle& enclosingrect, vector<vector<arearect>>& listarea, float* result)
//	{
//		for (size_t j = 0; j < aw; j++)
//		{
//			for (size_t i = 0; i < ah; i++)
//			{
//				auto& area = listarea[i][j];
//				if (area.x + rect.w > enclosingrect.w)
//				{
//					return false;
//				}
//				if (area.isfree)
//				{
//					auto heightstep = findheightstep(i, j, rect, listarea, area);
//					if (heightstep > -1)
//					{
//						auto widthstep = findwidthstep(i, j, rect, listarea, area, heightstep);
//						if (widthstep > -1)
//						{
//							result[0] = area.x;
//							result[1] = area.y;
//							result[2] = i;
//							result[3] = j;
//							result[4] = heightstep + i;
//							result[5] = widthstep + j;
//
//							return true;
//						};
//					}
//				}
//			}
//		}
//		return false;
//	};
//
//
//	void updatelistarea(vector<vector<arearect>>& listarea, const myrectangle& rect, int brow, int bcol, int erow, int ecol, bool print = false)
//	{
//		auto area = listarea[0][ecol];
//		auto maxwidthpos = area.x + area.w;
//		auto maxrectwidthpos = rect.x + rect.w;
//		auto w2 = maxwidthpos - maxrectwidthpos;
//		if (w2 > 0)
//		{
//			auto w1 = maxrectwidthpos - area.x;
//			aw++;
//			for (int i = 0; i < ah; i++)
//			{
//				auto& prearea = listarea[i][ecol];
//				prearea.w = w1;
//				auto x = prearea.x + w1;
//				auto y = prearea.y;
//				auto h = prearea.h;
//				auto isfree = prearea.isfree;
//				auto& row = listarea[i];
//				row.emplace(row.begin() + ecol + 1, w2, h, x, y, isfree);
//			}
//		}
//
//		if (print) {
//			cout << "-----*****************-----" << endl;
//			printlistarea(listarea);
//			cout << "-----*****************-----" << endl;
//		}
//
//		area = listarea[erow][0];
//
//		if (print) {
//			cout << "-----*********sad0********-----" << endl;
//			printlistarea(listarea);
//			cout << "-----*********sad0********-----" << endl;
//		}
//
//		auto maxheightpos = area.y + area.h;
//		auto maxrectheightpos = rect.y + rect.h;
//		auto h2 = maxheightpos - maxrectheightpos;
//
//
//		if (h2 > 0)
//		{
//			auto h1 = maxrectheightpos - area.y;
//			ah++;
//			vector<arearect> newrow;
//			if (print) {
//				cout << "-----*********sad1********-----" << endl;
//				printlistarea(listarea);
//				cout << "-----*********sad1********-----" << endl;
//			}
//			for (int i = 0; i < aw; i++)
//			{
//				auto& abovearea = listarea[erow][i];
//				abovearea.h = h1;
//				newrow.emplace_back(abovearea.w, h2, abovearea.x, abovearea.y + h1, abovearea.isfree);
//			}
//
//			if (print) {
//				cout << "-----*********sad********-----" << endl;
//				printlistarea(listarea);
//				cout << "-----*********sad********-----" << endl;
//			}
//
//
//			if (print) {
//				cout << "-----*********222222*******-----" << endl;
//				for (size_t i = 0; i < newrow.size(); i++)
//				{
//					auto& a = newrow[i];
//					a.print();
//				}
//				cout << "-----*********222222*******-----" << endl;
//			}
//			listarea.insert(listarea.begin() + erow + 1, newrow);
//		}
//
//		if (print) {
//			cout << "-----*******33333**********-----" << endl;
//			cout << "list area size: " << listarea.size() << " " << listarea[0].size() << endl;
//			printlistarea(listarea);
//			cout << "-----*******33333**********-----" << endl;
//		}
//
//
//		for (size_t i = brow; i < erow + 1; i++)
//		{
//			for (size_t j = bcol; j < ecol + 1; j++)
//			{
//				listarea[i][j].isfree = false;
//			}
//		}
//	};
//
//	std::vector<myrectangle> pack(std::vector<myrectangle>& listrect, const myrectangle& enclosingrect)
//	{
//		std::sort(listrect.begin(), listrect.end(), [](myrectangle& a, myrectangle& b) {
//			return b.h < a.h;
//		});
//
//		vector<vector<arearect>> listarea;
//		listarea.reserve(100000);
//		listarea.emplace_back(1, arearect(enclosingrect.w, enclosingrect.h, 0, 0, true));
//		//(1, std::vector<arearect>(1, arearect(enclosingrect.w, enclosingrect.h, 0, 0, true)));
//
//		float places[6];
//		for (size_t i = 0; i < listrect.size(); i++)
//		{
//			auto& rect = listrect[i];
//			if (findplace(rect, enclosingrect, listarea, places))
//			{
//				rect.x = places[0];
//				rect.y = places[1];
//				updatelistarea(listarea, rect, places[2], places[3], places[4], places[5]);
//				//printlistarea(listarea);
//			}
//		}
//
//		return listrect;
//	}
//};