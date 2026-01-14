#include "qglbegin.h"
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<time.h>
#include<unistd.h>

// www.amin-ahmadi.com

GLUquadric *quadric;

QGLBegin::QGLBegin(QWidget *parent, QStatusBar *sb) : QOpenGLWidget(parent)
{
    parentStatusBar = sb;
    st = new Struct();
    glwidth = 501;
    glheight = 711;
    editmode  = false;
    rRangeVP = 15.0;
    lRangeVP = -15.0;
    uRangeVP = 15.0;
    dRangeVP = -15.0;
    rRange = 15.0;
    lRange = -15.0;
    uRange = 15.0;
    dRange = -15.0;
    nRange = 15.0;
    scale = 1.0;
    xrot = 0.0; yrot = 0.0; rot_ang = 0.01;
    mouseX = 300; mouseY = 300; mouseYbef = 300; mouseXbef= 300;
    light_enabled = true; material_enabled = true;
    lclick = false; rclick = true; nobondmode = false;
    paint_sel_rectangle = false;
    draging = false;
    moving = false;
    quality = 20;
    qualityBond = 20;
    oldquality = 20;
    shininess = 100;
    selrectx = 0; selrecty = 0;
    xrot = yrot = zrot = 0.0;
    parentCombotxt = "";

    timer = new QTimer(this);
    timer->setInterval(time_delay);
    connect(timer, SIGNAL(timeout()), this, SLOT(playmovieslot()));
    freqtimer = new QTimer(this);
    freqtimer->setInterval(time_delay);
    connect(freqtimer, SIGNAL(timeout()), this, SLOT(playfreqslot()));

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

}

