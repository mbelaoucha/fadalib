
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
\author Christophe Alias
\author Marouane Belaoucha
\file adaan_assignment.h
\brief This file contain the description of Assignment
*/


#ifndef ADAAN_ASSIGNMENT_H
#define ADAAN_ASSIGNMENT_H

#include "index.h"
#include "program.h"
#include "controlstructure.h"
#include "read_reference.h"





namespace fada{

class Program;
class Control;
class FADA_Index;
class Read_Reference;

/*!
\class Assignment
\brief It represents assignments.
*/

class Assignment{

private:


//! \name Data to be Given
   string          assign_left_hand_side_array; //!<The name of the array in the left-hand-side of the assignment.
   FADA_Index*       assign_left_hand_side_index; //!<The access function of the cell of the array in the LHS of assignment.
   Expression*    assign_right_hand_side;      //!< the expression in the RHS of assignment.

   vector<string>      lhs_array_plus;
   vector<FADA_Index*>   lhs_index_plus;

//! \name Data to be computed
   vector<Read_Reference*>    references;   //!<All read references (by the rhs expression, and the lhs access-function)
   
   vector<Control*>   surrounded_by_controls;   //!< All enclosing controls
   vector<Condition*>    normalized_domain;   //!< The domain constraints
   vector<string>          loop_counters;      //!<List of all enclosing loops

//! \name Optional data
   int rank;                  //!< The ID of the statement
   string label;                  //!< The label


//! \name Simple setters
   //! \brief A setter for "loop_counters"
   void SetLoopCounters(vector<string> lc){loop_counters=lc;};
   //! \brief A setter for "assign_hand_side_array"
   void SetLHSArrayName(string s) {assign_left_hand_side_array=s;}
   //! \brief A setter for "assign_left_hand_side" and "assign_left_hand_side_index"
   void SetLHS(string name, FADA_Index* __index)
   {assign_left_hand_side_array=name;assign_left_hand_side_index=__index;};
   //! \brief A setter for "assign_right_hand_side"
   void SetRHS(Expression* __rhs) {assign_right_hand_side = __rhs;}
   //! \brief A setter for "surrounded_by_controls"
   vector<Control*>* GetSurroundingControls(void) {return &surrounded_by_controls;}
   //! \brief Push back a loop counter ID into the list "loop_counters"
   void AddCounter(string str) {loop_counters.push_back(str);}

//!\name Advanced setters
   //! \brief Initialize all properties to their NULL values.
   void Initialize(void);
   /*!
   \brief Build an assignment
   \param __lhs[in] : the assigned array name.
   \param __lhs_index[in] : its access fucntion.
   \param __rhs[in] : the right hand side of the assignment.
   */
   void Set(string __lhs, FADA_Index* __lhs_index, Expression* __rhs);
   //! \brief a private and useless constructor.
   Assignment( );

//!\name Collecting
   /*! 
   \brief Collect read references (from the RHS expression and the LHS index)
   \see CollectReferences()
   */
   void CollectReferences(void);
   /*!
   \brief Collect read references from a given expression.
   \param __expr[in] :
   */

