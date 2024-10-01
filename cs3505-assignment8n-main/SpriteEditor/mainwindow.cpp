#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QGridLayout>
#include "editor.h"
#include "canvas.h"
#include "preview.h"
#include <QObject>
#include <QApplication>
#include <QPixmap>
#include <qinputdialog.h>
#include <QMessageBox>
#include <QMutex>
/// @reviewed by will black
MainWindow::MainWindow(Editor &editor, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupTools(ui, editor);
    setupAnimationPreview(ui, editor);
    setupColorPicker(ui, editor);
    setupCanvas(ui, editor);
    setupActions(ui, editor);
    setupFrameSelection(ui,editor);

    update();
    currentFrame = 0;
    insertFrameButton(0);
    frameClicked(0);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::newSprite() {
    QInputDialog* inputDialog = new QInputDialog;
    inputDialog->setLabelText(QString("Enter New Canvas Size Between 0-64"));
    connect(inputDialog, &QInputDialog::textValueSelected, this, &MainWindow::createNewSprite);

    inputDialog->setEnabled(true);
    inputDialog->setVisible(true);
}

void MainWindow::createNewSprite(QString stringSize) {
    int size = stringSize.toInt(nullptr,10);
    bool isNotValidInput = size > 64 || size <= 0;
    if(isNotValidInput){
        // double checks that the input is valid
        QMessageBox::critical(nullptr, "Error", "please enter a valid input, valid inputs are 0-64");
        return;
    }
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(ui->framesScrollArea->widget()->layout());
    for(auto buttonToRemove : frameButtons){
        layout->removeWidget(buttonToRemove);
        delete buttonToRemove;
    }
    frameButtons.clear();
    currentFrame = 0;
    insertFrameButton(0);
    emit createNewSpriteSignal(size, size);
    frameClicked(0);

    ui->animationPreview->resetPreview();
}

void MainWindow::saveSprite() {
    QString filename = QFileDialog::getSaveFileName();
    emit saveSpriteSignal(filename);
}

void MainWindow::loadSprite() {
    QString filepath = QFileDialog::getOpenFileName();
    if(!filepath.endsWith(".ssp")){
        QMessageBox::critical(nullptr, "Error", "Incorrect file type.");
        return;
    }
    if(!filepath.isEmpty()){
        QHBoxLayout* layout = (QHBoxLayout*)(ui->framesScrollArea->widget()->layout());
        for(auto buttonToRemove : frameButtons){
            layout->removeWidget(buttonToRemove);
            delete buttonToRemove;
        }
        frameButtons.clear();
        currentFrame = 0;
        emit loadSpiteSignal(filepath);
        frameClicked(0);
    }

    QHBoxLayout* layout = (QHBoxLayout*)(ui->framesScrollArea->widget()->layout());
    for(auto buttonToRemove : frameButtons) {
        layout->removeWidget(buttonToRemove);
        delete buttonToRemove;
    }
    frameButtons.clear();
    currentFrame = 0;
    emit loadSpiteSignal(filepath);
    frameClicked(0);
}

void MainWindow::addFrame() {
    QPushButton* newFrameButton = new QPushButton( ui->framesScrollArea);
    newFrameButton->setFixedSize(75, 75);
    newFrameButton->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->framesScrollArea->widget()->layout();
    layout->addWidget(newFrameButton, Qt::AlignLeft);

    frameButtons.push_back(newFrameButton);

    emit frameAdded();

    int index = frameButtons.size() - 1;

    // Connect the button click signal to the dynamically created slot
    connect(newFrameButton, &QPushButton::clicked, [this, index]() { frameClicked(index); });
    layout->setAlignment(Qt::AlignLeft);
    frameClicked(index);
}

void MainWindow::insertFrameButton(int frameButtonNum){
    QPushButton* newFrameButton = new QPushButton(ui->framesScrollArea);
    newFrameButton->setFixedSize(75, 75);
    newFrameButton->setStyleSheet("QPushButton {background-color: rgb(240,240,240);}");

    QHBoxLayout* layout = (QHBoxLayout*)(ui->framesScrollArea->widget()->layout());
    layout->addWidget(newFrameButton, Qt::AlignLeft);

    frameButtons.push_back(newFrameButton);

    // Connect the button click signal to the dynamically created slot
    connect(newFrameButton, &QPushButton::clicked, [this, frameButtonNum]() { frameClicked(frameButtonNum); });
    layout->setAlignment(Qt::AlignLeft);
}

void MainWindow::frameClicked(int frameNum) {
    auto previousButton = frameButtons.at(currentFrame);
    currentFrame = frameNum;
    auto currentButton = frameButtons.at(currentFrame);
    previousButton->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}"); // make the previous button appear to be un-selected
    currentButton->setStyleSheet("QPushButton {background-color: rgb(160,160,160);}");
    emit frameSelected(currentFrame);
}

