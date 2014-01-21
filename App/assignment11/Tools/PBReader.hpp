/*
 * PBReader.hpp
 *
 *  Created on: Dec 11, 2012
 *      Author: kremer
 */

#ifndef PBREADER_HPP_
#define PBREADER_HPP_

#include <iostream>
#include <fstream>

#include "ArrayBuffer.hpp"

class PBReader {
public:
    PBReader() {}

    virtual ~PBReader() {}

    ArrayBuffer* readPBFile(const std::string& _fileName) {

        std::ifstream fileStream(_fileName.c_str(), std::ifstream::in);

        if(!fileStream.good()) {
            std::cerr << "could not open file " << _fileName << std::endl;
            return NULL;
        }

        int vertexCount = 0;
        fileStream >> vertexCount;

        float* data = new float[vertexCount * 10]; // all files contain 3 pos float, 3 normal floats, 3 color floats and 1 splat size!

        int pos = 0;
        while(fileStream.good() && pos < (vertexCount * 10)) {
            fileStream >> data[pos];
            pos++;
        }

        fileStream.close();

        ArrayBuffer* ab = new ArrayBuffer();
        ab->setData(vertexCount * 10 * sizeof(float), data);
        ab->defineAttribute("aPosition", GL_FLOAT, 3);
        ab->defineAttribute("aNormal", GL_FLOAT, 3);
        ab->defineAttribute("aColor", GL_FLOAT, 3);
        ab->defineAttribute("aSplatSize", GL_FLOAT, 1);

        delete[] data;

        // ab holds a copy of the data on the GPU memory
        return ab;
    }
};

#endif /* PBREADER_HPP_ */
