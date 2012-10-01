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

#include "controlstructure.h"
#include <global.h>

namespace fada{
/////////////////////////////////////////////////
/////////////////////////////////////////////////

Control::Control()
{
Initialize( );
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

Control::~Control()
{
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

void
Control::Initialize(void)
{
SetControlKind( ADaAn_NONE );
SetCounterName( "" );
SetUpperBound( NULL );
SetLowerBound( NULL );
SetCondition( NULL );
SetEnclosedAssigns( -1 , -1);
SetElseEnclosedAssigns( -1 , -1);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

void
Control::Set(string __counter, Expression* __lb, Expression* __ub)
{
SetControlKind( ADaAn_FOR );
SetCounterName( __counter );
SetUpperBound( __ub );
SetLowerBound( __lb );
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////


void
Control::Set(string __counter, Condition* __cond)
{
SetControlKind( ADaAn_WHILE );
SetCounterName( __counter );
SetCondition( __cond );
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////


void
Control::Set(Condition* __cond)
{
SetControlKind( ADaAn_IF );
SetCondition( __cond );
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////


Control::Control(string __counter, Expression* __lb, Expression* __ub)
{
Initialize( );
Set(__counter, __lb, __ub );
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////


Control::Control(string __counter, Condition* __cond)
{
Initialize( );
Set(__counter, __cond );
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

Control::Control(Condition* __cond)
{
Initialize( );
Set(__cond);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

void
Control::SwitchToIfElse(){
if(IsIf()){
   SetControlKind(ADaAn_IF_ELSE);
   return;
   }
cerr<<"\nControl::SwitchToIfElse .... fatal error (inapropriate case)\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////

Condition*
Control::GetDomainConstraints(int stmt)
{
Condition*   result;
if((IsLoop( ) || IsIf( )))
   {
   if( (stmt >= first_assign) && (stmt <= last_assign))
      {
      if(IsForLoop( ))
         {
         Inequation* ineq1=new Inequation ( new Expression(GetLoopCounter( )),FADA_GREATER_EQ,GetForLowerBound( ));
         Inequation* ineq2=new Inequation ( new Expression(GetLoopCounter( )),FADA_LESS_EQ,GetForUpperBound( ));
         return new Condition( new Condition(ineq1), FADA_AND , new Condition(ineq2));
         }
      else
         return GetCondition( );
      }
   else
      return new Condition(new Inequation (true));
   }
if(IsIfElse( ))
   {
   if( (stmt >= first_assign) && (stmt <= last_assign))
      return GetCondition( );
   else
      {
      if( (stmt >= else_first_assign) && (stmt <= else_last_assign))
         return GetCondition( )->FastNegation( );
      else
         return new Condition(new Inequation (true));
      }
   }
}

/////////////////////////////////////////////////
Condition*
Control::GetRegularDomain(int __id, vector<string>* __loop_counters, vector<string>* __param,LDemonstrator* __loop_ppts){
vector<string> variables=*__loop_counters;
if(IsLoop())
   variables.push_back(GetLoopCounter());
NormalizeConditions(__id,&variables,__param);
switch(GetNonAffineDomain()->size()){
   case 0:   goto simple_case;
      
   case 1:   {
      vector<Inequation*> v=*GetNonAffineDomain()->begin();
      if(v.size() <= 1)
         goto simple_case;
      }
   default:
      goto complicated_case;
   }

simple_case:
cout<<"\nSimple case";
exit(0);

if(IsIf() || IsIfElse() || IsWhileLoop())
   {
   if(IsWhileLoop()){
      cout<<"\nComplicated domain";
      exit(0);

      Inequation* ineq=new Inequation(new Expression(GetCounter()),FADA_GREATER_EQ,new Expression(0));
      Condition* count_ge_0 = new Condition (ineq);
      return new Condition(GetCondition(),FADA_AND,count_ge_0);
      }

   return GetCondition();
   }

if(IsForLoop()){
   Inequation* ineq1=new Inequation ( new Expression(GetLoopCounter( )),FADA_GREATER_EQ,GetForLowerBound( ));
   Inequation* ineq2=new Inequation ( new Expression(GetLoopCounter( )),FADA_LESS_EQ,GetForUpperBound( ));
   return new Condition( new Condition(ineq1), FADA_AND , new Condition(ineq2));
   }

cout<<"\nControl::GetRegularDomain ... inappropriate case 1";
exit(0);

complicated_case:
      cout<<"\nComplicated domain";
      exit(0);
Condition* result=new Condition(new Inequation(true));
for(vector<vector<Inequation*> >::iterator it=GetAffineDomain()->begin(); it != GetAffineDomain()->end(); ++it){
   result=new Condition(ToDNFTerm(&(*it)),FADA_AND,result);
   }
if(IsWhileLoop()){
   Inequation* ineq=new Inequation(new Expression(GetCounter()),FADA_GREATER_EQ,new Expression(1));
   Condition* count_ge_0 = new Condition (ineq);
   result=new Condition(count_ge_0,FADA_AND,result);
   }
ostringstream ch;
ch<<"domain_"<<__id;
Expression* non_affine=new Expression(ch.str());
for(vector<string>::iterator it=__loop_counters->begin(); it != __loop_counters->end();++it)
   non_affine->AddArgument(new Expression(*it));
if(IsLoop())
   non_affine->AddArgument(new Expression(GetLoopCounter()));
if(non_affine->IsVariable())
   __param->push_back(non_affine->GetVariableName());
return new Condition(new Condition(new Inequation(non_affine,FADA_NEQ,new Expression(0))),FADA_AND,result);
}

/////////////////////////////////////////////////

Condition*
Control::GetRegularDomain(void){
Condition* result;
if(IsIf() || IsIfElse() || IsWhileLoop())
   {
   if(IsWhileLoop()){
      Inequation* ineq=new Inequation(new Expression(GetCounter()),FADA_GREATER_EQ,new Expression(1));
      Condition* count_ge_0 = new Condition (ineq);
      result=new Condition(count_ge_0,FADA_AND,GetCondition());
      return result;
      }
   return GetCondition();
   }
if(IsForLoop()){
   Inequation* ineq1=new Inequation ( new Expression(GetLoopCounter( )),FADA_GREATER_EQ,GetForLowerBound( ));
   Inequation* ineq2=new Inequation ( new Expression(GetLoopCounter( )),FADA_LESS_EQ,GetForUpperBound( ));
   result = new Condition( new Condition(ineq1), FADA_AND , new Condition(ineq2));
   return result;
   }
cout<<"\nControl::GetRegularDomain(void) .... inappropriate case \n";
throw(10);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

string
Control::Print_str(int what_to_print)
{
string result;

ostringstream   s;
int   stmt1,stmt2;
GetEnclosedAssignments( &stmt1,&stmt2);
s<<"[ "<<stmt1<<", "<<stmt2<<" ]";
GetElsePartAssignments( &stmt1,&stmt2);
s<<"[ "<<stmt1<<", "<<stmt2<<" ]";

switch(GetControlKind( ))
   {
   case ADaAn_FOR      :result = "for("+GetLoopCounter( )+":"+GetForLowerBound( )->Print_str( )+":"+GetForUpperBound( )->Print_str( )+")"; break;
   case ADaAn_WHILE   :result = "while("+GetCondition( )->Print_str()+")"; break;
   case ADaAn_IF      :result = "if("+GetCondition( )->Print_str()+")"; break;
   case ADaAn_IF_ELSE   :result = "if the else("+GetCondition( )->Print_str()+")"; break;
   default: cerr<<endl<<"Fatal error, Control::Print_str(), unhandled operator ";
      throw(BAD_OPERATOR);
      exit(EXIT_FAILURE);
   }
result+="\tcounter = "+GetLoopCounter()+"\t"+s.str();
if(what_to_print == __SECURE_PRINT)
   return result;


result+="\n Affine constraints :";
for(vector<vector<Inequation*> >::iterator it=GetAffineDomain()->begin(); it != GetAffineDomain()->end(); ++it){
   result+="\n";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      result+="\t"+(*iti)->Print_str();
   }

result+="\nNon Affine constraints :";
for(vector<vector<Inequation*> >::iterator it=GetNonAffineDomain()->begin(); it != GetNonAffineDomain()->end(); ++it){
   result+="\n";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      result+="\t"+(*iti)->Print_str();
   }
result+="\n";
if(!IsIfElse())
   return result;


result+="\n Affine NOT DOMAIN :";
for(vector<vector<Inequation*> >::iterator it=GetAffineElseDomain()->begin(); it != GetAffineElseDomain()->end(); ++it){
   result+="\n";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      result+="\t"+(*iti)->Print_str();
   }

result+="\nNon Affine  NOT_DOMAIN :";
for(vector<vector<Inequation*> >::iterator it=GetNonAffineElseDomain()->begin(); it != GetNonAffineElseDomain()->end(); ++it){
   result+="\n";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      result+="\t"+(*iti)->Print_str();
   }
result+="\n";

return result;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

void
Control::ReferencedScalars(set<string>* scalars)
{
if(IsIf( ) || IsIfElse( ) || IsWhileLoop( )){
   GetCondition( )->ReferencedScalars( scalars );
   return ;
   }
if(IsForLoop( ))
   {
   GetForLowerBound( )->ReferencedScalars( scalars );
   GetForUpperBound( )->ReferencedScalars( scalars );
   return;
   }

cerr<<endl<<"Control::ReferencedScalars(), fatal error, unhandled control structure"<<endl;
throw(UNHANDLED_CASE);
exit(EXIT_FAILURE);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

string
Control::Generate_C_Code(void)
{

string result;

ostringstream   s;

switch(GetControlKind( ))
   {
   case ADaAn_FOR      :
         result= "for ( "+GetLoopCounter( );
         result+= " = "+GetForLowerBound( )->Generate_C_Code();
         result+= "; "+GetLoopCounter()+" <= "+GetForUpperBound()->Generate_C_Code();
         result+= "; ++" + GetLoopCounter() +")";
         break;
   case ADaAn_WHILE   :result = "while("+GetCondition( )->Generate_C_Code()+")"; break;
   case ADaAn_IF      :result = "if("+GetCondition( )->Generate_C_Code()+")"; break;
   case ADaAn_IF_ELSE   :result = "if/*then else*/("+GetCondition( )->Generate_C_Code()+")"; break;
   default: cerr<<endl<<"Fatal error, Control::Print_str(), unhandled operator ";
      throw(UNHANDLED_CASE);
      exit(EXIT_FAILURE);
   }
return result;

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////

string
Control::PrettyPrint_str(void){

string result;

switch(GetControlKind( ))
   {
   case ADaAn_FOR      :
         result= "for ( "+GetLoopCounter( );
         result+= " = "+GetForLowerBound( )->Generate_C_Code();
         result+= "; "+GetLoopCounter()+" <= "+GetForUpperBound()->Generate_C_Code();
         result+= "; ++" + GetLoopCounter() +")";
         return result;
         
   case ADaAn_WHILE   :result = "while("+GetCondition( )->Generate_C_Code()+")"; return result;
   case ADaAn_IF      :result = "if("+GetCondition( )->Generate_C_Code()+")"; return result;
   case ADaAn_IF_ELSE   :result = "if("+GetCondition( )->Generate_C_Code()+")"; return result;
   default: cerr<<endl<<"Fatal error, Control::Print_str(), unhandled operator ";
      throw(UNHANDLED_CASE);
      exit(EXIT_FAILURE);
   }
// return result;
}
////////////////////////////////////
////////////////////////////////////
void
Control::Substitute(map<string,string>* __mapping){
switch(GetControlKind()){
   case ADaAn_FOR      :
            if(__mapping->find(GetCounter()) != __mapping->end())
               SetCounterName(__mapping->operator[](GetCounter()));
            GetForLowerBound()->Substitute(__mapping);
            GetForUpperBound()->Substitute(__mapping);
            return;
   case ADaAn_WHILE   :
            if(__mapping->find(GetCounter()) != __mapping->end())
               SetCounterName(__mapping->operator[](GetCounter()));
            GetCondition()->Substitute(__mapping);
            return;
   case ADaAn_IF      :
            GetCondition()->Substitute(__mapping);
            return;
   case ADaAn_IF_ELSE   :
            GetCondition()->Substitute(__mapping);
            return;
   }

cerr<<"Control::Rename, Fatal error(unhandled operator)";
throw(UNHANDLED_CASE);
exit(EXIT_FAILURE);
}
/////////////////////////////////////////////////
void
Control::TagAllInequations(int id, int __deep, vector<string>* var){
vector<Expression*>*   iteration=new vector<Expression*>;

for(vector<string>::iterator it=var->begin(); it != var->end(); ++it)
   iteration->push_back(new Expression(*it));

for(vector<vector<Inequation*> >::iterator it=GetNonAffineDomain()->begin(); it != GetNonAffineDomain()->end(); ++it)
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti !=(*it).end(); ++iti)
      (*iti)->Tag(id,iteration);

for(vector<vector<Inequation*> >::iterator it=GetNonAffineElseDomain()->begin(); it != GetNonAffineElseDomain()->end(); ++it)
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      (*iti)->Tag(id,iteration);

}
/////////////////////////////////////////////////
void
Control::NormalizeConditions(int id_stmt,vector<string>* var, vector<string>* parameters){
Condition*       __domain=GetRegularDomain();


__domain=__domain->EliminateNotOperations(true);

vector<Condition*>   __terms=__domain->SplitToTerms();



vector<vector<Inequation*> > affine_for_sure;
vector<vector<Inequation*> > non_affine_for_the_moment;

for(vector<Condition*>::iterator it=__terms.begin(); it!= __terms.end(); ++it){
   vector<Inequation*>   all_term_inequations=(*it)->GetTermInequations();
   vector<Inequation*>   term_affine, term_non_affine;
   SortInequations(&all_term_inequations,&term_affine,&term_non_affine,var, parameters);

   Condition* eliminate_neq=ToDNFTerm(&term_affine);
   eliminate_neq=eliminate_neq->TraduceNEQInequations();
   vector<Condition*>    affine_affine_terms=eliminate_neq->SplitToTerms();
   for(vector<Condition*>::iterator itt=affine_affine_terms.begin(); itt !=  affine_affine_terms.end(); ++itt)
      affine_for_sure.push_back((*itt)->GetTermInequations());

   if(term_non_affine.size() > 0)
      non_affine_for_the_moment.push_back(term_non_affine);
   }
SetAffineInequations(&affine_for_sure);

SetNonAffineInequations(&non_affine_for_the_moment);

if(!IsIfElse())
   return;

   // hihiiii, i'm not obliged to rewrite code or/and redeclare other variables .... ngoul rani courage à 03:09  :-)
   {
   Condition*       __domain=GetRegularDomain();
   __domain=__domain->Negate(true);
   
   vector<Condition*>   __terms=__domain->SplitToTerms();
   
   vector<vector<Inequation*> > affine_for_sure;
   vector<vector<Inequation*> > non_affine_for_the_moment;
   
   for(vector<Condition*>::iterator it=__terms.begin(); it!= __terms.end(); ++it){
      vector<Inequation*>   all_term_inequations=(*it)->GetTermInequations();
      vector<Inequation*>   term_affine, term_non_affine;
      SortInequations(&all_term_inequations,&term_affine,&term_non_affine,var, parameters);
      Condition* eliminate_neq=ToDNFTerm(&term_affine);
      eliminate_neq=eliminate_neq->TraduceNEQInequations();
      vector<Condition*>    affine_affine_terms=eliminate_neq->SplitToTerms();
      for(vector<Condition*>::iterator itt=affine_affine_terms.begin(); itt !=  affine_affine_terms.end(); ++itt)
         affine_for_sure.push_back((*itt)->GetTermInequations());
   
      non_affine_for_the_moment.push_back(term_non_affine);
      }
   SetAffineElseInequations(&affine_for_sure);
   SetNonAffineElseInequations(&non_affine_for_the_moment);
   return;
   }
TagAllInequations(id_stmt, 0,var);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
};



