/* 
 * File:   master.cpp
 * Author: alex
 * 
 * Created on 21 gennaio 2013, 12.35
 */


# include "master.h"

using namespace std;

master::master() {
}

master::master(const master& orig) {
}

master::~master() {
}

void master::mainMaster(int argc,char **argv, long id, long p){
        int done;
        char toOpen[256];
        Job* travail=new Job;
        if (argc == 1){
            strncpy(toOpen,"Makefile\0",9);
        }
        else {
            strncpy(toOpen,argv[1],256);
        }
        travail->createNewJob(toOpen);
        cout << "all tache are in tWaiting, ready to start\n"; 
        travail->testJobDeps();
        cout << "available taches are now in tAvailble\n";
        // here available are in tAvailable, others are in tWaiting
        travail->run(id,p);
        
        if (argc == 3){
                if (strcmp(argv[2],"clean") == 0){
                        travail->finalize();
                }
                
        }
        
    //    else cout << "Unable to open file";
}