/* 
 * File:   tache.cpp
 * Author: alex
 * 
 * Created on 21 gennaio 2013, 12.35
 */

//#include <ompi/mpi/cxx/constants.h>




#include "tache.h"
#include "Job/Job.h"

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




bool tache::sendTache(long target_host,bool results){
    // Send preliminary informations
    // send id
    string buff;
//    cout << "Sending tache:" << this->getId() << "\n";
    if (results == true){
        //cout << "Sending results:" << this->getId() << " to source!\n";
        string result = convertFile(this->name);
        cout << "This is the result to send:" << result << "\n";
        sendData(result,target_host,RESULT);
        cout << "Message Sent!\n";
        return true;
    }
    else {
        int tag = ID_SEND;
        buff = tache::toString(this->id);
        cout << "SENDER:Sending id tache:" << this->getId() << " to " << target_host <<"!\n";
        sendData(buff,target_host,tag);
        sendData(this->name,target_host,NAME_SEND);
        //MPI::COMM_WORLD.Send((void*)buff.c_str(),buff.size(),MPI::CHAR,target_host,tag);
        // send command
        tag = COMMAND_SEND;
        cout << "SENDER:Sending command tache:" << this->getId() << " to " << target_host <<"!\n";
        //MPI::COMM_WORLD.Send((void *)this->command.c_str(),this->command.size(),MPI::CHAR,target_host,tag);
        sendData(this->command,target_host,tag);
        vector<string>::iterator it;
        for(it = this->dependencies.begin(); it != this->dependencies.end(); it++) {
            tag = NDEP_SEND;
            buff = (*it); 
            
//            if (Job::CheckPresenceOnHost(target_host,buff) == true){
//                cout << "SENDER: The file is already present!!!!!!!!!!!\n";
//                continue;
//            }
//            
            sendData(buff,target_host,tag);
            cout << "Sending dependence name " << buff <<" tache:" << this->getId() << " to " << target_host <<"!\n";
            buff = convertFile(*it);
            cout << "SENDER File: " << buff < "\n";
            tag = DEP_SEND;
            sendData(buff,target_host,tag);
         }
        sendData("END",target_host,END);
    }
    
}

bool tache::receiveTache(long target_host,bool results){
    // Send preliminary informations
    // send id
    
    int tag;
    MPI::Status status;
    int dimension=0;
    int source = 0;
    //string buff = tache::toString(this->id);
    do {
        //cout << "Probing...\n";
        MPI::COMM_WORLD.Probe(source,MPI::ANY_TAG,status);
        dimension = status.Get_count(MPI::CHAR);
        char b[dimension];
        memset(b,0,sizeof(b));
        //cout << "Receiving...\n";
        MPI::COMM_WORLD.Recv((void*)b,dimension,MPI::CHAR,MPI::ANY_SOURCE,MPI::ANY_TAG);
        tag = status.Get_tag();
        string buff(b);
        switch (tag){
            case ID_SEND:
                    this->id=atoi(b);
                    cout << "ID SAVED: " << this->id << "\n";
                    break;
            case NAME_SEND:
                    this->name=b;
                    cout << "NAME SAVED: " << this->name << "\n";
                    break;
            case COMMAND_SEND:
                    //cout << "Copying command...\n";
                    this->command = buff;
                    cout << "Copied command: " << this->command <<"\n";
                    break;
            case RESULT:
                MPI::COMM_WORLD.Recv((void*)b,dimension,MPI::CHAR,MPI::ANY_SOURCE,MPI::ANY_TAG);
                savefile(b,this->name);
                break;
            case DIE:
                return false;
            case NDEP_SEND:
                this->dependencies.push_back(buff);
                string namefile = string((char*)b);
                MPI::COMM_WORLD.Probe(MPI::ANY_SOURCE,MPI::ANY_TAG,status);
                dimension = status.Get_count(MPI::CHARACTER);
                char *file =(char*)malloc(sizeof(char)*dimension+1);
                int dim=dimension;
                MPI::COMM_WORLD.Recv(file,dim,MPI::CHAR,MPI::ANY_SOURCE,MPI::ANY_TAG);
                file[dimension]='\0';
                savefile(file,namefile);
                break;
        }
    } while (status.Get_tag() != END);
    //
    return true;
}



//
//bool tache::receiveTache(long target_host,bool results){
//    
//}



string tache::convertFile(string name){
        //cout << "Insitde convert:" << name << "\n";
        if (!testSingleDep(name)){
            return NULL;
        }
        std::ifstream in_file(name.c_str());
        cout << in_file << "\n";
        std::string the_str(std::istreambuf_iterator<char>( in_file),(std::istreambuf_iterator<char>()));
        //cout << "Insitde convert:" << the_str << "\n";
        return the_str;
}

string tache::toString(long i){
    string buff;
    ostringstream convert ;   // stream used for the conversion
    convert << i;      // insert the textual representation of 'Number' in the characters in the stream
    buff = convert.str();
    return buff;
}

long tache::getId(){
    return this->id;
}

void tache::savefile(char* content, string name){
    //cout << "Saving " << content <<" !\n";
    ofstream toSave;
    toSave.open(name.c_str());
    toSave << content;
    toSave.close();
    return;
}

void tache::sendData(string data, int dest, int tag)
{
    MPI::Request req;
    MPI::Status status;
    int error;
    size_t data_size = data.size() + 1;
    char* data_copy = (char*)malloc(sizeof(char)*(data_size));
    strncpy(data_copy, data.c_str(), data_size);
    do {
    //cout << "PRE-Sending data: " << data_copy << "\n size " << data_size << "dest" << dest << "tag " << tag << "\n";
    req = MPI::COMM_WORLD.Isend(data_copy, data_size, MPI::CHAR, dest,tag);
    
    req.Wait(status);
    if ((error = status.Get_error())){
        cout << "SENT IMPOSSIBLE\n";
        }
    } while (error != 0);
    //cout << "POST-Sending data: " << data_copy << "\n size " << data_size << "dest" << dest << "tag " << tag << "\n";
};

