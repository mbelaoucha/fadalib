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

/*!
\author Christophe Alias
\author Marouane Belaoucha
\file condition.h
\brief This file contain the description of the class Condition, and the type T_Logical_operator.
*/

#ifndef ADAAN_CONDITION_H
#define ADAAN_CONDITION_H

#include "inequation.h"
#include "quast.h"




namespace fada{
class Inequation;
class Quast;   //because of cyclic includes

/*!
\enum T_Logical_operator
\brief boolean operators. 
*/
enum T_Logical_operator
{
FADA_AND,   //!< logical AND operator
FADA_OR,   //!< logical OR operator
FADA_NOT,   //!< logical NOT operator
FADA_NOLP   //!< NO Logical oPerator :-)
};

/*!
\class Condition
\brief It models conditions.
Conditions are implemented as a binary tree. internal nodes contain operators, when leaves held atomic inequations.
*/
class Condition{
private:

//!\name for all nodes
bool is_leaf;       //!< am i a leaf ?
//!\name only for leaves
Inequation* ineq;//!< an inequation
//!\name only for internal nodes.
Condition* left_child;//!< the left side of the operation
Condition* right_child;//!< the right side operator for the logical operation
T_Logical_operator operation;//!< the logical operation




/*! \name Setters */
//! a setter for operation
inline void SetOperation(T_Logical_operator __op)    {operation=__op;}
//! a setter for left_child
inline void SetLeftChild(Condition* __child)    {left_child=__child;}
//! a setter for right_child
inline void SetRightChild(Condition* __child)    {right_child=__child;}
//! a setter for ineq
inline void SetInequation(Inequation* __I)    {ineq=__I;}

/*! \name Advanced setters */
//! Build the operation between two conditions
void Set( Condition* __lc,T_Logical_operator __op, Condition* __rc);
//! Copying condition
void Set(Condition* __c);
//! Building atomic condition
void Set(Inequation* __ineq);


void Initialize(void);
vector<Condition*> get_terms(void);
  Condition();

public:
 ~Condition();


//! \name Constructors
   /*!
   \brief construct a condition which helds atomic inequation.
   */
   Condition(Inequation* __ineq);
   /*!
   \brief construct a copy of a condition.
   \remarks The copy is not completely independent. The two condition may share some data.
   */
   Condition(Condition* __cond);
   
   /*!
   \brief construct a 
   */
   Condition( Condition*, T_Logical_operator,Condition*);



//! \name Getters   
   
   //! \brief am i a leaf ?
   bool IsLeaf(void) {return is_leaf;}
   
   /*! 
   \brief a getter for the right child
   \return the right child
   \remarks It, may, returns useless information when the caller is a leaf.
   */
   
   Condition* GetRightChild(void) {return right_child;}
   /*! 
   \brief a getter for the left child
   \return the left child
   \remarks It, may, returns useless information when the caller is a leaf.
   */
   
   Condition* GetLeftChild(void) {return left_child;}
   
   /*!
   \brief a getter for "ineq"
   \return the inequation held by the leaf.
   \remarks It returns a buggy inequation when the caller is not a leaf.
   */
   Inequation* GetInequation(void) {return ineq;}
   /*! a getter for the logical operator
   \see IsOR(), IsNot(), IsAND()
   */
   T_Logical_operator GetOperation (void) {return operation;}
   
   
   /*! \brief am i an AND operation ?
   \remarks make sure, the caller is not a leaf
   \see IsOR(), IsNot(), GetOperation()
   */
   bool IsAND(void) {return operation == FADA_AND;}
   
   /*! \brief am i an OR operation ?
   \remarks make sure, the caller is not a leaf
   \see IsAND(), IsNot(), GetOperation()
   */
   bool IsOR (void) {return operation == FADA_OR; }
   
   
   /*! \brief am i an NOT operation ?
   \remarks make sure, the caller is not a leaf
   \see IsAND(), IsOR(), GetOperation()
   */
   bool IsNot(void) {return operation == FADA_NOT; }



/*!
\brief It tests whether the condition is a part of a DNF (disjunctive normal form).
\remarks The condition is assumed to be normalized. No check is performed.
*/
bool IsTermInNormalizedCondition(){return IsLeaf( ) || (!IsLeaf() && IsAND( ));}


//! \name Duplicating

