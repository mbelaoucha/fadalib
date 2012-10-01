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
\file solver.h
\brief This file contains the description of the class  Solver
*/

#ifndef ADAAN_SOLVER_H
#define ADAAN_SOLVER_H
#include "expression.h"
#include "quast.h"
#include <piplib/piplib64.h>


namespace fada{
/*!
\class Solver
\brief It represents a parametric linear programming solvers.
There is only two known parametric solvers PIP (Parametric Integer Programming), and Omega. The current implementation integrate only PIP.
\remarks Constraints injected to this class are supposed to be affine. Otherelse, the execution will be aborted.
\remarks This class do not handle the union of polyhedra (disjunction of conjunctions of affine constraints).
\remarks Solving constraints (looking for the lexicographic max/min) is constly task, you should not call it for trivial systems.
*/
class Solver{
private:
vector<string> variables;          //! The variables of the system
vector<string> parameters;         //! symbolic constants
vector<Inequation*> constraints;      //! affine constraints ont variables and parameters

int index;               //! an integer, the quast to be tagged with
int deep;               //! an integer, the quast to be tagged with
bool is_pipized;
//!\name simple setters
   //! \brief a setter for "variables"
   void SetVariables(vector<string> *var) {variables=*var;}
   //! \brief a setter for "variables"
   void SetVariables(vector<string> var) {variables=var;}
   //! \brief a setter for "parameters"
   void SetParameters(vector<string> *param) {parameters=*param;}
   //! \brief a setter for "parameters"
   void SetParameters(vector<string> param) {parameters=param;}
   //! \brief a setter for "constraints"
   void SetConstraints(vector<Inequation*> *cons) {constraints=*cons;}
   //! \brief a setter for "index"
   void SetIndex(int val) {index=val;}
   //! \brief a setter for "deep"
   void SetDeep(int val) {deep=val;}

//!\name simple getters
   //! \brief a getter for "variables"
   vector<string>* GetVariables(void) {return &variables;}
   //! \brief a getter for "parameters"
   vector<string>* GetParameters(void) {return &parameters;}
   //! \brief a getter for "index"
   int GetIndex(void) {return index;}
   //! \brief a getter for "deep"
   int GetDeep(void) {return deep;}
   //! \brief a getter for "constraints"
   vector<Inequation*>* GetConstraints(void) {return &constraints;}

//! \name Advanced setters
   /*!
   \brief Set properties mapping the arguments.
   \param __stmt[in] : the identifier of a statement.
   \param __deep[in] : its deep.
   \param __var[in] : the variables.
   \param __param[in] : list of symbolic constants.
   \param __constraints[in] : affine constraints
   */
   void Set(int __stmt, int __deep, vector<string> __var, vector<string> __param, vector<Inequation*>* __constraints,bool pipized=false);
   /*!
   \brief Set all properties to NULL values.
   */
   void Initialize(void);


//! \name Preprocessing
   //!   \brief It codes the affine inequations into a matrix of coefficients.
   vector<vector<int> > CodeConstraints(void);
   //!   \brief It codes the affine inequations into a PIP structure.
   PipMatrix*    ToPIPMatrix(void);


//! \name processing

//! \brief It computes, using PIPLib, the lexicographic maximum of the set defined by the constraints the instance is initialized with.
Quast* PIPMax(void);
//! \brief It computes the lexicographic minimum using PIPlib.
Quast* PIPMin(void);

void   PIPize(vector<string>* new_var, vector<Inequation*>* new_ineq);
inline   void   SetPIPized(bool val){is_pipized=val;}
inline   bool   IsPIPized(void){return is_pipized;}
Solver(int  __stmt,int __deep, vector<string> __var, vector<string> __param, vector<Inequation*>* __const,bool pipized);
void   RemoveNegativeVariablesPIPLimitation(vector<string>*variables,vector<string>*param,vector<Inequation*>* constraints);
public:
  ~Solver();

//! \name Building
   /*!
   \brief It creates a solver with : 0 variables, 0 parameters and 0 constraints.
   \see Quast* Max(int,int, vector<string>, vector<string>, vector<Inequation*>*), Quast* Min(int,int, vector<string>, vector<string>, vector<Inequation*>*);
   This configuration is an environment (constraints are always uphled). If you decide to use this constructor, you shoud use "Max" or "Min" with parameters to solve 'real' problems.
   */
   Solver();
   
   /*!
   \brief It creates and initialize solver.
   \param __stmt : to tag with the solution.
   \param __deep : ti tag with the solution.
   \param __var : the variables of the system.
   \param __param : the symbolic constants of the system.
   \param __const : constraints describing the parametric polyhedron.
   \see Max(),MIN()
   After the call, we can say that the solver is initialized. You can use "Max()" and "Min()" to look for lexicographic maximum and minimum points.
   */
   Solver(int  __stmt,int __deep, vector<string> __var, vector<string> __param, vector<Inequation*>* __const);


//! \name Processing on build object

   /*!
   \brief Look for the lexicographic maximum of the polyhedron.
   \remarks the caller have to be correctly initialized.
   \see Min(), Solver()
   */
   Quast* Max(void);
   /*!
   \brief Look for the lexicographic minimum of the polyhedron.
   \remarks the caller have to be correctly initialized.
   \see Max(), Solver()
   */
   Quast* Min(void);

//! \name Build and process
   /*!
   \brief Initialize and compute the lexicographic maximum.
   \param __stmt : to tag with the solution.
   \param __deep : to tag with the solution.
   \param __var : the variables of the system.
   \param __param : the symbolic constants of the system.
   \param __const : constraints describing the parametric polyhedron.
   \see Max(),MIN(), Solver()
   */
   Quast* Max(int __stmt,int __deep, vector<string> __var , vector<string> __param, vector<Inequation*>* __const);
   
   /*!
   \brief Initialize and compute the lexicographic minimum.
   \param __var : the variables of the system.
   \param __param : the symbolic constants of the system.
   \param __const : constraints describing the parametric polyhedron.
   \see Max(),MIN(), Solver()
   */
   Quast* Min(vector<string>* __var, vector<string>* __par, vector<Inequation*>* __const);
   

//! \name Printing
   //! \brief print all properties into string
   string Print_str(void);
   //! \brief print all properties into the standard output file.
   void Print();
};
};
#endif


