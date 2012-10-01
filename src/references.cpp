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

#include "references.h"
#include <global.h>
#include <algorithm>   //gcc 4.3.x
#include <html.h>

namespace fada{
References::References(int __id,vector<string> * __counters,Condition* __domain, map<int, DomainConstraint*>* __controls,
   vector<Read_Reference*>* __rr, vector<Written_Reference*>* __wr,Statement* __org){

Initialize();
SetId(__id);
SetCounters(__counters);
SetDomain(__domain);
SetControls(__controls);
SetRV(__rr);
SetWV(__wr);
SetOrigin(__org);
}


References::References(int __id,Condition* __domain){
Initialize();
SetId(__id);
SetDomain(__domain);
}

References::References(int __id,vector<string> * __counters,Condition* __domain,
   vector<Read_Reference*>* __rr, vector<Written_Reference*>* __wr,Statement* __org){
Initialize();
SetId(__id);
SetCounters(__counters);
SetDomain(__domain);
// SetControls(__controls);
SetRV(__rr);
SetWV(__wr);
SetOrigin(__org);
}


References::~References()
{
}

/////////////////////////////
void
References::AddReadReference(Read_Reference* rr){
GetRV()->push_back(rr);
}

void
References::AddWrittenReference(Written_Reference* wr){
GetWV()->push_back(wr);
}

void
References::AddInnerCounter(string lc){
GetCounters()->push_back(lc);
}
/////////////////////////////

void
References::Initialize(void){
SetId(-1);
SetDomain(NULL);
SetOrigin(NULL);
GetRV()->clear();
GetWV()->clear();
GetCounters()->clear();
GetControls()->clear();
}

/////////////////////////////
/////////////////////////////

void
References::Set(int __id,vector<string> * __counters,Condition* __domain, vector<Read_Reference*>* __rr, vector<Written_Reference*>* __wr,Statement* __org){
SetId(__id);
SetCounters(__counters);
SetDomain(__domain);
SetRV(__rr);
SetWV(__wr);
SetOrigin(__org);
}

/////////////////////////////
/////////////////////////////

void
References::Set(int __id,vector<string> * __counters,Condition* __domain, map<int, DomainConstraint*>* __controls,
      vector<Read_Reference*>* __rr, vector<Written_Reference*>* __wr,Statement* __org){
SetId(__id);
SetCounters(__counters);
SetDomain(__domain);
SetRV(__rr);
SetWV(__wr);
SetOrigin(__org);
SetControls(__controls);
}

/////////////////////////////
/////////////////////////////

void
References::CollectReferencedScalars(set<string>*__written,set<string>*__read){
for(vector<Written_Reference*>::iterator it=GetWV()->begin(); it != GetWV()->end(); it++)
//    if((*it)->GetIndex()->GetSize()==0)
      __written->insert((*it)->GetName());
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); it++)
//    if((*it)->GetArrayIndex()->GetSize()==0)
      __read->insert((*it)->GetArrayName());
cerr<<"\nReferences::CollectReferencedScalars....\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}

/////////////////////////////
/////////////////////////////

void
References::AvoidCountersParametersDoubles(vector<string>* parameters){
Read_Reference* s;

vector<Read_Reference*> result;
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); ++it){
   string current=(*it)->GetArrayName()+(*it)->GetArrayIndex()->Generate_C_Code();
   if((find(GetCounters()->begin(), GetCounters()->end(), current) == GetCounters()->end()) &&
     (find(parameters->begin(), parameters->end(), current) == parameters->end())) {
      bool find=false;
      for(vector<Read_Reference*>::iterator it2=result.begin(); it2 != result.end(); ++it2){
         string __curr2=(*it2)->GetArrayName()+(*it2)->GetArrayIndex()->Generate_C_Code();
         if(current == __curr2){
            find=true;
            break;
            }
         }
      if(!find)
         result.push_back(*it);
      }
   }
SetRV(&result);
return;
}

/////////////////////////////
/////////////////////////////


