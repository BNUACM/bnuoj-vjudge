/* 
 * File:   CFJudger.h
 * Author: 51isoft
 *
 * Created on 2014年2月1日, 上午12:02
 */

#ifndef CFJUDGER_H
#define	CFJUDGER_H

#include "vjudge.h"
#include "VirtualJudger.h"

class CFJudger : public VirtualJudger {
public:
    CFJudger(JudgerInfo *);
    virtual ~CFJudger();
protected:
    void login();
    int submit(Bott *);
    Bott * getStatus(Bott *);
    string getCEinfo(Bott *);
    string getCsrfParams(string);
    string getttaValue();
    string getActionUrl();
    string convertResult(string);
    int calculatetta(string);
    string VerdictUrl(string, string);
    string getVerdict(string, string);
private:
    void initHandShake();
};

#endif	/* CFJUDGER_H */

