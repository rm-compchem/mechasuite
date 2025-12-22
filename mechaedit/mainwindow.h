#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QMimeData>
#include <QtGui/QIcon>
#include "qglbegin.h"
#include "multichoicedialog.h"
#include "multichoicedialog2.h"
#include "singlechoicedialog.h"
#include "table.h"
//#include "include/gifski.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    Ui::MainWindow *ui;

    vector<QGLBegin*> gl ;
    vector<Atom> clipboard;
    QGLBegin *cgl;               //current gl instance
    int ctab_index;             // current tab index
    int sphereSliderVal;
    int radioSliderVal;
    int cylSliderMoving;
    vector<float> oldRadiosSphereCyl;

    void tableWidget_update();
    void keyPressEvent(QKeyEvent *event);
    void save_picture(QString filename);
    void load_data(QString filename, QGLBegin *glwidget = NULL);
    void read_settings();
    void openFile(QString filePath);
    void parseArguments(int argc, char *argv[]);

private slots:
    //void on_openGLWidget_destroyed();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionSave_triggered();

    void on_actionRender_PovRay_triggered();

    void on_sphereSlider_sliderMoved(int value);

    void on_atomradioSlider_sliderMoved(int value);

    void on_actionSet_Unit_Cell_triggered();

    void on_actionFix_Selected_triggered();

    void on_actionunfix_selected_triggered();

    void on_actionInvert_Selection_triggered();

    void on_actionSelect_All_triggered();

    void on_actionSelect_by_bonds_triggered();

    void on_colorpushButton_clicked();

    void on_bondcylinderSlider_valueChanged(int value);

    void on_qualitySlider_valueChanged(int value);

    void on_rotationSlider_valueChanged(int value);

    void on_opacitySlider_valueChanged(int value);

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionClose_triggered();

    void on_combo_request();

    void on_openGLWidget_atoms_selected();

    void on_openGLWidget_atoms_deselected();

    void on_openGLWidget_data_changed();

    void on_actionplay_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_tabWidget_currentChanged();

    void on_tableWidget_itemChanged(QTableWidgetItem *current);

    void on_tableWidget_rowSelected();

    void on_actionsticks_triggered();

    void on_actioncustom_triggered();

    void on_actionconnectivity_triggered();

    void on_actionundo_triggered();

    void on_actionredo_triggered();

    void on_actiontranslation_vector_triggered();

    void on_actionto_x_value_triggered();

    void on_actionto_y_value_triggered();

    void on_actionto_z_value_triggered();

    void on_actionfrequencies_triggered();

    void on_actionstop_triggered();

    void on_actionplay_frequencies_triggered();

    void on_freqtable_sectionClicked(int logicalindex);

    void on_actioncustom_selection_triggered();

    void on_actioninside_Cell_triggered();

    void on_actioncenter_triggered();

    void on_actionNew_triggered();

    void on_actionalldefaults_triggered();

    void on_actioncolors_triggered();

    void on_actionwire_triggered();

    void on_actionhide_triggered();

    void on_actionshow_all_triggered();

    void on_actionBackground_color_triggered();

    void on_actioncoordinates_triggered();

    void on_actionbond_distances_per_type_triggered();

    void on_actionbond_triggered();

    void on_actiontime_delay_triggered();

    void on_actionpeli_png_triggered();

    void on_actionaverage_animation_triggered();

    void on_actionexport_CV_triggered();

    void on_actiondihedral_triggered();

    void on_actionSelect_fixed_triggered();

    void on_actionsave_current_style_triggered();

    void on_actionload_style_triggered();

    void on_peli_listWidget_step_deleted(vector<int> indices);

    void on_peli_listWidget_itemSelectionChanged();

    void on_actionx_y_and_z_triggered();

    void on_actionoriginal_triggered();

    void on_actionsphere_size_triggered();

    void on_actionradius_triggered();

    void on_shininessSlider_valueChanged(int value);

    void on_actionInsideSphereAny_triggered();

    void on_actionInsideSphereOnly_Bonded_triggered();

    void on_actionInfo_triggered();

    void on_actionFilterByDistance_triggered();

    void on_actionSite_Type_triggered();

    void on_actionDistances_per_type_triggered();

    void on_actionsphere_triggered();

    void on_actiondeselect_triggered();

    void on_actionUnreplicate_triggered();

    void on_actionRestoreSphere_triggered();

    void on_actionRestoreRadio_triggered();

    void on_actionRestoreBond_Radio_triggered();

    void on_actionInsertFragment_triggered();

    void on_atomradioSlider_sliderPressed();

    void on_atomradioSlider_sliderReleased();

    void on_sphereSlider_sliderPressed();

    void on_sphereSlider_sliderReleased();

    void dragEnterEvent(QDragEnterEvent *event);

    void dropEvent(QDropEvent *event);

    void on_actionSelectByElement_triggered();

    void on_actionOpenToolBar_triggered();

    void on_actionEditToolBar_toggled(bool arg1);

    void on_actionShow_Cell_toggled(bool arg1);

    void on_actionSpecularToolBar_toggled(bool arg1);

    void on_actionSpecular_toggled(bool arg1);

    void on_actionEnable_disable_Light_toggled(bool arg1);

    void on_actionSaveToolBar_triggered();

    void on_actionChange_to_element_triggered();

    void on_actionAtoms_Type_triggered();

    void on_actionTrack_Coors_toggled(bool arg1);

    void on_actionGroups_triggered();

    void on_actionFrom_Selected_triggered();

    void on_actionSave_asToolBar_triggered();

    void on_actionFreqs_Animation_gif_triggered();

    void on_actionAlign_to_Bond_triggered();

    void on_actionImage_toggled(bool arg1);

    void on_sectionImageToolBar_toggled(bool arg1);

    void on_actionFrom_Same_Sigma_triggered();

    void on_actionnmr_triggered();

    void on_actionLoad_Style_all_tabs_triggered();

    void on_actionas_png_triggered();

    void on_actionAround_Z_axis_triggered();

    void on_actionAround_Bond_triggered();

    void on_actionStructure_from_Averages_triggered();

    void on_actionCorrect_periodic_trajectory_triggered();

    void on_actionangle_triggered();

    void checkMemoryUsage();

    void on_actionrings_of_size_triggered();

    void on_actionLoad_Style_from_state_rs_triggered();

    void on_actionCenter_of_Mass_selected_triggered();

    void on_actionSet_view_range_triggered();

    void on_actionY_coor_COM_triggered();


    void on_actionAtom_with_n_bond_triggered();

    void on_action3d_triggered();

    void on_actiondots_triggered();

    void on_actionExport_Plumed_File_triggered();

    void on_actionadd_plane_triggered();

    void on_actionplane_by_index_triggered();

    void on_actionadd_arrow_triggered();

    void on_actionEliminate_Repeated_Atoms_triggered();

    void on_actionto_position_triggered();

    void on_actionIndex_triggered();

    void on_actionmake_supercell_triggered();

    void on_actionDownload_from_IZA_DB_triggered();

private:

};


#endif // MAINWINDOW_H
