/* 
 * File:   Bott.cpp
 * Author: 51isoft
 * 
 * Created on 2014年1月19日, 上午1:04
 */

#include "Bott.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

const string Bott::RAW_FILES_DIRECTORY = "raw_files/";
const string Bott::CHA_RAW_FILES_DIRECTORY = "cha_raw_files/";
const string Bott::RESULTS_DIRECTORY = "results/";
const string Bott::CHA_RESULTS_DIRECTORY = "cha_results/";
const string Bott::EXTENTION = ".bott";

const string Bott::SOURCE_CODE_BEGIN = "__SOURCE-CODE-BEGIN-LABLE__";
const string Bott::SOURCE_CODE_END = "__SOURCE-CODE-END-LABLE__";
const string Bott::COMPILE_INFO_BEGIN = "__COMPILE-INFO-BEGIN-LABLE__";
const string Bott::COMPILE_INFO_END = "__COMPILE-INFO-END-LABLE__";
const string Bott::DATA_DETAIL_BEGIN = "__DATA-DETAIL-BEGIN-LABLE__";
const string Bott::DATA_DETAIL_END = "__DATA-DETAIL-END-LABLE__";
const string Bott::CHALLENGE_DETAIL_BEGIN = "__CHALLENGE-DETAIL-BEGIN-LABLE__";
const string Bott::CHALLENGE_DETAIL_END = "__CHALLENGE-DETAIL-END-LABLE__";

Bott::Bott() {
  //ctor
}

Bott::~Bott() {
  //dtor
}

string Bott::parseUntil(string end) {
  string res = "", tmps;
  getline(fin, tmps);
  getline(fin, tmps);
  while (tmps != end) {
    res += tmps + "\n";
    getline(fin, tmps);
  }
  return res;
}

Bott::Bott(string filename) {
  Document document;
  document.Parse(loadAllFromFile(filename).c_str());
  if (document.HasMember("type")) {
    type = document["type"].GetInt();
  }
  if (document.HasMember("runid")) {
    runid = document["runid"].GetString();
  }
  if (document.HasMember("source")) {
    src = document["source"].GetString();
  }
  if (document.HasMember("compileInfo")) {
    ce_info = document["compileInfo"].GetString();
  }
  if (document.HasMember("language")) {
    language = document["language"].GetString();
  }
  if (document.HasMember("pid")) {
    pid = document["pid"].GetString();
  }
  if (document.HasMember("testcases")) {
    number_of_testcases = document["testcases"].GetString();
  }
  if (document.HasMember("timeLimit")) {
    time_limit = document["timeLimit"].GetString();
  }
  if (document.HasMember("caseLimit")) {
    case_limit = document["caseLimit"].GetString();
  }
  if (document.HasMember("memoryLimit")) {
    memory_limit = document["memoryLimit"].GetString();
  }
  if (document.HasMember("spjStatus")) {
    spj = document["spjStatus"].GetString();
  }
  if (document.HasMember("vname")) {
    vname = document["vname"].GetString();
  }
  if (document.HasMember("vid")) {
    vid = document["vid"].GetString();
  }
  if (document.HasMember("memoryUsed")) {
    memory_used = document["memoryUsed"].GetString();
  }
  if (document.HasMember("timeUsed")) {
    time_used = document["timeUsed"].GetString();
  }
  if (document.HasMember("result")) {
    result = document["result"].GetString();
  }
  if (document.HasMember("challenge")) {
    if (document["challenge"].HasMember("id")) {
      cha_id = document["challenge"]["id"].GetString();
    }
    if (document["challenge"].HasMember("dataType")) {
      data_type = document["challenge"]["dataType"].GetString();
    }
    if (document["challenge"].HasMember("dataLanguage")) {
      data_lang = document["challenge"]["dataLanguage"].GetString();
    }
    if (document["challenge"].HasMember("dataDetail")) {
      data_detail = document["challenge"]["dataDetail"].GetString();
    }
    if (document["challenge"].HasMember("detail")) {
      cha_detail = document["challenge"]["detail"].GetString();
    }
    if (document["challenge"].HasMember("result")) {
      cha_result = document["challenge"]["result"].GetString();
    }
  }
}

void Bott::addIntValue(Document & document, const char * name, int v) {
  Value value(v);
  document.AddMember(StringRef(name), value, document.GetAllocator());
}

void Bott::addStringValue(Document & document, const char * name, string v) {
  Value value(StringRef(v.c_str()));
  document.AddMember(StringRef(name), value, document.GetAllocator());
}

void Bott::addStringValueToRef(
    Document & document, Value & ref, const char * name, string v) {
  Value value(StringRef(v.c_str()));
  document.AddMember(StringRef(name), value, document.GetAllocator());
}

void Bott::toFile() {
  Document document;
  document.SetObject();
  addIntValue(document, "type", type);
  addStringValue(document, "runid", runid);
  addStringValue(document, "memoryUsed", memory_used);
  addStringValue(document, "timeUsed", time_used);
  addStringValue(document, "result", result);
  addStringValue(document, "compileInfo", ce_info);
  addStringValue(document, "remoteRunid", remote_runid);
  
  FILE *fp = fopen(out_filename.c_str(), "w");
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  document.Accept(writer);
  fprintf(fp, "%s", buffer.GetString());
  fclose(fp);
}
