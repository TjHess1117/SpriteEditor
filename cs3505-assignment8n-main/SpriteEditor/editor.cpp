#include "editor.h"
#include "QtCore/qsize.h"
#include "sprite.h"
#include "frame.h"
#include "tool.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
/// @reviewed by tj hess
Editor::Editor(int width, int height) {
    sprite = new Sprite(width, height);
}

Editor::~Editor() {
    delete sprite;
}

void Editor::addEmptyFrame() {
    Frame f(sprite->getWidth(), sprite->getHeight());
    sprite->pushFrame(f);
}

void Editor::addFrame(Frame& frame) {
    sprite->pushFrame(frame);
}

void Editor::duplicateFrame() {
    // Duplicate the current frame

    Frame currentFrame = sprite->getFrame(currentFrameIndex);
    sprite->insertFrame(currentFrame, currentFrameIndex + 1);
    QImage image = currentFrame.toImage();
    emit sendFrameButtonImage(image, currentFrameIndex + 1);
    emit addClonedImageToPreview(image, currentFrameIndex + 1);
}

void Editor::saveSlot(QString filename) {
    QFile file(filename + ".ssp");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    QJsonDocument doc = QJsonDocument::fromJson(sprite->toJson().toUtf8());
    out << doc.toJson(QJsonDocument::Indented);
    file.close();
}

void Editor::loadSlot(QString filepath) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        return;
    }

    QString data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (doc.isNull()) {
        return;
    }

    QJsonObject spriteObj = doc.object();
    delete sprite;

    std::vector<QImage> images;
    sprite = new Sprite(spriteObj);
    for(int i = 0; i < sprite->getFrameCount(); i++){
        emit insertFrameButton(i);
        QImage image = sprite->getFrame(i).toImage();
        images.push_back(image);
        emit sendFrameButtonImage(image, i);
    }
    emit sendFrames(images);
}

QPoint Editor::convertMouseToPixel(QPointF mouseCoords, QSize canvasSize) {
    // Access the width and height from the Sprite instance
    int width = sprite->getWidth();
    int height = sprite->getHeight();

    // Convert mouse coordinates to "virtual" pixel coordinates
    int pixelX = (mouseCoords.x() * width) / canvasSize.width();
    int pixelY = (mouseCoords.y() * height) / canvasSize.height();

    // Clamp the pixel coordinates to ensure they're within the sprite's bounds
    pixelX = std::max(0, pixelX) % width;
    pixelY = std::max(0, pixelY) % height;
    return QPoint(pixelX, pixelY);;
}


void Editor::setColor(const QColor &newColor) {
    currentColor = newColor;
    emit colorChanged(currentColor);
}
void Editor::createNewSpriteSlot(int Width,int height){
    delete sprite;

    sprite = new Sprite(Width, height);
    QImage blankSlate;
    blankSlate.fill(Qt::white);
    emit frameUpdated(blankSlate); // update the canvas to hold nothing so that the old sprite appears gone
}
void Editor::editFrame(const QPointF &mouseCoords, const QSize &canvasSize, bool dragTool) {

    QPoint pixelCords = convertMouseToPixel(mouseCoords, canvasSize);
    // the active tool will tell use what oporation to preform on the canvas
    switch (activeTool) {
    case ToolType::Pen:
        Tool::pen(pixelCords, currentColor, sprite->getFrame(currentFrameIndex));
        break;
    case ToolType::Eraser:
        Tool::eraser(pixelCords, sprite->getFrame(currentFrameIndex));
        break;
    case ToolType::Fill:
        if (dragTool) return;
        else Tool::fill(pixelCords, currentColor, sprite->getFrame(currentFrameIndex));
        break;
    case ToolType::EyeDropper:
        if (dragTool) return;
        else setColor(Tool::eyeDropper(pixelCords, sprite->getFrame(currentFrameIndex)));
        break;
    }
    Frame currentFrame = sprite->getFrame(currentFrameIndex);
    QImage imageToDisplay = currentFrame.toImage();

    emit frameUpdated(imageToDisplay);
    emit sendFrameButtonImage(imageToDisplay, currentFrameIndex);
}

void Editor::updateCurrentFrame(int frameIndex) {
    if (frameIndex < 0 || frameIndex >= sprite->getFrameCount())
        return;
    currentFrameIndex = frameIndex;
    Frame frame = sprite->getFrame(frameIndex);
    QImage image = frame.toImage();
    emit frameUpdated(image);
    emit sendFrameButtonImage(image, currentFrameIndex);
}

void Editor::removeFrameSlot(int frameIndex) {
    if (frameIndex < 0 || frameIndex >= sprite->getFrameCount())
        return;

    if (currentFrameIndex == frameIndex) // if the current frame was deleted, change the current frame
        currentFrameIndex--;
    sprite->eraseFrame(frameIndex);
}

void Editor::getFrames() {
    std::vector<QImage> images;
    for(int i = 0; i < sprite->getFrameCount(); i++)
        images.push_back(sprite->getFrame(i).toImage());
    emit sendFrames(images);
}
