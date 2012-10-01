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
\file elementarydependence.h
\author Marouane Belaoucha
\brief This file contains the definition of the class ElementaryDependence
*/

#ifndef ELEMENTARYDEPENDANCE_H
#define ELEMENTARYDEPENDANCE_H

#include <quast.h>
#include <ldemonstrator.h>
#include <options.h>

namespace fada{
class Quast;
class References;
class FADA_Index;
class Program;
class LDemonstrator;


/**
* \class ElementaryDependence
* \brief It describes a dependence between two statements at one level of precedence.
* A level of a precedence defines one DNF-term in the condition of lexicographic order between the dependent operations.
* The aim of this class is the compute the dependence between two statements in a level of precedence. That is done in two step algorithm : 1) recover and build appropriate constraints (using properties :"stmt1", "stmt2" and "deep"). 2) Compute the QUAST describing the dependence.
* If all built constraints are affine, the exact source can be computed and given in the property "quast". Otherelse, an approximation is computed in the property "quast", and some extra information is filled in properties: "properties" and "parameter_of_maximum".
*/


class ElementaryDependence{
private :
//! \name Absolutly required information to build objects
   int stmt1;            /*!< One of the two dependent statements (in the case of flow dependence stmt1 is the read statement). Used to recover and parameterize to context constraints. */
   int stmt2;            /*!< The second dependent statement (it is the write statement in the case of flow dependence). Used to recover the domain of the appropriate statement.*/
   int deep;            /*!< The level of precedence, it serves to build the precedence constraints */
   FADA_Index* array_access_index;   /*!< A pointer to the access-index*/

//! \name Properties can be built using methods, but required by FADA analysis
   vector<string> stmt1_iteration;               /*!< List of all enclosing parameterized counter-loops */
   vector<vector<Inequation*> > affine_environment;      /*!< All affine environment-constraints (disjunction of conjunctions)*/
   vector<vector<Inequation*> > non_affine_environment;   /*!< Non affine environment-constraints ((disjunction of conjunctions) */
   
   vector<vector<Inequation*> > affine_valid_stmt2;      /*!< The affine constraints from the domain of "stmt2" */
   vector<vector<Inequation*> > non_affine_valid_stmt2;   /*!< Non affine constraints from the domain of "stmt2" */
   
   vector<Inequation*> precedence;            /*!< Lexicographic precedence (between "stmt1" and "stmt2"  ) contraints*/
   
   vector<Inequation*> affine_index_equality;      /*!< Affine inequalities from the equality of access-indexes*/
   vector<Inequation*> non_affine_index_equality;      /*!< Non-affine inequalities from the equality of access-indexes */
   
