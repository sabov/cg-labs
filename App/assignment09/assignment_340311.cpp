/*
 * Basics of Computer Graphics Exercise
 */

#include "Tools/ABReader.hpp"
#include "Tools/PNGReader.hpp"
#include "Tools/TextureData.hpp"
#include "Tools/HelperFunctions.hpp"

#include "assignment.h"
using namespace std;

extern glm::mat4 g_ProjectionMatrix;

GLuint bunnyTexture;
GLuint earthTexture;

GLuint cubeMapTexture;
GLuint cubeMapTextureDebug;

GLuint sphereMapTexture;
GLuint sphereMapTextureDebug;

// add your team members names and matrikel numbers here:
void printStudents() {
    cout << "Student Tarasenko Petro, matrikel number 340826" << endl;
    cout << "Student Oleksandr Sabov, matrikel number 340311" << endl;
    cout << "Student Chih-Yun Tsai, matrikel number 328782" << endl;
}

//
// Control the app by pressing:
// R: model rotation on/off
// X: switch between debug and normal environment map / skytexture                  (debugTexture)
// E: only show the environment map or add the environment * 0.1 to the phong model (environmentOnly)
// C: switch between cubemapping and sphere mapping                                 (cubeMapping)
// M: switch between the meshes                                                     (meshNumber)
//
void drawScene(bool environmentOnly, int meshNumber, bool cubeMapping, bool debugTexture ) {
   //
   // When entering here, the correct shader is already active and the skybox is already rendered.
   // You need to bind the cubemap or spheremap texture based on 'cubeMapping' and use the
   // debug or non-debug texture based on 'debugTexture'.
   //
   // Set additional uniforms with
   //  g_shader->setUniform("name", <value> );
   //

   // =======================================================================
   // =======================================================================
   // Assignment code:
   // Setup environment textures:
   // =======================================================================
   // =======================================================================

    g_shader->setUniform("uUseCubeMapping", cubeMapping);
    g_shader->setUniform("uEnvironmentOnly", environmentOnly);

    if (debugTexture) {
        // bind debug spheremap and cubemap here

        glActiveTexture( GL_TEXTURE0 );
        if(cubeMapping) {
            glActiveTexture( GL_TEXTURE1 );
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        } else {
            glActiveTexture( GL_TEXTURE2 );
            glBindTexture(GL_TEXTURE_2D, sphereMapTextureDebug);
        }

    } else {
        // bind spheremap and cubemap here

        if(cubeMapping) {
            glActiveTexture( GL_TEXTURE1 );
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        } else {
            glActiveTexture( GL_TEXTURE2 );
            glBindTexture(GL_TEXTURE_2D, sphereMapTexture);
        }
    }

    // =======================================================================
    // End assignment code
    // =======================================================================

    
    // render the correct mesh:
    if (meshNumber == 0) {
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        g_vaoEarth->render();
    } else {
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D, bunnyTexture);
        g_vaoBunny->render();
    }
}

//
// gets a filename and creates an OpenGL texture with MipMaps.
//
GLuint readTexture2D( const string &fileName ) {
    PNGReader pngreader;
    TextureData* texture;
    GLuint gltexture;
    //
    // you will need:
    //
    // texture->getWidth()   <- image width
    // texture->getHeight()  <- image height
    // texture->getFormat()  <- format, e.g. GL_RGB
    // texture->getType()    <- data type, e.g. GL_UNSIGNED_BYTE
    // texture->getData()    <- a pointer to the data
    //

    texture = pngreader.readFile( fileName );
    glGenTextures(1, &gltexture);
    glBindTexture(GL_TEXTURE_2D, gltexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0, // MipMap level
        GL_SRGB,
        texture->getWidth(),
        texture->getHeight(),
        0,//no border !
        texture->getFormat(),
        texture->getType(),
        texture->getData() );

    glGenerateMipmap(GL_TEXTURE_2D);
    delete texture; // Not needed anymore!

    return gltexture;
}

