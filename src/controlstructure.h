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

#ifndef ADAAN_CONTROLSTRUCTURE_H
#define ADAAN_CONTROLSTRUCTURE_H

#include "condition.h"
/*!
\file adaan_controlstructure.h
\author Marouane Belaoucha
\brief This file contain the description of the class Control, and the type FADA_Control
*/


namespace fada{
/*!
\enum FADA_Control
\brief It models the kind of controls can be supported.
*/
enum FADA_Control
{
ADaAn_FOR,      /*!< for loop*/
ADaAn_IF,      /*!< conditional*/
ADaAn_IF_ELSE,      /*!< alternative */
ADaAn_WHILE,      /*!< while loop   */
ADaAn_NONE      /*!< none */
};


class LDemonstrator;
class Condition;
class Inequation;   //because of cyclic includes

/*!
\class Control 
\brief It models control structures.
*/

class Control{
private:

//! \name Only for loops
   string loop_counter;      //!< the identifier of the loop counter
//! \name For "for-loops" only 
   Expression* for_lower_bound; //!< the lower bound
   Expression* for_upper_bound; //!< the upper bound
//! \name for while-loops and conditionals 
   Condition* if_while_condition;   //!< while or if condition

//! \name for if-then-else
   int else_first_assign;      //!< the id of the first statement enclosed by the else-part.
   int else_last_assign;      //!< the id of the last statement enclosed by the else-part.

//! \name For any instance
   FADA_Control control_kind;   //!< the kind of control
   int first_assign;      //!< the id of the first enclosed statement
   int last_assign;      //!< the id of the last enclosed statement

//! \name properties can be computed
   vector<vector<Inequation*> > affine_domain;   //!< The normalized affine domain constraints of this control.
   vector<vector<Inequation*> > non_affine_domain;   //!< The normalized non affine domain constraints of this control.
   
   vector<vector<Inequation*> > affine_else_domain;   //!< The normalized affine domain constraints for statements enclosed by the else-part.
   vector<vector<Inequation*> > non_affine_else_domain;//!< The normalized non affine domain constraints for statements enclosed by the else-part.

//! \name Setters
   //! \brief A setter for "affine_domain"
   inline void SetAffineInequations(vector<vector<Inequation*> >* __ineq) {affine_domain=*__ineq;}
   //! \brief A setter for "non_affine_domain"
   inline void SetNonAffineInequations(vector<vector<Inequation*> >* __ineq) {non_affine_domain=*__ineq;}
   //! \brief A setter for "affine_else_domain"
   inline void SetAffineElseInequations(vector<vector<Inequation*> >* __ineq) {affine_else_domain=*__ineq;}
   //! \brief A setter for "non-affine_else_domain"
   inline void SetNonAffineElseInequations(vector<vector<Inequation*> >* __ineq) {non_affine_else_domain=*__ineq;}
   //! \brief A setter for "control_kind"
   inline void SetControlKind(FADA_Control control) {control_kind=control;};
   //! \brief A setter for "loop_counter"
   inline void SetCounterName(string name) {loop_counter=name;};
   //! \brief A setter for "for_upper_bound"
   inline void SetUpperBound(Expression* __exp) {for_upper_bound=__exp;}
   //! \brief A setter for "for_lower_bound"
   inline void SetLowerBound(Expression* __exp) {for_lower_bound=__exp;}
   //! \brief A setter for "if_while_condition"
   inline void SetCondition(Condition* __cond) {if_while_condition=__cond;}
   //! \brief A setter for "first_assign" and "last_assign"
   void SetEnclosedAssigns(int assign1, int assign2) {first_assign=assign1; last_assign=assign2;}
   //! \brief A setter for "else_first_assign" and "last_else_assign"
   void SetElseEnclosedAssigns(int assign1, int assign2){else_first_assign=assign1; else_last_assign=assign2;}

//! \name Advanced Setters
   /*!
   \brief Set all properties to their NULL values.
   */
   void Initialize(void);
   /*!
   \brief Create a "for" control
   \param __lc[in] : the loop counter' identifier.
   \param __lb[in] : the lower bound.
   \param __ub[in] : the upper bound.
   */
   void Set(string __lc, Expression* __lb, Expression* __ub);
   /*!
   \brief Create a "while" control
   \param __lc[in] : its loop counter' indetifier.
   \param __c[in] : its condition.
   */
   void Set(string __lc, Condition* __c);
   /*!
   \brief Create a "if" control
   \param __c[in] : its condition.
   */
   void Set(Condition* __c);
   /*!
   /*!
   \brief private and useless constructor
   */
   Control();
public:

Condition* GetDomainConstraints(int);
Condition* GetRegularDomain(void);

void      SwitchToIfElse();


Control(string, Expression*, Expression*);
Control(string, Condition*);
Control(Condition*);