   vector<string> stmt2_iteration;               /*!< List of enclosing loops (in a good order)*/
   vector<string> global_parameters;            /*!< List of all symbolic constants */

//! \name the result of FADA processing
   Quast*    quast;                  /*!< Quast describing when the dependence can occur */
   vector<string> parameter_of_maximum;            /*!< Name of the parameter of the maximum */
   LDemonstrator*   properties;            /*!< Some properties on the maximum of the parameter*/






//! \name Setters
   //! \brief A setter for "properties"
   void      SetProperties(LDemonstrator* dem)   {properties=dem;}
   //! \brief A setter for "stmt1"
   inline void    SetStmt1(int _1) {stmt1=_1;}
   //! \brief A setter for "stmt2"
   inline void    SetStmt2(int _2) {stmt2=_2;}
   //! \brief A setter for "deep"
   inline void    SetDeep(int _d) {deep=_d;}
   //! \brief A setter for "array_access_index"
   inline void    SetIndex(FADA_Index* __i) {array_access_index=__i;}
   //! \brief A setter for "affine_environment"
   inline void    SetAffineEnvironment(vector<vector<Inequation*> >* __c) {affine_environment=*__c;}
   //! \brief A setter for "non_affine_environment"
   inline void    SetNonAffineEnvironment(vector<vector<Inequation*> >* __c) {non_affine_environment=*__c;}
   //! \brief A setter for "affine_valid_stmt2"
   inline void    SetAffineValidStmt2(vector<vector<Inequation*> >* __c) {affine_valid_stmt2=*__c;}
   //! \brief A setter for "non_affine_stmt2"
   inline void    SetNonAffineValidStmt2(vector<vector<Inequation*> >* __c) {non_affine_valid_stmt2=*__c;}
   //! \brief A setter for "quast"
   inline void    SetQuast(Quast* __q) {quast=__q;}
   //! \brief A setter for "affine_index_equality"
   inline void    SetAffineIndexEquality(vector<Inequation*> * __aie) {affine_index_equality=*__aie;}
   //! \brief A setter for "non_affine_index_equality"
   inline void    SetNonAffineIndexEquality(vector<Inequation*> * __naie) {non_affine_index_equality=*__naie;}
   //! \brief A setter for "precedence"
   inline void    SetPrecedenceConstraints(vector<Inequation*>* __c) {precedence=*__c;}
   //! \brief A setter for "parameter_of_maximum"
   inline void    SetParameterOfTheMaximum(vector<string>* __p) {parameter_of_maximum=*__p;}
   //! \brief A setter for "stmt1_iteration"
   inline void SetStmt1Iteration(vector<string>* __c) {stmt1_iteration=*__c;}
   //! \brief A setter for "stmt2_iteration"
   inline void SetStmt2Iteration(vector<string>* __c) {stmt2_iteration=*__c;}
   //! \brief A setter for "global_parameters"
   inline void SetParameters(vector<string>* __p) {global_parameters=*__p;}


//! \name Advanced Setters
   /*!
   *\brief An advanced setter for the entire object.
   *\param __1 : the identifier of the first statement.
   *\param __2 : the identifier of the second statement.
   *\param __d : the deep of the precedence.
   *\param __i : the second statement access-index for the confictual array.
   * Arguments are mapped to (repectively) : stmt1, stmt2, deep and array_access_index.
   */
   inline void Set(int __1, int __2, int __d, FADA_Index* __i);
   inline void Set(int __1, int __2, int __d);
   
   /*!
   \brief A setter for all properties.
   */   
   void Set(vector<string>* __stmt1, vector<string>* __stmt2, vector<string>* __p,
   vector<vector<Inequation*> >* __ae,vector<vector<Inequation*> >* __nae,
   vector<vector<Inequation*> >* __ac, vector<vector<Inequation*> >* __nac,
   vector<Inequation*> * __preced, vector<Inequation*> * __aie,
   vector<Inequation*> * __anaie, Quast* __dep);
   /*!
   \brief Set all properties to NULL values.
   */
   void Initialize();


public:

//! \name Getters
   //! \brief a getter for "stmt1"
   inline int GetStmt1() {return stmt1;}
   //! \brief a getter for "stmt2"
   inline int GetStmt2() {return stmt2;}
   //! \brief a getter for "deep"
   inline int GetDeep() {return deep;}
   //! \brief a getter for "stmt1_iteration"
   inline vector<string>* GetStmt1Iteration(void) {return &stmt1_iteration;}
   //! \brief a getter for "stmt2_iteration"
   inline vector<string>* GetStmt2Iteration(void) {return &stmt2_iteration;}
   //! \brief a getter for "non_affine_index_equality"
   inline vector<Inequation*>* GetNonAffineIndexesEquality(void) {return &non_affine_index_equality;}
   //! \brief a getter for "affine_index_equality"
   inline vector<Inequation*>* GetAffineIndexesEquality(void) {return &affine_index_equality;}
   //! \brief a getter for "affine_environment"
   inline vector<vector<Inequation*> > * GetAffineEnvironment(void) {return &affine_environment;}
   //! \brief a getter for "non_affine_environment"
   inline vector<vector<Inequation*> > * GetNonAffineEnvironment(void) {return &non_affine_environment;}
   //! \brief a getter for "affine_valid_stmt2"
   inline vector<vector<Inequation*> > * GetAffineValidStmt2(void) {return &affine_valid_stmt2;}
   //! \brief a getter for "non_affine_valid_stmt2"
   inline vector<vector<Inequation*> > * GetNonAffineValidStmt2(void) {return &non_affine_valid_stmt2;}
   //! \brief a getter for "precedence"
   inline vector<Inequation*>* GetPrecedence(void) {return &precedence;}
   //! \brief a getter for "quast"
   inline Quast* GetQuast(void) {return quast;}
   //! \brief a getter for "parameter_of_maximum"
   inline vector<string>* GetParameterOfTheMaximum(void) {return &parameter_of_maximum;}