void MainWindow::deleteFrame() {
    // Deleting the window's only frame isn't allowed
    if (frameButtons.size() <= 1)
        return;

    if (currentFrame < 0 || currentFrame >= frameButtons.size())
        return;

    emit frameDeleted(currentFrame);

    QPushButton* currentFrameButton = frameButtons.at(currentFrame);
    QHBoxLayout* layout = (QHBoxLayout*)(ui->framesScrollArea->widget()->layout());

    layout->removeWidget(currentFrameButton); // remove the button from the view
    delete currentFrameButton;

    frameButtons.erase(frameButtons.begin() + currentFrame); // remove the button from the list of buttons;


    for (int i = currentFrame; i < frameButtons.size(); i++) {
        frameButtons[i]->disconnect(frameButtons[i], 0, 0, 0);

        // Connect the button click signal to the dynamically created slot
        connect(frameButtons[i], &QPushButton::clicked, [this, i]() { frameClicked(i); });
    }

    if(currentFrame  >= frameButtons.size())
        currentFrame--;

    QPushButton* newSelectedButton = frameButtons.at(currentFrame);
    newSelectedButton->setStyleSheet(QString("QPushButton {background-color: rgb(160,160,160);} QPushButton:pressed {background-color: rgb(160,160,160);}"));
    emit frameSelected(currentFrame);
}

void MainWindow::cloneFrame() {
    if(frameButtons.size() == 0)
        return;

    QPushButton* newFrameButton = new QPushButton(ui->framesScrollArea);
    newFrameButton->setFixedSize(75, 75);
    newFrameButton->setStyleSheet(QString("QPushButton {background-color: rgb(224,224,224);}"));

    QHBoxLayout* layout = (QHBoxLayout*)(ui->framesScrollArea->widget()->layout());
    layout->insertWidget(currentFrame + 1, newFrameButton);
    frameButtons.insert(frameButtons.begin() + currentFrame + 1, newFrameButton);

    for (int i = currentFrame + 1; i < frameButtons.size(); i++) {
        frameButtons[i]->disconnect(frameButtons[i], 0, 0, 0);

        // Connect the button click signal to the dynamically created slot
        connect(frameButtons[i], &QPushButton::clicked, [this, i]() { frameClicked(i); });
    }

    emit frameCloned();

    frameClicked(currentFrame + 1);
}

void MainWindow::receiveFrameButtonImage(QImage image, int index){
    image = image.scaled(75, 75);
    frameButtons[index]->setIconSize(QSize(75, 75));
    frameButtons[index]->setIcon(QPixmap::fromImage(image));
}

// ---------------------------------------------- SETUP REALM! ---------------------------------------------- //

void MainWindow::setupTools(Ui::MainWindow *ui, Editor &editor) {
    editor.setActiveTool(ToolType::Pen);
    ui->penTool->setDefault(true);

    QMainWindow::connect(ui->penTool, &QPushButton::clicked,
                         &editor,
                        [&editor, ui]() {
                            editor.setActiveTool(ToolType::Pen);
                            ui->penTool->setDefault(true);
                            ui->eraserTool->setDefault(false);
                            ui->fillTool->setDefault(false);
                            ui->eyeDropperTool->setDefault(false);
                        });
    QMainWindow::connect(ui->eraserTool, &QPushButton::clicked,
                         &editor,
                        [&editor, ui]() {
                            editor.setActiveTool(ToolType::Eraser);
                            ui->penTool->setDefault(false);
                            ui->eraserTool->setDefault(true);
                            ui->fillTool->setDefault(false);
                            ui->eyeDropperTool->setDefault(false);
                        });
    QMainWindow::connect(ui->fillTool, &QPushButton::clicked,
                         &editor,
                        [&editor, ui]() {
                            editor.setActiveTool(ToolType::Fill);
                            ui->penTool->setDefault(false);
                            ui->eraserTool->setDefault(false);
                            ui->fillTool->setDefault(true);
                            ui->eyeDropperTool->setDefault(false);
                        });
    QMainWindow::connect(ui->eyeDropperTool, &QPushButton::clicked,
                         &editor,
                        [&editor, ui]() {
                            editor.setActiveTool(ToolType::EyeDropper);
                            ui->penTool->setDefault(false);
                            ui->eraserTool->setDefault(false);
                            ui->fillTool->setDefault(false);
                            ui->eyeDropperTool->setDefault(true);
                        });
}

