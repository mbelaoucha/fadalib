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

/*!
\file statement.cpp
\author Marouane Belaoucha
*/
#include "statement.h"
#include "constants.h"
#include "global.h"
#include "algorithm"
#include "html.h"

namespace fada{
string 
Statement::ToHtml(string work_dir, string prefix, string indent,string def_frame,Options* op){
ostringstream result;

string link=ComputeLink (Concat(prefix,GetID()));

if(IsAssignment()){
   if(GetReferences()->GetRV()->size() > 0)   // there is some read variables
         result<<indent<<Linking(link,"",Concat("S",GetID())+" : "+GetAssignment()->Pretty_print_str());

   else
      result<<indent<<Concat("S",GetID())+" : "+GetAssignment()->Pretty_print_str();

   GetReferences()->ToHtml(work_dir,Concat(prefix, GetID()),def_frame,op);
   }
if(IsControl()){
   if(GetReferences()->GetRV()->size() > 0)
      result<<indent<<Linking(link,"",GetControl()->PrettyPrint_str());
   
   else
      result<<indent<<GetControl()->PrettyPrint_str();

   bool accodance=false;
   if(GetSurroundedStatements()->size() > 1){
      result<<indent+"   {";
      accodance=true;
      }
   
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      result<<(*it)->ToHtml(work_dir,prefix,indent+"   ",def_frame,op);

   if(accodance){
      result<<indent+"   }";
      accodance=true;
      }

   if(GetControl()->IsIfElse()){
      result<<indent<<"else";
      bool accodance=false;
      if(GetElseSurroundedStatements()->size() > 1){
         result<<indent+"   {";
         accodance=true;
         }

      for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin(); it != GetElseSurroundedStatements()->end(); ++it)
         result<<(*it)->ToHtml(work_dir,prefix,indent+"   ",def_frame,op);

         if(accodance){
      result<<indent+"   }";
      accodance=true;
      }

      }
   
   GetReferences()->ToHtml(work_dir,Concat(prefix, GetID()),def_frame,op);

   }

if(IsProgram()){
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      result<<(*it)->ToHtml(work_dir,prefix,indent,def_frame,op);
      }
return result.str();
}
////////////////////////////////////////////////
////////////////////////////////////////////////
Statement::~Statement()
{
}
////////////////////////////////////////////////
////////////////////////////////////////////////
void
Statement::Initialize(void)
{
this->SetProgram(false);
this->SetStatementKind(true);
this->SetAssignment(NULL);
this->SetControl(NULL);
vector<Statement*>   v;
this->SetSurrStmt(&v);
this->SetElseSurrStmt(&v);
}
////////////////////////////////////////////////
////////////////////////////////////////////////
void
Statement::Set(vector<Statement*>* __stmts)
{
this->SetProgram(true);
this->SetSurrStmt(__stmts);
}

////////////////////////////////////////////////
////////////////////////////////////////////////
Statement::Statement(vector<Statement*>* __stmts)
{
this->Initialize();
Set(__stmts);
}
////////////////////////////////////////////////
////////////////////////////////////////////////
void
Statement::Set(Assignment*   assign)
{
this->SetStatementKind(true);
this->SetAssignment(assign);
}
////////////////////////////////////////////////
////////////////////////////////////////////////

void
Statement::Set(Control* __control, vector<Statement*>* __surr)
{
this->SetStatementKind(false);
this->SetControl(__control);
this->SetSurrStmt(__surr);
}

////////////////////////////////////////////////
////////////////////////////////////////////////
void
Statement::Set(Control* __control, vector<Statement*>* __surr, vector<Statement*>* __surr_else)
{
this->SetStatementKind(false);
this->SetControl(__control);
this->SetSurrStmt(__surr);
this->SetElseSurrStmt(__surr_else);
GetControl()->SwitchToIfElse();
}


Statement::Statement(){
Initialize();
SetProgram(true);
}
////////////////////////////////////////////////
////////////////////////////////////////////////
Statement::Statement(Assignment* __assign)
{
this->Initialize();
this->Set(__assign);
}

////////////////////////////////////////////////
Statement::Statement(Control* __control){
Initialize();
this->SetStatementKind(false);
SetControl(__control);
}
////////////////////////////////////////////////
Statement::Statement(Control* __control, vector<Statement*>* __surr)
{
this->Initialize();
this->Set(__control, __surr);
}



