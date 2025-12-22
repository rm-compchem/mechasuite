#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow() :
//    QMainWindow(parent),
    QMainWindow(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    setWindowTitle(tr("RMOL. Have fun building cool Chemical Structures!!!"));
//    QList list
    QList<int> lista = {800,300};
    ui->splitter->setSizes(lista);
//    showMaximized();
    read_settings();
    // Set combo box of elements
    for(auto i:element_symbols){
        ui->comboBox->addItem(QString::fromStdString(i));
    }
    ui->tabWidget->setTabsClosable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    QString selfilter = tr("XYZ (*.xyz)");
    QString filepath = QFileDialog::getOpenFileName(
                this,
                "Open file",
                "",
                tr("All files (*.*);;xyz (*.xyz);;"
                   "VASP FORMAT (POSCAR* CONTCAR* OUTCAR*);;"
                   "cif (*.cif);;cml (*.cml);;rf (*.rf)"),
                &selfilter
                                                    );

    if(filepath.isEmpty()){
        return;
    }
    openFile(filepath);
}

void MainWindow::openFile(QString filePath){
    setCursor(Qt::WaitCursor);
    QString filename = QFileInfo(filePath).fileName();

    if(cgl->st->atoms.size() > 0){
        QGLBegin *page;
        page = new QGLBegin(this, ui->statusBar);
        gl.push_back(page);
        ui->tabWidget->addTab(page, filename);
        int in = gl.size()-1;

        //connect slot with signal for new widget----------------------------
        connect(
                    gl[in], &QGLBegin::comboRequest,
                    this, &MainWindow::on_combo_request
                    );
        connect(
                    gl[in], &QGLBegin::atoms_selected,
                    this, &MainWindow::on_openGLWidget_atoms_selected
                    );
        connect(
                    gl[in], &QGLBegin::atoms_deselected,
                    this, &MainWindow::on_openGLWidget_atoms_deselected
                    );
        connect(
                    gl[in], &QGLBegin::data_changed,
                    this, &MainWindow::on_openGLWidget_data_changed
                    );
        connect(
                    gl[in]->st, &Struct::data_changed,
                    this, &MainWindow::on_openGLWidget_data_changed
                    );
        //--------------------------------------------------------------------
        load_data(filePath, gl[in]);
    }else{
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), filename);
        load_data(filePath);
        tableWidget_update();
        //update the qlistwidget ---------------------------------
        ui->peli_listWidget->update_data(cgl->st->pelisize);
        //update the qlistwidget ---------------------------------
    }
    setCursor(Qt::ArrowCursor);
}

void MainWindow::load_data(QString filename, QGLBegin *glwidget){
    QDir dir(filename);
    if(!dir.isReadable()){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("File could not be read");
        msg->exec();
        return;
    }

    if(glwidget == NULL){
        glwidget = cgl;
    }
    if(filename.endsWith(".rs")){
        glwidget->load_rf(filename);
    }
    else{
        glwidget->st->load_data(filename);
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString selfilter = tr("XYZ (*.xyz)");
    QString file = QFileDialog::getSaveFileName(
                this,
                "Save As",
                "",
                tr("All files (*.*);;xyz (*.xyz);;"
                   "VASP FORMAT (POSCAR*);;"
                   "png (*png);; STATE (*.rs)"),
                &selfilter
                );

    if(file.isEmpty()) return;

    if(file.endsWith(".png") || file.endsWith(".jpg")){
        setCursor(Qt::WaitCursor);
        save_picture(file);
        setCursor(Qt::ArrowCursor);
        return;
    }

    cgl->st->current_file = file;
    if(file.contains("POSCAR") ||  file.contains("CONTCAR")){
        QStringList list = {"Selective dynamics", "Direct Coordinates", "VASP 4.x"};
        MultiChoiceDialog dialog(this, list);
        dialog.exec();
        if (!dialog.ok) return;

        if (dialog.items.count("Selective dynamics")==1){
            cgl->st->vasp.seldynamics = true;
        }
        else{
            cgl->st->vasp.seldynamics = false;
        }
        if (dialog.items.count("Direct Coordinates")==1){
            cgl->st->vasp.cartesian = false;
        }
        else{
            cgl->st->vasp.cartesian = true;
        }
        if (dialog.items.count("VASP 4.x")==1){
            cgl->st->vasp.old = true;
        }
        else{
            cgl->st->vasp.old= false;
        }
    }
    on_actionSave_triggered();
    // create an instance of qfileinfo to extract the name from the whole path and pass it to tabtext
    QFileInfo fi(file);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), fi.fileName());
}

void MainWindow::on_actionSave_triggered()
{
    cgl->st->reverse_rotation();

    if (cgl->st->current_file.contains(".com")){
        QMessageBox msgBox;
        msgBox.setText("Fixed atoms??");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok){
            cgl->st->gaussian.fixed = true;
        }
        else{
            cgl->st->gaussian.fixed = false;
        }
        cgl->st->save_gaussian_input(cgl->st->current_file);
    }
    else if(cgl->st->current_file.contains(".xyz")){
        if(cgl->st->pelisize>1){
            QMessageBox msg;
            msg.setText("This is a trajectory file. Do you wanto to save the whole trajctory?");
            msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msg.setDefaultButton(QMessageBox::Ok);
            int ret = msg.exec();
            if(ret == QMessageBox::Ok){
                cgl->st->save_xyz_trajectory(cgl->st->current_file);
            }else{
                cgl->st->save_xyz(cgl->st->current_file);
            }
        }else{
            cgl->st->save_xyz(cgl->st->current_file);
        }
    }
    else if(cgl->st->current_file.contains("POSCAR") || cgl->st->current_file.contains("CONTCAR")){
        cgl->st->save_poscar(cgl->st->current_file);
    }
    else if (cgl->st->current_file.endsWith(".rs")){
        cout<<"savinng rf "<<cgl->st->current_file.toStdString()<<endl;
        cgl->save_rf(cgl->st->current_file);
    }
    else{
        cgl->st->save_pwi(cgl->st->current_file);
    }
}

void MainWindow::save_picture(QString filename){
    bool ok;

    QString scale = QInputDialog::getText(this, tr("Scale Factor"),
                                          tr("Enter Scale Factor for Image size"),
                                          QLineEdit::Normal, "", &ok);
    if(scale.isEmpty() || !ok){
        return;
    }
    float sfactor = scale.toFloat();

    QGLBegin glpng(this);
    glpng.rRange = cgl->rRange;
    glpng.rRangeVP = cgl->rRangeVP;
    glpng.lRange = cgl->lRange;
    glpng.lRangeVP = cgl->lRangeVP;
    glpng.dRange = cgl->dRange;
    glpng.dRangeVP = cgl->dRangeVP;
    glpng.uRange = cgl->uRange;
    glpng.uRangeVP = cgl->uRangeVP;
    glpng.st = cgl->st;
    glpng.quality = cgl->quality;
    glpng.scatterplot_mode = cgl->scatterplot_mode;

    for(unsigned int i=0;i<3;i++){
        glpng.bc[i] = cgl->bc[i];
    }
    for(unsigned int i=0;i<3;i++){
        glpng.specref[i] = cgl->specref[i];
    }
    for(unsigned int i=0;i<3;i++){
        glpng.specular[i] = cgl->specular[i];
    }

    glpng.shininess = cgl->shininess;

    glpng.light_enabled = cgl->light_enabled;

    glpng.material_enabled = cgl->material_enabled;

    glpng.resize(cgl->width()*sfactor,  cgl->height()*sfactor);
    glpng.cropX1 = cgl->cropX1;
    glpng.cropY1 = cgl->cropY1;
    glpng.cropX2 = cgl->cropX2;
    glpng.cropY2 = cgl->cropY2;

    glpng.onSize();

    glpng.show();
    glpng.draw();
    glpng.hide();

    QImage image(cgl->width()*sfactor,cgl->width()*sfactor,QImage::Format_RGB32);

     image = glpng.grabFramebuffer();
     if (cgl->cropMode){
         image = image.copy(QRect(glpng.cropX1*sfactor,
                                  glpng.cropY1*sfactor,
                                  (glpng.cropX2-glpng.cropX1)*sfactor,
                                  (glpng.cropY2-glpng.cropY1)*sfactor));
     }
//    image.save(filename, "PNG", 100);
     image.save(filename);

}

void MainWindow::on_actionRender_PovRay_triggered()
{
    int in = ui->tabWidget->currentIndex();
    QString file = QFileDialog::getSaveFileName();
    gl[in]->renderpovray(file.toStdString());
}

void MainWindow::on_sphereSlider_sliderMoved(int value)
{
    float radio;
    radio = value * 0.01;
    cgl->st->update_spheres(radio);
    cgl->update();
}

void MainWindow::on_atomradioSlider_sliderMoved(int value)
{
    float radio;
    radio = value * 0.01;
    cgl->st->update_radios(radio, false);
    cgl->update();
}

void MainWindow::on_actionSet_Unit_Cell_triggered()
{
    QString current_cell;
    for(size_t i=0;i<3;i++){
        for(size_t j=0;j<3;j++){
            current_cell += QString::number(cgl->st->cell[i][j], 'f', 5) + "     ";
        }
        current_cell += "\n";
    }

    QStringList lineas, templineas;
    vector<float> valores;
    bool ok;
    QString text = QInputDialog::getMultiLineText(this, tr("Unit cell"),
                                                     tr(""), current_cell, &ok);
    if (!ok || text.isEmpty()) return;

//    lineas = split(text.toStdString(), '\n');
    lineas = text.split("\n", QString::SkipEmptyParts);
    for(auto &l:lineas){
        templineas = l.split(" ", QString::SkipEmptyParts);
        for(auto &tl:templineas){
            valores.push_back(tl.toFloat());
        }
    }
    if(valores.size() == 6){
        cgl->st->set_unit_cell(valores[0], valores[1], valores[2],
                                valores[3], valores[4], valores[5]);
    }
    else if(valores.size() == 9){
        float lat[3][3];
        for(unsigned int i = 0; i<3;++i){
            for(unsigned int j = 0; j < 3; j++){
                lat[i][j] = valores[3*i+j];
            }
        }
         cgl->st->set_unit_cell(lat);
    }



        //-----------shift cell lines------------
        for(unsigned int i=0;i<24;i++){
            cgl->st->cell_lines[i][0] -= cgl->st->Xcm;
            cgl->st->cell_lines[i][1] -= cgl->st->Ycm;
            cgl->st->cell_lines[i][2] -= cgl->st->Zcm;
        }
        //-----------shift cell lines------------

        cgl->st->glob_rotate(cgl->st->cell_lines, cgl->st->trotmat);
        cgl->st->showcell = true;
    //ojo tengo que arreglar lo de las celdas, estoy no funcionaaa
    cgl->update();
}

