/* 
 * File:   master.cpp
 * Author: alex
 * 
 * Created on 21 gennaio 2013, 12.35
 */


# include "master.h"

using namespace std;

int master::dimension;
int master::id;
int* master::nodes;

master::master() {
}

master::master(const master& orig) {
}

master::~master() {
}

void master::mainMaster(int argc,char **argv, long id, long p){
        pthread_t bet;
        pthread_attr_t att;
        int done;
        char toOpen[256];
        master::dimension=p;
        master::id=id;
        master::nodes=(int*)malloc((sizeof(int)*p));
        memset(nodes,0,p);
        Job* travail=new Job;
        if (argc == 1){
            strncpy(toOpen,"Makefile\0",9);
        }
        else {
            strncpy(toOpen,argv[1],256);
        }
        travail->createNewJob(toOpen);
        
        cout << "all tache are in tWaiting, ready to start\n"; 
        //cout << "available taches are now in tAvailble\n";
        // here available are in tAvailable, others are in tWaiting
        //travail->testJobDeps();
        travail->ComputeDependant();
        travail->run(id,p);
        
        
        
        if (argc == 3){
                if (strcmp(argv[2],"clean") == 0){
                        travail->finalize();
                }
                
        }
}
        
        void master::SetBusy(int id){
            //cout << "SENDER: SetBusy callded for node " << id << "\n";
            master::nodes[id]=1;
        }
        
        void master::SetFree(int id){
            //cout << "SENDER: SetFree callded for node " << id << "\n";
            master::nodes[id]=0;
        }
        
        int master::GetFree(){
            int i=1;
            while (i<=dimension && nodes[i]==1){
                i++;
            }
            if (i==dimension){
                //cout << "SENDER: GetFree --> "<< 0 << " returned\n"; 
                return 0;
            }
            else {
                //cout << "SENDER: GetFree --> "<< i << " returned\n";
                return i;
            }
        }
        
