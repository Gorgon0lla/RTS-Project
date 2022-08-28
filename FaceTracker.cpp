
#include "FaceTracker.hpp"


    FaceTracker::FaceTracker()
    {
        std::string clas_path = "/home/filip/RTS-Project/haarcascade_frontalface_alt.xml";
        classificator = std::make_unique<faceClassifier>();
        classificator->location = clas_path;
        this->camera_nmb = 0;
        this->setScaleFactor(1.0); //required non zero scale factor
    }
    FaceTracker::FaceTracker(int cam_num, std::string classificator_loc)
    {
        classificator = std::make_unique<faceClassifier>();
        classificator->location = classificator_loc;
        this->camera_nmb = cam_num;
        this->setScaleFactor(1.0); //required non zero scale factor
    }
    FaceTracker::FaceTracker(int cam_num, std::string classificator_loc, double scaleFactor)
    {
        classificator = std::make_unique<faceClassifier>();
        classificator->location = classificator_loc;
        this->camera_nmb = cam_num;
        this->setScaleFactor(scaleFactor); //required non zero scale factor
    }
    bool FaceTracker::openCamera()
    {
        camera.open(this->camera_nmb);
        if (camera.isOpened())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool FaceTracker::prepareDetection()
    {
        //loading location of our network       
        return this->classificator->faceDetector.load(this->classificator->location);
    }
    std::vector<cv::Rect>& FaceTracker::getFaces()
    {
        return this->faces;
    }
    bool FaceTracker::detectFaces()
    {
        // reading frame for classificator
        if(this->camera.isOpened())
        {
            if(!this->camera.read(frame))
            {
                return false;
            }
            //Detecting the faces in 'image_with_humanfaces' matrix//
            this->classificator->faceDetector.detectMultiScale(this->frame,
            this->faces,
            this->scaleFactor,
            4,
            cv::CASCADE_SCALE_IMAGE,
            cv::Size(30, 30));

            return true;
        }
        else 
        {
            return false;
        }
    }
    void FaceTracker::setScaleFactor(double var)
    {
        scaleFactor = var;
    }
    void FaceTracker::printRect()
    {
        for (int i = 0; i < faces.size(); i++)
            { //for locating the face
                cv::Mat faceROI = frame(faces[i]);
                int x = faces[i].x;
                int y = faces[i].y;
                int h = y + faces[i].height;
                int w = x + faces[i].width;

                //Drawing a rectangle using around the faces//
                cv::rectangle(frame,
                cv::Point(x, y),
                cv::Point(w, h),
                cv::Scalar(255, 0, 255),
                2,
                8,
                0);
            }
        cv::imshow("Face Detection",this->frame);    
    }

