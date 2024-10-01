#include "preview.h"
#include <QSignalBlocker>
#include <QTimer>
/// @reviewed by tj hess
Preview::Preview(QWidget *parent) : QLabel(parent) {
    QPixmap pixmap;
    frames.push_back(pixmap);
    currentPreview = 0;
    displayActualSize = false;
}

void Preview::receiveFrame(const QImage &image, int frameIndex) {
    QPixmap pixmap = displayActualSize ? QPixmap::fromImage(image) : QPixmap::fromImage(image.scaled(this->size(), Qt::KeepAspectRatio));

    const QSignalBlocker blocker(this);
    frames.at(frameIndex) = pixmap;
}

void Preview::resetPreviewFrames() {
    const QSignalBlocker blocker(this);
    frames.clear();
    currentPreview = 0;
}

void Preview::deleteFrame(int frameIndex) {
    if (frameIndex < 0 || (int)frames.size() <= frameIndex)
        throw std::out_of_range("Index is out of range. Thrown from the Preview class.");
    const QSignalBlocker blocker(this);
    frames.erase(frames.begin() + frameIndex);
    currentPreview = 0;
}

void Preview::loopPreview() {
    if (currentPreview == ((int)frames.size() - 1))
        currentPreview = 0;
    else
        currentPreview++;

    setPixmap(frames.at(currentPreview));

    int millisecondsPerFrame = 1000 / frameRate;
    QTimer::singleShot(millisecondsPerFrame, this, [this]{this->loopPreview();});
}

void Preview::startPreview(int frameRate) {
    this->frameRate = frameRate;
    int millisecondsPerFrame = 1000 / frameRate;
    QTimer::singleShot(millisecondsPerFrame, this, [this]{this->loopPreview();});
}

void Preview::fpsChanged(int frameRate) {
    const QSignalBlocker blocker(this);
    this->frameRate = frameRate;
}

void Preview::addEmptyFrame() {
    frames.push_back(QPixmap());
}

void Preview::loadNewFrames(std::vector<QImage> &images) {
    const QSignalBlocker blocker(this);
    resetPreviewFrames();
    if (displayActualSize) {
        for (const QImage &image : images){
            frames.push_back(QPixmap::fromImage(image));
        }
    }
    else{
        for (const QImage &image : images){
            frames.push_back(QPixmap::fromImage(image.scaled(size(), Qt::KeepAspectRatio)));
        }
    }

}

void Preview::addClonedImageToPreview(QImage image, int clonedImageIndex) {
    QPixmap pixmap = displayActualSize ? QPixmap::fromImage(image) : QPixmap::fromImage(image.scaled(this->size(), Qt::KeepAspectRatio));

    const QSignalBlocker blocker(this);
    frames.insert(frames.begin() + clonedImageIndex, pixmap);
}

void Preview::resetPreview() {
    resetPreviewFrames();
    frames.push_back(QPixmap());
}

void Preview::setDisplayActualSize(bool displayActualSize) {
    Preview::displayActualSize = displayActualSize;
}
