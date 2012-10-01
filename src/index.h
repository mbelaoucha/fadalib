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
\file index.h
\author Christophe Alias
\author   Marouane Belaoucha
\brief This file contain a description for the class FADA_Index.
*/

#ifndef ADAAN_INDEX_H
#define ADAAN_INDEX_H
#include "expression.h"
#include "condition.h"


namespace fada{

class Condition;   //because of cyclic includes.
/*!
\class FADA_Index
\brief This class models an index function to array accesses. It is implemented as a vector of Expressions
\see Expression
*/

class FADA_Index{
private:
vector<Expression*> index;      /*!< The index' function*/


/*!
\brief A setter to "index"
*/
void SetIndex(vector<Expression*>*v) {index=*v;}

/*!
\brief Initialization function to the NULL function.
The property "index" will be cleaned after the call. This function is called by all constructors.
*/
void Initialize(void);

// FADA_Index();



public:

   ~FADA_Index();

//! \name Building

   /*!
   \brief Construct from  vector of expressions.
   \param __v[in] : the multidimentianl function.
   \see Set()
   */
   
   FADA_Index(vector<Expression* > *  __v);
/*!
\brief A constructor which creates a NULL function.
*/

   FADA_Index(void);
   /*!
   \brief Construct from  vector of expressions.
   \param __v[in] : the multidimentianl function.
   \see FADA_Index()
   */
   void Set(vector<Expression* > * __v);
        void Set();

//! \name Getters
   /*!
   \brief A getter for "index"
   */
   vector<Expression*>* GetIndex(void) {return &index;}
   /*!
   \brief Get the dimentional of the function.
   */
   int          GetSize(void) {return index.size();}
   /*!
   \brief Get a item.
   \param __i[in] : the entry of the wanted item.
   \return the "__i"th item of "index"
   */
   Expression*    GetItem(int __i);

//! \name Printing
   /*!
   \brief Print the index (in a readable format) into the standard output file.
   \see Print_str(),PrintAll_str(), PrintAll()
   */
   void          Print(void);
   /*!
   \brief Print the index (in a readable format) into a string.
   \return a string containing the result of printing.
   \see Print(),PrintAll_str(), PrintAll()
   */
   string          Print_str(void);
   /*!
   \brief Dump the index (in a readable format) into a string.
   \param __var[in] : List of variables.
   \param __param[in] : List of parameters.
   \return a string containing the result of printing.
   \remarks the non affine entities will be followed the operation which referenced it.
   \see Print_str(),PrintAll_str(), PrintAll()
   */
   string          PrintAll_str(vector<string>* __var, vector<string>* __param);
   /*!
   \brief Dump the index (in a readable format) into a string.
   \param __var[in] : List of variables.
   \param __param[in] : List of parameters.
   \return a string containing the result of printing.
   \remarks the non affine entities will be followed the operation which referenced it.
   \see PrintAll_str(),Print_str(),Print()
   */
   void          PrintAll(vector<string>* var, vector<string>* param)   {cout<<PrintAll_str(var,param);}
   /*!
   \brief Print the index into a string, in a C format.
   \return a string containing the result of printing.
   */
   string         Generate_C_Code(void);



//! \name Substitution
   /*!
   \brief rename variables.
   \param __mapping : the mapping between the variables to be remplaced an their new names.
   \remarks renaming will be performed on the same object.
   \see Substitute()
   */
   void          Substitute(map<string, string>* __mapping);
   /*!
   \brief Substitute variables by expressions.
   \param __mapping : the mapping between the variables to be remplaced an their new values
   \return An index, result of substitution.
   \see Substitute()
   */
   FADA_Index*       Substitute(map<string, Expression*>* __mapping);

//! \name Duplicating
   /*!
   \brief It provides a fully independent copy of the index.
   */
   FADA_Index*       Copy(void);


//! \name Lexicographic operations
   /*!
   \brief It evaluates the LEXICOGRAPHIC-GREATER operator with "__common" common dimentionals.
   \param __index[in] : the function to be compared with the current.
   \param __common[in] : the common dimentionals which  have the same values.
   \remarks "this lex> __index"
   \see Lex_Eq(), Lex_Less()
   */

   Condition*   Lex_Sup(FADA_Index* __index, int __commun);
   /*!
   \brief It evaluates the LEXICOGRAPHIC-EQUALITY operator with "__common" common dimentionals.
   \param __index[in] : the function to be compared with the current.
   \param __common[in] : the common dimentionals which  have the same values.
   \remarks "this lex= __index"
   \see Lex_Sup(), Lex_Less()
   */
   Condition*   Lex_Eq(FADA_Index* __index, int __commun);
   
   /*!
   \brief It evaluates the LEXICOGRAPHIC-GREATER operator with "__common" common dimentionals.
   \param __index[in] : the function to be compared with the current.
   \remarks "this lex< __index"
   \see Lex_Eq(), Lex_Less()
   */
   Condition*    Lex_Less(FADA_Index* __index );
   /*!
   \brief It evalutates the condition for which two indexes are equal.
   \param __index[in] : the function to be compared with the current.
   \see operator==(), Lex_Eq()
   */
   Condition*    Equal(FADA_Index* __index);
   /*!
   \brief It evalutates the condition for which two indexes are equal.
   \param __index[in] : the function to be compared with the current.
   \see operator==(), Equal(), Lex_Eq()
   */
   Condition*    operator==(FADA_Index& __index);
   /*!
   \brief It evalutates the condition for which two indexes are equal.
   \param __index[in] : the function to be compared with the current.
   \see operator==(), Equal(), Lex_Eq()
   */
   Condition*    operator==(FADA_Index* __index);
   Condition*    operator<(FADA_Index* __index);

//!\name Miscellaneous
   FADA_Index*      ComputeDistance(vector<string>*, vector<string>*,bool);
   /*!
   \brief Collecting scalars.
   \param __scalars[inout] : all referenced scalars will be added here.
   */
   void         ReferencedScalars(set<string>* __scalars);
   /*!
   \brief Tag the index by an operation (an instance of a statement).
   \param __read_id[in]   : the identifier of the statement.
   \param __read_iter[in] : its instance.
   */
   void         Instanciate(int __read_id, vector<Expression*>* __read_iter);
};

};
#endif



