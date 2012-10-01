/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "html.h"
#include <sstream>

std::string
AddHTMLLink(std::string link,std::string text){
return "<a href=\""+link+"\">"+text+"</a>";
}

std::string
AddHTMLHeader(std::string title, std::string frames, bool is_framed){
std::string result;
result="<HTML>\n <HEAD>\n  <TITLE>\n   "+title+"\n   </TITLE>\n  </HEAD>\n "+frames;
result+="\n</HTML>";
return result;
}

std::string
AddHTMLHeader(std::string title, std::string body){
std::string result;
result="<HTML>\n <HEAD>\n  <TITLE>\n   "+title+"\n   </TITLE>\n  </HEAD>\n <BODY>\n  <H3>\n   "+body;
result+="\n  </H3>\n </BODY>\n</HTML>";
return result;
}

std::string
Linking(std::string hyper_link, std::string target,std::string click_me_message){
if(target == "")
   return "<a href=\""+hyper_link+"\">"+click_me_message+"</a>";
else
   return "<a href=\""+hyper_link+"\" target=\""+target+"\">"+click_me_message+"</a>";
}


std::string
Concat(std::string s, int i){
std::ostringstream res;
res<<s<<"_"<<i;
return res.str();
}


std::string
ComputeLink(std::string prefix, int i){
return "./"+Concat(prefix,i)+".html";
}

std::string
ComputeLink(std::string name){
return "./"+name+".html";
}


std::string
Verbatimize(std::string s){
return "<PRE>"+s+"</PRE>";
}

std::string
AddHTMLHeader(std::string title,std::string base, std::string body){
std::string result;
result="<HTML>\n <HEAD>\n  <TITLE>\n   "+title+"\n   </TITLE>\n";
result+="<BASE TARGET=\""+base+"\">\n  </HEAD>\n <BODY>\n  "+body;
result+="\n </BODY>\n</HTML>";
return result;
}

std::string
CreateFrames(std::string pgm,std::string ref,std::string def, std::string page1, std::string page2, std::string page3){
std::string result;
result="<FRAMESET COLS=\"30%,70%\">\n";
result+="<FRAME SRC=\""+page1+"\"  NAME=\""+pgm+"\">\n";
result+="<FRAMESET ROWS=\"05%,95%\">\n";
result+="<FRAME SRC=\""+page2+"\"  NAME=\""+ref+"\">\n";
result+="<FRAME SRC=\""+page3+"\"  NAME=\""+def+"\">\n";
result+="</FRAMESET>\n";
result+="</FRAMESET>\n";
return result;
}

std::string
GenerateHomePage(void){
return AddHTMLHeader("home","Here will be shown the definitions of variables. For that, choose a statement in the frame at the left, then select one variable (in the list above).");
}



