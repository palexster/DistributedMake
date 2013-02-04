/* 
 * File:   slave.cpp
 * Author: alex
 * 
 * Created on 21 gennaio 2013, 12.35
 */

#include "slave.h"


using namespace std;

slave::slave() {
}

slave::slave(const slave& orig) {
}

slave::~slave() {
}

void slave::mainSlave(int argc,char **argv, long id, long p){
    system("mkdir /tmp/alex"+id);
    chdir("/tmp/alex");
    bool end=true;
    bool status;
    while (end == true){
        tache *t = new tache;
        cout << "SLAVE" << id << ": Waiting to receive another task"<< id << "\n";
        end = t->receiveTache(0,false,id);
        if (end==false){
            break;
        }
        cout << "SLAVE" << id << ": Starting running tache " << t->name << "\n";
        status = t->run(id);
        if (status == false){
            cout << "SLAVE" << id << ": Houston, we have a problem\n";
//            cout << "SLAVE" << id << ": ID of the slave" << id <"\n"; 
//            cout << "SLAVE" << id << ": The TACHE " << t->name <"\n";
           
        }
        t->sendTache(0,true);
        delete t;
        //end=true;
    }
    cout << "SLAVE" << id << ": I'm exiting"<< id << "\n";
    return;
}

