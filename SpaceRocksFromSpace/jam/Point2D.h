#pragma once
#ifndef __POINT_2D_H__
#define __POINT_2D_H__
namespace jam
{
	template <class T>
	struct Point2D
	{
	public:
		/*
		Point2D<T>();
		Point2D<T>(const Point2D<T>& source);
		Point2D<T>(T x, T y); 
		*/
        // T& operator=(const T& other);
		T x;
		T y;
	};
}
#define Point2Df Point2D<float>
#define Point2Di Point2D<int>
#endif

