#pragma once
#include "TrackingMenager.hpp"


void setUpTracker();
bool askForFaces();
int detectFace();
void* real_time_thread(void*);
//std::unique_ptr<TrackingMenager> trackingMenager = nullptr;
//std::unique_ptr<FaceTracker> faceTracker = nullptr;
std::vector<std::vector<cv::Rect>> savedFaces;
sharedFaces shared_faces;
TrackingMenager trackingMenager;
FaceTracker faceTracker;