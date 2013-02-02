/* 
 * File:   Job.cpp
 * Author: alex
 * 
 * Created on 20 gennaio 2013, 14.18
 */

# include "Job/Job.h"

using namespace std;

std::map<const std::string, tache*> *Job::tMap;
std::deque<tache*> *Job::tAvailable;
std::deque<tache*> *Job::tWaiting;

Job::Job() {
    Job::nTaches = 1;
    tMap = new std::map<const std::string, tache*>;
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
            //this->name = this->name.substr(this->name.length()-2);
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
    //for (iter = tWaiting->begin(); iter != tWaiting->end(); ++iter){
    //control = tWaiting->end();
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
#ifdef VERBOSE             
             cout << "Tache id " << toEvaluate->command << " \n";
             cout << "Tache ready: " << toEvaluate->command << " \n";
             cout << "Dimension of tWaiting " << tWaiting->size() << " \n";
#endif             
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
    int target_host=id;
#ifdef VERBOSE
    cout << "To do: " << this->nTaches << " \n";
#endif
//#ifdef LOCAL    
//    while(total < this->nTaches){
//        tache* toRun = getNewTache();
//        toRun->run();
//        toRun->completed=true;
//#endif LOCAL
//#ifdef MPI
       while(tAvailable->size() == 0 || total < this->nTaches){
        tache* toRun = getNewTache();
        target_host= target_host++ % p;
        cout << "Scheduling " << toRun->getId() << " to machine " << target_host;
        toRun->sendTache(target_host,false);
        toRun->completed=true;
//#endif MPI        
#ifdef VERBOSE
        cout << "Done: " << total+1 << " \n";
#endif
        total++;
        this->testJobDeps();
    }
}

bool Job::finalize(){
    system(this->finalizeCommand.c_str());
    return true;
}

