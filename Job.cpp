

/* 
 * File:   Job.cpp
 * Author: alex
 * 
 * Created on 20 gennaio 2013, 14.18
 */

# include "Job/Job.h"
#include "master.h"
#include "slave.h"

using namespace std;

std::map<const std::string, tache*> *Job::tMap;
std::map<const int, std::vector<string>* > *Job::FileCache;
std::deque<tache*> *Job::tAvailable;
std::deque<tache*> *Job::tWaiting;

Job::Job() {
    Job::nTaches = 1;
    tMap = new std::map<const std::string, tache*>;
    tAvailable = new std::deque<tache*>;
    Job::FileCache = new std::map<const int, std::vector<string>* >;
}

Job::Job(const Job& orig) {
}


Job::~Job() {
}

void Job::createNewJob(std::string name){
    std::fstream myfile(name.c_str(), ios::in);
    string line,firstLine,secondLine;
    int dimension;
    int i=0;
    // each task is represented on two different lines
    // the first one represents name : \t dependencies
    // the second one the proper command to execute
    // read the line about all different task
    // HERE FIRST LINE
    if (myfile.good()){
            getline(myfile,line);
            vector<string> tokenFirst = tokenize(line," ");
            this->id=Job::getNewId();
            vector<string> tokens = tokenize(tokenFirst[0],":");
            cout << tokens[0] << " \n";
            this->name = tokens[0];
            cout << this->name << "\n";
            tokenFirst.erase(tokenFirst.begin());
            dimension = tokenFirst.size();
            while (i != dimension){
                string dep = tokenFirst[i];
                cout << dep << "\n";
                this->dependencies.push_back(dep);
                i++;
            }
//            delete(&name);
//            delete(&tokens);
    }
    // HERE SECOND LINE
        if (myfile.good()){
            //getline(myfile,this->finalizeCommand);
            getline(myfile,line);    
//            if (line.substr(0).compare("\t")){
//                 this->command=line.substr(1,line.length());
//                }
//            else {
                this->command = line;
//            }
            this->completed=false;
            cout << this->command;
    }
    //TachePair pair(this->name,this);
    string toAdd = this->name;
    addTacheToMap(this);
    this->init = false;
    this->done = false;
    //Read the subtaches
    while(myfile.good()){
        getline(myfile,line); //first line of the tache
        if(line.length() == 0){  // Skip any blank lines
                continue;
        }
        else if(line[0] == '#'){  // Skip any comment lines
                continue; 
        }
        else if(line.compare("clean:") == 0){
            getline(myfile,this->finalizeCommand);
        }
        else if(line.compare("check:") == 0){
            getline(myfile,this->testCommand);
        }
        else {
            getline(myfile,secondLine);
            tache* toInsert=createNewTache(line,secondLine);
            Job::addTacheToMap(toInsert);
            putInWaiting(toInsert);
            this->nTaches++;
        }
    }
    putInWaiting(this);
}


void Job::addTacheToMap(tache* tache1){
    Job::tMap->insert(Mappair(tache1->name,tache1));
    //cout <<  "add to the map another tache " << tache1->name << "\n";
}

bool Job::testJobDeps(){
    bool result;
    long i=0;
    if (tWaiting->size() == 0){
        return false;
    }
    long size = tWaiting->size();
    while (i<size){
        tache *toEvaluate = tWaiting->front();
        tWaiting->pop_front();
        if (toEvaluate->completed == false){
            if (toEvaluate->testTacheDeps()){
             scheduleNewTache(toEvaluate);
            }
            else {
                putInWaiting(toEvaluate);
            }
        }
        i++;
    }
    return true;
}

tache* Job::getNewTache(){
    tache *toReturn;
    if (tAvailable->size() == 0 ){
        toReturn = NULL;
    }
    else {
        toReturn = tAvailable->front();
        tAvailable->pop_front();
    }
    return toReturn;
}

void Job::scheduleNewTache(tache* toAdd){
    if (tAvailable == NULL){
        tAvailable = new std::deque<tache*>;
    }
    tAvailable->push_back(toAdd);
}
tache* Job::getWaitingTache(){
    tache *toReturn;
    if (tWaiting->front() == NULL ){
        toReturn = NULL;
    }
    else {
        toReturn = tWaiting->front();
        tWaiting->pop_front();
    }
    return toReturn;
}

void Job::putInWaiting(tache* toAdd){
    if (tWaiting == NULL){
        tWaiting = new std::deque<tache*>;
    }
    tWaiting->push_back(toAdd);
}

