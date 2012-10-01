
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
#ifndef LDEMONSTRATOR_H
#define LDEMONSTRATOR_H

#include <logicalclause.h>
#include <list>

namespace fada{
class LogicalClause;
class ContextQuast;

class AttachedClauses{
private:
LogicalClause* knowledge;
LogicalClause* attached;
bool were_unified;
LogicalClause* unified_clauses;
vector<Inequation*> valid_unification;

inline void SetKnowledge(LogicalClause* __fact) {knowledge=__fact;}
inline void SetAttached(LogicalClause* __attached) {attached=__attached;}
inline void WereUnified(bool val) {were_unified=val;}
inline void SetUnifiedClauses(LogicalClause* __unified) {unified_clauses=__unified;}
inline vector<Inequation*>* GetValidUnificationConstraints(void) {return & valid_unification;}
inline void SetValidUnificationConstraints(vector<Inequation*>* constraints) {valid_unification=*constraints;}

void Initialize(void);
void Set(LogicalClause* __fact,LogicalClause* __attached);
void Set(LogicalClause* __fact);

public:
 ~AttachedClauses();
 AttachedClauses(LogicalClause* __fact,LogicalClause* __attached);
 AttachedClauses(LogicalClause* __fact);

inline LogicalClause* GetKnowledge(void) {return knowledge;}
inline LogicalClause* GetAttached(void) {return attached;}
inline bool IsAttached(void) {return attached!=NULL;}
void Attach(LogicalClause* __lc);
inline bool WereUnified(void) {return were_unified;}
inline LogicalClause* GetUnified(void) {return unified_clauses;}
void Unify(vector<string>* __param);
string Print_str(void);
string Print_Tex_str(void);
string Full_Tex_Print_str(vector<string>* param);
inline void Print(void) {cout<<Print_str();}
void Detach(void) {SetAttached(NULL);}
ContextQuast* Traduce2ContextQuast(vector<vector<Inequation*> >* env ,vector<string>* __param);
string ToGraphVizLine(string __nn);
bool IsAccurate(vector<Inequation*>* precedence, vector<string>* param);
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

class LDemonstrator{
private:
list<AttachedClauses*> inference_stack;

inline list<AttachedClauses*>* GetStack(void) {return &inference_stack;}
inline void SetStack(list<AttachedClauses*>* __stack) {inference_stack=*__stack;}
LDemonstrator* RemoveDoubles();
public:

AttachedClauses* POP(void);
LDemonstrator* POPAffines(void);
AttachedClauses* GetHeader(void);
void PUSHBACK(LogicalClause*);
void PUSHINFRONT(LogicalClause*);
inline void CLEAR(void) {GetStack()->clear();}
void AttachToAll(LogicalClause* __clause);
void DetachAll(void);
vector<LogicalClause*> RecoverUnified();
void Inject(vector<LogicalClause*>* __unified);
void Try2UnifyAll(vector<string>* __param);
bool Inference(vector<string>* __param);
string Print_str(void);
string Print_Tex_str(void);
string Full_Tex_Print_str(vector<string>* param);
inline void Print() {cout<<Print_str();}
ContextQuast* Traduce2ContextQuast(vector<vector<Inequation*> >* __env,vector<string>* __param);
void Merge(LDemonstrator* __ld);
void EliminateDoubles(void);
inline int Size(void) {return GetStack()->size();}
vector<Inequation*> TraduceLoopProperties(vector<Inequation*>* __env, vector<Inequation*>* __valid);
LDemonstrator* Clone();
bool IsAccurate(vector<Inequation*>* precedence, vector<string>* param);
// LDemonstrator* RmoveDoubles();
 LDemonstrator();
 ~LDemonstrator();

};

};
#endif


