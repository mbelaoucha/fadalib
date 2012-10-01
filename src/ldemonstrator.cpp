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

#include "ldemonstrator.h"
#include <algorithm>   //gcc 4.3.x
#include <global.h>
#include <solver.h>


namespace fada{
AttachedClauses::~AttachedClauses(){
}

void
AttachedClauses::Attach(LogicalClause* __lc){
if(IsAttached()){
   cerr<<"\nLDemonstrator::Attach ... clause is already attached\n";
   throw(INAPPROPRIATE_CASE);
   return;
   }
SetAttached(__lc);
}



AttachedClauses::AttachedClauses(LogicalClause* __fact,LogicalClause* __attached){
Initialize();
Set(__fact,__attached);
}

AttachedClauses::AttachedClauses(LogicalClause* __fact){
Initialize();
Set(__fact);
}

////////////////////////////////////////////////////////

string
AttachedClauses::Print_str(void){
if(!IsAttached())
   return GetKnowledge()->Print_str()+" <<";
if(IsAttached() && ! WereUnified())
   return GetKnowledge()->Print_str()+" ?= "+GetAttached()->Print_str();
if(IsAttached() && WereUnified())
   return GetKnowledge()->Print_str()+" == "+GetAttached()->Print_str()+" >> "+GetUnified()->Print_str();

cerr<<"\nAttachedClauses::Print_str(void) .....FATAL ERROR(inappropriate case";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
AttachedClauses::Initialize(void){
SetKnowledge(NULL);
SetAttached(NULL);
WereUnified(false);
SetUnifiedClauses(NULL);
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
AttachedClauses::Set(LogicalClause* __fact,LogicalClause* __attached){
SetKnowledge(__fact);
SetAttached(__attached);
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
AttachedClauses::Set(LogicalClause* __fact){
SetKnowledge(__fact);
SetAttached(NULL);
}

////////////////////////////////////////////////////////
void
AttachedClauses::Unify(vector<string>* __param){
if(GetKnowledge() == NULL || GetAttached() == NULL){
   cerr<<"\nAttachedClauses::Unify ... FATAL ERROR (bad initialization)\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }
if((GetKnowledge()->GetNonAffineInequations()->size() > 0 ) && (GetAttached()->GetNonAffineInequations()->size() > 0)){
   SetUnifiedClauses(GetKnowledge()->Unify(GetAttached(),__param));
   WereUnified(GetUnified() != NULL);
/*   cout<<"\nAttachedClauses::Unify ... F\n";
   if(WereUnified())
      Print();*/
   return;
   }
WereUnified(false);
return;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
ContextQuast*
AttachedClauses::Traduce2ContextQuast(vector<vector<Inequation*> >*env, vector<string>* __param){
if(IsAttached()){
   cerr<<"\nAttachedClauses::Traduce2ContextQuast... warning(inappropriate case)\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }
return GetKnowledge()->TraduceToQuast(env, __param);
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
string
AttachedClauses::Print_Tex_str(void){
if(!IsAttached())
   return GetKnowledge()->Print_Tex_str();
if(IsAttached() && ! WereUnified())
   return GetKnowledge()->Print_Tex_str()+" \\stackrel{?}{=} "+GetAttached()->Print_Tex_str();
if(IsAttached() && WereUnified())
   return GetKnowledge()->Print_Tex_str()+" = "+GetAttached()->Print_Tex_str()+" \\equiv " +GetUnified()->Print_Tex_str();

cerr<<"\nAttachedClauses::Print_Tex_str... warning(inappropriate case)\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);

}
////////////////////////////////////////////////////////
string
AttachedClauses::Full_Tex_Print_str(vector<string>* param){
if(!IsAttached())
   return GetKnowledge()->Full_Tex_Print_str(param);

if(IsAttached() && ! WereUnified())
   return GetKnowledge()->Full_Tex_Print_str(param)+" \\stackrel{?}{=} "+GetAttached()->Full_Tex_Print_str(param);

if(IsAttached() && WereUnified())
   return GetKnowledge()->Full_Tex_Print_str(param)+" = "+GetAttached()->Full_Tex_Print_str(param)+" \\equiv " +GetUnified()->Full_Tex_Print_str(param);

cerr<<"\nAttachedClauses::Print_Tex_str... warning(inappropriate case)\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);

}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
bool
AttachedClauses::IsAccurate(vector<Inequation*>* precedence, vector<string>* param){
vector<Inequation*> all;
vector<string> all_var;
vector<string> empty;
all=MergeInequations(precedence,GetKnowledge()->GetAffineInequations());
all_var=MergeVariables(param,GetKnowledge()->GetVariables());
// cout<<"\nAttachedClauses::IsAccurate....\n"<<PrintInequations(&all);
Solver s(-1,-1,all_var,empty,&all);
all.clear();
Quast* max=s.Max()->EliminateRedondantLeaves(true);
bool res=false;

if(max->IsLeaf() && !max->IsEmpty())
   res=true;
// cout<<" .........."<<res<<"\n";
return res;
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

LDemonstrator::LDemonstrator()
{
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

LDemonstrator::~LDemonstrator()
{
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
AttachedClauses*
LDemonstrator::GetHeader(void){
if(GetStack()->size() > 0){
   AttachedClauses* result;
   result=*GetStack()->begin();
   return result;
   }
return NULL;
}
////////////////////////////////////////////////////////
AttachedClauses*
LDemonstrator::POP(void){
if(GetStack()->size() >0){
   AttachedClauses* result;
   result=*GetStack()->begin();
   GetStack()->erase(GetStack()->begin());
   return result;
   }
return NULL;
}

////////////////////////////////////////////////////////
void
LDemonstrator::PUSHBACK(LogicalClause* __clause){
AttachedClauses* new_elt= new AttachedClauses(__clause);
GetStack()->push_back(new_elt);
}
////////////////////////////////////////////////////////
void
LDemonstrator::PUSHINFRONT(LogicalClause* __clause){
AttachedClauses* new_elt= new AttachedClauses(__clause);
GetStack()->insert(GetStack()->begin(),new_elt);
}
////////////////////////////////////////////////////////
void
LDemonstrator::AttachToAll(LogicalClause* __clause){
for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it)
   (*it)->Attach(__clause);
return;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
LDemonstrator*
LDemonstrator::POPAffines(void){
LDemonstrator* result=new LDemonstrator();

int nb_repeats=GetStack()->size();

for(int i=0; i<nb_repeats;++i) {
   AttachedClauses* current=POP();
   if(current->GetKnowledge()->GetNonAffineInequations()->size() ==0){
      result->PUSHBACK(current->GetKnowledge());
      }
   else   {
      PUSHBACK(current->GetKnowledge());
      }
   }
return result;
}
////////////////////////////////////////////////////////
bool
LDemonstrator::Inference(vector<string>* __param){
// cout<<"\nLDemonstrator::Inference(vector<string>* __param){ ....\n";
// 
if(GetStack()->size() == 0)
   return this;
bool end=false;

LDemonstrator* affine=POPAffines();
// Print();
int nb_repeats=GetStack()->size();
nb_repeats*=nb_repeats;
bool some_affine_constraints=false;
if(GetStack()->size() ==0)
   end=true;

while(!end){
//    cout<<"\nLDemonstrator::Inference==========STACK\n";
//    Print();
   AttachedClauses* header=POP();
/*   cout<<"\nLDemonstrator::Inference==========HEADER\n";
   header->Print();*/
   int __nb_unified;
   if(header->GetKnowledge()->GetVariables()->size()==0){
      PUSHBACK(header->GetKnowledge());
      __nb_unified=0;
      
      }
   else{

      AttachToAll(header->GetKnowledge());
      Try2UnifyAll(__param);
      vector<LogicalClause*>* unified=new vector<LogicalClause*>();
      *unified=RecoverUnified();
      __nb_unified=unified->size();
      DetachAll();
      Inject(unified);
      
      PUSHBACK(header->GetKnowledge());
      LDemonstrator* traduced_affine=POPAffines();
   
      if(traduced_affine->GetStack()->size() != 0){
         affine->Merge(traduced_affine);
//          cout<<"\nInference ........ ohhh pas mal fin anticipée ...!\n";
//          traduced_affine->Print();
//          cout<<"\naffine ....\n";
//          affine->Print();
//          cout<<"\nInference ........ ohhh pas mal fin anticipée ...FIN\n";
         some_affine_constraints=true;
         end=true;
         }
      
      }
   
   nb_repeats--;
      
   if(nb_repeats==0) end=true;
//    cout<<"\n nb repeats = "<<nb_repeats<<"  taille pile d'inferences ="<<GetStack()->size()<<"  nb unified == "<<__nb_unified;
//    Print();

   }


// CLEAR();
Merge(affine);

// cout<<"\nLDemonstrator::Inference=================FIN DE DEMO \n";
// Print();
// cout<<"\nLDemonstrator::Inference(vector<string>* __param){ ....END\n";
// cout<<"\n================================================================\n";
return some_affine_constraints;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void
LDemonstrator::DetachAll(void){
for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it)
   (*it)->Detach();
return;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

vector<LogicalClause*>
LDemonstrator::RecoverUnified(){
vector<LogicalClause*> result;

for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it)
   if((*it)->WereUnified())
      result.push_back((*it)->GetUnified());
return result;
}


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void
LDemonstrator::Inject(vector<LogicalClause*>* __unified){
for(vector<LogicalClause*>::iterator it=__unified->begin(); it != __unified->end(); ++it)
   PUSHBACK(*it);
return;
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
LDemonstrator::Try2UnifyAll(vector<string>* __param){
for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it)
      (*it)->Unify(__param);
return;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

string
LDemonstrator::Print_Tex_str(void){
string result="";
for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end();++it)
   result+=(*it)->Print_Tex_str()+" \\\\ ";
return result;
}
////////////////////////////////////////////////////////
string
LDemonstrator::Full_Tex_Print_str(vector<string>* param){
string result="";
AttachedClauses* att;

for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end();++it)
   result+=(*it)->Full_Tex_Print_str(param)+" \\\\ ";
return result;
}
////////////////////////////////////////////////////////
string
LDemonstrator::Print_str(void){
string result;
for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it)
      result+="\n"+(*it)->Print_str();
return result+"\n";

}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
ContextQuast*
LDemonstrator::Traduce2ContextQuast(vector<vector<Inequation*> >* env, vector<string>* __param){
ContextQuast* result=new ContextQuast();
LDemonstrator* demo=RemoveDoubles();
// cout<<"\n COUCOUCOUCOUCOUCOUCOUCOUCOUCOUCOUCOUC =  "<<demo->Size()<<"\n";
for(list<AttachedClauses*>::iterator it=demo->GetStack()->begin(); it != demo->GetStack()->end(); ++it){
   ContextQuast* local=(*it)->Traduce2ContextQuast(env,__param);

   result=result->Max(local);
   result=result->Simplify(*env,__param);
   }

return result;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void
LDemonstrator::Merge(LDemonstrator* __ld){
for(list<AttachedClauses*>::iterator it=__ld->GetStack()->begin(); it != __ld->GetStack()->end(); ++it)
   PUSHBACK((*it)->GetKnowledge());
return ;
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
LDemonstrator::EliminateDoubles(void){
list<AttachedClauses*>   __stack;
for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it){
   bool found=false;
   for(list<AttachedClauses*>::iterator it2=__stack.begin(); it2 != __stack.end(); ++it2){
      if((*it)->Print_str() == (*it2)->Print_str()){
         found=true;
         break;
         }
      }
   if(!found)
      __stack.push_back(*it);

   }
   SetStack(&__stack);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

LDemonstrator*
LDemonstrator::Clone(){
LDemonstrator* result=new LDemonstrator();

for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it)
   result->PUSHBACK((*it)->GetKnowledge());
return result;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

bool
LDemonstrator::IsAccurate(vector<Inequation*>* precedence, vector<string>* param){
for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it)
   if((*it)->IsAccurate(precedence,param))
      return true;
return false;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// LDemonstrator*
// LDemonstrator::TraduceLoopProperties(vector<Inequation*>* __env, vector<Inequation*>* __valid){
// }

LDemonstrator*
LDemonstrator::RemoveDoubles(){
list<AttachedClauses*>   result;
for(list<AttachedClauses*>::iterator it=GetStack()->begin(); it != GetStack()->end(); ++it){
   bool insert=true;
   string ch=(*it)->Print_str();
   for(list<AttachedClauses*>::iterator it2=result.begin(); it2 != result.end(); ++it2)
      if(ch == (*it2)->Print_str()){
         insert=false;
         break;
         }
   if(insert)
      result.push_back(*it);
   }
LDemonstrator* res =new LDemonstrator();
res->SetStack(&result);
return res;
}

}
//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////  END   ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