void MainWindow::on_actionFix_Selected_triggered()
{
    int in = ui->tabWidget->currentIndex();

    for(auto i:gl[in]->st->selat){
        i->fixed = {true, true, true};
    }
}

void MainWindow::on_actionunfix_selected_triggered()
{
    int in = ui->tabWidget->currentIndex();
    for(auto i:gl[in]->st->selat){
        i->fixed = {false, false, false};
    }
}

void MainWindow::on_actionInvert_Selection_triggered()
{
    cgl->st->invert_selection();
    cgl->update();
}


void MainWindow::on_actionSelect_All_triggered()
{
    int in = ui->tabWidget->currentIndex();
    gl[in]->st->select_all();
    gl[in]->update();
}

void MainWindow::on_actionSelect_by_bonds_triggered()
{

    if(cgl->st->selat.size() < 1) return;
    vector<int> bonded;
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter Bonds"),
                                         tr("Bonds:"), QLineEdit::Normal,
                                        tr(""),  &ok);
    if (ok && !text.isEmpty()){
        int bonds = text.toInt();
//        int sa = cgl->st->selat[cgl->st->selat.size()-1]->index; //puedo coger varios indices en vez de uno
        for(auto sel:cgl->st->selat){
           bonded.push_back(sel->index);
        }
        bonded = cgl->st->reversed_bond_search(bonds, bonded);
        cgl->st->unselect_all();
        //tengo que quitar sel true de los atoms
        for(std::size_t i = 0, max = bonded.size(); i != max; ++i){
//            auto it = std::find(cgl->st->selat.begin(), cgl->st->selat.end(), &cgl->st->atoms[bonded[i]]);
//            if(it != cgl->st->selat.end()){
//                continue;
//            }
            cgl->st->atoms[bonded[i]].sel = true;
            cgl->st->selat.push_back(&cgl->st->atoms[bonded[i]]);
        }
        cgl->update();
    }
}

void MainWindow::on_colorpushButton_clicked()
{
    QColor buttoncolor = ui->colorpushButton->palette().button().color();

    QColor color = QColorDialog::getColor(buttoncolor);
    if(color.isValid()){
        float red, green, blue, sat=1.0;
        red = static_cast<float>(color.red())/255.0;
        green = static_cast<float>(color.green())/255.0;
        blue = static_cast<float>(color.blue())/255.0;
        sat = static_cast<float>(color.alpha())/255.0;
        cgl->st->update_color(red, green, blue, sat);
        cgl->update();
    }
}

void MainWindow::on_bondcylinderSlider_valueChanged(int value)
{
    int in = ui->tabWidget->currentIndex();
    float radio;
    radio = value * 0.01;
    gl[in]->st->update_cylinders(radio);
    gl[in]->update();
}

void MainWindow::on_qualitySlider_valueChanged(int value)
{
    int in = ui->tabWidget->currentIndex();

    gl[in]->quality = value;
    gl[in]->update();
}

void MainWindow::on_rotationSlider_valueChanged(int value)
{
    int in = ui->tabWidget->currentIndex();

    gl[in]->rot_ang = value * 0.01;
    gl[in]->update();
}

void MainWindow::on_opacitySlider_valueChanged(int value)
{
     int in = ui->tabWidget->currentIndex();

    float opacity;
    opacity = value/20.0;
    for(auto sel:gl[in]->st->selat){
        sel->color[3] = opacity;
    }

    //planes
    for(auto &p:cgl->st->planes){
        if(p.selected){
            p.color[3] = opacity;
        }
    }
       //planes
     gl[in]->update();
}

void MainWindow::on_actionCopy_triggered()
{
    Atom atcopy;
    clipboard.clear();
    for(auto sel:cgl->st->selat){
//        atcopy = *sel;
        atcopy = cgl->st->clone_atoms(*sel);
        atcopy.oind = -1;
        clipboard.push_back(atcopy);
    }
}

void MainWindow::on_actionPaste_triggered()
{
     cgl->st->paste_atoms(clipboard);
     cgl->update();
     tableWidget_update();
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::on_combo_request(){
    int in = ui->tabWidget->currentIndex();
    gl[in]->parentCombotxt = ui->comboBox->currentText();
}



void MainWindow::on_actionplay_triggered()
{
    cgl->playmovie();
}

void MainWindow::tableWidget_update(){
    cgl->st->reverse_rotation();
    cgl->st->round_finalcoors();
    ui->tableWidget->update_data(cgl->st->fatoms);
}

void MainWindow::on_openGLWidget_atoms_selected(){
     ui->tableWidget->clearSelection();
     int in = ui->tabWidget->currentIndex();
     Atom lsa = *gl[in]->st->selat.back(); //ls stands for last selected atom
     int sindex;
     QString element;

     if(cgl->st->selat.size()==1){
         ui->tableWidget->selectRow(cgl->st->selat[0]->index);
     }

    sindex = static_cast<int>(lsa.radio *100);
    ui->atomradioSlider->setSliderPosition(sindex);
    sindex = static_cast<int>(lsa.sphere*100);
    ui->sphereSlider->setSliderPosition(sindex);
    element = lsa.label;
    ui->comboBox->setCurrentText(element);

    QString bgc = QString("background-color: rgba( %1, %2, %3, %4);")
            .arg(lsa.color[0]*255).arg(lsa.color[1]*255).arg(lsa.color[2]*255).arg(lsa.color[3]*255);
    ui->colorpushButton->setStyleSheet(bgc);

    ui->statusBar->showMessage(cgl->parentLabelInfo);
}

void MainWindow::on_openGLWidget_atoms_deselected(){
    ui->tableWidget->clearSelection();
}

void MainWindow::on_openGLWidget_data_changed(){
    tableWidget_update();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if(index == 0 && ui->tabWidget->count()==1){
        gl[0]->clear();
        ui->tabWidget->setTabText(0, "Untitled");
        gl[0]->update();
        ui->actionShow_Cell->setChecked(false);
        tableWidget_update();
        return;
    }
    ui->tabWidget->removeTab(index);
    gl.erase(gl.begin()+index);
}

void MainWindow::on_tabWidget_currentChanged()
{
    ctab_index = ui->tabWidget->currentIndex();
    cgl = gl[ctab_index];
    if(cgl->editmode){
        ui->actionEditToolBar->setChecked(true);
    }else{
        ui->actionEditToolBar->setChecked(false);
    }
    if(cgl->st->valid_cell() && cgl->st->showcell){
        ui->actionShow_Cell->setChecked(true);
    }else{
        ui->actionShow_Cell->setChecked(false);
    }
    tableWidget_update();
    ui->freqtable->update_data(cgl->st->freqs);
    //update the qlistwidget ---------------------------------
    ui->peli_listWidget->update_data(cgl->st->pelisize);
    //update the qlistwidget ---------------------------------
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *itm)
{
    if(!ui->tableWidget->editing) return;
    ui->tableWidget->editing = false;

    int in = ui->tabWidget->currentIndex();
    if(itm->column() == 0){
        QString label = gl[in]->st->atoms[itm->row()].label;
        if(itm->text().compare(label) == 0){
            return;
        }

        //check if it is a valid simbol by searching in the global array element_symbols
        auto it = std::find(element_symbols.begin(), element_symbols.end(), itm->text().toStdString());
        if(it == element_symbols.end()){
            return;
        }
        gl[in]->st->update_atom_by_label(itm->row(),itm->text());
    }
    else{
        QTableWidgetItem *cooritm;
        array<float,3> refcoors = {0.0,0.0,0.0};
        cooritm = ui->tableWidget->item(itm->row(), 1);
        refcoors[0] = cooritm->text().toFloat();
        cooritm = ui->tableWidget->item(itm->row(), 2);
        refcoors[1] = cooritm->text().toFloat();
        cooritm = ui->tableWidget->item(itm->row(), 3);
        refcoors[2] = cooritm->text().toFloat();
        gl[in]->st->update_coor(refcoors, itm->row());
    }
    gl[in]->update();
}

void MainWindow::on_tableWidget_rowSelected(){
    int in = ui->tabWidget->currentIndex();
    //selected rows is a custom fuction/method I created in table class to know the selected rows
    gl[in]->st->add_to_selection(ui->tableWidget->selectedRows());
    gl[in]->update();
}

void MainWindow::on_actionsticks_triggered()
{
    int in = ui->tabWidget->currentIndex();
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter Radius"),
                                         tr("Radius: "), QLineEdit::Normal,
                                        tr(""),  &ok);
    if (!ok || text.isEmpty()){
        return;
    }
    float radius = text.toFloat();

    cgl->st->set_stick_view(radius);
    update();
}

void MainWindow::on_actioncustom_triggered()
{
    if(!cgl->st->valid_cell()){
        QMessageBox *msg = new QMessageBox();
        msg->setText("Unit Cell is not set\nPlease define a Unit Cell");
        msg->exec();
        return;
    }

    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter x y z"),
                                         tr("X Y Z: "), QLineEdit::Normal,
                                        tr(""),  &ok);
    if (!ok || text.isEmpty()){
        return;
    }
    QStringList values = text.split(" ", QString::SkipEmptyParts);
    if(values.size() != 3){
        QMessageBox *msg = new QMessageBox();
        msg->setText("Please enter three numbers ex: 2 2 2");
        msg->exec();
        return;
    }
    int x=values[0].toInt();
    int y=values[1].toInt();
    int z=values[2].toInt();
    if(x>10 || y>10 || z>10){
        QMessageBox *msg = new QMessageBox();
        msg->setText("Why do you event want to replicate that much??");
        msg->exec();
        return;
    }
    cgl->st->replicate_structure(x, y, z);
    cgl->update();
    tableWidget_update();
}


void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_I & event->modifiers() == Qt::ControlModifier & event->modifiers() == Qt::ShiftModifier){
        cout<<"modifffdjfs\n";
        on_actionInfo_triggered();
    }
    else if(event->key() == Qt::Key_W && event->modifiers() == Qt::ControlModifier){
        int index = ui->tabWidget->currentIndex();
        emit ui->tabWidget->tabCloseRequested(index);
    }
//    else if(event->key() == Qt::Key_I && event->modifiers() == Qt::ControlModifier){
//        int in = ui->tabWidget->currentIndex();
//        gl[in]->st->invert_selection();
//        gl[in]->update();
//    }
//    else if(event->key() == Qt::Key_E && event->modifiers() == Qt::ControlModifier){
//        on_actionSelectByElement_triggered();
//    }
    // the F group
