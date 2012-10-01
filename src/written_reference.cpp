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

#include "written_reference.h"
#include <cstdlib>
namespace fada{

Written_Reference::~Written_Reference()
{
}

void
Written_Reference::Initialize(){
Set("",NULL,NULL,NULL);
/*SetAnti(NULL);
SetIndex(NULL);
SetName("");
SetOutput(NULL);*/
return;
}

void
Written_Reference::Set(string __name, FADA_Index* __index){
SetName(__name);
SetIndex(__index);
}

void
Written_Reference::Set(string __name, FADA_Index* __index, Quast* __anti, Quast* __out){
SetName(__name);
SetIndex(__index);
SetAnti(__anti);
SetOutput(__out);
}

void
Written_Reference::Set(Expression* __expr){
if(__expr->IsArray()){
   Set(__expr->GetArrayName(), new FADA_Index(__expr->GetIndex()));
   return;
   }
if(__expr->IsVariable()){
   Set(__expr->GetVariableName(), new FADA_Index());
   return;
   }
cerr<<"\nWritten_Reference::Set(Expression* __expr) ... inappropriate case \n";
throw(10);
}


Written_Reference::Written_Reference(Expression* __expr){
Initialize();
Set(__expr);
}


Written_Reference::Written_Reference(string __name){
Initialize();
vector<Expression*>   empty;
Set(__name,new FADA_Index(&empty));
}

Written_Reference::Written_Reference(string __n, FADA_Index* __i){
Initialize();
Set(__n,__i);
}
Written_Reference::Written_Reference(string _n, FADA_Index* _i,Quast* _a, Quast* _o){
Initialize();
Set(_n,_i,_a,_o);
}

string
Written_Reference::Print_str(int level){
string result;
result=GetName()+GetIndex()->Generate_C_Code();
if(level == 0)
   return result;
cout<<"Written_Reference::Print_str, fatal error (inappropriate value) ...";
exit(EXIT_FAILURE);
}

void
Written_Reference::WrittenScalars(set<string>* __written){
if(GetIndex()->GetSize()==0)
        __written->insert(GetName());
}

void
Written_Reference::TagIndex(int __stmt, vector<Expression*>* __iter){
GetIndex()->Instanciate(__stmt,__iter);
}
}

