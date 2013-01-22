/* 
 * File:   Job.h
 * Author: alex
 *
 * Created on 20 gennaio 2013, 14.18
 */

#ifndef JOB_H
#define	JOB_H
#include <string>
#include <iostream>
#include <vector>
#include "../tache.h"

class Job : public tache{
public:
    Job();
    Job(const Job& orig);
    virtual ~Job();
    void addTache(std::string line);
private:
    std::vector<std::string> tokenize(const std::string & str, const std::string & delim);
};

#endif	/* JOB_H */

