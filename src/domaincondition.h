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

#ifndef DOMAINCONDITION_H
#define DOMAINCONDITION_H

#include <condition.h>

namespace fada{
class DomainConstraint{
Condition*   cond;
bool         is_positive;

inline void   SetCondition(Condition* __cond)    {cond=__cond;}
inline void   SetPositive(bool val=true)      {is_positive=val;}

void      Set(Condition* __cond, bool __pos);
void      Set(Condition* __cond);
void      Initialize(void);

public:
      ~DomainConstraint();

      DomainConstraint(Condition* __cond, bool __pos);
      DomainConstraint(Condition* __cond);

Condition*   GetCondition(void)   {return cond;      }
bool         IsPositive(void)   {return is_positive;   }

string         Print_str(void);
inline void      Print(void)      {cout<<Print_str();}


};
};
#endif
