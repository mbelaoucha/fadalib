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


#include <algorithm>
#include <agc.h>
#include "expression.h"
#include "global.h"
#include "latex.h"


// extern AGC ____myGC;
namespace fada{
Expression::Expression()
{
Initialize( );
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

Expression::~Expression()
{
____myGC.Trace(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

vector<int>
Expression::ToVector(vector<string>* c,map<string,int>* counters,vector<string>*p, map<string,int>* parameters){

int nb_counters = counters->size();
int nb_parameters = parameters->size();

int org_counter = 0;
int org_parameter = nb_counters;
int org_constant = nb_counters+nb_parameters;

int size = nb_counters + nb_parameters + 1;

vector<int> result(size);

if(this->IsLeaf()){
   if(this->IsValue()){
      result[org_constant]=this->GetValue();
      return result;
      }

   if(this->IsVariable()){
      if(find(c->begin(),c->end(),this->GetVariableName()) != c->end()){
         result[org_counter + counters->operator[](GetVariableName()) ] = 1;
         return result;
         }
      if(find(p->begin(), p->end(),this->GetVariableName()) != p->end()){
         result[org_parameter + parameters->operator[](GetVariableName()) ] = 1;
         return result;
         }
      }
   cerr<<"\nExpression::ToVector ... fatal error : leaf is not a counter, not a parameter and not an integer value '"<<Print_str()<<"'\n";
   throw(NON_AFFINE_ENTITY);
   exit(0);
   }
else
   {
   vector<int> fg_vector = this->GetLeftChild()->ToVector(c,counters,p, parameters);
   vector<int> fd_vector = this->GetRightChild()->ToVector(c,counters,p, parameters);

   switch(this->GetOperation())
      {
      case FADA_ADD:
         {
         for(int i=0; i<fg_vector.size(); i++)
            result[i] = fg_vector[i]+fd_vector[i];
         break;
         }
      case FADA_SUB:
         {
         for(int i=0; i<fg_vector.size(); i++)
            result[i] = fg_vector[i]-fd_vector[i];
         break;
         }
      case FADA_MUL:
         {
         if(this->GetLeftChild()->IsValue())
            {
            for(int i=0; i<fg_vector.size(); i++)
               result[i] = this->GetLeftChild()->GetValue()*fd_vector[i];
            break;
            }
         if(this->GetRightChild()->IsValue())
            {
            for(int i=0; i<fg_vector.size(); i++)
               result[i] = fg_vector[i]*this->GetRightChild()->GetValue();
            break;
            }

         cerr<<"\nExpression::ToVector ... fatal error : non affine expression\n";
         throw(NON_AFFINE_EXPRESSION);
         exit(1);
         }
      }
   }

return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void
Expression::Initialize( )
{
this->is_leaf=true;
this->t_leaf=FADA_value;
this->value=-1;
right__child=NULL;
left__child=NULL;
FuncArg_ArrayIndex.clear();
name="";
// MyCollector->Trace(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
Expression::Expression(Expression* e)
{
if(e->IsLeaf( ))
   {
   if(e->IsValue( ))
      Set(e->GetValue( ));
   if(e->IsVariable( ))
      Set(e->GetVariableName( ));
   if(e->IsFunction( ))
      Set(FADA_function, e->GetFunctionName( ), e->GetArguments( ));
   if(e->IsArray( ))
      Set(FADA_array, e->GetArrayName( ), e->GetIndex( ));
   Set(e->GetReadStmt(), e->GetIteration());
   int l,c;
   e->GetLocation(&l,&c);
   SetLocation(l,c);
   }
else
   {
   Set(e->GetLeftChild( ), e->GetOperation( ), e->GetRightChild( ));
   int l,c;
   e->GetLocation(&l,&c);
   SetLocation(l,c);
   }
return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
Expression::Expression(int val)
{
Initialize( );
Set(val);
} 

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

Expression::Expression( string var)
{
Initialize( );
Set( var );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


Expression::Expression( T_leaf type, string str, vector<Expression*>* vec)
{
Initialize( );
Set(type, str, vec );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Expression::Set(int __val)
{
is_leaf=true;
t_leaf=FADA_value;
value = __val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Expression::Set(string __name)
{
is_leaf=true;
t_leaf=FADA_variable;
name=__name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Expression::Set(T_leaf __leaf_kind, string __name, vector<Expression*>* __index)
{
is_leaf=true;
t_leaf=__leaf_kind;
name = __name;
SetArguments( __index );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Expression::Set(Expression* l_c,T_operation op, Expression* r_c)
{
is_leaf = false;
operation=op;
SetLeftChild( l_c );
SetRightChild( r_c );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

Expression::Expression( Expression* l_c, T_operation op, Expression* r_c)
{
Initialize( );
Set(l_c,op,r_c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

Expression*
Expression::Clone( )
{
return new Expression(this); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

Expression*
Expression::Copy( )
{
if(IsLeaf( ))
   return   new Expression(this);
return new Expression(GetLeftChild( )->Copy( ),operation,GetRightChild( )->Copy( ));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void
Expression::Print(void)
{
cout<<Print_str( );   
}


string
Expression::Full_Tex_Print_str(vector<string>* var, vector<string> *param){
ostringstream sout;

if(IsLeaf( ))
   {
   if(IsValue( ))   
      {
      sout<<GetValue( );
      return sout.str();
      }
   if(IsVariable( ))
      {
      if((find(var->begin(), var->end(),GetVariableName()) != var->end()) || (find(param->begin(), param->end(), GetVariableName()) !=param->end()))
         return GetVariableName( );

      sout<<GetVariableName()<<PrintTexOperation(GetReadStmt(),GetIteration());

      return sout.str();
      }
   if(IsFunction( ))
      {
      sout<<GetFunctionName( )<<" (";
      
      for(vector<Expression* >::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->Full_Tex_Print_str(var,param );
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" , ";
         }
      sout<<")";

      return sout.str();
      }
   if(IsArray( ))
      {
      sout<<GetArrayName( )<<PrintTexOperation(GetReadStmt(),GetIteration())<<" [ ";
         for(vector<Expression*>::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->Full_Tex_Print_str(var,param);
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" ][ ";
         }
      sout<<"]";
      return sout.str();
      }
   
   }


if(!GetLeftChild( )->IsLeaf( ))    sout << "(";
sout<<GetLeftChild( )->Full_Tex_Print_str( var, param);
if(!GetLeftChild( )->IsLeaf( ))   sout << ")";

switch(operation)
   {
   case FADA_ADD:  sout << "+"; break;
   case FADA_SUB: sout << "-"; break;
   case FADA_MUL: sout << "*"; break;
   case FADA_DIV: sout << "/"; break; //with newparms
   default:cerr<<endl<<"Expression::Print(void)  unknown operator "<<endl;
      cerr<<"\nLHS = "<<GetLeftChild( )->PrintAll_str( var,param);
      cerr<<"\nRHS = "<<GetRightChild( )->PrintAll_str(var,param )<<"\n";
      throw(BAD_OPERATOR);
      exit(0);
   }
if(!GetRightChild( )->IsLeaf( ))   sout << "(";
sout<<GetRightChild( )->Full_Tex_Print_str( var, param);
if(!GetRightChild( )->IsLeaf( ))   sout << ")";
return sout.str();
}
////////////////////////////////////////////////////
////////////////////////////////////////////////////
string   
Expression::PrintAll_str(vector<string>* var, vector<string>* param){
ostringstream sout;

if(IsLeaf( ))
   {
   if(IsValue( ))   
      {
      sout<<GetValue( );
      return sout.str();
      }
   if(IsVariable( ))
      {
      if((find(var->begin(), var->end(),GetVariableName()) != var->end()) || (find(param->begin(), param->end(), GetVariableName()) !=param->end()))
      return GetVariableName( );

      sout<<GetVariableName()<<PrintOperation(GetReadStmt(),GetIteration());

      return sout.str();
      }
   if(IsFunction( ))
      {
      sout<<GetFunctionName( )<<" (";
      
      for(vector<Expression* >::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->PrintAll_str(var,param );
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" , ";
         }
      sout<<")";

      return sout.str();
      }
   if(IsArray( ))
      {
      sout<<GetArrayName( )<<PrintOperation(GetReadStmt(),GetIteration())<<" [ ";
         for(vector<Expression*>::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->PrintAll_str(var,param);
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" ][ ";
         }
      sout<<"]";
      return sout.str();
      }
   
   }


if(!GetLeftChild( )->IsLeaf( ))    sout << "(";
sout<<GetLeftChild( )->PrintAll_str( var, param);
if(!GetLeftChild( )->IsLeaf( ))   sout << ")";

switch(operation)
   {
   case FADA_ADD:  sout << "+"; break;
   case FADA_SUB: sout << "-"; break;
   case FADA_MUL: sout << "*"; break;
   case FADA_DIV: sout << "/"; break; //with newparms
   default:cerr<<endl<<"Expression::Print(void)  unknown operator "<<endl;
      cerr<<"\nLHS = "<<GetLeftChild( )->PrintAll_str( var,param);
      cerr<<"\nRHS = "<<GetRightChild( )->PrintAll_str(var,param )<<"\n";
      throw(BAD_OPERATOR);
      exit(0);
   }
if(!GetRightChild( )->IsLeaf( ))   sout << "(";
sout<<GetRightChild( )->PrintAll_str( var, param);
if(!GetRightChild( )->IsLeaf( ))   sout << ")";
return sout.str();

}
////////////////////////////////////////////////////
////////////////////////////////////////////////////

string
Expression::Print_Tex_str(){
ostringstream sout;

if(IsLeaf( ))
   {
   if(IsValue( ))   
      {
      sout<<GetValue( );
      return sout.str();
      }
   if(IsVariable( ))
      {
      sout<<PreprocessVariableName(GetVariableName( ));

      return sout.str();
      }
   if(IsFunction( ))
      {
      sout<<PreprocessVariableName(GetFunctionName( ))<<" (";
      
      for(vector<Expression* >::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->Print_Tex_str( );
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" , ";
         }
      sout<<")";

      return sout.str();
      }
   if(IsArray( ))
      {
      sout<<PreprocessVariableName(GetArrayName( ))<<" [ ";
         for(vector<Expression*>::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->Print_Tex_str( );
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" ][ ";
         }
      sout<<"]";
      return sout.str();
      }
   
   }


if(!GetLeftChild( )->IsLeaf( ))    sout << "(";
sout<<GetLeftChild( )->Print_Tex_str( );
if(!GetLeftChild( )->IsLeaf( ))   sout << ")";

switch(operation)
   {
   case FADA_ADD:  sout << "+"; break;
   case FADA_SUB: sout << "-"; break;
   case FADA_MUL: sout << "*"; break;
   case FADA_DIV: sout << "/"; break; 
   case FADA_MOD: sout <<"%";break;
   default:cerr<<endl<<"Expression::Print(void)  unknown operator "<<endl;
      cerr<<"\nLHS = "<<GetLeftChild( )->Print_Tex_str( );
      cerr<<"\nRHS = "<<GetRightChild( )->Print_Tex_str( )<<"\n";
      throw(BAD_OPERATOR);
      exit(0);
   }
if(!GetRightChild( )->IsLeaf( ))   sout << "(";
sout<<GetRightChild( )->Print_Tex_str( );
if(!GetRightChild( )->IsLeaf( ))   sout << ")";
return sout.str();
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////

string
Expression::Print_str(void)
{
ostringstream sout;

if(IsLeaf( ))
   {
   if(IsValue( ))   
      {
      sout<<GetValue( );
      return sout.str();
      }
   if(IsVariable( ))
      {
      sout<<GetVariableName( );

      return sout.str();
      }
   if(IsFunction( ))
      {
      sout<<GetFunctionName( )<<" (";
      
      for(vector<Expression* >::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->Print_str( );
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" , ";
         }
      sout<<")";

      return sout.str();
      }
   if(IsArray( ))
      {
      sout<<GetArrayName( )<<" [ ";
         for(vector<Expression*>::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->Print_str( );
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" ][ ";
         }
      sout<<"]";
      return sout.str();
      }
   
   }


if(!GetLeftChild( )->IsLeaf( ))    sout << "(";
sout<<GetLeftChild( )->Print_str( );
if(!GetLeftChild( )->IsLeaf( ))   sout << ")";

switch(operation)
   {
   case FADA_ADD:  sout << "+"; break;
   case FADA_SUB: sout << "-"; break;
   case FADA_MUL: sout << "*"; break;
   case FADA_DIV: sout << "/"; break; 
   case FADA_MOD: sout <<"%";break;
   default:cerr<<endl<<"Expression::Print(void)  unknown operator "<<endl;
      cerr<<"\nLHS = "<<GetLeftChild( )->Print_str( );
      cerr<<"\nRHS = "<<GetRightChild( )->Print_str( )<<"\n";
      throw(BAD_OPERATOR);
      exit(0);
   }
if(!GetRightChild( )->IsLeaf( ))   sout << "(";
sout<<GetRightChild( )->Print_str( );
if(!GetRightChild( )->IsLeaf( ))   sout << ")";
return sout.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

Expression*
Expression::Normalize(void)
{
if(IsLeaf( ))
   return Clone( );
if(GetLeftChild( )->IsLeaf( ) && GetRightChild( )->IsLeaf( ))
   return Clone( );

Expression   *new_left_child,*new_right_child;
new_left_child=GetLeftChild( )->Normalize( );
new_right_child=GetRightChild( )->Normalize( );

if(IsMultiplication( ))
   return new_right_child->MultiplyNormalizedExpressions( new_left_child );

return new Expression( new_left_child, operation, new_right_child);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

Expression*
Expression::MultiplyNormalizedExpressions(Expression* E)
{
if(IsTermInNormalizedFormula( ))
   return MultiplyTermByNormalizedExpression( E );

return new Expression(GetLeftChild( )->MultiplyNormalizedExpressions( E), operation, GetRightChild( )->MultiplyNormalizedExpressions( E));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

Expression*
Expression::MultiplyTermByNormalizedExpression( Expression* E)
{

if(E->IsTermInNormalizedFormula( ))
   {
   Expression *notseful,*Term= new Expression(this,FADA_MUL,E);
   return (Term->SerializeTerm(&notseful))->NormalizeTerm( );
   }
return new Expression( MultiplyTermByNormalizedExpression( E->GetLeftChild( )), operation, MultiplyTermByNormalizedExpression( E->GetRightChild( )));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
Expression*
Expression::SerializeTerm( Expression** LastInternNode)
{
//TODO : another way todo,   no need for "LastInterneNode", explore FD and traite all leafs in FG

if(this->IsLeaf())
   {
   *LastInternNode=NULL;
   return this->Clone();
   }
if(this->GetLeftChild()->IsLeaf() && this->GetRightChild()->IsLeaf())
   {
   *LastInternNode = this->Clone();
   return *LastInternNode;
   }
Expression *FG,*FD,*Lfg,*Lfd;
FG=this->GetLeftChild()->SerializeTerm(&Lfg);
FD=this->GetRightChild()->SerializeTerm(&Lfd);

if(Lfd==NULL)
   {
   *LastInternNode=new Expression(Lfg->GetLeftChild(),FADA_MUL,FD);
   Lfg->SetLeftChild(*LastInternNode);
   return FG;
   };
if(Lfg==NULL)
   {
   *LastInternNode=new Expression(FG,FADA_MUL, Lfd->GetLeftChild());
   Lfd->SetLeftChild( *LastInternNode);
   return FD;
   };

Expression* E=new Expression( FD, FADA_MUL,Lfg->GetLeftChild());
Lfg->SetLeftChild( E );
*LastInternNode=Lfd;
return FG;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


Expression* 
Expression::NormalizeTerm(void)
{
if(this->IsValue())
   return new Expression( new Expression(1),FADA_MUL,this);
if(this->IsLeaf())
   return new Expression( this,FADA_MUL, new Expression(1));

Expression*FG,*FD,*Val,*Others;
FG=this->GetLeftChild()->NormalizeTerm();
FD=this->GetRightChild()->NormalizeTerm();
Val=new Expression(FG->GetRightChild(),FADA_MUL,FD->GetRightChild());
Others=new Expression(FG->GetLeftChild(),FADA_MUL,FD->GetLeftChild());
Val->Simplify();
Others->Simplify();
return new Expression(Others,FADA_MUL,Val);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

Expression*
Expression::Simplify(void)
{
  Expression *current = this;
  Expression *simplified = one_pass_simplify();

  while(current->Print_str() != simplified->Print_str())
    //Fix-point iteration
    {
      current = simplified;
      simplified = current->one_pass_simplify();
    }

  return simplified; 

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool
Expression::operator ==(Expression& e)
{


  return (IsLeaf( ) && e.IsLeaf( ) &&
   ((IsValue( ) && e.IsValue( ) && GetValue( )==e.GetValue( ))
   || (IsVariable( ) && e.IsVariable( ) && GetVariableName( ) == e.GetVariableName( ))
   || (IsArray( ) && e.IsArray( ) && *GetArguments( )==*e.GetArguments( ))
   || (IsFunction( ) && e.IsFunction( ) && *GetArguments( ) == *e.GetArguments( ))))
   || ( !IsLeaf( ) && ! e.IsLeaf( ) && operation == e.operation && *GetLeftChild()==*e.GetLeftChild( ) && *GetRightChild( ) == * e.GetRightChild( ));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


Expression*
Expression::one_pass_simplify()
{

if(IsLeaf( ))   return Clone();


//if operation with immediate operandes, perform operation
if(this->GetLeftChild()->IsValue() && this->GetRightChild()->IsValue() && IsAddition( ))
    return new Expression(this->GetRightChild()->GetValue() + this->GetLeftChild()->GetValue());

if(this->GetLeftChild()->IsValue() && this->GetRightChild()->IsValue() && IsSubstraction( ))
   return new Expression(this->GetLeftChild()->GetValue()-this->GetRightChild()->GetValue());

if(this->GetLeftChild()->IsValue() && this->GetRightChild()->IsValue() && IsMultiplication( ))
   return new Expression(  this->GetLeftChild()->GetValue() * this->GetRightChild()->GetValue());



  //0+x = x+0 = x
if(IsAddition( ) &&this->GetLeftChild()->IsValue() && GetLeftChild( )->GetValue( ) == 0) return GetRightChild( )->one_pass_simplify();
if(IsAddition( ) && this->GetRightChild()->IsValue() &&GetRightChild( )->GetValue() == 0) return GetLeftChild( ) ->one_pass_simplify();

  // x-0 = x
if(IsSubstraction( ) &&this->GetRightChild()->IsValue() && GetRightChild( )->GetValue( ) == 0) return GetLeftChild( )->one_pass_simplify();


  //1*x = x*1 = x
if(IsMultiplication( ) &&this->GetLeftChild()->IsValue() && GetLeftChild( )->GetValue( ) == 1) return GetRightChild( )->one_pass_simplify();
if(IsMultiplication( ) &&this->GetRightChild()->IsValue() && GetRightChild( )->GetValue() == 1) return GetLeftChild( ) ->one_pass_simplify();

  //0*x = x*0 = 0
if(IsMultiplication( ) &&this->GetLeftChild()->IsValue() && GetLeftChild( )->GetValue( ) == 0) return new Expression(0);
if(IsMultiplication( ) && this->GetRightChild()->IsValue() && GetRightChild( )->GetValue() == 0) return new Expression(0);

 // N*(x/N) = x      ... PIP' new param
/*if(IsMultiplication() && GetLeftChild()->IsValue() && GetRightChild()->IsDivision() && GetRightChild()->GetRightChild()->IsValue() && GetLeftChild()->GetValue()%GetRightChild()->GetRightChild()->GetValue()==0){
   int coef=GetLeftChild()->GetValue()/GetRightChild()->GetRightChild()->GetValue();
   if (coef != 1)
      return new Expression(new Expression(coef),FADA_MUL,GetRightChild()->GetLeftChild());
   else
      return GetRightChild()->GetLeftChild();
   }*/
// (N*x)/N
if(IsDivision() && GetRightChild()->IsValue() && GetLeftChild()->IsMultiplication() && GetLeftChild()->GetLeftChild()->IsValue() && GetLeftChild()->GetLeftChild()->GetValue()%GetRightChild()->GetValue()==0){
   int coef=GetLeftChild()->GetLeftChild()->GetValue()/GetRightChild()->GetValue();
   if (coef != 1)
      return new Expression(new Expression(coef),FADA_MUL,GetLeftChild()->GetRightChild());
   else
      return GetLeftChild()->GetRightChild();
   }

// (x*N)/N   ?

// x - x
if(IsSubstraction( ) && GetRightChild()->IsVariable() && GetLeftChild()->IsVariable() && GetRightChild()->GetVariableName()==GetLeftChild()->GetVariableName() ) return new Expression(0);



  //TODO: x+(-1*y) = x-y

// if(IsMultiplication( ) && this->GetRightChild()->IsSubstraction( ) && this->GetRightChild()->GetLeftChild()->GetValue() == 1)
//    return new Expression(this->GetLeftChild(),FADA_SUB,this->GetRightChild()->GetRightChild());
// 
// if(IsSubstraction( ) && this->GetRightChild()->IsSubstraction( ) && this->GetRightChild()->GetLeftChild()->GetValue() == 1 )
//    return new Expression(this->GetLeftChild(), FADA_ADD, this->GetRightChild()->GetRightChild());
//  
//   //n*(x/n) = x (newparms)
// if(   IsMultiplication( ) && GetLeftChild( )->IsValue( ) &&
//    GetRightChild( )->IsDivision( ) &&
//    GetRightChild( )->GetRightChild( )->IsValue( ) &&
//    GetLeftChild ( )->GetValue( ) == GetRightChild( )->GetRightChild( )->GetValue( ))
//       {
//       cout << "newparm simplification ok" << endl;
//       return   GetRightChild( )->GetLeftChild( )->one_pass_simplify( ); 
//       }
// 
//   
//   //TODO: + and * combs

  return new Expression(GetLeftChild( )->one_pass_simplify(), operation, GetRightChild( )->one_pass_simplify());
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
Expression::IsAffine(vector<string>* var, vector<string>* param)
{
// bool bidon;
// if(IsTermInNormalizedFormula( ))
//    return IsAffineTerm(&bidon, var, param);
// return (GetLeftChild()->IsAffine(var, param) && GetRightChild()->IsAffine(var, param));

if(IsLeaf()){
   if(IsValue())
      return true;
   if(IsVariable())
      if(IsIn(GetVariableName( ), var) || IsIn( GetVariableName( ), param))
         return true;
   return false;
   }
if(IsAddition() || IsSubstraction())
   return GetRightChild()->IsAffine(var,param) && GetLeftChild()->IsAffine(var,param);

if(IsMultiplication())
   if(GetRightChild()->IsValue())
      return GetLeftChild()->IsAffine(var,param);
   else
      if(GetLeftChild()->IsValue())
         return GetRightChild()->IsAffine(var,param);
      else
         return false;
if(IsDivision() ||IsModulo())
   if(GetLeftChild()->IsValue())
      return GetRightChild()->IsAffine(var,param);
   else
      return false;

cout<<"\nExpression::IsAffine ...............UNHANDLED_CASE\n";
throw(10);
return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
Expression::IsAffineTerm(bool* Contain_LC_P, vector<string>* var, vector<string>* param)
{
if(IsLeaf( ))
   {
   if(IsValue())
      {
      *Contain_LC_P=false;
      return true;
      };

   if(IsVariable( ))
      {
      if(IsIn(GetVariableName( ), var) || IsIn( GetVariableName( ), param))
         {
         *Contain_LC_P=true;
         return true;
         }
      *Contain_LC_P=false;
      return false;
      }

   if(IsFunction( ) || IsArray( ))
      {
      *Contain_LC_P=false;
      return false;
      };
   cerr<<endl<<"Expression::IsAffineTerm, fatal error : unhandled case"<<endl;
   throw(UNHANDLED_CASE);
   exit(1);
   }


bool Affine1,Affine2=true, Contain1, Contain2;
Affine1=this->GetRightChild()->IsAffineTerm(&Contain1, var, param);
if(Affine1)
   Affine2=this->GetLeftChild()->IsAffineTerm(&Contain2, var, param);
return (Affine1 && Affine2 && ! (Contain1 && Contain2));

}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void
Expression::ReferencedScalars(set<string>* scalars)
{

if(IsLeaf( )){
   if(IsVariable( ) )
         scalars->insert(GetVariableName( ));
   if(IsArray() && IsFunction())
      for(vector<Expression*>::iterator it=GetArguments()->begin(); it != GetArguments()->end(); ++it)   
         (*it)->ReferencedScalars(scalars);
   return;
   }


GetRightChild()->ReferencedScalars( scalars );
GetLeftChild()->ReferencedScalars( scalars);
}

/////////////////////////////////////////////////////////////////////////////////////////////
Expression*
Expression::Substitute(map<string, Expression*>* __mapping,bool __use_less){
Expression* result;

if(IsLeaf( ) && IsVariable( ) )
   {
   if(__mapping->find(GetVariableName( )) != __mapping->end())
      return  __mapping->operator[](GetVariableName( ));
   return Clone();
   }
if(IsLeaf() && IsValue())
   return Clone();
if(IsLeaf()){
   cerr<<"\nExpression::Substitute (map<..>, bool) ... fatal error (inapropriate case)\n";
   throw(UNHANDLED_CASE);
   exit(EXIT_FAILURE);
   }
return new Expression (GetLeftChild( )->Substitute( __mapping,__use_less  ), GetOperation(), GetRightChild( )->Substitute( __mapping ,__use_less));
}
/////////////////////////////////////////////////////////////////////////////////////////////
Expression*
Expression::Substitute(map <string, Expression*>* __mapping)
{

Expression* result;

if(IsLeaf( ) && IsVariable( ) && (__mapping->find(GetVariableName( )) != __mapping->end()))
   {
   result= __mapping->operator[](GetVariableName( ));
   vector<Expression*> new_iter;
   for(vector<Expression*>::iterator it=GetIteration()->begin(); it != GetIteration()->end(); ++it)
      new_iter.push_back((*it)->Substitute(__mapping,true));
   result->Instanciate(GetReadStmt(), &new_iter);
   return result;
   }

if(IsLeaf() && (IsArray() || IsFunction())){
   result=this->Clone();
   vector<Expression*>   new_arg;
   for(vector<Expression*>::iterator it=GetArguments()->begin(); it != GetArguments()->end(); ++it)
      new_arg.push_back((*it)->Substitute(__mapping));
   result->SetArguments(&new_arg);
      if(IsArray()){
         vector<Expression*> new_iter;
      for(vector<Expression*>::iterator it=GetIteration()->begin(); it != GetIteration()->end(); ++it)
         new_iter.push_back((*it)->Substitute(__mapping,true));
      result->Instanciate(GetReadStmt(),&new_iter);
      }
   return result;
   }

if(!IsLeaf( ))
   return new Expression (GetLeftChild( )->Substitute( __mapping  ), GetOperation(), GetRightChild( )->Substitute( __mapping ));

return this;   // is_leaf && is_value
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void
Expression::Substitute(map<string, string>*  __mapping)
{
if(IsLeaf( ) && IsVariable( ) && (__mapping->find(GetVariableName( )) != __mapping->end()))
   {
   SetName( __mapping->operator[](GetVariableName( )) );
   return;
   }
if(IsLeaf() && (IsArray() || IsFunction())){
   for(vector<Expression*>::iterator it=GetArguments()->begin(); it != GetArguments()->end(); ++it)
      (*it)->Substitute(__mapping);

   for(vector<Expression*>::iterator it=GetIteration()->begin(); it != GetIteration()->end(); ++it)
      (*it)->Substitute(__mapping);

   return;
   }
if(!IsLeaf( ))
   {
   GetLeftChild( )->Substitute( __mapping  );
   GetRightChild( )->Substitute( __mapping );
   return;
   }
return;
}
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
string
Expression::Generate_C_Code(void)
{
ostringstream sout;

if(IsLeaf( ))
   {
   if(IsValue( ))   
      {
      sout<<GetValue( );
      return sout.str();
      }
   if(IsVariable( ))   
      return GetVariableName( );

   if(IsFunction( ))
      {
      sout<<GetFunctionName( )<<" (";
      
      for(vector<Expression* >::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->Print_str( );
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" , ";
         }
      sout<<")";
      return sout.str();
      }

   if(IsArray( ))
      {
      sout<<GetArrayName( )<<" [ ";
         for(vector<Expression*>::iterator it=GetArguments( )->begin( );it!=GetArguments( )->end( );++it)
         {
         sout<<(*it)->Generate_C_Code( );
         vector<Expression*>::iterator it2=it;
         ++it2;
         if(it2!=GetArguments( )->end( ))
            sout<<" ][ ";
         }
      sout<<"]";
      return sout.str();
      }
   }


if(!GetLeftChild( )->IsLeaf( ))    sout << "(";
sout<<GetLeftChild( )->Generate_C_Code( );
if(!GetLeftChild( )->IsLeaf( ))   sout << ")";

switch(operation)
   {
   case FADA_ADD:  sout << "+"; break;
   case FADA_SUB: sout << "-"; break;
   case FADA_MUL: sout << "*"; break;
   case FADA_DIV: sout << "/"; break; //with newparms
   default:cerr<<endl<<"ADaAn_Expression::Print(void)  unknown operator "<<endl;
      cerr<<"\nLHS = "<<GetLeftChild( )->Generate_C_Code( );
      cerr<<"\nRHS = "<<GetRightChild( )->Generate_C_Code( )<<"\n";
      throw(BAD_OPERATOR);
      exit(0);
   }
if(!GetRightChild( )->IsLeaf( ))   sout << "(";
sout<<GetRightChild( )->Generate_C_Code( );
if(!GetRightChild( )->IsLeaf( ))   sout << ")";
return sout.str();

}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void
Expression::Instanciate(int __id,vector<Expression*>* __iter){
if(IsLeaf()){
   Set(__id,__iter);
   if(IsValue() || IsVariable())   return;
   if(IsArray() || IsFunction()){
      for(vector<Expression*>::iterator it=GetArguments()->begin(); it != GetArguments()->end(); it++)
         (*it)->Instanciate(__id,__iter);
      return;
      }
   cerr<<"\nExpression::Instanciate() ..... FATAL ERROR (inapropriate case)\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }
this->GetLeftChild()->Instanciate(__id,__iter);
this->GetRightChild()->Instanciate(__id,__iter);
return;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Expression* 
Expression::PIPize(vector<string>* new_variables, vector<Inequation*>* new_inequations){
if(IsDivision() && GetRightChild()->IsValue()){   // E/nb <=> forall V, E-nb< nb*V <= E ... so: E/nb=V
   Expression*lhs=GetLeftChild()->PIPize(new_variables,new_inequations);
   ostringstream var1;
   var1<<"__tmp__"<<new_variables->size();
   new_variables->push_back(var1.str());

   Expression* nb_x_tmp=new Expression(GetRightChild(), FADA_MUL, new Expression(var1.str()));
   Inequation*ineq1=new Inequation(new Expression(lhs,FADA_SUB,GetRightChild()), FADA_LESS,nb_x_tmp);
   Inequation * ineq2=new Inequation(nb_x_tmp,FADA_LESS_EQ,lhs);
   new_inequations->push_back(ineq1);
   new_inequations->push_back(ineq2);
   return new Expression(var1.str());
   }
if(IsModulo() && GetRightChild()->IsValue()){ //E%nb=R sothat 0<=R<nb, forall X, E=nb*X+R
   Expression*lhs=GetLeftChild()->PIPize(new_variables,new_inequations);
   ostringstream R,X;
   R<<"__tmp__"<<new_variables->size();
   X<<"__tmp__"<<new_variables->size()+1;
   
   new_variables->push_back(R.str());
   new_variables->push_back(X.str());

   Expression* nb_x_X=new Expression(GetRightChild(), FADA_MUL, new Expression(X.str()));
   Expression* R_expr=new Expression(R.str());
   Inequation*ineq1=new Inequation(new Expression(0),FADA_LESS_EQ,R_expr);
   Inequation*ineq2=new Inequation(R_expr,FADA_LESS,GetRightChild());
   Inequation*ineq3=new Inequation(lhs,FADA_EQ,new Expression(nb_x_X,FADA_ADD,R_expr));
   new_inequations->push_back(ineq1);
   new_inequations->push_back(ineq2);
   new_inequations->push_back(ineq3);
   return R_expr;
   }
return this;
}
/////////////////////////////////////////////////////////////////////////////////////////////

bool
Expression::IsFuzzy(set<string>* fuzzy_variables){
if(IsLeaf()){
   if(IsValue())   return false;
   if(IsArray() && IsFunction())   return true;
   if(IsVariable())
      if(find(fuzzy_variables->begin(),fuzzy_variables->end(),GetVariableName())==fuzzy_variables->end())   return false;
      else                                       return true;
   cerr<<"\nExpression::IsFuzzy ............. inappropriate case\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }
if(!GetLeftChild()->IsFuzzy(fuzzy_variables))
   return GetRightChild()->IsFuzzy(fuzzy_variables);
else
   return true;
}



Inequation   Expression::operator<(Expression&e)      {return *(new Inequation(this, FADA_LESS,&e));}
Inequation   Expression::operator<(Expression*e)      {return *(new Inequation(this, FADA_LESS,e));}
Inequation   Expression::operator<(int val)      {return *(new Inequation(this, FADA_LESS,new Expression(val)));}

Inequation   Expression::operator<=(Expression&e)      {return *(new Inequation(this, FADA_LESS_EQ,&e));}
Inequation   Expression::operator<=(Expression*e)      {return *(new Inequation(this, FADA_LESS_EQ,e));}
Inequation   Expression::operator<=(int val)      {return *(new Inequation(this, FADA_LESS_EQ,new Expression(val)));}

// Inequation   Expression::operator==(Expression&e)      {return *(new Inequation(this, FADA_EQ,&e));}
// Inequation   Expression::operator==(Expression*e)      {return *(new Inequation(this, FADA_EQ,e));}
// Inequation   Expression::operator==(int val)      {return *(new Inequation(this, FADA_EQ,new Expression(val)));}

Inequation   Expression::operator!=(Expression&e)      {return *(new Inequation(this, FADA_NEQ,&e));}
Inequation   Expression::operator!=(Expression*e)      {return *(new Inequation(this, FADA_NEQ,e));}
Inequation   Expression::operator!=(int val)      {return *(new Inequation(this, FADA_NEQ,new Expression(val)));}

Inequation   Expression::operator>(Expression&e)      {return *(new Inequation(this, FADA_GREATER,&e));}
Inequation   Expression::operator>(Expression*e)      {return *(new Inequation(this, FADA_GREATER,e));}
Inequation   Expression::operator>(int val)      {return *(new Inequation(this, FADA_GREATER,new Expression(val)));}

Inequation   Expression::operator>=(Expression&e)      {return *(new Inequation(this, FADA_GREATER_EQ,&e));}
Inequation   Expression::operator>=(Expression*e)      {return *(new Inequation(this, FADA_GREATER_EQ,e));}
Inequation   Expression::operator>=(int val)      {return *(new Inequation(this, FADA_GREATER_EQ,new Expression(val)));}


};



