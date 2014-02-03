/* 
 * File:   vjudge.cpp
 * Author: 51isoft
 *
 * Created on 2014年1月31日, 下午3:41
 */

#include "vjudge.h"
#include "JudgerInfo.h"
#include "Bott.h"

#include "JudgerFactory.h"

/**
 * Start a virtual judger
 * @param arg   Should be a JudgerInfo, contains oj specific contents
 * @return NULL
 */
void * start_judger(void * arg) {
    JudgerInfo * judger_info = (JudgerInfo *) arg;
    VirtualJudger * judger = NULL;
    
    try {
        judger = JudgerFactory::createJudger(judger_info);
        judger->run();
    } catch (Exception & e) {
        LOG(e.what());
    }
    
    pthread_exit(NULL);
}

int main() {
    // boost io performance
    ios::sync_with_stdio(false);
    
    curl_global_init(CURL_GLOBAL_ALL);
    vector<JudgerInfo> judgers = CONFIG->GetJudger_info();
    for (vector<JudgerInfo>::iterator it = judgers.begin();
            it != judgers.end();
            ++it) {
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, start_judger, (void *)(& (*it)));
    }
    while (true) {
    }
    
    curl_global_cleanup();
    return 0;
}

