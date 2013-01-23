/* 
 * File:   tache.h
 * Author: alex
 *
 * Created on 21 gennaio 2013, 12.35
 */

#ifndef TACHE_H
#define	TACHE_H
# include <vector>
# include <string>
# include <map>
# include <utility>
# include <deque>
# include <vector>

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
    bool operator < (const tache other);
    bool operator ==(const tache other);
    tache& operator =(const tache& cSource);
protected:
    static std::vector<std::string> tokenize(const std::string & str, const std::string & delim);
    tache();
    std::vector<tache> subTaches; // n° subTaches
    std::string command;
    int nTaches; // n° subtaches
    std::vector<std::string> dependencies;
    long id;
    static long tID;
};

//typedef std::pair<std::string,tache> TachePair;

#endif	/* TACHE_H */

