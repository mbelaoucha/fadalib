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
#include "lc_rhs_term.h"
#include "constants.h"

namespace fada{
//////////////////////////////////////////////////
//////////////////////////////////////////////////

LC_RHS_Term::LC_RHS_Term()
{
Initialize();
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

LC_RHS_Term::~LC_RHS_Term()
{
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
string
LC_RHS_Term::Print_Tex_str(void){
if(GetLexRelation() != NULL && GetInequation() != NULL){
   string result;
   result=GetLexRelation()->Print_Tex_str();
   result+=" \\wedge "+GetInequation()->Print_Tex_str();
   return result;
   }
if(GetLexRelation() == NULL && GetInequation() != NULL)
   return GetInequation()->Print_Tex_str();
cerr<<"\nLC_RHS_Term::Print_Tex_str\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

string
LC_RHS_Term::Print_str(void){
if(GetLexRelation() != NULL && GetInequation() != NULL){
   string result;
   result=GetLexRelation()->Print_str();
   result+=" and "+GetInequation()->Print_str();
   return result;
   }
if(GetLexRelation() == NULL && GetInequation() != NULL)
   return GetInequation()->Print_str();
cerr<<"\nLC_RHS_Term::Print_str\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////

void
LC_RHS_Term::Set(LexicographicInequation* __rela, Inequation* __ineq){
SetInequation(__ineq);
SetLexRelation(__rela);
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

void
LC_RHS_Term::Set(Inequation* __ineq){
SetLexRelation(NULL);
SetInequation(__ineq);
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

void
LC_RHS_Term::Initialize(void){
SetLexRelation(NULL);
SetInequation(NULL);
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

LC_RHS_Term::LC_RHS_Term(LexicographicInequation* __rela, Inequation* __ineq){
Set(__rela,__ineq);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////

LC_RHS_Term::LC_RHS_Term(Inequation* __ineq){
Set(__ineq);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////



vector<Inequation*>
LC_RHS_Term::ToInequations(void){
vector<Inequation*> result;
if(GetLexRelation() != NULL)
   result=GetLexRelation()->ToInequations();
if(GetInequation() != NULL)
   result.push_back(GetInequation());
return result;
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

LC_RHS_Term*
LC_RHS_Term::Copy(){
LC_RHS_Term* res;
LexicographicInequation* __rel;
*__rel=*GetLexRelation();
return new LC_RHS_Term(__rel,GetInequation()->Copy());
}
};
//////////////////////////////////////////////////
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//////////////////////////////////////////////////