void QGLBegin::initializeGL()
{
    glClearColor(bc[0], bc[1], bc[2], bc[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    enableGlobProp();
}

void QGLBegin::paintGL()
{
//    glClearColor(bc[0],bc[1],bc[2],bc[3]);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    enableGlobProp();
    draw();
}

void QGLBegin::update2(){
    paintGL();
}

void QGLBegin::onSize(){
    makeCurrent();
//    printf("OpenGL version supported by this platform (%s): \n",
//            glGetString(GL_VERSION));
//    printf("OpenGL version supported by this platform (%s): \n",
//           glGetString(GL_RENDERER));

//   printf("%i\n", context()->format().minorVersion());
//   printf("%i\n", context()->format().majorVersion());

    int w = width(), h = height();
    glwidth = w;  // gobal width
    glheight = h; // global heigh
    if(h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(w <= h){
        lRangeVP = lRange; rRangeVP = rRange;
        dRangeVP = dRange*h/w; uRangeVP = uRange*h/w;
//        gluPerspective(90.0, -nRange*height/width, -nRange, nRange);
//        glOrtho(lRange,rRange, dRange*h/w, uRange*h/w, -4*rRange, 2*rRange);
        glOrtho(lRange,rRange, dRangeVP, uRangeVP, -10*rRange, 10*rRange);
    }else{
        lRangeVP = lRange*w/h; rRangeVP = rRange*w/h;
        dRangeVP = dRange; uRangeVP = uRange;
//        gluPerspective(90.0, -nRange*width/height, -nRange, nRange);
//        glOrtho(lRange*w/h, rRange*w/h, dRange, uRange, -4*rRange, 2*rRange);
        glOrtho(lRangeVP, rRangeVP, dRange, uRange, -10*rRange, 10*rRange);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void QGLBegin::resizeGL(int w, int h)
{
    onSize();
}

void QGLBegin::enableGlobProp(){
    if(light_enabled){
           glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
           glEnable(GL_DEPTH_TEST);
           glEnable(GL_LIGHTING);
           glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
           glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
           glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
           glEnable(GL_LIGHT0);
           glEnable(GL_TEXTURE_2D);
           glBindTexture( GL_TEXTURE_2D, 1 );
           glEnable( GL_NORMALIZE );
       }else{
           glDisable(GL_LIGHTING);
       }
       if(material_enabled){
           glEnable(GL_COLOR_MATERIAL);
           glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
           glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
           glMateriali(GL_FRONT, GL_SHININESS, shininess);
           glColor3ub(230, 100, 100);
       }else{
           glDisable(GL_COLOR_MATERIAL);
       }

}

void QGLBegin::renderCone(std::array<float,3> coor1, std::array<float,3> coor2, float radio, float radio2, int subdiv, std::array<float,4> color){
    float x1, y1, z1, x2, y2, z2, vx, vy, vz, ax, rx, ry, v;
    x1 = coor1[0];
    y1 = coor1[1];
    z1 = coor1[2];
    x2 = coor2[0];
    y2 = coor2[1];
    z2 = coor2[2];

    vx = x2-x1;
    vy = y2-y1;
    vz = z2-z1;

    //handle the degenerate case of z1 == z2 with an approximation
    if (vz == 0) vz = .0001;

    v = sqrt(vx*vx + vy*vy + vz*vz);
    ax = 57.2957795*acos(vz/v);
    if (vz < 0.0) ax = -ax;
    rx = -vy*vz;
    ry = vx*vz;
    glPushMatrix();

    //draw the cylinder body
    glTranslatef(x1,y1,z1);
    glRotatef(ax, rx, ry, 0.0);
    glColor4f(color[0],color[1],color[2], color[3]);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radio, radio2, v, subdiv, 1);
    glPopMatrix();

}

void QGLBegin::renderBonds(std::array<float,3> coor1, std::array<float,3> coor2, float radio, int subdiv, std::array<float,4> color){
    float x1, y1, z1, x2, y2, z2, vx, vy, vz, ax, rx, ry, v;
    x1 = coor1[0];
    y1 = coor1[1];
    z1 = coor1[2];
    x2 = coor2[0];
    y2 = coor2[1];
    z2 = coor2[2];

    vx = x2-x1;
    vy = y2-y1;
    vz = z2-z1;

    //handle the degenerate case of z1 == z2 with an approximation
    if (vz == 0) vz = .0001;

    v = sqrt(vx*vx + vy*vy + vz*vz);
    ax = 57.2957795*acos(vz/v);
    if (vz < 0.0) ax = -ax;
    rx = -vy*vz;
    ry = vx*vz;
    glPushMatrix();

    //draw the cylinder body
    glTranslatef(x1,y1,z1);
    glRotatef(ax, rx, ry, 0.0);
    glColor4f(color[0],color[1],color[2], color[3]);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radio, radio, v, subdiv, 1);
    glPopMatrix();

}

void QGLBegin::draw(){
     array<float, 3> semis; // first, last, v1, v2, vdes;
//     float cosang;
    // void *font = GLUT_BITMAP_TIMES_ROMAN_10;


     glClearColor(bc[0],bc[1],bc[2],bc[3]);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     //show light in scene ------------------------------------------------------------------
     if(editLightMode){
         glPushMatrix();
         glTranslatef((GLfloat)lightPos[0], (GLfloat)lightPos[1], (GLfloat)lightPos[2]);
         glColor4f(1.f, 1.f, 1.f, 1.0f);
         quadric=gluNewQuadric();
         gluQuadricNormals(quadric, GLU_SMOOTH);
         gluSphere(quadric, 0.5, 12, 8);
         gluDeleteQuadric(quadric);
         glPopMatrix();
     }

//show light in scene ------------------------------------------------------------------



//     glPushAttrib(GL_ENABLE_BIT);
//     // glPushAttrib is done to return everything to normal after drawing

//     glLineStipple(1, 0xAAAA);  // [1]
//     glEnable(GL_LINE_STIPPLE);
//     glBegin(GL_LINES);
//     glVertex3f(-.5,.5,-.5);
//     glVertex3f(.5,.5,-.5);
//     glEnd();

//     glPopAttrib();

     for(auto &at:st->atoms){
         if (at.coor[0] < lRangeVP || at.coor[0] > rRangeVP || at.coor[1]<dRangeVP || at.coor[1]>uRangeVP){
             continue;
         }
         if(! at.visible){
             continue;
         }

         if(at.covered){
             continue;
         }

         for(auto bd:at.bonds){
             for(unsigned int i=0;i<3;i++){
                 semis[i] = (at.coor[i]+st->atoms[bd.neighbor].coor[i])/2.0;
             }

             if(st->atoms.size()>2000){
                 qualityBond = 5;
             }
             else{
                 qualityBond = quality;
             }

             quadric=gluNewQuadric();
             gluQuadricNormals(quadric, GLU_SMOOTH);
             renderBonds(at.coor, semis, bd.radio, qualityBond, at.color);
             gluDeleteQuadric(quadric);

             quadric=gluNewQuadric();
             gluQuadricNormals(quadric, GLU_SMOOTH);
             renderBonds(semis, st->atoms[bd.neighbor].coor, bd.radio, qualityBond, st->atoms[bd.neighbor].color);
             gluDeleteQuadric(quadric);
         }

         glPushMatrix();
         glTranslatef((GLfloat)at.coor[0], (GLfloat)at.coor[1], (GLfloat)at.coor[2]);
         glColor4f(at.color[0], at.color[1], at.color[2], at.color[3]);
         quadric=gluNewQuadric();
         gluQuadricNormals(quadric, GLU_SMOOTH);
//         glutSolidSphere(at.sphere, quality, 8);
         gluSphere(quadric, at.sphere, quality, 8);
         gluDeleteQuadric(quadric);
         glPopMatrix();
    }

     if(scatterplot_mode){
         for(auto sel:st->selat){
             if(!sel->visible){
                 continue;
             }
             glEnable(GL_BLEND);
             glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
             glColor4f(sel->color[0], sel->color[1],sel->color[2], sel->color[3]);
              for(unsigned int i=0;i<st->pelisize;i++){
                  glPushMatrix();
                  array<float, 3> ncoor = {sel->peli[i][0], sel->peli[i][1], sel->peli[i][2]};
                  st->glob_rotate(ncoor, st->trotmat);
                  glTranslatef((GLfloat)ncoor[0], (GLfloat)ncoor[1], (GLfloat)ncoor[2]);
                  quadric=gluNewQuadric();
                  gluQuadricNormals(quadric, GLU_SMOOTH);
                  gluSphere(quadric, sel->sphere, 10, 5);
                  gluDeleteQuadric(quadric);
                  glPopMatrix();
               }
          }
     }
     else if(scatterplot_mode_2d){
         for(auto sel:st->selat){
             if(!sel->visible){
                 continue;
             }
             glEnable(GL_BLEND);
             glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
             glColor4f(sel->color[0], sel->color[1],sel->color[2], sel->color[3]);
              for(unsigned int i=0;i<st->pelisize;i++){
                  glPushMatrix();
                  array<float, 3> ncoor = {sel->peli[i][0], sel->peli[i][1], sel->peli[i][2]};
                  st->glob_rotate(ncoor, st->trotmat);
                  glTranslatef((GLfloat)ncoor[0], (GLfloat)ncoor[1], (GLfloat)ncoor[2]);
                  glBegin(GL_POINTS);
                  glVertex3d((GLfloat)ncoor[0], (GLfloat)ncoor[1], (GLfloat)ncoor[2]);
                  glEnd();
                  glPopMatrix();
               }
          }
     }
     else{
         for(auto sel:st->selat){
             if(!sel->visible){
                 continue;
             }
             glPushMatrix();
             glEnable(GL_BLEND);
             glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
             glColor4f(0.0, 0.3, 1.0, 0.4);
             glTranslatef((GLfloat)sel->coor[0], (GLfloat)sel->coor[1], (GLfloat)sel->coor[2]);
             quadric=gluNewQuadric();
             gluQuadricNormals(quadric, GLU_SMOOTH);
             gluSphere(quadric, sel->sphere+0.2, quality, 5);
//             glutWireSphere(sel->sphere+0.2, quality, 5);
             gluDeleteQuadric(quadric);
             //------------------------------- text-----------
//             float color[4] = {sel->color[0], sel->color[1], sel->color[2], sel->color[3]};
//             glColor4fv(color);          // set text color
//             glRasterPos2i(sel->sphere+0.3, sel->sphere+0.3);        // place text position

//             // loop all characters in the string
//             const char *str = sel->label.toStdString().c_str();
//             while(*str)
//             {
//                 glutBitmapCharacter(font, *str);
//                 ++str;
//             }

             //glEnable(GL_TEXTURE_2D);
            //----------------------text-------
             glPopMatrix();
         }
     }


     if(paint_sel_rectangle){
         glColor4f(0.0, 0.3, 1.0, 0.3);
         glBegin(GL_QUADS);
         glVertex3f(selrectx, selrecty, 3*rRange);
         glVertex3f(selrectx, mouseY, 3*rRange);
         glVertex3f(mouseX, mouseY, 3*rRange);
         glVertex3f(mouseX, selrecty, 3*rRange);
         glEnd();
     }
     if(cropMode){
          glColor4f(1.0, 1.0, 1.0, 0.5);
          glBegin(GL_LINES);
          glVertex3f(selrectx, selrecty,3*rRange);
          glVertex3f(selrectx, mouseY,3*rRange);

          glVertex3f(selrectx, selrecty,3*rRange);
          glVertex3f(mouseX,selrecty,3*rRange);

          glVertex3f(mouseX, mouseY,3*rRange);
          glVertex3f(mouseX, selrecty,3*rRange);

          glVertex3f(mouseX, mouseY,3*rRange);
          glVertex3f(selrectx, mouseY,3*rRange);

          glEnd();
     }

     if(st->showcell){
	     //printf("showing cell \n");
         for(unsigned int i=0;i<3; i++){
		// glEnable (GL_POLYGON_SMOOTH);
		//glEnable (GL_BLEND);
		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glHint (GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
             glPushMatrix();
             glColor4f(st->cell_lines_colors[i][0], st->cell_lines_colors[i][1], st->cell_lines_colors[i][2], st->cell_lines_colors[i][3]);
             array<float, 4> color = {st->cell_lines_colors[i][0], st->cell_lines_colors[i][1], st->cell_lines_colors[i][2], st->cell_lines_colors[i][3]};
             array<float, 3> coor1 = {st->cell_lines[2*i][0], st->cell_lines[2*i][1], st->cell_lines[2*i][2]};
             array<float, 3> coor2 = {st->cell_lines[2*i+1][0], st->cell_lines[2*i+1][1], st->cell_lines[2*i+1][2]};
             array<float, 3> coor3;

             float lenarrow = 1.0f;
             float coef = lenarrow/(sqrt(st->cell[i][0]*st->cell[i][0] + st->cell[i][1]*st->cell[i][1] + st->cell[i][2]*st->cell[i][2]));

             for (unsigned int j=0;j<3;j++){
                 coor3[j] = coor2[j]-coor1[j]; // distance
                 coor3[j] = coor1[j]+coor3[j]*(1-coef);
             }
             quadric=gluNewQuadric();
             gluQuadricNormals(quadric, GLU_SMOOTH);
             renderBonds(coor1, coor3, st->cell_lines_widths[i], qualityBond, color);
             renderCone(coor2, coor3, 0.0f, st->cell_lines_widths[i]*3, qualityBond, color);
             gluDeleteQuadric(quadric);
             glPopMatrix();
         }

         for(unsigned int i=3;i<12;i++){
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_LINE_WIDTH);
	    glEnable(GL_LINE_SMOOTH);
            glLineWidth(st->cell_lines_widths[i]);
            glBegin(GL_LINES);
            glColor4f(st->cell_lines_colors[i][0], st->cell_lines_colors[i][1], st->cell_lines_colors[i][2], st->cell_lines_colors[i][3]);
            glVertex3f(st->cell_lines[2*i][0], st->cell_lines[2*i][1], st->cell_lines[2*i][2]);
            glVertex3f(st->cell_lines[2*i+1][0], st->cell_lines[2*i+1][1], st->cell_lines[2*i+1][2]);
	    //printf("%f  \n", st->cell_lines[2*i][0]);
            glEnd();
         }

//         for(unsigned int i=0;i<12;i++){
//            glEnable(GL_LINE_WIDTH);
//            glLineWidth(st->cell_lines_widths[i]);
//            glBegin(GL_LINES);
//            glColor4f(st->cell_lines_colors[i][0], st->cell_lines_colors[i][1], st->cell_lines_colors[i][2], st->cell_lines_colors[i][3]);
//            glVertex3f(st->cell_lines[2*i][0], st->cell_lines[2*i][1], st->cell_lines[2*i][2]);
//            glVertex3f(st->cell_lines[2*i+1][0], st->cell_lines[2*i+1][1], st->cell_lines[2*i+1][2]);
//            glEnd();
//         }
     }

     //draw arrows ---------------------------------------------------------------------
     for(auto ar:st->arrows){
         unsigned int i = ar.indices[0];
         unsigned int j = ar.indices[1];
         unsigned int k = ar.indices[2];
         glPushMatrix();
         quadric=gluNewQuadric();
         gluQuadricNormals(quadric, GLU_SMOOTH);
         renderBonds(st->atoms[i].coor, st->atoms[j].coor, st->atoms[i].cyl, qualityBond, st->atoms[j].color);
         renderCone(st->atoms[j].coor, st->atoms[k].coor, st->atoms[j].cyl, st->atoms[k].cyl, qualityBond, st->atoms[k].color);
         gluDeleteQuadric(quadric);
         glPopMatrix();
     }
     //draw arrows ---------------------------------------------------------------------

     //draw Planes ---------------------------------------------------------------------

     for(auto plane:st->planes){
          int i = plane.indices[0];
          int j = plane.indices[1];
          int k = plane.indices[2];
          int l = plane.indices[3];

          glColor4f(st->atoms[i].color[0],st->atoms[i].color[1], st->atoms[i].color[2], st->atoms[i].color[3]);
//          glEnable(GL_BLEND);
//          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          if(l == -1){
              glBegin(GL_TRIANGLES);
              glVertex3f(st->atoms[i].coor[0],st->atoms[i].coor[1], st->atoms[i].coor[2]);
              glVertex3f(st->atoms[j].coor[0],st->atoms[j].coor[1], st->atoms[j].coor[2]);
              glVertex3f(st->atoms[k].coor[0],st->atoms[k].coor[1], st->atoms[k].coor[2]);
          }
          else{
              glBegin(GL_QUADS);
              glVertex3f(st->atoms[i].coor[0],st->atoms[i].coor[1], st->atoms[i].coor[2]);
              glVertex3f(st->atoms[j].coor[0],st->atoms[j].coor[1], st->atoms[j].coor[2]);
              glVertex3f(st->atoms[k].coor[0],st->atoms[k].coor[1], st->atoms[k].coor[2]);
              glVertex3f(st->atoms[l].coor[0],st->atoms[l].coor[1], st->atoms[l].coor[2]);
          }
          glEnd();
     }
     //draw Planes ---------------------------------------------------------------------

//    char c = 'H';
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
 //    //glutBitmap
//    glRasterPos3d(0.0,0.0,0.0);

   // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
//   glFlush();
//   SwapBuffers();
   //scale = 0.0;
   //cout << "cuatro\n";
   return;
}

void QGLBegin::scatterplot(){
     array<float, 3> semis; // first, last, v1, v2, vdes;
//     float cosang;

     glClearColor(bc[0],bc[1],bc[2],bc[3]);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     for(auto &at:st->atoms){
         if (at.coor[0] < lRangeVP || at.coor[0] > rRangeVP || at.coor[1]<dRangeVP || at.coor[1]>uRangeVP){
             continue;
         }
         if(! at.visible){
             continue;
         }

         if(at.sel){
             continue;
         }

         for(auto bd:at.bonds){
             for(unsigned int i=0;i<3;i++){
                 semis[i] = (at.coor[i]+st->atoms[bd.neighbor].coor[i])/2.0;
             }
             quadric=gluNewQuadric();
             gluQuadricNormals(quadric, GLU_SMOOTH);
             renderBonds(at.coor, semis, bd.radio, quality, at.color);
             gluDeleteQuadric(quadric);

             quadric=gluNewQuadric();
             gluQuadricNormals(quadric, GLU_SMOOTH);
             renderBonds(semis, st->atoms[bd.neighbor].coor, bd.radio, quality, st->atoms[bd.neighbor].color);
             gluDeleteQuadric(quadric);
         }

         glPushMatrix();
         glTranslatef((GLfloat)at.coor[0], (GLfloat)at.coor[1], (GLfloat)at.coor[2]);
         glColor4f(at.color[0], at.color[1], at.color[2], at.color[3]);
         quadric=gluNewQuadric();
         gluQuadricNormals(quadric, GLU_SMOOTH);
         gluSphere(quadric, at.sphere, quality, 5);
         gluDeleteQuadric(quadric);
         glPopMatrix();
     }

     //draw the selected atoms as scatter points
     glEnable(GL_POINT_SMOOTH);
     glPointSize(8.0);
     glBegin(GL_POINTS);
         cout<<"begin rendering"<<endl;
     for(auto sel:st->selat){
         if(!sel->visible){
             continue;
         }
         for(int i=0;i<st->pelisize;i++){
//             cout <<"scattering\n";
            glVertex3f((GLfloat)sel->peli[i][0], (GLfloat)sel->peli[i][1], (GLfloat)sel->peli[i][2]);
         }
     }
      glEnd();
      //draw the selected atoms as scatter points

     if(st->showcell){
         glEnable(GL_LINE_WIDTH);
         glLineWidth(3);
         glBegin(GL_LINES);

         glColor4f(1.0, 0.0, 0.0, 0.8);
         glVertex3f(st->cell_lines[0][0], st->cell_lines[0][1], st->cell_lines[0][2]);
         glVertex3f(st->cell_lines[1][0], st->cell_lines[1][1], st->cell_lines[1][2]);
         glColor4f(0.0, 1.0, 0.0, 0.8);
         glVertex3f(st->cell_lines[2][0], st->cell_lines[2][1], st->cell_lines[2][2]);
         glVertex3f(st->cell_lines[3][0], st->cell_lines[3][1], st->cell_lines[3][2]);
         glColor4f(0.0, 0.0, 1.0, 0.8);
         glVertex3f(st->cell_lines[4][0], st->cell_lines[4][1], st->cell_lines[4][2]);
         glVertex3f(st->cell_lines[5][0], st->cell_lines[5][1], st->cell_lines[5][2]);
         glColor4f(1.0, 1.0, 1.0, 0.8);
         for(unsigned int i=6;i<24;i++){
             glVertex3f(st->cell_lines[i][0], st->cell_lines[i][1], st->cell_lines[i][2]);
         }
         glEnd();
     }
   return;
}


void QGLBegin::setRotation(float x, float y, float z)
{
    xrot = x;
    yrot = y;
    zrot = z;
    update();
}

void QGLBegin::wheelEvent(QWheelEvent *event){
    if(event->angleDelta().y()>0) {
       rRange -= 1;
       lRange += 1;
       uRange -= 1;
       dRange += 1;
    }else{
      rRange += 1;
      lRange -= 1;
      uRange += 1;
      dRange -= 1;
    }

    onSize();
    update();
    event->ignore();
}

void QGLBegin:: mouseMoveEvent(QMouseEvent *event){
    mouseXbef = mouseX; mouseYbef = mouseY;
    mouseX = event->x();
    mouseY = event->y();
    dx = (mouseX - mouseXbef);
    dy = (mouseY - mouseYbef);


    if(nowCropping){
        mouseX /= glwidth; mouseY = (glheight-mouseY)/glheight;
        mouseX *= (rRangeVP - lRangeVP); mouseY *= (uRangeVP-dRangeVP);
        mouseX += lRangeVP; mouseY += dRangeVP;
        update();
        return;
    }
    if(event->buttons() == Qt::LeftButton && event->modifiers()==Qt::ControlModifier){
        mouseX /= glwidth; mouseY = (glheight-mouseY)/glheight;
        mouseX *= (rRangeVP - lRangeVP); mouseY *= (uRangeVP-dRangeVP);
        mouseX += lRangeVP; mouseY += dRangeVP;
        paint_sel_rectangle = true;
        update();
        return;
    }
    else if(event->buttons() == Qt::LeftButton && event->modifiers()==Qt::ShiftModifier){
        setCursor(Qt::CrossCursor);
        st->rotate_coors(rot_ang, dy, dx);
        update();
        return;
    }

    else if(event->buttons() == Qt::RightButton && event->modifiers()==Qt::ShiftModifier){
        setCursor(Qt::ClosedHandCursor);
        if(dy != 0){
            dy = dy/glheight;
            dy *= (uRangeVP-dRangeVP);
            dRange += dy;
            uRange += dy;
        }
        if(dx != 0){
            dx /= glwidth;
            dx *= (rRangeVP - lRangeVP);
            rRange -= dx;
            lRange -= dx;
        }
        onSize();
        update();
        return;
    }
   else if(moveAlwaysInsideCellMode && event->buttons() == Qt::LeftButton){
        setCursor(Qt::ClosedHandCursor);
        dx /= glwidth; dy = -dy/glheight;
        dx *= (rRangeVP - lRangeVP); dy *= (uRangeVP-dRangeVP);
        st->dragInsideCell(dx, dy);
        update();
        return;
    }
   else if(editLightMode && event->buttons() == Qt::LeftButton){
        setCursor(Qt::ClosedHandCursor);
        dx /= glwidth; dy = -dy/glheight;
        dx *= (rRangeVP - lRangeVP); dy *= (uRangeVP-dRangeVP);
        lightPos[0] += dx;
        lightPos[1] += dy;
        update();
        return;
    }
    else if(event->buttons() == Qt::LeftButton && editmode && st->selat.size() > 0){
        setCursor(Qt::CrossCursor);
        draging = true;
        dx /= glwidth; dy = -dy/glheight;
        dx *= (rRangeVP - lRangeVP); dy *= (uRangeVP-dRangeVP);
        st->drag(dx, dy);
//        emit data_changed();
        update();
        //no se si deba ir antes o despue de update
        updateParentLabelInfo();
    }
    else if(event->buttons() == Qt::RightButton && editmode && st->selat.size() > 0){
        setCursor(Qt::ClosedHandCursor);
        st->rotate_selection(rot_ang, dy, dx);
        update();
    }

    else if(event->buttons() == Qt::LeftButton){
        setCursor(Qt::ClosedHandCursor);
        std::thread t(&Struct::rotate_coors, st, rot_ang, dy, dx, 0.0);
        t.join();
        update();
        return;
    }
    else if(event->buttons() == Qt::RightButton){
        setCursor(Qt::ClosedHandCursor);
        if(dy != 0){
            dy = dy/glheight;
            dy *= (uRangeVP-dRangeVP);
            dRange += dy;
            uRange += dy;
        }
        if(dx != 0){
            dx /= glwidth;
            dx *= (rRangeVP - lRangeVP);
            rRange -= dx;
            lRange -= dx;
        }
        onSize();
        update();
        return;
    }
}

void QGLBegin::mouseReleaseEvent(QMouseEvent *event){
    setCursor(Qt::ArrowCursor);
    quality = oldquality;
    if(event->button() == 1){
        if(draging){
            draging = false;
            st->add_to_before("dragged", potential_dragged_coors);
            potential_dragged_coors.clear();
            st->genbonds();
//            emit data_changed();
        }
        if(paint_sel_rectangle){
            paint_sel_rectangle = false;
            bool added = st->add_rectangular_selection(selrectx, selrecty, mouseX, mouseY);
            if(added){
                emit atoms_selected();
            }
        }
        if(nowCropping){
            nowCropping = false;
            cropX2 = event->x(); cropY2 = event->y();
//            printf("%f  %f %f %f\n", selrectx, selrecty, mouseX, mouseY);
        }
        update();        
    }
}

void QGLBegin::mouseDoubleClickEvent(QMouseEvent *event){

    if(event->button() == 1){
        if(draging){
            return;
        }
        st->unselect_all();
        // Is a way of say that the status bar will not be updated
        update();
    }
}


void QGLBegin::mousePressEvent(QMouseEvent *event){
    oldquality= quality;
    quality = 20;
    float x = event->x(), y = event->y();
    x /= glwidth; y = (glheight-y)/glheight;
    x *= (rRangeVP - lRangeVP); y *= (uRangeVP-dRangeVP);
    x += lRangeVP; y += dRangeVP;
    selrectx = x; selrecty = y;

    if(cropMode && event->buttons()==Qt::LeftButton){
        cropX1 = event->x(); cropY1 = event->y();
        nowCropping = true;
    }

//    if(event->buttons() == Qt::LeftButton && event->modifiers()==Qt::ShiftModifier){

//    }
   if(event->buttons() == Qt::LeftButton && event->modifiers()==Qt::ControlModifier){
        bool selected = st->add_to_selection(x, y);
        if (selected){
            updateParentLabelInfo();
//            emit atoms_selected();
            //parent_status_bar->showMessage(parentLabelInfo);
        }
        update();
    }
    else if(event->buttons() == Qt::LeftButton && editmode){
        // if no atoms is clicked return -1, the clicked atom index otherwise
        int at = clicked_on_atom(x, y);
        if(st->selat.size()>0){
            // keep the coordinates in case they are actually dragged
            for(auto sel:st->selat){
                potential_dragged_coors.push_back(sel->coor);
            }
            return;
        }
        if(at != -1){
            st->selat.clear();
            st->selat.push_back(&st->atoms[at]);
        }
        else{
          emit comboRequest();
            if(parentCombotxt.compare("") == 0){
                return;
            }
            st->add_atom(x, y, parentCombotxt);
            emit data_changed();
        }
        update();
    }
}

void QGLBegin::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_N && event->modifiers() == Qt::ControlModifier){
        st->shift_peli("f");
        update();
    }
    else if(event->key() == Qt::Key_B && event->modifiers() == Qt::ControlModifier){
        st->shift_peli("b");
        update();
    }
    else if(event->key() == Qt::Key_Plus && event->modifiers() == Qt::ControlModifier){
        rRange -= 1;
        lRange += 1;
        uRange -= 1;
        dRange += 1;
        onSize();
        update();
    }
    else if(event->key() == Qt::Key_Minus && event->modifiers() == Qt::ControlModifier){
        rRange += 1;
        lRange -= 1;
        uRange += 1;
        dRange -= 1;
        onSize();
        update();
    }
    else if(event->key() == Qt::Key_A && event->modifiers() == Qt::ControlModifier){
        st->select_all();
        update();
        emit atoms_selected();
    }
    else if(event->key() == Qt::Key_Z){
        st->reverse_rotation(1);
        st->reset_mat(st->trotmat);
        update();
        st->genbonds();
    }
    else if(event->key() == Qt::Key_X){
        st->reverse_rotation(1);
        st->reset_mat(st->trotmat);
        st->rotate_coors(1.5707963267948966, 0, 1);
        update();
    }
    else if(event->key() == Qt::Key_Y){
        st->reverse_rotation(1);
        st->reset_mat(st->trotmat);
        st->rotate_coors(1.5707963267948966, -1, 0);
        update();
    }
    else if(event->key() == Qt::Key_Delete){
        st->delete_selection();
        update();
        emit data_changed();
    }
    else if(event->key()== Qt::Key_Up){
        st->rotate_coors(0.8*rot_ang, -1.0 , 0.0);
        update();
    }
    else if(event->key()== Qt::Key_Down){
        st->rotate_coors(0.8*rot_ang, 1.0 , 0.0);
        update();
    }
    else if(event->key()== Qt::Key_Left){
        st->rotate_coors(0.8*rot_ang, 0.0 , -1.0);
        update();
    }
    else if(event->key()== Qt::Key_Right){
        st->rotate_coors(0.8*rot_ang, 0.0 , 1.0);
        update();
    }
    else if(event->key()== Qt::Key_U){
        if(!st->valid_cell()){
            return;
        }

        if(moveAlwaysInsideCellMode){
            moveAlwaysInsideCellMode = false;
        }
        else{
            moveAlwaysInsideCellMode = true;
            editmode = false;
            scatterplot_mode = false;
            scatterplot_mode_2d = false;
            editLightMode = false;
        }
    }
    else if(event->key() == Qt::Key_L){
        if(editLightMode){
            editLightMode = false;
        }
        else{
            moveAlwaysInsideCellMode = false;
            editmode = false;
            scatterplot_mode = false;
            scatterplot_mode_2d = false;
            editLightMode = true;
        }
    }
    else if(event->key() == Qt::Key_I){
        for(auto s:st->selat){
            cout<<s->index<<" ";
        }
        cout<<endl;
    }
    event->ignore();
}