string
References::Print_str(int level){
ostringstream   out;

out<<"\nID      :"<<GetStmtID();
out<<"\nCounters   :"<<PrintVector(GetCounters());
out<<"\nDomain      :"<<GetDomain()->Print_str();
out<<"\nPragma      :"<<PrintInequations(GetPragmaConstraints());
out<<"\nWritten variable:";
for(vector<Written_Reference*>::iterator it=GetWV()->begin(); it != GetWV()->end(); ++it){
   out<<"   "<<(*it)->Print_str(0);
   }
out<<"\nRead variable:";
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); ++it){
   out<<"\n   "<<(*it)->Print_str(level);
   }
return out.str();
}

/////////////////////////////
/////////////////////////////
string
References::Print_str(int level,Options* options){
ostringstream   out;

out<<"\nID      :"<<GetStmtID();
out<<"\nCounters   :"<<PrintVector(GetCounters());
out<<"\nDomain      :"<<GetDomain()->Print_str();
out<<"\nPragma      :"<<PrintInequations(GetPragmaConstraints());
out<<"\nWritten variable:";
for(vector<Written_Reference*>::iterator it=GetWV()->begin(); it != GetWV()->end(); ++it){
   out<<"   "<<(*it)->Print_str(0);
   }
out<<"\nRead variable:";
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); ++it){
   out<<"\n   "<<(*it)->Print_str(level,options);
   }
return out.str();
}

/////////////////////////////
/////////////////////////////
bool
References::IsWrittenIn(string array){
for(vector<Written_Reference*>::iterator it=GetWV()->begin(); it != GetWV()->end(); ++it){
   if((*it)->GetName()==array)
      return true;
   }
return false;
}

/////////////////////////////
/////////////////////////////
void
References::GenerateOptimalWayForFlowDependenciesComputing(int id,Program* __prog){
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it !=GetRV()->end(); ++it){
   vector<ElementaryDependence*>* __configs=new vector<ElementaryDependence*>();
   *__configs=__prog->GenerateOptimalWayForFlowDependenciesComputing(id,(*it)->GetArrayName(),(*it)->GetArrayIndex());
   (*it)->SetSchedule(__configs);
   }
   
}

/////////////////////////////
/////////////////////////////

void
References::ReferencedScalars(set<string>* __written, set<string>* __read){
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end();++it)
   (*it)->ReadScalars(__read);

for(vector<Written_Reference*>::iterator it=GetWV()->begin(); it != GetWV()->end();++it)
   (*it)->WrittenScalars(__written);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void
References::ComputeDefinitions(vector<string>* __global_parameters, vector<References*>* __references, void* __textual_prec, int (*fct) (void*, int,int),LDemonstrator* __global_ppts, Options* __options){
vector<string> new_parameters=*__global_parameters;
vector<string> parameterized_read_iteration=ParametrizeReadIteration(GetCounters());
map<string,Expression*> mapping;
vector<string>::iterator it2;
for(vector<string>::iterator it=parameterized_read_iteration.begin(),it2=GetCounters()->begin(); it != parameterized_read_iteration.end(); ++it,++it2){
   new_parameters.push_back(*it);
   mapping[*it2]=new Expression(*it);

   }
vector<vector<Inequation*> > parameterized_affine_environment;
vector<vector<Inequation*> > parameterized_non_affine_environment;

for(vector<vector<Inequation*> >::iterator it=GetAffineConstraints()->begin(); it!= GetAffineConstraints()->end(); ++it){
   vector<Inequation*> local;
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      local.push_back((*iti)->Substitute(&mapping));

   parameterized_affine_environment.push_back(local);
   }

for(vector<vector<Inequation*> >::iterator it=GetNonAffineConstraints()->begin(); it!= GetNonAffineConstraints()->end(); ++it){
   vector<Inequation*> local;
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      local.push_back((*iti)->Substitute(&mapping));
   parameterized_non_affine_environment.push_back(local);
   }

for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); ++it){
   (*it)->ComputeTheDefinition( 
            &new_parameters,
            __textual_prec,
            fct,
            __global_ppts,
            __references,
            &parameterized_read_iteration,
            &mapping,
            &parameterized_affine_environment,
            &parameterized_non_affine_environment,
            __options);

	(*it)->CollectParameterOfTheMaximumValues(*GetCounters());
//    (*it)->GetDefinition()->Print();
   }
