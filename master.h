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

class master {
public:
    master();
    master(const master& orig);
    virtual ~master();
    void mainMaster(int argc,char **argv, long id, long p);
private:

};

#endif	/* MASTER_H */

