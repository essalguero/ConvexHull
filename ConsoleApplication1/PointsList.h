#pragma once

#include<list>


#include "Point.h"

using namespace std;

class PointsList
{
public:
	PointsList();
	~PointsList();

	PointsList(const PointsList & pList);

	void addPoint(Point p);

	Point getPoint(int index);

	list<Point> getPointsList();

	int getSize();

private:
	list<Point> pointsList;


};

