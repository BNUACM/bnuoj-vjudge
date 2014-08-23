/* 
 * File:   judgertest.cpp
 * Author: 51isoft
 *
 * Created on 2014-2-3, 0:31:14
 */

#include "JudgerFactory.h"

/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "judgertest test 1" << std::endl;
}

void test2() {
    std::cout << "judgertest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (judgertest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
//    std::cout << "%SUITE_STARTING% judgertest" << std::endl;
//    std::cout << "%SUITE_STARTED%" << std::endl;
//
//    std::cout << "%TEST_STARTED% test1 (judgertest)" << std::endl;
//    test1();
//    std::cout << "%TEST_FINISHED% time=0 test1 (judgertest)" << std::endl;
//
//    std::cout << "%TEST_STARTED% test2 (judgertest)\n" << std::endl;
//    test2();
//    std::cout << "%TEST_FINISHED% time=0 test2 (judgertest)" << std::endl;
//
//    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    curl_global_init(CURL_GLOBAL_ALL);
    try {
        JudgerInfo * info = new JudgerInfo("HRBUST", "HRBUST", "bnuvjudge", "asdf1234#", 30);
        VirtualJudger * judger = JudgerFactory::createJudger(info);
        Bott * bott = new Bott;
        bott->Setout_filename("tests/test.bott");
//        bott->Setsrc("int main() {  return 0; }");
        bott->Setsrc("int main() { safssafasfasfas\nfasfasfasfa\nsfasfasfasfasfasfassafsaffregegrthrthrthrt\nhrthrtrhtrthdaas int main(){ return 0; }");
        bott->Setlanguage("1");
        bott->Setvid("1000");
        bott->Setrunid("1");
        judger->judge(bott, "tests/result.bott");

        delete judger;
        delete info;
    } catch (Exception & e) {
        cout << e.what() << endl;
    } catch (exception & e) {
        cout << e.what() << endl;
    }
    return (EXIT_SUCCESS);
}

