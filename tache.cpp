/* 
 * File:   tache.cpp
 * Author: alex
 * 
 * Created on 21 gennaio 2013, 12.35
 */

//#include <ompi/mpi/cxx/constants.h>

#include "tache.h"
#include "deque"

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