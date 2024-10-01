#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QObject>
#include <QWidget>
/*
 * the canvas class is responsible for tracking the mouse events
 * and sending them to the editor for individual frame adjustments.
 * it is also for sending the canvas image to corispond to what the user inputs
 * @authors: Noah Campbell, Will Black, Tanner Bergstrom, Tj Hess and Kevin Christiansen
 * @ version 3/31/2024
 * @reviewed by : Kevin Christiansen
*/
class Canvas : public QLabel
{
    Q_OBJECT
    public:
        /// A default constructor for Canvas objects.
        /// @param parent is the parent widget for 'this' Canvas
        Canvas(QWidget *parent = nullptr);
    private:
        /// Represents the image that the user is currently editing
        QImage image;

        /// @brief mouseMoveEvent Keeps track of the mouse moving
        /// @param event The mouse event
        void mouseMoveEvent(QMouseEvent *event) override;

        /// @brief mousePressEvent Keeps track of the mouse being pressed
        /// @param event The mouse event
        void mousePressEvent(QMouseEvent *event) override;

        /// \brief mouseReleaseEvent Keeps track of the mouse being released
        /// \param event The mouse event
        void mouseReleaseEvent(QMouseEvent *event) override;

        /// \brief resizeEvent Resizes the canvas
        /// \param event The resize event
        void resizeEvent(QResizeEvent *event) override;
    signals:
        /// \brief mouseAction Sends a signal that holds information about the mouse event
        /// \param mousePos The position of the mouse
        /// \param canvasSize The size of the canvas
        /// \param mouseDragging Keeps track of whether the mouse is currently dragging
        void mouseAction(const QPointF &mousePos, const QSize &canvasSize, bool mouseDragging);

    public slots:
        /// \brief setImage Sets the image that the canvas is currently holding
        /// \param iamge The image that the canvas will hold
        void setImage(QImage &iamge);
};

#endif // CANVAS_H
