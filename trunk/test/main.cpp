/* Rotating cube with color interpolation */

/* Demonstration of use of homogeneous coordinate
transformations and simple data structure for representing
cube from Chapter 4 */

/*Both normals and colors are assigned to the vertices */
/*Cube is centered at origin so (unnormalized) normals
are the same as the vertex values */

#define GLEW_STATIC

#include <Windows.h>    // for solving the Code::Blocks errors
#include <wingdi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "glm.h"
#include "textfile.h"
#include "imageIO.cpp"

#define PI 3.1415926535897

GLMmodel *myObj = NULL;
GLMmodel *myObj2 = NULL;
GLMmodel *myObj3 = NULL;
GLMmodel *myObj4 = NULL;

//int wave_mode = 0;
//GLint loc;
//GLhandleARB v,f,f2,p;

//GLfloat light_theta=0.0;
//GLfloat light0_pos[]={0.0,0.0,1.0, 1.0};

int width, height;
int start_x, start_y;
GLdouble theta = -PI/2, phi = PI / 2;
GLdouble eye_x = 0.0, eye_y = 3, eye_z = -3.5,
         center_x = eye_x + sin(phi) * cos(theta), center_y = eye_y + cos(phi), center_z = 4*sin(phi) * sin(theta),
         up_x = 0.0, up_y = 1.0, up_z = 0.0;
GLuint *textureid;
int number=23;
//ColorImage texture[2]; //Alduin
ColorImage texture[23]; //Alduin

int t=0;

bool inited = false;
bool shader_mode=true;

bool ridingMode = false;
float delta_x = 0.0, delta_y = 0.0, delta_z = 0.0;
float angle=0;

bool shader = false;
GLdouble lightTheta = 10.0;
GLfloat light0_pos[]={1.0, 0.0, 1.0, 1.0};
GLfloat light0_ambient[] = {0.4, 0.4, 0.4, 1.0};
GLfloat light0_diffuse[] = {0.7, 0.7, 0.7, 1.0};
GLfloat light0_specular[] = {1.0, 1.0, 1.0, 1.0};

void drawObj(GLMmodel *myObj,int j)
{
    if (! myObj) return;

    GLMgroup *groups = myObj->groups;
    //int j=0;
    while(groups)
    {
        glBindTexture(GL_TEXTURE_2D, textureid[j]);
        glBegin(GL_TRIANGLES);
        for(int i=0;i<groups->numtriangles;i+=1)
        {
            for (int v=0; v<3; v+=1)
            {
//                glNormal3fv(& myObj->vertices[myObj->triangles[groups->triangles[i]].nindices[v]*3 ]);
                glTexCoord2fv(& myObj->texcoords[myObj->triangles[groups->triangles[i]].tindices[v]*2 ]);
                glVertex3fv(& myObj->vertices[myObj->triangles[groups->triangles[i]].vindices[v]*3 ]);
            }
        }
        groups=groups->next;
        j+=1;
        glEnd();
    }
}

void setShaders()
{
    GLhandleARB v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB),
                f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB),
                p;

	char *vs = NULL, *fs = NULL;

	vs = textFileRead("myshader.v");
    fs = textFileRead("myshader.f");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSourceARB(v, 1, &vv, NULL);
	glShaderSourceARB(f, 1, &ff, NULL);

	free(vs);free(fs);

	glCompileShaderARB(v);
	glCompileShaderARB(f);

    p = glCreateProgramObjectARB();
	glAttachObjectARB(p,v);
	glAttachObjectARB(p,f);

	glLinkProgramARB(p);
	glUseProgramObjectARB(p);

    glUniform1iARB(glGetUniformLocationARB(p, "texture"), 0);
    glUniform3fARB(glGetUniformLocationARB(p, "light"), light0_pos[0], light0_pos[1], light0_pos[2]);
    glUniform4fARB(glGetUniformLocationARB(p, "l_ambient"), 0.4, 0.4, 0.4, 1.0 );
    glUniform4fARB(glGetUniformLocationARB(p, "l_diffuse"), 0.7, 0.7, 0.7, 1.0 );
    glUniform4fARB(glGetUniformLocationARB(p, "l_specular"), 1.0, 1.0, 1.0, 1.0 );
}

void drawtree(float x,float y,float z)
{
    glTranslatef(x, y, z); //Tree
    glScalef(2,2,2);
    drawObj(myObj4,21);
    glScalef(0.5,0.5,0.5);
    glTranslatef(-x, -y, -z);
}