    ~Control();

//! \name Getters
   //! \brief A getter for "first_assign" and "last_assign"
   void GetEnclosedAssignments(int *assign1, int *assign2){*assign1=first_assign;*assign2=last_assign;}
   //! \brief A getter for "else_first_assign" and "else_last_assign"
   void GetElsePartAssignments(int *assign1, int *assign2){*assign1=else_first_assign;*assign2=else_last_assign;}
   //! \brief A getter for "control_kind"
   FADA_Control GetControlKind(void) {return control_kind;}
   //! \brief A getter for "loop_counter"
   string GetLoopCounter(void) {return loop_counter;}
   //! \brief A getter for "for_upper_bound"
   Expression* GetForUpperBound(void) {return for_upper_bound;}
   //! \brief A getter for "for_lower_bound"
   Expression* GetForLowerBound(void) {return for_lower_bound;}
   //! \brief A getter for "if_while_condition"
   Condition* GetCondition(void) {return if_while_condition;}
   //! \brief A getter for "loop_counter"
   string GetCounter(void) {return loop_counter;}
   //! \brief A getter for "affine_domain"
   vector<vector<Inequation*> >* GetAffineDomain(void) {return &affine_domain;}
   //! \brief A getter for "affine_else_domain"
   vector<vector<Inequation*> >* GetAffineElseDomain(void) {return &affine_else_domain;}
   //! \brief A getter for "non_affine_domain"
   vector<vector<Inequation*> >* GetNonAffineDomain(void) {return &non_affine_domain;}
   //! \brief A getter for "non_affine_else_domain"
   vector<vector<Inequation*> >* GetNonAffineElseDomain(void) {return &non_affine_else_domain;}

//! \name Frequently Asked Questions
   //! \brief Am i a "for" loop ?
   bool IsForLoop(void) {return control_kind==ADaAn_FOR;}
   //! \brief Am i a "while" loop ?
   bool IsWhileLoop(void) {return control_kind==ADaAn_WHILE;}
   //! \brief Am i a loop ?
   bool IsLoop(void) {return control_kind==ADaAn_FOR || control_kind==ADaAn_WHILE;}
   //! \brief Am i an conditional ?
   bool IsIf(void) {return control_kind==ADaAn_IF;}
   //! \brief Am i an alternative ?
   bool IsIfElse(void) {return control_kind==ADaAn_IF_ELSE;}



Condition*   GetRegularDomain(int __id, vector<string>* __loop_counters, vector<string>* __param,LDemonstrator* __loop_ppts);

//! \name Printing
   /*!   
   \brief Print the control into a string.
   */
   string Print_str(int what_to_print=0);
   /*!
   \brief Print in a readable way the control into a string.
   */
   string PrettyPrint_str(void);
   /*!
   \brief Generate an appropriate control in a C format.
   */
   string Generate_C_Code(void);

//!\name Miscellaneous
   //! \brief rename variables.
   void Substitute(map<string,string>* __mapping);
   //! \brief Normalize domain constraints and sort them into affine and non affine constraints.
   void NormalizeConditions(int id,vector<string>* var, vector<string>* parameters);
   //! \brief Tagg all constraints by the read operation (an instance of a statement)
   void TagAllInequations(int id, int __deep, vector<string>* var);
   //! \brief Collect scalars
   void ReferencedScalars(set<string>*);
};
};
#endif



