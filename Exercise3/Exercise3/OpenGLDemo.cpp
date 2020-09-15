#include "OpenGLDemo.h"

// GLUT header
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

// system header
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// The GLM code for loading and displying OBJ mesh file
#include "glm.h"

// The trackball code for rotating the model
#include "trackball.h"

// The size of the GLUT window
int window_width  = 800;
int window_height = 600;

// The OBJ model
GLMmodel* pModel = NULL;

// The current modelview matrix
double pModelViewMatrix[16];

GLubyte *image;
int n;
int m;

// Texture objects and storage for texture image
GLuint textures[1024];

// If mouse left button is pressed
bool bLeftBntDown = false;

// Old position of the mouse
int OldX = 0;
int OldY = 0;

int main(int argc, char* argv[])
{
	// Initialize the GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Exercise3");
	init();

	// Set the callback function
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

/// Initialize the OpenGL
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)window_width/(float)window_height, 0.01f, 200.0f);

	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
  
	glEnable(GL_CULL_FACE);
    
    // Setup other misc features.
    glEnable( GL_LIGHTING );
    glEnable( GL_NORMALIZE );
    glShadeModel( GL_SMOOTH );
    
    // Setup lighting model.
	GLfloat light_model_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_direction[] = {0.0f, 0.0f, 10.0f, 0.0f};
	GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable( GL_LIGHT0 );

    // The obj file will be loaded
    char FileName[] = "./puget_tex.obj";
    
    // Center of the model
    float modelCenter[] = {0.0f, 0.0f, 0.0f};
    
    // If there is a obj model has been loaded, destroy it
    if ( pModel )
    {
        glmDelete( pModel );
        pModel = NULL;
    }
    
    // Load the new obj model
    pModel = glmReadOBJ( FileName );
    
    // Generate normal for the model
    glmFacetNormals( pModel );
    
    // Scale the model to fit the screen
    glmUnitize( pModel, modelCenter );
    
    // read a PPM texture image file
    glEnable(GL_TEXTURE_2D);
    texture();
    glTexImage2D(GL_TEXTURE_2D,0,3,n,m,0,GL_RGB,GL_UNSIGNED_BYTE, image);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0,1.0,1.0);
    
//    char PPMFileNmae[] = "./puget_tex.ppm";
//    glmReadPPM(textures, PPMFileNmae, 0);
    
    // Init the modelview matrix as an identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
}

/// Display the Object
void display()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslated( 0.0, 0.0, -5.0 );
    glMultMatrixd( pModelViewMatrix );

    if ( pModel )
    {
        glmDraw( pModel, GLM_FLAT );
    }

    glutSwapBuffers();
}


/// Reshape the Window
void reshape(int w, int h)
{
	// Update the window's width and height
	window_width  = w;
	window_height = h;

	// Reset the viewport
	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)window_width/(float)window_height, 0.01f, 200.0f);
	
	glutPostRedisplay();
}

/// Keyboard Messenge
void keyboard(unsigned char key, int x, int y)
{
	// The obj file will be loaded
//    char FileName[128] = "./puget_tex.obj";
//    char TitleName[128]= "";
//
//    // Center of the model
//    float modelCenter[] = {0.0f, 0.0f, 0.0f};

	switch ( key )
	{
//    case 'o':
//    case 'O':
////        PopFileOpenDlg( NULL, FileName, TitleName );
////            std::string filename = "gc_tex.obj";
////            filename.c_str();
////            pModel = glmReadOBJ(filename.c_str());
//
////            FileName[128] = "gc_tex.obj";
//
//        // If there is a obj model has been loaded, destroy it
//        if ( pModel )
//        {
//            glmDelete( pModel );
//            pModel = NULL;
//        }
//
//        // Load the new obj model
//        pModel = glmReadOBJ( FileName );
//
//        // Generate normal for the model
//        glmFacetNormals( pModel );
//
//        // Scale the model to fit the screen
//        glmUnitize( pModel, modelCenter );
//
//        // Init the modelview matrix as an identity matrix
//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
//        glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
//
//        break;

	case '+':
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pModelViewMatrix );
		glScaled( 1.05, 1.05, 1.05 );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
		break;

	case '-':
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pModelViewMatrix );
		glScaled( 0.95, 0.95, 0.95 );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

/// Mouse Messenge
void mouse(int button, int state, int x, int y)
{
	if ( pModel )
	{
		if ( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON )
		{
			OldX = x;
			OldY = y;
			bLeftBntDown = true;
		}
		else if ( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON )
		{
			bLeftBntDown = false;
		}
	}
}

/// Motion Function
void motion(int x, int y)
{
	if ( bLeftBntDown && pModel )
	{
		float fOldX =  2.0f*OldX/(float)window_width  - 1.0f;
		float fOldY = -2.0f*OldY/(float)window_height + 1.0f;
		float fNewX =  2.0f*x/(float)window_width  - 1.0f;
		float fNewY = -2.0f*y/(float)window_height + 1.0f;

		double pMatrix[16];
		trackball_opengl_matrix( pMatrix, fOldX, fOldY, fNewX, fNewY);

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pMatrix );
		glMultMatrixd( pModelViewMatrix );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );

		OldX = x;
		OldY = y;
		glutPostRedisplay();
	}
}

/// Idle function
void idle(void)
{
	glutPostRedisplay();
}

void texture(void)
{
    FILE *fd;
    int  k, nm;
    int i;
    char filename[] = "./puget_tex.ppm";
    char cline[1024];
    char p6[10];
    char width[10];
    char height[10];
    char depth[10];
    float s;
    int red, green, blue;
    
    fd = fopen(filename, "rb");
    if (!fd)
    {
        printf("Fail Open File :%s\n",filename);
    }
    
    fgets(cline,sizeof(cline),fd);
    // load file header for image info
    sscanf(cline,"%s %s %s %s",p6,width,height,depth);
    printf("\n%s  %s  %s  %s\n", p6, width,height,depth);
    if(p6[0]!='P'|| p6[1] != '6')
    {
        printf("%s is not a P6 PPM file!\n", filename);
        exit(0);
    }
    printf("%s is a PPM file\n",filename);
    
    n=atoi(width);
    m=atoi(height);
    k=atoi(depth);
    printf("width = %d height = %d max value= %d\n",n,m,k);
    
    nm = n*m;
    
    image=(GLubyte*)malloc(3*sizeof(GLubyte)*nm);
    fread(image,sizeof(unsigned char),n*m*3,fd);
    printf("read image\n");
}
