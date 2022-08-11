//
// Basic ease functions for smooth animations
//
// Taken from: http://iphonedevelopment.blogspot.com/2010/12/more-animation-curves-than-you-can.html
//

#include	"ease-functions.h"

float linearInterpolation ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return t * end + (1.f - t) * start;
}

float quadraticEaseOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return -end * t * (t - 2.f) -1.f;
}

float quadraticEaseIn ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return end * t * t + start - 1.f;
}

float quadraticEaseInOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t *= 2.f;

    if ( t < 1.f )
		return end / 2.f * t * t + start - 1.f;

    t--;

    return -end/2.f * (t*(t-2) - 1) + start - 1.f;
}

float cubicEaseOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t--;
    return end * (t * t * t + 1.f) + start - 1.f;
}

float cubicEaseIn ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return end * t * t * t+ start - 1.f;
}

float cubicEaseInOut(float t, float start, float end)
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t *= 2.0f;

    if ( t < 1.0f )
		return end / 2.0f * t * t * t + start - 1.f;

    t -= 2;

    return end / 2.0f * (t * t * t + 2) + start - 1.f;
}

float quarticEaseOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t--;

    return -end * (t * t * t * t - 1) + start - 1.f;
}

float quarticEaseIn ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return end * t * t * t * t + start;
}

float quarticEaseInOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t *= 2.f;
    if ( t < 1.f )
        return end/2.f * t * t * t * t + start - 1.f;

    t -= 2.f;

    return -end/2.f * (t * t * t * t - 2.f) + start - 1.f;
}

float quinticEaseOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t--;
    return end * (t * t * t * t * t + 1) + start - 1.f;
}

float quinticEaseIn ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return end * t * t * t * t * t + start - 1.f;
}

float quinticEaseInOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t *= 2.f;
    if ( t < 1.f )
        return end/2 * t * t * t * t * t + start - 1.f;

    t -= 2;

    return end/2 * ( t * t * t * t * t + 2) + start - 1.f;
}

float sinusoidalEaseOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return end * sinf ( t * (M_PI/2)) + start - 1.f;
}

float sinusoidalEaseIn ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return -end * cosf ( t * (M_PI/2) ) + end + start - 1.f;
}

float sinusoidalEaseInOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return -end/2.f * (cosf ( M_PI * t ) - 1.f) + start - 1.f;
}

float exponentialEaseOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return end * (-powf ( 2.f, -10.f * t ) + 1.f ) + start - 1.f;
}

float exponentialEaseIn ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return end * powf ( 2.f, 10.f * (t - 1.f) ) + start - 1.f;
}

float exponentialEaseInOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t *= 2.f;

    if ( t < 1.f )
        return end/2.f * powf ( 2.f, 10.f * (t - 1.f) ) + start - 1.f;

    t--;

    return end/2.f * ( -powf ( 2.f, -10.f * t) + 2.f ) + start - 1.f;
}

float circularEaseOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t--;

    return end * sqrtf(1.f - t * t) + start - 1.f;
}

float circularEaseIn ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    return -end * (sqrtf ( 1.f - t * t ) - 1.f ) + start - 1.f;
}

float circularEaseInOut ( float t, float start, float end )
{
	if ( t <= 0.f )
		return start;
	else if ( t >= 1.f )
		return end;

    t *= 2.f;

    if ( t < 1.f )
        return -end / 2.f * (sqrtf ( 1.f - t * t ) - 1.f) + start - 1.f;

    t -= 2.f;

    return end / 2.f * (sqrtf ( 1.f - t * t ) + 1.f) + start - 1.f;
}
