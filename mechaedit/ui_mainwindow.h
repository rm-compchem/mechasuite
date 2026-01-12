/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <pelitable.h>
#include "freqtable.h"
#include "table.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionRender_PovRay;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionInvert_Selection;
    QAction *actionFix_Selected;
    QAction *actionSave;
    QAction *actionImport_PwSCF_settings;
    QAction *actionClose;
    QAction *actionSet_Unit_Cell;
    QAction *actionSave_as;
    QAction *actionunfix_selected;
    QAction *actionSelect_All;
    QAction *actionSelect_by_bonds;
    QAction *actionplay;
    QAction *actionsticks;
    QAction *actioncustom;
    QAction *actionconnectivity;
    QAction *actionundo;
    QAction *actionredo;
    QAction *actiontranslation_vector;
    QAction *actionto_x_value;
    QAction *actionto_y_value;
    QAction *actionto_z_value;
    QAction *actionsphere_radio;
    QAction *actionfrequencies;
    QAction *actionstop;
    QAction *actionplay_frequencies;
    QAction *actioncustom_selection;
    QAction *actioninside_Cell;
    QAction *actioncenter;
    QAction *actionNew;
    QAction *actionalldefaults;
    QAction *actioncolors;
    QAction *actionwire;
    QAction *actionhide;
    QAction *actionshow_all;
    QAction *actionBackground_color;
    QAction *actioncoordinates;
    QAction *actionbond_distances_per_type;
    QAction *actionbond;
    QAction *actionangle;
    QAction *actiondihedral;
    QAction *actiontime_delay;
    QAction *actionpeli_png;
    QAction *actionaverage_animation;
    QAction *actionexport_CV;
    QAction *actionSelect_fixed;
    QAction *actionsave_current_style;
    QAction *actionload_style;
    QAction *actionx_y_and_z;
    QAction *actionoriginal;
    QAction *actionradius;
    QAction *actionsphere_size;
    QAction *actionbonded_and_inside_a_sphre;
    QAction *actionInsideSphereAny;
    QAction *actionInsideSphereOnly_Bonded;
    QAction *actionInfo;
    QAction *actionFilterByDistance;
    QAction *actionSite_Type;
    QAction *actionColor;
    QAction *actionsphere;
    QAction *actionDistances_per_type;
    QAction *actiondeselect;
    QAction *actionUnreplicate;
    QAction *actionRestoreSphere;
    QAction *actionRestoreRadio;
    QAction *actionRestoreBond_Radio;
    QAction *actionInsertFragment;
    QAction *actionSelectByElement;
    QAction *actionOpenToolBar;
    QAction *actionSaveToolBar;
    QAction *actionEditToolBar;
    QAction *actionShow_Cell;
    QAction *actionSpecular;
    QAction *actionEnable_disable_Light;
    QAction *actionSpecularToolBar;
    QAction *actionChange_to_element;
    QAction *actionAtoms_Type;
    QAction *actionTrack_Coors;
    QAction *actionGroups;
    QAction *actionFrom_Selected;
    QAction *actionSave_asToolBar;
    QAction *actionFreqs_Animation_gif;
    QAction *actionAlign_to_Bond;
    QAction *actionImage;
    QAction *sectionImageToolBar;
    QAction *actionFrom_Same_Sigma;
    QAction *actionnmr;
    QAction *actionLoad_Style_all_tabs;
    QAction *actionas_png;
    QAction *actionAround_Z_axis;
    QAction *actionAround_Bond;
    QAction *actionStructure_from_Averages;
    QAction *actionCorrect_periodic_trajectory;
    QAction *actionrings_of_size;
    QAction *actionLoad_Style_from_state_rs;
    QAction *actionCenter_of_Mass_selected;
    QAction *actionSet_view_range;
    QAction *actionY_coor_COM;
    QAction *actionAtom_with_n_bond;
    QAction *action3d;
    QAction *actiondots;
    QAction *actionExport_Plumed_File;
    QAction *actionadd_plane;
    QAction *actionplane_by_index;
    QAction *actionadd_arrow;
    QAction *actionEliminate_Repeated_Atoms;
    QAction *actionto_position;
    QAction *actionIndex;
    QAction *actionmake_supercell;
    QAction *actionDownload_from_IZA_DB;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTabWidget *tabWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_5;
    QSlider *atomradioSlider;
    QSlider *bondcylinderSlider;
    QSlider *shininessSlider;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *comboBox;
    QSlider *sphereSlider;
    QPushButton *colorpushButton;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_7;
    QSlider *qualitySlider;
    QSlider *rotationSlider;
    QSlider *opacitySlider;
    QSplitter *splitter_2;
    Table *tableWidget;
    PeliTable *peli_listWidget;
    FreqTable *freqtable;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuSave_all;
    QMenu *menuEdit;
    QMenu *menuDelete;
    QMenu *menuSelection;
    QMenu *menuselect_replicated_copy;
    QMenu *menuS_elect_inside_Sphere;
    QMenu *menuSelect_by;
    QMenu *menuInvert;
    QMenu *menuobject;
    QMenu *menuTool;
    QMenu *menureplicate;
    QMenu *menu_Translate;
    QMenu *menugeom_param;
    QMenu *menuCreate_group;
    QMenu *menuRotate;
    QMenu *menuObject;
    QMenu *menuCrystalography;
    QMenu *menuAnimation;
    QMenu *menutrack_CV;
    QMenu *menuexport;
    QMenu *menufilter;
    QMenu *menuScatte_rplot;
    QMenu *menuview;
    QMenu *menudefaults;
    QMenu *menuatom;
    QMenu *menustyles;
    QMenu *menuLight;
    QMenu *menuModels;
    QMenu *menuInsert;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(700, 500);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setAcceptDrops(true);
        MainWindow->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionOpen->setProperty("shortcutVisibleInContextMenu", QVariant(false));
        actionRender_PovRay = new QAction(MainWindow);
        actionRender_PovRay->setObjectName(QString::fromUtf8("actionRender_PovRay"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionInvert_Selection = new QAction(MainWindow);
        actionInvert_Selection->setObjectName(QString::fromUtf8("actionInvert_Selection"));
        actionFix_Selected = new QAction(MainWindow);
        actionFix_Selected->setObjectName(QString::fromUtf8("actionFix_Selected"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionImport_PwSCF_settings = new QAction(MainWindow);
        actionImport_PwSCF_settings->setObjectName(QString::fromUtf8("actionImport_PwSCF_settings"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionSet_Unit_Cell = new QAction(MainWindow);
        actionSet_Unit_Cell->setObjectName(QString::fromUtf8("actionSet_Unit_Cell"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/showcell.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSet_Unit_Cell->setIcon(icon2);
        QFont font;
        font.setFamily(QString::fromUtf8("Noto Sans Adlam"));
        actionSet_Unit_Cell->setFont(font);
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionSave_as->setProperty("shortcutVisibleInContextMenu", QVariant(true));
        actionunfix_selected = new QAction(MainWindow);
        actionunfix_selected->setObjectName(QString::fromUtf8("actionunfix_selected"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionSelect_by_bonds = new QAction(MainWindow);
        actionSelect_by_bonds->setObjectName(QString::fromUtf8("actionSelect_by_bonds"));
        actionplay = new QAction(MainWindow);
        actionplay->setObjectName(QString::fromUtf8("actionplay"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionplay->setIcon(icon3);
        actionsticks = new QAction(MainWindow);
        actionsticks->setObjectName(QString::fromUtf8("actionsticks"));
        actioncustom = new QAction(MainWindow);
        actioncustom->setObjectName(QString::fromUtf8("actioncustom"));
        actionconnectivity = new QAction(MainWindow);
        actionconnectivity->setObjectName(QString::fromUtf8("actionconnectivity"));
        actionundo = new QAction(MainWindow);
        actionundo->setObjectName(QString::fromUtf8("actionundo"));
        actionredo = new QAction(MainWindow);
        actionredo->setObjectName(QString::fromUtf8("actionredo"));
        actiontranslation_vector = new QAction(MainWindow);
        actiontranslation_vector->setObjectName(QString::fromUtf8("actiontranslation_vector"));
        actionto_x_value = new QAction(MainWindow);
        actionto_x_value->setObjectName(QString::fromUtf8("actionto_x_value"));
        actionto_y_value = new QAction(MainWindow);
        actionto_y_value->setObjectName(QString::fromUtf8("actionto_y_value"));
        actionto_z_value = new QAction(MainWindow);
        actionto_z_value->setObjectName(QString::fromUtf8("actionto_z_value"));
        actionsphere_radio = new QAction(MainWindow);
        actionsphere_radio->setObjectName(QString::fromUtf8("actionsphere_radio"));
        actionfrequencies = new QAction(MainWindow);
        actionfrequencies->setObjectName(QString::fromUtf8("actionfrequencies"));
        actionfrequencies->setCheckable(true);
        actionfrequencies->setChecked(false);
        actionstop = new QAction(MainWindow);
        actionstop->setObjectName(QString::fromUtf8("actionstop"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionstop->setIcon(icon4);
        actionplay_frequencies = new QAction(MainWindow);
        actionplay_frequencies->setObjectName(QString::fromUtf8("actionplay_frequencies"));
        actionplay_frequencies->setIcon(icon3);
        actioncustom_selection = new QAction(MainWindow);
        actioncustom_selection->setObjectName(QString::fromUtf8("actioncustom_selection"));
        actioninside_Cell = new QAction(MainWindow);
        actioninside_Cell->setObjectName(QString::fromUtf8("actioninside_Cell"));
        actioncenter = new QAction(MainWindow);
        actioncenter->setObjectName(QString::fromUtf8("actioncenter"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionalldefaults = new QAction(MainWindow);
        actionalldefaults->setObjectName(QString::fromUtf8("actionalldefaults"));
        actioncolors = new QAction(MainWindow);
        actioncolors->setObjectName(QString::fromUtf8("actioncolors"));
        actionwire = new QAction(MainWindow);
        actionwire->setObjectName(QString::fromUtf8("actionwire"));
        actionhide = new QAction(MainWindow);
        actionhide->setObjectName(QString::fromUtf8("actionhide"));
        actionshow_all = new QAction(MainWindow);
        actionshow_all->setObjectName(QString::fromUtf8("actionshow_all"));
        actionBackground_color = new QAction(MainWindow);
        actionBackground_color->setObjectName(QString::fromUtf8("actionBackground_color"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/color.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBackground_color->setIcon(icon5);
        actioncoordinates = new QAction(MainWindow);
        actioncoordinates->setObjectName(QString::fromUtf8("actioncoordinates"));
        actioncoordinates->setCheckable(true);
        actioncoordinates->setChecked(true);
        QFont font1;
        font1.setItalic(true);
        actioncoordinates->setFont(font1);
        actionbond_distances_per_type = new QAction(MainWindow);
        actionbond_distances_per_type->setObjectName(QString::fromUtf8("actionbond_distances_per_type"));
        actionbond = new QAction(MainWindow);
        actionbond->setObjectName(QString::fromUtf8("actionbond"));
        actionangle = new QAction(MainWindow);
        actionangle->setObjectName(QString::fromUtf8("actionangle"));
        actiondihedral = new QAction(MainWindow);
        actiondihedral->setObjectName(QString::fromUtf8("actiondihedral"));
        actiontime_delay = new QAction(MainWindow);
        actiontime_delay->setObjectName(QString::fromUtf8("actiontime_delay"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/time.png"), QSize(), QIcon::Normal, QIcon::Off);
        actiontime_delay->setIcon(icon6);
        actionpeli_png = new QAction(MainWindow);
        actionpeli_png->setObjectName(QString::fromUtf8("actionpeli_png"));
        actionaverage_animation = new QAction(MainWindow);
        actionaverage_animation->setObjectName(QString::fromUtf8("actionaverage_animation"));
        actionexport_CV = new QAction(MainWindow);
        actionexport_CV->setObjectName(QString::fromUtf8("actionexport_CV"));
        actionSelect_fixed = new QAction(MainWindow);
        actionSelect_fixed->setObjectName(QString::fromUtf8("actionSelect_fixed"));
        actionsave_current_style = new QAction(MainWindow);
        actionsave_current_style->setObjectName(QString::fromUtf8("actionsave_current_style"));
        actionload_style = new QAction(MainWindow);
        actionload_style->setObjectName(QString::fromUtf8("actionload_style"));
        actionx_y_and_z = new QAction(MainWindow);
        actionx_y_and_z->setObjectName(QString::fromUtf8("actionx_y_and_z"));
        actionoriginal = new QAction(MainWindow);
        actionoriginal->setObjectName(QString::fromUtf8("actionoriginal"));
        actionradius = new QAction(MainWindow);
        actionradius->setObjectName(QString::fromUtf8("actionradius"));
        actionsphere_size = new QAction(MainWindow);
        actionsphere_size->setObjectName(QString::fromUtf8("actionsphere_size"));
        actionbonded_and_inside_a_sphre = new QAction(MainWindow);
        actionbonded_and_inside_a_sphre->setObjectName(QString::fromUtf8("actionbonded_and_inside_a_sphre"));
        actionInsideSphereAny = new QAction(MainWindow);
        actionInsideSphereAny->setObjectName(QString::fromUtf8("actionInsideSphereAny"));
        actionInsideSphereOnly_Bonded = new QAction(MainWindow);
        actionInsideSphereOnly_Bonded->setObjectName(QString::fromUtf8("actionInsideSphereOnly_Bonded"));
        actionInfo = new QAction(MainWindow);
        actionInfo->setObjectName(QString::fromUtf8("actionInfo"));
        actionFilterByDistance = new QAction(MainWindow);
        actionFilterByDistance->setObjectName(QString::fromUtf8("actionFilterByDistance"));
        actionSite_Type = new QAction(MainWindow);
        actionSite_Type->setObjectName(QString::fromUtf8("actionSite_Type"));
        actionColor = new QAction(MainWindow);
        actionColor->setObjectName(QString::fromUtf8("actionColor"));
        actionsphere = new QAction(MainWindow);
        actionsphere->setObjectName(QString::fromUtf8("actionsphere"));
        actionDistances_per_type = new QAction(MainWindow);
        actionDistances_per_type->setObjectName(QString::fromUtf8("actionDistances_per_type"));
        actiondeselect = new QAction(MainWindow);
        actiondeselect->setObjectName(QString::fromUtf8("actiondeselect"));
        actionUnreplicate = new QAction(MainWindow);
        actionUnreplicate->setObjectName(QString::fromUtf8("actionUnreplicate"));
        actionRestoreSphere = new QAction(MainWindow);
        actionRestoreSphere->setObjectName(QString::fromUtf8("actionRestoreSphere"));
        actionRestoreRadio = new QAction(MainWindow);
        actionRestoreRadio->setObjectName(QString::fromUtf8("actionRestoreRadio"));
        actionRestoreBond_Radio = new QAction(MainWindow);
        actionRestoreBond_Radio->setObjectName(QString::fromUtf8("actionRestoreBond_Radio"));
        actionInsertFragment = new QAction(MainWindow);
        actionInsertFragment->setObjectName(QString::fromUtf8("actionInsertFragment"));
        actionInsertFragment->setProperty("shortcutVisibleInContextMenu", QVariant(true));
        actionSelectByElement = new QAction(MainWindow);
        actionSelectByElement->setObjectName(QString::fromUtf8("actionSelectByElement"));
        actionSelectByElement->setProperty("shortcutVisibleInContextMenu", QVariant(true));
        actionOpenToolBar = new QAction(MainWindow);
        actionOpenToolBar->setObjectName(QString::fromUtf8("actionOpenToolBar"));
        actionOpenToolBar->setCheckable(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("icons/open.png"), QSize(), QIcon::Normal, QIcon::On);
        icon7.addFile(QString::fromUtf8(":/icons/open.png"), QSize(), QIcon::Disabled, QIcon::Off);
        actionOpenToolBar->setIcon(icon7);
        actionSaveToolBar = new QAction(MainWindow);
        actionSaveToolBar->setObjectName(QString::fromUtf8("actionSaveToolBar"));
        actionSaveToolBar->setIcon(icon1);
        actionEditToolBar = new QAction(MainWindow);
        actionEditToolBar->setObjectName(QString::fromUtf8("actionEditToolBar"));
        actionEditToolBar->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/edit.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        icon8.addFile(QString::fromUtf8("icons/edit.jpg"), QSize(), QIcon::Normal, QIcon::On);
        actionEditToolBar->setIcon(icon8);
        actionShow_Cell = new QAction(MainWindow);
        actionShow_Cell->setObjectName(QString::fromUtf8("actionShow_Cell"));
        actionShow_Cell->setCheckable(true);
        actionShow_Cell->setIcon(icon2);
        actionSpecular = new QAction(MainWindow);
        actionSpecular->setObjectName(QString::fromUtf8("actionSpecular"));
        actionSpecular->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/specular.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSpecular->setIcon(icon9);
        actionEnable_disable_Light = new QAction(MainWindow);
        actionEnable_disable_Light->setObjectName(QString::fromUtf8("actionEnable_disable_Light"));
        actionEnable_disable_Light->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/light.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEnable_disable_Light->setIcon(icon10);
        actionSpecularToolBar = new QAction(MainWindow);
        actionSpecularToolBar->setObjectName(QString::fromUtf8("actionSpecularToolBar"));
        actionSpecularToolBar->setCheckable(true);
        actionSpecularToolBar->setIcon(icon9);
        actionChange_to_element = new QAction(MainWindow);
        actionChange_to_element->setObjectName(QString::fromUtf8("actionChange_to_element"));
        actionAtoms_Type = new QAction(MainWindow);
        actionAtoms_Type->setObjectName(QString::fromUtf8("actionAtoms_Type"));
        actionTrack_Coors = new QAction(MainWindow);
        actionTrack_Coors->setObjectName(QString::fromUtf8("actionTrack_Coors"));
        actionTrack_Coors->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/track.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTrack_Coors->setIcon(icon11);
        actionGroups = new QAction(MainWindow);
        actionGroups->setObjectName(QString::fromUtf8("actionGroups"));
        actionFrom_Selected = new QAction(MainWindow);
        actionFrom_Selected->setObjectName(QString::fromUtf8("actionFrom_Selected"));
        actionSave_asToolBar = new QAction(MainWindow);
        actionSave_asToolBar->setObjectName(QString::fromUtf8("actionSave_asToolBar"));
        actionSave_asToolBar->setIcon(icon1);
        actionFreqs_Animation_gif = new QAction(MainWindow);
        actionFreqs_Animation_gif->setObjectName(QString::fromUtf8("actionFreqs_Animation_gif"));
        actionAlign_to_Bond = new QAction(MainWindow);
        actionAlign_to_Bond->setObjectName(QString::fromUtf8("actionAlign_to_Bond"));
        actionImage = new QAction(MainWindow);
        actionImage->setObjectName(QString::fromUtf8("actionImage"));
        actionImage->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/crop.png"), QSize(), QIcon::Normal, QIcon::On);
        actionImage->setIcon(icon12);
        sectionImageToolBar = new QAction(MainWindow);
        sectionImageToolBar->setObjectName(QString::fromUtf8("sectionImageToolBar"));
        sectionImageToolBar->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/crop.png"), QSize(), QIcon::Normal, QIcon::Off);
        sectionImageToolBar->setIcon(icon13);
        actionFrom_Same_Sigma = new QAction(MainWindow);
        actionFrom_Same_Sigma->setObjectName(QString::fromUtf8("actionFrom_Same_Sigma"));
        actionnmr = new QAction(MainWindow);
        actionnmr->setObjectName(QString::fromUtf8("actionnmr"));
        actionLoad_Style_all_tabs = new QAction(MainWindow);
        actionLoad_Style_all_tabs->setObjectName(QString::fromUtf8("actionLoad_Style_all_tabs"));
        actionas_png = new QAction(MainWindow);
        actionas_png->setObjectName(QString::fromUtf8("actionas_png"));
        actionAround_Z_axis = new QAction(MainWindow);
        actionAround_Z_axis->setObjectName(QString::fromUtf8("actionAround_Z_axis"));
        actionAround_Bond = new QAction(MainWindow);
        actionAround_Bond->setObjectName(QString::fromUtf8("actionAround_Bond"));
        actionStructure_from_Averages = new QAction(MainWindow);
        actionStructure_from_Averages->setObjectName(QString::fromUtf8("actionStructure_from_Averages"));
        actionCorrect_periodic_trajectory = new QAction(MainWindow);
        actionCorrect_periodic_trajectory->setObjectName(QString::fromUtf8("actionCorrect_periodic_trajectory"));
        actionrings_of_size = new QAction(MainWindow);
        actionrings_of_size->setObjectName(QString::fromUtf8("actionrings_of_size"));
        actionLoad_Style_from_state_rs = new QAction(MainWindow);
        actionLoad_Style_from_state_rs->setObjectName(QString::fromUtf8("actionLoad_Style_from_state_rs"));
        actionCenter_of_Mass_selected = new QAction(MainWindow);
        actionCenter_of_Mass_selected->setObjectName(QString::fromUtf8("actionCenter_of_Mass_selected"));
        actionSet_view_range = new QAction(MainWindow);
        actionSet_view_range->setObjectName(QString::fromUtf8("actionSet_view_range"));
        actionY_coor_COM = new QAction(MainWindow);
        actionY_coor_COM->setObjectName(QString::fromUtf8("actionY_coor_COM"));
        actionAtom_with_n_bond = new QAction(MainWindow);
        actionAtom_with_n_bond->setObjectName(QString::fromUtf8("actionAtom_with_n_bond"));
        action3d = new QAction(MainWindow);
        action3d->setObjectName(QString::fromUtf8("action3d"));
        action3d->setCheckable(true);
        actiondots = new QAction(MainWindow);
        actiondots->setObjectName(QString::fromUtf8("actiondots"));
        actiondots->setCheckable(true);
        actionExport_Plumed_File = new QAction(MainWindow);
        actionExport_Plumed_File->setObjectName(QString::fromUtf8("actionExport_Plumed_File"));
        actionadd_plane = new QAction(MainWindow);
        actionadd_plane->setObjectName(QString::fromUtf8("actionadd_plane"));
        actionplane_by_index = new QAction(MainWindow);
        actionplane_by_index->setObjectName(QString::fromUtf8("actionplane_by_index"));
        actionadd_arrow = new QAction(MainWindow);
        actionadd_arrow->setObjectName(QString::fromUtf8("actionadd_arrow"));
        actionEliminate_Repeated_Atoms = new QAction(MainWindow);
        actionEliminate_Repeated_Atoms->setObjectName(QString::fromUtf8("actionEliminate_Repeated_Atoms"));
        actionto_position = new QAction(MainWindow);
        actionto_position->setObjectName(QString::fromUtf8("actionto_position"));
        actionIndex = new QAction(MainWindow);
        actionIndex->setObjectName(QString::fromUtf8("actionIndex"));
        actionmake_supercell = new QAction(MainWindow);
        actionmake_supercell->setObjectName(QString::fromUtf8("actionmake_supercell"));
        actionDownload_from_IZA_DB = new QAction(MainWindow);
        actionDownload_from_IZA_DB->setObjectName(QString::fromUtf8("actionDownload_from_IZA_DB"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(3);
        sizePolicy1.setVerticalStretch(3);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy2);
        splitter->setLineWidth(2);
        splitter->setMidLineWidth(2);
        splitter->setOrientation(Qt::Horizontal);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(0, 0));
        tabWidget->setMouseTracking(true);
        splitter->addWidget(tabWidget);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setSizeConstraint(QLayout::SetFixedSize);
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy3);
        label_3->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(label_3);

        label_4 = new QLabel(verticalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy3.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy3);
        label_4->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(label_4);

        label_9 = new QLabel(verticalLayoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy3.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy3);
        label_9->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(label_9);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setSizeConstraint(QLayout::SetFixedSize);
        atomradioSlider = new QSlider(verticalLayoutWidget);
        atomradioSlider->setObjectName(QString::fromUtf8("atomradioSlider"));
        sizePolicy3.setHeightForWidth(atomradioSlider->sizePolicy().hasHeightForWidth());
        atomradioSlider->setSizePolicy(sizePolicy3);
        atomradioSlider->setMaximum(500);
        atomradioSlider->setSingleStep(10);
        atomradioSlider->setPageStep(100);
        atomradioSlider->setOrientation(Qt::Horizontal);
        atomradioSlider->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_5->addWidget(atomradioSlider);

        bondcylinderSlider = new QSlider(verticalLayoutWidget);
        bondcylinderSlider->setObjectName(QString::fromUtf8("bondcylinderSlider"));
        sizePolicy3.setHeightForWidth(bondcylinderSlider->sizePolicy().hasHeightForWidth());
        bondcylinderSlider->setSizePolicy(sizePolicy3);
        bondcylinderSlider->setOrientation(Qt::Horizontal);
        bondcylinderSlider->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_5->addWidget(bondcylinderSlider);

        shininessSlider = new QSlider(verticalLayoutWidget);
        shininessSlider->setObjectName(QString::fromUtf8("shininessSlider"));
        sizePolicy3.setHeightForWidth(shininessSlider->sizePolicy().hasHeightForWidth());
        shininessSlider->setSizePolicy(sizePolicy3);
        shininessSlider->setMaximum(128);
        shininessSlider->setSliderPosition(100);
        shininessSlider->setOrientation(Qt::Horizontal);
        shininessSlider->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_5->addWidget(shininessSlider);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy3.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy3);
        label->setMaximumSize(QSize(100, 30));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy3.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy3);
        label_2->setMaximumSize(QSize(100, 30));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_2);

        label_5 = new QLabel(verticalLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy3.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy3);
        label_5->setMaximumSize(QSize(100, 30));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(253, 254, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(254, 254, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(126, 127, 127, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(169, 169, 170, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        label_5->setPalette(palette);

        horizontalLayout_2->addWidget(label_5);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        comboBox = new QComboBox(verticalLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEnabled(true);
        sizePolicy3.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy3);
        comboBox->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_3->addWidget(comboBox);

        sphereSlider = new QSlider(verticalLayoutWidget);
        sphereSlider->setObjectName(QString::fromUtf8("sphereSlider"));
        sizePolicy3.setHeightForWidth(sphereSlider->sizePolicy().hasHeightForWidth());
        sphereSlider->setSizePolicy(sizePolicy3);
        sphereSlider->setMaximumSize(QSize(16777215, 16777215));
        sphereSlider->setMaximum(200);
        sphereSlider->setOrientation(Qt::Horizontal);
        sphereSlider->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_3->addWidget(sphereSlider);

        colorpushButton = new QPushButton(verticalLayoutWidget);
        colorpushButton->setObjectName(QString::fromUtf8("colorpushButton"));
        sizePolicy.setHeightForWidth(colorpushButton->sizePolicy().hasHeightForWidth());
        colorpushButton->setSizePolicy(sizePolicy);
        colorpushButton->setMinimumSize(QSize(20, 30));
        colorpushButton->setMaximumSize(QSize(30, 30));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush7(QColor(251, 255, 255, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush8(QColor(253, 255, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        QBrush brush9(QColor(125, 127, 127, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush9);
        QBrush brush10(QColor(167, 170, 170, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        colorpushButton->setPalette(palette1);
        colorpushButton->setIcon(icon5);

        horizontalLayout_3->addWidget(colorpushButton);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetFixedSize);
        label_6 = new QLabel(verticalLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy3.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy3);
        label_6->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(label_6);

        label_7 = new QLabel(verticalLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy3.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy3);
        label_7->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(label_7);

        label_8 = new QLabel(verticalLayoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy3.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy3);
        label_8->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(label_8);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setSizeConstraint(QLayout::SetFixedSize);
        qualitySlider = new QSlider(verticalLayoutWidget);
        qualitySlider->setObjectName(QString::fromUtf8("qualitySlider"));
        sizePolicy3.setHeightForWidth(qualitySlider->sizePolicy().hasHeightForWidth());
        qualitySlider->setSizePolicy(sizePolicy3);
        qualitySlider->setMaximumSize(QSize(16777215, 16777215));
        qualitySlider->setMaximum(50);
        qualitySlider->setOrientation(Qt::Horizontal);
        qualitySlider->setTickPosition(QSlider::TicksBelow);
        qualitySlider->setTickInterval(0);

        horizontalLayout_7->addWidget(qualitySlider);

        rotationSlider = new QSlider(verticalLayoutWidget);
        rotationSlider->setObjectName(QString::fromUtf8("rotationSlider"));
        sizePolicy3.setHeightForWidth(rotationSlider->sizePolicy().hasHeightForWidth());
        rotationSlider->setSizePolicy(sizePolicy3);
        rotationSlider->setMaximumSize(QSize(16777215, 16777215));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush11(QColor(11, 84, 241, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush11);
        QBrush brush12(QColor(118, 162, 255, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush12);
        QBrush brush13(QColor(64, 123, 248, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        QBrush brush14(QColor(5, 42, 120, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush14);
        QBrush brush15(QColor(7, 56, 161, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush16(QColor(133, 169, 248, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        rotationSlider->setPalette(palette2);
        rotationSlider->setMaximum(50);
        rotationSlider->setValue(8);
        rotationSlider->setOrientation(Qt::Horizontal);
        rotationSlider->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_7->addWidget(rotationSlider);

        opacitySlider = new QSlider(verticalLayoutWidget);
        opacitySlider->setObjectName(QString::fromUtf8("opacitySlider"));
        sizePolicy3.setHeightForWidth(opacitySlider->sizePolicy().hasHeightForWidth());
        opacitySlider->setSizePolicy(sizePolicy3);
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush17(QColor(69, 115, 241, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush17);
        QBrush brush18(QColor(180, 200, 255, 255));
        brush18.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush18);
        QBrush brush19(QColor(124, 157, 248, 255));
        brush19.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush19);
        QBrush brush20(QColor(34, 57, 120, 255));
        brush20.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush20);
        QBrush brush21(QColor(46, 76, 161, 255));
        brush21.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush21);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush17);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush22(QColor(162, 185, 248, 255));
        brush22.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush22);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush17);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush18);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush19);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush20);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush21);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush17);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush22);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush20);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush17);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush18);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush19);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush20);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush21);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush20);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush20);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush17);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush17);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush17);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        opacitySlider->setPalette(palette3);
        opacitySlider->setAutoFillBackground(false);
        opacitySlider->setOrientation(Qt::Horizontal);
        opacitySlider->setTickPosition(QSlider::TicksBelow);
        opacitySlider->setTickInterval(0);

        horizontalLayout_7->addWidget(opacitySlider);


        verticalLayout->addLayout(horizontalLayout_7);

        splitter_2 = new QSplitter(verticalLayoutWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Ignored);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy4);
        splitter_2->setMaximumSize(QSize(16777215, 16777215));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush23(QColor(241, 238, 188, 255));
        brush23.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush23);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush24(QColor(248, 246, 221, 255));
        brush24.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush24);
        QBrush brush25(QColor(120, 119, 94, 255));
        brush25.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush25);
        QBrush brush26(QColor(161, 159, 125, 255));
        brush26.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush26);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush23);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush24);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush23);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush24);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush25);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush26);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush23);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush24);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush25);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush23);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush24);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush25);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush26);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush25);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush25);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush23);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush23);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush23);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        splitter_2->setPalette(palette4);
        splitter_2->setOrientation(Qt::Horizontal);
        tableWidget = new Table(splitter_2);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy5);
        tableWidget->setMinimumSize(QSize(0, 0));
        tableWidget->setMaximumSize(QSize(16777215, 16777215));
        tableWidget->setSizeIncrement(QSize(1, 1));
        tableWidget->setBaseSize(QSize(1, 0));
        tableWidget->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        tableWidget->setLineWidth(3);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidget->setGridStyle(Qt::DotLine);
        tableWidget->setColumnCount(4);
        splitter_2->addWidget(tableWidget);
        peli_listWidget = new PeliTable(splitter_2);
        peli_listWidget->setObjectName(QString::fromUtf8("peli_listWidget"));
        sizePolicy.setHeightForWidth(peli_listWidget->sizePolicy().hasHeightForWidth());
        peli_listWidget->setSizePolicy(sizePolicy);
        peli_listWidget->setMinimumSize(QSize(0, 0));
        peli_listWidget->setMaximumSize(QSize(16777215, 16777215));
        peli_listWidget->setLineWidth(2);
        peli_listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        splitter_2->addWidget(peli_listWidget);
        freqtable = new FreqTable(splitter_2);
        freqtable->setObjectName(QString::fromUtf8("freqtable"));
        sizePolicy.setHeightForWidth(freqtable->sizePolicy().hasHeightForWidth());
        freqtable->setSizePolicy(sizePolicy);
        freqtable->setMinimumSize(QSize(0, 0));
        freqtable->setMaximumSize(QSize(16777215, 16777215));
        splitter_2->addWidget(freqtable);

        verticalLayout->addWidget(splitter_2);

        splitter->addWidget(verticalLayoutWidget);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 700, 32));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuSave_all = new QMenu(menuFile);
        menuSave_all->setObjectName(QString::fromUtf8("menuSave_all"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuDelete = new QMenu(menuEdit);
        menuDelete->setObjectName(QString::fromUtf8("menuDelete"));
        menuSelection = new QMenu(menuBar);
        menuSelection->setObjectName(QString::fromUtf8("menuSelection"));
        menuselect_replicated_copy = new QMenu(menuSelection);
        menuselect_replicated_copy->setObjectName(QString::fromUtf8("menuselect_replicated_copy"));
        menuS_elect_inside_Sphere = new QMenu(menuSelection);
        menuS_elect_inside_Sphere->setObjectName(QString::fromUtf8("menuS_elect_inside_Sphere"));
        menuSelect_by = new QMenu(menuSelection);
        menuSelect_by->setObjectName(QString::fromUtf8("menuSelect_by"));
        menuInvert = new QMenu(menuSelection);
        menuInvert->setObjectName(QString::fromUtf8("menuInvert"));
        menuobject = new QMenu(menuSelection);
        menuobject->setObjectName(QString::fromUtf8("menuobject"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QString::fromUtf8("menuTool"));
        menureplicate = new QMenu(menuTool);
        menureplicate->setObjectName(QString::fromUtf8("menureplicate"));
        menu_Translate = new QMenu(menuTool);
        menu_Translate->setObjectName(QString::fromUtf8("menu_Translate"));
        menugeom_param = new QMenu(menuTool);
        menugeom_param->setObjectName(QString::fromUtf8("menugeom_param"));
        menuCreate_group = new QMenu(menuTool);
        menuCreate_group->setObjectName(QString::fromUtf8("menuCreate_group"));
        menuRotate = new QMenu(menuTool);
        menuRotate->setObjectName(QString::fromUtf8("menuRotate"));
        menuObject = new QMenu(menuTool);
        menuObject->setObjectName(QString::fromUtf8("menuObject"));
        menuCrystalography = new QMenu(menuBar);
        menuCrystalography->setObjectName(QString::fromUtf8("menuCrystalography"));
        menuAnimation = new QMenu(menuBar);
        menuAnimation->setObjectName(QString::fromUtf8("menuAnimation"));
        menutrack_CV = new QMenu(menuAnimation);
        menutrack_CV->setObjectName(QString::fromUtf8("menutrack_CV"));
        menuexport = new QMenu(menuAnimation);
        menuexport->setObjectName(QString::fromUtf8("menuexport"));
        menufilter = new QMenu(menuAnimation);
        menufilter->setObjectName(QString::fromUtf8("menufilter"));
        menuScatte_rplot = new QMenu(menuAnimation);
        menuScatte_rplot->setObjectName(QString::fromUtf8("menuScatte_rplot"));
        menuview = new QMenu(menuBar);
        menuview->setObjectName(QString::fromUtf8("menuview"));
        menudefaults = new QMenu(menuview);
        menudefaults->setObjectName(QString::fromUtf8("menudefaults"));
        menuatom = new QMenu(menuview);
        menuatom->setObjectName(QString::fromUtf8("menuatom"));
        menustyles = new QMenu(menuview);
        menustyles->setObjectName(QString::fromUtf8("menustyles"));
        menuLight = new QMenu(menuview);
        menuLight->setObjectName(QString::fromUtf8("menuLight"));
        menuModels = new QMenu(menuview);
        menuModels->setObjectName(QString::fromUtf8("menuModels"));
        menuInsert = new QMenu(menuBar);
        menuInsert->setObjectName(QString::fromUtf8("menuInsert"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuSelection->menuAction());
        menuBar->addAction(menuview->menuAction());
        menuBar->addAction(menuInsert->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuBar->addAction(menuCrystalography->menuAction());
        menuBar->addAction(menuAnimation->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionRender_PovRay);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);
        menuFile->addAction(menuSave_all->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionImport_PwSCF_settings);
        menuFile->addAction(actionClose);
        menuSave_all->addAction(actionas_png);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionundo);
        menuEdit->addAction(actionredo);
        menuEdit->addSeparator();
        menuEdit->addAction(actioncenter);
        menuEdit->addAction(menuDelete->menuAction());
        menuSelection->addAction(actionSelect_All);
        menuSelection->addAction(menuSelect_by->menuAction());
        menuSelection->addSeparator();
        menuSelection->addAction(menuInvert->menuAction());
        menuSelection->addAction(menuS_elect_inside_Sphere->menuAction());
        menuSelection->addAction(actionSelect_fixed);
        menuSelection->addAction(menuselect_replicated_copy->menuAction());
        menuSelection->addAction(actiondeselect);
        menuSelection->addAction(actionGroups);
        menuSelection->addAction(menuobject->menuAction());
        menuselect_replicated_copy->addAction(actionx_y_and_z);
        menuselect_replicated_copy->addAction(actionoriginal);
        menuS_elect_inside_Sphere->addAction(actionInsideSphereAny);
        menuS_elect_inside_Sphere->addAction(actionInsideSphereOnly_Bonded);
        menuSelect_by->addAction(actionSite_Type);
        menuSelect_by->addAction(actionColor);
        menuSelect_by->addAction(actionsphere);
        menuSelect_by->addAction(actionSelectByElement);
        menuSelect_by->addAction(actionSelect_by_bonds);
        menuSelect_by->addAction(actionnmr);
        menuSelect_by->addAction(actionrings_of_size);
        menuSelect_by->addAction(actionAtom_with_n_bond);
        menuSelect_by->addAction(actionIndex);
        menuInvert->addAction(actionInvert_Selection);
        menuInvert->addAction(actionAtoms_Type);
        menuobject->addAction(actionplane_by_index);
        menuTool->addAction(actionFix_Selected);
        menuTool->addAction(actionunfix_selected);
        menuTool->addSeparator();
        menuTool->addAction(menu_Translate->menuAction());
        menuTool->addAction(menuRotate->menuAction());
        menuTool->addAction(menureplicate->menuAction());
        menuTool->addAction(actionconnectivity);
        menuTool->addSeparator();
        menuTool->addAction(actionfrequencies);
        menuTool->addAction(actioncoordinates);
        menuTool->addSeparator();
        menuTool->addAction(menugeom_param->menuAction());
        menuTool->addAction(menuCreate_group->menuAction());
        menuTool->addAction(menuObject->menuAction());
        menureplicate->addAction(actioncustom);
        menureplicate->addAction(actioncustom_selection);
        menureplicate->addAction(actionUnreplicate);
        menureplicate->addAction(actionEliminate_Repeated_Atoms);
        menureplicate->addAction(actionmake_supercell);
        menu_Translate->addAction(actiontranslation_vector);
        menu_Translate->addAction(actionto_position);
        menu_Translate->addAction(actionto_x_value);
        menu_Translate->addAction(actionto_y_value);
        menu_Translate->addAction(actionto_z_value);
        menu_Translate->addAction(actioninside_Cell);
        menu_Translate->addAction(actionAlign_to_Bond);
        menugeom_param->addAction(actionbond_distances_per_type);
        menugeom_param->addAction(actionDistances_per_type);
        menugeom_param->addAction(actionCenter_of_Mass_selected);
        menuCreate_group->addAction(actionFrom_Selected);
        menuCreate_group->addAction(actionFrom_Same_Sigma);
        menuRotate->addAction(actionAround_Z_axis);
        menuRotate->addAction(actionAround_Bond);
        menuObject->addAction(actionadd_plane);
        menuObject->addAction(actionadd_arrow);
        menuCrystalography->addAction(actionSet_Unit_Cell);
        menuAnimation->addAction(actionplay);
        menuAnimation->addAction(actionplay_frequencies);
        menuAnimation->addAction(actionstop);
        menuAnimation->addSeparator();
        menuAnimation->addAction(menutrack_CV->menuAction());
        menuAnimation->addAction(actiontime_delay);
        menuAnimation->addAction(menuexport->menuAction());
        menuAnimation->addAction(actionaverage_animation);
        menuAnimation->addAction(menuScatte_rplot->menuAction());
        menuAnimation->addAction(menufilter->menuAction());
        menuAnimation->addAction(actionCorrect_periodic_trajectory);
        menutrack_CV->addAction(actionbond);
        menutrack_CV->addAction(actionangle);
        menutrack_CV->addAction(actiondihedral);
        menutrack_CV->addAction(actionY_coor_COM);
        menutrack_CV->addAction(actionexport_CV);
        menutrack_CV->addAction(actionExport_Plumed_File);
        menuexport->addAction(actionpeli_png);
        menuexport->addAction(actionFreqs_Animation_gif);
        menuexport->addAction(actionStructure_from_Averages);
        menufilter->addAction(actionFilterByDistance);
        menuScatte_rplot->addAction(action3d);
        menuScatte_rplot->addAction(actiondots);
        menuview->addAction(menudefaults->menuAction());
        menuview->addAction(menuModels->menuAction());
        menuview->addAction(menuatom->menuAction());
        menuview->addAction(actionBackground_color);
        menuview->addAction(menustyles->menuAction());
        menuview->addAction(menuLight->menuAction());
        menuview->addAction(actionImage);
        menuview->addAction(actionSet_view_range);
        menudefaults->addAction(actionalldefaults);
        menudefaults->addAction(actioncolors);
        menudefaults->addAction(actionRestoreSphere);
        menudefaults->addAction(actionRestoreRadio);
        menudefaults->addAction(actionRestoreBond_Radio);
        menuatom->addAction(actionhide);
        menuatom->addAction(actionshow_all);
        menuatom->addAction(actionradius);
        menuatom->addAction(actionsphere_size);
        menuatom->addAction(actionChange_to_element);
        menuatom->addAction(actionInfo);
        menustyles->addAction(actionsave_current_style);
        menustyles->addAction(actionload_style);
        menustyles->addAction(actionLoad_Style_all_tabs);
        menustyles->addAction(actionLoad_Style_from_state_rs);
        menuLight->addAction(actionSpecular);
        menuLight->addAction(actionEnable_disable_Light);
        menuModels->addAction(actionwire);
        menuModels->addAction(actionsticks);
        menuInsert->addAction(actionInsertFragment);
        menuInsert->addAction(actionDownload_from_IZA_DB);
        mainToolBar->addAction(actionOpenToolBar);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSaveToolBar);
        mainToolBar->addAction(actionSave_asToolBar);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionEditToolBar);
        mainToolBar->addAction(actionShow_Cell);
        mainToolBar->addAction(actionSpecularToolBar);
        mainToolBar->addAction(actionTrack_Coors);
        mainToolBar->addAction(sectionImageToolBar);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "&Open", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRender_PovRay->setText(QCoreApplication::translate("MainWindow", "&Render PovRay", nullptr));
        actionCopy->setText(QCoreApplication::translate("MainWindow", "&Copy", nullptr));
#if QT_CONFIG(shortcut)
        actionCopy->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPaste->setText(QCoreApplication::translate("MainWindow", "&Paste", nullptr));
#if QT_CONFIG(shortcut)
        actionPaste->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionInvert_Selection->setText(QCoreApplication::translate("MainWindow", "&Selection", nullptr));
#if QT_CONFIG(shortcut)
        actionInvert_Selection->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+I", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFix_Selected->setText(QCoreApplication::translate("MainWindow", "&Fix Selected", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "&Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionImport_PwSCF_settings->setText(QCoreApplication::translate("MainWindow", "&Import PwSCF settings", nullptr));
        actionClose->setText(QCoreApplication::translate("MainWindow", "&Close", nullptr));
#if QT_CONFIG(shortcut)
        actionClose->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSet_Unit_Cell->setText(QCoreApplication::translate("MainWindow", "&Set Unit Cell", nullptr));
#if QT_CONFIG(shortcut)
        actionSet_Unit_Cell->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_as->setText(QCoreApplication::translate("MainWindow", "Sa&ve as ...", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_as->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionunfix_selected->setText(QCoreApplication::translate("MainWindow", "&Unfix selected", nullptr));
        actionSelect_All->setText(QCoreApplication::translate("MainWindow", "Se&lect All", nullptr));
        actionSelect_by_bonds->setText(QCoreApplication::translate("MainWindow", "&Bonds", nullptr));
#if QT_CONFIG(shortcut)
        actionSelect_by_bonds->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+B", nullptr));
#endif // QT_CONFIG(shortcut)
        actionplay->setText(QCoreApplication::translate("MainWindow", "&Play Movie", nullptr));
#if QT_CONFIG(shortcut)
        actionplay->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionsticks->setText(QCoreApplication::translate("MainWindow", "&Licorice", nullptr));
#if QT_CONFIG(shortcut)
        actionsticks->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+L", nullptr));
#endif // QT_CONFIG(shortcut)
        actioncustom->setText(QCoreApplication::translate("MainWindow", "&All", nullptr));
#if QT_CONFIG(shortcut)
        actioncustom->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionconnectivity->setText(QCoreApplication::translate("MainWindow", "&connectivity", nullptr));
        actionundo->setText(QCoreApplication::translate("MainWindow", "&undo", nullptr));
#if QT_CONFIG(shortcut)
        actionundo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionredo->setText(QCoreApplication::translate("MainWindow", "&Redo", nullptr));
#if QT_CONFIG(shortcut)
        actionredo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actiontranslation_vector->setText(QCoreApplication::translate("MainWindow", "&Translation vector", nullptr));
        actionto_x_value->setText(QCoreApplication::translate("MainWindow", "to &x value", nullptr));
        actionto_y_value->setText(QCoreApplication::translate("MainWindow", "to &y value", nullptr));
        actionto_z_value->setText(QCoreApplication::translate("MainWindow", "to &z value", nullptr));
        actionsphere_radio->setText(QCoreApplication::translate("MainWindow", "sphere &radio", nullptr));
        actionfrequencies->setText(QCoreApplication::translate("MainWindow", "Fr&equencies", nullptr));
        actionstop->setText(QCoreApplication::translate("MainWindow", "&Stop", nullptr));
        actionplay_frequencies->setText(QCoreApplication::translate("MainWindow", "Play &Frequencies", nullptr));
#if QT_CONFIG(shortcut)
        actionplay_frequencies->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        actioncustom_selection->setText(QCoreApplication::translate("MainWindow", "&Selection", nullptr));
        actioninside_Cell->setText(QCoreApplication::translate("MainWindow", "&Inside Cell", nullptr));
        actioncenter->setText(QCoreApplication::translate("MainWindow", "C&enter", nullptr));
#if QT_CONFIG(shortcut)
        actioncenter->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Alt+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNew->setText(QCoreApplication::translate("MainWindow", "&New", nullptr));
        actionalldefaults->setText(QCoreApplication::translate("MainWindow", "&All defaults", nullptr));
#if QT_CONFIG(shortcut)
        actionalldefaults->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
        actioncolors->setText(QCoreApplication::translate("MainWindow", "&Color", nullptr));
        actionwire->setText(QCoreApplication::translate("MainWindow", "&Wire", nullptr));
        actionhide->setText(QCoreApplication::translate("MainWindow", "&Hide", nullptr));
#if QT_CONFIG(shortcut)
        actionhide->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+H", nullptr));
#endif // QT_CONFIG(shortcut)
        actionshow_all->setText(QCoreApplication::translate("MainWindow", "&Show all", nullptr));
        actionBackground_color->setText(QCoreApplication::translate("MainWindow", "&Background Color", nullptr));
#if QT_CONFIG(shortcut)
        actionBackground_color->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+B", nullptr));
#endif // QT_CONFIG(shortcut)
        actioncoordinates->setText(QCoreApplication::translate("MainWindow", "C&oordinates", nullptr));
        actionbond_distances_per_type->setText(QCoreApplication::translate("MainWindow", "&bond distances per type", nullptr));
        actionbond->setText(QCoreApplication::translate("MainWindow", "&New bond", nullptr));
        actionangle->setText(QCoreApplication::translate("MainWindow", "Ne&w angle", nullptr));
        actiondihedral->setText(QCoreApplication::translate("MainWindow", "New &dihedral", nullptr));
        actiontime_delay->setText(QCoreApplication::translate("MainWindow", "&Time delay", nullptr));
        actionpeli_png->setText(QCoreApplication::translate("MainWindow", "&Animated gif", nullptr));
#if QT_CONFIG(shortcut)
        actionpeli_png->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Alt+G", nullptr));
#endif // QT_CONFIG(shortcut)
        actionaverage_animation->setText(QCoreApplication::translate("MainWindow", "&Average Animation", nullptr));
        actionexport_CV->setText(QCoreApplication::translate("MainWindow", "&Export  CVs", nullptr));
        actionSelect_fixed->setText(QCoreApplication::translate("MainWindow", "Select &fixed", nullptr));
        actionsave_current_style->setText(QCoreApplication::translate("MainWindow", "&Save current style", nullptr));
        actionload_style->setText(QCoreApplication::translate("MainWindow", "&Load style", nullptr));
        actionx_y_and_z->setText(QCoreApplication::translate("MainWindow", "&x y and z ", nullptr));
        actionoriginal->setText(QCoreApplication::translate("MainWindow", "&original", nullptr));
        actionradius->setText(QCoreApplication::translate("MainWindow", "&Radius", nullptr));
        actionsphere_size->setText(QCoreApplication::translate("MainWindow", "S&phere size", nullptr));
        actionbonded_and_inside_a_sphre->setText(QCoreApplication::translate("MainWindow", "bonded and inside a sphre", nullptr));
        actionInsideSphereAny->setText(QCoreApplication::translate("MainWindow", "&Any", nullptr));
        actionInsideSphereOnly_Bonded->setText(QCoreApplication::translate("MainWindow", "&Only bonded", nullptr));
        actionInfo->setText(QCoreApplication::translate("MainWindow", "&Info", nullptr));
        actionFilterByDistance->setText(QCoreApplication::translate("MainWindow", "&Distance", nullptr));
        actionSite_Type->setText(QCoreApplication::translate("MainWindow", "&Site Type", nullptr));
        actionColor->setText(QCoreApplication::translate("MainWindow", "&Color", nullptr));
        actionsphere->setText(QCoreApplication::translate("MainWindow", "S&phere", nullptr));
        actionDistances_per_type->setText(QCoreApplication::translate("MainWindow", "&Distances per type", nullptr));
        actiondeselect->setText(QCoreApplication::translate("MainWindow", "&Deselect All", nullptr));
        actionUnreplicate->setText(QCoreApplication::translate("MainWindow", "&Unreplicate", nullptr));
        actionRestoreSphere->setText(QCoreApplication::translate("MainWindow", "&Sphere", nullptr));
#if QT_CONFIG(tooltip)
        actionRestoreSphere->setToolTip(QCoreApplication::translate("MainWindow", "Restore Sphere", nullptr));
#endif // QT_CONFIG(tooltip)
        actionRestoreRadio->setText(QCoreApplication::translate("MainWindow", "&Radio", nullptr));
        actionRestoreBond_Radio->setText(QCoreApplication::translate("MainWindow", "&Bond Radio", nullptr));
        actionInsertFragment->setText(QCoreApplication::translate("MainWindow", "&Fragment", nullptr));
#if QT_CONFIG(shortcut)
        actionInsertFragment->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+I", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelectByElement->setText(QCoreApplication::translate("MainWindow", "&Element", nullptr));
#if QT_CONFIG(shortcut)
        actionSelectByElement->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpenToolBar->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
#if QT_CONFIG(tooltip)
        actionOpenToolBar->setToolTip(QCoreApplication::translate("MainWindow", "Open file", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSaveToolBar->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionEditToolBar->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
#if QT_CONFIG(shortcut)
        actionEditToolBar->setShortcut(QCoreApplication::translate("MainWindow", "F2", nullptr));
#endif // QT_CONFIG(shortcut)
        actionShow_Cell->setText(QString());
#if QT_CONFIG(shortcut)
        actionShow_Cell->setShortcut(QCoreApplication::translate("MainWindow", "C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSpecular->setText(QCoreApplication::translate("MainWindow", "&Specular", nullptr));
        actionEnable_disable_Light->setText(QCoreApplication::translate("MainWindow", "&Enable/disable Light", nullptr));
        actionSpecularToolBar->setText(QString());
        actionChange_to_element->setText(QCoreApplication::translate("MainWindow", "&Change to Element ...", nullptr));
        actionAtoms_Type->setText(QCoreApplication::translate("MainWindow", "&Atoms Type", nullptr));
        actionTrack_Coors->setText(QString());
#if QT_CONFIG(shortcut)
        actionTrack_Coors->setShortcut(QCoreApplication::translate("MainWindow", "F3", nullptr));
#endif // QT_CONFIG(shortcut)
        actionGroups->setText(QCoreApplication::translate("MainWindow", "&Groups", nullptr));
        actionFrom_Selected->setText(QCoreApplication::translate("MainWindow", "&From Selected", nullptr));
        actionSave_asToolBar->setText(QCoreApplication::translate("MainWindow", "Save as", nullptr));
        actionFreqs_Animation_gif->setText(QCoreApplication::translate("MainWindow", "&Freqs Animation gif", nullptr));
        actionAlign_to_Bond->setText(QCoreApplication::translate("MainWindow", "&Align to Bond", nullptr));
        actionImage->setText(QCoreApplication::translate("MainWindow", "&Section Image", nullptr));
        sectionImageToolBar->setText(QString());
        actionFrom_Same_Sigma->setText(QCoreApplication::translate("MainWindow", "From &Same Sigma", nullptr));
        actionnmr->setText(QCoreApplication::translate("MainWindow", "&nmr", nullptr));
        actionLoad_Style_all_tabs->setText(QCoreApplication::translate("MainWindow", "L&oad Style all tabs", nullptr));
        actionas_png->setText(QCoreApplication::translate("MainWindow", "&as png", nullptr));
        actionAround_Z_axis->setText(QCoreApplication::translate("MainWindow", "&Around Z axis", nullptr));
        actionAround_Bond->setText(QCoreApplication::translate("MainWindow", "Around &Bond", nullptr));
        actionStructure_from_Averages->setText(QCoreApplication::translate("MainWindow", "&Structure from Averages", nullptr));
        actionCorrect_periodic_trajectory->setText(QCoreApplication::translate("MainWindow", "C&orrect periodic trajectory", nullptr));
        actionrings_of_size->setText(QCoreApplication::translate("MainWindow", "&Rings of size #", nullptr));
#if QT_CONFIG(tooltip)
        actionrings_of_size->setToolTip(QCoreApplication::translate("MainWindow", "Rings of size #", nullptr));
#endif // QT_CONFIG(tooltip)
        actionLoad_Style_from_state_rs->setText(QCoreApplication::translate("MainWindow", "Load Style &from state(.rs)", nullptr));
        actionCenter_of_Mass_selected->setText(QCoreApplication::translate("MainWindow", "&Center of Mass (selected)", nullptr));
        actionSet_view_range->setText(QCoreApplication::translate("MainWindow", "Set &Zoom", nullptr));
#if QT_CONFIG(shortcut)
        actionSet_view_range->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Alt+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionY_coor_COM->setText(QCoreApplication::translate("MainWindow", "&Y coor (COM)", nullptr));
        actionAtom_with_n_bond->setText(QCoreApplication::translate("MainWindow", "&Atom with n bond", nullptr));
#if QT_CONFIG(shortcut)
        actionAtom_with_n_bond->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Alt+B", nullptr));
#endif // QT_CONFIG(shortcut)
        action3d->setText(QCoreApplication::translate("MainWindow", "&3d", nullptr));
        actiondots->setText(QCoreApplication::translate("MainWindow", "&dots", nullptr));
        actionExport_Plumed_File->setText(QCoreApplication::translate("MainWindow", "Export &Plumed File", nullptr));
        actionadd_plane->setText(QCoreApplication::translate("MainWindow", "&add plane", nullptr));
#if QT_CONFIG(shortcut)
        actionadd_plane->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionplane_by_index->setText(QCoreApplication::translate("MainWindow", "&plane by index", nullptr));
        actionadd_arrow->setText(QCoreApplication::translate("MainWindow", "a&dd arrow", nullptr));
        actionEliminate_Repeated_Atoms->setText(QCoreApplication::translate("MainWindow", "&Eliminate Repeated Atoms", nullptr));
        actionto_position->setText(QCoreApplication::translate("MainWindow", "to &position", nullptr));
        actionIndex->setText(QCoreApplication::translate("MainWindow", "&Index", nullptr));
        actionmake_supercell->setText(QCoreApplication::translate("MainWindow", "&make supercell", nullptr));
        actionDownload_from_IZA_DB->setText(QCoreApplication::translate("MainWindow", "Download from IZA DB", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Atomic Radius", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Cylinder Radius", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "shininess", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Element", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Sphere Radius", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Atom Color", nullptr));
        colorpushButton->setText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "Quality", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Rot speed", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Opacity", nullptr));
#if QT_CONFIG(whatsthis)
        opacitySlider->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Label", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "z", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "Fi&le", nullptr));
        menuSave_all->setTitle(QCoreApplication::translate("MainWindow", "Sav&e all", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuDelete->setTitle(QCoreApplication::translate("MainWindow", "&Delete", nullptr));
        menuSelection->setTitle(QCoreApplication::translate("MainWindow", "Selectio&n", nullptr));
        menuselect_replicated_copy->setTitle(QCoreApplication::translate("MainWindow", "Select &replicated copy", nullptr));
        menuS_elect_inside_Sphere->setTitle(QCoreApplication::translate("MainWindow", "S&elect inside Sphere", nullptr));
        menuSelect_by->setTitle(QCoreApplication::translate("MainWindow", "Sele&ct by", nullptr));
        menuInvert->setTitle(QCoreApplication::translate("MainWindow", "&Invert", nullptr));
        menuobject->setTitle(QCoreApplication::translate("MainWindow", "&object", nullptr));
        menuTool->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        menureplicate->setTitle(QCoreApplication::translate("MainWindow", "&Repete", nullptr));
        menu_Translate->setTitle(QCoreApplication::translate("MainWindow", "&Translate", nullptr));
        menugeom_param->setTitle(QCoreApplication::translate("MainWindow", "Ca&lculate", nullptr));
        menuCreate_group->setTitle(QCoreApplication::translate("MainWindow", "Create &group", nullptr));
        menuRotate->setTitle(QCoreApplication::translate("MainWindow", "Rotate", nullptr));
        menuObject->setTitle(QCoreApplication::translate("MainWindow", "O&bject", nullptr));
        menuCrystalography->setTitle(QCoreApplication::translate("MainWindow", "Crystalo&graphy", nullptr));
        menuAnimation->setTitle(QCoreApplication::translate("MainWindow", "Ani&mation", nullptr));
        menutrack_CV->setTitle(QCoreApplication::translate("MainWindow", "&Collective Variable", nullptr));
        menuexport->setTitle(QCoreApplication::translate("MainWindow", "&Export", nullptr));
        menufilter->setTitle(QCoreApplication::translate("MainWindow", "F&ilter", nullptr));
        menuScatte_rplot->setTitle(QCoreApplication::translate("MainWindow", "Scatte&rplot", nullptr));
        menuview->setTitle(QCoreApplication::translate("MainWindow", "&View", nullptr));
        menudefaults->setTitle(QCoreApplication::translate("MainWindow", "&Restore Defaults", nullptr));
        menuatom->setTitle(QCoreApplication::translate("MainWindow", "&Atom", nullptr));
        menustyles->setTitle(QCoreApplication::translate("MainWindow", "St&yles", nullptr));
        menuLight->setTitle(QCoreApplication::translate("MainWindow", "&Light", nullptr));
        menuModels->setTitle(QCoreApplication::translate("MainWindow", "&Models", nullptr));
        menuInsert->setTitle(QCoreApplication::translate("MainWindow", "Inse&rt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
