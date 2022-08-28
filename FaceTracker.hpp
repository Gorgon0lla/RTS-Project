#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
//OpenCV libaries
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>



    struct faceClassifier //structure for classificator
    {
        std::string location;
        cv::CascadeClassifier faceDetector;
    };


    class FaceTracker
    {
        private:

            std::unique_ptr<faceClassifier> classificator;
            std::vector<cv::Rect> faces;
            double scaleFactor;
            cv::Mat frame;
            cv::VideoCapture camera;
            std::string classificator_location;
            int camera_nmb;

        public:
            FaceTracker();
            FaceTracker(int ,std::string);
            FaceTracker(int, std::string, double);
            ~FaceTracker(){}; // empty destructor
            bool openCamera();
            bool prepareDetection();
            bool detectFaces();
            std::vector<cv::Rect>& getFaces();
            void setScaleFactor(double var);
            void printRect();
    };
