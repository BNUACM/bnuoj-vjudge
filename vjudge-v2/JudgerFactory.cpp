/* 
 * File:   JudgerFactory.cpp
 * Author: 51isoft
 * 
 * Created on 2014年2月1日, 上午10:04
 */

#include "JudgerFactory.h"

JudgerFactory::JudgerFactory() {
}

JudgerFactory::~JudgerFactory() {
}

VirtualJudger * JudgerFactory::createJudger(JudgerInfo * judger_info) {
    string oj = judger_info->GetOj();
    if (oj == "CodeForces") {
        return new CFJudger(judger_info);
    } else if (oj == "PKU") {
        return new PKUJudger(judger_info);
    } else if (oj == "HDU") {
        return new HDUJudger(judger_info);
    } else {
        throw Exception("Unknown OJ type: " + oj);
    }
}