/* 
 * File:   slave.cpp
 * Author: alex
 * 
 * Created on 21 gennaio 2013, 12.35
 */

#include "slave.h"
#include "tache.h"

slave::slave() {
}

slave::slave(const slave& orig) {
}

slave::~slave() {
}

void slave::mainSlave(int argc,char **argv, long id, long p){
    bool end;
    while (end){
        tache *tache = new tache;
        end = tache->receiveTache(0,false);
        tache->run();
        tache->sendTache(0,true);
    }
}

