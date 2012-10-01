/***************************************************************************
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or *
 * (at your option) any later version. *
 * *
 * This program is distributed in the hope that it will be useful, *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
 * GNU General Public License for more details. *
 * *
 * You should have received a copy of the GNU General Public License *
 * along with this program; if not, write to the *
 * Free Software Foundation, Inc., *
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
 ***************************************************************************/

#ifndef LEXICOGRAPHICINEQUATION_H
#define LEXICOGRAPHICINEQUATION_H

/**
 @author belaoucha marouane <bem@prism.uvsq.fr>
*/


#include <inequation.h>

namespace fada{
enum T_Lexicographic_Operator
{
LEX_EQ,
LEX_NEQ,
LEX_LESS,
LEX_LESS_EQ,
LEX_GREATER_EQ,
LEX_GREATER,
LEX_NLEXOP,
};


class LexicographicInequation{
vector<string> lhs;
vector<string> rhs;
int deep;
T_Lexicographic_Operator operation;

inline void SetLHS(vector<string>* __lhs) {lhs=*__lhs;}
inline void SetRHS(vector<string>* __rhs) {rhs=*__rhs;}
inline void SetDeep(int __deep) {deep=__deep;}
inline void SetOp(T_Lexicographic_Operator __op) {operation=__op;}

void Set(vector<string>* __rhs,T_Lexicographic_Operator __op, int deep,vector<string>* __lhs );
void Initialize();

public:

 LexicographicInequation(vector<string>* __rhs,T_Lexicographic_Operator __op, int deep,vector<string>* __lhs );
 ~LexicographicInequation();

LexicographicInequation* Negate(void);

inline T_Lexicographic_Operator GetOp(void) {return operation;}
inline vector<string>* GetLHS(void) {return &lhs;}
inline vector<string>* GetRHS(void) {return &rhs;}
inline int GetDeep(void) {return deep;}
string Print_str(void);
inline void Print(void) {cout<<Print_str();}
vector<Inequation*> ToInequations(void);
string Print_Tex_str(void);
};
};
#endif