bool Job::run(const long id, const long p){
    int total = 0;
    int totalReceived=0;
    int target_host=1;
    string mapping[p];
    int busy[p];
    int candidate;
    memset(busy,0,p);
    MPI::Status status;
    cout << "tAvailable status: " <<tAvailable->size() << "\n";
    cout << "nTaches status: " << this->nTaches << "\n";
    cout << "p status: " << p << "\n";

    while(target_host<p){
        tache* toRun = getNewTache();
        if (toRun == NULL){
            cout << "Unexpected behaviour\n";
            return false;
        }
//        cout << "SENDER: tAvailable status: " <<tAvailable->size() << "\n";
//        cout << "SENDER: total status: " << total << "\n";
//        cout << "SENDER: totalReceived status: " << totalReceived << "\n";
//        cout << "SENDER: ntaches status: " << this->nTaches << "\n";
//        cout << "SENDER:New Tache taken " << toRun->name << " \n";
//        cout << "SENDER: Scheduling " << toRun->name << " to machine " << target_host << "\n";
        toRun->sendTache(target_host,false);
        mapping[target_host]=toRun->name;
        master::SetBusy(target_host);
        target_host++;
        total++;
       }
    
    while( totalReceived < this->nTaches-1){
      
       
//       if(totalReceived < total && totalReceived < nTaches){
           cout << "SENDER: tAvailable status: " <<tAvailable->size() << "\n";
           cout << "SENDER: total status: " << total << "\n";
           cout << "SENDER: totalReceived status: " << totalReceived << "\n";
           cout << "SENDER: ntaches status: " << this->nTaches << "\n";
          if (MPI::COMM_WORLD.Iprobe(MPI::ANY_SOURCE,MPI::ANY_TAG,status)){
           int tag = status.Get_tag();
           int source = status.Get_source();
           int dim = status.Get_count(MPI::CHAR);
//           cout << "SENDER: Receiving results from" << source <<" \n";
           if (tag != RESULT){
               cout << "Protocol error!";
               return false;
           }
           else {
//               cout << "SENDER: Completing tache " << tMap->at(mapping[source])->name << "\n";
               tache *t = tMap->at(mapping[source]);
               t->completed=true;
               char *buff=(char*)malloc(sizeof(char)*dim+1);
               MPI::COMM_WORLD.Recv(buff,dim,MPI::CHAR,MPI::ANY_SOURCE,MPI::ANY_TAG);
               savefile(buff,t->name);
               this->notifyDependant(t);
//               if ( tAvailable->size() < p){
               //cout << "Refill 1\n"  ;
//               }
               totalReceived++;
               master::SetFree(source);
               free(buff);
           }
//           if (tAvailable->size() == 0){
//                this->testJobDeps();
//           }
//           
           tache* toRun;
           if ( tAvailable->size() > 0){
               toRun = getNewTache();
               toRun->sendTache(source,false);
               mapping[source]=toRun->name;
               master::SetBusy(source);
               total++;
           }
           
      }
//        if ( tAvailable->size() < 3*p){
        //cout << "Refill 2\n"  ;
        //this->testJobDeps();
//        }
           
//           while (tAvailable->size() > 0){
//               tache* toRun = getNewTache();
//                       if (toRun == NULL){
//                                cout << "Unexpected behaviour\n";
//                                return false;
//                }
//               if ((candidate = master::GetFree()) == 0 ){
//                   break;
//               }
//               toRun->sendTache(candidate,false);
//               mapping[candidate]=toRun->name;
//               master::SetBusy(candidate);
//               total++;
//           }
       
        
    }
    this->finalizeCommand;
    cout << "The job is done! Bye!";
    this->signalEnd(p,id);
}



bool Job::finalize(){
    system(this->finalizeCommand.c_str());
    return true;
}

void Job::signalEnd(const long p,const long id){
    int i=1;
    string message("die");
    while(i<p){
        sendData(message,i,DIE);
        i++;
    }
};


void Job::ComputeDependant(){
    map<string, tache*>::iterator p;
    vector<string>::iterator it;
   //cout << "Map DIMENSION " << tMap->size() << "\n";
   for(p = tMap->begin(); p != tMap->end(); p++) {
       for(it = p->second->dependencies.begin(); it != p->second->dependencies.end(); it++) {
            if (tMap->count(*it) == 0 ){
                p->second->TobeTaches--;
                cout << p->first << "\n";
                cout << "ToBe DIMENSION " << p->second->TobeTaches << "\n";
                if (p->second->TobeTaches == 0){
                    scheduleNewTache(p->second);
                }
                continue;
            }        
           tMap->at(*it)->dependant.push_back(p->first);
           cout << p->first << "é dipendente da" << *it << "\n";
       }
     }
}


//#ifdef LOCAL    
//    while(total < this->nTaches){
//        tache* toRun = getNewTache();
//        toRun->run();
//        toRun->completed=true;
//#endif LOCAL
//#ifdef MPI

bool Job::CheckPresenceOnHost(int target_host,string file){
    //cout << "SENDER: Cache result for "<< target_host << "\n";
    if (FileCache->count(target_host) == 0){
        vector<string> *listFile = new vector<string>;
        listFile->push_back(file);
        FileCache->insert(pair<int, vector<string>* >(target_host, listFile));
        return false;
    }
    else {
        vector<string> *listFile = FileCache->at(target_host);
        for (std::vector<string>::iterator it = listFile->begin() ; it != listFile->end(); ++it){
            if (file.compare(*it) == 0){
                return true;
            }
        }
        listFile->push_back(file);
        //map.insert(pair<int, vector<string>* >(target_host, listFile));
        return false;
    }
}

void Job::notifyDependant(tache *t){
    vector<string>::iterator it;
    //cout << "tache name " << t->name << "\n";
    for(it = t->dependant.begin(); it != t->dependant.end(); it++) {
        //cout << "Mes dependants " << (*it) << "\n";
    }    
    for(it = t->dependant.begin(); it != t->dependant.end(); it++) {
        if (tMap->count(*it) == 0 ){
            continue;
        }
           tMap->at(*it)->TobeTaches--;
           if (tMap->at(*it)->TobeTaches == 0){
               Job::scheduleNewTache(tMap->at(*it));
           }
       }
}

void Job::checkStarters(){
    map<string,tache*>::iterator it;
        for(it =tMap->begin(); it != tMap->end(); it++) {
            if ((*it).second->TobeTaches == 0 ){
                cout << "I'm ready from the begin " << (*it).second->name <<"!\n";
            }
        }
}