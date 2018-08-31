#include "../../hdr/prototype.h"

void in_(string& src){
   while (1){
      int idx;
      if ((idx=find(src, "in("))<0) break;
      if ((idx=find(src, "in("))>=0){
         vector<P> v;
         int i = idx+3;
         bool flg = true;
         string type, name;  string key;
         while (flg){
            type = name = key = "";
            while (1){
                if (src[i]!=' ') break;
                ++i;
            }
            while (1){
               if (src[i]==')'){
                  flg = false;
                  break;
               }
               else if (src[i]=='['){
                  while (1){
                     if (src[i]==','){
                        break;
                     }
                     if (src[i]==')'){
                        flg = false;
                        break;
                     }
                     key += src[i];
                     ++i;
                  }
                  break;
               }
               else if (src[i]==' '){
                  ++i;
                  while (1){
                     if (src[i]==','){
                        break;
                     }
                     else if (src[i]==')'){
                        flg = false;
                        break;
                     }
                     else if (src[i]=='['){
                        while (1){
                           if (src[i]==','){
                              break;
                           }
                           if (src[i]==')'){
                              flg = false;
                              break;
                           }
                           key += src[i];
                           ++i;
                        }
                        break;
                     }
                     else {
                        name += src[i];
                        ++i;
                     }
                  }
                  break;
               }
               else if (src[i]==','){
                  break;
               }
               else {
                  type += src[i];
                  ++i;
                  continue;
               }
            }
            if (name==""){
               name = type;
               if (!v.empty()){
                  if ((v[v.size()-1].first).find("[")) type = (v[v.size()-1].first).substr(0, (v[v.size()-1].first).find("["));
                  else type = v[v.size()-1].first;
               }
               else type = "int";
            }
            type += key;
            v.push_back(P(type, name));
            dict[name] = type;
            ++i;
         }
         string strend = src.substr(i);
         src = src.substr(0, idx);
         string typ = v[0].first;
         int i_s = 0, i_e = 0;
         i = 0;
         while (i <= v.size()){
            if (v[i].first==typ && i!=v.size()) i_e = i;
            else {
               if (typ.find("[")==string::npos){
                  src += typ;
                  for (int j = i_s; j <= i_e; ++j){
                     if (j!=i_s) src += ",";
                     src += " " + v[j].second;
                  }
                  src += ";\n";
                  src += "cin";
                  for (int j = i_s; j <= i_e; ++j){
                     src += " >> " + v[j].second;
                  }
                  src += ";\n";
                  i_s =  i;  i_e = i;
               }
               else {
                  int cnt = 0;
                  for (int j = 0; j < typ.size(); ++j) if (typ[j]=='[') ++cnt;
                  switch (cnt){
                     case 1: {
                        src += typ.substr(0, typ.find("["));
                        string k = typ.substr(typ.find("[")+1, typ.find("]")-typ.find("[")-1);
                        for (int j = i_s; j <= i_e; ++j){
                           if (j!=i_s) src += ",";
                           src += " " + v[j].second + "[$" + k + "]";
                        }
                        src += ";\n";
                        src += "@for(int i = 0; i @< " + k + "; ++i) {cin";
                        for (int j = i_s; j <= i_e; ++j){
                           src += " >> " + v[j].second + "[$i]";
                        }
                        src += ";};\n";
                        i_s =  i;  i_e = i;
                        break;
                     }
                     case 2: {
                        src += typ.substr(0, typ.find("["));
                        string n = typ.substr(typ.find("[")+1, typ.find("]")-typ.find("[")-1);
                        string m = typ.substr(typ.rfind("[")+1, typ.rfind("]")-typ.rfind("[")-1);
                        for (int j = i_s; j <= i_e; ++j){
                           if (j!=i_s) src += ",";
                           src += " " + v[j].second + "[$" + n + "]" + "[$" + m + "]";
                        }
                        src += ";\n";
                        src += "@for(int i = 0; i @< " + n + "; ++i){;\n";
                        src += tab + "@for(int j = 0; j @< " + m + "; ++j){;\n" + tab + tab + "cin";
                        for (int j = i_s; j <= i_e; ++j){
                           src += " >> " + v[j].second + "[$i][$j]";
                        }
                        src += ";\n" + tab + "};\n};\n";
                        i_s =  i;  i_e = i;
                        break;
                     }
                  }
               }
            }
            if (i!=v.size()) typ = v[i].first;
            ++i;
         }
         src += strend;
      }
   }
}