   /*!
   \brief Cloning
   \return A copy of the caller
   \remarks The clone share all nodes with the caller. Only root nodes are independent.
   */
   Condition* Clone( );
   
   /*!
   \brief Copying
   \return A copy of the caller
   \remarks the copy is completely independent from the caller.
   */
   Condition* Copy ( );



//! \name Printing

   /*! \brief Print the condition to the standard output file.
   \see Print_str()
   */
   void Print ( );
   /*! 
   \brief Print the condition into a string
   \return a string containing the result of printing.
   */
   string Print_str( );

   /*!
   \brief Printing in a C format.
   \return printing respecting the C-format.
   */
   string Generate_C_Code(void);


   string PrintAll_str(vector<string>* var, vector<string>* param);
   void PrintAll(vector<string>* var, vector<string>* param)   {cout<<PrintAll_str(var,param);}


//! \name Negating
   /*!
   \brief Provide a condition which is a negation of the caller.
   \return the negation of the caller
   Negation is performed by adding a NOT-node as root to the caller. No propagation is performed, if you want interpret them you can use the method "EliminateNotOperations"
   \see EliminateNotOperations(), Negate()
   */
   Condition* FastNegation(void);
   
   
   /*!
   \brief Negate and propagate the negation. Operator ADaAn_NOT is systematically traduced.
   \see FastNegation()
   */
   Condition* Negate(void);
   
   /*!
   \brief Negate and propagate the negation. Operator ADaAn_NOT is systematically traduced.
   \param __smart : useless.
   \see FastNegation()
   */
   Condition* Negate(bool __smart);
   /*!
   \brief Computing negation
   \return the result of negation without NOT operations.
   \see Negate(), FastNegation()
   */
   Condition* NegateTermANDDestributeNotOperations(void);
   


//! \name Normalizing
   /*!
   \brief It computes an in DNF form normalized condition.
   \return a disjunction of conjunctions of inequations.
   \remarks The caller have to dont containt NOT operations.
   \see DistributeNormalizedCondition(), DistributeTermByNormalizedCondition()
   */
   Condition* DNForm(void);
   
   /*!
   \brief It destributes two normalized conditions.
   \param __cond : a normalized condition to be destributed on the caller terms.
   \return the normalized AND between the caller and "__cond"
   \remarks both the caller and "__cond" have to not containt NOT operations.
   \see DNForm(), DistributeTermByNormalizedCondition()
   */
   Condition* DistributeNormalizedCondition(Condition* __cond);
   
   /*!
   \brief It destributes a normalized condition (the caller) by a DNF term.
   \param __cond : a DNF term (conjunction of inequations).
   \return the normalized AND between the caller and "__cond"
   \remarks both the caller and "__cond" have to not containt NOT operations.
   \see DNForm(), DistributeNormalizedCondition()
   */
   Condition* DistributeTermByNormalizedCondition(Condition* __cond);

//! \name Substituting
   /*!
   \brief substitute some scalars by anothers.
   \param __mapping : scalars to be substituted.
   \remarks It substitutes scalars on the same object. The substitution is a simple renaming process.
   */
   void Substitute(map<string, string>* __mapping);
   
   /*!
   \brief substitute some scalars by expressions
   \param __mapping : scalars to be substituted.
   \return a condition after the substitution of variables.
   */
   Condition* Substitute(map<string, Expression*>* __mapping);


   /*!
   \brief It traduces all NOT operations in the in condition.
   \return a semantically equivalent condition which dont contain any NOT operation.
   \see FastNegation()
   */
   Condition* EliminateNotOperations(void);
   /*!
   \brief It traduces all NOT operations in the in condition.
   \param __smart : useless
   \return a semantically equivalent condition which dont contain any NOT operation.
   \see FastNegation()
   */
   Condition* EliminateNotOperations(bool __smart);
   
