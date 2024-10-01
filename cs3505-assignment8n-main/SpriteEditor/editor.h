#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <QColor>
#include "QtCore/qpoint.h"
#include "sprite.h"
/*
 * Editor class to manage editing actions within a sprite editing application.
 * Handles tool selection, color changes, canvas interactions, and file operations.
 * @authors: Noah Campbell, Will Black, Tanner Bergstrom, Tj Hess and Kevin Christiansen
 * @ version 3/31/2024
 * @reviewed by tj hess
 */

// Enum for available tools
enum class ToolType {
    Pen = 0,
    Eraser = 1,
    Fill = 2,
    EyeDropper = 3
};

class Editor : public QObject {
    Q_OBJECT
public:
    // Constructor and destructor
    Editor(int width, int height);
    ~Editor();

    /// @brief Converts mouse coordinates to canvas pixel coordinates.
    /// @param mouseCoords The QPointF representing mouse coordinates.
    /// @param canvasSize The QSize representing the canvas size.
    /// @return A QPointF containing the converted x and y pixel coordinates.
    QPoint convertMouseToPixel(QPointF mouseCoords, QSize canvasSize);

    /// @brief Adds an empty frame to the sprite.
    void addEmptyFrame();

    /// @brief Adds a frame with specific pixel data to the sprite.
    /// @param frame The frame to add.
    void addFrame(Frame& frame);
private:
    ToolType activeTool = ToolType::Pen; /// Currently selected tool.
    QColor currentColor = QColorConstants::Black; /// Currently selected color.
    Sprite* sprite; /// Pointer to the current sprite
    int currentFrameIndex = 0; /// Index of the current frame being displayed
    int currentPreviewFrame;
    bool showPreviewActualSize;
public slots:
    /// @brief Sets the active editing tool.
    /// @param tool The tool to be activated.
    void setActiveTool(ToolType tool) { activeTool = tool; }

    /// @brief Sets the editor's color.
    /// @param color The color to be set.
    void setColor(const QColor &color);

    /// @brief this is responsible for calling the correct tool on the frame and editing it accordingly
    /// @param QPointF that represents the mouse cordinates to be converted to pixle cords.
    /// @param QSize representing the canvas
    /// @param the boolean that represents if this is a dragg interaction or click
    void editFrame(const QPointF &mouseCoords, const QSize &canvasSize, bool draggTool);

    /// @brief this method creates a new sprite given a width and height
    /// @param width
    /// @param height
    void createNewSpriteSlot(int width, int height);

    /// @brief Saves the current sprite to a file.
    /// @param filename The name of the file to save to.
    void saveSlot(QString filename);

    /// @brief Loads a sprite from a file.
    /// @param filepath The path of the file to load from.
    void loadSlot(QString filepath);
    /// @brief Gets the frame that was selected and sends it as an image back to the view
    /// @param frameIndex The index of the frame that was selected
    void updateCurrentFrame(int frameIndex);

    /// @brief Removes the frame at frameIndex from the Sprite
    /// @param frameIndex The index of the frame to remove
    void removeFrameSlot(int frameIndex);

     /// @brief Duplicates a specific frame in the sprite.
    void duplicateFrame();

    /// @brief sends the updated frame images of the sprite to the preview
    void getFrames();
signals:
    /// @brief signal to send QImage frame from editor to the view
    /// @param frame to dispaly
    void sendPreviewFrame(QImage frame);

    /// @brief
    void getNextPreviewFrame();

    /// @brief sends a signal to the canvas to update the color
    /// @param color to send
    void colorChanged(const QColor &newColor);

    /// @brief signal to display the image on the appropriate fram button
    /// @param image to display on button
    /// @param index of button/frame number
    void sendFrameButtonImage(QImage frame, int index);

    /// @brief Emites a signal to the canvas when a frame is updated
    /// @param image to send to canvas
    void frameUpdated(QImage &frameDisplayed);

    /// @brief signal for when editor is loading a new sprite the view will insert the button
    /// this is exicuted as many frames as there are in the sprite
    /// @param the corisponding frame number
    void insertFrameButton(int frameNum);

    void addClonedImageToPreview(QImage image, int clonedImageIndex);

    void sendFrames(std::vector<QImage> &images);
};

#endif // EDITOR_H