GLuint readCubeTexture() {
    PNGReader pngreader;
    TextureData* texture;
    GLuint gltexture;

    glGenTextures(1, &gltexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, gltexture);


    TextureData* texture1 = pngreader.readFile("posx.png");
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        0, // MipMap level
        GL_RGB,
        texture1->getWidth(),
        texture1->getHeight(),
        0,//no border !
        texture1->getFormat(),
        texture1->getType(),
        texture1->getData() );

    TextureData* texture2 = pngreader.readFile("posy.png");
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        0, // MipMap level
        GL_RGB,
        texture2->getWidth(),
        texture2->getHeight(),
        0,//no border !
        texture2->getFormat(),
        texture2->getType(),
        texture2->getData() );

    TextureData* texture3 = pngreader.readFile("posz.png");
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        0, // MipMap level
        GL_RGB,
        texture3->getWidth(),
        texture3->getHeight(),
        0,//no border !
        texture3->getFormat(),
        texture3->getType(),
        texture3->getData() );

    TextureData* texture4 = pngreader.readFile("negx.png");
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        0, // MipMap level
        GL_RGB,
        texture4->getWidth(),
        texture4->getHeight(),
        0,//no border !
        texture4->getFormat(),
        texture4->getType(),
        texture4->getData() );

    TextureData* texture5 = pngreader.readFile("negy.png");
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        0, // MipMap level
        GL_RGB,
        texture5->getWidth(),
        texture5->getHeight(),
        0,//no border !
        texture5->getFormat(),
        texture5->getType(),
        texture5->getData() );

    TextureData* texture6 = pngreader.readFile("negz.png");
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        0, // MipMap level
        GL_RGB,
        texture6->getWidth(),
        texture6->getHeight(),
        0,//no border !
        texture6->getFormat(),
        texture6->getType(),
        texture6->getData() );

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    //delete texture; // Not needed anymore!

    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, gltexture, 0);

    return gltexture;
}

GLuint CreateCubemap()
{
    GLuint textureObject;
    PNGReader pngreader;
    TextureData* texture6 = pngreader.readFile("negz.png");
    glGenTextures(1, &textureObject);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureObject);
    for (int f = 0; f < 6; ++f) {
        GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + f;
        glTexImage2D(
            face,
            0, // MipMap level
            GL_SRGB,
            texture6->getWidth(),
            texture6->getHeight(),
            0,//no border !
            texture6->getFormat(),
            texture6->getType(),
            texture6->getData() );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, 
                    GL_TEXTURE_MIN_FILTER, 
                    GL_LINEAR_MIPMAP_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    return textureObject;
}

void initCustomResources() {
	
	// activate nicer sampling at seams of the cube map.
	// try to uncomment this and you will notice aliasing at the color edges
	// of the debug cubemap on the sphere mesh (the aliasing is much more subtle 
	// on the bunny):
	glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
	
    // Create your resources here, e.g. shaders, buffers,...

    ////////////////////////////////////////////////////////////////////////////
    // Shader:
    
    // you have to edit the shader for texture access, so rename them and read your shader
    // files here:
    g_shader = new ShaderProgram("envmap_340311.vsh", "envmap_340311.fsh");
    if (!g_shader->link()) exit(0);
    
    g_shader->use();
    g_shader->setUniform( "uMaterialDiffuse",  (int)0 );
    g_shader->setUniform( "uTextureCubeMap",   (int)1 );
    g_shader->setUniform( "uTextureSphereMap", (int)2 );
	g_shader->setUniform( "uMaterialSpecular", (int)3 );

    ////////////////////////////////////////////////////////////////////////////
    // Read textures:

    //
    // Simple 2D textures can be loaded with this little helper function.
    // You need to write your own code for the cubemaps. 
    //
    bunnyTexture = readTexture2D( "clownfishBunny.png" );
    earthTexture = readTexture2D( "blueMarble.png" );
    sphereMapTexture = readTexture2D( "sphereMap.png" );
    sphereMapTextureDebug = readTexture2D( "sphereMapDebug.png" );
    cubeMapTexture = CreateCubemap();

    PNGReader pngreader;
    TextureData* texture;


    // =======================================================================
    // =======================================================================
    // Assignment code:
    // Load sphere maps and cube maps here
    // =======================================================================
    // =======================================================================


    // =======================================================================
    // End assignment code
    // =======================================================================

}

void deleteCustomResources() {

    // Don't forget to delete your OpenGL resources (shaders, buffers, etc.)!

    glDeleteTextures(1, &earthTexture);
    glDeleteTextures(1, &bunnyTexture);
    glDeleteTextures(1, &cubeMapTexture);
    glDeleteTextures(1, &cubeMapTextureDebug);
}

