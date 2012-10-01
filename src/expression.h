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

  \file   expression.h
  \author Christophe Alias
  \author Marouane BELAOUCHA
  \date   25/10/2007
  \brief  Represents a Presburger formulas. Atomic term can be : an integer, variable, array cell or a function call.

*/


#ifndef ADAAN_EXPRESSION_H
#define ADAAN_EXPRESSION_H


#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <iostream>
// #include <inequation.h>



using namespace std;


namespace fada{
class Inequation;
/*!
\enum T_leaf
\brief Contain a description for the class Expression.
*/
enum T_leaf
{
FADA_value,      /*!< Value */
FADA_variable,      /*!< variable : describes both : loop-counters, symbolic constants and scalar*/
FADA_array,      /*!< array cell*/
FADA_function      /*!< function call */
};

enum T_operation
{
FADA_ADD,      /*!< Addition operator*/
FADA_SUB,      /*!< Substraction operator*/
FADA_MUL,      /*!< Multiplication operator*/
FADA_DIV,      /*!< Divide operator operator*/
FADA_MOD      /*!< Modulo operator */
};



/*!
\class Expression
\brief A type for different kinds of an expression-leaf
Expressions are viewed as binary tries. Leaves represent atomic terms (numbers, variables, array-cells and function calls), where internal nodes represent the operation between the left child and the right one.
*/

class Expression
{
private:

//! \name For all nodes
bool is_leaf;   //!< Am i a leaf ?
int id_stmt;   //!< the ID of the statement reading this expression.
vector<Expression*> iteration;   //!< and its instance.

int deep;   //!< useless information for FADA analysis.
int line;   //!< useless information for FADA analysis.
int column;   //!< useless information for FADA analysis.

//! \name For leaves only
T_leaf t_leaf;   //!< kind of information held by this leaf
int value;   //!< if leaf is a number, we got its value here
string name;   //!< used as name for : variables, arrays and functions
vector<Expression*> FuncArg_ArrayIndex;   //!< list of arguments for functions, and the access function for arrays.

//! \name For internal nodes only
T_operation operation;      //!< ok not atomic expression : so we need the operator
Expression* right__child;   //!< right  child
Expression* left__child;   //!< left child







//!\name Simple setters
   //! \brief A setter for "right__child"
   inline void SetRightChild(Expression* child) {right__child=child;}
   //! \brief A setter for "left__child"
   inline void SetLeftChild(Expression* child) {left__child=child;}
   //! \brief A setter for "FuncArg_ArrayIndex"
   inline void SetArguments(vector<Expression*>* v) {FuncArg_ArrayIndex=*v;}
   //! \brief A setter for "FuncArg_ArrayIndex"
   inline void SetIndex(vector<Expression*>* v) {FuncArg_ArrayIndex=*v;}
   //! \brief A setter for "FuncArg_ArrayIndex"
   inline void SetExpressionsVector(vector<Expression*>* v){FuncArg_ArrayIndex=*v;}
   //! \brief A setter for "name"
   inline void SetName(string str) {name=str;}

void Initialize(void);
     Expression();


//!\name Normalization
   /*!
   \brief Normalize an arithmetic term
   \return The normalized expression.
   \remarks The expression has to be an arithmetic term.
   \see NormalizeTerm(), SerializeTerm(), MultiplyNormalizedExpressions(),MultiplyTermByNormalizedExpression
   */
   Expression* NormalizeTerm(void);
   
   /*!
   \brief It Serializes an arithmetic term.
   \return Serialized term.
   \remarks The expression has to be an arithmetic term.
   Serialization will be done by deeping the always the right child. Any left-child can have only deep of level 1 or 0
   \see NormalizeTerm(), SerializeTerm(), MultiplyNormalizedExpressions(),MultiplyTermByNormalizedExpression
   */
   Expression* SerializeTerm(Expression** __LastInternNode);
   /*!
   \brief Multiply two normalized expressions.
   \param [in]__exp : the multiplier.
   \return The result of the multiplication in a normalized form.
   \remarks The current and the expression "__exp" are supposed to be normalized.
   \see NormalizeTerm(), SerializeTerm(), MultiplyNormalizedExpressions(),MultiplyTermByNormalizedExpression
   */
   Expression* MultiplyNormalizedExpressions(Expression* __exp);
   /*!
   \brief Multiply an arithmetic term by a normalized expression.
   \param [in]__exp : the multiplier.
   \return The result of the multiplication in a normalized form.
   \remarks The current object is supposed to be an arithmetic term. __exp have to be a normalized expression.
   \see NormalizeTerm(), SerializeTerm(), MultiplyNormalizedExpressions(),MultiplyTermByNormalizedExpression
   */
   Expression* MultiplyTermByNormalizedExpression(Expression* __exp);
   Expression* one_pass_simplify(void);




public:

inline void Set(int __id,vector<Expression*>* __iter) {id_stmt=__id; iteration=*__iter; }


//! \name Getters
   //!\brief A getter for "deep"
   int         GetDeep()      {return deep;}
   //!\brief A getter for "ietration
   vector<Expression*>* GetIteration(void) {return &iteration;}
   //!\brief A getter for "id_stmt"
   int GetReadStmt(void) {return id_stmt;}

