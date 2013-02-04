/* 
 * File:   master.h
 * Author: alex
 *
 * Created on 21 gennaio 2013, 12.35
 */

#ifndef MASTER_H
#define	MASTER_H
# include "Job/Job.h"
# include <string>
# include <string.h>
# include <mpi/mpi.h>
#include <sys/types.h>
#include <pthread.h>
#include <vector>

class master {
public:
    static int *nodes;
    master();
    master(const master& orig);
    virtual ~master();
    static void SetBusy(int id);
    static void SetFree(int id);
    static int GetFree();
    void mainMaster(int argc,char **argv, long id, long p);
    static int dimension;
    static int id;
//    void* tryTestJobs(void *t);
private:
    
};

#endif	/* MASTER_H */

