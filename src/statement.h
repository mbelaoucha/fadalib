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

#ifndef ADAAN_STATEMENT_H
#define ADAAN_STATEMENT_H

#include "assignment.h"
#include "controlstructure.h"
#include "elementarydependence.h"
#include "references.h"


/**
\file adaan_statement.h
\author Marouane Belaoucha
\brief This file contain the description of the class Statement
*/

namespace fada{
class Assignment;
class ElementaryDependence;
class References;
class Program;

/*!
\class Statement
\brief It represents statements.
*/
class Statement{
private:

int id;   //!< The identifier of the statement.
   
int first_stmt;   //!< the ID of the first enclosed statement.
int last_stmt;   //!< the ID of the last enclosed statement.
   
int first_else_stmt;   //!< the ID of the first statement enclosed by the else-part, if th statement is an alternative
int last_else_stmt;   //!< the ID of the last statement enclosed by the else-part, if th statement is an alternative

References* references;   //!< all references
bool is_main;      //!< is the main program (a root node)
vector<Inequation*>   pragma_constraints;

//!\name If not a main program
Statement* parent;

bool is_assign;      //!< is it an assignment ?
//!\name If assignment
Assignment* assign;   //!< the assignment

//! \name If control (not an assignment)

Control* control;   //!< a reference to the control
vector<Statement*> surrounded_statements;         //!< List of surrounded statements.
//! \name If alternative
vector<Statement*> surrounded_statements_else_part;   //!< List of statements surrounded by else branch


//!\name Setters
   //! \brief A setter for "is_assign"
   void SetStatementKind(bool kind) {is_assign=kind;}
   //! \brief A setter for "parent"
   void SetParent(Statement* __stmt){parent=__stmt;};
   //! \brief A setter for "assign"
   void SetAssignment(Assignment* __assign) {assign=__assign;}
   //! \brief A setter for "control"
   void SetControl(Control* __ctl) {control=__ctl;}
   //! \brief A setter for "surrounded_statements"
   void SetSurrStmt(vector<Statement*>* __stmts) {surrounded_statements=*__stmts;}
   //! \brief A setter for "surrounded_statements_else_part"
   void SetElseSurrStmt(vector<Statement*>* __stmts) {surrounded_statements_else_part=*__stmts;}
   void SetProgram(bool value=true) {is_main=value;}
   void Serialize(vector<Assignment*>*, vector<Control*>*);

//!\name Advanced setters
   void Initialize(void);
   void Set(Assignment* assign);
   void Set(Control* __control, vector<Statement*>* __surr);
   void Set(Control* __control, vector<Statement*>* __surr, vector<Statement*>* __surr_else);
   void Set(vector<Statement*>* __stmts);
   void Link2Parent(Statement* __parent);



void   BuildDomains(Condition* __c);

public:
    ~Statement();
 Statement();
 Statement(Assignment*);
 Statement(vector<Statement*>* __stmts);
 Statement(Control* __control);
 Statement(Control* __control, vector<Statement*>* __surr);
 Statement(Control* __control, vector<Statement*>* __surr, vector<Statement*>* __surr_else);

void   Enclose(Statement*,bool in_else_part=false);
void   Enclose(Assignment*,bool in_else_part=false);

Statement* GetParent(void) {return parent;}
bool IsAssignment(void) {return !is_main && is_assign;}
bool IsControl(void) {return !is_main && !is_assign;}
bool IsProgram(void) {return is_main;}

Assignment* GetAssignment(void) {return assign;}


Control* GetControl(void) {return control;}
vector<Statement*>* GetSurroundedStatements(void) {return &surrounded_statements;};
vector<Statement*>* GetElseSurroundedStatements(void) {return &surrounded_statements_else_part;}

string Print_str(string tabs);
void Print(void);
string Regenerate_C_Code(string prefix);

string Generate_C_Test_Code(string prefix);
void Link2Parent(void);

vector<ElementaryDependence*> GenerateOptimalScheduleForFlowDependenciesComputing(string array, int stmt);
vector<ElementaryDependence*> GenerateOptimalScheduleForFlowDependenciesComputing(string array, FADA_Index*__i,int stmt, int deep);
vector<ElementaryDependence*> GenerateDeepestOptimalScheduleForFlowDependenciesComputing(string array, FADA_Index*__i,int read_stmt, int deep);
vector<ElementaryDependence*> GenerateReversedConfigs(string array,FADA_Index* __i, int read_stmt, int deep);


Statement* Find(int stmt);
void SetID(int __id) {id=__id;}
void SetReferences(References* __stmt) {references=__stmt;}
int GetID(void) {return id;}
References* GetReferences(void) {return references;}

vector<ElementaryDependence*> ComputeOptimalScheduleForFlowDependenciesEvaluation(int rank,string array, int deep);

void SetEnclosed(int f, int l) {first_stmt=f;last_stmt=l;}
void GetEnclosed(int*f,int*l) {*f=first_stmt;*l=last_stmt;}
void SetElseEnclosed(int f, int l) {first_else_stmt=f; last_else_stmt=l;}
void GetElseEnclosed(int *f, int *l) {*f=first_else_stmt; *l=last_else_stmt;}

void ReferencedScalars(set<string>* __written, set<string>* __read);
void SingleCounterId(vector<string> * allcounters);
void SingleCounterId(void);
void Substitute(map<string,string>* __mapping);
void NormalizeConditions(vector<References*>* ref,vector<string>* param);
// inline void ComputeSources(Program* pgm,vector<string>* param, bool sa, bool alphas);

string GenerateHTMLIndex(vector<References*>* __references,string indent);
void GenerateHTMLIndex(string file_name,vector<References*>* __references);
string ToHtml(string work_dir, string file_name_prefix, string indent,string def_frame,Options* op);
bool IsSequentiallyBefore(int id1, int id2);
bool IsEnclosed(int id);
bool IsInThenPart(int id);
bool IsInElsePart(int id);
string   PrettyPrint(void);

inline vector<Inequation*>* GetPragmaConstraints()   {return&   pragma_constraints;}
void   AddPragmaConstraint(Inequation* ineq)      {GetPragmaConstraints()->push_back(ineq);}
void    CollectIrregularLoopsProperties(LDemonstrator* __ppts, vector<string> enclosing_counters);
};



};

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


#endif


