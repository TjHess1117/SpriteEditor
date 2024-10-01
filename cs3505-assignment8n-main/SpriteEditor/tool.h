#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QWidget>
#include "frame.h"
/*
 * the tool class is the static class for editing frames simply and effectivly
 * it has 4 diffrent tools for frame alterations and editor alterations
 * @authors: Noah Campbell, Will Black, Tanner Bergstrom, Tj Hess and Kevin Christiansen
 * @ version 3/31/2024
 *
 * @ Reviewed by: Tanner Bergstrom
*/
class Tool
{
public:
    /// @brief the pen alters one individule pixle
    /// @param the point to alter the image in pixle cords
    /// @param the color to alter the frame to
    /// @param the frame to alter
    static void pen(const QPoint &pixelPos, const QColor &penColor, Frame &subjectFrame);

    /// @brief the eraser alters one individule pixle
    /// @param the point to alter the image in pixle cords
    /// @param the frame to alter
    static void eraser(const QPoint &pixelPos, Frame &subjectFrame);

    /// @brief the eye drpper alters the editors current color by
    /// returning the color of the frame at the pixle cord
    /// @param the point to find the color of the frame
    /// @param the frame to find the color at the pixle position
    static QColor eyeDropper(const QPoint &pixelPos, const Frame &subjectFrame);

    /// @brief the fill tool fills in the area of selection on the frame
    /// @param the point to alter the image in pixle cords
    /// @param the color to alter the frame to
    /// @param the frame to alter
    static void fill(const QPoint &pixelPos, const QColor &fillColor, Frame &subjectFrame);
};

#endif // TOOL_H
