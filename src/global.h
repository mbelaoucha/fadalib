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

#ifndef _GLOBAL_H__
#define _GLOBAL_H__

#include <vector>
#include <constants.h>
#include <program.h>
#include <lexicographicinequation.h>
#include <lc_rhs_term.h>
#include <getopt.h>
#include <agc.h>

using namespace std;

/*!
\file global.h
\brief Contain a description of global functions
*/



namespace fada{


   extern AGC       ____myGC;
   extern set<string>   all_types;

//! \name Handling global options

   /*!
   \brief look for an option argument.
   \param s[in] : the option.
   \param argc[in] : the number of atomic words in the command-line used to run ADaAn.
   \param argv[in] : the vector of all words used to invocate ADaAn.
   \return the argument of the option "s"
   */
   void   ParseOptions(int argc, char* argv[],Options* options);


//! \name Common printings
   //! \brief Print a vector of strings
   string   PrintVector(vector<string>* v);
   void            PrintADaAn(void);
   void            PrintVersion(void);
   void            PrintHelp(void);
   //! \brief Print an operation(an instance of a statement)
   string PrintOperation(int  stmt_id ,vector<Expression*>* __iteration);
   string PrintOperation(int  stmt_id ,vector<Expression*>* __iteration,GraphPrinterOptions op);
   string PrintTexOperation(int  stmt_id ,vector<Expression*>* __iteration);



//! \name Common questions
   //! \brief Is "__str" is "__vector" ?
   bool            IsIn(string __str, vector<string>* __vector);
   //!\brief Is all "__scalars" is "__invariables" ?
   bool            InvariableIn(vector<string>* __scalars, vector<string>* __invariable);


//! \name Useful for lexicographic tasks
   //!  \brief compute the common loop counters.
   vector<string>         Common(vector<string>* v1,vector<string>* v2);
   //!  \brief provide a parameterized loop vector.
   vector<string>         ParametrizeReadIteration(vector<string>* loop_counters);
   //! \brief  compute the lexicographic precedence between two statements at a given level
   vector<Inequation*>   Precedence(vector<string>* __write, vector<string>* __read,int __common_loops,int __common);
   //! \brief  compute the lexicographic precedence between two statements.
   vector<Condition*>    lex_preced(int rank1, vector<string>* counters1, int rank2, vector<string>* counters2, vector<string>* param_counters2);


//! \name Exporting and importing from/to PIPlib structures.
   //! \brief Import a quast from a pip datastructure
   Quast*            FromPIPQuast(int __index, vector<string>* variables, PipQuast* pipquast);
   //! \brief Import a quast from a pip datastructure
   Quast*             FromPipQuast(int __index, int __deep,vector <string>* __counters, vector<string>* __params,PipQuast* __quast, map <int, Expression*> __new_param);
   //! \brief Import a pip expression
   Expression*         PipVectorToExpression(PipVector* __pipv, vector<string>* __param, map<int, Expression*> __new_param);
   //! \brief Rebuild an expression from a vector of coefficients
   Expression*         VectorToExpression(vector<int>* __coefs, vector<string>* __var, vector<string>* __param);
   //! \brief Import an index from a PIPlib Data
   FADA_Index*         PipListToIndex(PipList* __pipl,int __nb_variables, vector<string>* __param, map<int, Expression*> __new_param);
   //! \brief Generate a trivial true inequation
   vector<int>          GenerateTruePipVector(int);
   //! \brief Generate a trivial false inequation
   vector<int>         GenerateFalsePipVector(int);

   

//! \name Collecting
   //!\brief Collect references from an expression
   void            CollectReferences(Expression* __expr, vector<Read_Reference*>* _ref);
   //!\brief Collect references from an index
   void            CollectReferences(FADA_Index* __i, vector<Read_Reference*>* _ref);
   //!\brief Collect references from a condition
   void            CollectReferences(Condition* __i, vector<Read_Reference*>* _ref);
   //!\brief Collect references from an inequation
   void            CollectReferences(Inequation* __i, vector<Read_Reference*>* _ref);



//! \name FADA preprocessing
   /*!
   \brief Have to rename a loop counter, because its identifier is already used by another loop.
   \param counter[in] : its original name.
   \param to_be_avoided[in] : List of names to be avoided.
   \return a candidate new name for the loop counter.
   */
   string            NewCounterName(string counter, vector<string>* to_be_avoided);
   
