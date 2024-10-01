#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPushButton>
#include <QImage>
#include "editor.h"
#include <qinputdialog.h>
#include <QMutex>
/*
 * main window class provides functionality for dispaly features of the main window.
 * the mainwindow class includes frame selections, frame additions, frame deletions, frame clones, loading
 * and saving sprites, creating new sprites. the main window also has the ability to select pixles on a canvas
 * according to the selected tool and color, pen (this includes dragging), erase, eyedroper tool and fill tool.
 * @authors: Noah Campbell, Will Black, Tanner Bergstrom, Tj Hess and Kevin Christiansen
 * @ version 3/31/2024
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(Editor &editor, QWidget *parent = nullptr);
        ~MainWindow();

        QFileDialog fileDialog;

        QColor color;

        std::vector<QPushButton*> frameButtons;

        int currentFrame;
    signals:
        /// @brief sends the signal that the frame button has been pushed
        void frameAdded();

        /// @brief sends the signal the current Frame to the editor class
        /// @brief the integer corisponding to the current frame
        void frameSelected(int);

        /// @brief tells the editor that the clone button was pushed and to update accordingly
        void frameCloned();

        /// @brief displayes the frame
        /// @param the dispaly frame rate
        /// @param boolean saying if the frame should be shown at true size or not
        /// @param the frames actual size in width
        /// @param the frames actual size in height
        void updatePreviewFrame(int frameRate, bool showPreviewActualSize, int actualSizeWidth, int actualSizeHeight);

        /// @brief the signal to create a new sprite sent to the editor
        /// @param the new sprites width
        /// @param the new sprties height
        void createNewSpriteSignal(int width,int height);

        /// @brief The signal to save the current sprite to the said filename
        /// @brief the specified filename
        void saveSpriteSignal(QString filename);

        /// @brief the signal to load a sprite from the said file Path
        /// @param the file path to load from
        void loadSpiteSignal(QString filepath);

        /// @brief the signal to update the editor that a frame has been deleted
        /// @param the index of the frame
        void frameDeleted(int frameIndex);

        void startPreview(int frameRate);

        void getFrames(int actualSizeWidth, int actualSizeHeight, bool showPreviewActualSize);
    private slots:
        /// @brief the slot that catches the event of new sprite being pushed
        void newSprite();

        /// @brief the slot that catches the event of save sprite being pushed
        void saveSprite();

        /// @brief the slot that catches the event of load sprite being pushed
        void loadSprite();

        /// @brief the slot that catches the event of add frame button being pushed
        void addFrame();

        /// @brief the slot that catches the event of clone frame button being pushed
        void cloneFrame();

        /// @brief the slot that catches the event of delete frame button being pushed
        void deleteFrame();
    private:
        Ui::MainWindow *ui;
        QInputDialog inputDialog; // this is the custom dialog box that appears when the user presses new sprite
        ToolType tool = ToolType::Pen;

        /// @brief sets up connection methods for the tools.
        /// @param mainWindow
        /// @param editor
        void setupTools(Ui::MainWindow *ui, Editor &editor);

        /// @brief sets up connection methods for the animationPreiew.
        /// @param mainWindow
        /// @param editor
        void setupAnimationPreview(Ui::MainWindow *ui, Editor &editor);

        /// @brief sets up connection methods for the frameSelection.
        /// @param mainWindow
        /// @param editor
        void setupFrameSelection(Ui::MainWindow *ui, Editor &editor);

        /// @brief sets up connection methods for the colorPicker.
        /// @param mainWindow
        /// @param editor
        void setupColorPicker(Ui::MainWindow *ui, Editor &editor);

        /// @brief sets up main window.
        /// @param mainWindow
        /// @param editor
        void setupActions(Ui::MainWindow *ui, Editor &editor);

        /// @brief sets up canvas and its connections
        /// @param mainWindow
        /// @param editor
        void setupCanvas(Ui::MainWindow *ui, Editor &editor);
    public slots:
        /// @brief the slot that catchs the event of the create new sprite button being pushed
        /// @param new sprites height
        /// @param new sprites width
        void createNewSprite(QString size);

        /// @brief inserts the frame in the horizontal layout for display at the given index
        /// @param the index to place the button at
        void insertFrameButton(int frameButtonNum);

        /// @brief the slot to recive and display the corisponding frame to the frame button
        /// @param the image/frame to display
        /// @param the button index to place the image on
        void receiveFrameButtonImage(QImage image, int index);

        /// @brief send handles the event of a frame button being clicked
        /// @param the index of the frame on the horizontal layout
        void frameClicked(int);
};
#endif // MAINWINDOW_H
