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

#include "domaincondition.h"

namespace fada{
DomainConstraint::DomainConstraint(Condition* __cond, bool __pos)
{
Initialize();
Set(__cond,__pos);
}

DomainConstraint::DomainConstraint(Condition* __cond)
{
Initialize();
Set(__cond,true);
}


DomainConstraint::~DomainConstraint()
{
}

void DomainConstraint::Initialize()
{
Set(NULL,true);
}

void
DomainConstraint::Set(Condition* __cond, bool __pos)
{

SetCondition(__cond);
SetPositive(__pos);
}

void
DomainConstraint::Set(Condition* __cond)
{
SetCondition(__cond);
SetPositive();
}

string
Print_str(void){
}
};
