#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <dirent.h>

using namespace std;

// Path & Environment Utilities
string pwd(void) {
  char buf[PATH_MAX];
  if(getcwd(buf, PATH_MAX) == NULL)
    return "";
  return string(buf);
}

string get_env(const string &var) {
  const char *val = getenv(var.c_str());
  if(val == NULL) return "";
  return string(val);
}

// Type Conversion
int str2int(const string &s) {
  stringstream ss(s);
  int x = 0;
  ss >> x;
  return x;
}

double str2double(const string &s) {
  stringstream ss(s);
  double x = 0;
  ss >> x;
  return x;
}

string int2str(int x) {
  stringstream ss;
  ss << x;
  return ss.str();
}

string double2str(double x) {
  stringstream ss;
  ss << x;
  return ss.str();
}

// String Utilities
void split_words(vector<string> &out, const string &s) {
  out.clear();
  stringstream ss(s);
  string w;
  while(ss >> w)
    out.push_back(w);
}

void replace_all(string &s, const string &from, const string &to) {
  if(from.empty()) return;

  size_t pos = 0;
  while((pos = s.find(from, pos)) != string::npos) {
    s.replace(pos, from.length(), to);
    pos += to.length();
  }
}

bool extract_between(string &out, const string &src, const string &left, const string &right) {
  size_t p0 = src.find(left);
  if(p0 == string::npos) return false;

  p0 += left.size();
  size_t p1 = src.find(right, p0);
  if(p1 == string::npos) return false;

  out = src.substr(p0, p1-p0);
  return true;
}

bool extract_after(string &out, const string &src, const string &left) {
  size_t p = src.find(left);
  if(p == string::npos) return false;

  p += left.size();
  out = src.substr(p);
  return true;
}

bool pick_word(string &out, const string &s, int index) {
  stringstream ss(s);
  string w;
  int i = 0;
  while(ss >> w) {
    if(i == index) {
      out = w;
      return true;
    }
    i++;
  }
  return false;
}

bool contains(const string &s, const string &key) {
  return s.find(key) != string::npos;
}

// Directory / filesystem utilities
bool is_file(const string &path) {
  return (access(path.c_str(), F_OK) == 0);
}

bool is_dir(const string &path) {
  struct stat st;
  if(stat(path.c_str(), &st) != 0) return false;
  return (st.st_mode & S_IFDIR) != 0;
}

bool list_dir(vector<string> &out, const string &path) {
  out.clear();
  DIR *dp = opendir(path.c_str());
  if(!dp) return false;

  struct dirent *ent;
  while((ent = readdir(dp)) != NULL) {
    string name(ent->d_name);
    if(name != "." && name != "..")
      out.push_back(name);
  }
  closedir(dp);
  return true;
}

void collect_files(vector<string> &out, const string &root) {
  out.clear();

  vector<string> dirs;
  dirs.push_back(root);

  for(size_t i = 0; i < dirs.size(); i++) {
    const string current = dirs[i];

    vector<string> children;
    if(!list_dir(children, current)) {
      out.push_back(current);
      continue;
    }

    for(size_t k = 0; k < children.size(); k++) {
      string full = current+"/"+children[k];
      if(is_dir(full)) dirs.push_back(full);
      else out.push_back(full);
    }
  }
}

void search_key(const string &root, const string &key) {
  queue<string> q;
  q.push(root);

  while(!q.empty()) {
    string dir = q.front(); q.pop();

    vector<string> entries;
    if(!list_dir(entries, dir)) continue;

    for(size_t i = 0; i < entries.size(); i++) {
      string full = dir+"/"+entries[i];

      if(is_dir(full)) q.push(full);
      if(entries[i].find(key) != string::npos)
        cout << full << endl;
    }
  }
}

void remove_comments(string &line, bool &in_block) {
  string out;
  bool in_string = 0;

  for(size_t i = 0; i < line.size(); i++) {
    if(in_block) {
      if(i+1 < line.size() && line[i] == '*' && line[i+1] == '/') {
        in_block = 0;
        i++;
      }
      continue;
    }
    if(line[i] == '"' && (i == 0 || line[i-1] != '\\')) {
      in_string = !in_string;
      out += line[i];
      continue;
    }

    if(!in_string && i+1 < line.size() && line[i] == '/' && line[i+1] == '*') {
      in_block = 1;
      i++;
      continue;
    }
    if(!in_string && i+1 < line.size() && line[i] == '/' && line[i+1] == '/')
      break;

    out += line[i];
  }

  size_t end = out.size();
  while(end > 0 && isspace(out[end-1]))
    end--;
  out.resize(end);

  line = out;
}

// Duplicate Removal
void unique_strings(vector<string> &in) {
  set<string> seen;
  vector<string> out;

  for(size_t i = 0; i < in.size(); i++) {
    const string &s = in[i];
    if(s.empty()) continue;

    if(seen.count(s) == 0) {
      seen.insert(s);
      out.push_back(s);
    }
  }
  in = out;
}

// Binary / Hex utilities
unsigned int hex2dec(const string &hexstr) {
  unsigned int x = 0;
  stringstream ss;
  ss << hex << hexstr;
  ss >> x;
  return x;
}

string dec2hex(unsigned int x) {
  stringstream ss;
  ss << hex << x;
  return ss.str();
}

string dec2bin(unsigned int x) {
  if(x == 0) return "0";
  string s = "";
  while(x > 0) {
    s = char('0'+(x&1))+s;
    x >>= 1;
  }
  return s;
}

unsigned int bin2dec(const string &b) {
  unsigned int x = 0;
  size_t i = 0;
  while(i < b.size()) {
    x = (x << 1) | (b[i]-'0');
    i++;
  }
  return x;
}

// Braced content
void find_braced_content(vector<string> &out, const string &src) {
  out.clear();

  int depth = 0;
  size_t start = 0;

  for(size_t i = 0; i < src.size(); i++) {
    if(src[i] == '{') {
      if(depth == 0)
        start = i+1;
      depth++;
    } else if(src[i] == '}') {
      depth--;
      if(depth == 0) {
        string tmp = src.substr(start, i-start);
        replace_all(tmp, " ", "");
        replace_all(tmp, "\t", "");
        out.push_back(tmp);
      }
    }
  }
}

// Time Stamp Builder
string timestamp_now() {
  time_t t = time(NULL);
  struct tm *tmv = localtime(&t);
  if(!tmv) return "";

  int arr[6] = {
    tmv->tm_year + 1900,
    tmv->tm_mon + 1,
    tmv->tm_mday,
    tmv->tm_hour,
    tmv->tm_min,
    tmv->tm_sec
  }; 

  string s = "";
  for(int i = 0; i < 6; i++) {
    string v = int2str(arr[i]);
    if(v.size() == 1) v = "0"+v;
    if(i == 3) s += "_";
    s += v;
  }
  return s;
}

int main(int argc, char *argv[]) {
  cin.tie();
  ios::sync_with_stdio(0);

  return 0;
}