void QGLBegin::renderpovray(string outputfile){
//    //setlocale(LC_NUMERIC, "en_EN");
//    int at1, at2;
//    float semix, semiy, semiz;
//    float roughness = (128-shininess)/1280.0;
//    float pov_specular = (specref[0] + specref[1] + specref[0])/3.0;
////    cout << pov_specular<< endl;
////    string shadowless;
//    string cmd;

////    if(pov_specular == 0.0){
////        shadowless="shadowless";
////    }else{
////        shadowless = " ";
////    }

//    QString header = "global_settings {\n"
//                    "        ambient_light rgb <%1, %2, %3>\n"
//                    "       max_trace_level 15\n"
//                    "}\n\n"
//                    "background { color rgb <%4, %5, %6> }\n\n"
//                    "camera {\n"
//                    "       orthographic\n"
//                    "       location <%7, %8, %9>\n"
//                    "       angle %10\n"
//                    "       up <%11, %12, %13>\n"
//                    "       right <%16, %15, %14>\n"
//                    "       look_at <%17, %18, %19> }\n"

//                    "light_source {\n"
//                    "        <%20, %21, %22>\n"
//                    "        color rgb <%23, %24, %25> %26 \n"
//                    "        fade_distance 344.786765817096\n"
//                    "        fade_power 0\n"
//                    "        parallel\n"
//                    "        point_at <%27, %28, %29>\n"
//                    "}\n"
//                    "#default {\n"
//                    "        finish {ambient .8 diffuse 1 specular %30 roughness %31 metallic 0.001}\n"
//                    "}\n"

//                    "union {\n";

//    header = header.arg(ambientLight[0]).arg(ambientLight[1]).arg(ambientLight[2]);
//    header = header.arg(bc[0]).arg(bc[1]).arg(bc[2]);
//    header = header.arg((lRange+rRange)/2).arg((uRange+dRange)/2).arg((rRange-lRange)*2.5);
//    header = header.arg(26.565).arg(0.0).arg((uRange-dRange)/2).arg(0.0).arg((rRange-lRange)/2);
//    header = header.arg(0.0).arg(0.0).arg((lRange+rRange)/2).arg((uRange+dRange)/2).arg(0.0);
//    header = header.arg(lightPos[0]).arg(lightPos[1]).arg(lightPos[2]);
//    header = header.arg(specular[0]).arg(specular[1]).arg(specular[2]).arg("shadowless");
//    header = header.arg(0.0).arg(0.0).arg(0.0).arg(pov_specular).arg(roughness);

//    QString cylinder = "cylinder {\n"
//                "        <%1, %2, %3>,  <%4, %5, %6>, %7  \n"
//                "        pigment { rgbt <%8, %9, %10, %11> }\n"
//                   "}\n";

//    QString sphere = "sphere {\n"
//                    "       <%1, %2, %3>, %4\n"
//                    "        pigment { rgbt <%5, %6, %7, %8> }\n"
//                    "}\n";

//    QString cylindersalida, spheresalida ;

//    QFile file("temp.pov");
//    if (!file.open(QIODevice::WriteOnly)) {

//        cout<<"Coud not open file temp.pov\n";
//        return;
//        }

//    QTextStream out(&file);
//    out << header;

//    for(unsigned int i=0;i<st->coors.size();i++){
//        for(unsigned int j=0;j<st->bonds[i].neighbors.size();j++){
//            at1 = i;
//            at2 = st->bonds[i].neighbors[j];
//            semix = (-st->coors[at1][0] + -st->coors[at2][0])/2.0;
//            semiy = (st->coors[at1][1] + st->coors[at2][1])/2.0;
//            semiz = (st->coors[at1][2] + st->coors[at2][2])/2.0;
//            //semis = {semix, semiy, semiz};
//            if (semix != -st->coors[at1][0] && semiy != st->coors[at1][1] && semiz != st->coors[at1][2]){
//                    cylindersalida = cylinder.arg(-st->coors[at1][0]).arg(st->coors[at1][1]).arg(st->coors[at1][2]).arg(semix);
//                    cylindersalida = cylindersalida.arg(semiy).arg(semiz).arg(st->bonds[i].radii[j]).arg(st->coor_colors[i][0]);
//                    cylindersalida = cylindersalida.arg(st->coor_colors[i][1]).arg(st->coor_colors[i][2]).arg(1-st->coor_colors[i][3]);
//                    out << cylindersalida;
//                }
//            if (semix != -st->coors[at2][0] && semiy != st->coors[at2][1] && semiz != st->coors[at2][2]){
//                    cylindersalida = cylinder.arg(semix).arg(semiy).arg(semiz);
//                    cylindersalida = cylindersalida.arg( -st->coors[at2][0]).arg(st->coors[at2][1]).arg(st->coors[at2][2]);
//                    cylindersalida = cylindersalida.arg(st->bonds[i].radii[j]).arg(st->coor_colors[at2][0]).arg(st->coor_colors[at2][1]);
//                    cylindersalida = cylindersalida.arg(st->coor_colors[at2][2]).arg(1-st->coor_colors[at2][3]);
//                    out << cylindersalida;
//                }
//        }
//    }

//    for(unsigned int i=0;i<st->coors.size();i++){
//        spheresalida = sphere.arg(-st->coors[i][0]).arg(st->coors[i][1]).arg(st->coors[i][2]).arg(st->coor_spheres[i]);
//        spheresalida = spheresalida.arg(st->coor_colors[i][0]).arg(st->coor_colors[i][1]).arg(st->coor_colors[i][2]).arg(1-st->coor_colors[i][3]);
//        out<<spheresalida;
//    }
//    out << "}\nmerge {\n}" << endl;

//    file.close();

//    cmd = "povray +Itemp.pov +O" + outputfile + " +W2000 +H2000 +V -D +FN +Q9 -P -UD +UL +UV +A +AM2";
//    system(cmd.c_str());

}


