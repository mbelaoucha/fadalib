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
\file inequation.h
\brief This file containt the description of the class Inequation.
*/

#ifndef ADAAN_INEQUATION_H
#define ADAAN_INEQUATION_H


#include "expression.h"
#include "condition.h"
#include <vector>
#include <map>



namespace fada{
/*!
\enum T_predicate
\brief Operators for comparing.
*/
enum T_predicate
{
FADA_EQ,      /*!< EQual*/
FADA_NEQ,      /*!< Not EQual */
FADA_LESS,      /*!< LESS */
FADA_LESS_EQ,      /*!< LESS or EQual */
FADA_GREATER,      /*!< GREATER */
FADA_GREATER_EQ,   /*!< GREATER or EQual */
FADA_NO_COMP      /*!< NONE */
};

#define NO_SIMPLIFICATION      -1
#define ALWAYS_TRUE         0
#define ALWAYS_FALSE         1
#define FIRST_IMPLY_SECOND      2
#define SECOND_IMPLY_FIRST      3

#define NO_IMPLY       0
#define FIRST_IMPLIES_SECOND    1
#define SECOND_IMPLIES_FIRST   2



class Condition;   //because of cyclic includes

/*!
*\class Inequation
* \brief It represents inequations.
*/
class Inequation
{
private:

   Inequation();
void Initialize(void);


//! \name all object
   //! Am i a trivial value ?   
   bool is_value;      
//! \name have sense if is_value=true
   bool value;      //!< If yes, the boolean value

//! \name have sense if is_value=false
   Expression* right_hand_side;   //!<The right hand side of the inequation
   Expression* left_hand_side;   //!<The left hand side of the ienquation
   T_predicate predicate;         //!<comparaison operator


//! \name Simple Setters
   //! \brief a setter for "left_hand_side"
   void SetLHS(Expression* exp) {left_hand_side=exp;};
   //! \brief a setter for "right_hand_side"
   void SetRHS(Expression* exp) {right_hand_side=exp;};
   //! \brief a setter for "value"
   void SetValue(bool val) {value=val;};
   //! \brief a setter for "predicate"
   void SetPredicate(T_predicate pred) {predicate=pred;}


public:

   /*!
   \brief It codes affine inequation into a vector of coefficients.
   \param [int]__counters : List of all variables.
   \param [in]__map_counters : The mapping between counters and their identifiers.
   \param [in]__parameters : List of all parameters
   \param [in]__map__parameters : parameters mapped to their identifiers.
   \remarks The inequation must be affine.
   \see IsAffine()
   */
   vector<int> ToVector(vector<string>* __counters, map<string, int>* __map_counters,vector<string>* __parameters, map<string, int>* __map_parameters);


//! \name Building
   /*!
   \brief Building an inequation via predicate.
   \param __lhs : The left hand side of the compraison.
   \param __p : The comparaison' operator.
   \param __rhs : Its right hand side.
   \see Set()
   */   
   Inequation( Expression* __lhs,T_predicate __p, Expression* __rhs);
   /*!
   \brief Building a trivial value inequation
   \param __val : false or true ?
   \see Set()
   */   
   Inequation(bool __val);
   /*!
   \brief Building from an existing inequation
   \param __ineq : inequation to be copied
   \see Set()
   */   

   Inequation(Inequation* __ineq);
   ~Inequation();

//! \name Simple Getters
   //! \brief a getter for "right_hand_side"
   Expression* GetRHS(void) {return right_hand_side;}
   //! \brief a getter for "left_hand_side"
   Expression* GetLHS(void) {return left_hand_side;}
   //! \brief a getter for "value"
   bool GetValue(void) {return value;}
   //! \brief a getter for "is_value"
   bool IsValue(void) {return is_value;};
   //! \brief a getter for "predecate"
   T_predicate GetPredicate(void) {return predicate;}


//! \name Advanced Setters
   /*!
   \brief Advanced setter for building a valid inequation
   \param __lhs : The left hand side of the compraison.
   \param __p : The comparaison' operator.
   \param __rhs : Its right hand side.
   */   
   void Set( Expression* __lhs,T_predicate __p, Expression* __rhs);
   /*!
   \brief Setting inequations into trivial values
   \param __val : false or true ?
   \see Inequation()
   */   
   void Set(bool __val);





//! \name Duplicating
   /*!
   \brief Cloning inequations
   \return a clone of the caller
   \remarks the clone share some references with the caller. 
   \remarks It's the faster way to obtain a copy of an inequation
   \see
   */
   Inequation* Clone (void);
   /*!
   \brief provide a full copy.
   \return a copy of the caller.
   \remarks the copy is completely independent from the caller.
   \remarks It's a bit costlier than Clone().
   \see Clone()
   */
   Inequation* Copy (void);

   /*!
   \brief Exporting to ADaAn_COndition.
   \returns a semantically equivalent condition.
   \remarks ADaAn_NEQ (not equal) will be splitted into disjunction of two inequations.
   \see WithoutNEQ()
   */
   Condition* ToCondition(void);