//    else if(event->key() == Qt::Key_F2){
//        int in = ui->tabWidget->currentIndex();
//        if(gl[in]->editmode){
//            gl[in]->editmode = false;
//            ui->editcheckBox->setCheckState(Qt::Unchecked);
//        }
//        else{
//            gl[in]->editmode = true;
//            ui->editcheckBox->setCheckState(Qt::Checked);
//        }
//    }
    else if(event->key() == Qt::Key_Escape){
        if(cgl->draging){
            return;
        }
        cgl->stopmovie();
        cgl->st->unselect_all();
        // Is a way of say that the status bar will not be updated
        ui->tableWidget->clearSelection();
        cgl->update();
    }
    else if(event->key()==Qt::Key_0){
//        cgl->st->prueba();
        cgl->prueba();
        cgl->update();
        tableWidget_update();
    }
    else if(event->key()==Qt::Key_1 && cgl->hasFocus()){
        on_actionUnreplicate_triggered();
    }
    else if(event->key()==Qt::Key_PageUp){
        int in = ui->tabWidget->currentIndex();
        ui->tabWidget->setCurrentIndex(in-1);
    }
    else if(event->key()==Qt::Key_PageDown){
        int in = ui->tabWidget->currentIndex();
        ui->tabWidget->setCurrentIndex(in+1);
    }
}

void MainWindow::on_actionconnectivity_triggered()
{
    cgl->st->connectivity();
}

void MainWindow::on_actionundo_triggered()
{
    try{
        cgl->st->undo();
    }catch(...){
        QMessageBox msg(this);
        msg.setText("There was an unexected Error try to undo the changes!!! \nSorry about that!!");
        msg.exec();
        return;
    }

    cgl->update();
}

void MainWindow::on_actionredo_triggered()
{
    try{
        cgl->st->redo();
    }catch(...){
        QMessageBox msg(this);
        msg.setText("There was an unexected Error try to redo the changes!!! \nSorry about that!!");
        msg.exec();
        return;
    }
    cgl->update();

}

void MainWindow::on_actiontranslation_vector_triggered()
{
    bool ok;
    QString vector = QInputDialog::getText(this, "Tranlation ",
                                           "Enter Translation Vector",
                                           QLineEdit::Normal, "", &ok);
    if (!ok || vector.isEmpty()) return;

    QStringList vec = vector.split(" ", QString::SkipEmptyParts);
    float x = vec[0].toFloat();
    float y = vec[1].toFloat();
    float z = vec[2].toFloat();
    int in = ui->tabWidget->currentIndex();
    gl[in]->st->translate_selection(x, y, z);
    gl[in]->update();

}

void MainWindow::on_actionto_x_value_triggered()
{
    bool ok;
    QString vector = QInputDialog::getText(this, "Tranlation ",
                                           "Enter new x Coodinate: ",
                                           QLineEdit::Normal, "", &ok);
    if (!ok || vector.isEmpty()) return;

    QStringList vec = vector.split(" ", QString::SkipEmptyParts);
    int in = ui->tabWidget->currentIndex();
    float x = vec[0].toFloat();
    array<float,3> refcoors = {x,0.0,0.0};
    gl[in]->st->update_selection_coors(refcoors, 0);
    gl[in]->update();
}

void MainWindow::on_actionto_y_value_triggered()
{
    bool ok;
    QString vector = QInputDialog::getText(this, "Tranlation ",
                                           "Enter new y Coodinate: ",
                                           QLineEdit::Normal, "", &ok);
    if (!ok || vector.isEmpty()) return;

    QStringList vec = vector.split(" ", QString::SkipEmptyParts);
    int in = ui->tabWidget->currentIndex();
    float y = vec[0].toFloat();
    array<float,3> refcoors = {0.0,y,0.0};
    gl[in]->st->update_selection_coors(refcoors, 1);
    gl[in]->update();

}

void MainWindow::on_actionto_z_value_triggered()
{
    bool ok;
    QString vector = QInputDialog::getText(this, "Tranlation ",
                                           "Enter new Z Coodinate: ",
                                           QLineEdit::Normal, "", &ok);
    if (!ok || vector.isEmpty()) return;

    QStringList vec = vector.split(" ", QString::SkipEmptyParts);
    int in = ui->tabWidget->currentIndex();
    float z = vec[0].toFloat();
    array<float,3> refcoors = {0.0, 0.0, z};
    gl[in]->st->update_selection_coors(refcoors, 2);
    gl[in]->update();

}


void MainWindow::on_actionfrequencies_triggered()
{
    if(ui->freqtable->isHidden()){
        ui->freqtable->show();
    }
    else{
        ui->freqtable->hide();
    }
}

void MainWindow::on_actionstop_triggered()
{
    cgl->stopmovie();
}

void MainWindow::on_actionplay_frequencies_triggered()
{
    unsigned int in;
    QList<QTableWidgetItem*> itemlist = ui->freqtable->selectedItems();
    if(itemlist.size()>0){
        in = itemlist[0]->row();
    }
    else{
        in = 0;
    }
    if(cgl->st->freqs.size()==0){
        return;
    }
    cgl->playfreq(in);
}

void MainWindow::on_freqtable_sectionClicked(int logicalindex){
    if(cgl->timer->isActive() || !cgl->freqtimer->isActive()){
        return;
    }
    cgl->playfreq(logicalindex);
}

void MainWindow::on_actioncustom_selection_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter x y z"),
                                         tr("X Y Z: "), QLineEdit::Normal,
                                        tr(""),  &ok);
    if (!ok || text.isEmpty()){
        return;
    }
    QStringList values = text.split(" ", QString::SkipEmptyParts);
    int x=values[0].toInt();
    int y=values[1].toInt();
    int z=values[2].toInt();
    int in = ui->tabWidget->currentIndex();
    gl[in]->st->replicate_selection(x, y, z);
    gl[in]->update();
    tableWidget_update();
}

void MainWindow::on_actioninside_Cell_triggered()
{
    cgl->st->set_inv_mat();
    cgl->st->glob_rotate( cgl->st->selat,  cgl->st->totinvmat);
    cgl->st->unshift_coors( cgl->st->selat);
    cgl->st->frac_cart( cgl->st->selat, false);
    cgl->st->bring_inside_cell(true);
    cgl->st->frac_cart( cgl->st->selat, true);
    cgl->st->shift_coors( cgl->st->selat);
    cgl->st->glob_rotate( cgl->st->selat,  cgl->st->trotmat);
    cgl->st->genbonds();
    cgl->update();
}

void MainWindow::on_actioncenter_triggered()
{
    setCursor(Qt::WaitCursor);
    cgl->st->recenter();
    cgl->center();
    setCursor(Qt::ArrowCursor);
}

void MainWindow::on_actionNew_triggered()
{
    QGLBegin *page;
    page = new QGLBegin(this);
    gl.push_back(page);
    ui->tabWidget->addTab(page, "New Structure");
//    int in = gl.size()-1;
//    //gl[in]->st->load_data(filepath);
}

void MainWindow::on_actionalldefaults_triggered()
{
    cgl->st->set_default_view();
    cgl->update();
}

void MainWindow::on_actioncolors_triggered()
{
    cgl->st->set_default_color();
    cgl->update();
}

void MainWindow::on_actionwire_triggered()
{
    cgl->st->set_wire_view();
    cgl->update();
}

void MainWindow::on_actionhide_triggered()
{
    cgl->st->hide_selected();
    cgl->update();
}

void MainWindow::on_actionshow_all_triggered()
{
    cgl->st->showall();
    cgl->update();
}

void MainWindow::on_actionBackground_color_triggered()
{
    int in = ui->tabWidget->currentIndex();

    QColor color = QColorDialog::getColor();
    if(color.isValid()){
        // el problema de los colores raros esta en opengl no en qcolor
       gl[in]->bc[0] = color.red()/255.0;
       gl[in]->bc[1] = color.green()/255.0;
       gl[in]->bc[2] = color.blue()/255.0;
       gl[in]->bc[3] = color.hslSaturation()/255.0;
       gl[in]->update();
    }
}

void MainWindow::on_actioncoordinates_triggered()
{

    if(ui->tableWidget->isHidden()){
        ui->tableWidget->show();
        if(cgl->st->pelisize>1) ui->peli_listWidget->show();
    }
    else{
        ui->tableWidget->hide();
        ui->peli_listWidget->hide();
    }
}

void MainWindow::on_actionbond_distances_per_type_triggered()
{
    bool ok;
    QString line = QInputDialog::getText(this, "Distances", "Enter atom type", QLineEdit::Normal, "", &ok);
    if(!ok || line.isEmpty()) return;

    QStringList ats = line.split("-");
    if(ats.size()!=2){
        return;
    }
    QString distances;
    for(auto at:cgl->st->atoms){
        if(at.bonds.size()>0 && at.label.compare(ats[0])==0){
            for(auto b:at.bonds){
                if(cgl->st->atoms[b.neighbor].label.compare(ats[1])==0){
                    float d = dist(at.coor, cgl->st->atoms[b.neighbor].coor);
                    distances.append(QString::number(d));
                    distances.append("\n");
                }
            }
        }
        else if(at.bonds.size()>0 && at.label.compare(ats[1])==0){
            for(auto b:at.bonds){
                if(cgl->st->atoms[b.neighbor].label.compare(ats[0])==0){
                    float d = dist(at.coor, cgl->st->atoms[b.neighbor].coor);
                    distances.append(QString::number(d));
                    distances.append("\n");
                }
            }
        }
        cout <<distances.toStdString()<<endl;
    }
}

void MainWindow::on_actionbond_triggered()
{

    int code = 1;
    QString name;

    if(cgl->st->selat.size()!=2){
        QMessageBox *mg = new QMessageBox(this);
        mg->setText("Please select two atoms ");
        mg->exec();
        return;
    }

    while(code != 0){
        name = QInputDialog::getText(this, "Bond Name", "Enter Name");
        if(name.isEmpty()) return;

        code = cgl->st->newCollectiveVariable(name, "bond");
        if(code == 1){
            QMessageBox *mg = new QMessageBox(this);
            mg->setText("Name already exists !! Select another name");
            mg->exec();
        }
    }

}

void MainWindow::on_actiontime_delay_triggered()
{
    bool ok;
    QString td = QInputDialog::getText(this, "Time delay", "Enter Time delay (ms)", QLineEdit::Normal, "", &ok);
    if(!ok || td.isEmpty()) return;
    cgl->set_interval(td.toFloat());
}

