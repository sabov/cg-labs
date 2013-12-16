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
    cout << "Student Name 0, matrikel number 0" << endl;
    cout << "Student Name 1, matrikel number 1" << endl;
    cout << "Student Name 2, matrikel number 2" << endl;
    cout << "Student Name 3, matrikel number 3" << endl;
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

    if (debugTexture) {
        // bind debug spheremap and cubemap here
        //g_shader->setUniform("uUseCubeMapping", true );
        //g_shader->setUniform("uEnvironmentOnly", true );
    } else {
        // bind spheremap and cubemap here
        //glActiveTexture( GL_TEXTURE0 );
        //glBindTexture(GL_TEXTURE_2D, sphereMapTexture);
    }

    // =======================================================================
    // End assignment code
    // =======================================================================

    
    // render the correct mesh:
    if (meshNumber == 0) {
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture(GL_TEXTURE_2D, sphereMapTexture);
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
    g_shader = new ShaderProgram("envmap_123456.vsh", "envmap_123456.fsh");
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

