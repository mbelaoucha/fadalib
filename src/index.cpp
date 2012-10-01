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


#include "index.h"
#include "constants.h"
#include "global.h"



namespace fada{
/////////////////////////////////////////////////////////////////////////////////////////////

FADA_Index::FADA_Index(vector<Expression* > * index)
{
Initialize( );
Set(index);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void   
FADA_Index::Set(vector<Expression* > * index)
{
SetIndex( index ); // TODO:à revoir
}
/////////////////////////////////////////////////////////////////////////////////////////////

FADA_Index::FADA_Index()
{
Initialize( );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// void   
// FADA_Index::Set()
// {
// vector<Expression* >*  empty=new vector<Expression* >;
// SetIndex( index ); // TODO:à revoir
// delete empty;
// }

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

FADA_Index::~FADA_Index()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void         
FADA_Index::Initialize(void)
{
index.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
FADA_Index::Print(void)
{
cout << Print_str( );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void
FADA_Index::Instanciate(int read_id,vector<Expression*>* read_iter){
for(vector<Expression*>::iterator it=GetIndex()->begin(); it != GetIndex()->end(); ++it)
   (*it)->Instanciate(read_id,read_iter);
}
/////////////////////////////////////////////////////////////////////////////////////////////

string
FADA_Index::Print_str(void)
{
string result="";
for(vector<Expression*>::iterator it= GetIndex( )->begin();it != GetIndex( )->end(); ++it)
   {
   result+=(*it)->Print_str( );
   vector<Expression*>::iterator it2=it; 
   it2++;
   if(it2 != GetIndex( )->end())
      result+=", ";
   };
return result;
}   

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Expression*      
FADA_Index::GetItem(int item)
{
if(item > GetSize( ))
   return NULL;
return index[item];
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
FADA_Index::Equal(FADA_Index* Index)
{
Condition* result= new Condition(new Inequation (true) ); 
if(GetSize( ) != Index->GetSize( ))
   {
   cerr<<endl<<"FADA_Index::operator==(FADA_Index*) :Not the same dimension"<<endl;
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE); 
   }
for(int i=0; i<GetSize( ); ++i)
   {
   Inequation* ineq=new Inequation( GetItem( i),FADA_EQ, Index->GetItem( i));
   result = new Condition( result,FADA_AND,new Condition(ineq));
   };
return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Condition*
FADA_Index::operator==(FADA_Index* Index)
{
return Equal( Index );
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
Condition*
FADA_Index::operator==(FADA_Index& Index)
{
return Equal( &Index );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
Condition*      
FADA_Index::Lex_Less(FADA_Index* Index)
{
Condition* result=NULL;
int size1 = GetSize( );
int size2 = Index->GetSize( );
int min = (size1 < size2 ? size1 : size2);

for(int i=0; i<min; i++)
   {//level
   
   Condition* nest_pred=new Condition(new Inequation ( GetItem( i) , FADA_LESS, Index->GetItem( i ))); 
      // this[i] < Index [i] 

   for(int j=0; j<i; j++)
      //this[j] = Index[j], for each j in 0..i-1
      nest_pred = new Condition(nest_pred,FADA_AND,  new Condition(new Inequation ( GetItem( j ), FADA_EQ, Index->GetItem(  j))));

   if(result != NULL)
      result = new Condition( result,FADA_OR, nest_pred);
   else
      result = nest_pred;
   };

return result;
};

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
FADA_Index::Substitute(map<string, string>* __mapping)
{
for(vector<Expression*>::iterator it=GetIndex( )->begin(); it != GetIndex( )->end(); ++it)
   (*it)->Substitute(__mapping );
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
FADA_Index*
FADA_Index::Substitute(map<string, Expression*>*  __mapping)
{
vector<Expression*>  __vector;
for(vector<Expression*>::iterator it= GetIndex()->begin(); it != GetIndex()->end(); ++it){
   __vector.push_back((*it)->Substitute(__mapping));
   }
return new FADA_Index(&__vector);
/*cout<<endl<<"FADA_Index::Substitute(map<string, Expression*>  __mapping), not implemented yet !!"<<endl;
exit(EXIT_FAILURE);*/
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

FADA_Index*
FADA_Index::Copy(void)
{
vector<Expression*>* new_index=new vector<Expression*>;
for(vector<Expression*>::iterator it=GetIndex( )->begin(); it !=GetIndex( )->end (); ++it)
   new_index->push_back( (*it)->Copy( ) );

return new FADA_Index( new_index );
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
Condition*
FADA_Index::Lex_Sup(FADA_Index* __index, int common)
{

Condition *result;
bool firsttime=true;

for(int i=0; i<common; i++)

   {
   Condition *nest_pred = new Condition(new Inequation(GetItem(i),FADA_LESS,__index->GetItem(i)));
      //i1_i < i2_i
   for(int j=0; j<i; j++)
      nest_pred = new Condition(nest_pred,FADA_AND,new Condition( new Inequation(GetItem(j),FADA_EQ,__index->GetItem(j))));

   if(!firsttime)
      result = new Condition(result,FADA_OR, nest_pred);
   else
      {
      firsttime=false;
      result = nest_pred;
      }
      //add it to pred
   }

return result;
}

Condition*
FADA_Index::Lex_Eq(FADA_Index* __index, int commun)
{
if(commun == 0)
   return new Condition(new Inequation(true));

Condition*   result=new Condition(new Inequation(GetItem(0) ,FADA_EQ, __index->GetItem(0)));

for(int i=1; i<commun;++i)
   result=new Condition(result, FADA_AND, new Condition(new Inequation (GetItem(i),FADA_EQ,__index->GetItem(i))));

return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

FADA_Index*
FADA_Index::ComputeDistance(vector<string>* __commun_counters, vector<string>* __param,bool rename)
{
vector<string>::iterator      itc = __commun_counters->begin();
vector<Expression*>* res=new vector<Expression*>;
vector<string>*   parameters=new vector<string> ();
*parameters=*__param;

map<string,int>*   mapping_counters=new map<string,int>();
map<string,int>*   mapping_parameters=new map<string,int>();

int i=0;
for(vector<string>::iterator it= __param->begin(); it != __param->end() ; ++it,++i)
   mapping_parameters->operator[](*it) = i;

for(int i=0; i<__commun_counters->size();i++)
   {
   Expression*   __res;
   if(!rename)
      __res=new Expression(
         new Expression(FADA_Prefix+*itc),
         FADA_SUB,
         this->GetItem(i));
   else{
      __res=new Expression(
         new Expression(*itc),
         FADA_SUB,
         this->GetItem(i));
      parameters->push_back(*itc);
      mapping_parameters->operator[](*itc) = parameters->size()-1;
   }

   vector<string>*   var=new vector<string>();
   vector<int>* __vectorized=new vector<int>();
   *__vectorized=__res->ToVector(var,mapping_counters,parameters,mapping_parameters);
   res->push_back(VectorToExpression(__vectorized, var, parameters));

   delete var;
   delete __vectorized;
   itc++;
   }
delete mapping_counters;
delete mapping_parameters;
delete parameters;
return new FADA_Index(res);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

string
FADA_Index::Generate_C_Code(void)
{
string result="";
for(vector<Expression*>::iterator it= GetIndex( )->begin();it != GetIndex( )->end(); ++it)
   result+="[ "+(*it)->Generate_C_Code( )+" ]";
return result;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
FADA_Index::ReferencedScalars(set<string>* scalars){
for(vector<Expression*>::iterator it=GetIndex()->begin(); it != GetIndex()->end(); ++it)
   (*it)->ReferencedScalars(scalars);
}

// string 
// FADA_Index::PrintAll_str(vector<string>* var, vector<string>* param){
// string result;
// for(int i=0; i< GetSize(); ++i)
//    result+="["+GetItem(i)->PrintAll_str(var,param)+"]";
// return result;
// }
};



