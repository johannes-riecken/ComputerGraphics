//
// Class to hold simple meshes.
// For every vertex position, texture coordinate and TBN basis is stored
//

#pragma once
#ifndef	__BASIC_MESH__
#define	__BASIC_MESH__

#include <string>

#include "vec2.h"
#include "vec3.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

class	mat4;

using namespace std;

struct  BasicVertex
{
	vec3	pos;
	vec2	tex;
	vec3	n;
	vec3	t, b;
};

class BasicMesh
{
	int	         numVertices;
	int	         numTriangles;
	VertexArray	 vao;     // array with all bindings
	VertexBuffer vertBuf; // vertex data
	VertexBuffer indBuf;  // index buffer
	string		 name;

public:
	BasicMesh ( BasicVertex * vertices, const int * indices, int nv, int nt );

	void	render ();
	void	renderInstanced ( int primCount );

	const string& getName () const
	{
		return name;
	}

	int getNumVertices () const
	{
		return numVertices;
	}

	int getNumTriangles () const
	{
		return numTriangles;
	}

	void setName ( const string& theName )
	{
		name = theName;
	}
};

void	computeTangents ( BasicVertex& v0, const BasicVertex& v1, const BasicVertex& v2 );
void	computeNormals  ( BasicVertex * vertices, const int * indices, int nv, int nt );

BasicMesh * createSphere( const vec3& org, float radius, int n1, int n2 );
BasicMesh * createQuad  ( const vec3& org, const vec3& dir1, const vec3& dir2 );
BasicMesh * createBox   ( const vec3& pos, const vec3& size, const mat4 * mat = NULL, bool invertNormal = false );
BasicMesh * createTorus ( float r1, float r2, int n1, int n2 );
BasicMesh * createKnot  ( float r1, float r2, int n1, int n2 );

#endif
