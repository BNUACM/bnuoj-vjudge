/* 
 * File:   WHUJudger.cpp
 * Author: payper
 * 
 * Created on 2014年3月25日, 下午4:01
 */

#include "WHUJudger.h"

/**
 * Create a WHU Judger
 * @param _info Should be a pointer of a JudgerInfo
 */
WHUJudger::WHUJudger(JudgerInfo * _info) : VirtualJudger(_info) {
    socket->sendMessage(CONFIG->GetJudge_connect_string() + "\nWHU");

    language_table["1"] = "2";
    language_table["2"] = "1";
    language_table["3"] = "3";
    language_table["4"] = "4";
}

WHUJudger::~WHUJudger() {
}

/**
 * Login to WHU
 */
void WHUJudger::login() {

}

/**
 * Submit a run
 * @param bott      Bott file for Run info
 * @return Submit status
 */
int WHUJudger::submit(Bott * bott) {

}

/**
 * Get result and related info
 * @param bott  Original Bott info
 * @return Result Bott file
 */
Bott * WHUJudger::getStatus(Bott * bott) {
}

/**
 * Get compile error info from WHU
 * @param bott      Result bott file
 * @return Compile error info
 */
string WHUJudger::getCEinfo(Bott * bott) {

}

/**
 * Check whether the result is final
 * @param result        Current result
 * @return Is final one or not
 */
string WHUJudger::convertResult(string result) {

}