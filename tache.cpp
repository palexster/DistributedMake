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
    this->id=getNewId();
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
    tache toAdd;
    vector<string> firstTokens = tokenize(firstLine," ");
    toAdd.id=getNewId();
    toAdd.name=firstTokens[0];
    toAdd.name.substr(0,toAdd.name.length()-2);
    int i=1;
    int nDep = firstTokens.size();
    while(i<nDep){
        toAdd.dependencies.push_back(firstTokens[i]);
        i++;
    }
    toAdd.command=secondLine;
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