void   
Statement::Enclose(Assignment* assign,bool in_else_part){
Enclose(new Statement(assign),in_else_part);
}

void
Statement::Enclose(Statement* s,bool in_else_part){
if(!in_else_part)
   GetSurroundedStatements()->push_back(s);

else
   if(IsControl()){
      GetControl()->SwitchToIfElse();
      GetElseSurroundedStatements()->push_back(s);
      }
   else{
      cerr<<"\n Statement::AddStatement ... inappropriate case\n";
      throw(INAPPROPRIATE_CASE);
      }
}
////////////////////////////////////////////////
////////////////////////////////////////////////
Statement::Statement(Control* __control, vector<Statement*>* __surr, vector<Statement*>* __surr_else)
{
this->Initialize();
this->Set(__control, __surr, __surr_else);
}

////////////////////////////////////////////////
////////////////////////////////////////////////

void
Statement::Link2Parent(void){
if(this->IsProgram())
   this->SetParent(NULL);

if(this->IsAssignment())
      return;

for(vector<Statement*>::iterator it=this->GetSurroundedStatements()->begin(); it != this->GetSurroundedStatements()->end(); ++it){
   (*it)->SetParent(this);
   (*it)->Link2Parent();
   }

if(this->IsControl() && this->GetControl()->IsIfElse())
   for(vector<Statement*>::iterator it=this->GetElseSurroundedStatements()->begin(); it != this->GetElseSurroundedStatements()->end(); ++it){
      (*it)->SetParent(this);
      (*it)->Link2Parent();
      }
return;
}

////////////////////////////////////////////////
////////////////////////////////////////////////

string
Statement::Print_str(string tabs)
{
if(this->IsAssignment())
   return tabs+this->GetAssignment()->Print_str();

string result;
if(this->IsControl())
   result="\n"+tabs+this->GetControl()->Print_str();
else
   result="\n"+tabs+"main";

for(vector<Statement*>::iterator it=this->GetSurroundedStatements()->begin(); it != this->GetSurroundedStatements()->end(); ++it)
   result+="\n"+(*it)->Print_str(tabs+"\t");

if(!this->GetElseSurroundedStatements()->empty())
   {
   for(vector<Statement*>::iterator it=this->GetElseSurroundedStatements()->begin(); it != this->GetElseSurroundedStatements()->end(); ++it)
      result+="\n"+(*it)->Print_str(tabs+"\t");
   }
return result;
}
////////////////////////////////////////////////
////////////////////////////////////////////////
void
Statement::Print(void)
{
cout<<this->Print_str("");
}
////////////////////////////////////////////////
////////////////////////////////////////////////

string
Statement::Regenerate_C_Code(string prefix)
{
string   indent="\t";

if(this->IsAssignment())
   return prefix+this->GetAssignment()->Generate_C_Code()+";";

string result;

if(this->IsControl())
   {
   if(this->GetControl()->IsWhileLoop())
      result+=prefix+"int "+this->GetControl()->GetLoopCounter()+" = 0;";

   result+=prefix+this->GetControl()->Generate_C_Code();
   }

if(this->IsProgram())
   result+=prefix+"//main";
result+=prefix+"{";
if(this->IsControl() && this->GetControl()->IsWhileLoop())
   result+=prefix+indent+this->GetControl()->GetLoopCounter()+"++;";

for(vector<Statement*>::iterator it=this->GetSurroundedStatements()->begin(); it != this->GetSurroundedStatements()->end(); ++it)
   result+=(*it)->Regenerate_C_Code(prefix+indent);

result+=prefix+"}";

if(this->IsControl() && this->GetControl()->IsIfElse())
   {
   result+=prefix+"else";
   result+=prefix+indent+"{";
   for(vector<Statement*>::iterator it=this->GetElseSurroundedStatements()->begin(); it != this->GetElseSurroundedStatements()->end(); ++it)
      result+=(*it)->Regenerate_C_Code(prefix+indent);
   result+=prefix+indent+"}";
   }

return result;
}

