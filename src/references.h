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


/**
\file references.h
\author Marouane Belaoucha
\brief This file contains the definition of the class References
*/

#ifndef STMT_H
#define STMT_H

#include <condition.h>
#include <vector>
#include <written_reference.h>
#include <domaincondition.h>
#include <options.h>
#include "graphprinter.h"

namespace fada{
class Read_Reference;
class Statement;
class Written_Reference;
class DomainConstraint;
class Program;
class LDemonstrator;
class Graph;

/**
* \class References
* \brief describes references within statements in a program.

* A program is viewed as a vector of garded references. References are read/written variables. The gard is the condition of reading and/or writting such variables, and they depend on controls enclosing the statement the references come from.
*/

class References{

int id;               /*!< \brief the identifier of the entire references. */
Condition* domain;         /*!< \brief the gard-condition*/

map<int, DomainConstraint*>      controls;
vector<vector<Inequation*> >      affine_constraints;
vector<vector<Inequation*> >      non_affine_constraints;
vector<Inequation*>         pragma_constraints;


vector<Read_Reference*>       read_variables;       /*!< \brief  all read variables */
vector<Written_Reference*>       written_variables;   /*!< \brief  all written variables */
vector<string>             enclosing_counters;   /*!< \brief enclosing loops */


Statement* origin;            /*!< \brief if filled from an AST, the original statement the references come from */


//! \name Setters
   //! \brief a setter for "id"
   inline void SetId(int __id) {id=__id;}
   //! \brief a setter for "read_variables"
   inline void SetRV(vector<Read_Reference*>* rr) {read_variables=*rr;}
   //! \brief a setter for "written variables"
   inline void SetWV(vector<Written_Reference*>* wr) {written_variables=*wr;}
   //! \brief a setter for "controls"
   inline void SetControls(map<int,DomainConstraint*>*__controls){controls=*__controls;}
   //! \brief a setter for "enclosing_counters"
   inline void SetCounters(vector<string>* __counters) {enclosing_counters=*__counters;}
   inline void SetAffineConstraints(vector<vector<Inequation*> >* __affine)          {affine_constraints=*__affine;}
   inline void SetNonAffineConstraints(vector<vector<Inequation*> >* _non_affine)          {non_affine_constraints=*_non_affine;}
public:
   //! \brief a setter for "domain"
   inline void SetDomain(Condition* __domain) {domain=__domain;}
private:

//! \name Advanced setters
   /*!
   * \brief An initialization method.
   * Caller is initialized to empty values : no read variables, no written ones, no enclosing loops, domain == true. If we make analogy with a statement, it will be a NOP statement which is not enclosed by any control.
   \see Set(), References()
   */
   void Initialize(void);


   /*!
   * \brief An advanced setter for the entire important properties.
   * \param __id : the wanted identifier for the object.
   * \param __counters : a vector of all loops which enclose the references.
   * \param __domain : the domain-condition.
   * \param __rr : a vector of all read references.
   * \param __wr : a vector of all written references.
   * \param __org : the original statement these references come from.
   */
   void Set(int __id,vector<string> * __counters,Condition* __domain,
   vector<Read_Reference*>* __rr, vector<Written_Reference*>* __wr,Statement* __org);
   
   /*!
   * \brief Another advanced setter for the entire important properties. Used only when instances of the class "References" are build from an AST of a program.
   * \param __id : the wanted identifier for the object.
   * \param __counters : a vector of all loops which enclose the references.
   * \param __domain : the domain-condition.
   * \param __controls : a vector of all enclosing control structures.
   * \param __rr : a vector of all read references.
   * \param __wr : a vector of all written references.
   * \param __org : the original statement these references come from.
   */
   
   void Set(int __id,vector<string> * __counters,Condition* __domain, map<int, DomainConstraint*>* __controls,vector<Read_Reference*>* __rr, vector<Written_Reference*>* __wr,Statement* __org);


public:

   vector<vector<Inequation*> >*   GetAffineConstraints()          {return &affine_constraints;}
   vector<vector<Inequation*> >*   GetNonAffineConstraints()          {return &non_affine_constraints;}


//! \name Building References
   /*!
   * \brief One of the two constructors, the only way to build a valid objects and to modify properties on. This constructor will call the "References::Set" method.
   * \see Set()
   */
   References(int __id,vector<string> * __counters,Condition* __domain,
   vector<Read_Reference*>* __rr,
vector<Written_Reference*>*__wr,Statement* __org);

   References(int __id,Condition* __domain);


void   AddReadReference(Read_Reference*);
void   AddWrittenReference(Written_Reference*);
void   AddInnerCounter(string);
   /*!
   * \brief One of the two constructors, used when we want build objects from AST of programs. This constructor will call the "References::Set" method.
   * \see Set()
   */
   References(int __id,vector<string> * __counters,Condition* __domain, map<int, DomainConstraint*>* __controls,
   vector<Read_Reference*>* __rr, vector<Written_Reference*>* __wr,Statement* __org);


~References();

//! \name Getters
   //! \brief A getter for "domain"
   Condition* GetDomain(void) {return domain;}
   //! \brief A getter for "enclosing_counters"
   vector<string>* GetCounters(void) {return &enclosing_counters;}
   //! \brief A getter for "id"
   int GetStmtID(void) {return id;}
   //! \brief A getter for "origin"
   Statement* GetOrigin(void) {return origin;}
   //! \brief A getter for "read_variables"
   vector<Read_Reference*>* GetRV(void) {return &read_variables;}
   //! \brief A getter for "written_variables"
   vector<Written_Reference*>* GetWV(void) {return &written_variables;}
   //! \brief A getter for "controls"
   map<int,DomainConstraint*>* GetControls(void) {return &controls;}


inline void SetOrigin(Statement* org) {origin=org;}


//! \name Printing
   /*!
   * \brief Dump the current object into a string.
   * The printing can occur in different levels. With each level we can print some properties (supposed to be correctly computed). For example, we can print with level 0 after building an valid object. We can also print with level 1 if all sources of all read variables were coumputed, otherelse, we got a segmentation fault.
   * \param __level : It expresses the properties we want to print.
   * \return a string with the result of printing.
   */
   string Print_str(int __level);
   
