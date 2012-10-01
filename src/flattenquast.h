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

#ifndef FLATQUAST_H
#define FLATQUAST_H

#include <inequation.h>
#include <list>
#include <vector>

namespace fada{
class FADA_Index;
class Quast;
class Inequation;
class References;
class Graph;


class Branch{
private:

vector<vector<Inequation*> >      guard_condition;
bool                  is_bottom;
int                  id_stmt;
FADA_Index*               instance;

inline void SetCondition(vector<vector<Inequation*> >* __cond)   {guard_condition=*__cond;}
inline void SetStmt(int __stmt)                  {id_stmt=__stmt;}
inline void SetInstance(FADA_Index* __index)            {instance=__index;}
inline void SetBottom(bool __)                  {is_bottom=__;}
void   Set(vector<vector<Inequation*> >* __guard, int __id, FADA_Index* __instance);
void   Set(vector<vector<Inequation*> >* __guard);
void   Initialize(void);

public:
Branch(vector<vector<Inequation*> >* __guard, int __id, FADA_Index* __instance);
Branch(vector<vector<Inequation*> >* __guard);
~Branch();
inline vector<vector<Inequation*> >*    GetCondition(void)   {return & guard_condition;}
inline int               GetStmt(void)      {return id_stmt;}
inline FADA_Index*            GetInstance(void)   {return instance;}
inline bool               IsBottom(void)      {return is_bottom;}
string                  Print_str(string __prefix);
inline void               Print(string __prefix){cout<<Print_str(__prefix);}
string                  PrettyPrint_str(string __prefix);
string                  PrintGraphViz(string node_name_prefix);
void                  RemoveConstraintsOnAlphas(set<string>* alphas);
string                  Print_Tex_str(string);
bool                  SameSolution(Branch* __branch);
bool                  Merge(Branch* __branch);
};


class FlatQuast{
private:
   list<Branch*>            branches;
inline void               Initialize()   {branches.clear();}

public:
   FlatQuast()            {Initialize();}
   ~FlatQuast();
list<Branch*>*               GetBranches(void)   {return &branches;}
void                  AddBranch(Branch* __branch);
string                  Print_str(string __prefix);
inline   void               Print(string __prefix)      {cout<<Print_str(__prefix);}
string                  PrettyPrint_str(string __prefix);
string                  PrintGraphViz(void);
string                  Print_Tex_str(void);
void                  Print_Tex(string file_name);
void                  ToGraphVizFile(string file_name);
void                  RemoveConstraintsOnAlphas(set<string>* alphas);
void                  GenerateInstanceWizeEdges(unsigned long int read_statement,string conflictual,FADA_Index* read_index, Graph* g);
};
};
#endif