   void GetLocation(int*__line, int*__column) {*__line=line; *__column=column;}

//! \name Setters
   //! \brief a setter for "deep"
   void         SetDeep(int __deep);   
   //! \brief a setter for "iteration"
   void    SetIteration(vector<Expression*>* iter) {iteration=*iter;}

   void SetLocation(int __line, int __column) {line=__line; column=__column;}





//!\name Building expressions
   /*!
   \brief expression is a number.
   \param __val : the value of the number
   */
   Expression(int __val);
   /*!
   \brief expression is a scalar
   \param __var : the name of the variable
   */
   Expression(string __var);
   /*!
   \brief expression is an array cell or a function call
   \param __kind : to choose a function call or an array-cell-access
   \param __name : the name of the function, or of the array.
   \param __vect : arguments for functions, and access-function for array-cells.
   */
   Expression(T_leaf __kind, string __name, vector<Expression*>* __vect);
   /*!
   \brief expression is an operation
   \param __l_c : the first operator for the operation
   \param __op : the operation
   \param __r_c : the second operator
   */
   Expression( Expression* __l_c, T_operation __op, Expression* __r_c);
   /*!
   \brief duplicating expression
   \param __exp : the expression to be duplicated
   */
   Expression(Expression* __exp);
   ~Expression();

//!   \name Advanced Setters

   /*!
   \brief expression is a number.
   \param __val : the value of the number
   */

   void Set(int __val);
   /*!
   \brief expression is a scalar
   \param __var : the name of the variable
   */
   void Set(string __var);
   void Set(T_leaf, string, vector<Expression*>*);
   void Set( Expression* l_c,T_operation op, Expression* r_c);

//! \name Simple getters
   //! \brief Am i a leaf ?
   inline bool IsLeaf(void) {return this->is_leaf;}
//! \name Only for leaves
   //! \brief Am i a number?
   inline bool IsValue(void) {return is_leaf &&this->t_leaf==FADA_value;}
   //! \brief Am i an array-cell?
   inline bool IsArray(void) {return is_leaf && this->t_leaf==FADA_array;}
   //! \brief Am i a function call.
   inline bool IsFunction(void) {return is_leaf && this->t_leaf==FADA_function;}
   //! \brief Am i a variables
   inline bool IsVariable(void) {return is_leaf && this->t_leaf==FADA_variable;}

//! \name Only for internal nodes
   //! \brief Am i an add operation
   inline bool IsAddition(void) {return !is_leaf && operation==FADA_ADD;}
   //! \brief Am i a substruction
   inline bool IsSubstraction(void) {return !is_leaf && operation==FADA_SUB;}
   //! \brief Am i a multiplication.
   inline bool IsMultiplication(void) {return !is_leaf && operation==FADA_MUL;}
   inline bool IsModulo(void) {return !is_leaf && operation==FADA_MOD;}
   //! \brief Am i a division
   inline bool IsDivision(void) {return !is_leaf && operation==FADA_DIV;}
   //! \brief A getter for "right__child"
   inline Expression* GetRightChild(void) {return this->right__child;}
   //! \brief A getter for "left__child"
   inline Expression* GetLeftChild(void) {return this->left__child;}
   //! \brief A getter for "operation"
   inline T_operation GetOperation(void) {return this->operation;}
   //! \brief A getter for "FuncArg_ArrayIndex"
   vector<Expression* > * GetArguments(void) {return &this->FuncArg_ArrayIndex;}
   //! \brief A getter for "FuncArg_ArrayIndex"
   vector<Expression* > * GetIndex(void) {return &this->FuncArg_ArrayIndex;}
   //! \brief A getter for "name"
   inline string GetVariableName(void) {return this->name;}
   //! \brief A getter for "name"
   inline string GetFunctionName(void) {return this->name;}
   //! \brief A getter for "name"
   inline string GetArrayName(void) {return this->name;}
   //! \brief A getter for "value"
   inline int GetValue(void) {return value;}




//! \name Duplicating
   /*!
   \brief Cloning
   \return a copy of the caller.
   \remarks The clone share with the caller the quasi-totality of references. Only root nodes are independent.
   \remarks This method is faster than Copy().
   \see Copy()
   */
   Expression* Clone(void);
   /*!
   \brief Copying
   \return a copy of the caller.
   \remarks The copy is completely independent.
   \remarks This method can be a bit costiler in time for complicated long expressions.
   \see Clone()
   */
   Expression* Copy(void);

//! \name Printing
   /*!
   \brief It prints the current object into the standard output file.
   */
   void       Print(void);
   /*!
   \brief It returns the result of printing in a string.
   */
   string       Print_str(void);
   /*!
   \brief It prints the expression, and distingishes the non-affine entities.
   \param __var : list of variables.
   \param __param : list of parameters.
   \return the result of printing
   Non affine entities will be followed by a string if the format '<S:I>', where :(1) S is the identifier of the statement who references the expression; and (2) I is its instance.
   \see PrintAll(), Instanciate()
   */
   string      PrintAll_str(vector<string>* __var, vector<string>* __param);
   /*!
   \brief It prints the expression, and distingishes the non-affine entities.
   \param __var : list of variables.
   \param __param : list of parameters.
   Non affine entities will be followed by a string if the format '<S:I>', where :(1) S is the identifier of the statement who references the expression; and (2) I is its instance.
   \see PrintAll_str(), Instanciate()
   */
   inline void   PrintAll(vector<string>* var, vector<string>* param)   {cout<<PrintAll_str(var,param);}
   /*!
   \brief It prints the expression in a format that respects the language C.
   */
   string Generate_C_Code(void);



//!\name Collecting
   /*!
   \brief It collects all referenced variables.
   \param __scalars : all referenced scalars will be added here.
   */
   void ReferencedScalars(set<string>* __scalars);
      

//!\name Substitution
   /*!
   \brief Substitute scalars by expression
   \param __mapping : the mapping between variables to be remplaced by their new values.
   \return The result of substitution.
   \remarks The caller is not modified by this method. The result is given as a partially independent expression.
   */
   Expression* Substitute(map<string, Expression*>* __mapping);
   Expression* Substitute(map<string, Expression*>* __mapping,bool __use_less);
   
