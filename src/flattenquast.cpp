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

#include "flattenquast.h"
#include "global.h"
#include "graphviz.h"
#include "latex.h"
#include "graph.h"


namespace fada{

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
void
Branch::Set(vector<vector<Inequation*> >* __guard, int __id, FADA_Index* __instance){
SetBottom(false);
SetCondition(__guard);
SetInstance(__instance);
SetStmt(__id);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

Branch::Branch(vector<vector<Inequation*> >* __guard, int __id, FADA_Index* __instance){
Set(__guard,__id,__instance);
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

Branch::Branch(vector<vector<Inequation*> >* __guard){
Set(__guard);
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void
Branch::Set(vector<vector<Inequation*> >* __guard){
SetBottom(true);
SetCondition(__guard);
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
void
Branch::Initialize(void){
vector<vector<Inequation*> > empty;
Set(&empty);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

string
Branch::Print_str(string __prefix){
string result=__prefix;
if(IsBottom())
   result+="_|_   IF :";
else
   result+=PrintOperation(GetStmt(),GetInstance()->GetIndex())+" IF :";
result+=PrintInequations(GetCondition());
return  result;
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

string
Branch::PrettyPrint_str(string __prefix){
string result;
if(IsBottom())
   result+= __prefix+"Undefined Source,  IF:\t";
else
   result+=__prefix+PrintOperation(GetStmt(),GetInstance()->GetIndex())+",   IF :\t";

for(vector<vector<Inequation*> >::iterator it=GetCondition()->begin(); it != GetCondition()->end();++it){
   if(it !=GetCondition()->begin())
      result+=__prefix+"\tOR    ";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti){
      if(iti != (*it).begin())
         result+=" AND ";
      result+=(*iti)->Print_str();
      }
   }
if(GetCondition()->size()==0)
   result+=" TRUE ";
return result;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

string
Branch::Print_Tex_str(string title){
string result="";
// result+=title;

if(IsBottom())      result+="$\\bot$\n";
else         result+="$"+PrintOperation(GetStmt(),GetInstance()->GetIndex())+"$";

string constraints="";
for(vector<vector<Inequation*> >::iterator it=GetCondition()->begin(); it != GetCondition()->end();++it){
   if(it !=GetCondition()->begin())
      constraints+="\t\\\\";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti){
      if(iti != (*it).begin())
         constraints+=", ";
      constraints+=(*iti)->Print_Tex_str();
      }
   }
if(GetCondition()->size() == 0)
   constraints="TRUE";

return title+TexFigure("","",result+TexEqArray(constraints));

}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

bool
Branch::SameSolution(Branch* __branch){

if(IsBottom() && __branch->IsBottom())
   return true;
if(IsBottom() && ! __branch->IsBottom() || ! IsBottom() && __branch->IsBottom())
   return false;
return (PrintOperation(GetStmt(),GetInstance()->GetIndex()) ==  PrintOperation(__branch->GetStmt(),__branch->GetInstance()->GetIndex()));
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

bool
Branch::Merge(Branch* __branch){

if(!SameSolution(__branch)){

   return false;
   }
for(vector<vector<Inequation*> >::iterator it=__branch->GetCondition()->begin(); it != __branch->GetCondition()->end(); ++it)
   GetCondition()->push_back(*it);

return true;
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void
Branch::RemoveConstraintsOnAlphas(set<string>* __alphas){
vector<vector<Inequation* > > result;
for(vector <vector <Inequation* > >::iterator it=GetCondition()->begin(); it != GetCondition()->end(); ++it){
   vector<Inequation*> local_result;
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti){
      if(!(*iti)->IsFuzzy(__alphas))
         local_result.push_back(*iti);
      }
   if(local_result.size() > 0)
      result.push_back(local_result);
   }
SetCondition(&result);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

string
Branch::PrintGraphViz(string node_name_prefix){
string final_result;
string result;
for(vector<vector<Inequation*> >::iterator it=GetCondition()->begin(); it != GetCondition()->end();++it){
   if(it !=GetCondition()->begin())
      result+="\t\\n  OR    ";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti){
      if(iti != (*it).begin())
         result+="   AND   ";
      result+=(*iti)->Print_str();
      }
   }
if(GetCondition()->size()==0)
   result+=" TRUE ";

final_result=AddDOTNode(node_name_prefix+"GuardCondition",result);
string solution;
if(IsBottom())
   solution="Undefined";
else
   solution=PrintOperation(GetStmt(),GetInstance()->GetIndex());
final_result+=AddDOTNode(node_name_prefix+"Solution",solution);
final_result+=AddDotEdge(true,node_name_prefix+"GuardCondition",node_name_prefix+"Solution","");
return final_result;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

FlatQuast::~FlatQuast()
{
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

string
FlatQuast::Print_str(string __prefix){
return PrettyPrint_str(__prefix);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

string
FlatQuast::PrettyPrint_str(string __prefix){
string result=__prefix;
for(list<Branch*>::iterator it=GetBranches()->begin(); it != GetBranches()->end(); ++it)
   result+=(*it)->PrettyPrint_str(__prefix);
return result;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

string
FlatQuast::PrintGraphViz(void){
string result;
string node_name="Node1";
for(list<Branch*>::iterator it=GetBranches()->begin(); it != GetBranches()->end(); ++it){
   result+=(*it)->PrintGraphViz(node_name);
   node_name+="1";
   }
result=AddDOTHeader(true,result);
return result;   
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void
FlatQuast::ToGraphVizFile(string file_name){
WriteToFile(file_name,PrintGraphViz());
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

string
FlatQuast::Print_Tex_str(void){
string result="";
int i=1;
for(list<Branch*>::iterator it=GetBranches()->begin(); it != GetBranches()->end(); ++it){
   ostringstream oss;
   oss<<"Source #"<<i;
   result+=(*it)->Print_Tex_str(oss.str());
   ++i;
   }
// result=TexDocument(result);
// result=TexDocumentClass("",result);
return result;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void
FlatQuast::Print_Tex(string file_name){
WriteToFile(file_name,Print_Tex_str());
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


void
FlatQuast::AddBranch(Branch* __branch){
for(list<Branch*>::iterator it=GetBranches()->begin(); it != GetBranches()->end(); ++it){
   if((*it)->Merge(__branch))
      return;
   }
GetBranches()->push_back(__branch);
}

//////////////////////////////////////////////////////////////
void
FlatQuast::RemoveConstraintsOnAlphas(set<string>* __alphas){
for(list<Branch*>::iterator it=GetBranches()->begin(); it != GetBranches()->end(); ++it)
   (*it)->RemoveConstraintsOnAlphas(__alphas);
}

//////////////////////////////////////////////////////////////
void
FlatQuast::GenerateInstanceWizeEdges(unsigned long int read_stmt,string conflictual,FADA_Index* read_index, Graph* g){
for(list<Branch*>::iterator it=GetBranches()->begin(); it != GetBranches()->end(); ++it)
   {
   Edge e(conflictual,read_index,DNFTerms_to_Condition((*it)->GetCondition()),(*it)->GetInstance());

    if((*it)->IsBottom()){
//        g->AddEdge(-1,read_stmt,&e);
      }
    else
       g->AddEdge((*it)->GetStmt(),read_stmt,&e);
   }
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
};



