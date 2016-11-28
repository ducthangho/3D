// SimplePackingAlogorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PackingAlogorithm.h"
#include "Pack.h"

int test1()
{	
	//srand(1989);
	MyRectangle enclosingRect(40, 40);
	MyRectangle r1(15, 30);
	MyRectangle r2(20, 25);
	MyRectangle r3(9, 7);
	MyRectangle r4(20, 8);
	MyRectangle r5(10, 10);

	std::vector<MyRectangle> listRect;
	listRect.push_back(r1);
	listRect.push_back(r2);
	listRect.push_back(r3);
	listRect.push_back(r4);
	listRect.push_back(r5);

	std::vector<std::vector<AreaRect>> listArea;
	std::vector<AreaRect> row1;
	row1.emplace_back(15, 25, 0, 0, false);
	row1.emplace_back(20, 25, 15, 0, false);
	row1.emplace_back(5, 25, 35, 0, true);

	std::vector<AreaRect> row2;
	row2.emplace_back(15, 5, 0, 25, false);
	row2.emplace_back(20, 5, 15, 25, true);
	row2.emplace_back(5, 5, 35, 25, true);

	std::vector<AreaRect> row3;
	row3.emplace_back(15, 10, 0, 30, true);
	row3.emplace_back(20, 10, 15, 30, true);
	row3.emplace_back(5, 10, 35, 30, true);

	std::vector<AreaRect> row4;
	row4.emplace_back(15, 9, 0, 40, true);
	row4.emplace_back(20, 9, 15, 40, true);
	row4.emplace_back(5, 9, 35, 40, true);

	listArea.push_back(row1);
	listArea.push_back(row2);
	listArea.push_back(row3);
	listArea.push_back(row4);

	vector<vector<AreaRect>> listArea2;
	listArea2.reserve(100000);
	listArea2.emplace_back(1, AreaRect(enclosingRect.w, enclosingRect.h, 0, 0, true));
	
	PackingAlogorithm pa;
	float places[6];

	/*if (pa.findPlace(r1, enclosingRect, listArea2, places))
	{
		r1.x = places[0];
		r1.y = places[1];
		r1.print();
		pa.updateListArea(listArea2, r1, places[2], places[3], places[4], places[5]);
		pa.printListArea(listArea2);
	}
	cout << "--------------------------------" << endl;
	if (pa.findPlace(r2, enclosingRect, listArea2, places))
	{
		r2.x = places[0];
		r2.y = places[1];
		r2.print();
		pa.updateListArea(listArea2, r2, places[2], places[3], places[4], places[5]);
		pa.printListArea(listArea2);
	}
	cout << "--------------------------------" << endl;
	if (pa.findPlace(r5, enclosingRect, listArea2, places))
	{
		r5.x = places[0];
		r5.y = places[1];
		r5.print();
		pa.updateListArea(listArea2, r5, places[2], places[3], places[4], places[5]);
		pa.printListArea(listArea2);
	}
	cout << "--------------------------------" << endl;
	if (pa.findPlace(r4, enclosingRect, listArea2, places))
	{
		r4.x = places[0];
		r4.y = places[1];
		r4.print();
		pa.updateListArea(listArea2, r4, places[2], places[3], places[4], places[5],true);
		pa.printListArea(listArea2);
	}
	cout << "--------------------------------" << endl;
	if (pa.findPlace(r3, enclosingRect, listArea2, places))
	{
		r3.x = places[0];
		r3.y = places[1];
		r3.print();
		cout << "i: " << places[2] << "j: " << places[3] << "er: " << places[4] << "ec: " << places[5];
		pa.updateListArea(listArea2, r3, places[2], places[3], places[4], places[5],true);
		pa.printListArea(listArea2);
	}*/
	/*std::vector<MyRectangle> result = pa.pack(listRect, enclosingRect);
	for (auto& a : result)
	{
		a.print();
	}*/
	

	//std::vector<MyRectangle> listRect;
	Time tt;
	for (int i = 0; i < 20000; i++)
	{
		auto w = rand() % 30 + 1;
		auto h = rand() % 40 + 1;
		MyRectangle a(w,h);
		listRect.push_back(a);
	}
	std::cout << "Data creation time = " << tt.duration() << " ms" << std::endl;
	Time t;
	MyRectangle enclosingRect2(500,500);
	std::vector<MyRectangle> r = pa.pack(listRect, enclosingRect2);
	cout << r.size() << endl;
	std::cout << "Execution time = " << t.duration() << " ms" << std::endl;
	for (auto& a : r)
	{
		a.print();
	}
	return 0;
}
int test2()
{
	Node enclosingRect(0, 0, 40, 40);
	MyRectangle r1(15, 30);
	MyRectangle r2(20, 25);
	MyRectangle r3(9, 7);
	MyRectangle r4(20, 8);
	MyRectangle r5(10, 10);

	std::vector<MyRectangle> listRect;
	listRect.push_back(r1);
	listRect.push_back(r2);
	listRect.push_back(r3);
	listRect.push_back(r4);
	listRect.push_back(r5);

	Time tt;
	for (int i = 0; i < 20000; i++)
	{
		auto w = rand() % 30 + 1;
		auto h = rand() % 40 + 1;
		MyRectangle a(w, h);
		listRect.push_back(a);
	}
	std::cout << "Data creation time = " << tt.duration() << " ms" << std::endl;
	Time t;
	Node enclosingRect2(0,0,5000, 5000);
	enclosingRect2.pack(listRect, enclosingRect2);
	std::cout << "Execution time = " << t.duration() << " ms" << std::endl;

	for (auto& a : listRect)
	{
		a.print();
	}
	return 0;
}
int test()
{
	//Test it correctly or not
	MyRectangle enclosingRect(40, 40);
	MyRectangle r1(15, 30);
	MyRectangle r2(20, 25);
	MyRectangle r5(10, 10);
	MyRectangle r4(20, 8);
	MyRectangle r3(9, 7);

	std::vector<MyRectangle> listRect;
	listRect.push_back(r1);
	listRect.push_back(r2);
	listRect.push_back(r3);
	listRect.push_back(r4);
	listRect.push_back(r5);

	Node node(0, 0, 40, 40);

	node.packWithNoPreSort(listRect, node);
	for (auto rect : listRect)
	{
		rect.print();
	}


std:cout << "here is packing alogorthim in codeproject\n";
	PackingAlogorithm pa;
	pa.pack(listRect, enclosingRect);
	for (auto rect : listRect)
	{
		rect.print();
	}
}
int test3() {
	//test performance

	std::vector<MyRectangle> listRect;
	Time tt;
	for (int i = 0; i < 20000; i++)
	{
		auto w = rand() % 30 + 1;
		auto h = rand() % 40 + 1;
		MyRectangle a(w, h);
		listRect.push_back(a);
	}

	std::vector<MyRectangle> listRect2(listRect.begin(),listRect.end());
	std::cout << "Data creation time = " << tt.duration() << " ms" << std::endl;
	
	
	Node enclosingRect(0,0,500, 500);
	MyRectangle enclosingRect2(500,500);
	PackingAlogorithm pa;

	Time t1;
	enclosingRect.pack(listRect, enclosingRect);
	std::cout << "Execution time of alogorthim use binary tree= " << t1.duration() << " ms" << std::endl;
	
	Time t2;
	pa.pack(listRect2, enclosingRect2);
	std::cout << "Execution time of alogorthim use 2 dimension array vector = " << t1.duration() << " ms" << std::endl;

		// for (auto& a : listRect)
	// {
	// 	a.print();
	// }
	return 0;

}
int main()
{
	//test1();
	test();
	return 0;
}