////////////////////////////////////////////////
////////////////////////////////////////////////
void
Statement::SingleCounterId(void){
if(IsProgram()){
   vector<string>   __counters;
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->SingleCounterId(&__counters);
   return;
   }
cout<<"\nStatement::SingleCounterId, fatal error (Given statement is not a function)\n";
exit(EXIT_FAILURE);
}

////////////////////////////////////////////////
////////////////////////////////////////////////

void
Statement::SingleCounterId(vector<string>* counters){
if(IsAssignment())
   return;
if(IsControl() && GetControl()->IsLoop()){
   if(find(counters->begin(), counters->end(),GetControl()->GetCounter()) != counters->end()){
      string new_name=NewCounterName(GetControl()->GetCounter(), counters);
      map<string,string>   __mapping;
      __mapping[GetControl()->GetCounter()]=new_name;
      GetControl()->Substitute(&__mapping);
      for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
         (*it)->Substitute(&__mapping);

      counters->push_back(new_name);
      }
   counters->push_back(GetControl()->GetCounter());

   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->SingleCounterId(counters);
   return;   
   }
if(IsControl() && (GetControl()->IsIf() || GetControl()->IsIfElse())){

   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->SingleCounterId(counters);
   if(GetControl()->IsIfElse())
      for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin(); it != GetElseSurroundedStatements()->end(); ++it)
         (*it)->SingleCounterId(counters);
   return;
   }
cout<<"\nStatement::SingleCounterId, Fatal error (inappropriate case)\n";
exit(EXIT_FAILURE);
}
////////////////////////////////////////////////
////////////////////////////////////////////////



vector<ElementaryDependence*>
Statement::GenerateOptimalScheduleForFlowDependenciesComputing(string array, FADA_Index* __i,int stmt, int deep){
}

vector<ElementaryDependence*>
Statement::GenerateDeepestOptimalScheduleForFlowDependenciesComputing(string array,FADA_Index* __i, int read_stmt, int deep){

Statement*   __stmt=this;

Statement*   __ptr=__stmt;

Statement*   __ptr2;
vector<ElementaryDependence*>   result;

while((__ptr->IsControl() && !__ptr->GetControl()->IsLoop()) || (!__ptr->GetParent()->IsProgram()) ){
   __ptr2= __ptr->GetParent();
   if(!__ptr2->IsControl()){
      cout<<"\nStatement::GenerateOptimalScheduleForFlowDependenceComputing, fatal error (inappropriate case)\n";
      exit(EXIT_FAILURE);
      }

   if(__ptr2->GetControl()->IsIfElse()){
      vector<Statement*>::reverse_iterator position=
            find(
               __ptr2->GetSurroundedStatements()->rbegin(),
               __ptr2->GetSurroundedStatements()->rend(),
               __ptr);

         if(position != __ptr2->GetSurroundedStatements()->rend()){
            for(vector<Statement*>::reverse_iterator 
                  it= __ptr2->GetSurroundedStatements()->rbegin(); it != position; ++it){

               vector<ElementaryDependence*> local=(*it)->GenerateReversedConfigs(array, __i,read_stmt,deep);
               for(vector<ElementaryDependence*>::iterator itr=local.begin(); itr != local.end(); ++itr)
                  result.push_back(*itr);
               }

            }
         else{
            }
      }
   
   

   if(__ptr->GetControl()->IsLoop())
      --deep;
   __ptr=__ptr2;
   }


if(deep  != 0 ){
   cout<<"\nStatement::GenerateOptimalScheduleForFlowDependenceComputing, fatal error( inappropriate value for 'deep')\n";
   exit(EXIT_FAILURE);
   }

return result;

}


