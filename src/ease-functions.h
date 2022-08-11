//
// Basic ease functions for smooth animations
//
// Taken from: http://iphonedevelopment.blogspot.com/2010/12/more-animation-curves-than-you-can.html
//

#pragma once
#ifndef	__EASE_FUNCTIONS__
#define	__EASE_FUNCTIONS__

float linearInterpolation  ( float t, float start, float end );
float quadraticEaseOut     ( float t, float start, float end );
float quadraticEaseIn      ( float t, float start, float end );
float quadraticEaseInOut   ( float t, float start, float end );
float cubicEaseOut         ( float t, float start, float end );
float cubicEaseIn          ( float t, float start, float end );
float cubicEaseInOut       ( float t, float start, float end );
float quarticEaseOut       ( float t, float start, float end );
float quarticEaseIn        ( float t, float start, float end );
float quarticEaseInOut     ( float t, float start, float end );
float quinticEaseOut       ( float t, float start, float end );
float quinticEaseIn        ( float t, float start, float end );
float quinticEaseInOut     ( float t, float start, float end );
float sinusoidalEaseOut    ( float t, float start, float end );
float sinusoidalEaseIn     ( float t, float start, float end );
float sinusoidalEaseInOut  ( float t, float start, float end );
float exponentialEaseOut   ( float t, float start, float end );
float exponentialEaseIn    ( float t, float start, float end );
float exponentialEaseInOut ( float t, float start, float end );
float circularEaseOut      ( float t, float start, float end );
float circularEaseIn       ( float t, float start, float end );
float circularEaseInOut    ( float t, float start, float end );

#endif