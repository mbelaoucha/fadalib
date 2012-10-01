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



#include "condition.h"
#include "solver.h"
#include <vector>
#include <algorithm>

#include "constants.h"


namespace fada{

vector<Inequation*>   GenerateConstrainsForNewConfigutation(map<string, string> __mapping)
{
map<string, string>::iterator it=__mapping.begin();
vector<Inequation*>   result;
for(int i=0; i< __mapping.size(); ++i)
   result.push_back(new Inequation( new Expression(it->first),FADA_EQ, new Expression(it->second)));
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition::Condition()
{
Initialize( );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


Condition::~Condition()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition::Condition(Inequation* ineq)
{
Initialize( );
is_leaf=true;
SetInequation( ineq );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition::Condition(Condition* cond)
{
is_leaf=cond->is_leaf;
SetInequation( cond->GetInequation( ) );
SetLeftChild ( cond->GetLeftChild ( ) );
SetRightChild( cond->GetRightChild( ) );
SetOperation ( cond->GetOperation ( ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition::Condition(Condition* left,T_Logical_operator op,  Condition* right)
{
Initialize( );
is_leaf = false;
SetOperation( op );
SetLeftChild( left );
SetRightChild( right );
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void      
Condition::Set( Condition* left, T_Logical_operator op, Condition* right)
{
Initialize( );
is_leaf=false;
SetOperation( op );
SetLeftChild( left );
SetRightChild( right );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Condition::Set(Condition* cond)
{
is_leaf=cond->is_leaf;
SetInequation( cond->GetInequation( ) );
SetLeftChild ( cond->GetLeftChild ( ) );
SetRightChild( cond->GetRightChild( ) );
SetOperation ( cond->GetOperation ( ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Condition::Set(Inequation* ineq)
{
Initialize( );
is_leaf=true;
SetInequation( ineq );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Condition::Initialize(void)
{
SetInequation( NULL );
SetLeftChild ( NULL );
SetRightChild( NULL );
is_leaf=false;
SetOperation( FADA_NOLP );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
Condition::Clone (void)
{
if(IsLeaf( ))
   return new Condition(GetInequation( )->Clone( ));
return new Condition( GetLeftChild( ),GetOperation( ),GetRightChild( ));
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
Condition::Copy (void)
{
if(IsLeaf( ))
   return new Condition(GetInequation( )->Copy( ));

if(GetOperation() == FADA_NOT)
   return new Condition(GetLeftChild( )->Copy ( ),GetOperation( ), NULL );
return new Condition(GetLeftChild( )->Copy ( ),GetOperation( ), GetRightChild( )->Copy( ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Condition::Print    ( )
{
cout<<Print_str();
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

string
Condition::Print_str( )
{
if(IsLeaf( ))
   return GetInequation( )->Print_str( );


if(GetOperation( ) == FADA_NOT)
   return "Not ("+GetLeftChild( )->Print_str()+")";

string result;

result = "(";
result+= GetLeftChild( )->Print_str();
switch(GetOperation( ))
   {
   case FADA_AND:  result+= ") and ("; break;
   case FADA_OR :  result+= ") or ("; break;

   default:
      {
      cerr << "Condition::Print_str ==> Invalide operator" << endl;
      throw(BAD_OPERATOR);
      exit(1);
      }
   }
result += GetRightChild( )->Print_str();
result += ")";

return result;

}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
Condition::DNForm(void)
{
if(IsLeaf( ))                     return   Clone( );
if(GetRightChild( )->IsLeaf( ) && GetLeftChild( )->IsLeaf( ) )   return  Clone ( );

Condition    *left,*right;
left=GetLeftChild( )->DNForm( );
right=GetRightChild( )->DNForm( );
if(IsAND( ))
   return left->DistributeNormalizedCondition(right);

if(IsOR( ))
   return new Condition ( left,GetOperation( ), right);

cerr<<"\n\nCondition::DNForm(void), fatal error, unhandled operator \n"; 
throw(BAD_OPERATOR);
exit(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
Condition::DistributeNormalizedCondition(Condition* cond)
{
if(IsTermInNormalizedCondition())
   return DistributeTermByNormalizedCondition(cond);

return new Condition(   
            GetLeftChild( )->DistributeNormalizedCondition(cond),
            GetOperation( ),
            GetRightChild( )->DistributeNormalizedCondition(cond)
            );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
Condition::DistributeTermByNormalizedCondition(Condition* cond)
{
if(cond->IsTermInNormalizedCondition())
   {
     Condition *bidon,*Term= new Condition(this,FADA_AND,cond);
//   return (Terme->SerializeTerm(&bidon))->NormalizeTerm();
   return Term;
   };
return new Condition (
         this->DistributeTermByNormalizedCondition(cond->GetLeftChild( )),
         cond->GetOperation( ),
         this->DistributeTermByNormalizedCondition(cond->GetRightChild( ))
         );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


vector<Condition*> 
Condition::GetTerms(void)
{
vector<Condition*>   result, local;
if(IsTermInNormalizedCondition())
   {
   result.push_back(this);
   return result;
   }
result = GetLeftChild( )->GetTerms( );
local  = GetRightChild()->GetTerms( );
for(vector<Condition*>::iterator   it=local.begin(); it !=local.end(); ++it)
   result.push_back(*it);

return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

vector<Condition*>
Condition::get_terms(void)
{
if(IsTermInNormalizedCondition())
   {
   vector<Condition*> res;
   res.push_back(this);
   return res;
   }
vector<Condition*> v1=GetLeftChild ( )->get_terms( );
vector<Condition*> v2=GetRightChild( )->get_terms( );
vector<Condition*>::iterator it=v2.begin();
while(it != v2.end())
   {
   v1.push_back(*it);
   it++;
   }
return v1;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

vector<Inequation*>
Condition::GetTermInequations(void)
{
vector<Inequation*>   result;
if(IsLeaf( ))
   {
   result.push_back(GetInequation( ));
   return   result;
   }

result=GetLeftChild( )->GetTermInequations( );
vector<Inequation*>   local;
local=GetRightChild( )->GetTermInequations( );
for(vector<Inequation*>::iterator it=local.begin(); it != local.end(); ++it)
   result.push_back( *it );
return result;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void   SortInequations(vector<Inequation*>* all, vector<Inequation*>* affine, vector<Inequation*>* not_affine, vector<string>* var, vector<string>* param)
{
affine->clear();
not_affine->clear();
for(vector<Inequation*>::iterator it= all->begin(); it != all->end(); ++it)
   {
   if((*it)->IsAffine(var, param))
      affine->push_back(*it);
   else
      not_affine->push_back(*it);
   }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Condition::Substitute(map<string, string>* __mapping)
{
if(IsLeaf( )) 
   {
   GetInequation( )->Substitute( __mapping );
   return;
   }
GetLeftChild( )->Substitute( __mapping  );
if(GetOperation() != FADA_NOT)
   GetRightChild( )->Substitute( __mapping );
return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
Condition::Substitute(map<string, Expression*>* __mapping){
if(IsLeaf())
   return new Condition(GetInequation()->Substitute (__mapping));
if(GetOperation() == FADA_NOT)
   return new Condition(GetLeftChild()->Substitute(__mapping), GetOperation(), NULL);
return new Condition(GetLeftChild()->Substitute(__mapping), GetOperation(), GetRightChild()->Substitute(__mapping));
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
Condition::FastNegation(void)
{
return new Condition(this,FADA_NOT,  NULL);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
Condition*
Condition::EliminateNotOperations(bool smart)
{
if(IsLeaf( ))
   return this->GetInequation()->ToCondition();;

switch(GetOperation( ))
      {
      case FADA_NOT:return GetLeftChild( )->Negate(smart );
      case FADA_AND:break;
      case FADA_OR :break;
      default :
         cerr<<"Condition::EliminateNotOperations( ), fatal error, unhandled operator"<<endl;
         throw(BAD_OPERATOR);
         exit(EXIT_FAILURE);
      }
return new Condition(GetLeftChild( )->EliminateNotOperations( smart ), GetOperation( ), GetRightChild( )->EliminateNotOperations( smart ));
}

/////////////////////////////////////////////////////////////////////////////////////////////
Condition*
Condition::EliminateNotOperations(void)
{
if(IsLeaf( ))
   return this->GetInequation()->ToCondition();;

T_Logical_operator   new_op;
switch(GetOperation( ))
      {
      case FADA_NOT:return GetLeftChild( )->Negate( );
      case FADA_AND:break;
      case FADA_OR :break;
      default : cerr<<"Condition::EliminateNotOperations( ), fatal error, unhandled operator"<<endl;
         throw(BAD_OPERATOR);
            exit(0);
      }
return new Condition(GetLeftChild( )->EliminateNotOperations( ), GetOperation( ), GetRightChild( )->EliminateNotOperations( ));
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
Condition*
Condition::Negate(bool smart){
if(IsLeaf( ))
   return new Condition(GetInequation( )->Negate( smart ));

T_Logical_operator   new_op=FADA_NOLP;
switch(GetOperation( ))
      {
      case FADA_NOT:return GetLeftChild( )->EliminateNotOperations( smart );
      case FADA_AND:new_op=FADA_OR; break;
      case FADA_OR :new_op=FADA_AND; break;
      default : cerr<<"Condition::Negate( ), fatal error, unhandled operator"<<endl;throw(BAD_OPERATOR);exit(EXIT_FAILURE);
      }
return new Condition(GetLeftChild( )->Negate(smart),new_op,  GetRightChild( )->Negate( smart));

}

/////////////////////////////////////////////////////////////////////////////////////////////
Condition*
Condition::Negate( )
{
if(IsLeaf( ))
   return new Condition(GetInequation( )->Negate( ));

T_Logical_operator   new_op=FADA_NOLP;
switch(GetOperation( ))
      {
      case FADA_NOT:return GetLeftChild( )->EliminateNotOperations( );
      case FADA_AND:new_op=FADA_OR; break;
      case FADA_OR :new_op=FADA_AND; break;
      default : cerr<<"Condition::Negate( ), fatal error, unhandled operator"<<endl;throw(BAD_OPERATOR);exit(0);
      }
return new Condition(GetLeftChild( )->Negate( ),new_op,  GetRightChild( )->Negate( ));
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
bool
Condition::IsValue(bool __val)
{
return (IsLeaf( ) && GetInequation( )->IsValue( ) && GetInequation( )->GetValue( ) == __val );
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void
Condition::ReferencedScalars(set<string>* scalars)
{
if(IsLeaf( )){
    GetInequation( )->ReferencedScalars( scalars );
   return;
   }

GetLeftChild( )->ReferencedScalars( scalars );
GetRightChild( )->ReferencedScalars( scalars );
return ;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

string
Condition::Generate_C_Code(void)
{

if(IsLeaf( ))
   return GetInequation( )->Generate_C_Code( );

string result;

result = "(";
result+= GetLeftChild( )->Generate_C_Code();
switch(GetOperation( ))
   {
   case FADA_AND: result+= ") && ("; break;
   case FADA_OR:  result+= ") || ("; break;
   default:
      {
      cerr << "Condition::Print_str ==> Invalide operator" << endl;
      throw(BAD_OPERATOR);
      exit(1);
      }
   }
result += GetRightChild( )->Generate_C_Code();
result += ")";

return result;
}

////////////////////////////////////////////
////////////////////////////////////////////

Condition*
Condition::NegateTermANDDestributeNotOperations(void){
if(IsLeaf()){
   return this->FastNegation();
   }
if(! IsAND()){
   cerr<<"\nCondition::NegateTermANDDestributeNotOperations,  condition must be a conjunction...\n";
   throw(INAPPROPRIATE_CASE);
   exit(0);
   }

return new Condition(   GetLeftChild()->NegateTermANDDestributeNotOperations(), 
            FADA_OR, 
            GetRightChild()->NegateTermANDDestributeNotOperations()
         );
}
////////////////////////////////
////////////////////////////////

void
Condition::Instanciate(int __id,vector<Expression*>* __iter){

if(IsLeaf()){
   GetInequation()->Instanciate(__id, __iter);
   return;
   }
if(IsNot()){
   GetLeftChild()->Instanciate(__id, __iter);
   return;
   }
GetRightChild()->Instanciate(__id, __iter);
GetLeftChild()->Instanciate(__id, __iter);
return;
   
   
}
////////////////////////////////
////////////////////////////////

vector<Condition*>
Condition::SplitToTerms(void){
Condition* result;
result=DNForm();
return result->GetTerms();
}

////////////////////////////////
////////////////////////////////

Condition*
Condition::TraduceNEQInequations(void){
if(IsLeaf())
   return GetInequation()->WithoutNEQ();
return new Condition(GetLeftChild()->TraduceNEQInequations(), GetOperation(), GetRightChild()->TraduceNEQInequations());
}

////////////////////////////////
////////////////////////////////
string
Condition::PrintAll_str(vector<string>* var, vector<string>* param){
if(IsLeaf( ))
   return GetInequation( )->PrintAll_str(var,param);


if(GetOperation( ) == FADA_NOT)
   return "Not ("+GetLeftChild( )->PrintAll_str(var,param)+")";

string result;

result = "(";
result+= GetLeftChild( )->PrintAll_str(var,param);
switch(GetOperation( ))
   {
   case FADA_AND:  result+= ") and ("; break;
   case FADA_OR :  result+= ") or ("; break;

   default:
      {
      cerr << "Condition::Print_str ==> Invalide operator" << endl;
      throw(BAD_OPERATOR);
      exit(1);
      }
   }
result += GetRightChild( )->PrintAll_str(var,param);
result += ")";

return result;
}

};




