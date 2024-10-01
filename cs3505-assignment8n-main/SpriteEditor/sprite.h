#ifndef SPRITE_H
#define SPRITE_H

#include "frame.h"
#include <QString>
#include <QJsonObject>
/*
 * Sprite class represents an animation sprite, managing a collection of frames,
 * with functionalities for frame manipulation and JSON serialization.
 * @authors: Noah Campbell, Will Black, Tanner Bergstrom, Tj Hess and Kevin Christiansen
 * @ version 3/31/2024
 *
 * @ Reviewed by: Tanner Bergstrom
*/

class Sprite
{
    public:
        /// @brief Width and Height constructor for Sprite objects
        Sprite(int width, int height);

        /// @brief Json constructor
        Sprite(QJsonObject& spriteObj);

        /// @brief Copy constructor for Sprite objects
        Sprite(const Sprite& other);

        /// @brief Assignment operator for Sprite object
        Sprite& operator=(Sprite other);

        /// @brief Destructor for Sprite objects
        ~Sprite();

        /// @brief Retrieves a reference to a specific frame by index.
        /// @param index The index of the frame to retrieve.
        /// @return Reference to the Frame object at the specified index.
        Frame& getFrame(int index);

        /// @brief Serializes the sprite data to JSON format.
        /// @return A QString containing the JSON representation of the sprite.
        QString toJson() const;

        /// @brief Sets the current color
        /// @param Color to set the current color to
        /// @param Index of the frame being edited
        /// @param pixel X coordinate
        /// @param pixel Y coordinate
        void setPixelColor(int frameIndex, QColor color, int pixelX, int pixelY);

        /// @brief Gets the color of the pixel clicked on
        /// @param Index of the current frame
        /// @param pixel X coordinate
        /// @param pixel Y coordinate
        /// @return Color of the pixel
        QColor getPixelColor(int frameIndex, int pixelX, int pixelY);

        /// @brief insertFrame
        /// @param index
        void insertFrame(Frame& frame, int index);

        /// @brief Adds a new frame to the sprite, copying the provided frame.
        /// @param frame The frame to add to the sprite.
        void pushFrame(Frame& frame);

        /// @brief Removes a frame at a specified index.
        /// @param index The index of the frame to remove.
        void eraseFrame(int index);

        /// @brief Getter for width of Sprite
        int getWidth() { return width; }

        /// @brief Getter for height of Sprite
        int getHeight() { return height; }

        /// @brief Gets the current frame count of the sprite
        int getFrameCount() { return frames.size(); }
    private:
        // Holds the Frames that make up the Sprite
        std::vector<Frame> frames;
        // Width of the Sprite
        int width;
        // Height of the Sprite
        int height;
};

#endif // SPRITE_H
