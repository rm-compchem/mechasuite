#include "mainwindow.h"
#include <QApplication>
//#include "globals.h"
#include <QSurfaceFormat>
#include <QOpenGLContext>


int main(int argc, char *argv[])
{
    set_init_properties();
    QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::NoProfile);
    format.setVersion(1,1);
    format.setRenderableType(QSurfaceFormat::OpenGL);

    QPixmap pixmap(":/icons/logo.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();

    #ifdef defined (Q_OS_WIN32)
        QApplication::setStyle("fusion");
    #endif

//    char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
    MainWindow *w = new MainWindow();
    w->setWindowIcon(QIcon(":/icons/logo.png"));
    w->showMaximized();
    splash.finish(w);
    w->parseArguments(argc, argv);
    //glutInit(&argc, argv);
    return a.exec();
}
