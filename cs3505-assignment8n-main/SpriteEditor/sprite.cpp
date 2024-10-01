#include "sprite.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <algorithm>
/// @reviewed by tanner
Sprite::Sprite(int width, int height) : width(width), height(height) {
    if (width <= 0 || height <= 0) {
        width = 0;
        height = 0;
    }
    frames.push_back(Frame(width, height)); // Create the initial frame
}

Sprite::Sprite(QJsonObject& spriteObj) {
    width = spriteObj["width"].toInt();
    height = spriteObj["height"].toInt();

    QJsonArray framesArray = spriteObj["frames"].toArray();
    for (const QJsonValue& frameVal : framesArray) {
        QJsonObject frameObj = frameVal.toObject();
        Frame frame(width, height, frameObj);

        frames.push_back(frame);
    }
}

Sprite::Sprite(const Sprite& other) : frames(other.frames), width(other.width), height(other.height) {}

Sprite& Sprite::operator=(Sprite other) {
    std::swap(width, other.width);
    std::swap(height, other.height);
    std::swap(frames, other.frames);
    return *this;
}

Sprite::~Sprite() {}

Frame& Sprite::getFrame(int index) {
    return frames[index];
}

void Sprite::insertFrame(Frame& frame, int index) {
    frames.insert(frames.begin() + index, frame);
}

void Sprite::pushFrame(Frame& frame) {
    frames.push_back(frame);
}

void Sprite::eraseFrame(int index) {
    frames.erase(frames.begin() + index);
}

void Sprite::setPixelColor(int frameIndex, QColor color, int pixelX, int pixelY) {
    frames[frameIndex].setPixelColor(pixelX, pixelY, color);
}

QColor Sprite::getPixelColor(int frameIndex, int pixelX, int pixelY) {
    return frames[frameIndex].getPixelColor(pixelX, pixelY);
}

QString Sprite::toJson() const {
    QJsonArray framesArray;
    for (const Frame& frame : frames) {
        QJsonDocument frameDoc = QJsonDocument::fromJson(frame.toJson().toUtf8());
        framesArray.append(frameDoc.object());
    }

    QJsonObject spriteObj;
    spriteObj["width"] = width;
    spriteObj["height"] = height;
    spriteObj["frames"] = framesArray;
    QJsonDocument doc(spriteObj);
    return doc.toJson(QJsonDocument::Compact); // Use Compact for a more condensed JSON string
}
