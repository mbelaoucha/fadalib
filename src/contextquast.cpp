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

#include "contextquast.h"
#include <global.h>
#include <solver.h>

namespace fada{
///////////////////////////////////////////////////
///////////////////////////////////////////////////
ContextQuast::~ContextQuast()
{
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

void
ContextQuast::Initialize(void){
SetLeaf();
SetCondition(NULL);
SetThenPart(NULL);
SetElsePart(NULL);
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

void
ContextQuast::Set(vector<vector<Inequation*> >*   __polyhedron){
SetLeaf();
// SetEmpty(false);
SetLeafConstraints(__polyhedron);
return;
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

void
ContextQuast::Set(void){
SetLeaf();
// SetEmpty(false);
vector<vector<Inequation*> > __polyhedron;
SetLeafConstraints(&__polyhedron);
return;
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

void
ContextQuast::Set(Condition*   __cond, ContextQuast* __then, ContextQuast* __else){
SetLeaf(false);
// SetEmpty();
SetCondition(__cond);
SetThenPart(__then);
SetElsePart(__else);
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

ContextQuast::ContextQuast(void){
Initialize();
Set();
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////

ContextQuast::ContextQuast(vector<vector<Inequation*> >*   __polyhedron){
Initialize();
Set(__polyhedron);
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

ContextQuast::ContextQuast(Condition*   __cond, ContextQuast* __then, ContextQuast* __else){
Initialize();
Set(__cond,__then,__else);
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

ContextQuast*
ContextQuast::Max(ContextQuast* context_quast){
if(IsLeaf() && context_quast->IsLeaf())
   return new ContextQuast(&AND_DNF(GetPolyhedrons(),context_quast->GetPolyhedrons()));
   

if(!IsLeaf())
   return new ContextQuast(GetCondition(), GetThenPart()->Max(context_quast),GetElsePart()->Max(context_quast));
if(!context_quast->IsLeaf())
   return new ContextQuast(context_quast->GetCondition(), this->Max(context_quast->GetThenPart()), this->Max(context_quast->GetElsePart()));

cerr<<"\nContextQuast::Max .... unhandled case \n";
throw(UNHANDLED_CASE);
exit(EXIT_FAILURE);   
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
string
ContextQuast::Print_str(string prefix){
if(IsLeaf())
   return prefix+PrintInequations(/*prefix,*/GetPolyhedrons());

string result="";
result+=prefix+"IF("+GetCondition()->Print_str()+")";
result+=GetThenPart()->Print_str(prefix+"    ");
result+=prefix+"ELSE";
result+=GetElsePart()->Print_str(prefix+"    ");
return result;
   
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
ContextQuast*
ContextQuast::Simplify(vector<vector<Inequation*> > __env,vector<string>* parameters){
if(__env.size() == 0)   return new ContextQuast();
if(IsLeaf())
   return new ContextQuast(&EliminateDoubles(GetPolyhedrons()));

vector<Inequation*>   __env_then, __env_else;
__env_then.push_back(GetCondition()->GetInequation());
__env_else.push_back(GetCondition()->GetInequation()->Negate(true));
vector<vector<Inequation*> > __local_then, __local_else;
__local_then.push_back(__env_then);
__local_else.push_back(__env_else);
__local_then=AND_DNF(&__env,&__local_then);
__local_else=AND_DNF(&__env,&__local_else);

if(IsUnsatisfiable(&__local_then,parameters))
   return GetElsePart()->Simplify(__local_else,parameters);
if(IsUnsatisfiable(&__local_else,parameters))
   return GetThenPart()->Simplify(__local_then,parameters);

return new ContextQuast(GetCondition(),GetThenPart()->Simplify(__local_then,parameters), GetElsePart()->Simplify(__local_else,parameters));
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////
// ContextQuast*
// ContextQuast::Simplify(vector<Inequation*> __env, vector<string>* parameters){
// 
// if(IsLeaf())
//    return new ContextQuast(&EliminateDoubles(GetPolyhedrons()));
// 
// vector<Inequation*> env_then=__env;
// vector<Inequation*> env_else=__env;
// env_then.push_back(GetCondition()->GetInequation());
// env_else.push_back(GetCondition()->GetInequation()->Negate(true));
// 
// /*switch(GetCondition()->GetInequation()->SimplifyWith(&__environmnt,__param)){
//    case ALWAYS_TRUE: return GetThenPart()->Simplify(__environmnt,__param);
//    case ALWAYS_FALSE: return GetElsePart()->Simplify(__environmnt,__param);
//    }*/
// // switch(GetCondition()->GetInequation()->Negate(true)->SimplifyWith(&__environmnt,__param)){
// //    case ALWAYS_TRUE:return GetElsePart()->Simplify(__environmnt,__param);
// //    case ALWAYS_FALSE:return GetThenPart()->Simplify(__environmnt,__param);
// //    }
// 
// vector<Inequation*>   __env_then, __env_else;
// __env_then=__env;
// __env_else=__env;
// __env_then.push_back(GetCondition()->GetInequation());
// __env_else.push_back(GetCondition()->GetInequation()->Negate(true));
// 
// 
// if(IsUnsatisfiable(&__env_else,parameters))
//    return GetThenPart()->Simplify(__env_then,parameters);
// 
// 
// if(IsUnsatisfiable(&__env_then,parameters))
//    return GetElsePart()->Simplify(__env_else,parameters);
// 
// return new ContextQuast(GetCondition(),GetThenPart()->Simplify(env_then, parameters), GetElsePart()->Simplify(env_else,parameters));
// }

///////////////////////////////////////////////////
///////////////////////////////////////////////////

vector<vector<Inequation*> >
ContextQuast::GetPolyhedrons(vector<Inequation*> __env){
if(!IsLeaf()){
   vector<Inequation*> __then, __else;
   __then=__env;
   __else=__env;
   if(!GetCondition()->IsLeaf()){
      cerr<<"\nContextQuast::GetPolyhedrons ... WARNING(invalid data)\n";
      throw(BAD_OBJECT);
      exit(EXIT_FAILURE);
      }
   __then.push_back(GetCondition()->GetInequation());
   __else.push_back(GetCondition()->GetInequation()->Negate(true));

   return Merge(&GetThenPart()->GetPolyhedrons(__then),&GetElsePart()->GetPolyhedrons(__else));
   }
vector<vector<Inequation*> >  __local;
__local.push_back(__env);

return AND_DNF(GetPolyhedrons(),&__local);
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

ContextQuast*
ContextQuast::EliminateUnreachableNodes(vector<vector<Inequation*> >__env, vector<string>* __param){
if(IsLeaf()){
   return new ContextQuast(&SimplifyInequations(&EliminateDoubles(&TraduceNEQOperations(GetPolyhedrons())),&__env,__param));
   }

vector<Inequation*>__then;__then.push_back(GetCondition()->GetInequation());
vector<Inequation*>__else;__else.push_back(GetCondition()->GetInequation()->Negate(true));

vector<vector<Inequation*> > ___then; ___then.push_back(__then);
vector<vector<Inequation*> > ___else; ___else.push_back(__else);

vector<vector<Inequation*> >env_then;
vector<vector<Inequation*> >env_else;


env_then=AND_DNF(&__env,&___then);

env_else=AND_DNF(&__env,&___else);


if(IsUnsatisfiable(&env_then,__param))
      return GetElsePart()->EliminateUnreachableNodes(env_else,__param);
if(IsUnsatisfiable(&env_else,__param))
      return GetThenPart()->EliminateUnreachableNodes(env_then,__param);


return new ContextQuast(GetCondition(),GetThenPart()->EliminateUnreachableNodes(env_then, __param), GetElsePart()->EliminateUnreachableNodes(env_else,__param));

}

}