void MainWindow::on_actionpeli_png_triggered()
{
   return;

//    bool ok;
//    QStringList framesData;
//    double sizePerImage;
//    while(framesData.size() != 3){
//        QString td = QInputDialog::getText(this, "Skip", "Enter fisrt, last and step frame(eg. 1 1000 2)", QLineEdit::Normal, "", &ok);
//        if(!ok || td.isEmpty()) return;
//        framesData = td.split(" ", QString::SkipEmptyParts);
//    }
//
//    int firstFrame = framesData[0].toInt();
//    int lastFrame = framesData[1].toInt();
//    int stepFrame = framesData[2].toInt();
//
//    if(lastFrame > cgl->st->pelisize){
//        QMessageBox *msg = new QMessageBox();
//        msg->setText("last step too large!!!");
//        msg->exec();
//        return;
//    }
//
//
//    QString scale = QInputDialog::getText(this, tr("Scale Factor"),
//                                          tr("Enter Scale Factor for Image size"),
//                                          QLineEdit::Normal, "", &ok);
//    if(scale.isEmpty() || !ok){
//        return;
//    }
//    float sfactor = scale.toFloat();
//
//    QString selfilter = tr("gif (*.gif)");
//    QString fileName = QFileDialog::getSaveFileName(
//                this,
//                "Export Gif Animation",
//                "",
//                tr("All files (*.*);;gif (*.gif);;"),
//                &selfilter
//                );
//
//    if(fileName.isEmpty()){
//        return;
//    }
//
//    //check for gif ending
//    if(!fileName.endsWith(".gif")){
//        fileName.append(".gif");
//    }
//    //check for gif ending
//
//    setCursor(Qt::WaitCursor);
//
//    QGLBegin glpng(this);
//    glpng.rRange = cgl->rRange;
//    glpng.rRangeVP = cgl->rRangeVP;
//    glpng.lRange = cgl->lRange;
//    glpng.lRangeVP = cgl->lRangeVP;
//    glpng.dRange = cgl->dRange;
//    glpng.dRangeVP = cgl->dRangeVP;
//    glpng.uRange = cgl->uRange;
//    glpng.uRangeVP = cgl->uRangeVP;
//    glpng.st = cgl->st;
//    glpng.quality = cgl->quality;
//
//    for(unsigned int i=0;i<3;i++){
//        glpng.bc[i] = cgl->bc[i];
//    }
//    for(unsigned int i=0;i<3;i++){
//        glpng.specref[i] = cgl->specref[i];
//    }
//    for(unsigned int i=0;i<3;i++){
//        glpng.specular[i] = cgl->specular[i];
//    }
//    glpng.shininess = cgl->shininess;
//    glpng.light_enabled = cgl->light_enabled;
//    glpng.material_enabled = cgl->material_enabled;
//
//    glpng.cropX1 = cgl->cropX1;
//    glpng.cropY1 = cgl->cropY1;
//    glpng.cropX2 = cgl->cropX2;
//    glpng.cropY2 = cgl->cropY2;
//    glpng.resize(cgl->width()*sfactor,  cgl->height()*sfactor);
//    glpng.onSize();
//
//    //check if ram memory can cope with size
//    updateMemoryUsage();
//    if(glpng.cropMode){
//        sizePerImage = (glpng.cropX2-glpng.cropX1)*sfactor*(glpng.cropY2-glpng.cropY1)*sfactor*0.00390625;  //0.00390625 = 4/1024.
//    }
//    else{
//        sizePerImage = glpng.width()*sfactor*glpng.height()*sfactor*0.00390625;
//    }
//    cout <<sizePerImage*((lastFrame-firstFrame)/stepFrame)<<endl;
//    cout <<availableMemory*0.8<<endl;
//    if(sizePerImage*((lastFrame-firstFrame)/stepFrame) >= availableMemory*0.8){
//        QMessageBox *msg = new QMessageBox(this);
//        msg->setText("Your do not have enough memory to create the GIF image\n");
//        msg->exec();
//        setCursor(Qt::ArrowCursor);
//        return;
//    }
//
//    //check if ram memory can cope with size
//
//    const GifskiSettings settings;
//    gifski *g = gifski_new(&settings);
//    gifski_set_file_output(g, fileName.toStdString().c_str());
//
//    for(unsigned int i=firstFrame; i<lastFrame; i += stepFrame){
//        glpng.st->shift_peli(i);
//        glpng.draw();
//        glpng.show();
//        glpng.hide();
//
//        QImage image(cgl->width(), cgl->width(), QImage::Format_RGB32);
//        image = glpng.grabFramebuffer();
//        if (cgl->cropMode){
//            image = image.copy(QRect(glpng.cropX1*sfactor,
//                                     glpng.cropY1*sfactor,
//                                     (glpng.cropX2-glpng.cropX1)*sfactor,
//                                     (glpng.cropY2-glpng.cropY1)*sfactor));
//        }
//
//
//        QString qstring_names = QString("%1_.png").arg(i, 4, 10, QChar('0'));
//        image.save(qstring_names);
//        const char* fnames = qstring_names.toStdString().c_str();
//        int res = gifski_add_frame_png_file(g, i, fnames, 5);
//        QFile::remove(qstring_names);
//     }
//    int res = gifski_finish(g);
//    setCursor(Qt::ArrowCursor);
}

void MainWindow::on_actionaverage_animation_triggered()
{
    bool ok;
    QString td = QInputDialog::getText(this, "Average", "Enter inteval", QLineEdit::Normal, "", &ok);
    if(!ok || td.isEmpty()) return;
    int interval = td.toInt();
    cgl->st->average_peli(interval);
}

void MainWindow::on_actionexport_CV_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "Enter File Name");
    if(file.isEmpty()) return;
    cgl->st->export_collective_variable(file);
}

void MainWindow::on_actiondihedral_triggered()
{
    int code = 1;
    QString name;

    if(cgl->st->selat.size()!=4){
        QMessageBox *mg = new QMessageBox(this);
        mg->setText("Please select 4 atoms ");
        mg->exec();
        return;
    }

    while(code != 0){
        name = QInputDialog::getText(this, "Dihedral Name", "Enter Name");
        if(name.isEmpty()) return;

        code = cgl->st->newCollectiveVariable(name, "dihedral");
        if(code == 1){
            QMessageBox *mg = new QMessageBox(this);
            mg->setText("Name already exists !! Select another name");
            mg->exec();
        }
    }
}


void MainWindow::on_actionSelect_fixed_triggered()
{
    cgl->st->select_fixed();
    cgl->update();
}

void MainWindow::on_actionsave_current_style_triggered()
{
    bool ok;
    QString stylename = QInputDialog::getText(this, tr("Style Name"),
                                              tr("Enter Name"),QLineEdit::Normal , "", &ok);

    if(!ok || stylename.isEmpty()) return;

    cgl->save_style(stylename);

}

void MainWindow::on_actionload_style_triggered()
{
    QDir dir(settings_folder);
    if(!dir.exists()){
        cout<<"cannot find settings folder\n";
       return;
    }

    QFileInfoList list = dir.entryInfoList(QStringList()<<"*.style", QDir::Files);
    QStringList listnames;
    for(auto l:list){
        listnames.append(l.fileName().remove(".style"));
    }
    SingleChoiceDialog dialog(this, listnames);
    dialog.exec();
    if(!dialog.ok || dialog.selected.isEmpty()) return;

    cgl->load_style(dialog.selected);
}

void MainWindow::on_peli_listWidget_step_deleted(vector<int> indices){
    cgl->st->del_peli_by_index(indices);
}

void MainWindow::on_peli_listWidget_itemSelectionChanged(){
    QList<QListWidgetItem*> selected = ui->peli_listWidget->selectedItems();
    if(selected.size()!=1) return;
    if(selected[0]->text().compare("Steps")==0) return;

    cgl->st->shift_peli(selected[0]->text().toInt()-1);
    QString status_msg = QString::number(cgl->st->peli_pos) + ", step: ";
    status_msg += QString::number(cgl->st->animation[cgl->st->peli_pos].step) + ", Time: ";
    status_msg += QString::number(cgl->st->animation[cgl->st->peli_pos].time) + ", Energy: ";
    status_msg += QString::number(cgl->st->animation[cgl->st->peli_pos].energy);
    ui->statusBar->showMessage(status_msg);

    cgl->update();
}

void MainWindow::on_actionx_y_and_z_triggered()
{
    if(!cgl->st->replicated){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("Structure is not replicated");
        msg->exec();
        return;
    }
    if(cgl->st->selat.size()==0){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("Please select at least one atom and try again");
        msg->exec();
        return;
    }

    cgl->st->select_replicated_copy();
}

void MainWindow::on_actionoriginal_triggered()
{
    if(!cgl->st->replicated){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("Structure is not replicated");
        msg->exec();
        return;
    }
    if(cgl->st->selat.size()==0){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("Please select at least one atom and try again");
        msg->exec();
        return;
    }
    cgl->st->select_original_copy();
}

void MainWindow::on_actionsphere_size_triggered()
{
    bool ok;
    QString radio = QInputDialog::getText(this, "Sphere ",
                                           "Enter new sphere size: ",
                                           QLineEdit::Normal, "", &ok);
    if (!ok || radio.isEmpty()) return;

    float newradio = radio.toFloat();
     cgl->st->update_spheres(newradio, true);
     cgl->update();
}

void MainWindow::on_actionradius_triggered()
{
    bool ok;
    QString radio = QInputDialog::getText(this, "Sphere ",
                                           "Enter new covalent radius: ",
                                           QLineEdit::Normal, "", &ok);
    if (!ok || radio.isEmpty()) return;

    float newradio = radio.toFloat();
     cgl->st->update_radios(newradio);
     cgl->update();
}


void MainWindow::on_shininessSlider_valueChanged(int value)
{
    cgl->shininess = value;
    cgl->update();
}

