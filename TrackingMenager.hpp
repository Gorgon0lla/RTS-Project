#pragma once
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <atomic>
#include <mutex>
#include <thread>
#include <signal.h>
#include <time.h>
#include <memory.h>
#include "FaceTracker.hpp"


    struct sharedFaces
    {
        std::mutex mutex;
        std::vector<cv::Rect> faces;
    };

    class TrackingMenager
    {

        private:
            
            void* monitorThread(void*);
            static void* monitorThread_ptr(void* pv); // real time
        // for timer func    
            static void real_time_thread(int sigNumb, siginfo_t *si, void *uc)
            {
                TrackingMenager *ptrTrackingMenager = 
                    reinterpret_cast<TrackingMenager *> (si->si_value.sival_ptr);
                        int result = ptrTrackingMenager->faceDetect();
                if (result == 0)
                {
                    ptrTrackingMenager->savedFaces.push_back(ptrTrackingMenager->shared_faces->faces);

                    if(ptrTrackingMenager->savedFaces.size() >= 20)
                    {
                    ptrTrackingMenager->savedFaces.erase(ptrTrackingMenager->savedFaces.begin());
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
            }


            std::unique_ptr<FaceTracker> Tracker;
            // threads attributes
            pthread_attr_t main_monitor_attr;
            pthread_attr_t viewer_attr;
            // Threads
            pthread_t main_monitor_thread;
            pthread_t viewer_thread;
            // FOR TIMERS - CURRENTLY NOT USED
            //timers
            timer_t monitor_timer;
            timer_t viewer_timer;
            //signal variables
            struct sigevent monitor_timer_ev;
            struct sigevent viewer_timer_ev;
            //timers specification
            struct itimerspec viewer_timer_spec;
            struct itimerspec monitor_timer_spec;
            // signal actions
            struct sigaction monitorSignalAction;
            struct sigaction viewerSignalAction;
            //signal blocking set
            sigset_t monitorigBlockSet;
            sigset_t viewerSigBlockSet;
            // Common thread variables
            std::shared_ptr<sharedFaces> shared_faces; //shared variable betweeen threads
            std::vector<std::vector<cv::Rect>> savedFaces;
            bool askForFaces();
            struct thread_fun_args
            {
                TrackingMenager* This ;
                // other args as required; in this case
                void* actual_arg ;
                thread_fun_args( TrackingMenager* t, void* p ) 
                : This(t), actual_arg(p) {}
            };



        public:
            TrackingMenager();
            TrackingMenager(std::string);
            bool startTracking();
            bool startTracking_timer();
            bool prepareTracking();
            int faceDetect();





    };