return;

}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void
References::ComputeDefinitions(vector<string>* __global_parameters, vector<References*>* __references, vector<vector<bool> >* __textual_prec, LDemonstrator* __global_ppts, Options* __options){

vector<string> new_parameters=*__global_parameters;
vector<string> parameterized_read_iteration=ParametrizeReadIteration(GetCounters());
map<string,Expression*> mapping;
vector<string>::iterator it2;
for(vector<string>::iterator it=parameterized_read_iteration.begin(),it2=GetCounters()->begin(); it != parameterized_read_iteration.end(); ++it,++it2){
   new_parameters.push_back(*it);
   mapping[*it2]=new Expression(*it);

   }
vector<vector<Inequation*> > parameterized_affine_environment;
vector<vector<Inequation*> > parameterized_non_affine_environment;

for(vector<vector<Inequation*> >::iterator it=GetAffineConstraints()->begin(); it!= GetAffineConstraints()->end(); ++it){
   vector<Inequation*> local;
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      local.push_back((*iti)->Substitute(&mapping));

   parameterized_affine_environment.push_back(local);
   }

for(vector<vector<Inequation*> >::iterator it=GetNonAffineConstraints()->begin(); it!= GetNonAffineConstraints()->end(); ++it){
   vector<Inequation*> local;
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      local.push_back((*iti)->Substitute(&mapping));
   parameterized_non_affine_environment.push_back(local);
   }

for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); ++it){
   (*it)->ComputeTheDefinition( 
            &new_parameters,
            __textual_prec,
            __global_ppts,
            __references,
            &parameterized_read_iteration,
            &mapping,
            &parameterized_affine_environment,
            &parameterized_non_affine_environment,
            __options);
   (*it)->CollectParameterOfTheMaximumValues(*GetCounters());
   }
return;

}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
string 
References::ToHtml(string work_dir,string name_prefix, string target_frame,Options* op,bool __useless){
string sub_index="";
int i=1;
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); ++it,++i){
        sub_index+="      <a href=\""+ComputeLink(Concat(name_prefix,i))+"\">"+(*it)->PrettyPrint_str()+"</a>";
        (*it)->ToHtml(work_dir,Concat(name_prefix,i),op);
        }
return AddHTMLHeader("references",target_frame,Verbatimize(sub_index));
}
///////////////////////
///////////////////////
void
References::ToHtml(string  work_dir, string name_prefix,string __target,Options* op){
string full_file_name=work_dir+name_prefix+".html";
WriteToFile(full_file_name,ToHtml(work_dir,name_prefix,__target,op,true));
}

///////////////////////
///////////////////////

void
References::RenameNewParameters(){
map<string,string> __mapping;
for(vector<string>::iterator it=GetCounters()->begin(); it != GetCounters()->end(); ++it)
   __mapping[FADA_Prefix+(*it)]=(*it);
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); ++it)
   (*it)->GetDefinition()->Substitute(&__mapping);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////

string
References::GenerateVCGNodes(GraphPrinterOptions op){
ostringstream sout;
sout<<"\n node: { title: \""<<op.internal_node_prefix<<GetStmtID()<<"\" label: \"";

if(op.print_rank)
   sout<<"\\n ID # "<<GetStmtID();
if(op.go_after_statements){
   if(op.print_label)
      if(GetOrigin()->IsAssignment())
         sout<<"\\n "<<GetOrigin()->GetAssignment()->GetLabel();
      else
         sout<<"\\nID # "<<GetStmtID();
   if(op.print_statement)
      sout<<"\\n"<<GetOrigin()->PrettyPrint();
   }
sout<<"\" }";

return sout.str();


}
//////////////////////////////////////////////////
//////////////////////////////////////////////////

string
References::GenerateGraphVizNodes(GraphPrinterOptions op){
ostringstream sout;
sout<<"\n"<<op.internal_node_prefix<<GetStmtID()<<" [label=\"";

if(op.print_rank)
   sout<<"\\n ID # "<<GetStmtID();
if(op.go_after_statements){
   if(op.print_label)
      if(GetOrigin()->IsAssignment())
         sout<<"\\nLabel = "<<GetOrigin()->GetAssignment()->GetLabel();
      else
         sout<<"\\nLabel = ID # "<<GetStmtID();
   if(op.print_statement)
      sout<<"\\n"<<GetOrigin()->PrettyPrint();
   }
sout<<"\"];\n";

return sout.str();

}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void
References::BuildInstanceWiseEdges(Graph* graph){
int i=0;
for(vector<Read_Reference*>::iterator it=this->GetRV()->begin(); it != GetRV()->end(); ++it,++i)
   {
   Read_Reference*   ref=*it;
   Quast *q=ref->GetDefinition();
   FlatQuast fq;
   q->Flatten(&fq);
   fq.GenerateInstanceWizeEdges(GetStmtID(),ref->GetArrayName(),ref->GetArrayIndex(),graph);
   }

}



