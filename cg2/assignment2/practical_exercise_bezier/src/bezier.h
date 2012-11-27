#pragma once

#include <vector>

class BezierAnimation
{

public:

	struct Point
	{
		Point( const float f1, const float f2 ) : x( f1 ), y( f2 ) {}
		Point operator-( Point const& other ) const { return Point( x-other.x, y-other.y ); }
		Point operator+( Point const& other ) const { return Point( x+other.x, y+other.y ); }
		Point operator*( const float factor ) const { return Point( factor*x, factor*y ); }
		Point lerp( const float factor, Point const& other ) const { return ( *this ) + ( other - ( *this ) ) * factor; }

		float x, y;
	};

	struct ControlPoints
	{
		ControlPoints() : c0( 0.f, 0.f ), c1( 0.f, 0.f ), c2( 0.f, 0.f ), c3( 0.f, 0.f ) {}
		ControlPoints( Point const& p0, Point const& p1, Point const& p2, Point const& p3 ) : c0( p0 ), c1( p1 ), c2( p2 ), c3( p3 ) {}
		ControlPoints lerp( const float f, ControlPoints const& p ) const { return ControlPoints( c0.lerp( f, p.c0 ), c1.lerp( f, p.c1 ), c2.lerp( f, p.c2 ), c3.lerp( f, p.c3 ) ); }
	
		Point c0, c1, c2, c3;
	};

	BezierAnimation( const unsigned int timePerSet, const unsigned int sets );
	void update( const unsigned int timeInMilliseconds );

	Point const& ctrl0() { return mCurrent.c0; }
	Point const& ctrl1() { return mCurrent.c1; }
	Point const& ctrl2() { return mCurrent.c2; }
	Point const& ctrl3() { return mCurrent.c3; }

private:

	static Point randomPoint();

	ControlPoints					mCurrent;
	unsigned int					mTimePerSet;
	unsigned int					mNumSets;
	std::vector< ControlPoints >	mControlPoints;

};