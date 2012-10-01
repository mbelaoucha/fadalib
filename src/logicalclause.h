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

#ifndef LOGICALCLAUSE_H
#define LOGICALCLAUSE_H
#include <inequation.h>
#include <lexicographicinequation.h>
#include <lc_rhs_term.h>


namespace fada{
class ContextQuast; //cyclic includes
class LC_RHS_Term;

class LogicalClause{
private:
vector<Inequation*> affine_inequations;
vector<Inequation*> non_affine_inequations;
vector<LC_RHS_Term*> rhs;
vector<string> variables;

inline void SetVariables(vector<string>* __v) {variables=*__v;}
inline void SetAffineInequations(vector<Inequation*>* __ineqs) {affine_inequations=*__ineqs;}
inline void SetNonAffineInequations(vector<Inequation*>* __ineqs){non_affine_inequations=*__ineqs;}

inline void SetRHS(vector<LC_RHS_Term*>*__i) {rhs=*__i;}
void Set(vector<string>* __var,vector<Inequation*>* __affine, vector<Inequation*>* __non_affine, vector<LC_RHS_Term*>* __lex);
inline void Initialize(void);

public:
// LogicalClause();
 LogicalClause(vector<string>* __var,vector<Inequation*>* __affine, vector<Inequation*>* __non_affine,vector<LC_RHS_Term*>* __lex);
 ~LogicalClause();

inline vector<string>* GetVariables(void) {return &variables;}
inline vector<Inequation*>* GetAffineInequations(void) {return &affine_inequations;}
inline vector<LC_RHS_Term*>* GetLexRelations(void) {return &rhs;}
inline vector<Inequation*>* GetNonAffineInequations(void) {return &non_affine_inequations;}
inline bool IsAffine(void) {return non_affine_inequations.size()==0;}
string Print_str();
string Print_Tex_str();
string Full_Tex_Print_str(vector<string>* param);
void Print() {cout<<Print_str();}
string ToGraphVizLine(string __nn);

LogicalClause* Unify(LogicalClause* __clause, vector<string>* __param);

ContextQuast* TraduceToQuast(vector<vector<Inequation*> >* env, vector<string>* __param);
vector<vector<Inequation*> > RHS2Inequations(void);


};

LogicalClause* LoopProperty(vector<string>* enclosing_loops, string loop_counter, Inequation* non_affine_condition);
bool
__Unify(Inequation* ineq1, vector<string>* var1,Inequation* ineq2, vector<string>* var2, vector<string>* param,vector<Inequation*>* inequations);

bool
Equal(Expression* exp1, vector<string>* var1,Expression* exp2, vector<string>* var2, vector<string>* param,vector<Inequation*>* inequations);
};
#endif