//*********************************************************************
//*********************************************************************
vector<ElementaryDependence*>
Statement::GenerateReversedConfigs(string array,FADA_Index*__i, int read_stmt, int deep){
vector<ElementaryDependence*> result;
if(IsAssignment()){
   if(GetAssignment()->GetLHSArray() == array ){
      ElementaryDependence*   dep=new ElementaryDependence(read_stmt, GetID(), deep,__i);
      result.insert(result.begin(),dep);
      }
   return result;
   }
if(IsControl()){
   for(vector<Statement*>::reverse_iterator it=GetSurroundedStatements()->rbegin(); it != GetSurroundedStatements()->rend(); ++it){
      vector<ElementaryDependence*> local=(*it)->GenerateReversedConfigs(array, __i,read_stmt,deep);
      for(vector<ElementaryDependence*>::iterator itr=local.begin(); itr != local.end(); ++itr)
         result.push_back(*itr);
      }

   if(GetControl()->IsIfElse()){
      for(vector<Statement*>::reverse_iterator it=GetElseSurroundedStatements()->rbegin(); it != GetElseSurroundedStatements()->rend(); ++it){

         vector<ElementaryDependence*> local=(*it)->GenerateReversedConfigs(array, __i,read_stmt,deep);
         for(vector<ElementaryDependence*>::iterator itr=local.begin(); itr != local.end(); ++itr)
            result.push_back(*itr);
         }
      }
   return result;
   }
if(IsProgram()){
   for(vector<Statement*>::reverse_iterator it=GetSurroundedStatements()->rbegin(); it != GetSurroundedStatements()->rend(); ++it){
      vector<ElementaryDependence*> local=(*it)->GenerateReversedConfigs(array,__i, read_stmt,deep);
      for(vector<ElementaryDependence*>::iterator itr=local.begin(); itr != local.end(); ++itr)
         result.push_back(*itr);
      }
   return result;
   }

cout<<"\nStatement::GenerateReversedConfigs, fatal error (unhandled case)\n";
exit(EXIT_FAILURE);
}

//*********************************************************************
void
Statement::Substitute(map<string,string>* __mapping){
if(IsAssignment()){
   GetAssignment()->Rename(__mapping);
   return;
   }
if(IsControl()){
   GetControl()->Substitute(__mapping);
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->Substitute(__mapping);
   if(GetControl()->IsIfElse())
      for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin(); it != GetElseSurroundedStatements()->end(); ++it)
         (*it)->Substitute(__mapping);
   return;
   }
if(IsProgram()){
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->Substitute(__mapping);
   return;
   }
cout<<"\nStatement::Substitute, fatal error (inappropriate case)\n";
exit(EXIT_FAILURE);
}
////////////////////////////////////////////////
////////////////////////////////////////////////

string
Statement::Generate_C_Test_Code(string prefix)
{
string   indent="\t";

if(this->IsAssignment())
   {
   string result= this->GetAssignment()->Generate_C_Test_Code(prefix);
   result+=prefix+this->GetAssignment()->Generate_C_Code()+";";
   return result;
   }

string result;

if(this->IsControl())
   {
   if(this->GetControl()->IsWhileLoop())
      result+=prefix+"int "+this->GetControl()->GetLoopCounter()+" = -1;";

   result+=prefix+this->GetControl()->Generate_C_Code();
   }

if(this->IsProgram())
   result+=prefix+"//main";
result+=prefix+"{";
if(this->IsControl() && this->GetControl()->IsLoop())
   result+=prefix+indent+"int "+ FADA_Prefix+this->GetControl()->GetLoopCounter()+"="+this->GetControl()->GetLoopCounter()+";";
if(this->IsControl() && this->GetControl()->IsWhileLoop())
   result+=prefix+indent+this->GetControl()->GetLoopCounter()+"++;";

for(vector<Statement*>::iterator it=this->GetSurroundedStatements()->begin(); it != this->GetSurroundedStatements()->end(); ++it)
   result+=(*it)->Generate_C_Test_Code(prefix+indent);

result+=prefix+"}";

if(this->IsControl() && this->GetControl()->IsIfElse())
   {
   result+=prefix+"else";
   result+=prefix+indent+"{";
   for(vector<Statement*>::iterator it=this->GetElseSurroundedStatements()->begin(); it != this->GetElseSurroundedStatements()->end(); ++it)
      result+=(*it)->Regenerate_C_Code(prefix+indent);
   result+=prefix+indent+"}";
   }

return result;
}

////////////////////////////////////////////////
////////////////////////////////////////////////

void
Statement::NormalizeConditions(vector<References*>* ref,vector<string>* param){
if(IsAssignment())
   return;
if(IsProgram()){
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->NormalizeConditions(ref,param);
   return;
   }
if(IsControl() && (GetControl()->IsWhileLoop() || GetControl()->IsIf() || GetControl()->IsIfElse() || GetControl()->IsForLoop() )){
   vector<string> loopcounters=*ref->operator[](GetID())->GetCounters();
   if(GetControl()->IsLoop())
      loopcounters.push_back(GetControl()->GetLoopCounter());

   GetControl()->NormalizeConditions(GetID(),&loopcounters,param);
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->NormalizeConditions(ref,param);

   if(GetControl()->IsIfElse())
      for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin(); it != GetElseSurroundedStatements()->end(); ++it)
         (*it)->NormalizeConditions(ref,param);
   return;
   }
