/* Rotating cube with color interpolation */

/* Demonstration of use of homogeneous coordinate
transformations and simple data structure for representing
cube from Chapter 4 */

/*Both normals and colors are assigned to the vertices */
/*Cube is centered at origin so (unnormalized) normals
are the same as the vertex values */

#define GLEW_STATIC

#include <Windows.h>    // for solving the Code::Blocks errors
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
int wave_mode = 0;
GLint loc;
GLhandleARB v,f,f2,p;

GLfloat light_theta=0.0;
GLfloat light0_pos[]={0.0,0.0,1.0, 1.0};

int width, height;
int start_x, start_y;
GLdouble theta = PI, phi = PI / 2;
GLdouble eye_x = 0.5, eye_y = 0.0, eye_z = 0.0,
         center_x = eye_x + sin(phi) * cos(theta), center_y = eye_y + cos(phi), center_z = sin(phi) * sin(theta),
         up_x = 0.0, up_y = 1.0, up_z = 0.0;

bool inited = false;
bool shader_mode=true;

void setShaders()
{
	char *vs = NULL,*fs = NULL;

	if (! inited) {
		v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	}

    vs = textFileRead("toon.vert");
    fs = textFileRead("diffuse.frag");
//    if (shader_mode)
//    {
//        fs = textFileRead("toon.frag");
//	}
//	else
//    {
//        fs = textFileRead("diffuse.frag");
//    }

    const char * vv = vs;
	const char * ff = fs;
	glShaderSourceARB(v, 1, &vv,NULL);
	glShaderSourceARB(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShaderARB(v);
	glCompileShaderARB(f);

	if (! inited) {
		p = glCreateProgramObjectARB();
	}
	glAttachObjectARB(p,v);
	glAttachObjectARB(p,f);

	glLinkProgramARB(p);

	glUseProgramObjectARB(p);

	loc = glGetUniformLocationARB(p, "angle");

}

ColorImage x;
void drawOBJ()
{
     if (! myObj) return;

     glBegin(GL_TRIANGLES);

    //printf("%s\n",myObj->groups->name);

    GLMgroup *groups = myObj->groups;
//    GLMmaterial* material;
//    for(int i=0;i<x.xRes*x.yRes;i+=1)
//    {
//        glNormal3f(x.pPixel[i].R,x.pPixel[i].G,x.pPixel[i].B);
//    }
    while(groups)
    {
//        material = &myObj->materials[groups->material];
//        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambient);
//        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
//        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->specular);
//        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
        for(int i=0;i<groups->numtriangles;i+=1)
        {
            for (int v=0; v<3; v+=1)
            {
                //glNormal3fv(& myObj->vertices[myObj->triangles[groups->triangles[i]].nindices[v]*3 ]);
                glTexCoord2fv(& myObj->texcoords[myObj->triangles[groups->triangles[i]].tindices[v]*2 ]);
                glVertex3fv(& myObj->vertices[myObj->triangles[groups->triangles[i]].vindices[v]*3 ]);
            }
        }
        groups=groups->next;
    }

     glEnd();
//    glmDraw(myObj,GLM_TEXTURE|GLM_SMOOTH|GLM_MATERIAL);
    //glmDraw(myObj,GLM_TEXTURE);
}

float eyex=0.5,eyey=0.0,eyez=0.0,turnhorizon=0.0,turnvertical=0.0,turnback=-1;

void display(void)
{
    /* display callback, clear frame buffer and z buffer,
        rotate cube and draw, swap buffers */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawOBJ();

    //glMatrixMode(GL_PROJECTION);//==
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluPerspective(50.0, 1.0, 0.01, 10.0);//==
    //glMatrixMode(GL_MODELVIEW);//==
    //glLoadIdentity();//===
    gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, up_x, up_y, up_z);
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

//float horizon_walk_displacement=0.01,vertical_walk_displacement=0.01,horizon_view_displacement=0.05,vertical_view_displacement=0.05;
void keyboard(unsigned char key,int x,int y)
{
    //²¾°Ê
    if(key == 'q' || key == 'Q') //quit
        exit(0);
    if(key == 'w' || key == 'W') //move forward
    {
        eye_x += 0.01 * sin(phi) * cos(theta);
        eye_y += 0.01 * cos(phi);
        eye_z += 0.01 * sin(phi) * sin(theta);
        center_x += 0.01 * sin(phi) * cos(theta);
        center_y += 0.01 * cos(phi);
        center_z += 0.01 * sin(phi) * sin(theta);
    }
    if(key == 's' || key == 'S') //move backward
    {
        eye_x -= 0.01 * sin(phi) * cos(theta);
        eye_y -= 0.01 * cos(phi);
        eye_z -= 0.01 * sin(phi) * sin(theta);
        center_x -= 0.01 * sin(phi) * cos(theta);
        center_y -= 0.01 * cos(phi);
        center_z -= 0.01 * sin(phi) * sin(theta);
    }
    if(key == 'a' || key == 'A') //move left
    {
        eye_x += 0.01 * sin(phi) * sin(theta);
        eye_z += -0.01 * sin(phi) * cos(theta);
        center_x += 0.01 * sin(phi) * sin(theta);
        center_z += -0.01 * sin(phi) * cos(theta);
    }
    if(key == 'd' || key == 'D') //move right
    {
        eye_x += -0.01 * sin(phi) * sin(theta);
        eye_z += 0.01 * sin(phi) * cos(theta);
        center_x += -0.01 * sin(phi) * sin(theta);
        center_z += 0.01 * sin(phi) * cos(theta);
    }
    if(key == 'r' || key == 'R') // up
    {
        eye_y += 0.01;
        center_y += 0.01;
    }
    if(key == 'f' || key == 'F') // down
    {
        eye_y -= 0.01;
        center_y -= 0.01;
    }

    if(key=='+')
    {
        light_theta+=PI/6;
        if(light_theta>=PI*2)
        {
            light_theta-=PI*2;
        }
        light0_pos[1]=sin(light_theta);
        light0_pos[2]=cos(light_theta);
    }
    else if(key=='-')
    {
        light_theta-=PI/6;
        if(light_theta<=0)
        {
            light_theta+=PI*2;
        }
        light0_pos[1]=sin(light_theta);
        light0_pos[2]=cos(light_theta);
    }

    if(key==27)
    {
        exit(0);
    }

    if(key=='t' || key=='T')
    {
        shader_mode=!shader_mode;
        setShaders();
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
        theta -= 0.1;
        if(theta <= -2 * PI) theta += 2 * PI;
    }
    if(key == GLUT_KEY_RIGHT) // turn right
    {
        theta += 0.1;
        if(theta >= 2 * PI) theta -= 2 * PI;
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
    gluPerspective(50.0, ratio, 0.01, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    myObj = glmReadOBJ("alduin.obj");
    glmUnitize(myObj);
    ColorImage texture[2];
//    printf("%d\n",myObj->numgroups);
//    while(myObj->groups!=NULL)
//    {
//        printf("%s %d\n",myObj->groups->name,myObj->groups->numtriangles);
//        myObj->groups=myObj->groups->next;
//    }
//    glutInit(&argc, argv);
//    readPPM("appm/alduin_n.ppm", &x);
//    for(int i=0;i<x.xRes*x.yRes;i+=1)
//    {
//        printf("%d %d %d\n",x.pPixel[i].R,x.pPixel[i].G,x.pPixel[i].B);
//    }
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

    glEnable(GL_TEXTURE_2D);
    printf("Reading texture from image file...\n");
    readPPM("appm/alduin.ppm", &texture[0]);
    //readPPM("appm/alduin_n.ppm", &x);
    //readPPM("ppm/Connor_clothes3_D.ppm", &texture[1]);
    glTexImage2D(GL_TEXTURE_2D,0,3,texture[0].xRes,texture[0].yRes,0,GL_RGB,GL_UNSIGNED_BYTE, texture[0].pPixel);
    //glTexImage2D(GL_TEXTURE_2D,0,3,texture[1].xRes,texture[1].yRes,0,GL_RGB,GL_UNSIGNED_BYTE, texture[1].pPixel);
    //The default is the same as: glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glewInit();
    //setShaders();

	glutMainLoop();
    return 0;
}