int QGLBegin::clicked_on_atom(float x, float y){
    float distx, disty, maxz=-1000;
    int fs=-1;
    Atom atom;

    for(unsigned int i=0;i<st->atoms.size();i++){
        atom = st->atoms[i];
        distx = abs(x - atom.coor[0]);
        disty = abs(y - atom.coor[1]);
        if(distx < atom.sphere && disty < atom.sphere && atom.coor[2] >= maxz){
            fs = i;
            maxz = atom.coor[2];
        }
    }
    return fs;
}

void QGLBegin::clear(){
    st->atoms.clear();
    st->selat.clear();
    st->animation.clear();
    st->freqs.clear();
    st->showcell = false;
    st->beforedata.clear();
    st->afterdata.clear();
    st->reset_mat(st->trotmat);
}

void QGLBegin::playmovie(){
    if(st->peli_pos==st->pelisize-1){
        st->peli_pos = -1;
    }
    timer->start();
}


void QGLBegin::playfreq(unsigned int freq){
    //freq is the currently selected vibrational mode
    st->cfreq = freq;
    st->freq_pos = 5;
    if(freqtimer->isActive()) return;
    freqtimer->start();
}

void QGLBegin::playmovieslot(){
//        m.lock();
        if(!st->shift_peli("f")){
            timer->stop();
//            m.unlock();
            return;
        }
        QString status_msg = QString::number(st->peli_pos) + ", step: ";
        status_msg += QString::number(st->animation[st->peli_pos].step) + ", Time: ";
        status_msg += QString::number(st->animation[st->peli_pos].time) + ", Energy: ";
        status_msg += QString::number(st->animation[st->peli_pos].energy);
        parentStatusBar->showMessage(status_msg);
        update();
//        m.unlock();
}

