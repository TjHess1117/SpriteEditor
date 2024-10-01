/*
 * This SpriteEditor project creates a gui where a user can create, edit, save, and load Sprites.
 *
 * @authors Tj Hess, Tanner Bergstrom, Noah Campbell, Will Black, Kevin Christiansen
 * @version 4/1/24
 */


#include "mainwindow.h"
#include <QApplication>
#include <QColor>
/// @reviewed by tj
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Editor editor(10, 10);

    QWidget widget;
    MainWindow w(editor);
    w.show();
    return a.exec();
}
