
#include "FaceMonitorFunc.hpp"
#ifndef FACE_MONITOR_FUNC_CPP
#define FACE_MONITOR_FUNC_CPP

void setUpTracker(std::string path)
{
    //trackingMenager = std::make_unique<faceMonitor::TrackingMenager>();
    //faceTracker = std::make_unique<faceMonitor::FaceTracker>();
 
}

bool askForFaces()
{
    try
    {
        shared_faces.faces = faceTracker.getFaces();
        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }   
}

int detectFace()
{
    if (faceTracker.detectFaces())
    {
        const std::lock_guard<std::mutex> lock(shared_faces.mutex);
        if (askForFaces())
        {
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return 1;
    }
}

void *real_time_thread(void* )
{
    void* ret;
    int result = detectFace();

        if (result == 0)
    {
        savedFaces.push_back(shared_faces.faces);
        if(savedFaces.size() >= 20)
        {
            savedFaces.erase(savedFaces.begin());
            //logger inf
        }
        //logger inf
    }
    else if( result == 1)
    {
            // logger inf
    }
    else if( result == 2)
    {
        //logger inf
    }
    return ret;
}




#endif

int main()
{
    
    std::cout<<"Hello there"<<std::endl;
    std::string classificator_path = "/home/filip/RTS-Project/haarcascade_frontalface_alt.xml";
    setUpTracker();

    

    
    return 0;
}



