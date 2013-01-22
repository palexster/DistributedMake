/* 
 * File:   tache.h
 * Author: alex
 *
 * Created on 21 gennaio 2013, 12.35
 */

#ifndef TACHE_H
#define	TACHE_H
#include <vector>

class tache {
public:
    tache();
    tache(const tache& orig);
    virtual ~tache();
private:
    std::vector<tache> subTaches; // n° subTaches
    int nTaches; // n° subtaches
    
};

#endif	/* TACHE_H */