   /*!
   \brief Substitute scalars by anothers
   \param __mapping : the mapping between scalars the be subsituted and their new names.
   \remarks Substitution is performed on the same object.
   */
   void          Substitute(map<string, string>* __mapping);


//!\name Tagging
   /*!
   \brief Tag the expression by an operation.
   \param __id : the identifier of the statement
   \param __iter : its instance.
   */
   void Instanciate(int __id,vector<Expression*>* __iter);

//!\name Miscellaneous
   /*!
   \brief It checks whether the expression is the incrementation of a variable.
   \param __var : a name of a variable.
   \return expression = __var+1;
   */
   bool      IsInc(string __var);
   /*!
   \brief It checks whether expression is a term in a normalized formula.
   \return a boolean value to say if the current can be considred as a term in a normalized formula.
   A normalized formula is the sum of arithmetic terms. A term is the multiplication and/or the division of atomic expressions.
   */
   inline bool IsTermInNormalizedFormula(void)
   {return ((IsLeaf( )) ||
   (!IsLeaf( ) && (IsMultiplication( )||IsDivision( ))));
   }
   /*!
   \brief It checks whether expression is affine on parameters and variables.
   \param __var : list of variables.
   \param __param : list of parameters.
   */
   bool IsAffine(vector<string>* __var, vector<string>* __param);
   /*!
   \brief It checks whether the arithmetic term is affine or not
   */
   bool IsAffineTerm(bool*, vector<string>* var, vector<string>* param);
   /*!
   \brief It normalizes the expression.
   \return a normilized but semantically equivalent expression.
   The normilized is written as a sum of arithmetic terms?
   */
   
   Expression* Normalize(void);
   /*!
   \brief Simplify the expression.
   \return simplified but semantically equivalent expression.
   This method performs operations when the operators are known.
   */
   Expression* Simplify(void);
   /*!
   \brief
   \remarks This operator performs a syntaxic comparaison. Equivalent expressions, written differently will be considred as not equivalent.
   */
   bool operator == (Expression& e);
   /*!
   \brief It codes the affine expression into a vector of coefficients
   */
   vector<int> ToVector(vector<string>* counters,map<string,int>* counters_ranks,vector<string>* parameters, map<string,int>* parameters_ranks);