void MainWindow::read_settings(){
//    ifstream file1, file2, file3, file4, file5; // Open for reading
    QStringList lsplitted, lsplitted2;
    QString line, atomo, homepath;

    //define home path
    homepath = QDir::homePath();
    applicationFolder = QCoreApplication::applicationDirPath();
    //define settings folder where all config is stored
#ifdef Q_OS_UNIX
    settings_folder = homepath + "/.rmol/";
    fragmentsFolder = applicationFolder +"/fragments";
#elif defined (Q_OS_WIN32)
    settings_folder = homepath + "\\.rmol\\";
    fragmentsFolder = applicationFolder + "\\fragments\\";
#endif

    QDir dir(settings_folder);
    if(!dir.exists()){
        cout<<"cannot find settings folder\n";
       return;
    }

    cout<<"reading config files\n";
    // leo los radios
    QFile file1(settings_folder+".radii");
    if(!file1.open(QIODevice::ReadOnly)) {
//        QMessageBox::information(0, "error", file1.errorString());
         cout<<"error reading file radii\n ";
    }

    QTextStream in(&file1);

    while(!in.atEnd()) {
        line = in.readLine();
        lsplitted = line.split("=");
        radios[lsplitted[0]] = lsplitted[1].toFloat();
    }

    file1.close();


    // leo las esferas

    file1.setFileName(settings_folder+".spheres");
    if(!file1.open(QIODevice::ReadOnly)) {
//        QMessageBox::information(0, "error", file1.errorString());
         cout<<"error reading file spheres\n";
    }
    QTextStream in2(&file1);

    while(!in2.atEnd()) {
        line = in2.readLine();
        lsplitted = line.split("=");
        spheres[lsplitted[0]] = lsplitted[1].toFloat();
    }

    file1.close();

    //leo colores

    file1.setFileName(settings_folder+".colors");
    if(!file1.open(QIODevice::ReadOnly)) {
//        QMessageBox::information(0, "error", file1.errorString());
         cout<<"error reading file colors\n";
    }
    QTextStream in3(&file1);

    while(!in3.atEnd()) {
        line = in3.readLine();
        lsplitted = line.split("=");
        atomo = lsplitted[0];
        lsplitted2 = lsplitted[1].split(",");
        colors[atomo][0] = lsplitted2[0].toFloat();
        colors[atomo][1] = lsplitted2[1].toFloat();
        colors[atomo][2] = lsplitted2[2].toFloat();
        colors[atomo][3] = lsplitted2[3].toFloat();
    }

    file1.close();

        // leo los radios
    file1.setFileName(settings_folder+".cylinders");
    if(!file1.open(QIODevice::ReadOnly)) {
//        QMessageBox::information(0, "error", file1.errorString());
         cout<<"error reading file cylinders\n";
    }
    QTextStream in4(&file1);

    while(!in4.atEnd()) {
        line = in4.readLine();
        lsplitted = line.split("=");
        cylinders[lsplitted[0]] = lsplitted[1].toFloat();
    }

    file1.close();

    // leo las masas
    file1.setFileName(settings_folder+".masas");
    if(!file1.open(QIODevice::ReadOnly)) {
//        QMessageBox::information(0, "error", file1.errorString());
        cout<<"error reading file masas\n";
    }
    QTextStream in5(&file1);

    while(!in5.atEnd()) {
        line = in5.readLine();
        lsplitted = line.split("=");
        masas[lsplitted[0]] = lsplitted[1].toFloat();
    }

    file1.close();


    // leo las lineas de la celda--------------------------------------
   file1.setFileName(settings_folder+".cell");
    if(!file1.open(QIODevice::ReadOnly)) {
        //        QMessageBox::information(0, "error", file1.errorString());
        cout<<"error reading file .cell\n";
    }
    QTextStream in6(&file1);

    unsigned int cindex = 0;

    try{
        while(!in6.atEnd()) {
            line = in6.readLine();
            lsplitted = line.split(" ", QString::SkipEmptyParts);
            clw[cindex] = lsplitted[0].toFloat();
            clc[cindex][0] = lsplitted[1].toFloat();
            clc[cindex][1] = lsplitted[2].toFloat();
            clc[cindex][2] = lsplitted[3].toFloat();
            clc[cindex][3] = lsplitted[4].toFloat();
            cindex ++;
        }

    }catch(...){
            cout << "there was an error reading .cell file settings, bad format found!!";
            clw = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
            clc[0]= {1.0, 0.0, 0.0, 0.8};
            clc[1]= {0.0, 1.0, 0.0, 0.8};
            clc[2]= {0.0, 0.0, 1.0, 0.8};
            clc[3]= {0.0, 0.0, 0.0, 0.8};
            clc[4]= {0.0, 0.0, 0.0, 0.8};
            clc[5]= {0.0, 0.0, 0.0, 0.8};
            clc[6]= {0.0, 0.0, 0.0, 0.8};
            clc[7]= {0.0, 0.0, 0.0, 0.8};
            clc[8]= {0.0, 0.0, 0.0, 0.8};
            clc[9]= {0.0, 0.0, 0.0, 0.8};
            clc[10]= {0.0, 0.0, 0.0, 0.8};
            clc[11]= {0.0, 0.0, 0.0, 0.8};

        }

    file1.close();
}

void MainWindow::on_actionInsideSphereAny_triggered()
{
    if(cgl->st->selat.size() != 1) return;
    float radio;
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter radio"),
                                         tr("Radio:"), QLineEdit::Normal,
                                        tr(""),  &ok);
    if (ok && !text.isEmpty()){
        radio = text.toFloat();
        cgl->st->select_inside_sphere(radio);
    }
    cgl->update();
}

void MainWindow::on_actionInsideSphereOnly_Bonded_triggered()
{
    if(cgl->st->selat.size() != 1) return;
    float radio;
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter radio"),
                                         tr("Radio(A):"), QLineEdit::Normal,
                                        tr(""),  &ok);
    if (ok && !text.isEmpty()){
        radio = text.toFloat();
        cgl->st->select_inside_sphere_bonded(radio);
    }
    cgl->update();
}

void MainWindow::on_actionInfo_triggered()
{
    if(cgl->st->selat.size() == 0){
        return;
    }
    QString info;
    for(auto sel:cgl->st->selat){
        info += sel->getInfo();
        info += "-----------------------\n\n";
    }
//    Atom *sel = cgl->st->selat[0];
//    QString info = sel->getInfo();
    QMessageBox *msg = new QMessageBox(this);
    msg->setText(info);
    msg->open();
}

void MainWindow::on_actionFilterByDistance_triggered()
{
    if(cgl->st->selat.size() != 2) return;
    float low_umb, high_umb;
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter filter Range"),
                                         tr("Range:"), QLineEdit::Normal,
                                        tr(""),  &ok);
    if (!ok && text.isEmpty()){
       return;
    }
    QString filename = "movie.xyz";
    QFile file;
    unsigned int in1,in2;
    in1 = cgl->st->selat[0]->index;
    in2 = cgl->st->selat[1]->index;

    QStringList umbs = text.split("-", QString::SkipEmptyParts);
    low_umb = umbs[0].toFloat();
    high_umb = umbs[1].toFloat();
    vector<int> pindices;
    for(unsigned int i=0;i<cgl->st->pelisize;i++){
        float d = dist(cgl->st->atoms[in1].peli[i], cgl->st->atoms[in2].peli[i]);
        if(d>low_umb && d<high_umb){
            pindices.push_back(i);
        }
    }


//    QMessageBox *box = new QMessageBox("title", "text", QMessageBox::Save, QMessageBox::Close);
    QMessageBox *msg = new QMessageBox();
    msg->setText(QString::number(pindices.size())+" files will be created. Want to continue?");
    msg->addButton(QMessageBox::Save);
    msg->addButton(QMessageBox::Cancel);
    if(msg->exec() == QMessageBox::Cancel){
        return;
    }

    for(auto p:pindices){
        file.setFileName(QString::number(p)+"_"+filename);
        if(!file.open(QIODevice::WriteOnly)) {
            continue;
//            QMessageBox::information(0, "error", outfile.errorString());
        }
        QTextStream out(&file);
        out<<cgl->st->atoms.size()<<endl;
        out<<"Step = "<<cgl->st->animation[p].step;
        out<<",time = "<<cgl->st->animation[p].time;
        out<<",E ="<<cgl->st->animation[p].energy<<endl;
        for(auto at:cgl->st->atoms){
            out<<at.label<<"    ";
            for(unsigned int i=0;i<3;i++){
                out<<at.peli[p][i]+cgl->st->animation[p].com[i]<<"    ";
            }
            out<<endl;
        }
        file.close();

    }
}

void MainWindow::on_actionSite_Type_triggered()
{
    QStringList site_types;
    for(auto at:cgl->st->atoms){
        if(!site_types.contains(at.atom_site_label)){
            site_types.append(at.atom_site_label);
        }
    }
    MultiChoiceDialog2 *dialog = new MultiChoiceDialog2(this, site_types);
    dialog->exec();
    if(!dialog->ok){
        return;
    }
    for(auto itm:dialog->selectedItems){
        for(auto &at:cgl->st->atoms){
            if (at.atom_site_label.compare(itm)==0){
                at.sel = true;
                cgl->st->selat.push_back(&at);
            }
        }
    }
    cgl->update();
}

void MainWindow::on_actionDistances_per_type_triggered()
{
    bool ok;
    QString line = QInputDialog::getText(this, "Distances", "Enter atom type", QLineEdit::Normal, "", &ok);
    if(!ok || line.isEmpty()) return;

    QStringList ats = line.split("-");
    if(ats.size()!=2){
        return;
    }
    QString distances;
    QString outputtext;
    for(auto sel:cgl->st->selat){
        if(sel->label.compare(ats[0])==0){
            for(auto sel2:cgl->st->selat){
                if(sel2->label.compare(ats[1])==0){
                    float d = dist(sel->coor, sel2->coor);
                    outputtext = sel->label;
                    outputtext.append(QString::number(sel->index));
                    outputtext.append("-");
                    outputtext.append(sel2->label);
                    outputtext.append(QString::number(sel2->index));
                    outputtext.append(": ");
                    outputtext.append(QString::number(d));
                    distances.append(outputtext);
                    distances.append("\n");
                }
            }
        }
    }
    cout <<distances.toStdString()<<endl;
}

void MainWindow::on_actionsphere_triggered()
{
    bool ok;
    QString line = QInputDialog::getText(this, "Selection by Sphere", "Enter value or range(Ex 0.6, 0.6-0.8)", QLineEdit::Normal, "", &ok);
    if(!ok || line.isEmpty()) return;

    QStringList ss = line.split("-");
    if(ss.size()==1){
        float sphere = ss[0].toFloat();
        cgl->st->selectBySphere(sphere);
    }
    else if(ss.size()==2){
        float sphereRange[] = {ss[0].toFloat(), ss[1].toFloat()};
        cgl->st->selectBySphere(sphereRange);
    }
    else{
        return;
    }
    cgl->update();
}

void MainWindow::on_actiondeselect_triggered()
{
    cgl->st->selat.clear();
    //cgl->update();
}

