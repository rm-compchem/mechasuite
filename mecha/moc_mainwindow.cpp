/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[147];
    char stringdata0[4229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 26), // "on_actionSave_as_triggered"
QT_MOC_LITERAL(4, 63, 23), // "on_actionSave_triggered"
QT_MOC_LITERAL(5, 87, 32), // "on_actionRender_PovRay_triggered"
QT_MOC_LITERAL(6, 120, 27), // "on_sphereSlider_sliderMoved"
QT_MOC_LITERAL(7, 148, 5), // "value"
QT_MOC_LITERAL(8, 154, 30), // "on_atomradioSlider_sliderMoved"
QT_MOC_LITERAL(9, 185, 32), // "on_actionSet_Unit_Cell_triggered"
QT_MOC_LITERAL(10, 218, 31), // "on_actionFix_Selected_triggered"
QT_MOC_LITERAL(11, 250, 33), // "on_actionunfix_selected_trigg..."
QT_MOC_LITERAL(12, 284, 35), // "on_actionInvert_Selection_tri..."
QT_MOC_LITERAL(13, 320, 29), // "on_actionSelect_All_triggered"
QT_MOC_LITERAL(14, 350, 34), // "on_actionSelect_by_bonds_trig..."
QT_MOC_LITERAL(15, 385, 26), // "on_colorpushButton_clicked"
QT_MOC_LITERAL(16, 412, 34), // "on_bondcylinderSlider_valueCh..."
QT_MOC_LITERAL(17, 447, 29), // "on_qualitySlider_valueChanged"
QT_MOC_LITERAL(18, 477, 30), // "on_rotationSlider_valueChanged"
QT_MOC_LITERAL(19, 508, 29), // "on_opacitySlider_valueChanged"
QT_MOC_LITERAL(20, 538, 23), // "on_actionCopy_triggered"
QT_MOC_LITERAL(21, 562, 24), // "on_actionPaste_triggered"
QT_MOC_LITERAL(22, 587, 24), // "on_actionClose_triggered"
QT_MOC_LITERAL(23, 612, 16), // "on_combo_request"
QT_MOC_LITERAL(24, 629, 30), // "on_openGLWidget_atoms_selected"
QT_MOC_LITERAL(25, 660, 32), // "on_openGLWidget_atoms_deselected"
QT_MOC_LITERAL(26, 693, 28), // "on_openGLWidget_data_changed"
QT_MOC_LITERAL(27, 722, 23), // "on_actionplay_triggered"
QT_MOC_LITERAL(28, 746, 30), // "on_tabWidget_tabCloseRequested"
QT_MOC_LITERAL(29, 777, 5), // "index"
QT_MOC_LITERAL(30, 783, 27), // "on_tabWidget_currentChanged"
QT_MOC_LITERAL(31, 811, 26), // "on_tableWidget_itemChanged"
QT_MOC_LITERAL(32, 838, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(33, 856, 7), // "current"
QT_MOC_LITERAL(34, 864, 26), // "on_tableWidget_rowSelected"
QT_MOC_LITERAL(35, 891, 25), // "on_actionsticks_triggered"
QT_MOC_LITERAL(36, 917, 25), // "on_actioncustom_triggered"
QT_MOC_LITERAL(37, 943, 31), // "on_actionconnectivity_triggered"
QT_MOC_LITERAL(38, 975, 23), // "on_actionundo_triggered"
QT_MOC_LITERAL(39, 999, 23), // "on_actionredo_triggered"
QT_MOC_LITERAL(40, 1023, 37), // "on_actiontranslation_vector_t..."
QT_MOC_LITERAL(41, 1061, 29), // "on_actionto_x_value_triggered"
QT_MOC_LITERAL(42, 1091, 29), // "on_actionto_y_value_triggered"
QT_MOC_LITERAL(43, 1121, 29), // "on_actionto_z_value_triggered"
QT_MOC_LITERAL(44, 1151, 30), // "on_actionfrequencies_triggered"
QT_MOC_LITERAL(45, 1182, 23), // "on_actionstop_triggered"
QT_MOC_LITERAL(46, 1206, 35), // "on_actionplay_frequencies_tri..."
QT_MOC_LITERAL(47, 1242, 27), // "on_freqtable_sectionClicked"
QT_MOC_LITERAL(48, 1270, 12), // "logicalindex"
QT_MOC_LITERAL(49, 1283, 35), // "on_actioncustom_selection_tri..."
QT_MOC_LITERAL(50, 1319, 30), // "on_actioninside_Cell_triggered"
QT_MOC_LITERAL(51, 1350, 25), // "on_actioncenter_triggered"
QT_MOC_LITERAL(52, 1376, 22), // "on_actionNew_triggered"
QT_MOC_LITERAL(53, 1399, 30), // "on_actionalldefaults_triggered"
QT_MOC_LITERAL(54, 1430, 25), // "on_actioncolors_triggered"
QT_MOC_LITERAL(55, 1456, 23), // "on_actionwire_triggered"
QT_MOC_LITERAL(56, 1480, 23), // "on_actionhide_triggered"
QT_MOC_LITERAL(57, 1504, 27), // "on_actionshow_all_triggered"
QT_MOC_LITERAL(58, 1532, 35), // "on_actionBackground_color_tri..."
QT_MOC_LITERAL(59, 1568, 30), // "on_actioncoordinates_triggered"
QT_MOC_LITERAL(60, 1599, 42), // "on_actionbond_distances_per_t..."
QT_MOC_LITERAL(61, 1642, 23), // "on_actionbond_triggered"
QT_MOC_LITERAL(62, 1666, 29), // "on_actiontime_delay_triggered"
QT_MOC_LITERAL(63, 1696, 27), // "on_actionpeli_png_triggered"
QT_MOC_LITERAL(64, 1724, 36), // "on_actionaverage_animation_tr..."
QT_MOC_LITERAL(65, 1761, 28), // "on_actionexport_CV_triggered"
QT_MOC_LITERAL(66, 1790, 27), // "on_actiondihedral_triggered"
QT_MOC_LITERAL(67, 1818, 31), // "on_actionSelect_fixed_triggered"
QT_MOC_LITERAL(68, 1850, 37), // "on_actionsave_current_style_t..."
QT_MOC_LITERAL(69, 1888, 29), // "on_actionload_style_triggered"
QT_MOC_LITERAL(70, 1918, 31), // "on_peli_listWidget_step_deleted"
QT_MOC_LITERAL(71, 1950, 11), // "vector<int>"
QT_MOC_LITERAL(72, 1962, 7), // "indices"
QT_MOC_LITERAL(73, 1970, 39), // "on_peli_listWidget_itemSelect..."
QT_MOC_LITERAL(74, 2010, 28), // "on_actionx_y_and_z_triggered"
QT_MOC_LITERAL(75, 2039, 27), // "on_actionoriginal_triggered"
QT_MOC_LITERAL(76, 2067, 30), // "on_actionsphere_size_triggered"
QT_MOC_LITERAL(77, 2098, 25), // "on_actionradius_triggered"
QT_MOC_LITERAL(78, 2124, 31), // "on_shininessSlider_valueChanged"
QT_MOC_LITERAL(79, 2156, 34), // "on_actionInsideSphereAny_trig..."
QT_MOC_LITERAL(80, 2191, 42), // "on_actionInsideSphereOnly_Bon..."
QT_MOC_LITERAL(81, 2234, 23), // "on_actionInfo_triggered"
QT_MOC_LITERAL(82, 2258, 35), // "on_actionFilterByDistance_tri..."
QT_MOC_LITERAL(83, 2294, 28), // "on_actionSite_Type_triggered"
QT_MOC_LITERAL(84, 2323, 37), // "on_actionDistances_per_type_t..."
QT_MOC_LITERAL(85, 2361, 25), // "on_actionsphere_triggered"
QT_MOC_LITERAL(86, 2387, 27), // "on_actiondeselect_triggered"
QT_MOC_LITERAL(87, 2415, 30), // "on_actionUnreplicate_triggered"
QT_MOC_LITERAL(88, 2446, 32), // "on_actionRestoreSphere_triggered"
QT_MOC_LITERAL(89, 2479, 31), // "on_actionRestoreRadio_triggered"
QT_MOC_LITERAL(90, 2511, 36), // "on_actionRestoreBond_Radio_tr..."
QT_MOC_LITERAL(91, 2548, 33), // "on_actionInsertFragment_trigg..."
QT_MOC_LITERAL(92, 2582, 32), // "on_atomradioSlider_sliderPressed"
QT_MOC_LITERAL(93, 2615, 33), // "on_atomradioSlider_sliderRele..."
QT_MOC_LITERAL(94, 2649, 29), // "on_sphereSlider_sliderPressed"
QT_MOC_LITERAL(95, 2679, 30), // "on_sphereSlider_sliderReleased"
QT_MOC_LITERAL(96, 2710, 14), // "dragEnterEvent"
QT_MOC_LITERAL(97, 2725, 16), // "QDragEnterEvent*"
QT_MOC_LITERAL(98, 2742, 5), // "event"
QT_MOC_LITERAL(99, 2748, 9), // "dropEvent"
QT_MOC_LITERAL(100, 2758, 11), // "QDropEvent*"
QT_MOC_LITERAL(101, 2770, 34), // "on_actionSelectByElement_trig..."
QT_MOC_LITERAL(102, 2805, 30), // "on_actionOpenToolBar_triggered"
QT_MOC_LITERAL(103, 2836, 28), // "on_actionEditToolBar_toggled"
QT_MOC_LITERAL(104, 2865, 4), // "arg1"
QT_MOC_LITERAL(105, 2870, 26), // "on_actionShow_Cell_toggled"
QT_MOC_LITERAL(106, 2897, 32), // "on_actionSpecularToolBar_toggled"
QT_MOC_LITERAL(107, 2930, 25), // "on_actionSpecular_toggled"
QT_MOC_LITERAL(108, 2956, 37), // "on_actionEnable_disable_Light..."
QT_MOC_LITERAL(109, 2994, 30), // "on_actionSaveToolBar_triggered"
QT_MOC_LITERAL(110, 3025, 36), // "on_actionChange_to_element_tr..."
QT_MOC_LITERAL(111, 3062, 29), // "on_actionAtoms_Type_triggered"
QT_MOC_LITERAL(112, 3092, 28), // "on_actionTrack_Coors_toggled"
QT_MOC_LITERAL(113, 3121, 25), // "on_actionGroups_triggered"
QT_MOC_LITERAL(114, 3147, 32), // "on_actionFrom_Selected_triggered"
QT_MOC_LITERAL(115, 3180, 33), // "on_actionSave_asToolBar_trigg..."
QT_MOC_LITERAL(116, 3214, 38), // "on_actionFreqs_Animation_gif_..."
QT_MOC_LITERAL(117, 3253, 32), // "on_actionAlign_to_Bond_triggered"
QT_MOC_LITERAL(118, 3286, 22), // "on_actionImage_toggled"
QT_MOC_LITERAL(119, 3309, 30), // "on_sectionImageToolBar_toggled"
QT_MOC_LITERAL(120, 3340, 34), // "on_actionFrom_Same_Sigma_trig..."
QT_MOC_LITERAL(121, 3375, 22), // "on_actionnmr_triggered"
QT_MOC_LITERAL(122, 3398, 38), // "on_actionLoad_Style_all_tabs_..."
QT_MOC_LITERAL(123, 3437, 25), // "on_actionas_png_triggered"
QT_MOC_LITERAL(124, 3463, 32), // "on_actionAround_Z_axis_triggered"
QT_MOC_LITERAL(125, 3496, 30), // "on_actionAround_Bond_triggered"
QT_MOC_LITERAL(126, 3527, 42), // "on_actionStructure_from_Avera..."
QT_MOC_LITERAL(127, 3570, 46), // "on_actionCorrect_periodic_tra..."
QT_MOC_LITERAL(128, 3617, 24), // "on_actionangle_triggered"
QT_MOC_LITERAL(129, 3642, 16), // "checkMemoryUsage"
QT_MOC_LITERAL(130, 3659, 32), // "on_actionrings_of_size_triggered"
QT_MOC_LITERAL(131, 3692, 43), // "on_actionLoad_Style_from_stat..."
QT_MOC_LITERAL(132, 3736, 42), // "on_actionCenter_of_Mass_selec..."
QT_MOC_LITERAL(133, 3779, 33), // "on_actionSet_view_range_trigg..."
QT_MOC_LITERAL(134, 3813, 29), // "on_actionY_coor_COM_triggered"
QT_MOC_LITERAL(135, 3843, 35), // "on_actionAtom_with_n_bond_tri..."
QT_MOC_LITERAL(136, 3879, 21), // "on_action3d_triggered"
QT_MOC_LITERAL(137, 3901, 23), // "on_actiondots_triggered"
QT_MOC_LITERAL(138, 3925, 37), // "on_actionExport_Plumed_File_t..."
QT_MOC_LITERAL(139, 3963, 28), // "on_actionadd_plane_triggered"
QT_MOC_LITERAL(140, 3992, 33), // "on_actionplane_by_index_trigg..."
QT_MOC_LITERAL(141, 4026, 28), // "on_actionadd_arrow_triggered"
QT_MOC_LITERAL(142, 4055, 43), // "on_actionEliminate_Repeated_A..."
QT_MOC_LITERAL(143, 4099, 30), // "on_actionto_position_triggered"
QT_MOC_LITERAL(144, 4130, 24), // "on_actionIndex_triggered"
QT_MOC_LITERAL(145, 4155, 33), // "on_actionmake_supercell_trigg..."
QT_MOC_LITERAL(146, 4189, 39) // "on_actionDownload_from_IZA_DB..."

    },
    "MainWindow\0on_actionOpen_triggered\0\0"
    "on_actionSave_as_triggered\0"
    "on_actionSave_triggered\0"
    "on_actionRender_PovRay_triggered\0"
    "on_sphereSlider_sliderMoved\0value\0"
    "on_atomradioSlider_sliderMoved\0"
    "on_actionSet_Unit_Cell_triggered\0"
    "on_actionFix_Selected_triggered\0"
    "on_actionunfix_selected_triggered\0"
    "on_actionInvert_Selection_triggered\0"
    "on_actionSelect_All_triggered\0"
    "on_actionSelect_by_bonds_triggered\0"
    "on_colorpushButton_clicked\0"
    "on_bondcylinderSlider_valueChanged\0"
    "on_qualitySlider_valueChanged\0"
    "on_rotationSlider_valueChanged\0"
    "on_opacitySlider_valueChanged\0"
    "on_actionCopy_triggered\0"
    "on_actionPaste_triggered\0"
    "on_actionClose_triggered\0on_combo_request\0"
    "on_openGLWidget_atoms_selected\0"
    "on_openGLWidget_atoms_deselected\0"
    "on_openGLWidget_data_changed\0"
    "on_actionplay_triggered\0"
    "on_tabWidget_tabCloseRequested\0index\0"
    "on_tabWidget_currentChanged\0"
    "on_tableWidget_itemChanged\0QTableWidgetItem*\0"
    "current\0on_tableWidget_rowSelected\0"
    "on_actionsticks_triggered\0"
    "on_actioncustom_triggered\0"
    "on_actionconnectivity_triggered\0"
    "on_actionundo_triggered\0on_actionredo_triggered\0"
    "on_actiontranslation_vector_triggered\0"
    "on_actionto_x_value_triggered\0"
    "on_actionto_y_value_triggered\0"
    "on_actionto_z_value_triggered\0"
    "on_actionfrequencies_triggered\0"
    "on_actionstop_triggered\0"
    "on_actionplay_frequencies_triggered\0"
    "on_freqtable_sectionClicked\0logicalindex\0"
    "on_actioncustom_selection_triggered\0"
    "on_actioninside_Cell_triggered\0"
    "on_actioncenter_triggered\0"
    "on_actionNew_triggered\0"
    "on_actionalldefaults_triggered\0"
    "on_actioncolors_triggered\0"
    "on_actionwire_triggered\0on_actionhide_triggered\0"
    "on_actionshow_all_triggered\0"
    "on_actionBackground_color_triggered\0"
    "on_actioncoordinates_triggered\0"
    "on_actionbond_distances_per_type_triggered\0"
    "on_actionbond_triggered\0"
    "on_actiontime_delay_triggered\0"
    "on_actionpeli_png_triggered\0"
    "on_actionaverage_animation_triggered\0"
    "on_actionexport_CV_triggered\0"
    "on_actiondihedral_triggered\0"
    "on_actionSelect_fixed_triggered\0"
    "on_actionsave_current_style_triggered\0"
    "on_actionload_style_triggered\0"
    "on_peli_listWidget_step_deleted\0"
    "vector<int>\0indices\0"
    "on_peli_listWidget_itemSelectionChanged\0"
    "on_actionx_y_and_z_triggered\0"
    "on_actionoriginal_triggered\0"
    "on_actionsphere_size_triggered\0"
    "on_actionradius_triggered\0"
    "on_shininessSlider_valueChanged\0"
    "on_actionInsideSphereAny_triggered\0"
    "on_actionInsideSphereOnly_Bonded_triggered\0"
    "on_actionInfo_triggered\0"
    "on_actionFilterByDistance_triggered\0"
    "on_actionSite_Type_triggered\0"
    "on_actionDistances_per_type_triggered\0"
    "on_actionsphere_triggered\0"
    "on_actiondeselect_triggered\0"
    "on_actionUnreplicate_triggered\0"
    "on_actionRestoreSphere_triggered\0"
    "on_actionRestoreRadio_triggered\0"
    "on_actionRestoreBond_Radio_triggered\0"
    "on_actionInsertFragment_triggered\0"
    "on_atomradioSlider_sliderPressed\0"
    "on_atomradioSlider_sliderReleased\0"
    "on_sphereSlider_sliderPressed\0"
    "on_sphereSlider_sliderReleased\0"
    "dragEnterEvent\0QDragEnterEvent*\0event\0"
    "dropEvent\0QDropEvent*\0"
    "on_actionSelectByElement_triggered\0"
    "on_actionOpenToolBar_triggered\0"
    "on_actionEditToolBar_toggled\0arg1\0"
    "on_actionShow_Cell_toggled\0"
    "on_actionSpecularToolBar_toggled\0"
    "on_actionSpecular_toggled\0"
    "on_actionEnable_disable_Light_toggled\0"
    "on_actionSaveToolBar_triggered\0"
    "on_actionChange_to_element_triggered\0"
    "on_actionAtoms_Type_triggered\0"
    "on_actionTrack_Coors_toggled\0"
    "on_actionGroups_triggered\0"
    "on_actionFrom_Selected_triggered\0"
    "on_actionSave_asToolBar_triggered\0"
    "on_actionFreqs_Animation_gif_triggered\0"
    "on_actionAlign_to_Bond_triggered\0"
    "on_actionImage_toggled\0"
    "on_sectionImageToolBar_toggled\0"
    "on_actionFrom_Same_Sigma_triggered\0"
    "on_actionnmr_triggered\0"
    "on_actionLoad_Style_all_tabs_triggered\0"
    "on_actionas_png_triggered\0"
    "on_actionAround_Z_axis_triggered\0"
    "on_actionAround_Bond_triggered\0"
    "on_actionStructure_from_Averages_triggered\0"
    "on_actionCorrect_periodic_trajectory_triggered\0"
    "on_actionangle_triggered\0checkMemoryUsage\0"
    "on_actionrings_of_size_triggered\0"
    "on_actionLoad_Style_from_state_rs_triggered\0"
    "on_actionCenter_of_Mass_selected_triggered\0"
    "on_actionSet_view_range_triggered\0"
    "on_actionY_coor_COM_triggered\0"
    "on_actionAtom_with_n_bond_triggered\0"
    "on_action3d_triggered\0on_actiondots_triggered\0"
    "on_actionExport_Plumed_File_triggered\0"
    "on_actionadd_plane_triggered\0"
    "on_actionplane_by_index_triggered\0"
    "on_actionadd_arrow_triggered\0"
    "on_actionEliminate_Repeated_Atoms_triggered\0"
    "on_actionto_position_triggered\0"
    "on_actionIndex_triggered\0"
    "on_actionmake_supercell_triggered\0"
    "on_actionDownload_from_IZA_DB_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
     134,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  684,    2, 0x08 /* Private */,
       3,    0,  685,    2, 0x08 /* Private */,
       4,    0,  686,    2, 0x08 /* Private */,
       5,    0,  687,    2, 0x08 /* Private */,
       6,    1,  688,    2, 0x08 /* Private */,
       8,    1,  691,    2, 0x08 /* Private */,
       9,    0,  694,    2, 0x08 /* Private */,
      10,    0,  695,    2, 0x08 /* Private */,
      11,    0,  696,    2, 0x08 /* Private */,
      12,    0,  697,    2, 0x08 /* Private */,
      13,    0,  698,    2, 0x08 /* Private */,
      14,    0,  699,    2, 0x08 /* Private */,
      15,    0,  700,    2, 0x08 /* Private */,
      16,    1,  701,    2, 0x08 /* Private */,
      17,    1,  704,    2, 0x08 /* Private */,
      18,    1,  707,    2, 0x08 /* Private */,
      19,    1,  710,    2, 0x08 /* Private */,
      20,    0,  713,    2, 0x08 /* Private */,
      21,    0,  714,    2, 0x08 /* Private */,
      22,    0,  715,    2, 0x08 /* Private */,
      23,    0,  716,    2, 0x08 /* Private */,
      24,    0,  717,    2, 0x08 /* Private */,
      25,    0,  718,    2, 0x08 /* Private */,
      26,    0,  719,    2, 0x08 /* Private */,
      27,    0,  720,    2, 0x08 /* Private */,
      28,    1,  721,    2, 0x08 /* Private */,
      30,    0,  724,    2, 0x08 /* Private */,
      31,    1,  725,    2, 0x08 /* Private */,
      34,    0,  728,    2, 0x08 /* Private */,
      35,    0,  729,    2, 0x08 /* Private */,
      36,    0,  730,    2, 0x08 /* Private */,
      37,    0,  731,    2, 0x08 /* Private */,
      38,    0,  732,    2, 0x08 /* Private */,
      39,    0,  733,    2, 0x08 /* Private */,
      40,    0,  734,    2, 0x08 /* Private */,
      41,    0,  735,    2, 0x08 /* Private */,
      42,    0,  736,    2, 0x08 /* Private */,
      43,    0,  737,    2, 0x08 /* Private */,
      44,    0,  738,    2, 0x08 /* Private */,
      45,    0,  739,    2, 0x08 /* Private */,
      46,    0,  740,    2, 0x08 /* Private */,
      47,    1,  741,    2, 0x08 /* Private */,
      49,    0,  744,    2, 0x08 /* Private */,
      50,    0,  745,    2, 0x08 /* Private */,
      51,    0,  746,    2, 0x08 /* Private */,
      52,    0,  747,    2, 0x08 /* Private */,
      53,    0,  748,    2, 0x08 /* Private */,
      54,    0,  749,    2, 0x08 /* Private */,
      55,    0,  750,    2, 0x08 /* Private */,
      56,    0,  751,    2, 0x08 /* Private */,
      57,    0,  752,    2, 0x08 /* Private */,
      58,    0,  753,    2, 0x08 /* Private */,
      59,    0,  754,    2, 0x08 /* Private */,
      60,    0,  755,    2, 0x08 /* Private */,
      61,    0,  756,    2, 0x08 /* Private */,
      62,    0,  757,    2, 0x08 /* Private */,
      63,    0,  758,    2, 0x08 /* Private */,
      64,    0,  759,    2, 0x08 /* Private */,
      65,    0,  760,    2, 0x08 /* Private */,
      66,    0,  761,    2, 0x08 /* Private */,
      67,    0,  762,    2, 0x08 /* Private */,
      68,    0,  763,    2, 0x08 /* Private */,
      69,    0,  764,    2, 0x08 /* Private */,
      70,    1,  765,    2, 0x08 /* Private */,
      73,    0,  768,    2, 0x08 /* Private */,
      74,    0,  769,    2, 0x08 /* Private */,
      75,    0,  770,    2, 0x08 /* Private */,
      76,    0,  771,    2, 0x08 /* Private */,
      77,    0,  772,    2, 0x08 /* Private */,
      78,    1,  773,    2, 0x08 /* Private */,
      79,    0,  776,    2, 0x08 /* Private */,
      80,    0,  777,    2, 0x08 /* Private */,
      81,    0,  778,    2, 0x08 /* Private */,
      82,    0,  779,    2, 0x08 /* Private */,
      83,    0,  780,    2, 0x08 /* Private */,
      84,    0,  781,    2, 0x08 /* Private */,
      85,    0,  782,    2, 0x08 /* Private */,
      86,    0,  783,    2, 0x08 /* Private */,
      87,    0,  784,    2, 0x08 /* Private */,
      88,    0,  785,    2, 0x08 /* Private */,
      89,    0,  786,    2, 0x08 /* Private */,
      90,    0,  787,    2, 0x08 /* Private */,
      91,    0,  788,    2, 0x08 /* Private */,
      92,    0,  789,    2, 0x08 /* Private */,
      93,    0,  790,    2, 0x08 /* Private */,
      94,    0,  791,    2, 0x08 /* Private */,
      95,    0,  792,    2, 0x08 /* Private */,
      96,    1,  793,    2, 0x08 /* Private */,
      99,    1,  796,    2, 0x08 /* Private */,
     101,    0,  799,    2, 0x08 /* Private */,
     102,    0,  800,    2, 0x08 /* Private */,
     103,    1,  801,    2, 0x08 /* Private */,
     105,    1,  804,    2, 0x08 /* Private */,
     106,    1,  807,    2, 0x08 /* Private */,
     107,    1,  810,    2, 0x08 /* Private */,
     108,    1,  813,    2, 0x08 /* Private */,
     109,    0,  816,    2, 0x08 /* Private */,
     110,    0,  817,    2, 0x08 /* Private */,
     111,    0,  818,    2, 0x08 /* Private */,
     112,    1,  819,    2, 0x08 /* Private */,
     113,    0,  822,    2, 0x08 /* Private */,
     114,    0,  823,    2, 0x08 /* Private */,
     115,    0,  824,    2, 0x08 /* Private */,
     116,    0,  825,    2, 0x08 /* Private */,
     117,    0,  826,    2, 0x08 /* Private */,
     118,    1,  827,    2, 0x08 /* Private */,
     119,    1,  830,    2, 0x08 /* Private */,
     120,    0,  833,    2, 0x08 /* Private */,
     121,    0,  834,    2, 0x08 /* Private */,
     122,    0,  835,    2, 0x08 /* Private */,
     123,    0,  836,    2, 0x08 /* Private */,
     124,    0,  837,    2, 0x08 /* Private */,
     125,    0,  838,    2, 0x08 /* Private */,
     126,    0,  839,    2, 0x08 /* Private */,
     127,    0,  840,    2, 0x08 /* Private */,
     128,    0,  841,    2, 0x08 /* Private */,
     129,    0,  842,    2, 0x08 /* Private */,
     130,    0,  843,    2, 0x08 /* Private */,
     131,    0,  844,    2, 0x08 /* Private */,
     132,    0,  845,    2, 0x08 /* Private */,
     133,    0,  846,    2, 0x08 /* Private */,
     134,    0,  847,    2, 0x08 /* Private */,
     135,    0,  848,    2, 0x08 /* Private */,
     136,    0,  849,    2, 0x08 /* Private */,
     137,    0,  850,    2, 0x08 /* Private */,
     138,    0,  851,    2, 0x08 /* Private */,
     139,    0,  852,    2, 0x08 /* Private */,
     140,    0,  853,    2, 0x08 /* Private */,
     141,    0,  854,    2, 0x08 /* Private */,
     142,    0,  855,    2, 0x08 /* Private */,
     143,    0,  856,    2, 0x08 /* Private */,
     144,    0,  857,    2, 0x08 /* Private */,
     145,    0,  858,    2, 0x08 /* Private */,
     146,    0,  859,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 32,   33,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   48,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 71,   72,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 97,   98,
    QMetaType::Void, 0x80000000 | 100,   98,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  104,
    QMetaType::Void, QMetaType::Bool,  104,
    QMetaType::Void, QMetaType::Bool,  104,
    QMetaType::Void, QMetaType::Bool,  104,
    QMetaType::Void, QMetaType::Bool,  104,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  104,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  104,
    QMetaType::Void, QMetaType::Bool,  104,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_actionOpen_triggered(); break;
        case 1: _t->on_actionSave_as_triggered(); break;
        case 2: _t->on_actionSave_triggered(); break;
        case 3: _t->on_actionRender_PovRay_triggered(); break;
        case 4: _t->on_sphereSlider_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_atomradioSlider_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_actionSet_Unit_Cell_triggered(); break;
        case 7: _t->on_actionFix_Selected_triggered(); break;
        case 8: _t->on_actionunfix_selected_triggered(); break;
        case 9: _t->on_actionInvert_Selection_triggered(); break;
        case 10: _t->on_actionSelect_All_triggered(); break;
        case 11: _t->on_actionSelect_by_bonds_triggered(); break;
        case 12: _t->on_colorpushButton_clicked(); break;
        case 13: _t->on_bondcylinderSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_qualitySlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_rotationSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_opacitySlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_actionCopy_triggered(); break;
        case 18: _t->on_actionPaste_triggered(); break;
        case 19: _t->on_actionClose_triggered(); break;
        case 20: _t->on_combo_request(); break;
        case 21: _t->on_openGLWidget_atoms_selected(); break;
        case 22: _t->on_openGLWidget_atoms_deselected(); break;
        case 23: _t->on_openGLWidget_data_changed(); break;
        case 24: _t->on_actionplay_triggered(); break;
        case 25: _t->on_tabWidget_tabCloseRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->on_tabWidget_currentChanged(); break;
        case 27: _t->on_tableWidget_itemChanged((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 28: _t->on_tableWidget_rowSelected(); break;
        case 29: _t->on_actionsticks_triggered(); break;
        case 30: _t->on_actioncustom_triggered(); break;
        case 31: _t->on_actionconnectivity_triggered(); break;
        case 32: _t->on_actionundo_triggered(); break;
        case 33: _t->on_actionredo_triggered(); break;
        case 34: _t->on_actiontranslation_vector_triggered(); break;
        case 35: _t->on_actionto_x_value_triggered(); break;
        case 36: _t->on_actionto_y_value_triggered(); break;
        case 37: _t->on_actionto_z_value_triggered(); break;
        case 38: _t->on_actionfrequencies_triggered(); break;
        case 39: _t->on_actionstop_triggered(); break;
        case 40: _t->on_actionplay_frequencies_triggered(); break;
        case 41: _t->on_freqtable_sectionClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->on_actioncustom_selection_triggered(); break;
        case 43: _t->on_actioninside_Cell_triggered(); break;
        case 44: _t->on_actioncenter_triggered(); break;
        case 45: _t->on_actionNew_triggered(); break;
        case 46: _t->on_actionalldefaults_triggered(); break;
        case 47: _t->on_actioncolors_triggered(); break;
        case 48: _t->on_actionwire_triggered(); break;
        case 49: _t->on_actionhide_triggered(); break;
        case 50: _t->on_actionshow_all_triggered(); break;
        case 51: _t->on_actionBackground_color_triggered(); break;
        case 52: _t->on_actioncoordinates_triggered(); break;
        case 53: _t->on_actionbond_distances_per_type_triggered(); break;
        case 54: _t->on_actionbond_triggered(); break;
        case 55: _t->on_actiontime_delay_triggered(); break;
        case 56: _t->on_actionpeli_png_triggered(); break;
        case 57: _t->on_actionaverage_animation_triggered(); break;
        case 58: _t->on_actionexport_CV_triggered(); break;
        case 59: _t->on_actiondihedral_triggered(); break;
        case 60: _t->on_actionSelect_fixed_triggered(); break;
        case 61: _t->on_actionsave_current_style_triggered(); break;
        case 62: _t->on_actionload_style_triggered(); break;
        case 63: _t->on_peli_listWidget_step_deleted((*reinterpret_cast< vector<int>(*)>(_a[1]))); break;
        case 64: _t->on_peli_listWidget_itemSelectionChanged(); break;
        case 65: _t->on_actionx_y_and_z_triggered(); break;
        case 66: _t->on_actionoriginal_triggered(); break;
        case 67: _t->on_actionsphere_size_triggered(); break;
        case 68: _t->on_actionradius_triggered(); break;
        case 69: _t->on_shininessSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 70: _t->on_actionInsideSphereAny_triggered(); break;
        case 71: _t->on_actionInsideSphereOnly_Bonded_triggered(); break;
        case 72: _t->on_actionInfo_triggered(); break;
        case 73: _t->on_actionFilterByDistance_triggered(); break;
        case 74: _t->on_actionSite_Type_triggered(); break;
        case 75: _t->on_actionDistances_per_type_triggered(); break;
        case 76: _t->on_actionsphere_triggered(); break;
        case 77: _t->on_actiondeselect_triggered(); break;
        case 78: _t->on_actionUnreplicate_triggered(); break;
        case 79: _t->on_actionRestoreSphere_triggered(); break;
        case 80: _t->on_actionRestoreRadio_triggered(); break;
        case 81: _t->on_actionRestoreBond_Radio_triggered(); break;
        case 82: _t->on_actionInsertFragment_triggered(); break;
        case 83: _t->on_atomradioSlider_sliderPressed(); break;
        case 84: _t->on_atomradioSlider_sliderReleased(); break;
        case 85: _t->on_sphereSlider_sliderPressed(); break;
        case 86: _t->on_sphereSlider_sliderReleased(); break;
        case 87: _t->dragEnterEvent((*reinterpret_cast< QDragEnterEvent*(*)>(_a[1]))); break;
        case 88: _t->dropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 89: _t->on_actionSelectByElement_triggered(); break;
        case 90: _t->on_actionOpenToolBar_triggered(); break;
        case 91: _t->on_actionEditToolBar_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 92: _t->on_actionShow_Cell_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 93: _t->on_actionSpecularToolBar_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 94: _t->on_actionSpecular_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 95: _t->on_actionEnable_disable_Light_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 96: _t->on_actionSaveToolBar_triggered(); break;
        case 97: _t->on_actionChange_to_element_triggered(); break;
        case 98: _t->on_actionAtoms_Type_triggered(); break;
        case 99: _t->on_actionTrack_Coors_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 100: _t->on_actionGroups_triggered(); break;
        case 101: _t->on_actionFrom_Selected_triggered(); break;
        case 102: _t->on_actionSave_asToolBar_triggered(); break;
        case 103: _t->on_actionFreqs_Animation_gif_triggered(); break;
        case 104: _t->on_actionAlign_to_Bond_triggered(); break;
        case 105: _t->on_actionImage_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 106: _t->on_sectionImageToolBar_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 107: _t->on_actionFrom_Same_Sigma_triggered(); break;
        case 108: _t->on_actionnmr_triggered(); break;
        case 109: _t->on_actionLoad_Style_all_tabs_triggered(); break;
        case 110: _t->on_actionas_png_triggered(); break;
        case 111: _t->on_actionAround_Z_axis_triggered(); break;
        case 112: _t->on_actionAround_Bond_triggered(); break;
        case 113: _t->on_actionStructure_from_Averages_triggered(); break;
        case 114: _t->on_actionCorrect_periodic_trajectory_triggered(); break;
        case 115: _t->on_actionangle_triggered(); break;
        case 116: _t->checkMemoryUsage(); break;
        case 117: _t->on_actionrings_of_size_triggered(); break;
        case 118: _t->on_actionLoad_Style_from_state_rs_triggered(); break;
        case 119: _t->on_actionCenter_of_Mass_selected_triggered(); break;
        case 120: _t->on_actionSet_view_range_triggered(); break;
        case 121: _t->on_actionY_coor_COM_triggered(); break;
        case 122: _t->on_actionAtom_with_n_bond_triggered(); break;
        case 123: _t->on_action3d_triggered(); break;
        case 124: _t->on_actiondots_triggered(); break;
        case 125: _t->on_actionExport_Plumed_File_triggered(); break;
        case 126: _t->on_actionadd_plane_triggered(); break;
        case 127: _t->on_actionplane_by_index_triggered(); break;
        case 128: _t->on_actionadd_arrow_triggered(); break;
        case 129: _t->on_actionEliminate_Repeated_Atoms_triggered(); break;
        case 130: _t->on_actionto_position_triggered(); break;
        case 131: _t->on_actionIndex_triggered(); break;
        case 132: _t->on_actionmake_supercell_triggered(); break;
        case 133: _t->on_actionDownload_from_IZA_DB_triggered(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 134)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 134;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 134)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 134;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