   LDemonstrator*   GetProperties(void)   {return properties;}
   
   //! \brief a getter for "index"
   inline FADA_Index* GetIndex(void) {return array_access_index;}
   //! \brief a getter for "global_parameters"
   vector<string>* GetParameters(void) {return &global_parameters;}

//!   \name Filling the rest of properties
   /*!
   *\brief This method builds the constraints related to the dependence detween "stmt1" and "stmt2" at the deep "deep".
   *\param __parameterized_iteration : always a fixed (to a parameterized) iteration always for "stmt1".
   *\param __global_parameters :global symbolic constants.
   *\param __references : the entire references. It play the role of a whole program, and it is needed in order to recover domain constraints.
   *\return It fills the caller with the approriate constraints. all we need later is to apply the FADA model on it.
   */


   void BuildConstraints(
               vector<string>*      __global_parameters,
               vector<References*>*    __references,
               vector<string>*      __parameterized_iteration,
               vector<vector<Inequation* > >* __affine_env,
               vector<vector<Inequation* > >* __non_affine_env,
               string __conflictual_variables,
               FADA_Index* __parameterized_Index
               );


   void ComputeTheMaximum(
      LDemonstrator*      __global_properties,
      Options* options
      );

   
   /*!
   *\brief This method build and apply the FADA processing.
   *\param __parameterized_iteration : always a fixed (to a parameterized) iteration always for "stmt1".
   *\param __global_parameters :global symbolic constants.
   *\param __pgm : the entire AST of the program. It is needed to solve the textual order between statements.
   *\return It fills the "quast" property. May be "properties" and "parameter_of_maximum" are updated if some non-affine constraints occur.
   *\see BuildConstraints()
   */
//    void BuildAndComputeMax(vector<string>* __parameterized_iteration,vector<string>* __global_parameters, Program* __pgm,bool alphas,bool sa);


//! \name Frequently Asked Questions
   /*!
   \brief Checks whether it is some non affine constraints.
   */
   
   bool IsThereNonAffineConstraints();
   
   /*!
   \brief Checks whether two instances are semantically equal.
   */
   inline bool Equal(ElementaryDependence* ed)
   {return GetStmt1()==ed->GetStmt1() &&GetStmt2()==ed->GetStmt2() &&GetDeep()==ed->GetDeep();}
   
public:
//! \name Building
   /*!
   \param __1[in] : first dependent statement id.
   \param __2[in] : second dependent statement id.
   \param __d[in] : the deep of the dependence precedence.
   \param __i[in] : the index for the first statement access.
   \remarks These are the basic information, required properties by FADA processing can be built using "BuildConstraints". The FADA processing is done by "BuildAndComputeMax".
   \see Set(),BuildAndComputeMax(), BuildConstraints()
   */
   ElementaryDependence(int __1, int __2, int __d,FADA_Index* __i);
   ElementaryDependence(int __1, int __2, int __d);

   ~ElementaryDependence();

//! \name Printing
   string  Print_str(int level=0);
   inline  void Print(int level=0)    {cout<<Print_str(level);}
   string   Full_Tex_Print_str();
   string  PrintProperties();
   void   CollectMaximumOfParameterValues(map<string,string>* map,vector<string> loops);

////!\name Miscellaneous
   
};
};
#endif


