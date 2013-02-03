/* 
 * File:   slave.cpp
 * Author: alex
 * 
 * Created on 21 gennaio 2013, 12.35
 */

#include "slave.h"
#include "tache.h"

using namespace std;

slave::slave() {
}

slave::slave(const slave& orig) {
}

slave::~slave() {
}

void slave::mainSlave(int argc,char **argv, long id, long p){
    bool end=true;
    while (end){
        tache *t = new tache;
        cout << "SLAVE: Waiting to receive another task"<< id << "\n";
        end = t->receiveTache(0,false);
        cout << "SLAVE: Starting running tache " << t->name << "\n";
        t->run();
        t->sendTache(0,true);
        delete t;
        end=true;
    }
}

