/* 
 * File:   Job.cpp
 * Author: alex
 * 
 * Created on 20 gennaio 2013, 14.18
 */

# include "Job/Job.h"

using namespace std;

std::map<const std::string, tache*> *Job::tMap;

Job::Job() {
    Job::nTaches = 0;
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
            getline(myfile,this->finalizeCommand);
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
        else {
            getline(myfile,secondLine);
            tache* toInsert=createNewTache(line,secondLine);
            Job::addTacheToMap(toInsert);
        }
    }
       
}


void Job::addTacheToMap(tache* tache1){
    Job::tMap->insert(Mappair(tache1->name,tache1));
    //cout <<  "add to the map another tache " << tache1->name << "\n";
}

bool Job::testJobDeps(){
    bool result;
    std::map<const std::string, tache*>::iterator iter;
    std::string* strToReturn = new std::string("");
    for (iter = tMap->begin(); iter != tMap->end(); iter++){
        if (iter->second->completed == false){
            if (iter->second->testTacheDeps()){
                cout << iter->first;
            }
        }
    }
    return true;
}