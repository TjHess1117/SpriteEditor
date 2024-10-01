#include "canvas.h"
#include <QMouseEvent>
/// @reviewed by kevin
Canvas::Canvas(QWidget *parent) : QLabel(parent) {
    setPalette(QColorConstants::White);
    image = QImage(size(), QImage::Format_ARGB32);
    image.fill(Qt::GlobalColor::transparent);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    emit mouseAction(event->position(), size(), true);
}
void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (rect().contains(event->position().toPoint()))
        emit mouseAction(event->position(), size(), true);
}
void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseAction(event->position(), size(), false);
}

void Canvas::resizeEvent(QResizeEvent *event) {
    QWidget *parent = parentWidget();
    if (!parent) return;

    int newWidth = event->size().width();
    int newHeight = event->size().height();

    if (newWidth < newHeight)
        if (newHeight < parent->width() * 0.5)
            setGeometry(pos().x(), pos().y(), newHeight, newHeight);
        else setGeometry(pos().x(), pos().y(), newWidth, newWidth);
    else
        if (newHeight < parent->height() - 150)
            setGeometry(pos().x(), pos().y(), newWidth, newWidth);
        else setGeometry(pos().x(), pos().y(), newHeight, newHeight);

    setImage(image);
}

void Canvas::setImage(QImage &image) {
    if (image.size() != size())
        Canvas::image = image.scaled(size(), Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(Canvas::image));
}