void drawforest()
{
    for(int i=0;i<20;i+=1)
    {
        drawtree(20-2*i,1.55,40);
    }
    for(int i=0;i<20;i+=1)
    {
        drawtree(19-2*i,1.55,37);
    }
    for(int i=0;i<20;i+=1)
    {
        drawtree(18-2*i,1.55,34);
    }
    for(int i=0;i<20;i+=1)
    {
        drawtree(17-2*i,1.55,31);
    }
    for(int i=0;i<20;i+=1)
    {
        drawtree(16-2*i,1.55,28);
    }
    for(int i=0;i<20;i+=1)
    {
        drawtree(15-2*i,1.55,25);
    }
}

void drawsoldier1(float x,float y,float z)
{
    glTranslatef(x, y, z);   //Kenshiro
    drawObj(myObj2,5);
    glTranslatef(-x, -y, -z);
}

void drawmilitary1()
{
    for(int i=0;i<5;i+=1)
    {
        drawsoldier1(15,0.6,0-2*i);
    }
    for(int i=0;i<5;i+=1)
    {
        drawsoldier1(13,0.6,0-2*i);
    }
}

void drawsoldier2(float x,float y,float z)
{
    glTranslatef(x, y, z);    //Edward
    drawObj(myObj3,9);
    glTranslatef(-x, -y, -z);
}

void drawmilitary2()
{
    for(int i=0;i<5;i+=1)
    {
        drawsoldier2(-15,0.6,0-2*i);
    }
    for(int i=0;i<5;i+=1)
    {
        drawsoldier2(-13,0.6,0-2*i);
    }
}

void display(void)
{
    /* display callback, clear frame buffer and z buffer,
        rotate cube and draw, swap buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, up_x, up_y, up_z);

    //lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    light0_pos[0] = cos(lightTheta*PI/180.0);
    light0_pos[1] = 1/sin(PI/4) * sin(lightTheta*PI/180.0);
    light0_pos[2] = cos(lightTheta*PI/180.0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    //

    glEnable(GL_TEXTURE_2D);

    glTranslatef(0.0, 1.55, 0.0);   //moveable Dragon
    glScalef(8,8,8);
    drawObj(myObj,0);
    glScalef(0.125,0.125,0.125);
    glTranslatef(0.0, -1.55, 0.0);

    glTranslatef(delta_x, delta_y, delta_z);
    glRotatef(-angle,0,1,0);

    drawmilitary1();

    drawmilitary2();

    glTranslatef(0.0, 1.55, 0.0);    //Dragon
    glScalef(8,8,8);
    drawObj(myObj,0);
    glScalef(0.125,0.125,0.125);
    glTranslatef(0.0, -1.55, 0.0);

    drawforest();

    glScalef(8,8,8);    //environment
    glTranslatef(0.0, 0.375, 0.0);
    glBindTexture(GL_TEXTURE_2D, textureid[2]);
    glBegin( GL_QUADS ); //ground
        glTexCoord2f(0.0,0.0);
		glVertex3f( 10.0, -0.43, 10.0 );

        glTexCoord2f(0.0,1.0);
		glVertex3f( 10.0, -0.43, -10.0 );

        glTexCoord2f(1.0,1.0);
		glVertex3f( -10.0, -0.43, -10.0 );

        glTexCoord2f(1.0,0.0);
		glVertex3f( -10.0, -0.43, 10.0 );
	glEnd();

    glBindTexture(GL_TEXTURE_2D, textureid[3]);
	glBegin( GL_QUADS ); //sky
		glTexCoord2f(0.0,0.0);
		glVertex3f( 10.0, 9.0/*14.43*/, 10.0 );

        glTexCoord2f(0.0,1.0);
		glVertex3f( 10.0, 9.0, -10.0 );

        glTexCoord2f(1.0,1.0);
		glVertex3f( -10.0, 9.0, -10.0 );

        glTexCoord2f(1.0,0.0);
		glVertex3f( -10.0, 9.0, 10.0 );
	glEnd();

    //cylinder
	GLUquadricObj* objCylinder = gluNewQuadric();
	glRotatef(90, 1, 0, 0);
	glTranslated(0.0, 0.0, -9.6);
	glBindTexture(GL_TEXTURE_2D, textureid[4]);
	gluQuadricTexture(objCylinder, true);
    gluCylinder(objCylinder, 10.0/*buttom radius*/, 10.0/*top radius*/, 10.0/*height*/, 32, 5);

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {
        start_x = x;
        start_y = y;
    }
}

