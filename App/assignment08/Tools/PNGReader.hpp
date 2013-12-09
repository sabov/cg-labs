/*
 */

#ifndef PNGREADER_HPP_
#define PNGREADER_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

#include "TextureData.hpp"
#include "lodepng.h"

using namespace std;


struct LodepngFile
{
public:
    LodepngFile(const std::string& _filename)
    {
        unsigned int errorCode = lodepng_load_file(&mData, &mSize, _filename.c_str());
        if(errorCode)
        {
            std::stringstream errorMsg;
            cerr << "LodePNG error while loading file " << _filename << " - " << errorCode << ": " << lodepng_error_text(errorCode);
            mData = NULL;
            throw std::runtime_error(errorMsg.str());
        }
    }

    ~LodepngFile()
    {
        free(mData);
    }

    unsigned char* mData;
    size_t mSize;
};


class PNGReader {
public:
    PNGReader() {}
    virtual ~PNGReader() {}

    TextureData* readPNGFile(const std::string &_filename) {
	  TextureData *data = NULL;

	  unsigned int errorCode;
	  unsigned int width, height;
	  LodePNGState state;

	  // Load the PNG file from disk
	  lodepng_state_init(&state);

	  try
	  {
		  LodepngFile lodepngFile(_filename);

		  // Extract metadata (bit depth, color type)
		  errorCode = lodepng_inspect(&width, &height, &state, lodepngFile.mData, lodepngFile.mSize);
		  if(errorCode)
		  {
			  std::stringstream errorMsg;
			  errorMsg << "LodePNG error " << errorCode << ": " << lodepng_error_text(errorCode);
			  throw std::runtime_error(errorMsg.str());
		  }

		  unsigned int bitdepth = state.info_png.color.bitdepth;
		  LodePNGColorType colorType = state.info_png.color.colortype;

		  unsigned int channels = 0;
		  GLenum glFormat = 0;

		  if(colorType == LCT_GREY)       { channels = 1; glFormat = GL_RED;  }
		  if(colorType == LCT_GREY_ALPHA) { channels = 2; glFormat = GL_RG;   }
		  if(colorType == LCT_RGB)        { channels = 3; glFormat = GL_RGB;  }
		  if(colorType == LCT_RGBA)       { channels = 4; glFormat = GL_RGBA; }
		  if(colorType == LCT_PALETTE)    { channels = 4; glFormat = GL_RGBA; colorType = LCT_RGBA; } // force LodePNG to convert paletted data to RGBA

		  GLenum glType = 0;
		  if(bitdepth ==  8) glType = GL_UNSIGNED_BYTE;
		  if(bitdepth == 16) glType = GL_UNSIGNED_SHORT;

		  if(channels == 0 || glFormat == 0 || glType == 0)
		  {
			  std::stringstream errorMsg;
			  errorMsg << "Could not load " << _filename << ": " << "unsupported bit depth or format";
			  throw std::runtime_error(errorMsg.str());
		  }

		  // Decode the image
		  unsigned char* image;
		  errorCode = lodepng_decode_memory(&image, &width, &height, lodepngFile.mData, lodepngFile.mSize, colorType, bitdepth);
		  if(errorCode)
		  {
			  std::stringstream errorMsg;
			  errorMsg << "LodePNG error while decoding file " << _filename << " - " << errorCode << ": " << lodepng_error_text(errorCode);
			  throw std::runtime_error(errorMsg.str());
		  }

		  // LodePNG decodes 16 bit PNGs in big endian format ==> Convert
		  if(bitdepth == 16)
			  for(unsigned int i = 0; i < (width * height * channels * 2); i += 2)
				  std::swap(image[i], image[i+1]);

		  // Wrap the data in a TextureData object
		  data = new TextureData();
		  data->setData((GLubyte*)image);
		  data->setWidth(width);
		  data->setHeight(height);
		  data->setType(glType);
		  data->setFormat(glFormat);
	  }
	  catch(std::runtime_error& error)
	  {
		  cerr << error.what() << std::endl;
	  }

	  return data;
    }
};

#endif /* PNGREADER_HPP_ */
