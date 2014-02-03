/* 
 * File:   JudgerFactory.h
 * Author: 51isoft
 *
 * Created on 2014年2月1日, 上午10:04
 */

#ifndef JUDGERFACTORY_H
#define	JUDGERFACTORY_H

#include "JudgerInfo.h"

#include "VirtualJudger.h"
#include "CFJudger.h"

class JudgerFactory {
public:
    JudgerFactory();
    virtual ~JudgerFactory();
    static VirtualJudger * createJudger(JudgerInfo *);
private:

};

#endif	/* JUDGERFACTORY_H */