void mouseMotion(int x, int y)
{
    theta += 2 * static_cast<double> (x - start_x) / width;
    if(theta > 2 * PI) theta -= 2 * PI;
    if(theta < -2 * PI) theta += 2 * PI;
    GLdouble tmp = phi;
    phi += 2 * static_cast<double> (y - start_y) / height;
    if(phi > 0 && phi < PI)
        center_y = eye_y + cos(phi);
    else
        phi = tmp;
    center_x = eye_x + sin(phi) * cos(theta);
    center_z = eye_z + sin(phi) * sin(theta);
    start_x = x;
    start_y = y;
}

void keyboard(unsigned char key,int x,int y)
{
    //²¾°Ê
    if(key == 'q' || key == 'Q') //quit
        exit(0);
    if(key == 'w' || key == 'W') //move forward
    {
        if(!ridingMode)
        {
            eye_x += 0.1 * sin(phi) * cos(theta);
            eye_y += 0.1 * cos(phi);
            eye_z += 0.1 * sin(phi) * sin(theta);
            center_x += 0.1 * sin(phi) * cos(theta);
            center_y += 0.1 * cos(phi);
            center_z += 0.1 * sin(phi) * sin(theta);
        }
        else
            delta_z += 0.2;
    }
    if(key == 's' || key == 'S') //move backward
    {
        if(!ridingMode)
        {
            eye_x -= 0.1 * sin(phi) * cos(theta);
            eye_y -= 0.1 * cos(phi);
            eye_z -= 0.1 * sin(phi) * sin(theta);
            center_x -= 0.1 * sin(phi) * cos(theta);
            center_y -= 0.1 * cos(phi);
            center_z -= 0.1 * sin(phi) * sin(theta);
        }
        else
            delta_z -= 0.2;
    }
    if(key == 'a' || key == 'A') //move left
    {
        if(!ridingMode)
        {
            eye_x += 0.1 * sin(phi) * sin(theta);
            eye_z += -0.1 * sin(phi) * cos(theta);
            center_x += 0.1 * sin(phi) * sin(theta);
            center_z += -0.1 * sin(phi) * cos(theta);
        }
        else
            delta_x += 0.2;
    }
    if(key == 'd' || key == 'D') //move right
    {
        if(!ridingMode)
        {
            eye_x += -0.1 * sin(phi) * sin(theta);
            eye_z += 0.1 * sin(phi) * cos(theta);
            center_x += -0.1 * sin(phi) * sin(theta);
            center_z += 0.1 * sin(phi) * cos(theta);
        }
        else
            delta_x -= 0.2;
    }
    if(key == 'r' || key == 'R') // up
    {
        if(!ridingMode)
        {
            eye_y += 0.1;
            center_y += 0.1;
        }
        else
            delta_y -= 0.2;
    }
    if(key == 'f' || key == 'F') // down
    {
        if(!ridingMode)
        {
            eye_y -= 0.1;
            center_y -= 0.1;
        }
        else
            delta_y += 0.2;
    }

    if(key=='+')
    {
        lightTheta += 10.0;
//        lightTheta+=PI/6;
//        if(lightTheta>=PI*2)
//        {
//            lightTheta-=PI*2;
//        }
//        light0_pos[1]=sin(lightTheta);
//        light0_pos[2]=cos(lightTheta);
    }
    else if(key=='-')
    {
        lightTheta -= 10.0;
//        lightTheta-=PI/6;
//        if(lightTheta<=0)
//        {
//            lightTheta+=PI*2;
//        }
//        light0_pos[1]=sin(lightTheta);
//        light0_pos[2]=cos(lightTheta);
    }


    if(key==27)
    {
        exit(0);
    }

    if(key == 'e' || key == 'E')
    {
        ridingMode = !ridingMode;
        eye_x = 0.0;
        eye_y = 3;
        eye_z = -3.5;
        theta = -PI/2;
        phi = PI / 2;
    }
    if(key == 't' || key == 'T')
    {
        shader = !shader;
        if(shader)
        {
            setShaders();
        }
    }
}