void MainWindow::on_actionUnreplicate_triggered()
{
    if(cgl->st->replication[0]==1 && cgl->st->replication[1]==1 && cgl->st->replication[2]==1){
        return;
    }
    this->setCursor(Qt::WaitCursor);
    cgl->st->add_to_before("replication_all");
    cgl->st->unReplicate();
    tableWidget_update();
    cgl->update();
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::on_actionRestoreSphere_triggered()
{
    for(auto &sel:cgl->st->selat){
        sel->sphere = spheres[sel->label];
    }
    cgl->update();
}

void MainWindow::on_actionRestoreRadio_triggered()
{
    for(auto &sel:cgl->st->selat){
        sel->radio = radios[sel->label];
    }
    cgl->st->genbonds();
    cgl->update();
}

void MainWindow::on_actionRestoreBond_Radio_triggered()
{
    for(auto &sel:cgl->st->selat){
        sel->cyl = cylinders[sel->label];
    }
    cgl->st->genbonds();
    cgl->update();

}

void MainWindow::on_actionInsertFragment_triggered()
{

    QString filepath = QFileDialog::getOpenFileName(
                this,
                "Open file",
                fragmentsFolder,
                tr("cml (*.cml)"));

    if(filepath.isEmpty()){
        return;
    }
    setCursor(Qt::WaitCursor);
    cgl->st->appendFragmentCml(filepath);
    setCursor(Qt::ArrowCursor);
    cgl->update();
}

void MainWindow::on_atomradioSlider_sliderPressed()
{
    //quizas hacer clear aqui o dejarlo para el released???
    radioSliderVal = ui->atomradioSlider->value();
    if(cgl->st->selat.size()>0){
        for(auto sel:cgl->st->selat){
            oldRadiosSphereCyl.push_back(sel->radio);
        }
    }
}

void MainWindow::on_atomradioSlider_sliderReleased()
{
    if(ui->atomradioSlider->value() == radioSliderVal){
        oldRadiosSphereCyl.clear();
        return;
    }
    CTRLZ bef;
    bef.action = "change_radius";
    for(auto sel:cgl->st->selat){
        bef.indices.push_back(sel->index);
    }
    bef.radios = oldRadiosSphereCyl;
    cgl->st->beforedata.push_back(bef);
    oldRadiosSphereCyl.clear();
}


void MainWindow::on_sphereSlider_sliderPressed()
{
    //quizas hacer clear aqui o dejarlo para el released???
    sphereSliderVal = ui->sphereSlider->value();
    if(cgl->st->selat.size()>0){
        for(auto sel:cgl->st->selat){
            oldRadiosSphereCyl.push_back(sel->sphere);
        }
    }
}

void MainWindow::on_sphereSlider_sliderReleased()
{
    if(ui->sphereSlider->value() == sphereSliderVal){
        oldRadiosSphereCyl.clear();
        return;
    }
    CTRLZ bef;
    bef.action = "change_sphere";
    for(auto sel:cgl->st->selat){
        bef.indices.push_back(sel->index);
    }
    cout<<"changing spheres, adding to before\n";
    bef.spheres = oldRadiosSphereCyl;
    cgl->st->beforedata.push_back(bef);
    oldRadiosSphereCyl.clear();

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event){
    foreach (const QUrl &url, event->mimeData()->urls()){
        QString fileName = url.toLocalFile();
        openFile(fileName);
    }
    cgl->update();
}

void MainWindow::on_actionSelectByElement_triggered()
{
    QStringList elements;
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter Symbols"),
                                         tr("Elements:"), QLineEdit::Normal,
                                        tr(""),  &ok);
    if (ok && !text.isEmpty()){
         elements = text.split(QRegularExpression(" "));
         cgl->st->select_by_elements(elements);
         cgl->update();
    }
}

void MainWindow::on_actionOpenToolBar_triggered()
{
    on_actionOpen_triggered();
}

void MainWindow::on_actionEditToolBar_toggled(bool arg1)
{
    if(arg1){
        cgl->editmode = true;
    }else{
        cgl->editmode = false;
    }
}

void MainWindow::on_actionShow_Cell_toggled(bool arg1)
{
    if(!cgl->st->valid_cell()){
//        QMessageBox *msg = new QMessageBox(this);
//        msg->setText(tr("Current cell is not valid\nPlease enter new cell"));
//        msg->showNormal();
        ui->actionShow_Cell->setChecked(false);
        return;
    }
    if(arg1){
        cgl-> st->showcell = true;
    }else{
        cgl-> st->showcell = false;
    }
    cgl->update();
}

void MainWindow::on_actionSpecularToolBar_toggled(bool arg1)
{
    if(arg1){
        cgl->specular[0] = 0.8;
        cgl->specular[1] = 0.8;
        cgl->specular[2] = 0.8;
        cgl->specular[3] = 0.8;
    }
    else{
        cgl->specular[0] = 0.0;
        cgl->specular[1] = 0.0;
        cgl->specular[2] = 0.0;
        cgl->specular[3] = 0.0;
    }
//    cgl->enableGlobProp();
    //hay como un loop aqui
    ui->actionSpecular->setChecked(arg1);
    cgl->update();
}

void MainWindow::on_actionSpecular_toggled(bool arg1)
{
    on_actionSpecularToolBar_toggled(arg1);
    ui->actionSpecularToolBar->setChecked(arg1);
}

void MainWindow::on_actionEnable_disable_Light_toggled(bool arg1)
{
    if(arg1){
         cgl->light_enabled=true;
    }else{
         cgl->light_enabled=false;
    }

//    cgl->material_enabled = false;
    cgl->update();
}

void MainWindow::on_actionSaveToolBar_triggered()
{
    on_actionSave_triggered();
}

void MainWindow::on_actionChange_to_element_triggered()
{
    bool ok;
    QStringList checkElements;
    QString line;
    while(checkElements.size() != 1){
        line = QInputDialog::getText(this, "Change Element to", "Enter New Element Symbol)", QLineEdit::Normal, "", &ok);
        if(!ok || line.isEmpty()) return;
        checkElements = line.split(" ", QString::SkipEmptyParts);
        if(checkElements.size()!=1){
            QMessageBox *msg = new QMessageBox(this);
            msg->setText("Please enter only one Symbol");
            msg->exec();
        }
    }


    string element = line.replace(" ", "").toStdString();
    //this has to be change if more elements are included in elements
    for (unsigned int i=0;i<73;i++){
        if(element_symbols[i].compare(element)==0){
            cgl->st->changeElements(QString::fromStdString(element));
        }
    }
    tableWidget_update();
}

void MainWindow::on_actionAtoms_Type_triggered()
{
    cgl->st->invertSelectionAtomsType();
}

void MainWindow::parseArguments(int argc, char *argv[]){
    setCursor(Qt::WaitCursor);
    if(argc >1){
        for(int i = 1; i < argc; i++){
            QGLBegin *page;
            page = new QGLBegin(this, ui->statusBar);
            gl.push_back(page);
            QString filename = QFileInfo( argv[i]).fileName();
            ui->tabWidget->addTab(page, filename);
            load_data(argv[i], gl[i-1]);
        }
    }
    else{
        QGLBegin *page;
        page = new QGLBegin(this, ui->statusBar);
        gl.push_back(page);
        ui->tabWidget->addTab(page, tr("Untitled"));
    }

    //Esto luego lo tengo que poner en openFile

    for(unsigned int i=0; i<gl.size();i++){
        connect(
                    gl[i], &QGLBegin::comboRequest,
                    this, &MainWindow::on_combo_request
                    );
        connect(
                    gl[i], &QGLBegin::atoms_selected,
                    this, &MainWindow::on_openGLWidget_atoms_selected
                    );
        connect(
                    gl[i], &QGLBegin::atoms_deselected,
                    this, &MainWindow::on_openGLWidget_atoms_deselected
                    );
        connect(
                    gl[i], &QGLBegin::data_changed,
                    this, &MainWindow::on_openGLWidget_data_changed
                    );
        connect(
                    gl[i]->st, &Struct::data_changed,
                    this, &MainWindow::on_openGLWidget_data_changed
                    );
    }

    connect(ui->freqtable->verticalHeader(),SIGNAL(sectionClicked(int)), this, SLOT(on_freqtable_sectionClicked(int)));


    // set the current tab index and gl instance
    ctab_index = ui->tabWidget->currentIndex();
    cgl = gl[ctab_index];
    tableWidget_update();
    //set show cell
    if(cgl->st->showcell) ui->actionShow_Cell->setChecked(true);
    // update the freqtable
    ui->freqtable->update_data(cgl->st->freqs);
    //update the qlistwidget ---------------------------------
    ui->peli_listWidget->update_data(cgl->st->pelisize);
    setCursor(Qt::ArrowCursor);
}

void MainWindow::on_actionTrack_Coors_toggled(bool arg1)
{
    cgl->setTrackParam(arg1);
}

void MainWindow::on_actionGroups_triggered()
{
    QStringList groupsNames;
    for (auto g:cgl->st->groups){
        groupsNames.append(g.name);
    }

    MultiChoiceDialog2 dlg(this, groupsNames);
    dlg.exec();
    if(!dlg.ok){
        return;
    }
    for(auto name:dlg.selectedItems){
        for(auto g:cgl->st->groups){
            if(name==g.name){
                for(auto i:g.indices){
                    cgl->st->atoms[i].sel = true;
                    cgl->st->selat.push_back(&cgl->st->atoms[i]);
                }
            }
        }
    }
}

void MainWindow::on_actionFrom_Selected_triggered()
{
    int output = -1;
    if(cgl->st->selat.size()==0){
        return;
    }
    while(output != 0){
        bool ok;
        QString groupName = QInputDialog::getText(this, "Group Name", "Enter Group Name",
                                                  QLineEdit::Normal, "", &ok);
        if(groupName.isEmpty() || !ok){
            return;
        }
        output = cgl->st->createGroup(groupName);
        if(output == 1){
            QMessageBox *msg = new QMessageBox(this);
            msg->setText("There is a group with that name");
            msg->exec();
        }
    }
}

void MainWindow::on_actionSave_asToolBar_triggered()
{
    on_actionSave_as_triggered();
}

