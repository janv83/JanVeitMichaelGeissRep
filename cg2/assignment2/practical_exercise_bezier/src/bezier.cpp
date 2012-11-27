#include "bezier.h"

#include <random>

using namespace std;

BezierAnimation::BezierAnimation( const unsigned int timePerSet, const unsigned int sets ) :
	mNumSets( sets ), mTimePerSet( timePerSet ), mControlPoints( sets )
{
	for ( vector< ControlPoints >::iterator it = mControlPoints.begin(); it != mControlPoints.end(); ++it )
	{
		*it = ControlPoints( randomPoint(), randomPoint(), randomPoint(), randomPoint() );
	}

	mCurrent = mControlPoints[ 0 ];
}

void BezierAnimation::update( const unsigned int timeInMilliseconds )
{
	unsigned int interval = timeInMilliseconds / mTimePerSet;
	unsigned int currIndex = interval % mNumSets;
	unsigned int goalIndex = ( currIndex + 1 ) % mNumSets;

	unsigned int t = timeInMilliseconds - interval * mTimePerSet;
	float factor = t / static_cast< float >( mTimePerSet );

	ControlPoints curr = mControlPoints[ currIndex ];
	ControlPoints goal = mControlPoints[ goalIndex ];

	mCurrent = curr.lerp( factor, goal );
}

BezierAnimation::Point BezierAnimation::randomPoint()
{
	float x = -1.f + rand()/( RAND_MAX/( 2.f ) );
	float y = -1.f + rand()/( RAND_MAX/( 2.f ) );
	return Point( x, y );
}