void QGLBegin::playfreqslot(){
    st->shift_freq_desp();
    update();
}

void QGLBegin::stopmovie(){
    timer->stop();
    freqtimer->stop();
}

void QGLBegin::set_interval(float td){
    time_delay = td;
    timer->setInterval(time_delay);
    freqtimer->setInterval(time_delay);
}


void QGLBegin::save_style(QString stylename){
    QDir dir(settings_folder);
    if(!dir.exists()){
        dir.mkdir(settings_folder);
    }

    QFile outfile(settings_folder + stylename + ".style");

    if(!outfile.open(QIODevice::WriteOnly)){
        QMessageBox::information(0, "error", outfile.errorString());
    }
    QTextStream out(&outfile);

    for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
            out << st->trotmat[i][j] << "  ";
        }
        out << " \n";
    }
        out << lRange << "  " << rRange << "  " << uRange << "  " << dRange << "    \n";
        // save background color
        for(unsigned int j=0;j<4;j++){
            out << bc[j] << "  ";
        }
        out<<"\n";

        QStringList labels;

        for(auto &at:st->atoms){
            if(labels.contains(at.label)) continue;

            out << at.label << "  "<<at.sphere <<"  "<<at.radio<<"  "<<at.cyl<<"  ";
            for(unsigned int j=0;j<4;j++){
                out << at.color[j] << "  ";
            }
            out<<endl;
            labels.append(at.label);
        }
        outfile.close();
}

