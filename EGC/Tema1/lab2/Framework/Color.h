/*
 * TEMA 1: GEOMETRY WARS
 *
 * nume:		Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 333 CA
 */
#pragma once

class Color
{
public:
	float r;
	float g;
	float b;
public:
	Color()
	{
		r = g = b = 0;
	}

	Color(float _r,float _g,float _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}

	~Color(){}
};