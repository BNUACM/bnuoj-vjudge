/* 
 * File:   CFGymJudger.cpp
 */

#include "CFGymJudger.h"

/**
 * Create a CF Gym Judger
 * @param _info Should be a pointer of a JudgerInfo
 */
CFGymJudger::CFGymJudger(JudgerInfo * _info) : CFJudger(_info) {
}

CFGymJudger::~CFGymJudger() {
}

void CFGymJudger::initHandShake(){
    socket->sendMessage(CONFIG->GetJudge_connect_string() + "\nCodeForcesGym");
}

/**
 * Submit a run
 * @param bott      Bott file for Run info
 * @return Submit status
 */
int CFGymJudger::submit(Bott * bott) {
    // prepare cid and pid from vid
    string contest, problem;
    if (!RE2::PartialMatch(bott->Getvid(), "([0-9]*)(.*)", &contest, &problem)) {
        throw Exception("Invalid vid.");
    }

    string submit_url = "http://codeforces.com/gym/"+contest+"/submit";
    string csrf = getCsrfParams(submit_url);

    string source = bott->Getsrc();
    // add random extra spaces in the end to avoid same code error
    srand(time(NULL));
    source += '\n';
    while (rand()%120) source+=' ';
    
    // prepare form for post
    struct curl_httppost * formpost = NULL;
    struct curl_httppost * lastptr  = NULL;
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "action",
                 CURLFORM_COPYCONTENTS, "submitSolutionFormSubmitted",
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "submittedProblemIndex",
                 CURLFORM_COPYCONTENTS, problem.c_str(),
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "programTypeId",
                 CURLFORM_COPYCONTENTS, bott->Getlanguage().c_str(),
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "source",
                 CURLFORM_COPYCONTENTS, source.c_str(),
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "sourceCodeConfirmed",
                 CURLFORM_COPYCONTENTS, "true",
                 CURLFORM_END);
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "_tta",
                 CURLFORM_COPYCONTENTS, getttaValue().c_str(),
                 CURLFORM_END);
    
    prepareCurl();
    curl_easy_setopt(curl, CURLOPT_URL, (submit_url + "?csrf_token=" + csrf).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    performCurl();
    curl_formfree(formpost);
    
    // check submit status
    string html = loadAllFromFile(tmpfilename);
    if (html.find("You have submitted exactly the same code before") != string::npos) {
        return VirtualJudger::SUBMIT_SAME_CODE;
    } else if (html.find("Choose valid language") != string::npos) {
        return VirtualJudger::SUBMIT_INVALID_LANGUAGE;
    } else if (html.find("<span class=\"error for__source\">") != string::npos) {
        return VirtualJudger::SUBMIT_COMPILE_ERROR;
    } else if (html.find("<a href=\"/enter\">Enter</a>") != string::npos) {
        return SUBMIT_OTHER_ERROR;
    }
    return VirtualJudger::SUBMIT_NORMAL;
}

/**
 * Get run details from Codeforces
 * @param contest       Contest ID
 * @param runid         Remote runid
 * @return Verdict details
 */
string CFGymJudger::getVerdict(string contest, string runid) {
    prepareCurl();
    curl_easy_setopt(curl, CURLOPT_URL, ((string)"http://codeforces.com/gym/" + contest + "/submission/" + runid).c_str());
    performCurl();
    
    string html = loadAllFromFile(tmpfilename);
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(html);
    hcxselect::Selector selector(dom);
    
    // load all roundbox in verdict page
    try {
        selector = selector.select("#content .roundbox");
    } catch (...) {
        log("Parse verdict error, use empty result instead.");
        return "";
    }
    
    // find the one contains error message
    for (hcxselect::Selector::const_iterator it = selector.begin(); it != selector.end(); ++it) {
        string content = html.substr((*it)->data.offset(), (*it)->data.length());
        if (content.find("<div  class=\"error\">") != string::npos) return content;
    }
    
    return "";
}
