
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
   \file adaan_quast.h
        \author Christophe Alias
   \author Marouane Belaoucha
   This file contains the modelisation of the class Quast.
*/

#ifndef ADAAN_QUAST_H
#define ADAAN_QUAST_H

#include "condition.h"
#include "index.h"
#include "program.h"
#include <map>
#include <piplib/piplib64.h>
#include <contextquast.h>
#include <flattenquast.h>


namespace fada{
class Condition;
class FADA_Index;
class Program;
class FlatQuast;
class ContextQuast;   //because of cyclic includes

/*!
\class Quast
\brief QUAST (QUasi-Affine Selection Tree).
Quast are used to provide a disjunctive polyhedra describing solutions of a parametric linear system. a QUAST is a binary tree, leaves contain solutions, when internal nodes held an affine inequation which leads us to choose one valid branch.
For the seek of generality we remplaced inequations in internal nodes by conditions.
*/

class Quast{
private:

//! \name For all nodes
   bool is_leaf;            /*!< Am i a leaf ?*/

//! \name For internal nodes only
   Condition* condition;      /*!< for internal nodes, we got the condition*/
   Quast* then_part;         /*!< a sub-quast guarded by "condition" */
   Quast* else_part;         /*!< a sub-quast guarded by NOT "condition" */

//! \name For leaves only
   bool is_empty;            /*!< Ok i am a leaf, but am i empty ?*/
   int assignment;            /*!< all right, i'm a non-empty leaf. I'll provide the source statement */
   FADA_Index* vertex;         /*!< The computed instance of "assignment" */
   int deep;            /*!< useless property */
   vector<string> loop_counters;

//! \name Setters
   //! \brief A setter for "is_leaf"
   void SetLeaf(bool val) {is_leaf=val;}
   //! \brief a setter for "is_empty"
   void SetEmpty(bool val) {is_empty=val;}
   //! \brief a setter for "condition"
   void SetCondition(Condition* cond) {condition=cond;}
   //! \brief a setter for "then_part"
   void SetThenPart(Quast* quast) {then_part=quast;}
   //! \brief a setter for "else_part"
   void SetElsePart(Quast* quast) {else_part=quast;}
   //! \brief a setter for "vertex"
   void SetVertex(FADA_Index* __vertex) {vertex=__vertex;}
   //! \brief a setter for "assignment"
   void SetAssignment(int val) {assignment=val;}
   //! \brief a setter for "deep"
   void SetDeep(int val) {deep=val;}
   //! \brief a setter for "loop_counters"
   void SetCounters(vector<string>* counters) {loop_counters=*counters;}

//! \name Advanced Setters
   /*!
   \brief An advanced setter for the entire object.
   \return The caller will be set to an empty leaf (also called, bottom).
   */
   void Set();
   
   /*!
   \brief An advanced setter for the entire object.
   \param __index : mapped to "assignment"
   \param __deep : mapped to "deep"
   \param __counters : mapped to "loop_counters"
   \param __vertex : mapped to "vertex"
   \return The caller will be set to a non-empty leaf. It will contain the solution (for us, the dependent operation).
   */
   void Set(int __index,int _deep, vector<string>* __counters, FADA_Index* __vertex);
   
   /*!
   \brief An advanced setter for the entire object.
   \param __cond : mapped to "condition".
   \param __then : mapped to "then_part".
   \param __else : mapped to "else_part".
   \return Create a root quast, the condition is "__cond". The then-subquast is "__then", and "__else" as the else-subquast.
   */
   void Set(Condition* __cond, Quast* __then, Quast* __else);
   
   /*!
   \brief An advanced setter for the entire object.
   \param __index : mapped to "assignment".
   \param __deep : mapped to "deep".
   \param __params : parameters may be referenced by the nodes of the quast.
   \param __quast     : the quast to be imported.
   \param __new_param : sometimes PIP add some new parameters.
   PS: This function has been written By Christophe Alias.
   */
   void Set(int __index, int __deep,vector <string>* __counters, vector<string>* __params,PipQuast* __quast, map <int, Expression*> __new_param);
   
