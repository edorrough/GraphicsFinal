/*
 *  Evan Dorrough - HW3 - Textures and Lighting
 *
 *  Demonstrates using lighting and textures.Demonstrates using lighting and textures
 *  to create a scene consisting of solid objects that can be viewed in three dimensions under user control. 
 *  The light should move and be contollable, and the textures add realism to the scene.
 *
 *
 *  Majority of structure is taken from example 15. the rest comes from my previous homework assignment.
 *
 *  Key bindings:
 *  l          Switch through lighting choices (floating, streetlight, none)
 *  t          Change textures
 *  m          Toggles texture mode modulate/replace
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int mode=0;       //  Texture mode
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=2;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 30;  // Diffuse intensity (%)
int specular  =   65;  // Specular intensity (%)
int foo = 0;
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   1;  // Elevation of light
float zlight  =   0;  // Z position of light
unsigned int texture[5]; // Texture names


//my code for objects in my scene
static void sidewalk()
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();

  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[4]);


  glBegin(GL_QUADS);// angled section
  glNormal3f(0.0f, 1.0f, -1.0f);
  glTexCoord2f(0,5); glVertex3f(3, 0, 1);//front left
  glTexCoord2f(0,0); glVertex3f(-3, 0, 1);//back left
  glTexCoord2f(1,0); glVertex3f(-3, 0.2, 1.2);//back right elevated
  glTexCoord2f(1,5); glVertex3f(3, 0.2, 1.2);//front right elevated
  glEnd();

  glBegin(GL_QUADS);// flat section
  glNormal3f(0.0f, 1.0f, 0.0f);
  glTexCoord2f(0,3); glVertex3f(3, 0.2, 1.2);//front left
  glTexCoord2f(0,0); glVertex3f(-3, 0.2, 1.2);//back left
  glTexCoord2f(1,0); glVertex3f(-3, 0.2, 2);//back right
  glTexCoord2f(1,3); glVertex3f(3, 0.2, 2);//front right 
  glEnd();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

static void pole(double quad, double base, double top, double height)
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();

  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[2]);

  glBegin(GL_POLYGON);
  GLUquadricObj *obj = NULL;
  obj = gluNewQuadric();
  gluQuadricDrawStyle(obj, GLU_FILL);
  gluQuadricTexture(obj, GLU_TRUE);
  gluQuadricNormals(obj, GLU_FLAT);
  gluQuadricOrientation(obj, GLU_OUTSIDE);
  //glColor3f(67, 70, 75);

  gluCylinder(obj, base, top, height, 30, 30);
  glEnd();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

//  from gluSphere
static void bulb(double quad, double radius)
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();

  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[3]);


  glBegin(GL_POLYGON);
  GLUquadricObj *obj = NULL;
  obj = gluNewQuadric();
  gluQuadricDrawStyle(obj, GLU_FILL);
  gluQuadricTexture(obj, GLU_TRUE);
  gluQuadricNormals(obj, GLU_SMOOTH);
  gluQuadricOrientation(obj, GLU_OUTSIDE);
  //glColor3f(255, 255, 0);
  gluSphere(obj, radius, 30, 30);
  glEnd();
  //  Undo transformations
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

static void road()
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();

  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[0]);


  glBegin(GL_QUADS);
  glColor3f(0.1, 0.1, 0.1);
  glNormal3f(0.0f, 1.0f, 0.0f);
  glTexCoord2f(3,1); glVertex3f(3, 0, 1);
  glTexCoord2f(0,1); glVertex3f(-3, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-3, 0, -1);
  glTexCoord2f(3,0); glVertex3f(3, 0, -1);
  glEnd();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

static void stripe()
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();

  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[1]);


  glBegin(GL_QUADS);
  //glColor3f(255, 255, 0);
  glNormal3f(0.0f, 1.0f, 0.0f);
  glTexCoord2f(5,1); glVertex3f(0.5, 0.01, 0.1);
  glTexCoord2f(0,1); glVertex3f(-0.5, 0.01, 0.1);
  glTexCoord2f(0,0); glVertex3f(-0.5, 0.01, -0.1);
  glTexCoord2f(5,0); glVertex3f(0.5, 0.01, -0.1);
  glEnd();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);

}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

static void drawRoad()
{
  glPushMatrix();
  road();
  glTranslatef(-2, 0, 0);
  stripe();
  glTranslatef(2, 0, 0);
  stripe();
  glTranslatef(2, 0, 0);
  stripe();
  glPopMatrix();
}
static void drawStreetlight()
{
  glPushMatrix();
  glRotatef(90, -1, 0, 0);
  pole(1, 0.1, 0.05, 2);
  glRotatef(-90, -1, 0, 0);
  glTranslatef(0, 1.9, 0);
  glRotatef(30, -1, 0, 0);
  pole(1, 0.05, 0.05, 1);
  glRotatef(-30, -1, 0, 0);
  glTranslatef(0, 0.5, 0.866);
  bulb(1, 0.1);
  glPopMatrix();
}
static void drawSidewalk()
{
  glPushMatrix();
  glTranslatef(-1, 0, 1);
  sidewalk();
  //glTranslatef(0, 0, 0);  
  glRotatef(180, 0, 1, 0);
  sidewalk();
  glPopMatrix();
}
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Length of axes
   const double len=2.0;
   //  Eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   
   //  Light switch
   if (light == 0)
      glDisable(GL_LIGHTING);
   else if (light == 1)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,zlight,1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(2.7*Cos(zh), ylight, zlight, 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   }
   else if(light == 2)//STREETLIGHT is not parented to physical model
   {
          // Streetlight values
      float Ambient[]   = {0.01*ambient,0.01*ambient,0,1.0};
      float Diffuse[]   = {0.01*diffuse,0.01*diffuse,0,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0,1.0};
      float Position[]  = {1, 3.4, 0.866, 1};
      float Direction[]  = {0, 0, 0};
      float Exponent[] = {16};  //Spotlight intensity, not more than 128
      float Cutoff[] = {100};  //Spotlight range, between 0 and 90, 180
      glEnable(GL_NORMALIZE);
      glEnable(GL_LIGHTING);
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, Direction);
      glLightfv(GL_LIGHT0,GL_SPOT_EXPONENT, Exponent);
      glLightfv(GL_LIGHT0,GL_SPOT_CUTOFF, Cutoff);
   }


  //  Draw scene
  glPushMatrix();
  drawRoad();
  glTranslatef(1, 0, -1);
  drawStreetlight();
  drawSidewalk();
  glPopMatrix();

   
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate");
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle texture mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
   {
      light++;
      if (light == 3)
         {
            light = 0;
         }
   }
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(45,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("HW3 Scene - Textures and Lighting");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[0] = LoadTexBMP("asphalt.bmp");
   texture[1] = LoadTexBMP("stripe.bmp");
   texture[2] = LoadTexBMP("pole.bmp");
   texture[3] = LoadTexBMP("lamp.bmp");
   texture[4] = LoadTexBMP("sidewalk.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
