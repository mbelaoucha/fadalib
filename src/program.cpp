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

// authors: 
// Christophe Alias
// Marouane Belaoucha


#include "program.h"
#include "constants.h"
#include "global.h"
#include "html.h"
#include <algorithm>



extern bool      Parse_Program(char* filename, fada::Program* adaan_prog);
namespace fada{


//////////////////////////////////////////////////////////////////////////////////////////

Program::Program()
{
global_properties=new LDemonstrator();
}

//////////////////////////////////////////////////////////////////////////////////////////


Program::~Program()
{
}


//////////////////////////////////////////////////////////////////////////////////////////

bool
Program::Parse(string file_name)
{
if(Parse_Program((char*)file_name.c_str(),this))
   {
   cout<<endl<<"syntaxic error"<<endl;
   exit(0);
   }
   
return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
string
Program::Print_str()
{
string controls;

string result;

string   syntaxtree="\n\n\n\n Abstract Syntax Tree\n"+this->GetSyntaxTree()->Print_str("\t");
return "\n Label = '"+GetLabel()+"'\n\n"+result+"\n\n"+controls+syntaxtree;

}

//////////////////////////////////////////////////////////////////////////////////////////

void
Program::Print()
{
cout<<Print_str();
}
//////////////////////////////////////////////////////////////////////////////////////////
bool
Program::IsSequentiallyBefore(int id1, int id2, vector<string>* common){
if( id1 >= id2)
   return false;


Statement*   deepest_common_control;
if(common->size() == 0)
   deepest_common_control=GetSyntaxTree();
else{
//    vector<string>::iterator it=common->end();
//    --it;
//    deepest_common_control=GetLoop(*it);
   deepest_common_control=GetSyntaxTree();
   }
return deepest_common_control->IsSequentiallyBefore(id1,id2);
}


//////////////////////////////////////////////////////////////////////////////////////////
void
Program::LookForParameters(bool extended){
set<string> read, written;

vector<set<string> > read_scalars;
vector<set<string> > written_scalars;
vector<set<string> > written_in_loops_scalars;
set<string> curr_read, curr_written;

/*for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it != GetNormalizedStmts()->end();){
   
   (*it)->Print(0);
   if((*it)->GetCounters()->size() > 0){
      read_scalars.push_back(curr_read);
      written_scalars.push_back(curr_written);
      curr_read.clear();
      curr_written.clear();

      while(it!=GetNormalizedStmts()->end() && (*it)->GetCounters()->size() >0){
         (*it)->ReferencedScalars(&curr_written,&curr_read);
         ++it;
         }
      
      written_in_loops_scalars.push_back(curr_written);
      read_scalars.push_back(curr_read);
      written_scalars.push_back(curr_written);
      curr_read.clear();
      curr_written.clear();
      }
   else
         (*it)->ReferencedScalars(&curr_written,&curr_read);
   if(it != GetNormalizedStmts()->end()) ++it;
   }*/

for(vector<Statement*>::iterator it=GetSyntaxTree()->GetSurroundedStatements()->begin(); it != GetSyntaxTree()->GetSurroundedStatements()->end(); ++it){
   if((*it)->IsControl() && (*it)->GetControl()->IsLoop()){
      read_scalars.push_back(curr_read);
      written_scalars.push_back(curr_written);
      curr_read.clear();
      curr_written.clear();
      }
   (*it)->ReferencedScalars(&curr_written,&curr_read);
   if((*it)->IsControl() && (*it)->GetControl()->IsLoop()){
      written_in_loops_scalars.push_back(curr_written);
      read_scalars.push_back(curr_read);
      written_scalars.push_back(curr_written);
      curr_read.clear();
      curr_written.clear();
      }
   }


vector<set<string> >::iterator itr=read_scalars.begin();
vector<set<string> >::iterator itw=written_scalars.begin();

set<string> extra_param;
while(itr != read_scalars.end()){
   for(set<string>::iterator it=(*itr).begin(); it != (*itr).end(); ++it)
      read.insert(*it);
   for(set<string>::iterator it=(*itw).begin(); it != (*itw).end(); ++it)
      written.insert(*it);
   for(set<string>::iterator it=written.begin(); it != written.end(); ++it){
      vector<set<string> >::iterator itw2=itw;
      ++itw2;
      while(   (itw2 != written_scalars.end()) && 
         find((*itw2).begin(),(*itw2).end(),*it)!= (*itw2).end()) {
         ++itw2;
         }
      if(itw2 == written_scalars.end())
         extra_param.insert(*it);
      }
   ++itr;
   ++itw;
   }


for(set<string>::iterator it=read.begin(); it != read.end(); ++it){
   if(find(written.begin(), written.end(),*it)==written.end())
      extra_param.insert(*it);
   }

set<string> final_param;
for(vector<set<string> >::iterator it=written_in_loops_scalars.begin(); it != written_in_loops_scalars.end();++it){
   for(set<string>::iterator it2=(*it).begin(); it2!= (*it).end();++it2)
      {
      set<string>::iterator found=find(extra_param.begin(), extra_param.end(),*it2);
      if(found != extra_param.end())
         extra_param.erase(found);
      }
   }
written_in_loops_scalars.push_back(curr_written);
vector<string> symbolic_constants;
symbolic_constants.resize(extra_param.size());
int i=0;
for(set<string>::iterator it=extra_param.begin(); it != extra_param.end(); ++it,++i){
   symbolic_constants[i]=*it;   
   }


SetGlobalParameters(&symbolic_constants);
SetAllParameters(GetGlobalParameters());
}
//////////////////////////////////////////////////////////////////////////////////////////

string
Program::ToHtml(Options* op){
string dir=op->io.output_directory;
string command="mkdir "+dir+"images/";
system(command.c_str());

string name_prefix=GetLabel()+"_definitions";
string   full_name=dir+name_prefix+".html";

string title="Dataflow analysis results for'"+this->GetLabel()+"'";

string index_file=dir+"index_"+GetLabel()+".html";

string frame1="PROGRAM";
string frame2="REFERENCES";
string frame3="DEFENITIONS";

ostringstream   oss;
oss<<0;
string   pgm="./"+name_prefix+".html";
string   ref="./"+name_prefix+"_"+oss.str()+".html";
oss<<"_"<<1;
string   def="./"+name_prefix+"_home.html";
string home_page=dir+name_prefix+"_home.html";
WriteToFile(home_page,GenerateHomePage());

string index_code=AddHTMLHeader(title, CreateFrames(frame1,frame2,frame3,pgm,ref,def),true);

WriteToFile(full_name,AddHTMLHeader(title, frame2, Verbatimize(GetSyntaxTree()->ToHtml(dir,name_prefix,"\n",frame3,op))));

WriteToFile(index_file,index_code);

return index_file;
}
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


string
Program::Generate_GraphViz_Graph(string file_name, bool print_rank, bool print_label, bool print_assign, bool print_condition, bool print_relation, bool rename, bool bottom,bool alphas)
{
FILE* f;

string file=file_name+".dot";
cout<<endl<<"generating a 'dot' file ("<<file<<")... ";

string graphviz=Generate_GraphViz_Graph(print_rank,print_label,print_assign,print_condition,print_relation,rename,bottom,alphas);
// cout<<"\n\n"<<graphviz;
WriteToFile(file,graphviz);

cout<<" done";

string command="dot -T"_dot_out_format_" "+file_name+".dot -o "+file_name+"."_dot_out_format_;

cout<<endl<<"executing GraphViz conversion ("<<command<<") ... ";
system(command.c_str());
return file_name+"."_dot_out_format_;
}

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
string
Program::Generate_GraphViz_Graph(bool print_rank, bool print_label, bool print_assign, bool print_condition, bool print_relation, bool rename,bool bottom,bool alphas)
{


ostringstream nodes;
if(bottom)
   nodes << endl << " "FADA_Bottom" [label = \"_|_\", shape=ellipse, color=red];";


for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it != GetNormalizedStmts()->end(); ++it)
   nodes<<(*it)->GenerateGraphVizNodes(print_rank,print_label,print_assign);




string edges ="";




for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it != GetNormalizedStmts()->end(); ++it)
   edges+=(*it)->GenerateGraphVizEdges(this,print_condition,print_relation,rename,bottom);


string format="bgcolor=azure;";
format +="\nnode [shape=ellipse, color=blue, style=filled, fontcolor=white ];";
format +="\nedge [arrowsize=2, color=gold];";

string result= "digraph G {" + format+nodes.str()+edges + "}";

return result;
}

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
string
Program::GenerateOracle(void)
{
// ostringstream result;
// ostringstream finalresult;
// cout<<endl<<"'Program::GenerateOracle' => Go";
// cout<<endl<<"allstatements size ="<<this->GetNbAssignments();
// 
// finalresult<<"\n\n\n#ifndef __TEST_DEFINITIONS_H__";
// finalresult<<"\n#define __TEST_DEFINITIONS_H__";
// 
// finalresult<<"\n\n\n\n#include \"adaan_oracle.h\"";
// finalresult<<"\n#include <vector.h>";
// finalresult<<"\n#include <iostream>";
// 
// finalresult<<"\n\n\n#define __TEST_DEBUG__";
// finalresult<<"\n\n\n\nvoid   __Test_Definitions(void)";
// finalresult<<"\n{";
// 
// string tabs="\t";
// finalresult<<"\n"<<tabs<<"__Test_Info __t_i;";
// finalresult<<"\n"<<tabs<<"vector < int > v;";
// 
// 
// vector<string >   variables;
// vector<int>   sizes;
// 
// 
// for(int i=0;i<this->GetGlobalParameters()->size();++i)
//    {
//    result<<"\n"<<tabs<<"for(int "<<GetGlobalParameters()->operator[](i)   <<"=-10;"<<GetGlobalParameters()->operator[](i)<<"<100;++"<<GetGlobalParameters()->operator[](i)<<")";
//    tabs+="\t";
//    result<<"\n"<<tabs<<"{";
//    result<<"\n"<<tabs<<"cout <<endl<<\"Going whith "<<GetGlobalParameters()->operator[](i)<<" = \"<<"<<
//    GetGlobalParameters()->operator[](i)<<" <<endl;";
//    }
// 
// 
// //for each new parameter-values, clear old acesses
// 
// this->GetAllReferencedVariables(& sizes, &variables);
// vector<string>::iterator varit=variables.begin();
// vector<int>::iterator   sizesit=sizes.begin();
// 
// while(varit != variables.end())
//    {
//    result<<endl<<tabs<<"__Touch_Traces_Stack  DaAn_Traces_"<<*varit;
//    for(int i=0; i<*sizesit; ++i)
//       result<<"[1000]";
//    result<<";";
// 
//    result<<endl<<tabs<<"int "<<*varit;
//    for(int i=0; i<*sizesit; ++i)
//       result<<"[1000]";
//    result<<";";
// 
//    ++varit;
//    ++sizesit;
//    }
// 
// 
// 
// int Deep=0;
// int MaxDeep=0;
// for(int i=0;i<this->GetNbAssignments();++i)
//    {
//    cout<<endl<<"stmt = "<<i;
//    if(this->allstatements[i]->IsWhile())
//       result<<endl<<tabs<<"int "<<this->allstatements[i]->V_ID<<" = -1 ;";
// 
//    result<<this->allstatements[i]->print_str(tabs,&tabs);
// 
//    if(this->allstatements[i]->IsAssign())
//       {
//       result<<this->allstatements[i]->assignement->GenerateCCode(this,tabs);
//       if(Deep >0 )
//          result<<"\n"<<tabs<<"Alpha["<<(this->allstatements[i]->assignement->stmt_rank)<<"]["<<Deep<<"] = "<< get_counter_string(this->allstatements[i]->assignement->surrounding_counters[Deep-1])<<";";
//       else
//          result<<"\n"<<tabs<<"Alpha["<<(this->allstatements[i]->assignement->stmt_rank)<<"]["<<Deep<<"] = "<< 0 <<";";
//       }
// 
//    if(this->allstatements[i]->IsBlocBegin() && this->allstatements[i-1]->IsLoop())
//       {
//       Deep++;
//       if(this->allstatements[i-1]->IsWhile())
//          result<<endl<<tabs<<this->allstatements[i-1]->V_ID<<"++;";
//       result<<endl<<tabs<<"int "<<this->allstatements[i-1]->V_ID<<"_p ="<<this->allstatements[i-1]->V_ID<<";";
//       result<<endl<<tabs<<"cout<<endl<<\" current value of '"<<this->allstatements[i-1]->V_ID<<"' is \" <<"<<this->allstatements[i-1]->V_ID<<";";
//       }
//    if(this->allstatements[i]->IsBlocEnd())
//       Deep--;
//    if(Deep >MaxDeep)
//       MaxDeep=Deep;
//    }
// 
// for(int i=0;i<this->nb_parameters;++i)
//    {
//    result<<"\n"+tabs+"}";
//    string ntabs(tabs,0, tabs.length()-1);
//    tabs=ntabs;
//    }
// result<<endl<<"}";
// 
// finalresult<<"\n"<<tabs<<"int Alpha["<<this->statement.size()<<"]["<<MaxDeep<<"];";
// finalresult<<result.str();
// finalresult<<endl<<"/*int main(void)";
// finalresult<<endl<<"{";
// finalresult<<endl<<"__Test_Definition()";
// finalresult<<endl<<"return 0;";
// finalresult<<endl<<"}*/";
// finalresult<<endl<<"#endif";
// 
// 
// cout<<endl<<"'Program::GenerateCTestCode' => Done";
// return finalresult.str();

}
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void
Program::GenerateOracle(string file_name){
//TODO: chercher le code sur la machine pissaro
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*void
Program::IterativeAnalysis(void){
//TODO: chercher le code sur la machine pissaro

}*/
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*void
Program::GetAllReferencedVariables(vector<int>*   dim, vector<string>*   var)
{

var->clear();
dim->clear();

for(int i = 0; i < this->GetNbAssignments(); i++)
   {
   Assignment*   assign=this->GetAssignment(i);
   if(find(var->begin(),var->end(), assign->GetLHSArray())==var->end())
      {
      var->push_back(assign->GetLHSArray());
      dim->push_back(assign->GetLHSIndex()->GetSize());
      }

   for   (vector<Read_Reference*>::iterator it_rhs=assign->GetReferences()->begin();
      it_rhs !=assign->GetReferences()->end();++it_rhs)
      // for each rhs reference
      {
      Read_Reference*   __ref=*it_rhs;
      if(find(var->begin(),var->end(), __ref->GetArrayName())==var->end())
         {
         var->push_back(__ref->GetArrayName());
         dim->push_back(__ref->GetArrayIndex()->GetSize());
         }
      }
   }
}*/
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// string
// Program::Generate_C_Test_Code(void)
// {
// string indent="\n";
// string header;
// header+="#ifdef HAVE_CONFIG_H\n";
// header+="#include <config.h>\n";
// header+="#endif\n\n\n";
// 
// header+="#include <iostream>\n";
// header+="#include <cstdlib>\n";
// header+="#include \"CTestData.h\"\n\n";
// header+="\nusing namespace std;\n\n";
// 
// 
// 
// string declarations="\n\n\n//referenced variables and arrays";
// 
// vector<int>   sizes;
// vector<string>   arrays;
// 
// this->GetAllReferencedVariables(&sizes, &arrays);
// vector<int>::iterator its=sizes.begin();
// string trace_declaration="\n\n\n// define objects to trace memory-cell-accesses";
// for(vector<string>::iterator it=arrays.begin(); it != arrays.end(); ++it, ++its)
//    {
//    string dim=ADaAn_Size+*it;
//    declarations+="\nint "+*it;
//    trace_declaration+="\n"ADaAn_Tracer_Class" "ADaAn_Tracer_Prefix+*it;
//    for(int i=0; i<*its; ++i)
//       {
//       ostringstream size;
//       size<<dim<<"_"<<i;
//       declarations+="["+size.str()+"]";
//       trace_declaration+="["+size.str()+"]";
//       header+="#define "+size.str()+" 100\n";
//       }
//    declarations+=";";
//    trace_declaration+=";";
//    }
// 
// declarations+=trace_declaration;
// 
// string front_end="\n\n// test for each for each value of each parameter";
// front_end+="\n//please choose appropriate values for your function's parameters ... ";
// for(vector<string>::iterator it=this->GetGlobalParameters()->begin(); it != this->GetGlobalParameters()->end(); ++it)
//    {
//    front_end+=indent+"for(int "+*it+" = "+ADaAn_Lower_bound+(*it)+" ;"+ (*it)+" <= "+ ADaAn_Upper_bound+(*it)+"; ++"+*it+"){";
//    header+="\n#define "+(string)ADaAn_Lower_bound+*it+" -10";
//    header+="\n#define "+(string)ADaAn_Upper_bound+*it+" 100";
//    indent+="\t";
//    }
// declarations+=front_end;
// string result;
// result="\n\n"ADaAn_Tracer_Prototype"{\n"+declarations;
// 
// string back_end="";
// 
// for(vector<string>::iterator it=this->GetGlobalParameters()->begin(); it != this->GetGlobalParameters()->end(); ++it)
//    back_end+=indent+"}";
// 
// result +=this->GetSyntaxTree()->Generate_C_Test_Code(indent);
// result+=back_end+"\nreturn 0;\n}";
// return header+result;
// }

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// void
// Program::Generate_C_Test_Code(string file_name)
// {
// FILE*f;
// 
// f=fopen(file_name.c_str(),"w");
// fprintf(f,"%s",Generate_C_Test_Code().c_str());
// fclose(f);
// return;
// }

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void
Program::RenameNewParameters(void){

for(vector<References*>::iterator ita=GetNormalizedStmts()->begin(); ita != GetNormalizedStmts()->end(); ++ita)
   (*ita)->RenameNewParameters();
}
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

string
Program::Generate_C_str(void){
return this->GetSyntaxTree()->Regenerate_C_Code("\n");
}

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void
Program::Generate_c(string filename){
FILE* output=fopen(filename.c_str(),"w");
fprintf(output,"%s",Generate_C_str().c_str());
fclose(output);
}

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void
Program::Normalize(void){
vector<string>   c;c.clear();
map<int, DomainConstraint*>   __map;
Normalize(GetSyntaxTree(),new Condition(new Inequation(true)),__map,c);
}

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void
Program::Normalize(
         Statement* __stmt, 
         Condition* __domain,
         map<int,DomainConstraint*> __controls, 
         vector<string> __counters)
   {
if(__stmt->IsProgram()){
   vector<string>   c;c.clear();
   map<int, DomainConstraint*>   __map;
   for(vector<Statement*>::iterator it=__stmt->GetSurroundedStatements()->begin(); it != __stmt->GetSurroundedStatements()->end(); ++it)
      this->Normalize(*it,new Condition(new Inequation(true)),__map,c);

   vector<Statement*>::iterator      it=__stmt->GetSurroundedStatements()->end();--it;
   int f,l,useless;
   if((*it)->IsControl() && (*it)->GetControl()->IsIfElse())
      (*it)->GetElseEnclosed(&useless,&l);
   else
      (*it)->GetEnclosed(&useless,&l);

   it=__stmt->GetSurroundedStatements()->begin();
   (*it)->GetEnclosed(&f,&useless);
   __stmt->SetEnclosed(f,l);

   return;
   }

if(__stmt->IsAssignment()){

   int __id=GetNormalizedStmts()->size();
   

   vector<Read_Reference*>   vr;
   vector<Written_Reference*> vw;

   CollectReferences(__stmt->GetAssignment()->GetLHSIndex(), &vr);
   CollectReferences(__stmt->GetAssignment()->GetRHSExpression(), &vr);

   Written_Reference*   wr= 
         new Written_Reference(   __stmt->GetAssignment()->GetLHSArray(),
                     __stmt->GetAssignment()->GetLHSIndex()
                  );
   vw.push_back(wr);

   References*   new_stmt=new References (__id,&__counters,__domain,&__controls, &vr,&vw, __stmt);
   new_stmt->SetPragmaConstraints(__stmt->GetPragmaConstraints());
   
   __stmt->SetEnclosed(__id,__id);
   Connect(__stmt,new_stmt);
   GetNormalizedStmts()->push_back(new_stmt);

   return;
   }

if(__stmt->IsControl()){
    
   if(__stmt->GetControl()->IsWhileLoop()){
//       cout<<"\nProgram::Normalize, while loops are not handled yet\n";
//        exit(EXIT_FAILURE);

      vector<Read_Reference*>   vr;
      vector<Written_Reference*> vw;

      CollectReferences(__stmt->GetControl()->GetRegularDomain(), &vr);

      int __id=GetNormalizedStmts()->size();

      __counters.push_back(__stmt->GetControl()->GetCounter());
      Condition*   new_domain=new Condition(__domain, FADA_AND,__stmt->GetControl()->GetRegularDomain());

      vector<Expression*>   iteration;
      for(vector<string>::iterator   it=__counters.begin(); it != __counters.end(); ++it)
         iteration.push_back(new Expression(*it));
      
      new_domain->GetRightChild()->Instanciate(__id,&iteration);
      __controls[__id]=new DomainConstraint(new_domain->GetRightChild());


      References*   new_stmt=new References (__id,&__counters,new_domain,&__controls,&vr,&vw, __stmt);
      new_stmt->SetPragmaConstraints(__stmt->GetPragmaConstraints());
      Connect(__stmt,new_stmt);
      GetNormalizedStmts()->push_back(new_stmt);


      for(vector<Statement*>::iterator it=__stmt->GetSurroundedStatements()->begin(); it != __stmt->GetSurroundedStatements()->end(); ++it)
         this->Normalize(*it, new_domain,__controls,__counters);

      vector<Statement*>::iterator      it=__stmt->GetSurroundedStatements()->end();--it;

      int f,l,useless;
      if((*it)->IsControl() && (*it)->GetControl()->IsIfElse())
         (*it)->GetElseEnclosed(&useless,&l);
      else
         (*it)->GetEnclosed(&useless,&l);

      __stmt->SetEnclosed(__id,l);

      return;
      }


   if(__stmt->GetControl()->IsIf()){
      int __id=GetNormalizedStmts()->size();
      vector<Read_Reference*>   vr;
      vector<Written_Reference*> vw;      //no  written variable

      CollectReferences(__stmt->GetControl()->GetRegularDomain(), &vr);

      References*   new_stmt=new References (__id,&__counters,__domain,&__controls,&vr,&vw, __stmt);
      new_stmt->SetPragmaConstraints(__stmt->GetPragmaConstraints());
      Connect(__stmt,new_stmt);
      GetNormalizedStmts()->push_back(new_stmt);

      Condition*   new_domain=new Condition(__domain, FADA_AND,__stmt->GetControl()->GetRegularDomain());
      vector<Expression*>   iteration;

      for(vector<string>::iterator   it=__counters.begin(); it != __counters.end(); ++it)
         iteration.push_back(new Expression(*it));

      new_domain->GetRightChild()->Instanciate(__id,&iteration);

      __controls[__id]=new DomainConstraint(new_domain->GetRightChild());   //positive

      for(vector<Statement*>::iterator it=__stmt->GetSurroundedStatements()->begin(); it != __stmt->GetSurroundedStatements()->end(); ++it)
         this->Normalize(*it, new_domain,__controls,__counters);
      
      
   
      vector<Statement*>::iterator      it=__stmt->GetSurroundedStatements()->end();--it;
      int f,l,useless;
      if((*it)->IsControl() && (*it)->GetControl()->IsIfElse())
         (*it)->GetElseEnclosed(&useless,&l);
      else
         (*it)->GetEnclosed(&useless,&l);

      __stmt->SetEnclosed(__id,l);
      return;
      }

   if(__stmt->GetControl()->IsIfElse()){

      int __id=GetNormalizedStmts()->size();

      vector<Read_Reference*>   vr;
      vector<Written_Reference*> vw;      // no written reference
      CollectReferences(__stmt->GetControl()->GetRegularDomain(), &vr);


      References*   new_stmt=new References (__id,&__counters,__domain,&__controls,&vr,&vw, __stmt);
      new_stmt->SetPragmaConstraints(__stmt->GetPragmaConstraints());
      Connect(__stmt,new_stmt);
      GetNormalizedStmts()->push_back(new_stmt);


      Condition*   new_domain=new Condition(__domain, FADA_AND,__stmt->GetControl()->GetRegularDomain());

      vector<Expression*>   iteration;
      for(vector<string>::iterator   it=__counters.begin(); it != __counters.end(); ++it)
         iteration.push_back(new Expression(*it));

      new_domain->GetRightChild()->Instanciate(__id,&iteration);

      map<int, DomainConstraint*>   then=__controls;
      then[__id] = new DomainConstraint(new_domain->GetRightChild());   //positive
      
      for(vector<Statement*>::iterator it=__stmt->GetSurroundedStatements()->begin(); it != __stmt->GetSurroundedStatements()->end(); ++it)
         this->Normalize(*it, new_domain,then,__counters);

      __controls[__id] = new DomainConstraint(new_domain->GetRightChild(),false);   //positive

      Condition*   new_else_domain=new Condition(__domain, FADA_AND,__stmt->GetControl()->GetRegularDomain()->FastNegation());

      for(vector<Statement*>::iterator it=__stmt->GetElseSurroundedStatements()->begin(); it != __stmt->GetElseSurroundedStatements()->end(); ++it)
         this->Normalize(*it, new_else_domain,__controls,__counters);

      
      vector<Statement*>::iterator it=__stmt->GetElseSurroundedStatements()->end();--it;

      int f,l,useless;
      if((*it)->IsControl() && (*it)->GetControl()->IsIfElse())
         (*it)->GetElseEnclosed(&useless,&l);
      else
         (*it)->GetEnclosed(&useless,&l);

      it=__stmt->GetElseSurroundedStatements()->begin();

      (*it)->GetEnclosed(&f,&useless);
      __stmt->SetElseEnclosed(f,l);

//       it=__stmt->GetSurroundedStatements()->begin();
//       (*it)->GetEnclosed(&f,&useless);
      it=__stmt->GetSurroundedStatements()->end();--it;

      if((*it)->IsControl() && (*it)->GetControl()->IsIfElse())
         (*it)->GetElseEnclosed(&useless,&l);
      else
         (*it)->GetEnclosed(&useless,&l);

      __stmt->SetEnclosed(__id,l);

      return;
      }

   if(__stmt->GetControl()->IsForLoop()){
//       cout<<"\nProgram::Normalize, it is a for loop\n";
//        exit(EXIT_FAILURE);

      vector<Read_Reference*>   vr;
      vector<Written_Reference*> vw;

      CollectReferences(__stmt->GetControl()->GetForLowerBound(),&vr);
      CollectReferences(__stmt->GetControl()->GetForUpperBound(),&vr);
      int __id=GetNormalizedStmts()->size();

      References*   new_stmt=new References (__id,&__counters,__domain,&__controls,&vr,&vw, __stmt);
      new_stmt->SetPragmaConstraints(__stmt->GetPragmaConstraints());
      Connect(__stmt,new_stmt);
      GetNormalizedStmts()->push_back(new_stmt);

      Condition*   new_domain=new Condition(__domain, FADA_AND,__stmt->GetControl()->GetRegularDomain());
      __counters.push_back(__stmt->GetControl()->GetCounter());

      vector<Expression*>   iteration;
      for(vector<string>::iterator   it=__counters.begin(); it != __counters.end(); ++it)
         iteration.push_back(new Expression(*it));
      
      new_domain->GetRightChild()->Instanciate(__id,&iteration);
      __controls[__id]=new DomainConstraint(new_domain->GetRightChild());

      for(vector<Statement*>::iterator it=__stmt->GetSurroundedStatements()->begin(); it != __stmt->GetSurroundedStatements()->end(); ++it)
         this->Normalize(*it, new_domain,__controls,__counters);

       
      vector<Statement*>::iterator      it=__stmt->GetSurroundedStatements()->end();--it;
      int f,l,useless;
      if((*it)->IsControl() && (*it)->GetControl()->IsIfElse())
         (*it)->GetElseEnclosed(&useless,&l);
      else
         (*it)->GetEnclosed(&useless,&l);

      __stmt->SetEnclosed(__id,l);


      return;
      }

    cout<<"\nProgram::Normalize, inappropriate case\n";
   exit(EXIT_FAILURE);

   }
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void
Program::PrintNormalized(int info_to_print){
cout<<"\n/////////////////////////////////////////////////";
cout<<"\nSymbolic constants are  = "<<PrintVector(GetGlobalParameters());
for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it !=GetNormalizedStmts()->end(); ++it){
   cout<<"\n********************************************";
   cout<<(*it)->Print_str(info_to_print,GetOptions());
   }
cout<<"\n/////////////////////////////////////////////////";
}

///////////////////////////////////////////////////////////////////////////
void 
Program::PrintSources(string file_name){
string fn=file_name+".log";
string result;
result="\n/////////////////////////////////////////////////";
for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it !=GetNormalizedStmts()->end(); ++it){
   result+="\n********************************************";
   result+=(*it)->Print_str(1);
   }
result+="\n/////////////////////////////////////////////////\n";
WriteToFile(fn,result);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void
Program::PreprocessNormalized(){
for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it != GetNormalizedStmts()->end(); ++it)
   (*it)->NormalizeDomainConstraints(GetGlobalParameters());
return;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void
Program::Preprocess(void){
if(GetSyntaxTree()){
   GetSyntaxTree()->Link2Parent();
   GetSyntaxTree()->SingleCounterId();
   LookForParameters(true);
   Normalize();
   LookForParameters(true);
   NormalizeDomains();
      
   ComputeSequentialPrecedence();
   AvoidCountersAndSymbolicConstants();
   GenerateOptimalWayForFlowDependenciesComputing();
   global_properties=new LDemonstrator();
   CollectPropertiesOnIrregularLoops(global_properties);
   }   
PreprocessNormalized();   
return ;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void
Program::ComputeSequentialPrecedence(){
vector<string> common;
for(int i=0; i<GetNormalizedStmts()->size(); ++i){
   vector<bool> local;
   for(int j=0; j<=i;j++)
      local.push_back(false);
   for(int j=i+1; j<GetNormalizedStmts()->size();j++)
      local.push_back(IsSequentiallyBefore(i,j,&common));
   GetSequentialPrecedenceMatrix()->push_back(local);
   }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<ElementaryDependence*>
Program::GenerateOptimalWayForFlowDependenciesComputing(int stmt,string array,FADA_Index* __i){
vector<ElementaryDependence*> result;
References*   __references=GetNormalizedStmts()->operator[](stmt);
int common=__references->GetCounters()->size();
Statement* statement=__references->GetOrigin();

bool was_while=false;
Statement* parent=statement->GetParent();
if(statement->IsControl() && statement->GetControl()->IsWhileLoop()){
   parent=statement;
//    common--;
   was_while=true;
   }



int regular_begin=stmt;
int regular_end;
int reverse_begin=stmt-1;
int reverse_end;

while(!parent->IsProgram()){
   map<int, int>   to_be_avoided;
   vector<ElementaryDependence*>   local_if;

   while(parent->IsControl() && (parent->GetControl()->IsIf() || parent->GetControl()->IsIfElse())){
      reverse_end=statement->GetID();

      if(parent->GetControl()->IsIf())
         reverse_end=parent->GetID();
      else{
         if(find(parent->GetSurroundedStatements()->begin(), parent->GetSurroundedStatements()->end(),statement) == parent->GetSurroundedStatements()->end()){
            int last;
            parent->GetElseEnclosed(&reverse_end,&last);
            int _min,_max;
            parent->GetEnclosed(&_min,&_max);
            to_be_avoided[_min]=_max;
            }
         else{
            int _min,_max;
            parent->GetElseEnclosed(&_min,&_max);
            to_be_avoided[_min]=_max;
            }
         }

      local_if=ComputeDependenciesConfigs(reverse_begin,reverse_end,stmt,array,__i, common,GetNormalizedStmts());

   
      reverse_begin=parent->GetID();
      statement=parent;
      parent=statement->GetParent();
      }

   vector<ElementaryDependence*>   valid=EliminateValues2In(&result, &to_be_avoided);

   

   for(vector<ElementaryDependence*>::iterator it=valid.begin(); it != valid.end(); ++it)
      result.push_back(*it);



   // parent = loop
   
   if(!parent->IsProgram()){


      parent->GetEnclosed(&reverse_end,&reverse_begin);
   
      vector<ElementaryDependence*>   local;
      if(was_while)
         local=ComputeDependenciesConfigs(stmt,reverse_end,stmt,array,__i, common,GetNormalizedStmts());
      else
         local=ComputeDependenciesConfigs(stmt-1,reverse_end,stmt,array,__i, common,GetNormalizedStmts());
   

      for(vector<ElementaryDependence*>::iterator it=local.begin(); it != local.end(); ++it)
         result.push_back(*it);
   

      local=ComputeDependenciesConfigs(reverse_begin,reverse_end,stmt,array,__i, common -1,GetNormalizedStmts());
      for(vector<ElementaryDependence*>::iterator it=local.begin(); it != local.end(); ++it)
         result.push_back(*it);

   
//       local=ComputeDependenciesConfigs(stmt-1,reverse_end,stmt,array,__i, common -1,GetNormalizedStmts());
//       for(vector<ElementaryDependence*>::iterator it=local.begin(); it != local.end(); ++it)
//          result.push_back(*it);
//    
//       local=ComputeDependenciesConfigs(regular_end,stmt,  stmt,array, __i,common -1,GetNormalizedStmts());
//       for(vector<ElementaryDependence*>::iterator it=local.begin(); it != local.end(); ++it)
//          result.push_back(*it);

   
      statement=parent;
      parent=statement->GetParent();
      if(was_while==true)
         was_while=false;
      else;
      --common;
      }
   }

if(common != 0){
   cout<<"\nProgram::GenerateOptimalWayForFlowDependenciesComputing (common != 0)"<<common<<"\n";
   exit(EXIT_FAILURE);
   }

vector<ElementaryDependence*>    local=ComputeDependenciesConfigs(0,statement->GetID(),stmt,array,__i, common,GetNormalizedStmts());
for(vector<ElementaryDependence*>::iterator it=local.begin(); it != local.end(); ++it)
   result.push_back(*it);


return  EliminateDoubles(&result);
}
/////////////////////////////////
/////////////////////////////////

void
Program::GenerateOptimalWayForFlowDependenciesComputing(void){
for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it != GetNormalizedStmts()->end(); ++it)
   (*it)->GenerateOptimalWayForFlowDependenciesComputing((*it)->GetStmtID(),this);
}

/////////////////////////////////
/////////////////////////////////
void
Program::NormalizeDomains(void){
GetSyntaxTree()->NormalizeConditions(GetNormalizedStmts(),GetGlobalParameters());
}

/////////////////////////////////
/////////////////////////////////
void
Program::AvoidCountersAndSymbolicConstants(void){
for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it != GetNormalizedStmts()->end(); ++it)
   (*it)->AvoidCountersParametersDoubles(GetGlobalParameters());
}

/////////////////////////////////
/////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void      
Program::ComputeSourcesForAllReadVariables(){
Preprocess();
vector<References*>* __whole_program=GetNormalizedStmts();
for(vector<References*>::iterator it=__whole_program->begin(); it != __whole_program->end(); ++it)

   (*it)->ComputeDefinitions(   GetGlobalParameters(),
               GetNormalizedStmts(),
               GetSequentialPrecedenceMatrix(),
               GetGlobalProperties(),
               GetOptions()
               );
RenameNewParameters();
}



void
Program::ComputeSourcesOnNormalizedReferences(){
PreprocessNormalized();
vector<References*>* __whole_program=GetNormalizedStmts();
for(vector<References*>::iterator it=__whole_program->begin(); it != __whole_program->end(); ++it)

   (*it)->ComputeDefinitions(   GetGlobalParameters(),
               GetNormalizedStmts(),
               GetSequentialPrecedenceMatrix(),
               GetGlobalProperties(),
               GetOptions()
               );
RenameNewParameters();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void
Program::CollectPropertiesOnIrregularLoops(LDemonstrator* __properties){
vector<string> empty;
GetSyntaxTree()->CollectIrregularLoopsProperties(__properties,empty);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void
Program::CollectLoops(set<string>* all_loops){
for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it!=GetNormalizedStmts()->end(); ++it){
   for(vector<string>::iterator itc=(*it)->GetCounters()->begin(); itc != (*it)->GetCounters()->end();++itc)
      all_loops->insert(*itc);
   }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<string>   
Program::LookForParallelLoops(){
set<string>   s;
CollectLoops(&s);
vector<string> all_loops;
for(set<string>::iterator it=s.begin(); it!=s.end(); ++it)
   all_loops.push_back(*it);

for(vector<References*>::iterator it=GetNormalizedStmts()->begin(); it != GetNormalizedStmts()->end(); ++it){
   References* ref;
//    ref->LookForParallelLoops(&all_loops,*GetNormalizedStmts());
   (*it)->LookForParallelLoops(&all_loops,*GetNormalizedStmts());
   }

cout<<" \n================ Parallel Loops Detector:";
if(all_loops.size() == 0)
   cout<<"\n      All loops carry dependences";
else
   cout<<"\n      Parallel loops are : "<<PrintVector(&all_loops);
cout<<" \n================================";
return all_loops;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

}
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////  END   /////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//