void MainWindow::on_actionFreqs_Animation_gif_triggered()
{
	return;
/*    bool ok;

    QString scale = QInputDialog::getText(this, tr("Scale Factor"),
                                          tr("Enter Scale Factor for Image size"),
                                          QLineEdit::Normal, "", &ok);
    if(scale.isEmpty() || !ok){
        return;
    }
    float sfactor = scale.toFloat();

    QString selfilter = tr("gif (*.gif)");
    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Exort Gif Animation",
                "",
                tr("All files (*.*);;gif (*.gif);;"),
                &selfilter
                );

    if(fileName.isEmpty()){
        return;
    }

    //check for gif ending
    if(!fileName.endsWith(".gif")){
        fileName.append(".gif");
    }
    //check for gif ending

    setCursor(Qt::WaitCursor);
    //check which freq mode is selected--------------------------------
    unsigned int in;
    QList<QTableWidgetItem*> itemlist = ui->freqtable->selectedItems();
    if(itemlist.size()>0){
        in = itemlist[0]->row();
    }
    else{
        in = 0;
    }
    if(cgl->st->freqs.size()==0){
        return;
    }
        //check which freq mode is selected----------------------------

    //create temp dir
    QString tempFolderName = ".temp_gif_rmol";
    QDir tempDir(tempFolderName);
//    cout <<tempDir.path().toStdString()<<endl;
    tempDir.makeAbsolute();
//    cout <<tempDir.path().toStdString()<<endl;
    if(!tempDir.exists()){
        tempDir.mkdir(tempDir.path());
    }
    //create temp dir

    QGLBegin glpng(this);
    glpng.rRange = cgl->rRange;
    glpng.rRangeVP = cgl->rRangeVP;
    glpng.lRange = cgl->lRange;
    glpng.lRangeVP = cgl->lRangeVP;
    glpng.dRange = cgl->dRange;
    glpng.dRangeVP = cgl->dRangeVP;
    glpng.uRange = cgl->uRange;
    glpng.uRangeVP = cgl->uRangeVP;
    glpng.st = cgl->st;
    glpng.quality = cgl->quality;

    for(unsigned int i=0;i<3;i++){
        glpng.bc[i] = cgl->bc[i];
    }
    for(unsigned int i=0;i<3;i++){
        glpng.specref[i] = cgl->specref[i];
    }
    for(unsigned int i=0;i<3;i++){
        glpng.specular[i] = cgl->specular[i];
    }
    glpng.shininess = cgl->shininess;
    glpng.light_enabled = cgl->light_enabled;
    glpng.material_enabled = cgl->material_enabled;

    glpng.cropX1 = cgl->cropX1;
    glpng.cropY1 = cgl->cropY1;
    glpng.cropX2 = cgl->cropX2;
    glpng.cropY2 = cgl->cropY2;

    glpng.resize(cgl->width()*sfactor,  cgl->height()*sfactor);
    glpng.onSize();

    glpng.st->cfreq = in;
    glpng.st->freq_pos = -1;
    glpng.st->freq_forward = true;
    for(unsigned int i=0;i<2*glpng.st->freqSteps;i++){
        glpng.st->shift_freq_desp();
        glpng.show();
        glpng.update();
        glpng.hide();

        QImage image(cgl->width()*sfactor, cgl->width()*sfactor,QImage::Format_RGB32);
         image = glpng.grabFramebuffer();
         if (cgl->cropMode){
             image = image.copy(QRect(glpng.cropX1*sfactor,
                                      glpng.cropY1*sfactor,
                                      (glpng.cropX2-glpng.cropX1)*sfactor,
                                      (glpng.cropY2-glpng.cropY1)*sfactor));
         }

        image.save(tempFolderName + QString("/%1_rmol_output_gif.png").arg(i, 4, 10, QChar('0')));
    }

        QString command = "convert -delay 3 -loop 0 " + tempFolderName+ "/0*_rmol_output_gif.png  " + fileName;
//        cout<<fileName.toStdString()<<endl;
        int gif = system(command.toStdString().c_str());
//        int gif = system("convert -delay 3 -loop 0 0*_rmol_output_gif.png  animation.gif");
        tempDir.removeRecursively();
        if(gif != 0){
            QMessageBox *msg = new QMessageBox(this);
            msg->setText("Gif could not be created\nCode error:"+QString::number(gif));
            msg->exec();
        }

    setCursor(Qt::ArrowCursor);
    */
}

void MainWindow::on_actionAlign_to_Bond_triggered()
{
    //Poner Qmessage si no hay 3 seleccionado
    bool ok;
    QString dStr = QInputDialog::getText(this, "Distance",
                                      "Enter Distance from atom",
                                      QLineEdit::Normal, "", &ok);
    if(!ok || dStr.isEmpty()){
        return;
    }

    cgl->st->alignToBond(dStr);
    cgl->update();

}

void MainWindow::on_actionImage_toggled(bool arg1)
{
    if (arg1){
        cgl->cropMode = true;
    }
    else{
        cgl->cropMode = false;
        cgl->update();
    }
}

void MainWindow::on_sectionImageToolBar_toggled(bool arg1)
{
    on_actionImage_toggled(arg1);
}

void MainWindow::on_actionFrom_Same_Sigma_triggered()
{
    //Poner Qmessage si no hay 3 seleccionado
    bool ok;
    QString dStr = QInputDialog::getText(this, "Tolerance",
                                      "Enter Tolerance",
                                      QLineEdit::Normal, "", &ok);
    if(!ok || dStr.isEmpty()){
        return;
    }

    float tol = dStr.toFloat();



}

void MainWindow::on_actionnmr_triggered()
{
    //Poner Qmessage si no hay 3 seleccionado
    bool ok;
    QString dStr = QInputDialog::getText(this, "NMR shielding",
                                      "Enter value",
                                      QLineEdit::Normal, "", &ok);
    if(!ok || dStr.isEmpty()){
        return;
    }
    float nmr = dStr.toFloat();

    cgl->st->selectByNMR(nmr);

}

void MainWindow::on_actionLoad_Style_all_tabs_triggered()
{
    QDir dir(settings_folder);
    if(!dir.exists()){
        cout<<"cannot find settings folder\n";
       return;
    }

    QFileInfoList list = dir.entryInfoList(QStringList()<<"*.style", QDir::Files);
    QStringList listnames;
    for(auto l:list){
        listnames.append(l.fileName().remove(".style"));
    }
    SingleChoiceDialog dialog(this, listnames);
    dialog.exec();
    if(!dialog.ok || dialog.selected.isEmpty()) return;


    for(unsigned int i=0;i<gl.size();i++){
         gl[i]->load_style(dialog.selected);
    }
}

void MainWindow::on_actionas_png_triggered()
{
    for(unsigned int i=0;i<gl.size();i++){
        cgl = gl[i];
        save_picture(cgl->st->current_file+".png");
    }
}

void MainWindow::on_actionAround_Z_axis_triggered()
{
    //Poner Qmessage si no hay 3 seleccionado
    bool ok;
    QString dStr = QInputDialog::getText(this, "Angle",
                                      "Enter Angle",
                                      QLineEdit::Normal, "", &ok);
    if(!ok || dStr.isEmpty()){
        return;
    }

    float ang = dStr.toFloat();
    ang = ang*3.14159265359/180.0;
    float m[3][3] = {{cos(ang), -sin(ang), 0}, {sin(ang), cos(ang), 0}, {0,0,1}};
    cgl->st->glob_rotate(cgl->st->trotmat, m);
    cgl->st->glob_rotate(cgl->st->cell_lines, m);
    cgl->st->glob_rotate(cgl->st->atoms, m);
    cgl->update();
}

void MainWindow::on_actionAround_Bond_triggered()
{    bool ok;
     QString dStr = QInputDialog::getText(this, "Angle",
                                       "Enter Angle",
                                       QLineEdit::Normal, "", &ok);
     if(!ok || dStr.isEmpty()){
         return;
     }

     if(cgl->st->selat.size()!=2) {
         return;
     }

     float vec[3];
     for(unsigned int i=0;i<3;i++){
        vec[i] = cgl->st->selat[1]->coor[i] - cgl->st->selat[0]->coor[i];
      }

     VectorNormalisation(vec);
     float ang = dStr.toFloat();
     ang = ang*3.14159265359/180.0;
     float m[3][3] = {{cos(ang)+vec[0]*vec[0]*(1-cos(ang)),
                       vec[0]*vec[1]*(1-cos(ang))-vec[2]*sin(ang),
                       vec[0]*vec[2]*(1-cos(ang))+vec[1]*sin(ang)},
                      {vec[1]*vec[0]*(1-cos(ang))+vec[2]*sin(ang),
                      cos(ang)+vec[1]*vec[1]*(1-cos(ang)),
                      vec[1]*vec[2]*(1-cos(ang))-vec[0]*sin(ang)},
                      {vec[2]*vec[0]*(1-cos(ang))-vec[1]*sin(ang),
                      vec[2]*vec[1]*(1-cos(ang))+vec[0]*sin(ang),
                      cos(ang)+vec[2]*vec[2]*(1-cos(ang))}};

     cgl->st->glob_rotate(cgl->st->trotmat, m);
     cgl->st->glob_rotate(cgl->st->cell_lines, m);
     cgl->st->glob_rotate(cgl->st->atoms, m);
     cgl->update();

}

void MainWindow::on_actionStructure_from_Averages_triggered()
{
    QString file = QFileDialog::getSaveFileName(
                this,
                "Save As",
                "",
                tr("All files (*.*);;xyz (*.xyz);;"
                   "VASP FORMAT (POSCAR*);;"
                   "png (*png);;rf (*.rf)")
                );

    if(file.isEmpty()) return;

    unsigned int nat = cgl->st->atoms.size();
    float bond_length=0.0, bond_angle=0.0, torsion=0.0;
    vector<float> distances, angles, torsions;

    for(unsigned int i=3;i<nat;i++){
        bond_length = cgl->st->get_traj_ave_distance(i, i-1);
        distances.emplace_back(bond_length);
        bond_angle = cgl->st->get_traj_ave_angle(i, i-1, i-2);
        angles.emplace_back(bond_angle);
        torsion = cgl->st->get_traj_ave_dihedral(i, i-1, i-2, i-3);
        torsions.emplace_back(torsion);
    }

    //----------------------write output ----------------------------
    QFile outfile(file);
    if(!outfile.open(QIODevice::WriteOnly)){
        cout<<"There was an error opening: "<<file.toStdString()<<endl;
    }
    QTextStream out(&outfile);
    out<<cgl->st->atoms[0].label<<endl;
    out<<cgl->st->atoms[1].label<<"  1  r2"<<endl;
    out<<cgl->st->atoms[2].label<<"  2  r3  1  a3"<<endl;

    for(unsigned int i=3;i<nat;i++){
        // pongo i para referirme al atomo anterior. i = 3 es el atomo 4
        out<<cgl->st->atoms[i].label<<" "<<i<<" r"<<i+1<<"  "<<i-1<<"  a"<<i+1<<"  "<<i-2<<"   d"<<i+1<<" "<<endl;
    }
    out<<""<<endl;
    //Get distance of the first two atoms
    bond_length = cgl->st->get_traj_ave_distance(0, 1);
    out<<"r2   "<<bond_length<<endl;
    //Get distance between the second and third atom
    bond_length = cgl->st->get_traj_ave_distance(1, 2);
    out<<"r3   "<<bond_length<<endl;
    bond_angle = cgl->st->get_traj_ave_angle(2, 1, 0);
    out<<"a3   "<<bond_angle<<endl;

    // ---- iterate over the variables stores in the vectors -------
    for(unsigned int i=0;i<distances.size();i++){
        out<<"r"<<i+4<<"  "<<distances[i]<<endl;
        out<<"a"<<i+4<<"  "<<angles[i]<<endl;
        out<<"d"<<i+4<<"  "<<torsions[i]<<endl;
    }

    outfile.close();

    return;
}

void MainWindow::on_actionCorrect_periodic_trajectory_triggered()
{
    cgl->st->frac_cart(cgl->st->atoms, false, true);
    array<float, 3> before;
    for(auto &at:cgl->st->atoms){
        before = at.peli[0];
        for(unsigned int i=1; i<at.peli.size();i++){
            for(unsigned int j=0;j<3;j++){
                if(at.peli[i][j] - before[j] < -0.85){
                    at.peli[i][j] += 1.0;
                }
                else if(at.peli[i][j] - before[j] > 0.85){
                    at.peli[i][j] -= 1.0;
                }
            }
        }
    }
    cgl->st->frac_cart(cgl->st->atoms, true, true);
    cgl->update();
}