   void CollectReferences(Expression* __expr);
   /*!
   \brief Collect read references from an index
   \param __index[in] : the considered index.
   */
   void CollectReferences(FADA_Index* __index);
   /*!
   \brief Add a read array cell to the list of collected references.
   \param __name[in] : the name of the array.
   \param __index[in] : the access function
   */
   void AddRHSReference(string __name, FADA_Index* __index);

public:

//!\name Setters
   //! \brief A setter for "label"
   void SetLabel(string str) {label=str;}
   //! \brief A setter for "normalized_domain"
   void SetDomain(vector<Condition*>* vect) {normalized_domain=*vect;}
   //! \brief A setter for "rank"
   void SetRank(int val) {rank= val;}

//!\name Getters
   //! \brief A getter for "references"
   vector<Read_Reference*>* GetReferences(void) {return &references;}
   //! \brief A setter for "label"
   string GetLabel(void) {return label;}
   //! \brief A setter for "normalized_domain"
   vector<Condition*>* GetDomain(void) {return &normalized_domain;}
   //! \brief A setter for "loop_counters"
   vector<string>* GetLoopCounters(void) {return &loop_counters;};
   //! \brief A setter for "assign_left_hand_side_array"
   string GetLHSArray(void) {return assign_left_hand_side_array;}
   //! \brief A setter for "assign_left_hand_side_index"
   FADA_Index* GetLHSIndex(void) {return assign_left_hand_side_index;}
   //! \brief A setter for "assign_right_hand_side"
   Expression* GetRHSExpression(void) {return assign_right_hand_side;}
   //! \brief A setter for "rank"
   int GetRank(void) {return rank;}





   Assignment(string , FADA_Index* , Expression* );
   Assignment(string, Expression*);
   Assignment(Expression* __rhs);
   ~Assignment();

//! \name Dumping
   //! \brief readable printing into a string
   string Print_str();
   //! \brief complete printing into a string
   string Full_Print_str();
   //! \brief readable printing into a string
   string Pretty_print_str();
   //! \brief printing in a C format into a string
   string Generate_C_Code(void);
   //! \brief readable printing into the standard output file
   inline void Print()   {cout<<Pretty_print_str();}
   /*! 
   \brief Generate some HTML pages summerizing results of computation.
   \param dir[in] : the output directory.
   */
   string ToHtml(string dir);
   /*!
   \brief Generate the dependence graph edges respecting the GraphViz format.
   \param __pgm[in] : the AST of the program.
   \param print_condition[in] : print dependence validity conditions ?
   \param print_relation[in] : print the whole relation of dependence ?
   \param rename[in] : rename loop counters by adding the prefix "current_"
   \param bottom[in] : generate edges for the node "bottom"
   \param alphas[in] : for future using.
   */
   string Generate_GraphViz_Edges(Program* __pgm,bool print_condition, bool print_relation, bool rename,bool bottom,bool alphas);
   
   



//!\name Miscellaneous
   //! \brief Add a control to the enclosing controls.
   void AddControl(Control* __c);
   /*! 
   \brief Evaluate a condition for which there is a lexicographic order between the caller and "__a"
   \param __a[in] : a regular assignment.
   \return A disjunction of conditions.
   */
   vector<Condition*> LexicographicPrecedence(Assignment* __a);
   //! \brief Some preprocessing task before running the FADA
   void Prepare(void);
   //! \brief FADA processing
   void ComputeAllDefinitions(vector<string>* global_parameters, Program* prog);

   //! \brief Rename variables.
   void RenameNewParameters(map<string, string> *__mapping);
   /*! 
   \brief Collect all referenced scalars.
   \param __written[inout] : a set of all collected written scalars.
   \param __read[inout] : a set of all collected read scalars.
   */
   void ReferencedScalars(set<string>* __written, set<string>* __read);

   //! \brief Rename variables.
   void Rename(map<string, string>* __mapping);
   /*!
   \brief Generate a C test code in order to check whether sources are correctly computed.
   \param prefix[in] : an identation, added to each code-line.
   */
   string Generate_C_Test_Code(string prefix);

//!\name Questions :)
   //! Is the assignment an incrementation of the variable "__variable" ?
   bool IsInc(string __variable);
   //! Am i a scalar initialization ?
   bool IsScalarInitialization(string* __scalar);
   //! Am i enclosed by some controls ?
   bool IsControled(void);
   void Add_LHS_PLUS(string array, FADA_Index* index);
   vector<string>*   get_lhs_array_plus()   {return &lhs_array_plus;};
   vector<FADA_Index*>*   get_lhs_index_plus()   {return &lhs_index_plus;};
};
};
#endif



