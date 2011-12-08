#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <conio.h>

#pragma warning(disable:4996 4100)

#include "PlatformConfigHACD.h"
#include "wavefront.h"
#include "HACD.h"

namespace HACD
{
	HACD_API *gHACD = NULL;
};


float getFloatArg(int arg,int argc,const char **argv)
{
	float ret = 0;
	if ( arg < argc )
	{
		ret = (float)atof(argv[arg]);
	}
	else
	{
		printf("Error: Missing input argument value at argument location %d.\r\n",arg+1);
	}
	return ret;
}

int getIntArg(int arg,int argc,const char **argv)
{
	int ret = 0;
	if ( arg < argc )
	{
		ret = atoi(argv[arg]);
	}
	else
	{
		printf("Error: Missing input argument value at argument location %d.\r\n",arg+1);
	}
	return ret;
}
void main(int argc,const char ** argv)
{
	if ( argc == 1 )
	{
		printf("Usage: TestHACD <wavefront.obj> (options)\r\n");
		printf("\r\n");
		printf("Options:\r\n");
		printf("-v	<count>	: Max Hull Vertices (default 64)\r\n");
		printf("-m	<count>	: Maximum number of hulls (default 256)\r\n");
		printf("\r\n");
		printf("Example: TestHACD hornbug.obj -m 40 -v 64\r\n");
		printf("\r\n");
	}
	else
	{
		HACD::HACD_API::Desc desc;
		const char *wavefront = argv[1];
		int scan = 2;
		while ( scan < argc )
		{
			const char *option = argv[scan];
			if ( strcmp(option,"-v") == 0 )
			{
				desc.mMaxHullVertices = getIntArg(scan+1,argc,argv);
				scan+=2;
			}
			else if ( strcmp(option,"-m") == 0 )
			{
				desc.mMaxHullCount = getIntArg(scan+1,argc,argv);
				scan+=2;
			}
			else
			{
				scan++;
			}
		}

		HACD::gHACD = HACD::createHACD_API();
		if  ( HACD::gHACD )
		{
			WavefrontObj obj;
			unsigned int tcount = obj.loadObj(wavefront,false);
			if ( tcount )
			{
				desc.mTriangleCount = obj.mTriCount;
				desc.mVertexCount = obj.mVertexCount;
				desc.mIndices = (hacd::HaU32 *)obj.mIndices;
				desc.mVertices = obj.mVertices;
			}
			if ( desc.mTriangleCount )
			{
				hacd::HaU32 hullCount = HACD::gHACD->performHACD(desc);
				if ( hullCount != 0 )
				{
					printf("Produced %d output convex hulls.\r\n", hullCount );
					FILE *fph = fopen("ConvexDecomposition.obj", "wb");
					if ( fph )
					{
						fprintf(fph,"# Input mesh '%s' produced %d convex hulls.\r\n", wavefront, hullCount );
						hacd::HaU32 *baseVertex = new hacd::HaU32[hullCount];
						hacd::HaU32 vertexCount = 0;
						for (hacd::HaU32 i=0; i<hullCount; i++)
						{
							const HACD::HACD_API::Hull *hull = HACD::gHACD->getHull(i);
							if ( hull )
							{
								baseVertex[i] = vertexCount;
								fprintf(fph,"## Hull %d has %d vertices.\r\n", i+1, hull->mVertexCount );
								for (hacd::HaU32 i=0; i<hull->mVertexCount; i++)
								{
									const hacd::HaF32 *p = &hull->mVertices[i*3];
									fprintf(fph,"v %0.9f %0.9f %0.9f\r\n", p[0], p[1], p[2] );
								}
								vertexCount+=hull->mVertexCount;
							}
						}
						for (hacd::HaU32 i=0; i<hullCount; i++)
						{
							const HACD::HACD_API::Hull *hull = HACD::gHACD->getHull(i);
							if ( hull )
							{
								hacd::HaU32 startVertex = baseVertex[i];
								fprintf(fph,"# Convex Hull %d contains %d triangles and %d vertices.  Starting vertex index is: %d It has a volume of: %0.9f\r\n", i+1, hull->mTriangleCount, hull->mVertexCount, startVertex);
								for (hacd::HaU32 j=0; j<hull->mTriangleCount; j++)
								{
									hacd::HaU32 i1 = hull->mIndices[j*3+0]+startVertex+1;
									hacd::HaU32 i2 = hull->mIndices[j*3+1]+startVertex+1;
									hacd::HaU32 i3 = hull->mIndices[j*3+2]+startVertex+1;
									fprintf(fph,"f %d %d %d\r\n", i1, i2, i3 );
								}
							}
						}
					}
					else
					{
						printf("Failed to open output file.\r\n");
					}
				}
			}
			else
			{
				printf("Failed to load Wavefront OBJ file '%s'\r\n",wavefront);
			}

		}
		else
		{
			printf("Failed to load the HACD DLL\r\n");
		}
	}
}
