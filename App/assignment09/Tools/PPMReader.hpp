/*
 * PPMReader.hpp
 *
 *  Created on: Nov 29, 2012
 *      Author: kremer
 */

#ifndef PPMREADER_HPP_
#define PPMREADER_HPP_

#include <fstream>
#include <iostream>

#include "TextureData.hpp"

using namespace std;

class PPMReader {
public:
    PPMReader() {}
    virtual ~PPMReader() {}

    TextureData* readPPMFile(const std::string &fileName) {

        std::ifstream fileStream(fileName.c_str(), std::ifstream::in);

        if(!fileStream.good()) {
            cerr << "could not open file " << fileName << endl;
            return NULL;
        }

        TextureData *texData = new TextureData;

        int width, height, maxValue;

        std::string line;
        fileStream >> line; // ignore the header

        // note: we assume no comments in this file!
        // a general ppm loader has to be more clever!

        fileStream >> width;
        fileStream >> height;
        fileStream >> maxValue; // we will ignore this and assume the value range is 0..255

        unsigned char *data = new unsigned char[width * height * 3]; // we assume RGB

        int pos = 0;
        while(fileStream.good() && pos < (width * height * 3)) {
            int i;
            fileStream >> i;
            data[pos] = (unsigned char) i;
            pos++;
        }
        fileStream.close();

        texData->setData(data); // data will get deleted by the TextureData destructor!
        texData->setDepth(1);   // 2D so, depth is 1
        texData->setHeight(height);
        texData->setWidth(width);
        texData->setFormat(GL_RGB);
        texData->setType(GL_UNSIGNED_BYTE); // == unsigned char

        return texData;
    }
};

#endif /* PPMREADER_HPP_ */