void MainWindow::on_actionangle_triggered()
{
    int code = 1;
    QString name;

    if(cgl->st->selat.size()!=3){
        QMessageBox *mg = new QMessageBox(this);
        mg->setText("Please select three atoms ");
        mg->exec();
        return;
    }

    while(code != 0){
        name = QInputDialog::getText(this, "Angle Name", "Enter Name");
        if(name.isEmpty()) return;
        code = cgl->st->newCollectiveVariable(name, "angle");
        if(code == 1){
            QMessageBox *mg = new QMessageBox(this);
            mg->setText("Name already exists !! Select another name");
            mg->exec();
        }
    }
}

void MainWindow::checkMemoryUsage(){
    updateMemoryUsage();
    if(availableMemory < 102400.0 ){
        cout <<"this process is using too much memory\n Exiting\n";
        abort();
    }
}

void MainWindow::on_actionrings_of_size_triggered()
{
//    unsigned int index, ringSize;
//    QStringList list;
//    if(cgl->st->selat.size() != 1) {
//        while(list.size()!=2){
//            QString str = QInputDialog::getText(this, "Ring Selector", "Enter the Index of atom belonging to the Ring and the size of the Ring");
//            list = str.split(" ", QString::SkipEmptyParts);
//        }
//        index = list[0].toInt();
//        ringSize = list[1].toInt();
//    }else{
//        index = cgl->st->selat[0]->index;
//        QString str = QInputDialog::getText(this, "Ring Selector", "Enter the size of the Ring");
//        ringSize = str.toFloat();
//    }


//    for(auto sel:cgl->st->selat){
//       bonded.push_back(sel->index);
//    }
//    bonded = cgl->st->reversed_bond_search(bonds, bonded);
//    cgl->st->unselect_all();
//    //tengo que quitar sel true de los atoms
//    for(std::size_t i = 0, max = bonded.size(); i != max; ++i){
////            auto it = std::find(cgl->st->selat.begin(), cgl->st->selat.end(), &cgl->st->atoms[bonded[i]]);
////            if(it != cgl->st->selat.end()){
////                continue;
////            }
//        cgl->st->atoms[bonded[i]].sel = true;
//        cgl->st->selat.push_back(&cgl->st->atoms[bonded[i]]);
//    }
    return;
}

void MainWindow::on_actionLoad_Style_from_state_rs_triggered()
{
    QString selfilter = tr("STATE (*.rs)");
    QString filePath = QFileDialog::getOpenFileName(
                this,
                "Open file",
                "",
                tr("rf (*.rf)"),
                &selfilter
                                                    );

    if(filePath.isEmpty()){
        return;
    }
    cgl->st->reverse_rotation(1);
    cgl->loadStyleFromState(filePath);
    cgl->update();
}

void MainWindow::on_actionCenter_of_Mass_selected_triggered()
{
    array<float,3> tempCenterOfMass = {0.0,0.0,0.0};
    vector<array<float,3>> tempcoors;
    QString text;
    for(auto sel:cgl->st->selat){
        tempcoors.push_back(sel->coor);
    }

    // set inv rot mat
    cgl->st->set_inv_mat();
    cgl->st->glob_rotate(tempcoors, cgl->st->trotmat);
    cgl->st->unshift_coors(tempcoors);


    for(auto sel:tempcoors){
        //update temporal center of mass
        for(int i=0;i<3;i++){
            tempCenterOfMass[i] += sel[i];
        }
    }
    for(int i=0;i<3;i++){
        tempCenterOfMass[i] /= cgl->st->selat.size() ;
        text += QString::number(tempCenterOfMass[i], 'f', 3);
        text += "  ";
    }

    QMessageBox *msg = new QMessageBox(this);
    msg->setText(text);
    msg->exec();
}

void MainWindow::on_actionSet_view_range_triggered()
{
    bool ok;
    float zoom;
    QString str = QInputDialog::getText(this, "Zoom position", "Enter Number", QLineEdit::Normal, "",&ok);
    if(!ok){
        return;
    }
    try{
        zoom = str.toFloat();
    }catch(...){
        return;
    }
    cgl->rRange = 15 - zoom;
    cgl->lRange = -15 + zoom;
    cgl->uRange = 15 - zoom;
    cgl->dRange = -15 + zoom;
    cgl->onSize();
    cgl->update();
}

void MainWindow::on_actionY_coor_COM_triggered()
{
    int code = 1;
    QString name;

    if(cgl->st->selat.size()==0){
        QMessageBox *mg = new QMessageBox(this);
        mg->setText("Please select at least one atom ");
        mg->exec();
        return;
    }    

    while(code != 0){
        name = QInputDialog::getText(this, "Y Name", "Enter Name");
        if(name.isEmpty()) return;
        code = cgl->st->newCollectiveVariable("Y coordinates", "y");
        if(code == 1){
            QMessageBox *mg = new QMessageBox(this);
            mg->setText("Name already exists !! Select another name");
            mg->exec();
        }
    }
}



void MainWindow::on_actionAtom_with_n_bond_triggered()
{
    bool ok;
    vector<int> nbonds;
    QString str = QInputDialog::getText(this, "Select by Bonds", "Number of bonds.(ex, 1 or 1 2 ", QLineEdit::Normal, "",&ok);
    if(!ok){
        return;
    }
    try{
        QStringList l = str.split(" ",QString::SkipEmptyParts);
        for(auto i:l){
            nbonds.emplace_back(i.toInt());
        }
    }catch(...){
        return;
    }

    cgl->st->selectAtomsWithNBonds(nbonds);
    cgl->update();
}

void MainWindow::on_action3d_triggered()
{
    cgl->scatterplot_mode_2d = false;
    ui->actiondots->setChecked(false);

    if(!cgl->scatterplot_mode && cgl->st->pelisize<2){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("This not a trajectory file!!\n");
        msg->exec();

        ui->action3d->setChecked(false);
        return;
    }
    if(!cgl->scatterplot_mode && cgl->st->selat.size()==0){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("Please select at least one atom render all points of the trajectory");
        msg->exec();
    }
    if(!cgl->scatterplot_mode) cgl->scatterplot_mode = true;
    else cgl->scatterplot_mode = false;
//    cgl->update();
    return;
}

void MainWindow::on_actiondots_triggered()
{
    cgl->scatterplot_mode = false;
    ui->action3d->setChecked(false);

    if(!cgl->scatterplot_mode_2d && cgl->st->pelisize<2){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("This not a trajectory file!!\n");
        msg->exec();

        ui->actiondots->setChecked(false);
        return;
    }
    if(!cgl->scatterplot_mode_2d && cgl->st->selat.size()==0){
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("Please select at least one atom render all points of the trajectory");
        msg->exec();
    }
    if(!cgl->scatterplot_mode_2d) cgl->scatterplot_mode_2d = true;
    else cgl->scatterplot_mode_2d = false;
//    cgl->update();
    return;
}

void MainWindow::on_actionExport_Plumed_File_triggered()
{
    QString file = QFileDialog::getSaveFileName(
                this,
                "Save As",
                "");

    if(file.isEmpty()) return;

    cgl->st->exportPlumedFileCV(file);
}

void MainWindow::on_actionadd_plane_triggered()
{
    if(cgl->st->selat.size() == 4){
        Plane p;
        for(unsigned int i=0;i<4;++i){
            p.indices[i] = cgl->st->selat[i]->index;
        }

        cgl->st->planes.emplace_back(p);
    }
    else if(cgl->st->selat.size() == 3){
        Plane p;
        for(unsigned int i=0;i<3;++i){
            p.indices[i] = cgl->st->selat[i]->index;
        }
        p.indices[3] = -1;

        cgl->st->planes.emplace_back(p);
    }

    cgl->update();
}

void MainWindow::on_actionplane_by_index_triggered()
{
    bool ok;
    vector<int> nbonds;
    QString str = QInputDialog::getText(this, "Select by Bonds", "Number of bonds.(ex, 1 or 1 2 ", QLineEdit::Normal, "",&ok);
    if(!ok){
        return;
    }

    try{
        QStringList l = str.split(" ",QString::SkipEmptyParts);
        for(auto i:l){
            nbonds.emplace_back(i.toInt());
        }
    }catch(...){
        return;
    }

    for(auto v:nbonds){
        cgl->st->planes[v].selected = true;
    }

    cgl->update();
}

void MainWindow::on_actionadd_arrow_triggered()
{
    if(cgl->st->selat.size() == 3){
        Arrow a;
        a.indices[0] = cgl->st->selat[0]->index;
        a.indices[1] = cgl->st->selat[1]->index;
        a.indices[2] = cgl->st->selat[2]->index;
        cgl->st->arrows.emplace_back(a);
    }

    cgl->update();
}

void MainWindow::on_actionEliminate_Repeated_Atoms_triggered()
{
    cgl->st->remove_duplicates(true);
    cgl->update();
}

void MainWindow::on_actionto_position_triggered()
{
    bool ok;
    float coor[3];
    QString str = QInputDialog::getText(this, "Translate", "Enter x y z coordinates", QLineEdit::Normal, "",&ok);
    if(!ok){
        return;
    }

    try{
        QStringList l = str.split(" ",QString::SkipEmptyParts);
        for(unsigned int i=0;i<3;i++){
            coor[i] = l[i].toFloat();
        }
    }catch(...){
        return;
    }

    cgl->st->translateSelectionTo(coor);
    cgl->update();

}

void MainWindow::on_actionIndex_triggered()
{
    bool ok;
    QString str = QInputDialog::getText(this, "Translate", "Enter x y z coordinates", QLineEdit::Normal, "",&ok);
    if(!ok){
        return;
    }

    cgl->st->selectByIndex(str);
    cgl->update();

}

void MainWindow::on_actionmake_supercell_triggered()
{
    cgl->st->make_supercell();
    cgl->update();
}

void MainWindow::on_actionDownload_from_IZA_DB_triggered()
{
    QString name = QInputDialog::getText(this, "Download dialog", "Enter zeolite Code (ex MFI)");
    if(name.isEmpty()) return;

    QString cmd = "wget europe.iza-structure.org/IZA-SC/cif/"+
            name + ".cif  -O/tmp/"+
            name + ".cif 2>/dev/null";
    int outcode = system(cmd.toStdString().c_str());

    if(outcode != 0){
        printf("There was an error. Please check the code and that you have wget installed\n");
        return;
    }

    openFile("/tmp/"+name+".cif");

}
