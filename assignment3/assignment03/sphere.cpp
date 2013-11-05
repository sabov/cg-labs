#include "sphere.h"

#include <iostream>
using namespace std;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

Sphere::Sphere( unsigned int detail ) {
    if (detail < 4) detail = 4;
    stacks = detail;
    slices = detail;
    radius = 1.0;
    uploadedDataToGPU = false;
    arrayBuffer = 0;

    int triangleCount = 2*slices + (stacks - 2)*slices*2;
    vertexCount   = 3*triangleCount;
    elementCount = 4;

    vertexData = new float[ elementCount * vertexCount ];

    unsigned int vertexInArrayPosition = 0;
    for (unsigned int sl = 0; sl < slices; ++sl) {
        // top triangle:
        {
            int st = 0;
            addTriangle( 0,st,  sl+1,st+1,  sl,st+1 , vertexInArrayPosition );
        }
        // middle quads:
        for (unsigned int st = 1; st < stacks-1; ++st) {
            addTriangle( sl,st,   sl+1,st,   sl,st+1, vertexInArrayPosition );
            addTriangle( sl+1,st, sl+1,st+1, sl,st+1, vertexInArrayPosition );
        }
        // bottom triangle:
        {
            addTriangle( 0, stacks,  sl, stacks-1,  sl+1, stacks-1, vertexInArrayPosition );
        }
    }
}

Sphere::~Sphere() {
    delete[] vertexData;
    glDeleteBuffers( 1, &arrayBuffer );
}

void Sphere::draw() {
    if (uploadedDataToGPU == false) {
        uploadedDataToGPU = true;
        glGenBuffers( 1, &arrayBuffer );
        glBindBuffer( GL_ARRAY_BUFFER, arrayBuffer );
        glBufferData( GL_ARRAY_BUFFER, sizeof(float)*elementCount*vertexCount, vertexData, GL_STATIC_DRAW );
    }

    glBindBuffer( GL_ARRAY_BUFFER, arrayBuffer );
    glVertexAttribPointer( 0, elementCount, GL_FLOAT, GL_FALSE, elementCount*sizeof(float), 0 );
    glEnableVertexAttribArray( 0 );
    glDrawArrays( GL_TRIANGLES, 0, vertexCount );
}


glm::vec3 Sphere::positionOnSphere( int sliceNumber, int stackNumber ) {
    glm::vec3 position;

    double alpha = (M_PI/stacks)*stackNumber;
    double beta  = ((2.0*M_PI)/slices)*sliceNumber;

    double ringRadius = radius * sin(alpha);
    position.x = sin( beta ) * ringRadius;
    position.y = cos( beta ) * ringRadius;
    position.z = radius * cos( alpha );

    return position;
}

void Sphere::addTriangle( unsigned int sl0, unsigned int st0,
                  unsigned int sl1, unsigned int st1,
                  unsigned int sl2, unsigned int st2,
                  unsigned int &posOffset ) {
    glm::vec3 p[3];

    p[0] = positionOnSphere( sl0, st0 );
    p[1] = positionOnSphere( sl1, st1 );
    p[2] = positionOnSphere( sl2, st2 );

    for (int j = 0; j < 3; ++j) {
        vertexData[ posOffset + 0 ] = p[j].x;
        vertexData[ posOffset + 1 ] = p[j].y;
        vertexData[ posOffset + 2 ] = p[j].z;
        vertexData[ posOffset + 3 ] = 1.0f;
        posOffset += elementCount;
    }
}





