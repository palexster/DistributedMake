/* 
 * File:   Job.cpp
 * Author: alex
 * 
 * Created on 20 gennaio 2013, 14.18
 */

# include "Job/Job.h"

using namespace std;



Job::Job() {
    
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
            vector<string> tokens = tokenize(line," ");
            this->id=Job::getNewId();
            cout << tokens[0];
            name = (tokens[0]);
            this->name = name.substr(name.length()-1);
            tokens.erase(tokens.begin());
            dimension = tokens.size();
            while (i != dimension){
                string dep = tokens[i];
                this->dependencies.push_back(dep);
                i++;
            }
            delete(&name);
            delete(&tokens);
    }
    // HERE SECOND LINE
        if (myfile.good()){
            getline(myfile,this->finalizeCommand);
    }
    //TachePair pair(this->name,this);
    string toAdd = this->name;
    tMap.insert(Mappair(this->name,this));
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
            
        }
    }
    
    
    
    while (myfile.good()) {  // If a line was successfully read
        {
            getline(myfile,line);
            if(line.length() == 0)  // Skip any blank lines
                continue;
            else if(line[0] == '#')  // Skip any comment lines
                continue; // Got a valid data line so return with this line
            else {
                
            }
            //this->addTache(line);
            
            cout << line << endl;
        }
    }
       
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