cout<<"\nStatement::NormalizeConditions, FATAL ERROR (inappropriate case)\n";
exit(EXIT_FAILURE);
}

////////////////////////////////////////////////
////////////////////////////////////////////////

/*void
Statement::ComputeSources(Program* pgm,vector<string>* param,bool sa,bool alphas){
GetReferences()->ComputeDefinitions(param,pgm,sa,alphas);
}*/
////////////////////////////////////////////////
////////////////////////////////////////////////

bool
Statement::IsSequentiallyBefore(int id1, int id2){
if(id1 >= id2)
   return false;
if(IsAssignment())
   return false;
if(IsControl() && id1 == GetID())
   return true;
if(IsControl() || IsProgram()){
   if(IsControl() && GetControl()->IsIfElse()){
      if(IsInThenPart(id1) && IsInElsePart(id2)){
//          cout<<"\n Statement("<<id1<<") Is In Then Part, Where Statement("<<id2<<") in Else Part\n";
         return false;
         }
      if(IsInThenPart(id1)){
         bool found=false;
         for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin();it!= GetSurroundedStatements()->end(); ++it){
            if(!found){
               (*it)->IsEnclosed(id1);
               if((*it)->IsEnclosed(id2))
                  return (*it)->IsSequentiallyBefore(id1,id2);
   
               found=true;
               }
            else{
               if((*it)->IsEnclosed(id2))
                  return true;
               }
            }
         cout<<"\nStatement::IsSequentiallyBefore ... fatal error (inappropriate case)[1]]\n";
         exit(EXIT_FAILURE);
         }
      if(IsInElsePart(id1)){
         bool found=false;
         for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin();it!= GetElseSurroundedStatements()->end(); ++it){
            if(!found){
               (*it)->IsEnclosed(id1);
               if((*it)->IsEnclosed(id2))
                  return (*it)->IsSequentiallyBefore(id1,id2);
   
               found=true;
               }
            else{
               if((*it)->IsEnclosed(id2))
                  return true;
               }
            }
         cout<<"\nStatement::IsSequentiallyBefore ... fatal error (inappropriate case)\n[2]";
         exit(EXIT_FAILURE);
         }
      cout<<"\nStatement::IsSequentiallyBefore ... fatal error (inappropriate case)[3]\n";
      exit(EXIT_FAILURE);
      }
   else{
      bool found=false;
      for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it){
         if(!found){
            if((*it)->IsEnclosed(id1)){
               if((*it)->IsEnclosed(id2))
                  return (*it)->IsSequentiallyBefore(id1,id2);

               found=true;
               }
            }
         else{
            if((*it)->IsEnclosed(id2))
               return true;
            }
         }
      cout<<"\nStatement::IsSequentiallyBefore ... fatal error (inappropriate case)[4]\n";
      exit(EXIT_FAILURE);
      }
   }

}
////////////////////////////////////////////////
////////////////////////////////////////////////
bool
Statement::IsEnclosed(int id){
int tf,tl;
GetEnclosed(&tf,&tl);
if(IsControl() && GetControl()->IsIfElse()){
   int ef,el;
   GetElseEnclosed(&ef,&el);
   return id >=tf && id <= el;
   }
return id>=tf && id <=tl;
}
////////////////////////////////////////////////
////////////////////////////////////////////////
bool
Statement::IsInThenPart(int id){
if(IsControl() && GetControl()->IsIfElse()){
   int f,l;
   GetEnclosed(&f,&l);
   return id >= f && id <= l;
   }
cout<<"\nStatement::IsInThenPart .... fatal error (inaproriate case)\n";
exit(EXIT_FAILURE);
}

bool
Statement::IsInElsePart(int id){
if(IsControl() && GetControl()->IsIfElse()){
   int f,l;
   GetElseEnclosed(&f,&l);
   return id >= f && id <= l;
   }
cout<<"\nStatement::IsInElsePart .... fatal error (inaproriate case)\n";
exit(EXIT_FAILURE);

}

