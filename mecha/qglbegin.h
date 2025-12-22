#ifndef QGLBEGIN_H
#define QGLBEGIN_H

// www.amin-ahmadi.com

//#include <GL/glew.h>
#include <QtWidgets>
#include <GL/glu.h>
//#include <GL/glut.h>
#include "struct.h"
#include <mutex>

class QGLBegin : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit QGLBegin(QWidget *parent = 0, QStatusBar *sb=0);
    QStatusBar *parentStatusBar;
    Struct *st;
    bool editmode;
    bool editLightMode = false;
    bool moveAlwaysInsideCellMode = false;
    bool scatterplot_mode = false;
    bool scatterplot_mode_2d = false;
    int glwidth, glheight; // global width and global height
    int cropX1, cropX2, cropY1, cropY2;
    float selrectx, selrecty;
    int quality, oldquality;
    int qualityBond;
    int shininess;
    float mouseX, mouseY, mouseYbef, mouseXbef;
    float xrot, yrot, zrot, rot_ang;
    float dx, dy, scale;
    float nRange;
    float rRange, lRange, uRange, dRange;
    float rRangeVP, lRangeVP, uRangeVP, dRangeVP;
    float ambientLight[4] = {0.35, 0.35, 0.35, 1};
    float diffuseLight[4] = {0.75, 0.75, 0.75, 0.7};
//    float specular[4]  = {0.0, 0.0, 0.0, 0.0};
    float specular[4]  = {1.0, 1.0, 1.0, 1.0};
    float specref[4] = {1.0, 1.0, 1.0, 1.0};
//    float specref[4] = {0.8, 0.8, 0.8, 1.0};
    float lightPos[4] = {-50.0, 50.0, 100.0, 1.0};
//    float lightPos[4] = {0.0, 0.0, 50.0, 1.0};
    float bc[4] = {0.1, 0.1, 0.1, 0.0};
    float time_delay = 1;
    bool light_enabled, material_enabled;
    bool lclick, rclick, nobondmode;
    bool paint_sel_rectangle;
    bool draging;
    bool moving;
    bool trackParam = false;
    bool cropMode = false, nowCropping = false;
    QString parentCombotxt = "";
    QString parentLabelInfo = "";
    QTimer *timer;
    QTimer *freqtimer;
    static mutex m;
    vector <array<float, 3>> potential_dragged_coors;
    array<int, 4> paramIndices;
    unsigned int paramIndicesSize=0;

    void setRotation(float x, float y, float z);
    void enableGlobProp();
    void draw();
    void scatterplot();
    void onSize();
    void renderCone(std::array<float,3> coor1, std::array<float,3> coor2, float radio, float radio2, int subdiv, std::array<float,4> color);
    void renderBonds(std::array<float,3> coor1, std::array<float,3> coor2, float radio, int subdiv, std::array<float,4> color);
    void renderpovray(string outputfile);
    int clicked_on_atom(float x, float y);
    void save_style(QString stylename);
    void load_style(QString stylename);
    void loadStyleFromState(QString stylename);
    void clear();
    void update2();
    void updateParentLabelInfo();
    void playmovie();
    void stopmovie();
    void playfreq(unsigned int freq = 0);
    void set_interval(float td);
    void load_rf(QString inputfile);
    void save_rf(QString filename);
    void setTrackParam(bool arg);
    void center();
    void prueba();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
//    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
   void atoms_selected();
   void atoms_deselected();
   void comboRequest();
   void data_changed();

public slots:
   void playmovieslot();
   void playfreqslot();

private:

};



#endif // QGLBEGIN_H