void MainWindow::setupAnimationPreview(Ui::MainWindow *ui, Editor &editor) {

    Preview *animPrev = ui->animationPreview;

    QMainWindow::connect(&editor, &Editor::frameUpdated,
                         animPrev,
                        [animPrev, this](QImage &image) {
                             animPrev->receiveFrame(image, currentFrame);
                        });

    QMainWindow::connect(ui->fpsSlider, &QSlider::valueChanged,
                         animPrev, &Preview::fpsChanged);

    QMainWindow::connect(this, &MainWindow::frameDeleted,
                         animPrev, &Preview::deleteFrame);

    QMainWindow::connect(this, &MainWindow::frameAdded,
                         animPrev, &Preview::addEmptyFrame);

    QMainWindow::connect(&editor, &Editor::addClonedImageToPreview,
                         animPrev, &Preview::addClonedImageToPreview);

    QMainWindow::connect(ui->actualSizeButton, &QCheckBox::stateChanged,
                         &editor,
                        [animPrev, &editor](bool actualSize) {
                            animPrev->setDisplayActualSize(actualSize);
                            editor.getFrames();
                        });

    QMainWindow::connect(&editor, &Editor::sendFrames,
                         animPrev, &Preview::loadNewFrames);

    animPrev->startPreview(ui->fpsSlider->value());
}

void MainWindow::setupFrameSelection(Ui::MainWindow *ui, Editor &editor) {
    QMainWindow::connect(ui->addFrame, &QPushButton::clicked,
                         this, &MainWindow::addFrame);
    QMainWindow::connect(ui->deleteFrame, &QPushButton::clicked,
                         this, &MainWindow::deleteFrame);
    QMainWindow::connect(ui->cloneFrame, &QPushButton::clicked,
                         this, &MainWindow::cloneFrame);

    QMainWindow::connect(this, &MainWindow::frameSelected,
                         &editor, &Editor::updateCurrentFrame);
    QMainWindow::connect(this, &MainWindow::frameAdded,
                         &editor, &Editor::addEmptyFrame);
    QMainWindow::connect(this, &MainWindow::frameDeleted,
                         &editor, &Editor::removeFrameSlot);

    QMainWindow::connect(this, &MainWindow::frameCloned,
                         &editor, &Editor::duplicateFrame);
    QMainWindow::connect(&editor, &Editor::sendFrameButtonImage,
                         this, &MainWindow::receiveFrameButtonImage);

}

void MainWindow::setupActions(Ui::MainWindow *ui, Editor &editor) {
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newSprite);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveSprite);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadSprite);

    connect(this,&MainWindow::createNewSpriteSignal, &editor, &Editor::createNewSpriteSlot);
    connect(this,&MainWindow::saveSpriteSignal, &editor, &Editor::saveSlot);
    connect(this,&MainWindow::loadSpiteSignal, &editor, &Editor::loadSlot);

    connect(&editor, &Editor::insertFrameButton, this, &MainWindow::insertFrameButton);
}

void MainWindow::setupColorPicker(Ui::MainWindow *ui, Editor &editor) {
    QPushButton *colorPicker = ui->colorPicker;

    QMainWindow::connect(colorPicker, &QPushButton::clicked,
                         &editor,
                        [&editor]() {
                            // Sets the paint color using a QColorDialog
                            editor.setColor(QColorDialog::getColor(Qt::GlobalColor::white, nullptr, QString(), QColorDialog::ShowAlphaChannel));
                        });
    QMainWindow::connect(&editor, &Editor::colorChanged,
                         colorPicker,
                        [colorPicker](const QColor &newColor) {
                            // Sets the background color using a QColorDialog
                            QPalette pal = colorPicker->palette();
                            pal.setBrush(QPalette::AlternateBase, newColor);
                            colorPicker->setPalette(pal);
                            colorPicker->setBackgroundRole(QPalette::AlternateBase);
                        });

    editor.setColor(QColorConstants::Black);
}

void MainWindow::setupCanvas(Ui::MainWindow *ui, Editor &editor) {

    QMainWindow::connect(&editor, &Editor::frameUpdated,
                         ui->canvas, &Canvas::setImage);
    QMainWindow::connect(ui->canvas, &Canvas::mouseAction,
                         &editor, &Editor::editFrame);
}