   bool   IsFuzzy(set<string>* fuzzy_variables);
   string   Print_Tex_str();
   string   Full_Tex_Print_str(vector<string>* var, vector<string> *param);
   Expression* PIPize(vector<string>* new_variables, vector<Inequation*>* new_inequations);

Expression   operator+(Expression&e2)   {return *(new Expression(this,FADA_ADD,&e2));}
Expression   operator+(Expression*e2)   {return *(new Expression(this,FADA_ADD,e2));}
Expression   operator+(int val)      {return *(new Expression(this,FADA_ADD,new Expression(val)));}


Expression   operator-(Expression&e2)   {return *(new Expression(this,FADA_SUB,&e2));}
Expression   operator-(Expression*e2)   {return *(new Expression(this,FADA_SUB,e2));}
Expression   operator-(int val)      {return *(new Expression(this,FADA_SUB,new Expression(val)));}

Expression   operator*(Expression&e2)   {return *(new Expression(this,FADA_MUL,&e2));}
Expression   operator*(Expression*e2)   {return *(new Expression(this,FADA_MUL,e2));}
Expression   operator*(int val)      {return *(new Expression(this,FADA_MUL,new Expression(val)));}

Expression   operator/(Expression&e2)   {return *(new Expression(this,FADA_DIV,&e2));}
Expression   operator/(Expression*e2)   {return *(new Expression(this,FADA_DIV,e2));}
Expression   operator/(int val)      {return *(new Expression(this,FADA_DIV,new Expression(val)));}

Expression   operator=(Expression&e)      {return *(e.Clone());}
Expression   operator=(Expression*e)      {return *(e->Clone());}
Expression   operator=(string str)      {return *(new Expression(str));}
// Expression   operator=(char* str)      {return *(new Expression((string)str));}
Expression   operator=(int val)      {return *(new Expression(val));}


Inequation   operator<(Expression&e)   ;//   {return *(new Inequation(this, FADA_LESS,&e));}
Inequation   operator<(Expression*e)   ;//   {return *(new Inequation(this, FADA_LESS,e));}
Inequation   operator<(int val)   ;//   {return *(new Inequation(this, FADA_LESS,new Expression(val)));}

Inequation   operator<=(Expression&e);//      {return *(new Inequation(this, FADA_LESS_EQ,&e));}
Inequation   operator<=(Expression*e);//      {return *(new Inequation(this, FADA_LESS_EQ,e));}
Inequation   operator<=(int val)   ;//   {return *(new Inequation(this, FADA_LESS_EQ,new Expression(val)));}

// Inequation   operator==(Expression&e)      {return *(new Inequation(this, FADA_EQ,&e));}
// Inequation   operator==(Expression*e)      {return *(new Inequation(this, FADA_EQ,e));}
// Inequation   operator==(int val)      {return *(new Inequation(this, FADA_EQ,new Expression(val)));}

Inequation   operator!=(Expression&e);//      {return *(new Inequation(this, FADA_NEQ,&e));}
Inequation   operator!=(Expression*e);//      {return *(new Inequation(this, FADA_NEQ,e));}
Inequation   operator!=(int val)   ;//   {return *(new Inequation(this, FADA_NEQ,new Expression(val)));}

Inequation   operator>(Expression&e)   ;//   {return *(new Inequation(this, FADA_GREATER,&e));}
Inequation   operator>(Expression*e)   ;//   {return *(new Inequation(this, FADA_GREATER,e));}
Inequation   operator>(int val)   ;//   {return *(new Inequation(this, FADA_GREATER,new Expression(val)));}

Inequation   operator>=(Expression&e);//      {return *(new Inequation(this, FADA_GREATER_EQ,&e));}
Inequation   operator>=(Expression*e);//      {return *(new Inequation(this, FADA_GREATER_EQ,e));}
Inequation   operator>=(int val)   ;//   {return *(new Inequation(this, FADA_GREATER_EQ,new Expression(val)));}




int      AddIndex(Expression* e){
   if(IsVariable() || IsArray() || IsFunction()){
      GetIndex()->push_back(e);
      t_leaf=FADA_array;
      return true;   //legal
      }
   return false;      //illegal
   }

int      AddArgument(Expression* e){
   if(IsVariable() || IsArray() || IsFunction()){
      GetArguments()->push_back(e);
      t_leaf=FADA_function;
      return true;   //legal
      }
   return false;      //illegal
   }


// 
// Expression*   operator+(Expression&e2)   {return new Expression(this,FADA_ADD,&e2);}
// Expression*   operator+(Expression*e2)   {return new Expression(this,FADA_ADD,e2);}

};

};
#endif