   /*!
   \brief Set all properties to their NULL values.
   */   
   void Initialize(void);

//!   \name GraphViz
   /*!
   \brief generate a GraphViz graph which reprensents the quast.
   \param parent_node_name[in] : the name of the variable reprensenting the parent.
   \see ToGraphViz()
   */
   string ToGraphViz(string parent_node_name);
   /*!
   \brief generate a GraphViz graph which reprensents the quast.
   \see ToGraphViz()
   */
   string ToGraphViz(void);


Quast* Compress(Condition*, Quast*, string*);
// Quast* EliminateUnsatisfiableLeaves(Condition*, vector<string>*);
// Quast* FastSimplify(bool);



public:

 ~Quast();

//! \name Building
   /*!
   \brief Create an empty leaf
   */
   Quast();
   
   /*!
   \brief Create a non-empty quast.
   \param __index : mapped to "assignment"
   \param __deep : mapped to "deep"
   \param __counters : mapped to "loop_counters"
   \param __vertex : mapped to "vertex"
   \return The caller will be set to a non-empty leaf. It will contain the solution (for us, the dependent operation).
   */
   Quast(int __index,int __deep, vector<string>* __counters, FADA_Index* __vertex);
   
   /*!
   \brief A constructor for creating a root quast.
   \param __cond : mapped to "condition".
   \param __then : mapped to "then_part".
   \param __else : mapped to "else_part".
   \return Create a root quast, the condition is "__cond". The then-subquast is "__then", and "__else" as the else-subquast.
   */
   Quast(Condition* __cond, Quast* __then, Quast* __else);
   
   /*!
   \brief import a quest from Pip structures.
   \param __index : mapped to "assignment".
   \param __deep : mapped to "deep".
   \param __params : parameters may be referenced by the nodes of the quast.
   \param __quast     : the quast to be imported.
   PS: This function has been written By Christophe Alias.
   \see Set()
   */
   Quast(int __index,int __deep, vector <string>* __counters, vector<string>* __params,PipQuast* __quast);
   

//! \name Getters
   /*!
   \brief A getter for "vertex"
   */
   FADA_Index* GetVertex(void) {return vertex;}
   //! \brief A getter for "assignment"
   int GetAssignment(void) {return assignment;}
   //! \brief A getter for "deep"
   int GetDeep(void) {return deep;}
   //! \brief A getter for "loop_counters"
   vector<string>* GetCounters(void) {return &loop_counters;}
   //! \brief A getter for "then_part"
   Quast* GetThenPart(void) {return then_part;}
   //! \brief A getter for "else_part"
   Quast* GetElsePart(void) {return else_part;}
   //! \brief A getter for "condition"
   Condition* GetCondition(void) {return condition;}

//! \name Frequently Asked Questions !
   //! \brief Am i a leaf ?
   bool IsLeaf(void) {return is_leaf;}
   //! \brief Am i an internal node ?
   bool IsIf(void) {return !is_leaf;}
   //! \brief Am i a bottom ?
   bool IsEmpty(void) {return is_leaf && is_empty ;}
   //! \brief checks whether there is some empty leaves
   bool       ContainBottoms(void);

//! \name Duplicating
   /*!
   \brief Copying
   \return provides an independent copy of the current object.
   \see Clone()
   It is very time-consuming function, another alternative to copy quasts is by using Clone()
   */
   
   Quast* Copy(void);
   
   /*!
   \brief Copying
   \return The current and the returned quast share all internal nodes. Only roots are partially independent.
   A faster way to obtain a copy of a quast. but result is not completely independent from the caller. To have independent objects, you may use Copy()
   \see Copy()
   */
   Quast* Clone(void);
   

//! \name Printing
   /*!
   \brief It prints (in if-then-else scheme) the quast into the standard output file.
   \param __indent : the indentation.
   */
   void Print(string __indent="");
   
   /*!
   \brief It prints (in if-then-else scheme) the quast into string.
   \param __indent : the indentation.
   \return a string containing the result of printing.
   */
   string Print_str(string __indent="");
   
   /*!
   \brief It prints the quast into string. The quast is printed in a flatten scheme.
   \param __indent : the indentation.
   \param __root : am i root ?
   \return a string containing the result of printing.
   */
   string Print_flatten(string __indent,bool root);
   
   /*!
   \brief It creates a .dot file (to be compiled with dot)
   \param __filename : the name of the file to write in
   */
   void ToGraphVizFile(string __filename);

