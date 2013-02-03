/* 
 * File:   tache.h
 * Author: alex
 *
 * Created on 21 gennaio 2013, 12.35
 */

#ifndef TACHE_H
#define	TACHE_H
# include <vector>
# include "mpi.h"

# include <string>
# include <map>
# include <utility>
# include <deque>
# include <vector>
# include <vector>
# include <cstdlib>
# include <mpi/mpi.h>
# include <cmath>
# include <ios>
# include <fstream>
# include <ctime>
# include <iostream>
# include <istream>
# include <deque>
# include <ostream>
# include <fstream>
# include <iostream>
# include <iterator>
# include <sstream>

#define ID_SEND 0
#define COMMAND_SEND 1
#define NDEP_SEND 2
#define DEP_SEND 3
#define END 4
#define RESULT 5
#define DIE 6
#define NAME_SEND 7
class tache {
public:
    bool init;
    bool done;
    tache(const tache& orig);
    virtual ~tache();
    long getId();
    void setId();
    std::string name;
    static tache* createNewTache(std::string firstLine,std::string secondLine);
    static long getNewId();
    bool testTacheDeps();
    bool run();
    static bool testSingleDep(std::string filename);
    bool operator < (const tache other);
    bool operator ==(const tache other);
    tache& operator =(const tache& cSource);
    bool receiveTache(long target_host, bool results);
    bool completed;
    void savefile(char* content,std::string name);
    std::string command;
    void sendData(std::string data, int dest, int tag);
    bool sendTache(long toRun, bool results);
    static std::string toString(long i);
    static std::string convertFile(std::string name);
    tache();
protected:
    static std::vector<std::string> tokenize(const std::string & str, const std::string & delim);

    std::vector<tache> subTaches; // n° subTaches
    
    int nTaches; // n° subtaches
    std::vector<std::string> dependencies;
    long id;
    static long tID;

};

//typedef std::pair<std::string,tache> TachePair;

#endif	/* TACHE_H */