void special(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) // look up
        if(phi - 0.02 > 0) phi -= 0.02;
    if(key == GLUT_KEY_DOWN) // look down
        if(phi + 0.02 < PI) phi += 0.02;
    if(key == GLUT_KEY_LEFT) // turn left
    {
        if(!ridingMode)
        {
            theta -= 0.1;
            if(theta <= -2 * PI) theta += 2 * PI;
        }
        else
        {
            angle += 1;
        }
    }
    if(key == GLUT_KEY_RIGHT) // turn right
    {
        if(!ridingMode)
        {
            theta += 0.1;
            if(theta >= 2 * PI) theta -= 2 * PI;
        }
        else
        {
            angle -= 1;
        }
    }
    center_x = eye_x + sin(phi) * cos(theta);
    center_y = eye_y + cos(phi);
    center_z = eye_z + sin(phi) * sin(theta);
}

void myReshape(int w, int h)
{
    width = w;
    height = h;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(50.0, ratio, 0.1, 100000.0);
    glMatrixMode(GL_MODELVIEW);
}

void loadTexture(GLuint textureid,char* filename)
{
    readPPM(filename,&texture[t]);
    glBindTexture(GL_TEXTURE_2D, textureid);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,texture[t].xRes,texture[t].yRes,GL_RGB,GL_UNSIGNED_BYTE, texture[t].pPixel);
    t+=1;
}

void init()
{
    textureid = new GLuint[number];
    glGenTextures(number, textureid);

    printf("Load Dragron texture\n");
    loadTexture(textureid[0], "appm/alduin.ppm");
    loadTexture(textureid[1], "appm/alduineyes.ppm");

    printf("\nLoad environment texture\n");
    loadTexture(textureid[2], "environmentppm/grass.ppm");
    loadTexture(textureid[3], "environmentppm/sky.ppm");
    loadTexture(textureid[4], "environmentppm/scene.ppm");

    printf("Load Kenshiro texture\n");
    loadTexture(textureid[5], "kppm/Kenshiro_default_d.ppm");
    loadTexture(textureid[6], "kppm/Kenshiro_hair_torso_d.ppm");
    loadTexture(textureid[7], "kppm/Kenshiro_props_d.ppm");
    loadTexture(textureid[8], "kppm/Kenshiro_default_d.ppm");

    printf("\nLoad Edward texture\n");
    loadTexture(textureid[9], "eppm/Edward_Kenway_Necklace_D.ppm");
    loadTexture(textureid[10], "eppm/Edward_Kenway_Mouth_D.ppm");
    loadTexture(textureid[11], "eppm/Edward_Kenway_Head_D.ppm");
    loadTexture(textureid[12], "eppm/Edward_Kenway_Arms_D.ppm");
    loadTexture(textureid[13], "eppm/Edward_Kenway_Hair_D.ppm");
    loadTexture(textureid[14], "eppm/Edward_Kenway_Eye_D.ppm");
//    loadTexture(textureid[14], "eppm/Edward_Kenway_Eye_Reflection_D.ppm");
    loadTexture(textureid[15], "eppm/Edward_Kenway_Eye_D.ppm");
    loadTexture(textureid[16], "eppm/Edward_Kenway_Dagger2_D.ppm");
    loadTexture(textureid[17], "eppm/Edward_Kenway_Dagger1_D.ppm");
    loadTexture(textureid[18], "eppm/Edward_Kenway_Clothes3_D.ppm");
    loadTexture(textureid[19], "eppm/Edward_Kenway_Clothes2_D.ppm");
    loadTexture(textureid[20], "eppm/Edward_Kenway_Clothes1_D.ppm");

    printf("\nLoad Tree texture\n");
    loadTexture(textureid[21], "tppm/palm_leaf_d.ppm");
    loadTexture(textureid[22], "tppm/palm_trunk_d.ppm");
}

int main(int argc, char **argv)
{
    myObj = glmReadOBJ("alduin.obj");
    myObj2 = glmReadOBJ("Kenshiro_outfit_A.obj");
    myObj3 = glmReadOBJ("Edward_Kenway.obj");
    myObj4 = glmReadOBJ("palm_tree.obj");

//    while(myObj4->groups)
//    {
//        printf("%s\n",myObj4->groups->name);
//        myObj4->groups=myObj4->groups->next;
//    }
    glmUnitize(myObj);
    glmUnitize(myObj2);
    glmUnitize(myObj3);
    glmUnitize(myObj4);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500,500);

    glutCreateWindow("HW3");
	glutDisplayFunc(display);
	glutReshapeFunc(myReshape);
	glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
	glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */

    init();

	glewInit();
	if(shader)
	{
	    setShaders();
	}

	glutMainLoop();
    return 0;
}