   /*!
   \brief It splits all Not Equal inequations into a disjunction LESS and GREATER operation
   \return a semantically equivalent condition which dont contain any NotEqual inequation
   */
   Condition* DistributeNEQInequations(void);
   
   /*!
   \brief Traducing NOT operations
   \return a semantically equivalent condition without any NOT operation.
   */
   Condition* TraduceNEQInequations(void);

//! \name Advanced getters
   /*!
   \brief split the caller into DNF terms.
   \return The list of all DNF terms.
   \remarks The caller is supposed to be normalized.
   \see SplitToTerms()
   */
   vector<Condition*> GetTerms(void);

   /*!
   \brief An advanced boolean function on conditions.
   \param __val : a boolean
   \return It checks whether the condition: is atomic, has a trivial the value __val
   */
   bool IsValue(bool __val);
   
   /*!
   \brief split the caller into DNF terms.
   \return The list of all DNF terms.
   \remarks The caller is supposed to be normalized.
   \see GetTerms()
   */
   vector<Condition*> SplitToTerms(void);

   /*!
   \brief Collecting referenced scalars
   \param[out]__scalars : says whether the simplified condition has a trivial value.
   */
   void ReferencedScalars(set<string>* __scalars);

   /*!
   \brief Split inequations
   \return the vector of all inequations
   \remarks Current object must be a DNF term.
   \see SplitTerms(), GetTerms()
   */
   vector<Inequation*> GetTermInequations(void);
   
   /*!
   \brief Check wether the condition is trivial.
   \return a semantically equivalent condition without any NOT operation.
   */
   bool IsTrivial(vector<string>* __parameters, bool* __value);

//! \name Simplificating 
   /*!
   \brief Simplify conditions.
   \param[in]__param : all symbolic constants
   \param[out]__trivial : says whether the simplified condition has a trivial value.
   \param[out]__value : if simplified is trivial, we got its value in this parameter.
   \return The simplified condition.
   \see SimplifyTerm()
   */
   Condition* Simplify(bool* __trivial, bool* __value,vector<string>* __param);
   
   /*!
   \brief Simplify DNF term.
   \param[in]__param : all symbolic constants
   \return The simplified condition.
   \remarks The current condition have to be a DNF term.
   \see Simplify()
   */
   Condition* SimplifyTerm(vector<string>* __param);







//! advanced setters
/*!
\brief Tagging conditions
\param[in]__id : the identifier of the satement referencing all constraints of current condition.
\param[in]__iter : the instance of __id being referencing constraints.
\remarks this method will tag all constraints of the current condition.
*/

void Instanciate(int __id, vector<Expression*>* __iter);




Condition   operator&&(Condition&c)      {return *(new Condition(this,FADA_AND,&c));}
Condition   operator&&(Condition*c)      {return *(new Condition(this,FADA_AND,c));}

Condition   operator||(Condition&c)      {return *(new Condition(this,FADA_OR,&c));}
Condition   operator||(Condition*c)      {return *(new Condition(this,FADA_OR,c));}

Condition   operator!()      {return *(this->FastNegation());}

Condition   operator=(Inequation &i)      {return *(new Condition(&i));}
Condition   operator=(Inequation*i)      {return *(new Condition(i));}

};

//void DeclareVariables(vector<string> var);
//void DeclareParameters(vector<string> param);
/*!
\brief Split affine from non affine constraints.
\param[in]__all : constraints to be sorted.
\param[out]__affine : affine constraints from __all.
\param[out]__non_affine : non affine constraints from __all.
\param[in]__var : variables to be considered.
\param[in]__param : symbolic constants
*/
void SortInequations(vector<Inequation*>* __all, vector<Inequation*>* __affine, vector<Inequation*>* __non_affine, vector<string>* __var, vector<string>* __param);
   
}
#endif



