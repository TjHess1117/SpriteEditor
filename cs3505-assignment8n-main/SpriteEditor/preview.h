#ifndef PREVIEW_H
#define PREVIEW_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QImage>
/*
 * the preview class is responsible for cycling throught the frames at the provided fps and
 * displaying it to the main window.
 * @authors: Noah Campbell, Will Black, Tanner Bergstrom, Tj Hess and Kevin Christiansen
 * @ version 3/31/2024
 * @ reviewd by tj hess
*/
class Preview : public QLabel
{
    Q_OBJECT
    public:
        /// @brief A default constructor for Preview objects.
        /// @param parent is the parent widget for 'this' Preview
        Preview(QWidget *parent = nullptr);
    private:
        /// Holds the frames of the current sprite
        std::vector<QPixmap> frames;

        /// Keeps track of the frame that is currently being displayed in the preview
        int currentPreview;
        int frameRate;
        bool displayActualSize;

        /// @brief Resets the frames in the preview when a new sprite is created or an old
        /// sprite is loaded.
        void resetPreviewFrames();
    public slots:
        /// @brief Receives an updated frame from the Editor and updates the frames that the Preview
        /// holds to include the new frame
        /// @param pixmap The pixmap of the updated frame
        /// @param frameIndex The index of the updated frame
        void receiveFrame(const QImage &image, int frameIndex);

        /// @brief Deletes the frame at frameIndex
        /// @param the index of the frame to delete
        void deleteFrame(int frameIndex);

        /// @brief Displays the preview by looping through each frame
        /// @param frameRate The number of different frames to display each second
        void loopPreview();
        /// @brief chatches the event that the fps slider chnages
        /// @param the frame rate to display things at.
        void fpsChanged(int frameRate);

        /// @brief starts the preview animation loop
        /// @param frameRate The number of different frames to display each second
        void startPreview(int frameRate);

        /// @brief this loads the next frame in the cycle to be displayed
        /// @param the QImage to dispaly
        void loadNewFrames(std::vector<QImage> &images);
        /// @brief this will add and empty frame the the preview to
        /// sync up the sprite when a new sprte is created
        void addEmptyFrame();

        /// @brief this method is responsible for adding the cloned image to the sequence of frames
        /// @param the image to clone
        /// @param the index to insert the cloned image
        void addClonedImageToPreview(QImage image, int clonedImageIndex);

        /// @brief a full reset of the preview for loding new sprites
        void resetPreview();
        /// @brief a event that cactchs and handles the actuaalSize button being pushed
        /// @param the bool that says if it was pushed or not
        void setDisplayActualSize(bool displayActualSize);
};

#endif // PREVIEW_H
