
#include "TrackingMenager.hpp"



    TrackingMenager::TrackingMenager() 
    { 
        Tracker = std::make_unique<FaceTracker>();
        shared_faces = std::make_shared<sharedFaces>();
    }
    TrackingMenager::TrackingMenager(std::string classifier_path)
    {
        Tracker = std::make_unique<FaceTracker>(0,
                                                classifier_path,
                                                 1.2);
        shared_faces = std::make_shared<sharedFaces>();
        

    }

    bool TrackingMenager::askForFaces()
    {
        try
        {
          shared_faces->faces = Tracker->getFaces();
          return true;
        }
        catch(const std::exception& e)
        {
            return false;
        }   
    }

    int TrackingMenager::faceDetect()
    {
        if (this->Tracker->detectFaces())
        {
            const std::lock_guard<std::mutex> lock(shared_faces->mutex);
            if (this->askForFaces())
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

    void *TrackingMenager::monitorThread(void* )
    {
        void* ret;
        int result = this->faceDetect();
        if (result == 0)
        {
            savedFaces.push_back(shared_faces->faces);
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

    void* TrackingMenager::monitorThread_ptr(void* pv)
    {
        thread_fun_args* tf_args = static_cast<thread_fun_args*>(pv) ;
        TrackingMenager* This = tf_args->This ;
        void* args = tf_args->actual_arg ;
        void* result = This->monitorThread( args );
        delete tf_args ;
        return result ;
    }

    bool TrackingMenager::startTracking_timer()
    {
        int status;

        if(!this->prepareTracking())
        {
            return false;
        }
        //pthread_attr_init(&main_monitor_attr);
        //pthread_attr_setschedpolicy(&main_monitor_attr,SCHED_FIFO);
        // Define the timer specification
        // One second till first occurrence
        this->monitor_timer_spec.it_value.tv_sec = 1;
        this->monitor_timer_spec.it_value.tv_nsec = 0;
        // setting interval
        this->monitor_timer_spec.it_value.tv_sec = 1;
        this->monitor_timer_spec.it_value.tv_nsec = 3000;
        // Clear the sa_mask
        sigemptyset(&this->monitorSignalAction.sa_mask);
        // set the SA_SIGINFO flag to use the extended signal-handler function
        this->monitorSignalAction.sa_flags = SA_SIGINFO;

        // Define sigaction method
        // This function will be called by the signal
        this->monitorSignalAction.sa_sigaction = TrackingMenager::real_time_thread;


       /* if ( (status = pthread_create(CLOCK_REALTIME, &monitor_timer_ev, &monitor_timer) )
        {
            return false;
        }
        else
        {
            return true;
        } */
        return false;
    }
    
    bool TrackingMenager::startTracking()
    {
        int status;

        if(!this->prepareTracking())
        {
            return false;
        }
        pthread_attr_init(&main_monitor_attr);
        pthread_attr_setschedpolicy(&main_monitor_attr,SCHED_FIFO);


        if (status = pthread_create(&main_monitor_thread, &main_monitor_attr, monitorThread_ptr, NULL) )
        {
            return false;
        }
        else
        {
            return true;
        }
    }


    bool TrackingMenager::prepareTracking()
    {
        if(Tracker->openCamera() && Tracker->prepareDetection())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

