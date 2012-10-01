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
#ifndef LC_RHS_TERM_H
#define LC_RHS_TERM_H

#include <cstdlib>
#include <lexicographicinequation.h>

using namespace std;
/**
   @author belaoucha marouane <bem@prism.uvsq.fr>
*/

namespace fada{
class LexicographicInequation;

class LC_RHS_Term{
private:
   LexicographicInequation*   lex_relation;
   Inequation*      inequation;

   inline void SetLexRelation(LexicographicInequation* __rela)   {lex_relation=__rela;}
   inline void SetInequation(Inequation* __ineq)      {inequation=__ineq;}
   void   Set(LexicographicInequation* __rela, Inequation* __ineq);
   void   Set(Inequation* __ineq);
   void   Initialize(void);

   LC_RHS_Term();
public:
   LC_RHS_Term(LexicographicInequation* __rela, Inequation* __ineq);
   LC_RHS_Term(Inequation* __ineq);
   string Print_str(void);
   LexicographicInequation*   GetLexRelation()   {return lex_relation;}
   Inequation*      GetInequation()      {return inequation;}
   ~LC_RHS_Term();
   vector<Inequation*>   ToInequations(void);
   string         Print_Tex_str(void);
   LC_RHS_Term*      Copy();
};
};
#endif



