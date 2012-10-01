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


#include "assignment.h"
#include "constants.h"
#include <stdlib.h>


namespace fada{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Assignment::Assignment()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Assignment::~Assignment()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void
Assignment::AddRHSReference(string __name, FADA_Index* __index)
{

GetReferences()->push_back(new Read_Reference(__name,__index));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Assignment::CollectReferences(void)
{
CollectReferences( GetRHSExpression( ));
CollectReferences( GetLHSIndex( ) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Assignment::CollectReferences(Expression* __expr)
{
if(__expr->IsLeaf( ))
   {
   if(__expr->IsVariable( ))
      {
      vector<Expression*>   v;
      FADA_Index*   empty=new FADA_Index(&v);
      AddRHSReference( __expr->GetVariableName( ), empty);
      return;
      }
   if(__expr->IsArray( ))
      {
      FADA_Index*   __index=new FADA_Index(__expr->GetIndex( ));
      AddRHSReference( __expr->GetArrayName( ), __index);
      for(vector<Expression*>::iterator it= __expr->GetIndex( )->begin(); it !=  __expr->GetIndex( )->end(); ++it)
         CollectReferences(  (*it) );
      return;
      }
   if(__expr->IsFunction( ))
      {
      for(vector<Expression*>::iterator it= __expr->GetArguments( )->begin(); it !=  __expr->GetArguments( )->end(); ++it)
         CollectReferences( *it );
      return;
      }
   if(__expr->IsValue())
      return;

   cout<<"\n\nAssignment::CollectReferences, fata error : invalid leaf information\n";
    exit(1);
   }
CollectReferences( __expr->GetLeftChild( ) );
CollectReferences( __expr->GetRightChild( ) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Assignment::CollectReferences(FADA_Index* __index)
{
for(int i=0; i<__index->GetSize( ); ++i)
   CollectReferences( __index->GetItem( i ) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Assignment::Set(string __lhs, FADA_Index* __lhs_index, Expression* __exp)
{
SetLHS( __lhs, __lhs_index);
SetRHS( __exp );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Assignment::Initialize(void)
{
SetRank( -1 );
SetLHS( "", NULL );
SetRHS( NULL );
vector<string> empty;
SetLoopCounters( empty );
//References.clear();
GetReferences()->clear();
normalized_domain.clear();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Assignment::Assignment(string __lhs, Expression* __rhs){
Initialize( );
Set( __lhs, new FADA_Index(), __rhs );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Assignment::Assignment(string __lhs, FADA_Index* __lhs_index, Expression* __rhs)
{
Initialize( );
Set( __lhs, __lhs_index, __rhs );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Assignment::Assignment(Expression* __rhs){
Initialize( );
Set("", new FADA_Index(), __rhs );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Assignment::AddControl(Control* __control)
{
surrounded_by_controls.push_back(__control);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Assignment::Prepare(void)
{
CollectReferences( );      //collect the referenced entities   


vector<Control*>::iterator it=GetSurroundingControls( )->begin();
Condition*   domain=new Condition(new Inequation(true));
while(it != GetSurroundingControls( )->end())
   {
   Control* st;
    
   if((*it)->IsLoop())
      AddCounter( (*it)->GetLoopCounter());

   Condition*   __st_domain=(*it)->GetDomainConstraints(GetRank());

   domain= new Condition(domain,FADA_AND,  __st_domain);
   ++it;
   }

Condition*   __no_not= domain->EliminateNotOperations();
Condition*   __dnform= __no_not->DNForm();
vector<Condition*>* new_domain=new vector<Condition*>();
*new_domain=__dnform->GetTerms();

SetDomain(new_domain);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
string
Assignment::Pretty_print_str(){

string result=GetLHSArray()+GetLHSIndex()->Generate_C_Code()+ " = "+ GetRHSExpression()->Print_str()+";";

return result;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
string
Assignment::Print_str()
{
ostringstream   s;
s<<GetRank();
string result=s.str()+" -> "+GetLHSArray()+"["+GetLHSIndex()->Print_str()+"] = "+GetRHSExpression()->Print_str()+";";

string controls;

for(vector<Control*>::iterator it=GetSurroundingControls()->begin(); it != GetSurroundingControls()->end(); ++it)
   controls+="\n\t"+(*it)->Print_str();

string __domain;
for(vector<Condition*>::iterator it=GetDomain()->begin(); it != GetDomain()->end(); ++it)
   __domain+="\n\t"+(*it)->Print_str();

string __counters="\t";
for(vector<string>::iterator it=GetLoopCounters()->begin(); it != GetLoopCounters()->end(); ++it)
   __counters+="\t"+(*it);

result+="\n\n''controls"+controls+"\n\n''domain"+__domain+"\n\n''counters"+__counters;
return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool
Assignment::IsControled(void)
{

for(vector<Condition*>::iterator it=GetDomain( )->begin(); it != GetDomain( )->end(); ++it)
   if(!(*it)->IsValue(true))
      return false;
return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

string
Assignment::Full_Print_str()
{
string result="\n\n\n////////////// statement ///////////////////////////\n";
result+=Print_str();
 result+="\n////////////// definitions ///////////////////////////\n";
for(vector<Read_Reference*>::iterator it=this->GetReferences()->begin(); it != this->GetReferences()->end(); ++it)
   result +="\n"+(*it)->Print_str(0);

return result;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


string
Assignment::ToHtml(string dir)
{

/*ostringstream   html;
vector<Read_Reference*>*   rhs=GetReferences();

string source_line=this->GetLHSArray()+this->GetLHSIndex()->Generate_C_Code()+" = "+this->GetRHSExpression()->Print_str();

html<<"<HTML> \n<HEAD> \n <TITLE> Definition of all right-hand-side references for assignment N�"<<this->GetRank()<<"</TITLE> \n </HEAD> \n    <BODY>";
html<<source_line;
html<<"<HR WIDTH=\"100%\" COLOR=\"#6699FF\" SIZE=\"20\">";
int i=-1;
for(vector<Read_Reference*>::iterator it=rhs->begin(); it != rhs->end(); ++it)
   {
   ++i;
   Read_Reference*   ref=*it;

   ADaAn_Quast *q=ref->GetDefinition();

   ostringstream   fn; fn<<this->GetRank()<<"_"<<i<<"_";
   
   html<<"\n"+ref->ToHtml(dir, fn.str());   
   }

html<<"\n</BODY>";
html<<"\n</HTML>";


//writting in file
ostringstream file_name;
file_name<<"assign_"<<this->GetRank()<<"_.html";

FILE*   f;
string full_file_name=dir+file_name.str();
f=fopen(full_file_name.c_str(),"w");
if(f != NULL){
   fprintf(f,"%s", html.str().c_str());
   fclose(f);
   }
else
   cout<<"\n\nWARNING, file '"<<full_file_name<<"' can not be created \n\n";


return file_name.str();*/
}


////////////////////////////////////////////////
void
Assignment::Rename(map<string, string>* __mapping){
if(__mapping->find(GetLHSArray()) != __mapping->end())
        SetLHSArrayName(__mapping->operator[](GetLHSArray()));
GetLHSIndex()->Substitute(__mapping);
GetRHSExpression()->Substitute(__mapping);
}

////////////////////////////////////////////////

string
Assignment::Generate_GraphViz_Edges(Program* prog, bool print_condition, bool print_relation, bool rename,bool bottom,bool alphas)
{
/*string result;
for(vector<Read_Reference*>::iterator it=this->GetReferences()->begin(); it != GetReferences()->end(); ++it)
   {
   Read_Reference*   ref=*it;
   ADaAn_Quast *q=ref->GetDefinition();
   Condition*   cond=new Condition(new Inequation (true));
   result+=q->Generate_GraphViz_Edges(prog,GetRank(),NULL,print_condition, print_relation, rename,bottom,alphas);
   }
return result;*/
cout<<"\nAssignment::Generate_GraphViz_Edges .... not implemented\n";
exit(EXIT_FAILURE);
}

////////////////////////////////////////////////
////////////////////////////////////////////////

string
Assignment::Generate_C_Code(void)
{
return this->GetLHSArray()+this->GetLHSIndex()->Generate_C_Code()+" = "+ this->GetRHSExpression()->Generate_C_Code(); 
}


////////////////////////////////////////////////
void
Assignment::ReferencedScalars(set<string>* __written, set<string>* __read){
if(GetLHSIndex()->GetSize()==0)
   __written->insert(GetLHSArray());
vector<string>::iterator itarray=lhs_array_plus.begin();
vector<FADA_Index*>::iterator itindex=lhs_index_plus.begin();
while(itarray != lhs_array_plus.end() && itindex != lhs_index_plus.end()){
   if((*itindex)->GetSize()==0)
      __written->insert(*itarray);
   itarray++;
   itindex++;
   }

GetLHSIndex()->ReferencedScalars(__read);
GetRHSExpression()->ReferencedScalars(__read);
return ;
}
////////////////////////////////////////////////


string
Assignment::Generate_C_Test_Code(string prefix)
{
string result;


for(vector<Read_Reference*>::iterator it=this->GetReferences()->begin(); it != this->GetReferences()->end(); ++it)
   result+=(*it)->Generate_C_Test_Code(prefix);


result+=prefix+"/*********************** array writing access   simulation  Go--> *******************/";

result+=prefix+"vector<int> v;";      //creating vector to store current iteration

for(vector<string>::iterator it=this->GetLoopCounters()->begin(); it != this->GetLoopCounters()->end();++it)
   result+=prefix+"v.push_back("+(*it)+");";   //storing each loop conter value

ostringstream rank;
rank<<this->GetRank();
result+=prefix+"__Test_Info  __t_i("+rank.str()+",v);";
ostringstream ss;
ss<<"\t\t\t //Statement N�:"+rank.str()+" defines the array-case : "<<this->GetLHSArray() + this->GetLHSIndex()->Generate_C_Code();
ss<<" durant the iteration 'v'";
result+=ss.str();
result+=prefix+FADA_Tracer_Prefix+this->GetLHSArray()+this->GetLHSIndex()->Generate_C_Code()+".Touch(__t_i) ;"; 
result+=prefix+"/*********************** array writing access   simulation -->End  ******************/";
result+=prefix+"/****************** DEBUG : 'Statement::GenerateCCode(Program* p,string tabs)' **********/";
return result;

}



void
Assignment::RenameNewParameters(map<string, string> *__mapping){
for(vector<Read_Reference*>::iterator it=GetReferences()->begin(); it != GetReferences()->end(); ++it){
   (*it)->GetDefinition()->Substitute(__mapping);
   }
}


bool
Assignment::IsInc(string variable){
if(GetLHSIndex()->GetSize() > 0)
   return false;
Expression* expr=GetRHSExpression();
bool inc=(!expr->IsLeaf() && expr->IsAddition() && ((expr->GetRightChild()->IsValue() && expr->GetRightChild()->GetValue()==1) && expr->GetLeftChild()->IsVariable() && expr->GetLeftChild()->GetVariableName()== variable) ||(expr->GetLeftChild()->IsValue() && expr->GetLeftChild()->GetValue()==1) && expr->GetRightChild()->IsVariable() && expr->GetRightChild()->GetVariableName()== variable);

return inc;
}

bool
Assignment::IsScalarInitialization(string* __scalar){
if(GetLHSIndex()->GetSize()==0){
   *__scalar=GetLHSArray();
   return true;
   }
return false;
}
void
Assignment::Add_LHS_PLUS(string array, FADA_Index* index){
lhs_array_plus.push_back(array);
lhs_index_plus.push_back(index);
}

};