   string   Print_str(int level,Options* options);
   /*!
   * \brief Print, into the standard output file, the properties of the caller.
   * It is a personnelized version of Print_str(int __level).
   * \param __level : It expresses the properties we want to print.
   * \see Print_str()
   */
   inline void Print(int __level) {cout<<Print_str(__level);}



//! \name Frequently Asked Questions
   /*!
   * \brief Says if the cells of "array" belong to the written variables.
   * \param __array : can be a scalar, or an array.
   */
   bool IsWrittenIn(string __array);
   
   /*!
   * \brief Says if the cells of "array" belong to the read variables.
   * \param __array : can be a scalar, or an array.
   */
   bool IsRead(string __array);

//!\name Miscellaneous
   /*!
   \brief Generate a schedule to optimize the computation of definitions for read variables.
   \param __id[] : the identifier of the statement reading the memory cell.
   \param __prog[in] : the program, we look for potential sources in.
   */

   void GenerateOptimalWayForFlowDependenciesComputing(int __id, Program* __prog);

   /*!
   *\brief It Collects all read and all written scalars.
   *\param __written : a set all written scalars will be added in.
   *\param __read : a set all read scalars will be added in.
   */
   void ReferencedScalars(set<string>* __written, set<string>* __read);
   
   /*!
   * \brief Remove loop counters, global parameters, and doubles from the list of read and written variables.
   * In fact, there no need to compute the source of loop counters and global parameters. That's because we know from where values of loop counters come from, and we know also that global parameters are not modified by any instruction. For doubles, it is costlier to compute their sources multiple times (because of the overhead of calling the solver and building the quasts).
   */
   
   void AvoidCountersParametersDoubles(vector<string>* parameters);


   /*!
   *\brief It compute the definition of all read variables and array cells.
   *\param __global_parameters : the list of all global parameters.
   *\param __pgm : the AST of the original program (needed to evaluate the textual order between statements).
   *\param __sa : activate/deactivate the structural analysis.
   */

//    void ComputeDefinitions(vector<string>* __global_parameters, vector<References*>* __references, vector<vector<bool> >* __textual_prec, LDemonstrator* __global_ppts, bool __sa,bool alphas);

   void ComputeDefinitions(vector<string>* __global_parameters, vector<References*>* __references, vector<vector<bool> >* __textual_prec, LDemonstrator* __global_ppts, Options* __options);


   void ComputeDefinitions(vector<string>* __global_parameters, vector<References*>* __references, void* __textual_prec, int (*fct) (void*, int,int),LDemonstrator* __global_ppts, Options* __options);

   string ToHtml(string __work_dir,string __name_prefix, string __target_frame,Options* op,bool __useless);
   void   ToHtml(string  __work_dir, string __name_prefix,string __target,Options* op);

   /*!
   \brief rename variables by anothers in all properties.
   */
   void RenameNewParameters();
   /*!
   \brief Generate a code to draw dependence graph nodes, in GraphViz format.
   \param __rank : show the rank of the assignment ?
   \param __label : show its label ?
   \param __assign : show the original line of code ?
   \return a string containing the result.
   */
   string GenerateGraphVizNodes(bool __rank, bool __label, bool __assign);
   /*!
   \brief Generate a code to draw dependence graph edges, in GraphViz format.
   \param __prog[in] : a link to the AST of the program.
   \param __print_condition[in] : print the condition of the validity of edges ?
   \param __print_relation[in] : print the whole relation of dependence ?
   \param __rename[in] : add the prefix "current_" to loop counters ?
   \param __bottom[in] : generate a bottom node for not initialized memory cells with their validity-conditions
   \return a string containing the result.
   */
   string GenerateGraphVizEdges(Program* __prog,bool __print_condition, bool __print_relation,bool __rename, bool __bottom);
   string GenerateGraphVizNodes(GraphPrinterOptions op);
   string GenerateVCGNodes(GraphPrinterOptions op);
   
   void      NormalizeDomainConstraints(vector<string>* parameters);
   void      TagIndexes();
   void      CollectReferencedScalars(set<string>*__written,set<string>*__read);
   void      AddPragmaConstraint(Inequation*);
   inline vector<Inequation*>*   GetPragmaConstraints(void)      {return &pragma_constraints;}
   void      SetPragmaConstraints(vector<Inequation*>* __pc)      {pragma_constraints=*__pc;}
   void   BuildInstanceWiseEdges(Graph* graph);
   void   LookForParallelLoops(vector<string>* parallel_loops,vector<References*>program);


};
};
#endif


