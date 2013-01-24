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

class Job : public tache{
public:
    static std::map<const std::string, tache*> *tMap;
    Job();
    Job(const Job& orig);
    virtual ~Job();
    void createNewJob(std::string name);
    static void addTacheToMap(tache* tache);
private:
    
//    std::string init;
    std::string finalizeCommand;
    
};

typedef std::pair<std::string, tache*> Mappair;
#endif	/* JOB_H */