   /*!
   \brief It generates adges for the dependence graph (in GraphViz format).
   \param __pgm[in] : the analyzed program.
   \param __read_stmt[in] : read statement id.
   \param __read_ref[in] : read reference id.
   \param __cond[in] : the dependence validity condition.
   \param __print_condition[in] : print condition of validity ?
   \param __print_relation[in] : print the whole relation of dependence ?
   \param __rename[in] : a prefix "current_" has been added to loop counters ?
   \param __bottom[in] : generate edges for the node "bottom" ?
   \return GraphViz code lines.
   */
   string Generate_GraphViz_Edges(Program* __pgm, int __read_stmt, int __read_ref,Condition* __cond, bool __print_condition, bool __print_relation, bool __rename,bool __bottom);
   /*!
   \brief Generate a peice of code, that can check wether the computation is correct.
   \param __ArrayID[in] : the name the read array.
   \param __Access[in] : its access-function
   \param __prefix[in] : an indentation, added to each code-line.
   \return a C code.
   */
   string Generate_C_Test_Code(string __ArrayID, FADA_Index* __AccessIndex,string __prefix);


//! \name Substituting
   /*!
   \brief It substitutes some scalars by anothers in the quast.
   \param __mapping : the mapping of scalars to be substituted 
   */
   void Substitute(map<string, string>* __mapping);
   
   /*!
   \brief It substitutes some scalars by expressions and provides another quast.
   \param __mapping : the mapping of scalars to be substituted.
   \return the result of substitution.
   */
   Quast* Substitute(map<string, Expression*>* __mapping);

//! \name Simplification


   /*!
   \brief Eliminate unsatifiable nodes in a given context.
   \param __environment[in] : the context (assumed to be tautology)
   \param __parameters[in] : symbolic constants
   */

   Quast* Simplify(vector<vector<Inequation*> > __environemnt, vector<string>* __param);

   /*!
   \brief Eliminate useless nodes.
   \param __use_hard_one[in] : activate or not the use of a costlier approach.
   \remarks concerned nodes are those which can be written as : "if(c) then Q else Q"  they can be simplified to the quast "Q"
   */
   Quast*   EliminateRedondantLeaves(bool __use_hard_one);


//!\name Miscellaneous
   /*!
   \brief Remplace all bottoms in the current quast by "__quast".
   \param __quast[in] : a regular quast.
   \return the result of substitution of empty leaves by "__quast".
   \remarks This can optimize the computation of a source when we sort smartly a "plan" for potential source operations following their priotrity. Otherelse, we can use Max().
   \see Max()
   */
   Quast*    PutToBottoms(Quast* __quast);
   /*!
   \brief It computes the maximum between two quasts.
   \param __q[in] : a regular quast.
   \return the maximum between the current quast and "__q"
   */
   Quast*    Max(Quast* __q);
   /*!
   \brief It traduces a context quast into a quast. simplifications can be performed.
   \param __cq[in] : the conext quast to be traduced.
   \param __context : the context of the quast (constraints assumed to be tautologies)
   \param __parameters[in] : symbolic constants.
   \return traduced quast.
   */
//    Quast*    Simplify(ContextQuast* __cq, vector<Inequation*> __context,vector<string>* __parameters);
   Quast*    Simplify(ContextQuast* __cq, vector<vector<Inequation*> > __context,vector<string>* __parameters);


   /*!
   \brief It tries to compact conditions on internal nodes having the same else-part.
   \return a compacted quast, but semantically equivalent.
   \see Uncompress()
   */
   Quast* Compress(void);
   
   /*!
   \brief Split conditions into a atomic inequations. some sub-quasts have to be duplicated.
   \return an uncompressed quast, but semantically equivalent
   \see Compress()
   */
   Quast* Uncompress(void);

   /*!
   \brief Collect non empty leaves.
   */

   vector<FADA_Index*> GetVertices(void);

   void   Flatten(vector<vector<Inequation *> > env, FlatQuast* q);
   void   Flatten( FlatQuast* q);
   void   DropAddedVariables(int nb_original);

   void   LookForLoopParallelism(vector<string> original, vector<string>* parallel ,vector<References*> program);
   void   SubstituteMaximumOfParameterValues(map<string,string> map);
};

Quast* SingleInequation(Condition*, Quast* __then, Quast* __else);
};
#endif