   /*!
   \brief Exporting to ADaAn_COndition.
   \returns a semantically equivalent condition.
   \remarks ADaAn_NEQ (not equal) will be splitted into disjunction of two inequations.
   \see ToCondition()
   */
   Condition* WithoutNEQ(void);

//! \name Negation
   /*!
   \brief Negate the ienquation.
   \param __smart : no-meaning, it's here to over-charge the other Negate().
   \remarks negation is performed by redefining the predicate.
   \see Negate()
   */
   Inequation* Negate (bool __smart);
   /*!
   \brief Negate the ienquation.
   \see Negate()
   */
   Condition* Negate (void);

//! \name Frequently Asked Questions
   /*!
   \brief Checks whether the inequation is affine on variables and parameters.
   \param __var[in] : the considered variables.
   \param __param[in] : the considered  symbolic constants.
   Simply check if the LHS and the RHS are affine on same variables and same parameters.
   */
   bool IsAffine(vector<string>* __var, vector<string>* __param);
   /*!
   \brief It checks whether the inequation is trivial.
   \return trivial or not.
   \param __value[out] : if "true" returned we got the boolean value of the inequation via this parameter.
   \param __param[in] :
   Trivial inequations are never upheld inequations whatever the value of parameters or variables. Example :  i < i+1, i <= i
   */
   bool IsTrivial(bool* __value, vector<string>* __param);


// int   CanBeSimplifiedWith(Inequation* ineq, vector<Inequation*>* param);

//! \name Substituting
   /*!
   \brief Remplace variables by expressions.
   \param __mapping[in] : the mapping between variables and their substitution values.
   \see Substitute()
   */
   Inequation* Substitute(map<string, Expression*>* __mapping);
   /*!
   \param __mapping[in] : the mapping between variables and their substitution values.
   \remarks Substitution is performed on the caller.
   \see Substitute()
   */
   void Substitute(map<string, string>* __mapping);
   


//! \name Printing
   string   Print_Tex_str(void);
   string   Full_Tex_Print_str(vector<string>* var, vector<string> *param);
   /*!
   \brief Generate the inequation in C format
   \return a string containing the result of printing
   \see Print(), Print_str(), PrintAll_str(), PrintAll().
   */
   string Generate_C_Code(void);
   /*!
   \brief Printing into the standard output file.
   \see Generate_C_Code(), Print_str(), PrintAll_str(), PrintAll().
   */
   void Print ( void );
   /*!
   \brief Printing into a string
   \return a string containing the result of printing
   \see Print(), Generate_C_Code(), PrintAll_str(), PrintAll().
   */
   string Print_str ( void );
   /*!
   \brief full dumping of the ineqution.
   \param __var[in] : list of variables.
   \param __param[in] : list of parameters.
   \return a string containing the result of printing
   \remarks non affine entities will be followed by a special print (viewing the operation wich referenced the entity)
   \see Print(), Print_str(), Generate_C_Code(), PrintAll().
   */
   string PrintAll_str(vector<string>* var, vector<string>* param);
   /*!
   \brief full dumping of the ineqution in the standard output file.
   \param __var[in] : list of variables.
   \param __param[in] : list of parameters.
   \remarks non affine entities will be followed by a special print (viewing the operation wich referenced the entity)
   \see Print(), Print_str(), PrintAll_str(), Generate_C_Code().
   */
   void PrintAll(vector<string>* var, vector<string>* param)   {cout<<PrintAll_str(var,param);}



//! \name Tagging
   /*!
   \brief Tag the ienquation by an instance of a statement.
   \param __id[in] : the identifier of the statement.
   \param __iiter[in] : its instance.
   \see Tag()
   */
   void Instanciate(int __id, vector<Expression*>* __iter);
   /*!
   \brief Tag the ienquation by an instance of a statement.
   \param __id[in] : the identifier of the statement.
   \param __iiter[in] : its instance.
   \remarks an alias to Instanciate()
   \see Instanciate()
   */
   inline void Tag(int __id,vector<Expression*>* __iter) {Instanciate(__id, __iter);}

//! \name Simplification and Normalization
   /*!
   \brief It checks wether the current inequation is implied or imply "__ineq"
   \param __ineq[in] : the second ineqation. 
   \param __var[in] : list of variables
   \param __param[in] : list of parameters.
   \return yes or not one inequation can imply the other.
   \param __which_one[out] : if the result is "true", this variable identify which inequation impies the other.
   \remarks both inequations have to be affine.
   */
   bool Imply(Inequation* __ineq,vector<string>* __var, vector<string>* __param, bool* __which_one);
   /*!
   */
//    int CanBeSimplifiedWith(Inequation*, vector<string>* param);
//! \name Miscellaneous

   vector<string> ReferencedVariables(vector<string>* var);
   /*!
   \brief Collecting scalars.
   \param __scalars [inout] : collected scalars will be added to this set.
   */
   void ReferencedScalars(set<string>* __scalars);
   /*!
   \brief 
   \param __lc[in] a variable.
   \remarks This method handles inequations written like : __lc <[=] UpperBound[-1].
   \return NULL if the inequation does not express that kind of constraints. The upper bound otheresle.
   */
   Expression*   UpperBound(string __lc);

   bool   IsFuzzy(set<string>* fuzzy_variables);
   Inequation*   PIPize(vector<string>* new_var, vector<Inequation*>* new_ineq);
   int   SimplifyWith(Inequation* ineq, vector<string>* param);
   int   SimplifyWith(vector<vector<Inequation*> >* __ineq,vector<string>*param);
   int   SimplifyWith(vector<Inequation*>* __ineq,vector<string>*param);


Inequation   operator=(Inequation&e)      {return *(e.Clone());}
Inequation   operator=(Inequation*e)      {return *(e->Clone());}
Inequation   operator=(bool val)      {return *(new Inequation(val));}
Condition   operator&&(Inequation*i);   //{return *(new Condition(new Condition(this)),FADA_AND,new Condition(i));}
Condition   operator&&(Inequation &i);
Condition   operator||(Inequation *i);
Condition   operator||(Inequation &i);

// Inequation   operator=(Expression&e)      {return *(new Inequation(&e,FADA_NEQ,new Expression(0)));}
// Inequation   operator=(Expression*e)      {return *(new Inequation(e,FADA_NEQ,new Expression(0)));}

};
/*!
\brief compute the opposite of "__op"
*/
T_predicate  Opposite(T_predicate __op);
};
#endif


