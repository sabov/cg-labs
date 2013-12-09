/*
 * ABReader.hpp
 *
 *  Created on: Nov 29, 2012
 *      Author: kremer
 */

#ifndef ABREADER_HPP_
#define ABREADER_HPP_

#include <fstream>
#include <iostream>

#include "HelperFunctions.hpp"

#include "ArrayBuffer.hpp"

using namespace std;

class ABReader {
public:
    ABReader() {}
    virtual ~ABReader() {}

    ArrayBuffer* readABFile(const std::string &fileName) {

        std::ifstream fileStream(fileName.c_str(), std::ifstream::in);

        if(!fileStream.good()) {
            cerr << "could not open file " << fileName << endl;
            return NULL;
        }

        int vertexCount;
        fileStream >> vertexCount;

        float *data = new float[vertexCount * 8]; // all files contain 3 pos float, 3 normal floats and 2 texture coordinates!

        int pos = 0;
    	
        while(fileStream.good() && pos < (vertexCount * 8)) {
            fileStream >> data[pos];
            
            pos++;
        }
        fileStream.close();
        
        ArrayBuffer *ab = new ArrayBuffer();
        ab->setData(vertexCount * 8 * sizeof(float), data);
        ab->defineAttribute("aPosition", GL_FLOAT, 3);
        ab->defineAttribute("aNormal", GL_FLOAT, 3);
        ab->defineAttribute("aTexCoord", GL_FLOAT, 2);

        delete[] data; // ab holds a copy of the data on the GPU memory

        return ab;
    }
};

#endif /* ABREADER_HPP_ */
