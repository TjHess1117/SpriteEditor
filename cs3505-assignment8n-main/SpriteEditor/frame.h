#ifndef FRAME_H
#define FRAME_H

#include <QString>
#include <QColor>
#include <QImage>
#include <QJsonObject>

/*
 * Frame class represents a single frame in a sprite, managing pixel data and providing
 * functionalities for pixel manipulation and JSON serialization.
 * @authors: Noah Campbell, Will Black, Tanner Bergstrom, Tj Hess and Kevin Christiansen
 * version 3/31/2024
 * @ reviewed by Noah Campbell
 */

class Frame {
public:
    // Constructors and destructors
    Frame(int width, int height);
    Frame(const Frame& other);
    ~Frame();
    Frame& operator=(Frame other);

    /// @brief Serializes the frame data to JSON format.
    /// @return A QString containing the JSON representation of the frame.
    QString toJson() const;
    Frame(int width, int height, QJsonObject& frameObj);

    /// @brief Gets the color of a specific pixel.
    /// @param pixelX The x-coordinate of the pixel.
    /// @param pixelY The y-coordinate of the pixel.
    /// @return The QColor of the specified pixel.
    QColor getPixelColor(int pixelX, int pixelY) const;

    /// @brief Sets the color of a specific pixel.
    /// @param pixelX The x-coordinate of the pixel to be set.
    /// @param pixelY The y-coordinate of the pixel to be set.
    /// @param color The color to set the pixel to.
    void setPixelColor(int pixelX, int pixelY, QColor color);

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    /// @brief toImage Use this method to turn a frame into a QImage.
    /// This QImage can be sent to the view for it to be displayed.
    /// @return QImage that represents the frame pixels
    QImage toImage();
    ///@brief duplicates frame
    Frame duplicateFrame();
private:
    QColor** pixels; // 2D array for storing pixel colors
    int width;       // Width of the frame
    int height;      // Height of the frame
};

#endif // FRAME_H
