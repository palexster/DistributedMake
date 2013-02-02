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

bool tache::sendTache(long target_host){
    // Send preliminary informations
    // send id
    string buff;
    int tag = ID_SEND;
    buff = tache::toString(this->id);
    MPI::COMM_WORLD.Send((void*)buff.c_str(),buff.size(),MPI::CHAR,target_host,tag);
    // send command
    tag = COMMAND_SEND;
    MPI::COMM_WORLD.Send((void *)this->command.c_str(),this->command.size(),MPI::CHAR,target_host,tag);
    vector<string>::iterator it;
    for(it = this->dependencies.begin(); it != this->dependencies.end(); it++) {
        tag = NDEP_SEND;
        buff = (*it); 
        MPI::COMM_WORLD.Send((void*)buff.c_str(),buff.size(),MPI::CHAR,target_host,tag);
        buff = convertFile(*it);
        tag = DEP_SEND;
        MPI::COMM_WORLD.Send((void*)buff.c_str(),buff.size(),MPI::CHAR,target_host,tag);
    }

    
}

bool tache::receiveTache(long target_host){
    // Send preliminary informations
    // send id
    char *b;
    int tag;
    MPI::Status status;
    int dimension=0;
    int source = 0;
    //string buff = tache::toString(this->id);
    do {
        MPI::COMM_WORLD.Probe(source,MPI::ANY_TAG,status);
        dimension = status.Get_count(MPI::CHAR);
        b[dimension];
        MPI::COMM_WORLD.Recv((void*)b,dimension,MPI::CHAR,MPI::ANY_SOURCE,MPI::ANY_TAG);
        tag = status.Get_tag();
        string buff(b);
        switch (tag){
            case ID_SEND:
                    this->id=atoi(b);
                    break;
            case COMMAND_SEND:        
                    this->command = buff;
                    break;
            case NDEP_SEND:
//                const string dep = buff
                this->dependencies.push_back(buff);
                string namefile(b);
                MPI::COMM_WORLD.Probe(source,MPI::ANY_TAG,status);
                dimension = status.Get_count(MPI::CHAR);
                b[dimension];
                MPI::COMM_WORLD.Recv((void*)b,dimension,MPI::CHAR,MPI::ANY_SOURCE,MPI::ANY_TAG);
                tag = status.Get_tag();
                savefile(b,namefile);
                break;
        }
    } while (status.Get_tag() != END);
    // send command
    tag = COMMAND_SEND;
    MPI::COMM_WORLD.Recv((void *)this->command.c_str(),this->command.size(),MPI::CHAR,target_host,tag);
    // send dep_n°
    tag = NDEP_SEND;
//    buff = tache::toString(this->dependencies.size()); 
    MPI::COMM_WORLD.Recv((void *)this->dependencies.size(),1,MPI::INT,target_host,tag);
    vector<string>::iterator it;
    tag = DEP_SEND;
    for(it = this->dependencies.begin(); it != this->dependencies.end(); it++) {
        string buff = convertFile(*it); 
        MPI::COMM_WORLD.Recv((void *)this->dependencies.size(),1,MPI::INT,target_host,tag);
    }
    // filename

    ;
    
}

string tache::convertFile(string name){
        if (!testSingleDep(name)){
            return NULL;
        }
        std::ifstream in_file(name.c_str());
        std::string the_str(std::istreambuf_iterator<char>( in_file),(std::istreambuf_iterator<char>()));
        return the_str;
}

string tache::toString(long i){
    string buff;
    ostringstream convert ;   // stream used for the conversion
    convert << i;      // insert the textual representation of 'Number' in the characters in the stream
    buff = convert.str();
}

long tache::getId(){
    return this->id;
}

void tache::savefile(char* content, string name){
    ofstream toSave;
    toSave.open(name.c_str());
    toSave << content;
    toSave.close();
    return;
}