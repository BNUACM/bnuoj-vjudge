/* 
 * File:   AizuJudger.h
 * Author: 51isoft
 *
 * Created on 2014年8月13日, 下午2:53
 */

#ifndef AIZUJUDGER_H
#define	AIZUJUDGER_H

#include "vjudge.h"
#include "VirtualJudger.h"

class AizuJudger : public VirtualJudger {
public:
    AizuJudger(JudgerInfo *);
    virtual ~AizuJudger();
    void initHandShake();
private:
    void login();
    int submit(Bott *);
    Bott * getStatus(Bott *);
    string getCEinfo(Bott *);
    string convertResult(string);
};

#endif	/* AIZUJUDGER_H */

