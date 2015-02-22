/* 
 * File:   UralJudger.cpp
 * Author: payper
 * 
 * Created on 2014年3月25日, 下午1:22
 */

#include "UralJudger.h"

/**
 * Create a Ural Judger
 * @param _info Should be a pointer of a JudgerInfo
 */
UralJudger::UralJudger(JudgerInfo * _info) : VirtualJudger(_info) {
    socket->sendMessage(CONFIG->GetJudge_connect_string() + "\nUral");

    language_table["1"] = "26";
    language_table["2"] = "25";
    language_table["3"] = "32";
    language_table["4"] = "31";
    language_table["6"] = "11";
    
    if (!RE2::PartialMatch(info->GetUsername(), "([0-9]*)", &author_id)) {
        throw Exception("Cannot retrieve author id from username");
    }
}

UralJudger::~UralJudger() {
}

/**
 * Ural doesn't require login, it uses judgeID to identify the submitter
 */
void UralJudger::login() {
}

/**
 * Submit a run
 * @param bott      Bott file for Run info
 * @return Submit status
 */
int UralJudger::submit(Bott * bott) {
    
    // prepare form for post
    struct curl_httppost * formpost = NULL;
    struct curl_httppost * lastptr  = NULL;
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "Action",
                 CURLFORM_COPYCONTENTS, "submit",
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "SpaceID",
                 CURLFORM_COPYCONTENTS, "1",
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "JudgeID",
                 CURLFORM_COPYCONTENTS, info->GetUsername().c_str(),
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "Language",
                 CURLFORM_COPYCONTENTS, bott->Getlanguage().c_str(),
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "ProblemNum",
                 CURLFORM_COPYCONTENTS, bott->Getvid().c_str(),
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "Source",
                 CURLFORM_COPYCONTENTS, bott->Getsrc().c_str(),
                 CURLFORM_END);

    
    prepareCurl();
    curl_easy_setopt(curl, CURLOPT_URL, "http://acm.timus.ru/submit.aspx?space=1");
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    // have to fetch header for runid
    string headerfilename = tmpfilename + "_header";
    FILE * headerfile = fopen(headerfilename.c_str(), "w");
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, headerfile);

    performCurl();
    curl_formfree(formpost);
    fclose(headerfile);
    
    // check submit status
    string html = loadAllFromFile(headerfilename);
    if (html.find("303 See Other") == string::npos) {
        return SUBMIT_OTHER_ERROR;
    }

    return VirtualJudger::SUBMIT_NORMAL;
}

/**
 * Get result and related info
 * @param bott  Original Bott info
 * @return Result Bott file
 */
Bott * UralJudger::getStatus(Bott * bott) {
    time_t begin_time = time(NULL);
    int count=10;
    
    Bott * result_bott = new Bott;

    // fetch runid
    string runid, header = loadAllFromFile(tmpfilename + "_header");
    if (!RE2::PartialMatch(header, "(?s)X-SubmitID: ([0-9]*)", &runid)) {
        throw Exception("Failed to get remote runid.");
    }
    result_bott->Setremote_runid(trim(runid));

    while (true) {
        // check wait time
        if (time(NULL) - begin_time > info->GetMax_wait_time()) {
            throw Exception("Failed to get current result, judge time out.");
        }
        
        prepareCurl();
        // count can be set to 100 if Ural is very busy
        curl_easy_setopt(curl, CURLOPT_URL, ("http://acm.timus.ru/status.aspx?space=1&count="+intToString(count)).c_str());
        performCurl();
        
        string html = loadAllFromFile(tmpfilename);
        string status;
        string result, time_used, memory_used;
        
        // get the row for runid, if not found, try fetch more runs
        if (!RE2::PartialMatch(html, "(?s)(<TR.*?\"id\">"+runid+".*?</TR>)", &status)) {
            log("Trying to fetch more runs...");
            count += 10;
            continue;
        }
        
        // get result
        if (!RE2::PartialMatch(status, "(?s)<TD class=\"verdict.*?>(.*?)</TD>", &result)) {
            throw Exception("Failed to get current result.");
        }
        result = trim(result);
        if (isFinalResult(result)) {
            // result is the final one
            result = convertResult(result);
            if (result != "Compile Error") {
                // no details for CE results
                if (!RE2::PartialMatch(status,
                        "<TD class=\"runtime\">(.*?)</TD><TD class=\"memory\">(.*?) KB</TD>",
                        &time_used, &memory_used)) {
                    throw Exception("Failed to parse details from status row.");
                }
                int time_ms = stringToDouble(time_used) * 1000 + 0.001;
                time_used = intToString(time_ms);
            } else {
                memory_used = time_used = "0";
            }
            result_bott->Settype(RESULT_REPORT);
            result_bott->Setresult(result);
            result_bott->Settime_used(trim(time_used));
            result_bott->Setmemory_used(trim(memory_used));
            break;
        }
    }
    return result_bott;
}

/**
 * Get compile error info from Ural
 * @param bott      Result bott file
 * @return Compile error info
 */
string UralJudger::getCEinfo(Bott * bott) {
    prepareCurl();
    curl_easy_setopt(curl, CURLOPT_URL, ("http://acm.timus.ru/ce.aspx?id=" + bott->Getremote_runid()).c_str());
    performCurl();
    
    string info = loadAllFromFile(tmpfilename);
    return info;
}

/**
 * Check whether the result is final
 * @param result        Current result
 * @return Is final one or not
 */
string UralJudger::convertResult(string result) {
    if (result.find("Compilation error") != string::npos) return "Compile Error";
    if (result.find("Time limit exceeded") != string::npos) return "Time Limit Exceed";
    if (result.find("Memory limit exceeded") != string::npos) return "Memory Limit Exceed";
    if (result.find("Output limit exceeded") != string::npos) return "Output Limit Exceed";
    if (result.find("Wrong answer") != string::npos) return "Wrong Answer";
    if (result.find("Crash") != string::npos) return "Runtime Error";
    if (result.find("Runtime error") != string::npos) return "Runtime Error";
    return trim(result);
}