////////////////////////////////////////////////
string
Statement::PrettyPrint(void){
if(IsAssignment())
   return Regenerate_C_Code("");
if(IsControl())
   return GetControl()->PrettyPrint_str();
return "";
}
////////////////////////////////////////////////
void
Statement::BuildDomains(Condition* __c){
GetReferences()->SetDomain(__c);
if(IsAssignment())
   return;
Condition* new_domain=new Condition(__c, FADA_AND, GetControl()->GetRegularDomain());
for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
   (*it)->BuildDomains(new_domain);
if(IsControl() && GetControl()->IsIfElse()){
   Condition* new_domain=new Condition(__c, FADA_AND, GetControl()->GetRegularDomain()->Negate());
   for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin(); it != GetElseSurroundedStatements()->end(); ++it)
      (*it)->BuildDomains(new_domain);
   return;
   }
return;
}
////////////////////////////////////////////////
void 
Statement::Serialize(vector<Assignment*>* __assign, vector<Control*>* __control){
if(IsAssignment()){
   __assign->push_back(GetAssignment());
   return;
   }
if(IsControl()){
   __control->push_back(GetControl());
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); it ++)
      (*it)->Serialize(__assign,__control);
   if(GetControl()->IsIfElse())
      for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin(); it!= GetElseSurroundedStatements()->end(); it++)
         (*it)->Serialize(__assign,__control);
   return;
   }
if(IsProgram()){
__control->push_back(GetControl());
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); it ++)
      (*it)->Serialize(__assign,__control);
   return;
   }
cout<<"\nStatement::Serialize .... fatal error (inappropriate case)\n";
exit(EXIT_FAILURE);
}
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
void
Statement::ReferencedScalars(set<string>* __written, set<string>* __read){
if(IsAssignment()){
   GetAssignment()->ReferencedScalars(__written, __read);
   return;
   }
if(IsProgram()){
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); it++)
      (*it)->ReferencedScalars(__written,__read);
   return;
   }
if(IsControl()){
   if(GetControl()->IsLoop())
      __written->insert(GetControl()->GetLoopCounter());
   GetControl()->ReferencedScalars(__read);
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); it++)
      (*it)->ReferencedScalars(__written,__read);
   if(GetControl()->IsIfElse())
      for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin(); it != GetElseSurroundedStatements()->end(); it++)
            (*it)->ReferencedScalars(__written,__read);
   return;
   }
cout<<"\nStatement::GetReferencedScalars..... fadal error (inapropirate case)\n";
exit(EXIT_FAILURE);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void
Statement::CollectIrregularLoopsProperties(LDemonstrator* __ppts,vector<string>enclosing_counters){
if(IsAssignment())
   return;
if(IsProgram()){
   vector<string> empty;
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->CollectIrregularLoopsProperties(__ppts,empty);
   return;
   }
if(IsControl() && GetControl()->IsWhileLoop()){
   if(GetControl()->GetCondition()->IsLeaf())
      __ppts->PUSHBACK(LoopProperty(&enclosing_counters,GetControl()->GetCounter(),GetControl()->GetCondition()->GetInequation()));
   }

if(IsControl() && GetControl()->IsLoop()){
   enclosing_counters.push_back(GetControl()->GetCounter());
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->CollectIrregularLoopsProperties(__ppts,enclosing_counters);
   return;
   }
if(IsControl() && GetControl()->IsIf()){
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->CollectIrregularLoopsProperties(__ppts,enclosing_counters);
   return;
   }
if(IsControl() && GetControl()->IsIfElse()){
   for(vector<Statement*>::iterator it=GetSurroundedStatements()->begin(); it != GetSurroundedStatements()->end(); ++it)
      (*it)->CollectIrregularLoopsProperties(__ppts,enclosing_counters);
   for(vector<Statement*>::iterator it=GetElseSurroundedStatements()->begin(); it!= GetElseSurroundedStatements()->end(); it++)
      (*it)->CollectIrregularLoopsProperties(__ppts,enclosing_counters);
   return;
   }

if(IsAssignment())
   return;

return;
}

}
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////  END   /////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//



