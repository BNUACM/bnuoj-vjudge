/* 
 * File:   Bott.h
 * Author: 51isoft
 *
 * Created on 2014年1月19日, 上午1:04
 */

#ifndef BOTT_H
#define BOTT_H

#include "vjudge.h"

class Bott {
public:
  /** Default constructor */
  Bott();
  Bott(string filename);
  /** Default destructor */
  virtual ~Bott();

  int Gettype() {
    return type;
  }

  void Settype(int val) {
    type = val;
  }

  string Getrunid() {
    return runid;
  }

  void Setrunid(string val) {
    runid = val;
  }

  string Getcha_id() {
    return cha_id;
  }

  void Setcha_id(string val) {
    cha_id = val;
  }

  string Getsrc() {
    return src;
  }

  void Setsrc(string val) {
    src = val;
  }

  string Getlanguage() {
    return language;
  }

  void Setlanguage(string val) {
    language = val;
  }

  string Getpid() {
    return pid;
  }

  void Setpid(string val) {
    pid = val;
  }

  string Getnumber_of_testcases() {
    return number_of_testcases;
  }

  void Setnumber_of_testcases(string val) {
    number_of_testcases = val;
  }

  string Gettime_limit() {
    return time_limit;
  }

  void Settime_limit(string val) {
    time_limit = val;
  }

  string Getcase_limit() {
    return case_limit;
  }

  void Setcase_limit(string val) {
    case_limit = val;
  }

  string Getmemory_limit() {
    return memory_limit;
  }

  void Setmemory_limit(string val) {
    memory_limit = val;
  }

  string Getspj() {
    return spj;
  }

  void Setspj(string val) {
    spj = val;
  }

  string Getvname() {
    return vname;
  }

  void Setvname(string val) {
    vname = val;
  }

  string Getvid() {
    return vid;
  }

  void Setvid(string val) {
    vid = val;
  }

  string Getmemory_used() {
    return memory_used;
  }

  void Setmemory_used(string val) {
    memory_used = val;
  }

  string Gettime_used() {
    return time_used;
  }

  void Settime_used(string val) {
    time_used = val;
  }

  string Getresult() {
    return result;
  }

  void Setresult(string val) {
    result = val;
  }

  string Getce_info() {
    return ce_info;
  }

  void Setce_info(string val) {
    ce_info = val;
  }

  string Getdata_type() {
    return data_type;
  }

  void Setdata_type(string val) {
    data_type = val;
  }

  string Getdata_detail() {
    return data_detail;
  }

  void Setdata_detail(string val) {
    data_detail = val;
  }

  string Getdata_lang() {
    return data_lang;
  }

  void Setdata_lang(string val) {
    data_lang = val;
  }

  string Getcha_result() {
    return cha_result;
  }

  void Setcha_result(string val) {
    cha_result = val;
  }

  string Getcha_detail() {
    return cha_detail;
  }

  void Setcha_detail(string val) {
    cha_detail = val;
  }

  string Getout_filename() {
    return out_filename;
  }

  void Setout_filename(string val) {
    out_filename = val;
  }

  string Getremote_runid() const {
    return remote_runid;
  }

  void Setremote_runid(string remote_runid) {
    this->remote_runid = remote_runid;
  }

  void toFile();

  void save() {
    toFile();
  }

  static const string RAW_FILES_DIRECTORY;
  static const string CHA_RAW_FILES_DIRECTORY;
  static const string RESULTS_DIRECTORY;
  static const string CHA_RESULTS_DIRECTORY;
  static const string EXTENTION;

  static const string SOURCE_CODE_BEGIN;
  static const string SOURCE_CODE_END;
  static const string COMPILE_INFO_BEGIN;
  static const string COMPILE_INFO_END;
  static const string DATA_DETAIL_BEGIN;
  static const string DATA_DETAIL_END;
  static const string CHALLENGE_DETAIL_BEGIN;
  static const string CHALLENGE_DETAIL_END;
protected:
private:
  int type;
  string runid;
  string cha_id;
  string src;
  string language;
  string pid;
  string number_of_testcases;
  string time_limit;
  string case_limit;
  string memory_limit;
  string spj;
  string vname;
  string vid;
  string memory_used;
  string time_used;
  string result;
  string ce_info;
  string data_type;
  string data_lang;
  string data_detail;
  string cha_result;
  string cha_detail;
  string remote_runid;

  map <string, string> infos;
  fstream fin;

  string parseUntil(string end);
  string out_filename;

};

#endif // BOTT_H