void QGLBegin::load_style(QString stylename){
    st->set_inv_mat();

    QFile infile(settings_folder+stylename+".style");

    if(!infile.open(QIODevice::ReadOnly)){
        QMessageBox::information(0, "error", infile.errorString());
    }
    QTextStream in(&infile);

    QStringList lsplitted;
    QStringList lines;

    while(!in.atEnd()) {
        lines.append(in.readLine());
    }
    infile.close();

    for(int i=0;i<3;i++){
        lsplitted = lines[i].split(" ", QString::SkipEmptyParts);
        for(unsigned int j=0;j<3;j++){
            st->trotmat[i][j] = lsplitted[j].toFloat();
        }
    }

    //read ranges
    lsplitted = lines[3].split(" ", QString::SkipEmptyParts);
    lRange = lsplitted[0].toFloat(); rRange = lsplitted[1].toFloat();
    uRange = lsplitted[2].toFloat(); dRange = lsplitted[3].toFloat();

    //background color
     lsplitted = lines[4].split(" ", QString::SkipEmptyParts);
    for(unsigned int j=0;j<4;j++){
        bc[j] = lsplitted[j].toFloat();
    }

    for(auto &at:st->atoms){
        for(unsigned int i=5;i<lines.size();i++){
            lsplitted = lines[i].split(" ", QString::SkipEmptyParts);
            if(lsplitted[0].compare(at.label)==0){
                at.sphere = lsplitted[1].toFloat();
                at.radio = lsplitted[2].toFloat();
                at.cyl = lsplitted[3].toFloat();
                for(unsigned int k=0;k<4;k++){
                    at.color[k] = lsplitted[k+4].toFloat();
                }
            }
        }
    }
    st->glob_rotate(st->atoms, st->totinvmat);
    st->glob_rotate(st->cell_lines, st->totinvmat);
    st->glob_rotate(st->atoms, st->trotmat);
    st->glob_rotate(st->cell_lines, st->trotmat);

    st->genbonds();
    update();
    return;

}

