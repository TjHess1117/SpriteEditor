#include "frame.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
/// @reviewed by noah
Frame::Frame(int width, int height) : width(width), height(height) {
    pixels = new QColor*[height];
    for (int row = 0; row < height; row++) {
        pixels[row] = new QColor[width];
        for (int col = 0; col < width; col++)
            pixels[row][col] = Qt::GlobalColor::transparent;
    }
}

Frame::Frame(const Frame& other) : width(other.width), height(other.height) {
    pixels = new QColor*[height];
    for (int row = 0; row < height; ++row) {
        pixels[row] = new QColor[width];
        for (int col = 0; col < width; ++col)
            pixels[row][col] = other.pixels[row][col];
    }
}

Frame::~Frame() {
    for (int row = 0; row < height; ++row)
        delete[] pixels[row];
    delete[] pixels;
}

Frame& Frame::operator=(Frame other) {
    // deallocatePixels();

    std::swap(width, other.width);
    std::swap(height, other.height);
    for (int row = 0; row < height; ++row) {
        pixels[row] = new QColor[width];
        for (int col = 0; col < width; ++col)
            std::swap(pixels[row][col], other.pixels[row][col]);
    }

    return *this;
}

QColor Frame::getPixelColor(int x, int y) const {
    if ((0 <= x && x < width) && (0 <= y && y < height))
        return pixels[y][x];
    throw std::out_of_range("Index is out of range");
}

void Frame::setPixelColor(int x, int y, QColor color) {
    if ((x < 0 || width <= x) || (y < 0 || height <= y))
        throw std::out_of_range("Index is out of range");
    pixels[y][x] = color;
}

QString Frame::toJson() const {
    QColor *color;

    QJsonArray rows;
    for (int row = 0; row < height; ++row) {
        QJsonArray cols;
        for (int col = 0; col < width; ++col) {
            color = &pixels[row][col];
            // Convert each pixel's color to an RGB string or object
            QJsonObject colorObj;
            colorObj["r"] = color->red();
            colorObj["g"] = color->green();
            colorObj["b"] = color->blue();
            colorObj["a"] = color->alpha();
            cols.append(colorObj);
        }
        rows.append(cols);
    }

    QJsonObject frameObj;
    frameObj["pixels"] = rows;

    QJsonDocument doc(frameObj);
    return doc.toJson(QJsonDocument::Compact);
}

Frame::Frame(int width, int height, QJsonObject& frameObj) : width(width), height(height) {
    QJsonArray pixelsArray = frameObj["pixels"].toArray();

    pixels = new QColor*[height];
    for (int row = 0; row < height; row++) {
        pixels[row] = new QColor[width];
        QJsonArray rowObj = pixelsArray[row].toArray();
        for (int col = 0; col < width; col++) {
            QJsonObject colorObj = rowObj[col].toObject();
            QColor color(colorObj["r"].toInt(), colorObj["g"].toInt(), colorObj["b"].toInt(), colorObj["a"].toInt());
            pixels[row][col] = color;
        }
    }
}

QImage Frame::toImage() {
    QImage image(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            QColor color = getPixelColor(x, y);
            image.setPixelColor(x, y, color);
        }

    return image;
}
