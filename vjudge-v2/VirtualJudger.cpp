/* 
 * File:   VirtualJudger.cpp
 * Author: 51isoft
 * 
 * Created on 2014年1月31日, 下午9:08
 */

#include "VirtualJudger.h"

const int VirtualJudger::MIN_SOURCE_LENGTH = 15;
const int VirtualJudger::SLEEP_INTERVAL = 10;

const int VirtualJudger::SUBMIT_NORMAL           = 0;
const int VirtualJudger::SUBMIT_SAME_CODE        = 1;
const int VirtualJudger::SUBMIT_INVALID_LANGUAGE = 2;
const int VirtualJudger::SUBMIT_COMPILE_ERROR    = 3;
const int VirtualJudger::SUBMIT_OTHER_ERROR      = 4;


/**
 * Basic constructor for virtual judger
 * @param _info JudgerInfo pointer
 */
VirtualJudger::VirtualJudger(JudgerInfo * _info) {
    info = _info;
    socket = new SocketHandler;
    tmpfilename = CONFIG->GetTmpfile_path() + info->GetId();
    cookiefilename = CONFIG->GetCookies_path() + info->GetId();
    logged_in = false;
}

VirtualJudger::~VirtualJudger() {
    delete socket;
}

/**
 * Generate special results with some default values
 * @param bott          Bott file to store results
 * @param result        Result
 */
void VirtualJudger::generateSpecialResult(Bott * bott, string result) {
    bott->Settype(RESULT_REPORT);
    bott->Settime_used("0");
    bott->Setmemory_used("0");
    bott->Setresult(result);
    bott->Setce_info("");
    bott->Setremote_runid("0");
}

/**
 * Send a submit to remote, and store the judge result
 * @param bott      Submit info
 * @param filename  File store the result
 */
void VirtualJudger::judge(Bott * bott, string filename) {
    // convert language
    if (language_table.find(bott->Getlanguage()) == language_table.end()) {
        throw Exception("Unsupported language.");
    }
    bott->Setlanguage(language_table[bott->Getlanguage()]);
    
    // login
    if (!logged_in) { // Not logged in, try login
        clearCookies();
        login();
        log("Logged in.");
        logged_in = true;
    }
    
    // submit
    int submit_status = submit(bott);
    if (submit_status == SUBMIT_OTHER_ERROR) {
        // first time fail, blame login status...
        log((string)"Submit error. Assume not logged in.");
        logged_in = false;
        clearCookies();
        login();
        log("Logged in.");
        logged_in = true;
        submit_status = submit(bott);
        if (submit_status == SUBMIT_OTHER_ERROR) {
            // second time fail, blame frequency
            log("Submit error. Assume should sleep for a while, sleeping " + intToString(VirtualJudger::SLEEP_INTERVAL) + " seconds.");
            sleep(VirtualJudger::SLEEP_INTERVAL);
            submit_status = submit(bott);
        }
    } else if (submit_status == SUBMIT_INVALID_LANGUAGE && info->GetOj() == "SPOJ" && bott->Getlanguage() == "41") {
        // Special HACK for Invalid Language on SPOJ, since it has two C++ type
        // And each of them covers a certain subset of problems
        submit_status = submit(bott);
    }
    if (submit_status != SUBMIT_OTHER_ERROR) log("Submitted.");
    
    // check submit status
    sleep(1); // sleep 1 second, just in case remote status table is not refreshed
    Bott * result;
    if (submit_status == SUBMIT_NORMAL) {
        // get status
        result = getStatus(bott);
        result->Setrunid(bott->Getrunid());
        if (result->Getresult() == "Compile Error") {
            try {
                result->Setce_info(getCEinfo(result));
            } catch (...) {
                log("Failed to get CE info, use empty one instead.");
                result->Setce_info("");
            }
        }
    } else {
        result = new Bott;
        result->Setrunid(bott->Getrunid());
        switch (submit_status) {
            case SUBMIT_SAME_CODE:
                generateSpecialResult(result, "Judge Error (Same Code)");
                break;
            case SUBMIT_INVALID_LANGUAGE:
                generateSpecialResult(result, "Judge Error (Invalid Language)");
                break;
            case SUBMIT_COMPILE_ERROR:
                generateSpecialResult(result, "Compile Error");
                break;
            default:
                log("Submit Error.");
                generateSpecialResult(result, "Judge Error");
        }
    }
    result->Setout_filename(filename);
    result->save();
    log("Done judging. Result: " + result->Getresult() + ", remote runid: " + result->Getremote_runid());
    delete result;
}