void QGLBegin::load_rf(QString inputfile){
    QFile file(inputfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream in(&file);
    array<float, 3> tempcoor;
    QStringList lsplitted;
    QStringList lines;
    Atom atom;
    QString line;
    array<float, 4> tempcolor;
    unsigned int nat;
    Animation anim;
    vector<GROUP> groups;
    vector<Plane> planes;
    vector<Arrow> arrows;

    while(!in.atEnd()) {
        line = in.readLine();
        // reading groups-----------------------------------------------
        if(line.startsWith("$g")){
            GROUP group;
            QStringList strl = line.split(" ", QString::SkipEmptyParts);
            group.name = strl[1];
            for(unsigned int j = 2;j<strl.size();j++){
                group.indices.emplace_back(strl[j].toInt());
            }
            groups.emplace_back(group);
            continue;
        }
        // reading planes -----------------------------------------------
        if(line.startsWith("$plane")){
            Plane plane;
            QStringList strl = line.split(" ", QString::SkipEmptyParts);
            for(unsigned int j = 1;j<strl.size();j++){
                plane.indices[j-1] = strl[j].toInt();
            }
            planes.emplace_back(plane);
            continue;
        }
        // reading arrows -----------------------------------------------
        if(line.startsWith("$arrow")){
            Arrow arrow;
            QStringList strl = line.split(" ", QString::SkipEmptyParts);
            for(unsigned int j = 1;j<strl.size();j++){
                arrow.indices[j-1] = strl[j].toInt();
            }
            arrows.emplace_back(arrow);
            continue;
        }

        lines.append(line);
    }

    file.close();
    // read number of atoms
    nat = lines[0].toInt();

    for(unsigned int i=1;i<nat+1;i++){
        lsplitted = lines[i].split(" ", QString::SkipEmptyParts);
        tempcoor[0] = lsplitted[1].toFloat();
        tempcoor[1] = lsplitted[2].toFloat();
        tempcoor[2] = lsplitted[3].toFloat();
        atom = Atom(tempcoor, lsplitted[0], i-1);
        atom.radio = lsplitted[4].toFloat();
        atom.sphere = lsplitted[5].toFloat();
        atom.cyl = lsplitted[6].toFloat();
        atom.chemical_shift = lsplitted[7].toFloat();
        atom.atom_site_label = lsplitted[8];
        for(unsigned int j=0;j<4;j++){
            tempcolor[j] = lsplitted[j+7].toFloat();
        }
        atom.color = tempcolor;
        atom.peli.push_back(tempcoor);
        atom.pelidisp.push_back({0.0,0.0,0.0});
        st->atoms.push_back(atom);
        for(unsigned int k=0;k<3;k++){
            anim.com[k] += tempcoor[k];
        }
    }
    //setup animation------------------------
    for(unsigned int k=0;k<3;k++){
        anim.com[k] /= st->atoms.size();
    }
    st->animation.emplace_back(anim);
    //setup animation------------------------


    //read rotation matrix
    for(unsigned int i=0;i<3;i++){
        lsplitted = lines[i+1+nat].split(" ", QString::SkipEmptyParts);
        for(unsigned int j=0;j<3;j++){
            st->trotmat[i][j] = lsplitted[j].toFloat();
        }
    }

    //read rangess
    lsplitted = lines[nat+4].split(" ", QString::SkipEmptyParts);
    lRange = lsplitted[0].toFloat(); rRange = lsplitted[1].toFloat();
    uRange = lsplitted[2].toFloat(); dRange = lsplitted[3].toFloat();

    //background color
     lsplitted = lines[nat+5].split(" ", QString::SkipEmptyParts);
    for(unsigned int j=0;j<4;j++){
        bc[j] = lsplitted[j].toFloat();
    }

    //read cell information
    for(unsigned int i=0;i<3;i++){
        lsplitted = lines[i+nat+6].split(" ", QString::SkipEmptyParts);
        for(unsigned int j=0;j<3;j++){
            st->cell[i][j] = lsplitted[j].toFloat();
        }
    }



    st->groups = groups;
    st->planes = planes;
    st->arrows = arrows;
    st->set_cell_lines();
    st->showcell = true;
    st->shift_to_mass_center();
    st->glob_rotate(st->atoms, st->trotmat);
    st->glob_rotate(st->cell_lines, st->trotmat);
    st->genbonds();
    return;
}

void QGLBegin::save_rf(QString filename){
    QFile outfile(filename);
    cout<<"saving rssss!!!"<<endl;

    if(!outfile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "error", outfile.errorString());
    }
    QTextStream out(&outfile);
    int nat;
    nat = st->fatoms.size();
    // saving number of atoms
    out << nat << " \n";
    //saving atoms
    for(auto &at:st->fatoms){
        out << at.label << "     ";
        for(unsigned int j=0;j<3;j++){
            out << at.coor[j] << "     ";
        }
        out << at.radio << "     ";
        out << at.sphere << "     ";
        out << at.cyl << "     ";
        for(unsigned int j=0;j<4;j++){
            out << at.color[j] << "     ";
        }
        out<<at.chemical_shift<<"   ";
        out<<at.atom_site_label<<"   ";
        out << "    \n";
    }

    // sabing rotation matrix
    for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
            out << st->trotmat[i][j] << "  ";
        }
        out << " \n";
    }
    // saving lrange -------------------------------------------------------------
    out << lRange << "  " << rRange << "  " << uRange <<"  "<< dRange <<"    \n";

    // save background color
    for(unsigned int j=0;j<4;j++){
        out << bc[j] << "  ";
    }
    out<<"\n";

    //save cell information
    for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
            out<< QString::number(st->cell[i][j], 'f', 5)<<"   ";
        }
        out<<"\n";
    }
    //saving groups --------------------------------------------------------------
    for(auto g:st->groups){
        out<<"$g ";
        out<<g.name<<"    ";
        for(auto j:g.indices){
            out<<j <<"  ";
        }
        out<<endl;
    }
    //saving planes --------------------------------------------------------------
    for(auto p:st->planes){
        out<<"$plane ";
        for(auto j:p.indices){
            out<<j <<"  ";
        }
        out<<endl;
    }
    //saving arrows --------------------------------------------------------------
    for(auto a:st->arrows){
        out<<"$arrow ";
        for(auto j:a.indices){
            out<<j <<"  ";
        }
        out<<endl;
    }
    outfile.close();
    return;
}

void QGLBegin::center(){
    float minX=10000.0, minY=10000.0, maxX=-10000.0, maxY=-10000.0;

    for(auto &at:st->atoms){
        if(at.coor[0]>maxX){
            maxX = at.coor[0];
        }
        if(at.coor[0]<minX){
            minX = at.coor[0];
        }
        if(at.coor[1]>maxY){
            maxY = at.coor[1];
        }
        if(at.coor[1]<minY){
            minY = at.coor[1];
        }
    }
    float deltay = maxY-minY;
    float deltax = maxX-minX;
    if(minY <= minX){
        if(deltax <= deltay){
            lRange = minY-1;
            dRange = minY-1;
            rRange = maxY+1;
            uRange = maxY+1;
        }else{
            lRange = minY-1;
            dRange = minY-1;
            rRange = minY+deltax+1;
            uRange = minY+deltax+1;
        }
    }
    else{
        if(deltax >= deltay){
            lRange = minX-5;
            dRange = minX-5;
            rRange = maxX+5;
            uRange = maxX+5;
        }else{
            lRange = minX-5;
            dRange = minX-5;
            rRange = minX+deltay+5;
            uRange = minX+deltay+5;
        }
    }
    printf("%f  %f  %f  %f\n", lRange, rRange, dRange, uRange);
    onSize();
    update();
}

