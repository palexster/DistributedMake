/* 
 * File:   Job.h
 * Author: alex
 *
 * Created on 20 gennaio 2013, 14.18
 */

#ifndef JOB_H
#define	JOB_H
# include <string>
# include <iostream>
# include <vector>
# include <cstdlib>
# include <cmath>
# include <ios>
# include <fstream>
# include <ctime>
#include "../tache.h"
# include <deque>

class Job : public tache{
public:
    static std::map<const std::string, tache*> *tMap;
    static std::map<const int, std::vector<std::string> > *FileCache;
    Job();
    Job(const Job& orig);
    virtual ~Job();
    void createNewJob(std::string name);
    bool testJobDeps();
    static void addTacheToMap(tache* tache);
    static tache* getNewTache();
    static void scheduleNewTache(tache* toAdd);
    static tache* getWaitingTache();
    static void putInWaiting(tache* toAdd);
    void signalEnd(const long p,const long id);
    bool finalize();
    bool run(const long id, const long p);
    void checkJobs();
private:
    
//    std::string init;
    std::string finalizeCommand;
    static std::deque<tache*> *tAvailable;
    static std::deque<tache*> *tWaiting;
};

typedef std::pair<std::string, tache*> Mappair;
#endif	/* JOB_H */

