/* 
 * File:   slave.h
 * Author: alex
 *
 * Created on 21 gennaio 2013, 12.35
 */

#ifndef SLAVE_H
#define	SLAVE_H

class slave {
public:
    slave();
    slave(const slave& orig);
    void mainSlave(int argc,char*argv[], long id, long p);
    virtual ~slave();
private:

};

#endif	/* SLAVE_H */