void QGLBegin::loadStyleFromState(QString inputfile){
    QFile file(inputfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream in(&file);
    QStringList lsplitted;
    QStringList lines;
    QString line;
    array<float, 4> tempcolor;
    unsigned int nat, natToRead;
    vector<GROUP> groups;
    GROUP group;

    while(!in.atEnd()) {
        line = in.readLine();
        if(line.startsWith("$g")){
            QStringList strl = line.split(" ", QString::SkipEmptyParts);
            group.name = strl[1];
            for(unsigned int j = 2;j<strl.size();j++){
                group.indices.emplace_back(strl[j].toInt());
            }
            groups.emplace_back(group);
            continue;
        }
        lines.append(line);
    }

    file.close();
    // read number of atoms
    nat = lines[0].toInt();
    if(nat <= st->atoms.size()){
        natToRead = nat;
    }else{
        natToRead = st->atoms.size();
    }
    for(unsigned int i=0;i<natToRead;i++){
        lsplitted = lines[i+1].split(" ", QString::SkipEmptyParts);
        st->atoms[i].radio = lsplitted[4].toFloat();
        st->atoms[i].sphere = lsplitted[5].toFloat();
        st->atoms[i].cyl = lsplitted[6].toFloat();
//            atom.chemical_shift = lsplitted[7].toFloat();   esto no porque es solo el estilo lo que voy a cargar
//            atom.atom_site_label = lsplitted[8];
        for(unsigned int j=0;j<4;j++){
            tempcolor[j] = lsplitted[j+7].toFloat();
        }
        st->atoms[i].color = tempcolor;
    }

    //read rotation matrix
    for(unsigned int i=0;i<3;i++){
        lsplitted = lines[i+1+nat].split(" ", QString::SkipEmptyParts);
        for(unsigned int j=0;j<3;j++){
            st->trotmat[i][j] = lsplitted[j].toFloat();
        }
    }

    //read rangess
    lsplitted = lines[nat+4].split(" ", QString::SkipEmptyParts);
    lRange = lsplitted[0].toFloat(); rRange = lsplitted[1].toFloat();
    uRange = lsplitted[2].toFloat(); dRange = lsplitted[3].toFloat();

    //background color
     lsplitted = lines[nat+5].split(" ", QString::SkipEmptyParts);
    for(unsigned int j=0;j<4;j++){
        bc[j] = lsplitted[j].toFloat();
    }


    st->groups = groups;
//    st->set_cell_lines();
    st->showcell = true;
    st->glob_rotate(st->atoms, st->trotmat);
    st->glob_rotate(st->cell_lines, st->trotmat);
    st->genbonds();

    onSize();
    update();
    return;
}

void QGLBegin::prueba(){

}

void QGLBegin::updateParentLabelInfo(){
    if(trackParam){
        if(paramIndicesSize == 2){
            Atom a1 = st->atoms[paramIndices[0]];
            Atom a2 = st->atoms[paramIndices[1]];
            parentLabelInfo.clear();
            float bonddist = dist(a1.coor, a2.coor);
            parentLabelInfo.append(a1.label);
            parentLabelInfo.append(QString::number(a1.index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(a2.label);
            parentLabelInfo.append(QString::number(a2.index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));
        }
        else if(paramIndicesSize == 3){
            Atom a1 = st->atoms[paramIndices[0]];
            Atom a2 = st->atoms[paramIndices[1]];
            Atom a3 = st->atoms[paramIndices[2]];
            parentLabelInfo.clear();
            float bonddist = dist(a1.coor, a2.coor);
            parentLabelInfo.append(a1.label);
            parentLabelInfo.append(QString::number(a1.index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(a2.label);
            parentLabelInfo.append(QString::number(a2.index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));

            parentLabelInfo.append(";  ");
            bonddist = dist(a2.coor, a3.coor);
            parentLabelInfo.append(a2.label);
            parentLabelInfo.append(QString::number(a2.index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(a3.label);
            parentLabelInfo.append(QString::number(a3.index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));
            float atomsAngle = angle(a1.coor, a2.coor, a3.coor);
            parentLabelInfo.append(";  Angle: ");
            parentLabelInfo.append(QString::number(atomsAngle));
        }
        else if(paramIndicesSize == 4){
            Atom a1 = st->atoms[paramIndices[0]];
            Atom a2 = st->atoms[paramIndices[1]];
            Atom a3 = st->atoms[paramIndices[2]];
            Atom a4 = st->atoms[paramIndices[3]];
            parentLabelInfo.clear();
            float bonddist = dist(a1.coor, a2.coor);
            parentLabelInfo.append(a1.label);
            parentLabelInfo.append(QString::number(a1.index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(a2.label);
            parentLabelInfo.append(QString::number(a2.index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));

            parentLabelInfo.append(";  ");
            bonddist = dist(a2.coor, a3.coor);
            parentLabelInfo.append(a2.label);
            parentLabelInfo.append(QString::number(a2.index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(a3.label);
            parentLabelInfo.append(QString::number(a3.index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));
            float atomsAngle = angle(a1.coor, a2.coor, a3.coor);
            parentLabelInfo.append(";  Angle: ");
            parentLabelInfo.append(QString::number(atomsAngle));


            parentLabelInfo.append(";  ");
            bonddist = dist(a3.coor, a4.coor);
            parentLabelInfo.append(a3.label);
            parentLabelInfo.append(QString::number(a3.index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(a4.label);
            parentLabelInfo.append(QString::number(a4.index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));
            atomsAngle = angle(a2.coor, a3.coor, a4.coor);
            parentLabelInfo.append(";  Angle: ");
            parentLabelInfo.append(QString::number(atomsAngle));
            float dihedralAng = dihedral(a1.coor, a2.coor, a3.coor, a4.coor);
            parentLabelInfo.append(";  Diheral: ");
            parentLabelInfo.append(QString::number(dihedralAng));
        }
    }else{
        if(st->selat.size() == 1){
            parentLabelInfo.clear();
            parentLabelInfo.append(st->selat[0]->atom_site_label);
            parentLabelInfo.append(tr(": "));
            parentLabelInfo.append(st->selat[0]->label);
            parentLabelInfo.append(QString::number(st->selat[0]->index+1));
        }
        else if(st->selat.size() == 2){
            parentLabelInfo.clear();
            float bonddist = dist(st->selat[0]->coor, st->selat[1]->coor);
            parentLabelInfo.append(st->selat[0]->label);
            parentLabelInfo.append(QString::number(st->selat[0]->index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(st->selat[1]->label);
            parentLabelInfo.append(QString::number(st->selat[1]->index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));
        }
        else if(st->selat.size() == 3){
            parentLabelInfo.append(";  ");
            float bonddist = dist(st->selat[1]->coor, st->selat[2]->coor);
            parentLabelInfo.append(st->selat[1]->label);
            parentLabelInfo.append(QString::number(st->selat[1]->index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(st->selat[2]->label);
            parentLabelInfo.append(QString::number(st->selat[2]->index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));
            float atoms_angle = angle(st->selat[0]->coor, st->selat[1]->coor, st->selat[2]->coor);
    //                float atoms_angle = angle(array<float,3>{1,1,1},array<float,3>{0,0,0}, array<float,3>{2,2,2});
            parentLabelInfo.append(";  Angle: ");
            parentLabelInfo.append(QString::number(atoms_angle));
        }
        else if(st->selat.size() == 4){
            parentLabelInfo.append(";  ");
            float bonddist = dist(st->selat[2]->coor, st->selat[3]->coor);
            parentLabelInfo.append(st->selat[2]->label);
            parentLabelInfo.append(QString::number(st->selat[2]->index+1));
            parentLabelInfo.append("-");
            parentLabelInfo.append(st->selat[3]->label);
            parentLabelInfo.append(QString::number(st->selat[3]->index+1));
            parentLabelInfo.append(": ");
            parentLabelInfo.append(QString::number(bonddist));
            float atomsAngle = angle(st->selat[0]->coor, st->selat[1]->coor, st->selat[2]->coor);
    //                float atoms_angle = angle(array<float,3>{1,1,1},array<float,3>{0,0,0}, array<float,3>{2,2,2});
            parentLabelInfo.append(";  Angle: ");
            parentLabelInfo.append(QString::number(atomsAngle));
            float dihedralAng = dihedral(st->selat[0]->coor, st->selat[1]->coor, st->selat[2]->coor, st->selat[3]->coor);
            parentLabelInfo.append(";  Diheral: ");
            parentLabelInfo.append(QString::number(dihedralAng));
        }
    }

    parentStatusBar->showMessage(parentLabelInfo);
}

void QGLBegin::setTrackParam(bool arg){
    for(unsigned int i=0;i<4;i++){
        paramIndices[i] = -1;
    }

    if(arg){
        paramIndicesSize = 0;
        for(auto sel:st->selat){
            paramIndices[paramIndicesSize] = sel->index;
            paramIndicesSize ++;
            if (paramIndicesSize == 4){
                break;
            }
        }
        trackParam = true;
    }
    else{
        trackParam = false;
    }
}