/**
 * Start virtual judger
 */
void VirtualJudger::run() {
    log("Judger started");
    while (true) {
        socket->receiveFile(tmpfilename);
        Bott * bott = new Bott(tmpfilename);
        log((string) "Received a new task, problem: " + bott->Getpid() + ".");
        string result_filename = Bott::RESULTS_DIRECTORY + bott->Getrunid();
        if (bott->Gettype() == NEED_JUDGE) {
            // Currently for vjudge, only NEED_JUDGE is supported
            try {
                if (bott->Getsrc().length() < MIN_SOURCE_LENGTH) {
                    // source code too short, may cause problem in remote OJ, such as PKU
                    bott->Setout_filename(result_filename);
                    generateSpecialResult(bott, "Compile Error");
                    bott->Setce_info("Source code too short, minimum length: " + intToString(MIN_SOURCE_LENGTH));
                    bott->save();
                } else {
                    judge(bott, result_filename);
                }
            } catch (Exception & e) {
                // Exception occurs, set to Judge Error
                log((string)"Judge error! Reason: " + e.what());
                
                // reuse bott file
                bott->Setout_filename(result_filename);
                generateSpecialResult(bott, "Judge Error");
                bott->save();
            } catch (exception & e) {
                // Exception occurs, set to Judge Error
                log((string)"Judge error! Reason: " + e.what());
                
                // reuse bott file
                bott->Setout_filename(result_filename);
                generateSpecialResult(bott, "Judge Error");
                bott->save();
            }
            delete bott;
        } else {
            delete bott;
            throw Exception("Type not supported.");
        }
        socket->sendFile(result_filename);
    }
}

/**
 * Log with judger identifier
 * @param message       Log message
 */
void VirtualJudger::log(string message) {
    LOG(message, info->GetId());
}

/**
 * Clear current cookies
 */
void VirtualJudger::clearCookies() {
    FILE * fp = fopen(cookiefilename.c_str(), "w");
    fclose(fp);
}

/**
 * Initialize curl pointer with common options
 */
void VirtualJudger::prepareCurl() {
    
    curl = curl_easy_init();
    if (!curl) {
        throw Exception("Curl init failed.");
    }
    
    // set basic curl options
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, CONFIG->GetMax_curl_time());
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONFIG->GetMax_curl_time());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl");
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiefilename.c_str());
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiefilename.c_str());
}

/**
 * Execute curl, store the result in tmpfile, then cleanup curl
 */
void VirtualJudger::performCurl() {
    curl_file = fopen(tmpfilename.c_str(), "w");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl_file);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    
    CURLcode curl_result = curl_easy_perform(curl);
    fclose(curl_file);
    if (curl_result != CURLE_OK) {
        throw Exception("Curl failed");
    }
    curl_easy_cleanup(curl);
}

/**
 * Check whether the result is final
 * @param result        Current result
 * @return Is final one or not
 */
bool VirtualJudger::isFinalResult(string result) {
    result = trim(result);
    
    // Minimum length result is "Accept"
    if (result.length() < 6) return false;
    if (result.find("Waiting") != string::npos) return false;
    if (result.find("Running") != string::npos) return false;
    if (result.find("Judging") != string::npos) return false;
    if (result.find("Queuing") != string::npos) return false;
    if (result.find("Compiling") != string::npos) return false;
    if (result.find("queue") != string::npos) return false;
    if (result.find("Pending") != string::npos) return false;
    
    return true;
}