#include "tool.h"
#include "frame.h"
#include <stack>
/// @reviewed by tanner
void Tool::pen(const QPoint &pixelPos, const QColor &penColor, Frame &subjectFrame) {
    subjectFrame.setPixelColor(pixelPos.x(), pixelPos.y(), penColor);
}
void Tool::eraser(const QPoint &pixelPos, Frame &subjectFrame) {
    subjectFrame.setPixelColor(pixelPos.x(), pixelPos.y(), QColor(Qt::transparent));
}

QColor Tool::eyeDropper(const QPoint &pixelPos, const Frame &subjectFrame) {
    return subjectFrame.getPixelColor(pixelPos.x(), pixelPos.y());
}

void Tool::fill(const QPoint &pixelPos, const QColor &fillColor, Frame &subjectFrame) {
    // recursivly run fill untill there are no pixls near "fill pixle" that have already been set to a color
    int pixelX = pixelPos.x();
    int pixelY = pixelPos.y();

    QColor initialColor = subjectFrame.getPixelColor(pixelX, pixelY);
    if (initialColor == fillColor) return;

    std::stack<QPoint> points;
    points.push(pixelPos);

    while (!points.empty()) {
        pixelX = points.top().x();
        pixelY = points.top().y();
        points.pop();

        if (subjectFrame.getPixelColor(pixelX, pixelY) == initialColor)
            subjectFrame.setPixelColor(pixelX, pixelY, fillColor);
        else continue;

        // Left case
        if (pixelX > 0) points.push(QPoint(pixelX - 1, pixelY));
        // Top case
        if (pixelY > 0) points.push(QPoint(pixelX, pixelY - 1));
        // Right case
        if (pixelX < subjectFrame.getWidth() - 1) points.push(QPoint(pixelX + 1, pixelY));
        // Bottom case
        if (pixelY < subjectFrame.getWidth() - 1) points.push(QPoint(pixelX, pixelY + 1));
    }
}
