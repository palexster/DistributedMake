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
    Job();
    Job(const Job& orig);
    virtual ~Job();
    void createNewJob(std::string name);
    static std::map<const std::string, tache*> tMap;
private:
    std::vector<std::string> tokenize(const std::string & str, const std::string & delim);
//    std::string init;
    std::string finalizeCommand;
    
};

typedef std::pair<std::string, tache*> Mappair;
#endif	/* JOB_H */