string
References::GenerateGraphVizNodes(bool rank, bool label, bool assign){
ostringstream sout;

sout<<"\nS"<<GetStmtID()<<" [label=\"";

if(rank)
   sout<<"\\nRank = "<<GetStmtID();
if(label)
   if(GetOrigin()->IsAssignment())
      sout<<"\\nLabel = "<<GetOrigin()->GetAssignment()->GetLabel();
   else
      sout<<"\\nLabel = ID_"<<GetStmtID();
if(assign)
   sout<<"\\n"<<GetOrigin()->PrettyPrint();

sout<<"\"];\n";

return sout.str();
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
string
References::GenerateGraphVizEdges(Program* pgm,bool print_condition, bool print_relation, bool rename,bool bottom){
string result;
int i=0;
for(vector<Read_Reference*>::iterator it=this->GetRV()->begin(); it != GetRV()->end(); ++it,++i)
   {
   Read_Reference*   ref=*it;
   Quast *q=ref->GetDefinition();
   Condition*   cond=new Condition(new Inequation (true));
   result+=q->Generate_GraphViz_Edges(pgm,GetStmtID(),i,cond,print_condition, print_relation, rename,bottom);
   }
return result;

}
//////////////////////////////////////////////////
//////////////////////////////////////////////////

void
References::NormalizeDomainConstraints(vector<string>* __parameters){


Condition* normalized=GetDomain()->EliminateNotOperations(true)->DNForm();

vector<Condition*>   __terms=normalized->SplitToTerms();

vector<vector<Inequation*> > affine_for_sure;
vector<vector<Inequation*> > non_affine_for_the_moment;

for(vector<Condition*>::iterator it=__terms.begin(); it!= __terms.end(); ++it){
   vector<Inequation*>   all_term_inequations=(*it)->GetTermInequations();
   vector<Inequation*>   term_affine, term_non_affine;
   SortInequations(&all_term_inequations,&term_affine,&term_non_affine,GetCounters(), __parameters);

   Condition* eliminate_neq=ToDNFTerm(&term_affine);
   eliminate_neq=eliminate_neq->TraduceNEQInequations();
   vector<Condition*>    affine_affine_terms=eliminate_neq->SplitToTerms();
   for(vector<Condition*>::iterator itt=affine_affine_terms.begin(); itt !=  affine_affine_terms.end(); ++itt)
      affine_for_sure.push_back((*itt)->GetTermInequations());

   if(term_non_affine.size() > 0)
      non_affine_for_the_moment.push_back(term_non_affine);
   }
SetAffineConstraints(&affine_for_sure);
SetNonAffineConstraints(&non_affine_for_the_moment);
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
void
References::TagIndexes(){
vector<Expression*>* instance=new vector<Expression*>();
for(vector<string>::iterator it=GetCounters()->begin(); it != GetCounters()->end(); ++it)
   instance->push_back(new Expression(*it));
for(vector<Written_Reference*>::iterator it=GetWV()->begin(); it!= GetWV()->end(); ++it)
   (*it)->TagIndex(GetStmtID(),instance);

for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it!= GetRV()->end(); ++it)
   (*it)->TagIndex(GetStmtID(),instance);
delete instance;
return;
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void
References::AddPragmaConstraint(Inequation* in){
vector<Expression*> v;
for(vector<string>::iterator it=GetCounters()->begin(); it != GetCounters()->end(); ++it)
   v.push_back(new Expression(*it));

in->Instanciate(GetStmtID(),&v);
GetPragmaConstraints()->push_back(in);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void
References::LookForParallelLoops(vector<string>* parallel_loops,vector<References*>program){
Read_Reference* rr;
for(vector<Read_Reference*>::iterator it=GetRV()->begin(); it != GetRV()->end(); ++it)
   (*it)->LookForParallelism(*GetCounters(),parallel_loops,program);

}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

}   //namespace fada


