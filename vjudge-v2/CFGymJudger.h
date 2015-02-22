/* 
 * File:   CFGymJudger.h
 */

#ifndef CFGYMJUDGER_H
#define	CFGYMJUDGER_H

#include "CFJudger.h"

class CFGymJudger : public CFJudger {
public:
    CFGymJudger(JudgerInfo *);
    virtual ~CFGymJudger();
    void initHandShake();
protected:
    int submit(Bott *);
    string getVerdict(string, string);
};

#endif	/* CFGYMJUDGER_H */

