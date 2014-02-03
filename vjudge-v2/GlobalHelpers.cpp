#include "GlobalHelpers.h"

/**
 * Convert an integer to a string
 * @param i     The integer
 * @return      The converted string
 */
string intToString(int i) {
    char str[15];
    sprintf(str, "%d", i);
    return (string)str;
}

/**
 * Convert a string to an integer
 * WARNING: did not check if it's valid
 * @param str   The string
 * @return      The converted interger
 */
int stringToInt(string str) {
    return atoi(str.c_str());
}


/**
 * Get current date/time, format is YYYY-MM-DD HH:mm:ss
 * https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
 * @return Current datetime in YYYY-MM-DD HH:mm:ss
 */
const string currentDateTime() {
    time_t     now = time(NULL);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);

    return buf;
}

/**
 * Get current date
 * https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
 * @return Current date in YYYY-MM-DD
 */
const string currentDate() {
    time_t     now = time(NULL);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

    return buf;
}

/**
 * Split the string into pieces by the delimeter
 * taken from https://stackoverflow.com/a/236803
 * @param str                   The original string
 * @param delim                 Delimeter
 * @param removeAppendedNull    Where to remove the appended empty strings
 * @return Splitted string
 */
vector<string> split(const string &str, char delim, bool removeAppendedNull) {
    vector<string> elems;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    if (removeAppendedNull) {
        while (!elems.empty() && elems.back().empty()) {
            elems.pop_back();
        }
    }
    return elems;
}

/**
 * Split the string into pieces by the delimeter, ignore appended empty strings
 * @param str   The original string
 * @param delim Delimeter
 * @return Splitted string
 */
vector<string> split(const string &str, char delim) {
    return split(str, delim, true);
}


/**
 * Load the whole text file content to a string
 * @param filename      File to load
 * @return File content
 */
string loadAllFromFile(string filename) {
    int tried = 0;
    string res = "", tmps;
    fstream fin(filename.c_str(), fstream::in);
    
    while (fin.fail() && tried++ < 10) {
        fin.open(filename.c_str(), fstream::in);
        return res;
    }
    
    if (fin.fail()) return res;
    while (getline(fin,tmps)) {
        if (res != "") res += "\n";
        res += tmps;
        if (fin.eof()) break;
    }
    fin.close();
    return res;
}

char dec2hexChar(short int n) {
    if (0 <= n && n <= 9) return char( short('0') + n);
    else if (10 <= n && n <= 15)return char( short('A') + n - 10);
    else return char(0);
}

short int hexChar2dec(char c) {
    if ('0' <= c && c <= '9') return short(c - '0');
    else if ('a' <= c && c <= 'f') return (short(c - 'a') + 10);
    else if ('A' <= c && c <= 'F') return (short(c - 'A') + 10);
    else return -1;
}

/**
 * URL escape a string
 * @param URL   Orignal string
 * @return URL escaped string
 */
string escapeURL(const string &URL) {
    string result = "";
    for (unsigned int i = 0; i < URL.size(); i++) {
        char c = URL[i];
        if (
                ('0' <= c && c <= '9') ||
                ('a' <= c && c <= 'z') ||
                ('A' <= c && c <= 'Z') ||
                c == '/' || c == '.'
                ) result += c;
        else {
            int j = (short int) c;
            if (j < 0) j += 256;
            int i1, i0;
            i1 = j / 16;
            i0 = j - i1 * 16;
            result += '%';
            result += dec2hexChar(i1);
            result += dec2hexChar(i0);
        }
    }
    return result;
}

/**
 * Unescape a URL escaped string
 * @param URL   URL escaped string
 * @return Orignal string
 */
string unescapeURL(const string &URL) {
    string result = "";
    for (unsigned int i = 0; i < URL.size(); i++) {
        char c = URL[i];
        if (c != '%') result += c;
        else {
            char c1 = URL[++i];
            char c0 = URL[++i];
            int num = 0;
            num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
            result += char(num);
        }
    }
    return result;
}

/**
 * Trim leading and trailing spaces
 * @param str   Original string
 * @return Trimmed string
 */
string trim(string str) {
    string spaces = " \t\n\r";
    size_t start = str.find_first_not_of(spaces);
    size_t end = str.find_last_not_of(spaces);
    return str.substr(start, end - start + 1);
}