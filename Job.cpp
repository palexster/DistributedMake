/* 
 * File:   Job.cpp
 * Author: alex
 * 
 * Created on 20 gennaio 2013, 14.18
 */

#include "Job/Job.h"
#include "tache.h"

using namespace std;

Job::Job() {
    
}

Job::Job(const Job& orig) {
}

Job::~Job() {
}

void Job::addTache(string line){
    vector<string> tokens = tokenize(line," ");
    
}       

vector<string> Job::tokenize(const string & str, const string & delim)
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

