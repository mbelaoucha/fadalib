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

#include "agc.h"


namespace fada{
AGC::AGC()
{
}


AGC::~AGC()
{
}


void
AGC::Free(Expression* __exp){
set<Expression*>::iterator it=expressions.find(__exp);
if(it != expressions.end()){
   expressions.erase(it);
   }
delete __exp;
}

void
AGC::Free(Inequation* __ineq){
set<Inequation*>::iterator it=inequations.find(__ineq);
if(it != inequations.end())
   inequations.erase(it);

delete __ineq;
}

void
AGC::Free(Condition* __cond){
set<Condition*>::iterator it=conditions.find(__cond);
if(it != conditions.end())
   conditions.erase(it);
delete  __cond;
}

void
AGC::Free(FADA_Index* __index){
set<FADA_Index*>::iterator it=indexes.find(__index);
if(it != indexes.end())
   indexes.erase(it);
delete __index;
}

void
AGC::Free(Quast*   __quast){
set<Quast*>::iterator it=quasts.find(__quast);
if(it != quasts.end())
   quasts.erase(it);
delete __quast;
}

void
AGC::Free(FlatQuast*   __fq){
set<FlatQuast*>::iterator it=flatquasts.find(__fq);
if(it != flatquasts.end())
   flatquasts.erase(it);
delete __fq;
}

void
AGC::FreeAllExpressions(){
for(set<Expression*>::iterator it=expressions.begin(); it != expressions.end(); ++it)
   delete *it;
expressions.clear();
}

void
AGC::FreeAllInequations(){
for(set<Inequation*>::iterator it=inequations.begin(); it != inequations.end(); ++it)
   delete *it;
inequations.clear();
}

void
AGC::FreeAllIndexes(){
for(set<FADA_Index*>::iterator it=indexes.begin(); it != indexes.end(); ++it)
   delete *it;
indexes.clear();

}

void
AGC::FreeAllFlatQuasts(){
for(set<FlatQuast*>::iterator it=flatquasts.begin(); it != flatquasts.end(); ++it)
   delete *it;
flatquasts.clear();

}

void
AGC::FreeAllConditions(){
for(set<Condition*>::iterator it=conditions.begin(); it != conditions.end(); ++it)
   delete *it;
conditions.clear();

}
};