   /*!
   \brief Compute the optimal way to evaluate a flow dependence between two statements.
   \param __begin[in] : start looking for write operations from the satement identified by "__begin".
   \param __end[in] : acheive running when you reach this statement.
   \param read_stmt[in] : the read statement identifier.
   \param read_array[in] : the conflictual array.
   \param __i[in] : the read access function.
   \param common[in] : the number of common loops.
   \param __references : all "read_stmt" read-references.
   */
   vector<ElementaryDependence*>   ComputeDependenciesConfigs(int __begin,int __end, int read_stmt,string read_array,FADA_Index*__i,int common, vector<References*>* __references);

   vector<ElementaryDependence*>   EliminateValues2In(vector<ElementaryDependence*>*, map<int,int>*);
   /*!
   \brief Avoid the computation of the same thing twice.
   */
   vector<ElementaryDependence*>   EliminateDoubles(vector<ElementaryDependence*>* configs);


//! \name Handling constraints
   //! \brief Build a condition from a vector (conjunction) of affine inequations.
   Condition*         ToDNFTerm(vector<Inequation*>* __affine_constraints);
   Condition*         DNFTerms_to_Condition(vector<vector<Inequation*> >* inequations);
   //! \brief Build the AND of the two disjunction of conjunctions conditions.
   vector<vector<Inequation*> >    AND_DNF(vector<vector<Inequation*> > *, vector<vector<Inequation*> > *);
   //! \brief Rename scalars by anothers.
   vector<vector<Inequation*> >   RENAME(vector<vector<Inequation*> >*, map<string,Expression*>*);
   //! \brief Checks wether one of "inequations" can imply "ineq"
   bool            IMPLY(vector<Inequation*>* inequations, Inequation* ineq,vector<string>* param);

   bool            IMPLY(vector<vector<Inequation*> >* inequations, Inequation* ineq,vector<string>* param);



   vector<vector<Inequation*> >      Merge(vector<vector<Inequation*> >* __rela1,vector<vector<Inequation*> >* __rela2);
   //! \brief Performs the AND of two DNF terms
   vector<Inequation*>   MergeInequations(vector<Inequation*>* __ineq1, vector<Inequation*> * ineq2);
   //! \brief Negates a DNF term.
   vector<Inequation*>   NegateInequations(vector<Inequation*>* __ineqs);

   vector<LC_RHS_Term*>   Merge(vector<LC_RHS_Term*>* __rela1,vector<LC_RHS_Term*>* __rela2);

   //! \brief Avoid redendent inequations
   vector<Inequation*>   EliminateDoubles(vector<Inequation*>*);
   //! \brief Avoid useless inequations
   vector<Inequation*>   EliminateTrueAndDoubles(vector<Inequation*>*);

   //! \brief Printing inequations   
   string   PrintTexInequations(vector<Inequation*>* vecteur);

string            PrintInequations(vector<Inequation*>* vecteur);
   //! \brief Printing inequations
   string            PrintInequations(string prefix, vector<Inequation*>* vecteur);
   //! \brief Printing inequations
   string            PrintInequations(vector<vector<Inequation*> >* vecteur);





//! \name HTML and GraphViz
   void   Connect(Statement* __stmt, References* __ref);
   void     WriteToFile(string file_name, string contnent);
   

//!\name Miscellaneous
   //! \brief Merges two vectors
   vector<LexicographicInequation*> Merge(vector<LexicographicInequation*>* __rela1,vector<LexicographicInequation*>* __rela);
   //! \brief Merges two vectors
   vector<string>         MergeVariables(vector<string>* var1,vector<string>* var2);
   /*! 
   \brief Computes the maximum lexicographic of two quasts in a smart way.
   \param _q1[in] : the first quast.
   \param _q2[in] : the second quast.
   \param _pgm[in] : a reference to the AST of the program.
   \param _conext[in] : the context of this computation (assumed as a tautology).
   \param _param[in] : symbolic constants.
   */
   Quast*         Max(Quast* _q1, Quast* _q2, Program* _pgm, vector<Inequation*> _context, vector<string>* _param);
   Quast*         Max(Quast* _q1, Quast* _q2, vector<References*>* __ref, vector<vector<bool> >* _textual_preced, vector<vector<Inequation*> > _context, vector<string>* _param);
   
