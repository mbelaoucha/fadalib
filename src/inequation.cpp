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


#include "inequation.h"
#include "global.h"
#include "condition.h"
#include <algorithm>



namespace fada{
///////////////////////////////////////////
///////////////////////////////////////////
T_predicate
Opposite(T_predicate t){

switch(t){
   case FADA_EQ       : return FADA_NEQ;
   case FADA_NEQ      : return FADA_EQ;
   case FADA_LESS      : return FADA_GREATER_EQ;
   case FADA_LESS_EQ   : return FADA_GREATER;
   case FADA_GREATER   : return FADA_LESS_EQ;
   case FADA_GREATER_EQ   : return FADA_LESS;
   default         : cerr<<"\n Opposite(T_predicate t) .... FATAL ERROR (bad object) \n";
         throw(BAD_OPERATOR);
         exit(EXIT_FAILURE);
   }
}


///////////////////////////////////////////
///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


vector<int>
Inequation::ToVector(vector<string>* c, map<string, int>* counters,vector<string>*p, map<string, int>* parameters)
{

if(IsValue()){
   if(GetValue())
      return GenerateTruePipVector(c->size()+p->size());
   return GenerateFalsePipVector(c->size()+p->size());
   }
vector<int> coded_lhs = this->GetLHS()->ToVector(c, counters,p , parameters);
vector<int> coded_rhs = this->GetRHS()->ToVector(c, counters,p , parameters);

int dim=coded_lhs.size();
vector<int> result(dim+1);


switch(this->GetPredicate())
   {
   case FADA_LESS:
      {
      for(int i=1; i<=dim; i++)
         result[i] = coded_rhs[i-1] - coded_lhs[i-1];
      result[dim]--;
      result[0] = 1; //>=
      return result;
      }
   case FADA_LESS_EQ:
      {
      for(int i=1; i<=dim; i++)
         result[i] = coded_rhs[i-1] - coded_lhs[i-1];

      result[0] = 1; //>=
      return result;
      }
    case FADA_GREATER:
            {
      for(int i=1; i<=dim; i++)
         result[i] = coded_lhs[i-1] - coded_rhs[i-1];
      result[dim]--;
      result[0] = 1; //>=
      return result;

      }
   case FADA_GREATER_EQ:
      {
      for(int i=1; i<=dim; i++)
         result[i] = coded_lhs[i-1] - coded_rhs[i-1];

      result[0] = 1; //>=
      return result;
      }
   case FADA_EQ:
      {
      for(int i=1; i<=dim; i++)
         result[i] = coded_lhs[i-1] - coded_rhs[i-1];

      result[0] = 0; // equality
      return result;
      }
   default:
      cerr<<"\nInequation::ToVector ... fatal error : unhandled operator\n";
      cerr<<Print_str()<<"\n";
      throw(BAD_OPERATOR);
      exit(EXIT_FAILURE);
   }
return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Inequation::Inequation()
{
Initialize( );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Inequation::~Inequation()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void         
Inequation::Initialize(void)
{
SetLHS( NULL);
SetRHS( NULL);
SetValue( false);
SetPredicate( FADA_NO_COMP);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Inequation::Inequation(Expression* lhs,T_predicate pred, Expression* rhs)
{
Initialize( );
SetPredicate( pred );
SetLHS( lhs );
SetRHS( rhs );
is_value = false;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Inequation::Inequation(bool val)
{
Initialize( );
is_value=true;
SetValue( val );
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Inequation::Inequation(Inequation* ineq)
{
Initialize( );
is_value=ineq->is_value;
SetValue( ineq->GetValue( ) );
SetRHS  ( ineq->GetRHS  ( ) );
SetLHS  ( ineq->GetLHS  ( ) );
SetPredicate( ineq->GetPredicate( ) );
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


void
Inequation::Set( Expression* lhs,T_predicate pred, Expression* rhs)
{
Initialize( );
is_value = false;
SetPredicate( pred );
SetLHS ( lhs );
SetRHS ( rhs );
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Inequation::Set(bool  val)
{
Initialize( );
is_value = true;
SetValue( val);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Inequation::Print( void )
{
cout<<Print_str( );
}

/////////////////////////////////////////////////////////////////////////////////////////////
string
Inequation::Full_Tex_Print_str(vector<string>* var, vector<string> *param){
if(IsValue( )){
   if(GetValue( ))
      return "true";
   else
      return "false";
   }

string result;
result = GetLHS( )->Full_Tex_Print_str( var, param);
switch(GetPredicate( ))
   {
   case FADA_EQ      : result += " = "; break;
   case FADA_NEQ      : result += " \\neq "; break;
   case FADA_LESS      : result += " < "; break;
   case FADA_LESS_EQ   : result += " \\leq "; break;
   case FADA_GREATER   : result += " > "; break;
   case FADA_GREATER_EQ   :result += " \\geq "; break;
   default: cerr<<endl<<"'Inequation::Print_Tex_str' ==> fatal error, invalide predicate"; 
      throw(BAD_OPERATOR);
      exit(EXIT_FAILURE);
   }
result += GetRHS( )->Full_Tex_Print_str(var, param );
return result;   
}
/////////////////////////////////////////////////////////////////////////////////////////////

string
Inequation::Print_Tex_str(void){
if(IsValue( )){
   if(GetValue( ))
      return "true";
   else
      return "false";
   }
string result;
result = GetLHS( )->Print_Tex_str( );
switch(GetPredicate( ))
   {
   case FADA_EQ      : result += " = "; break;
   case FADA_NEQ      : result += " \\neq "; break;
   case FADA_LESS      : result += " < "; break;
   case FADA_LESS_EQ   : result += " \\leq "; break;
   case FADA_GREATER   : result += " > "; break;
   case FADA_GREATER_EQ   :result += " \\geq "; break;
   default: cerr<<endl<<"'Inequation::Print_Tex_str' ==> fatal error, invalide predicate"; 
      throw(BAD_OPERATOR);
      exit(EXIT_FAILURE);
   }
result += GetRHS( )->Print_Tex_str( );
return result;   

}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

string
Inequation::Print_str( void )
{
if(IsValue( ))
   if(GetValue( ))
      return "true";
   else
      return "false";
string result;
result = GetLHS( )->Print_str( );
switch(GetPredicate( ))
   {
   case FADA_EQ      : result += " == "; break;
   case FADA_NEQ      : result += " != "; break;
   case FADA_LESS      : result += " < "; break;
   case FADA_LESS_EQ   : result += " <= "; break;
   case FADA_GREATER   : result += " > "; break;
   case FADA_GREATER_EQ   :result += " >= "; break;
   default: cerr<<endl<<"'Inequation::Print_str' ==> fatal error, invalide predicate"; 
      throw(BAD_OPERATOR);
      exit(0);
   }
result += GetRHS( )->Print_str( );
return result;   
};   

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Inequation*
Inequation::Negate   (bool smart){
if(IsValue( ))
   return new Inequation(!GetValue( ));

T_predicate new_pred;
switch( GetPredicate( ) )
   {
   case FADA_LESS      : new_pred=FADA_GREATER_EQ; break;
   case FADA_LESS_EQ   : new_pred=FADA_GREATER; break;
   case FADA_GREATER   : new_pred=FADA_LESS_EQ; break;
   case FADA_GREATER_EQ   : new_pred=FADA_LESS; break;
   case FADA_NEQ      : new_pred=FADA_EQ; break;
   case FADA_EQ      : new_pred=FADA_NEQ; break;

   default: cerr<<endl<<"'Inequation::Negate(smart)' ==> fatal error, invalide predicate"; 
      throw(BAD_OPERATOR);
      exit(EXIT_FAILURE);   
   }

return new Inequation( GetLHS( ),new_pred, GetRHS( ));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
Condition*
Inequation::Negate(void)
{
if(IsValue( ))
   return new Condition(new Inequation(!GetValue( )));

T_predicate new_pred;
switch( GetPredicate( ) )
   {
   case FADA_LESS      : new_pred=FADA_GREATER_EQ; break;
   case FADA_LESS_EQ   : new_pred=FADA_GREATER; break;
   case FADA_GREATER   : new_pred=FADA_LESS_EQ; break;
   case FADA_GREATER_EQ   : new_pred=FADA_LESS; break;
   case FADA_NEQ      : new_pred=FADA_EQ; break;
   case FADA_EQ      :{
            Inequation* ineq1= new Inequation( GetLHS( ),FADA_LESS,GetRHS( ));
            Inequation* ineq2= new Inequation( GetLHS( ),FADA_GREATER, GetRHS( ));
            return new Condition( new Condition(ineq1),FADA_OR, new Condition(ineq2));
            } 
   default: cerr<<endl<<"'Inequation::Negate' ==> fatal error, invalide predicate"; 
      throw(BAD_OPERATOR);
      exit(0);   
   }

return new Condition(new Inequation( GetLHS( ),new_pred, GetRHS( )));
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Inequation*
Inequation::Clone(void)
{
if(IsValue( ) )
   return new Inequation(GetValue( ) );
return new Inequation ( GetLHS( )->Clone( ), GetPredicate( ) ,GetRHS( )->Clone( ));   
};
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Inequation*
Inequation::Copy(void)
{
if(IsValue( ) )
   return new Inequation(GetValue( ) );
return new Inequation ( GetLHS( )->Copy ( ),GetPredicate( ) , GetRHS( )->Copy ( ));
};

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


bool
Inequation::IsAffine(vector<string>* var, vector<string>* param)
{
if(IsValue())
   return true;
bool Affine_lhs=true,Affine_rhs=true;
Affine_lhs=GetLHS( )->IsAffine( var, param );
if(Affine_lhs)
   Affine_rhs=GetRHS( )->IsAffine( var, param);
return Affine_lhs && Affine_rhs;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
Inequation*
Inequation::Substitute(map<string, Expression*>* __mapping)
{
if(!IsValue()){
   return new Inequation( GetLHS( )->Substitute(__mapping ),GetPredicate( ), GetRHS()->Substitute( __mapping ));
   }
else
   return Clone();
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void
Inequation::Substitute(map<string, string>* __mapping)
{
if(!IsValue( ))
   {
   GetLHS( )->Substitute( __mapping );
   GetRHS( )->Substitute( __mapping );
   }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
Inequation::ToCondition(void)
{
return new Condition(this);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Inequation::ReferencedScalars(set<string>* scalars)
{

if(IsValue( ))
   return ;

GetLHS( )->ReferencedScalars( scalars );
GetRHS( )->ReferencedScalars( scalars );

}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/*bool
Inequation::IsTrivial(bool* __value, vector<string>* __param)
{
map<string,int> __map_param;
map<string,int> __empty;
vector<string> var;
int i=0;
for(vector<string>::iterator it=__param->begin(); it != __param->end(); ++it,++i)
   __map_param[*it]=i;

vector<int> encoded=ToVector(&var,&__empty,__param,&__map_param);

int constante=encoded[encoded.size()-1];
for(i=1; i<encoded.size()-1;++i)
   if (encoded[i] !=0)
      return false;
if(encoded[0]==0)   //equality
   *__value=(constante==0);
else
   *__value=(constante >=0);
return true;


Serialized_Expression*   __this=GetFromNormalizedADaAnInequation(this,&var,__param);
if(!__this->IsTrivial())
   return false;
*__value=(__this->IsTrivialTrue());
return true;
}*/
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

string
Inequation::Generate_C_Code(void)
{
if(IsValue( ))
   if(GetValue( ))
      return "true";
   else
      return "false";
string result;
result = GetLHS( )->Generate_C_Code( );
switch(GetPredicate( ))
   {
   case FADA_EQ      : result += " == "; break;
   case FADA_NEQ      : result += " != "; break;
   case FADA_LESS      : result += " < "; break;
   case FADA_LESS_EQ   : result += " <= "; break;
   case FADA_GREATER   : result += " > "; break;
   case FADA_GREATER_EQ   :result += " >= "; break;
   default: cerr<<endl<<"'Inequation::Generate_C_Code(void) ==> fatal error, invalide predicate"; 
      throw(BAD_OPERATOR);
      exit(0);
   }
result += GetRHS( )->Generate_C_Code( );
return result;   

}

///////////////////////////////////////////
///////////////////////////////////////////

int
Inequation::SimplifyWith(Inequation* ineq,vector<string>* param){
map<string,int> __map_var,__map_param;
vector<string> var;
cout<<"\n Simplify With ******************************* \n";
cout<<"\n Inequa 1: "<<Print_str();
cout<<"\n Inequa 2: "<<ineq->Print_str();

int i=0;
for(vector<string>::iterator it=param->begin(); it != param->end(); ++it,++i)
   __map_param[*it]=i;

if(GetPredicate() != FADA_NEQ && ineq->GetPredicate() != FADA_NEQ){
   vector<int> __this=this->ToVector(&var,&__map_var,param,&__map_param);
   vector<int> __ineq=ineq->ToVector(&var,&__map_var,param,&__map_param);
   if(__this.size() != __ineq.size()){
      cout<<"\nInequation::Imply .... inappropriate case \n";
      throw(10);
      }
   for(i=1; i<__this.size()-1; i++)
      if(__this[i]-__ineq[i] != 0){
         cout<<"\n === NO_SIMPLIFICATION";
         return NO_SIMPLIFICATION;
         }

   int constante=__this[__this.size()]-__ineq[__this.size()];
   if(__this[0]==__ineq[0] && __this[0]==0){ //equalities
      if(constante != 0){
         cout<<"\n === ALWAYS_FALSE";
         return ALWAYS_FALSE;
         }
      cout<<"\n === ALWAYS_TRUE";
      return ALWAYS_TRUE;
      }
   if(__this[0]==__ineq[0] && __this[0]==1){ //inequalities
      if(constante == 0)
         return ALWAYS_TRUE;
      if((constante < 0)){
         cout<<"\n === '1 => 2'";
         return FIRST_IMPLY_SECOND;
         }
      cout<<"\n === '2 => 1'";
      return SECOND_IMPLY_FIRST;
      }
   if(__this[0]!=__ineq[0] && __this[0]==0){ // this is an equality ineq is an inequality
      if(constante >= 0){
         cout<<"\n === ALWAYS_TRUE";
         return ALWAYS_TRUE;
         }
      cout<<"\n === ALWAYS_FALSE";
      return ALWAYS_FALSE;
      }

   if(__this[0]!=__ineq[0] && __ineq[0]==0){ // ineq is an equality this is an inequality
      if(constante > 0){
         cout<<"\n === ALWAYS_FALSE";
         return ALWAYS_FALSE;
         }
      cout<<"\n === '2 => 1'";
      return SECOND_IMPLY_FIRST;
      }
   cout<<"\n === No Simplif";
   return NO_SIMPLIFICATION;
/*   Serialized_Expression * __this = GetFromNormalizedADaAnInequation(this,var,param);
   Serialized_Expression * __ineq = GetFromNormalizedADaAnInequation(ineq,var,param);
   return __this->Imply(__ineq, _1_2);*/
   }

cout<<"\nInequation::SimplifyWith .... not completed yet\n";
throw(10);
/*if(GetPredicate() == FADA_NEQ && ineq->GetPredicate() == FADA_NEQ){
   Inequation* __this1,*__this2;
   Inequation* __ineq1,*__ineq2;

   __this1=new Inequation(GetLHS(),FADA_GREATER, GetRHS());
   __this2=new Inequation(GetLHS(),FADA_LESS, GetRHS());
   __ineq1=new Inequation(ineq->GetLHS(),FADA_LESS, ineq->GetRHS());
   __ineq2=new Inequation(ineq->GetLHS(),FADA_GREATER, ineq->GetRHS());

   if(__this1->SimplifyWith(__ineq1,param,_1_2))
      return true;
   if(__this1->SimplifyWith(__ineq2,param,_1_2))
      return true;
   if(__this2->SimplifyWith(__ineq1,param,_1_2))
      return true;
   if(__this2->SimplifyWith(__ineq2,param,_1_2))
      return true;


   return false;
   }

if(GetPredicate() == FADA_NEQ ){
   Inequation* __this1,*__this2;


   __this1=new Inequation(GetLHS(),FADA_GREATER, GetRHS());
   __this2=new Inequation(GetLHS(),FADA_LESS, GetRHS());

   if(__this1->SimplifyWith(ineq,var,param,_1_2))
      return true;


   if(__this2->SimplifyWith(ineq,var,param,_1_2))
      return true;

   return false;
   }

if(ineq->GetPredicate() == FADA_NEQ ){

   Inequation* __ineq1,*__ineq2;

   __ineq1=new Inequation(ineq->GetLHS(),FADA_LESS, ineq->GetRHS());
   __ineq2=new Inequation(ineq->GetLHS(),FADA_GREATER, ineq->GetRHS());


   if(this->SimplifyWith(__ineq1,var,param,_1_2))
      return true;


   if(this->SimplifyWith(__ineq2,var,param,_1_2))
      return true;

   return false;
   }*/
}
///////////////////////////////////////////
///////////////////////////////////////////
int
Inequation::SimplifyWith(vector<Inequation*>* __ineq,vector<string>*param){
cout<<"\nInequation::SimplifyWith===============================\n";
cout<<"\nInequations: "<<PrintInequations(__ineq);
cout<<"\nInequation: "<<Print_str();

for(vector<Inequation*>::iterator it=__ineq->begin(); it != __ineq->end(); it++){
   switch((*it)->SimplifyWith(this,param)){
      case FIRST_IMPLY_SECOND:
      case ALWAYS_TRUE:cout<<"\n ALWAYS TRUE \n";return ALWAYS_TRUE;
      case ALWAYS_FALSE:cout<<"\n ALWAYS FALSE\n";return ALWAYS_FALSE;
      }
   }
cout<<"\n NO SIMPLIF \n";
return NO_SIMPLIFICATION;
}
///////////////////////////////////////////
///////////////////////////////////////////

int
Inequation::SimplifyWith(vector<vector<Inequation*> >* __ineq,vector<string>*param){
int nb_falses=0;
for(vector<vector<Inequation*> >::iterator it=__ineq->begin(); it != __ineq->end(); ++it){
   switch(SimplifyWith(&(*it),param)){
      case ALWAYS_TRUE: return ALWAYS_TRUE;
      case ALWAYS_FALSE:nb_falses++;break;
      }
   }
if(nb_falses == __ineq->size())
   return ALWAYS_FALSE;
return NO_SIMPLIFICATION;
}
///////////////////////////////////////////
///////////////////////////////////////////
bool
Inequation::Imply(Inequation* ineq,vector<string> * var, vector<string> * param, bool* _1_2){
map<string,int> __map_var,__map_param;
int i=0;
for(vector<string>::iterator it=var->begin(); it != var->end(); ++it,++i)
   __map_var[*it]=i;
i=0;
for(vector<string>::iterator it=param->begin(); it != param->end(); ++it,++i)
   __map_param[*it]=i;

if(GetPredicate() != FADA_NEQ && ineq->GetPredicate() != FADA_NEQ){
   vector<int> __this=this->ToVector(var,&__map_var,param,&__map_param);
   vector<int> __ineq=ineq->ToVector(var,&__map_var,param,&__map_param);
   if(__this.size() != __ineq.size()){
      cout<<"\nInequation::Imply .... inappropriate case \n";
      throw(10);
      }
   for(i=1; i<__this.size()-1; i++)
      if(__this[i]-__ineq[i] != 0)
         return false;
   int constante=__this[__this.size()]-__ineq[__this.size()];
   if(__this[0]==__ineq[0] && __this[0]==0){ //equalities
      if(constante != 0) return false;
      *_1_2=true;
      return true;
      }
   if(__this[0]==__ineq[0] && __this[0]==1){ //equalities
      *_1_2=!(constante >= 0);
      return true;
      }
   if(__this[0]!=__ineq[0] && __this[0]==0){ // this is an equality
      *_1_2=(constante >= 0);
      return true;
      }
   return false;
/*   Serialized_Expression * __this = GetFromNormalizedADaAnInequation(this,var,param);
   Serialized_Expression * __ineq = GetFromNormalizedADaAnInequation(ineq,var,param);
   return __this->Imply(__ineq, _1_2);*/
   }
if(GetPredicate() == FADA_NEQ && ineq->GetPredicate() == FADA_NEQ){
   Inequation* __this1,*__this2;
   Inequation* __ineq1,*__ineq2;

   __this1=new Inequation(GetLHS(),FADA_GREATER, GetRHS());
   __this2=new Inequation(GetLHS(),FADA_LESS, GetRHS());
   __ineq1=new Inequation(ineq->GetLHS(),FADA_LESS, ineq->GetRHS());
   __ineq2=new Inequation(ineq->GetLHS(),FADA_GREATER, ineq->GetRHS());

   if(__this1->Imply(__ineq1,var,param,_1_2))
      return true;
   if(__this1->Imply(__ineq2,var,param,_1_2))
      return true;
   if(__this2->Imply(__ineq1,var,param,_1_2))
      return true;
   if(__this2->Imply(__ineq2,var,param,_1_2))
      return true;


   return false;
   }

if(GetPredicate() == FADA_NEQ ){
   Inequation* __this1,*__this2;


   __this1=new Inequation(GetLHS(),FADA_GREATER, GetRHS());
   __this2=new Inequation(GetLHS(),FADA_LESS, GetRHS());

   if(__this1->Imply(ineq,var,param,_1_2))
      return true;


   if(__this2->Imply(ineq,var,param,_1_2))
      return true;

   return false;
   }

if(ineq->GetPredicate() == FADA_NEQ ){

   Inequation* __ineq1,*__ineq2;

   __ineq1=new Inequation(ineq->GetLHS(),FADA_LESS, ineq->GetRHS());
   __ineq2=new Inequation(ineq->GetLHS(),FADA_GREATER, ineq->GetRHS());


   if(this->Imply(__ineq1,var,param,_1_2))
      return true;


   if(this->Imply(__ineq2,var,param,_1_2))
      return true;

   return false;
   }

}

///////////////////////////////////////////
///////////////////////////////////////////

void
Inequation::Instanciate(int __id, vector<Expression*>* __iter){
if(IsValue())
   return;
GetLHS()->Instanciate(__id, __iter);
GetRHS()->Instanciate(__id, __iter);
return;
}

///////////////////////////////////////////
///////////////////////////////////////////
Condition*
Inequation::WithoutNEQ(void){
if(IsValue() || !IsValue() && GetPredicate() != FADA_NEQ)
   return new Condition(this);

return new Condition
   (
   new Condition
      (
      new Inequation
         (
         GetLHS(),
         FADA_GREATER,
         GetRHS()
         )
      )
   ,FADA_OR
   ,new Condition
      (
      new Inequation
         (
         GetRHS(),
         FADA_GREATER,
         GetLHS()
         )
      )
   );
}

////////////////////////////////////////////
////////////////////////////////////////////
string 
Inequation::PrintAll_str(vector<string>* var, vector<string>* param){
if(IsValue( ))
   if(GetValue( ))
      return "true";
   else
      return "false";
string result;
result = GetLHS( )->PrintAll_str(var, param);
switch(GetPredicate( ))
   {
   case FADA_EQ      : result += " == "; break;
   case FADA_NEQ      : result += " != "; break;
   case FADA_LESS      : result += " < "; break;
   case FADA_LESS_EQ   : result += " <= "; break;
   case FADA_GREATER   : result += " > "; break;
   case FADA_GREATER_EQ   : result += " >= "; break;
   default: cerr<<endl<<"'Inequation::Generate_C_Code(void) ==> fatal error, invalide predicate";
      throw(BAD_OPERATOR);
      exit(0);
   }
result += GetRHS( )->PrintAll_str(var,param);
return result;   
}

////////////////////////////////////////////
////////////////////////////////////////////

Expression*
Inequation::UpperBound(string __lc){
if(!GetLHS()->IsLeaf())
   return NULL;
if(!GetLHS()->IsVariable() || GetLHS()->IsVariable() && GetLHS()->GetVariableName() != __lc)
   return NULL;
if(GetPredicate() != FADA_LESS_EQ && GetPredicate() != FADA_LESS)
   return NULL;
if(GetPredicate() == FADA_LESS_EQ)
   return GetRHS();
return new Expression(GetRHS(),FADA_SUB,new Expression(1));
}
////////////////////////////////////////////
////////////////////////////////////////////

bool
Inequation::IsFuzzy(set<string>* fuzzy_variables){
if(IsValue())   return false;
else return GetRHS()->IsFuzzy(fuzzy_variables) || GetLHS()->IsFuzzy(fuzzy_variables);
}
////////////////////////////////////////////
Inequation*
Inequation::PIPize(vector<string>* new_var, vector<Inequation*>* new_ineq){
if(IsValue())
   return this;
return new Inequation(GetLHS()->PIPize(new_var,new_ineq),GetPredicate(),GetRHS()->PIPize(new_var,new_ineq));
}
////////////////////////////////////////////
Condition
Inequation::operator&&(Inequation*i){
return *(new Condition(new Condition(this)),FADA_AND,new Condition(i));
}

Condition
Inequation::operator&&(Inequation &i){
return *(new Condition(new Condition(this)),FADA_AND,new Condition(&i));
}
Condition
Inequation::operator||(Inequation *i){
return *(new Condition(new Condition(this)),FADA_OR,new Condition(i));
}
Condition
Inequation::operator||(Inequation &i){
return *(new Condition(new Condition(this)),FADA_OR,new Condition(&i));
}




}



