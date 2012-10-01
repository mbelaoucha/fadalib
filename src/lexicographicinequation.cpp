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

#include "lexicographicinequation.h"
#include <global.h>
#include <cstdlib>   //gcc 4.3.x


namespace fada{
////////////////////////////
////////////////////////////

LexicographicInequation::LexicographicInequation(vector<string>* __lhs,T_Lexicographic_Operator __op, int __deep,vector<string>* __rhs )
{
Initialize();
Set(__lhs,__op,__deep,__rhs);
}

////////////////////////////
////////////////////////////

void
LexicographicInequation::Set(vector<string>* __lhs,T_Lexicographic_Operator __op, int __deep,vector<string>* __rhs )
{
SetLHS(__lhs);
SetRHS(__rhs);
SetOp(__op);
SetDeep(__deep);
}

////////////////////////////
////////////////////////////

LexicographicInequation::~LexicographicInequation()
{
}

////////////////////////////
////////////////////////////

void
LexicographicInequation::Initialize()
{
vector<string>   empty;
Set(&empty,LEX_NLEXOP,-1,&empty);
}

////////////////////////////
////////////////////////////

LexicographicInequation*
LexicographicInequation::Negate(void){
T_Lexicographic_Operator   new_op;
switch(GetOp()){
   case LEX_EQ      :new_op=LEX_NEQ;break;
   case LEX_NEQ      :new_op=LEX_EQ;break;
   case LEX_GREATER   :new_op=LEX_LESS_EQ;break;
   case LEX_GREATER_EQ   :new_op=LEX_LESS;break;
   case LEX_LESS      :new_op=LEX_GREATER_EQ;break;
   case LEX_LESS_EQ   :new_op=LEX_GREATER;break;
   default   :
      cerr<<"\nLexicographicInequation::Negate ... FATAL ERROR (unhandled case)\n";
      throw(INAPPROPRIATE_CASE);
      exit(EXIT_FAILURE);
   }
return new LexicographicInequation(GetLHS(),new_op,GetDeep(),GetRHS());
}

////////////////////////////
////////////////////////////

string
LexicographicInequation::Print_Tex_str(void){
string   result=PrintVector(GetLHS());
switch(GetOp()){
   case LEX_EQ      :result+="=";break;
   case LEX_NEQ      :result+="\\ncong";break;
   case LEX_LESS      :result+="\\prec";break;
   case LEX_LESS_EQ   :result+="\\preccurlyeq";break;
   case LEX_GREATER_EQ   :result+="\\succcurlyeq";break;
   case LEX_GREATER   :result+="\\succ";break;
   default         :
      cerr<<"\nLexicographicInequation::Print_str ... FATAL ERROR(inappropriate operator)\n";
      throw(INAPPROPRIATE_CASE);
      exit(EXIT_FAILURE);
   }
ostringstream   oss;
oss<<"_{lex("<<GetDeep()<<")}";
result+=oss.str()+PrintVector(GetRHS());
return result;
}
////////////////////////////
////////////////////////////
string
LexicographicInequation::Print_str(void){
string   result=PrintVector(GetLHS());
switch(GetOp()){
   case LEX_EQ      :result+="==";break;
   case LEX_NEQ      :result+="!=";break;
   case LEX_LESS      :result+="<";break;
   case LEX_LESS_EQ   :result+="<=";break;
   case LEX_GREATER_EQ   :result+=">=";break;
   case LEX_GREATER   :result+=">";break;
   default         :
      cerr<<"\nLexicographicInequation::Print_str ... FATAL ERROR(inappropriate operator)\n";
      throw(INAPPROPRIATE_CASE);
      exit(EXIT_FAILURE);
   }
ostringstream   oss;
oss<<"lex("<<GetDeep()<<")";
result+=oss.str()+PrintVector(GetRHS());
return result;
}
////////////////////////////
////////////////////////////
vector<Inequation*>
LexicographicInequation::ToInequations(void){
vector<Inequation*> result;
vector<string>::iterator itr=GetRHS()->begin();
vector<string>::iterator itl=GetLHS()->begin();
for(int i=0; i<GetDeep(); ++i,++itr,++itl)
   
   result.push_back(new Inequation(new Expression(*itl),FADA_EQ,new Expression(*itr)));
if(itr != GetRHS()->end() && itl != GetLHS()->end()){
   switch(GetOp()){
      case LEX_EQ      :result.push_back(new Inequation(new Expression(*itl),FADA_EQ,new Expression(*itr)));break;
      case LEX_NEQ      :result.push_back(new Inequation(new Expression(*itl),FADA_NEQ,new Expression(*itr)));break;
      case LEX_LESS      :result.push_back(new Inequation(new Expression(*itl),FADA_LESS,new Expression(*itr)));break;
      case LEX_LESS_EQ   :result.push_back(new Inequation(new Expression(*itl),FADA_LESS_EQ,new Expression(*itr)));break;
      case LEX_GREATER_EQ   :result.push_back(new Inequation(new Expression(*itl),FADA_GREATER_EQ,new Expression(*itr)));break;
      case LEX_GREATER   :result.push_back(new Inequation(new Expression(*itl),FADA_GREATER,new Expression(*itr)));break;
      default         :
         cerr<<"\nLexicographicInequation::Print_str ... FATAL ERROR(inappropriate operator)\n";
         throw(INAPPROPRIATE_CASE);
         exit(EXIT_FAILURE);
         }
   }
return result;
}
////////////////////////////
////////////////////////////
};


