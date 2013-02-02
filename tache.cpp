/* 
 * File:   tache.cpp
 * Author: alex
 * 
 * Created on 21 gennaio 2013, 12.35
 */

//#include <ompi/mpi/cxx/constants.h>

#include "tache.h"

using namespace std;

long tache::tID = 0;

tache::tache() {
    this->id=0;
}

tache::tache(const tache& orig) {
    bool completed=false;
    std::deque<int> tache;
    
    // List -->  
    // N° tache
    // 
}

tache::~tache() {
}

tache* tache::createNewTache(string firstLine,string secondLine){
    tache* toAdd=new tache;
    vector<string> firstTokens = tokenize(firstLine,":");
    toAdd->id=getNewId();
    vector<string> depTokens; 
    //vector<string> tokens = tokenize(firstTokens[0],":");
    //cout << tokens[0] << " \n";
    toAdd->name=firstTokens[0];
    toAdd->completed=false;
    //unsigned pos = toAdd->name.find("\t");
    cout << "tache name: " << toAdd->name << "\n";
    //toAdd->name=toAdd->name.substr(0,pos);
    if (firstTokens.size() > 1){
    if (firstTokens[1].substr(0).compare("\t")){
        string toParse = firstTokens[1].substr(1,firstTokens[1].size()-1);
        depTokens = tokenize(toParse," ");
    }
    else {
        depTokens = tokenize(firstTokens[1]," ");
    }
    int i=0;
    int nDep = depTokens.size();
    while(i<nDep){
        #ifdef VERBOSE
        cout << "Dependency: " << depTokens[i] << "\n";
        #endif
        if (depTokens[i] == ""){
            i++;
            continue;
        }
        toAdd->dependencies.push_back(depTokens[i]);
        i++;
    }
    }
    vector<string> command = tokenize(secondLine,"\t");
    cout << "Command: " << command[0] << "\n" ;
    toAdd->command=command[0];
    return toAdd;
}

long  tache::getNewId(){
    return tache::tID++;
}

bool tache::operator ==(tache other)
    {
      if(this->name == other.name)
      {
        return true;
      }
      else return false;
    }

bool tache::operator < (const tache other)
    {
      if(this->name < other.name)
      {
        return true;
      }
      else return false;
    }

tache& tache::operator= (const tache& cSource)
{
    // check for self-assignment by comparing the address of the
    // implicit object and the parameter
    if (this == &cSource)
        return *this;
 
    // do the copy
 
    // return the existing object
    return *this;
}

vector<string> tache::tokenize(const string & str, const string & delim)
{
  vector<string> tokens;
  size_t p0 = 0, p1 = string::npos;

  while(p0 != string::npos)
  {
    p1 = str.find_first_of(delim, p0);
    if(p1 != p0)
    {
      string token = str.substr(p0, p1 - p0);
      tokens.push_back(token);
    }
    p0 = str.find_first_not_of(delim, p1);
 }
return tokens;
}

// the job could be executed


bool tache::testTacheDeps()
{
#ifdef VERBOSE
    cout << "Dimension of the deps " << this->dependencies.size() << "\n";
    cout << "Dimension of the deps " << this->command << "\n";
#endif
    if (this->dependencies.size() == 0){
        return true;
    }
      for (std::vector<string>::iterator it = this->dependencies.begin() ; it != this->dependencies.end(); ++it){
          string str = *it;      
          if (!this->testSingleDep(str)){
                    return false;
          }
      }
    return true;
};

bool tache::testSingleDep(string filename)
{
  ifstream ifile(filename.c_str());
  return ifile;
}

bool tache::run(){
    cout << this->command << "\n";
    system(this->command.c_str());
    return true;
}