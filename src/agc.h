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

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

/**
   @author belaoucha marouane <bem@prism.uvsq.fr>
*/

#include <set>
#include <quast.h>
#include <condition.h>

namespace fada{

class AGC{
set<Expression*>   expressions;
set<Inequation*>   inequations;
set<Condition*>      conditions;

set<FADA_Index*>   indexes;
set<Quast*>      quasts;
set<FlatQuast*>      flatquasts;

void      FreeAllExpressions();
void      FreeAllInequations();
void      FreeAllIndexes();
void      FreeAllFlatQuasts();
void      FreeAllConditions();

public:
    AGC();
    ~AGC();

inline void   Trace(Expression* __exp)   {expressions.insert(__exp);}
inline void   Trace(Inequation* __ineq)   {inequations.insert(__ineq);}
inline void   Trace(Condition* __cond)   {conditions.insert(__cond);}
inline void   Trace(FADA_Index* __index)   {indexes.insert(__index);}
inline void   Trace(Quast* __quast)      {quasts.insert(__quast);}
inline void   Trace(FlatQuast* __fq)      {flatquasts.insert(__fq);}

void      Free(Expression* __exp);
void      Free(Inequation* __ineq);
void      Free(Condition* __cond);
void      Free(FADA_Index* __index);
void      Free(Quast*   __quast);
void      Free(FlatQuast*   __fq);

};

};
#endif


