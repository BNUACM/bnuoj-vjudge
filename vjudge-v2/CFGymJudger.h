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
private:
    void initHandShake();
    int submit(Bott *);
    string getVerdictUrl(string, string);
};

#endif	/* CFGYMJUDGER_H */