   Quast*         Max(Quast* _q1, Quast* _q2, vector<References*>* __ref, void* _textual_preced, int (*fct)(void*,int,int), vector<vector<Inequation*> > _context, vector<string>* _param);
   /*!
   \brief Computes a more precise lexicographic precendence (between two operations) constraints.
   \param _pgm[in] : a reference for the program
   \param s1 : first statement
   \param s2[in] : second statement
   \param index1[in] : the instance of the first statement
   \param index2[in] : the instance of the second statement
   \param common_counters[in] : the common loops.
   \param s1_before_s2[out] : is "S1" textually specified before "S2" ?
   \param s2_before_s1[out] :is "S2" textually specified before "S1" ?
   \return the strict precedence constraints
   */
//    Condition*      SmartPrecedenceComputing(Program* _pgm, 
//                      int s1, int s2, 
//                      FADA_Index* index1, FADA_Index* index2,
//                      vector<string>* common_counters, bool*s1_before_s2, bool*s2_before_s1);

   Condition*      SmartPrecedenceComputing(vector<vector<bool> >* textual_preced, 
                     int s1, int s2, 
                     FADA_Index* index1, FADA_Index* index2,
                     vector<string>* common_counters, bool*s1_before_s2, bool*s2_before_s1);

   Condition*      SmartPrecedenceComputing(void* textual_preced, 
                     int (*fct)(void*,int,int),
                     int s1, int s2, 
                     FADA_Index* index1, FADA_Index* index2,
                     vector<string>* common_counters, bool*s1_before_s2, bool*s2_before_s1);


   
   //! \brief Traduces inequations to a context quast.
   ContextQuast*         TraduceToContextQuast(Quast*,vector<string>* var, vector<vector<Inequation*> >* inequations);

   vector<vector<Inequation*> >   SimplifyInequations(vector<vector<Inequation*> >* __to_be_simplified, vector<Inequation*>* __env, vector<string>* __param);

   vector<vector<Inequation*> >   SimplifyInequations(vector<vector<Inequation*> >* __to_be_simplified, vector<vector<Inequation*> >* __env, vector<string>* __param);

   vector<vector<Inequation*> >   TraduceNEQOperations(vector<vector<Inequation*> >* __const);
   vector<vector<Inequation*> >   TraduceNEQOperations(vector<Inequation*>* __const);
   void            CollectNEQInequations(vector<Inequation*>* __all,vector<Inequation*>* __neq,vector<Inequation*>* __others);
   
   vector<vector<Inequation*> >   DistributeNEQuation(Inequation* ineq1,Inequation* ineq2,vector<vector<Inequation*> >* result);
   vector<vector<Inequation*> >   EliminateDoubles(vector<vector<Inequation*> >*);
   vector<string>         RemoveDoubles(vector<string>* _v);
   vector<vector<Inequation*> >   RemoveAlphas(vector<vector<Inequation*> >* ineq,vector<string>* alphas);
   vector<Inequation*>      RemoveAlphas(vector<Inequation*>* ineq,vector<string>* alphas);
   vector<vector<Inequation*> >   RemoveFlase(vector<vector<Inequation*> >* __v, vector<string>* __param);

   vector<Inequation*>      Substitute(vector<Inequation*>* ineq,map<string, Expression*>* __map);
   vector<vector<Inequation*> >   Substitute(vector<vector<Inequation*> >* __v, map<string, Expression*>* __map);

bool   IsUnsatisfiable(vector<Inequation*>* constraints, vector<string>* parameters);
bool   IsUnsatisfiable(vector<vector<Inequation*> >* constraints, vector<string>* parameters);
bool   IsType(string identifier);
bool   AddType(string identifier);
void    PrintAllTypes();
void   InitTypes();

vector<Inequation*>   RecoverAndSubstituteAlphas(vector<string>* alphas, vector<Inequation*>* constraints);
vector<vector<Inequation*> >    RecoverAndSubstituteAlphas(vector<string>* alphas, vector<vector<Inequation*> > * constraints);
};

// int   IsDefinedType(char* ch);
#endif



