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

#include "latex.h"
#include <global.h>


namespace fada{
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

string
TexFigure(string caption,string label,string figure_content){
string result;
result="\n\\begin{figure}\n";
result+=figure_content;
if(caption!="")
   result+="\n\\caption{"+caption+"}\n";
if(label!="")
   result+="\n\\label{"+label+"}\n";

result+="\n\\end{figure}";
return result;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

string
TexEqArray(string equations){
string result;
result="\n\\begin{eqnarray}\n";
result+=equations;
result+="\n\\end{eqnarray}\n";
return result;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

string
TexDocument(string document){
string result;
result="\n\\begin{document}\n";
result+=document;
result+="\n\\end{document}\n";
return result;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

string
TexDocumentClass(string options, string doc){
string result;
result="\n\\documentclass[";
if(options!="")
   result+=options;
else
   result+="a4paper,10pt";
result+="]{article}\n";
return result+doc;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
string
TexIndex(string indexed, string index){
return indexed+"_{"+index+"}";
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
string
TexExponent(string base, string exponent){
return base+"^{"+exponent+"}";
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
string
TexPrintDNFTerm(vector<Inequation*>* __v){
vector<Inequation*> v=EliminateTrueAndDoubles(__v);
string result="";
for(vector<Inequation*>::iterator it=v.begin(); it != v.end(); ++it){
   if(it != v.begin())   result+=" \\wedge ";
   result+=(*it)->Print_Tex_str();
   }
return result;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
string
TexFullPrintDNFTerm(vector<Inequation*>* __v, vector<string> * __var, vector<string>* __param){
string result="";
for(vector<Inequation*>::iterator it=__v->begin(); it != __v->end(); ++it){
   if(it != __v->begin())   result+=" \\wedge ";
   result+=(*it)->Full_Tex_Print_str(__var, __param);
   }
return result;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
string
TexPrintDNFCondition(vector<vector<Inequation*> >* __v){
string result="";
for(vector<vector<Inequation*> >::iterator it=__v->begin(); it != __v->end(); ++it){
   if(it != __v->begin())   result+="\\\\ \n \\vee ";
   result+=TexPrintDNFTerm(&(*it));
   }
return result;
}

//////////////////////////////////////////////////////////
string
TexFullPrintDNFCondition(vector<vector<Inequation*> >* __v, vector<string>* __var,vector<string>* __param){
string result="";
for(vector<vector<Inequation*> >::iterator it=__v->begin(); it != __v->end(); ++it){
   if(it != __v->begin())   result+="\\\\ \n \\vee ";
   result+=TexFullPrintDNFTerm(&(*it),__var,__param);
   }
return result;

}
//////////////////////////////////////////////////////////
string
TexMath(string equations){
return "\n\\begin{math}\n"+equations+"\n\\end{math}\n";
}
//////////////////////////////////////////////////////////
string
PreprocessVariableName(string s){
string r="";
for(int i=0;i<s.size();i++){
   cout<<"\n"<<s[i];
   if(s[i]=='_' && i<s.size() -1 && s[i+1]!='{')
      r+="\\";
   r+=s[i];
   }
return r;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
}


