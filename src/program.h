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
\file adaan_program.h
\author  Christophe Alias
\author Marouane Belaoucha
\brief This file contain the description of the class Program.
*/


#ifndef ADAAN_PROGRAM_H
#define ADAAN_PROGRAM_H

#include "assignment.h"
#include "statement.h"
#include "controlstructure.h"
#include "references.h"
#include "domaincondition.h"
#include <elementarydependence.h>


namespace fada{
class Assignment;
class Statement;
class FADA_Index;
class Quast;
class References;   //cyclic includes

/*!
\class Program
\brief reprensents a piece of code, a function or a main program.
*/
class Program {
private:
string label;               //!< the name of the program
vector<string> global_parameters;      //!< all read only scalars
Statement* syntax_tree;            //!< its abstract syntax tree

vector<References*> normalized_stmts;      //!< vector of governed references

vector<string>       all_known_parameters;      //!< all symbolic constants
vector<vector<bool> >   sequential_precedence;
Options         options;
LDemonstrator*      global_properties;



//! \name Preprocessing
   //! \brief loops must be identified by their loop counters, renaming can occur
   vector<string>    SingleCounterId(void);
//   //! \brief build governed references from an AST.
   void NormalizeDomains(void);
   void   BuildDomains(void);
   void   ComputeSequentialPrecedence();
   string ToHtml(Options* op);

   string ToHtml(string dir, string file_name);

void   CollectPropertiesOnIrregularLoops(LDemonstrator* __properties);

public:

LDemonstrator*      GetGlobalProperties()   {return global_properties;}
//! \name Getters
   vector<vector<bool> >*   GetSequentialPrecedenceMatrix()   {return & sequential_precedence;}
   //! \brief A getter for "global_parameters"
   vector<string>* GetGlobalParameters(void) {return &global_parameters;}
   //! \brief A getter for "normalized_stmts"
   vector<References*>* GetNormalizedStmts(void) {return &normalized_stmts;}
   //! \brief A setter for "syntax_tree"
   void SetSyntaxTree(Statement* __main) {this->syntax_tree=__main;}
   //! \brief A getter for "syntax_tree"
   Statement* GetSyntaxTree(void) {return this->syntax_tree;}
   //! \brief A getter for "all_known_parameters"
   vector<string>* GetAllParameters(void) {return &all_known_parameters;}
   //! \brief A getter for "label"
   string GetLabel(void) {return label;}
   inline Options* GetOptions(void)   {return &options;}

//! \name Setters
   inline void SetOptions(Options* op)      {options=*op;}
   //! \brief A setter for "all_known_parameters"
   void SetAllParameters(vector<string>* vect) {all_known_parameters=*vect;}
   //! \brief A getter for "label"
   void SetLabel(string str) {label=str;}
    //! \brief A setter for "global_parameters"
   void SetGlobalParameters( vector<string>* __gp) {global_parameters=*__gp;}
//! \name building
   //! \brief Parsing from a filre
   bool Parse(string);
   //! \brief look for read only scalars from the AST
   void LookForGlobalParameters(void);
   //! \brief look for read only scalars from the AST
   void LookForParameters(bool extended);
   //! \brief Remove loop counters from the read and write references.
   void AvoidCounters(vector<string>*);
   //! \brief Remove loop counters and symbolic constants from the read and write references.
   void AvoidCountersAndSymbolicConstants(void);
   //! \brief Build an empty program.
    Program();
   void RemoveTwoAssignments(void);

 

//! \name Dumping
   //! \brief out write the program in a C format
   string Generate_C_str(void);
   //! \brief out write the program in a C format
   void Generate_c(string filename);
   //! \brief printing the program
   string Print_str();
   //! \brief printing the program in the standard output file
   void Print(void);

   //! \brief Dump all properties
   string Full_Print_str();

   //! \brief Print the governed references
   void PrintNormalized(int);

   void PrintSources(string);

   //! \brief generate the index for all references.
   string GenerateHTMLIndex(void);
   //! \brief generate html pages with the analysis' results


   //! \brief generate html pages with the analysis' results
   string ToHtml(void)      {return ToHtml(GetOptions());}

   /*! 
   \brief generate a dependence graph in GraphViz Format
   \param file_name[in] : the file name
   \param print_rank[in] :print the identifier of statements ?
   \param print_label[in] : print the statement labels ?
   \param print_assign[in] : print the original statement code-line ?
   \param print_condition[in] : print the dependence validity condition ?
   \param relation[in]   : print the entire dependence relation ?
   \param rename[in] : add the prefix "current_" to all loop counters ?
   \param bottom[in] : add the node "bottom" for not initialized variables ?
   \param alphas[in] : hide alphas ?
   return nothing
   */
   string Generate_GraphViz_Graph(string file_name, bool print_rank, bool print_label, bool print_assign, bool print_condition, bool print_relation, bool rename,bool bottom,bool alphas);
   /*! 
   \brief generate a dependence graph in GraphViz Format
   \param print_rank[in] :print the identifier of statements ?
   \param print_label[in] : print the statement labels ?
   \param print_assign[in] : print the original statement code-line ?
   \param print_condition[in] : print the dependence validity condition ?
   \param relation[in]   : print the entire dependence relation ?
   \param rename[in] : add the prefix "current_" to all loop counters ?
   \param bottom[in] : add the node "bottom" for not initialized variables ?
   \param alphas[in] : hide alphas ?
   \return a graph in GraphViz format.
   */

   string Generate_GraphViz_Graph(bool print_rank, bool print_label, bool print_assign, bool print_condition, bool print_relation,bool rename,bool bottom,bool alphas);

 


//! \name Postprocessing
   /*!
   \brief Generate a C program which check wether computed dependence graph is valid.
   \return the C program
   */
   string GenerateOracle(void);
   /*!
   \brief Generate a C program which check wether computed dependence graph is valid.
   \param file_name : the name of the file to write in.
   */
   void GenerateOracle(string file_name);
   /*!
   \brief Generate a C program which check wether computed dependence graph is valid.
   \return the C program
   */
   string Generate_C_Test_Code(void);
   /*!
   \brief Generate a C program which check wether computed dependence graph is valid.
   \param file_name : the name of the file to write in.
   */
   void Generate_C_Test_Code(string file_name);

   /*!
   \brief rename parameterized loop counters to their original names.
   */
   void RenameNewParameters(void);
   ~Program();

//! \name Preprocessing
   //! \brief Map loops to their loop counters
   void MapLoopsWithTheirCounters(Statement*);
   /*! \brief Compute a "plan" the evaluate smartly the definitions of variables read by a statement.
   \param stmt[in] : the read statement.
   \param array[in] : conflictual array.
   \param __i[in] : access function
   */
   vector<ElementaryDependence*> GenerateOptimalWayForFlowDependenciesComputing(int stmt, string array,FADA_Index*__i);
   /*!
   \brief Compute a "plan" the evaluate smartly the definitions of variables read by all statements.
   \see GenerateOptimalWayForFlowDependenciesComputing()
   */
   void GenerateOptimalWayForFlowDependenciesComputing(void);
   /*!
   \brief collect the referenced scalars.
   */
   void GetAllReferencedVariables(vector<int>* __size, vector<string>* __id);
   /*!
   \brief Lower an AST into a vector geverned references.
   */
   void Normalize(void);
   /*!
   \brief Lower a statement into a vector geverned references.
   \param __stmt[in] : the concerned statement.
   \param __domain[in] : Its iteration domain.
   \param __controls[in] : enclosing controls.
   \param __counters[in] : enclosing loops.
   */
   void Normalize(
   Statement* __stmt,
   Condition* __domain,
   map<int,DomainConstraint*> __controls,
   vector<string> __counters
   );
//   //! \brief preprocessing for FADA analysis
//   void Prepare(void);
   //! \brief preprocessing for FADA analysis
   void Preprocess(void);

   void   PreprocessNormalized();
//! \name Processing
   //! \brief Evaluate the sequential precedence between two statements.
   bool IsSequentiallyBefore(int id1, int id2, vector<string>* common);
   //! \brief THE FADA PROCESSING
//    void ComputeSourcesForAllReadVariables(bool sa,bool alphas);
   void ComputeSourcesForAllReadVariables();
   void ComputeSourcesOnNormalizedReferences();
//   //! \brief Apply the ietarative analysis.
//   void    IterativeAnalysis(void);

   void      CollectLoops(set<string>* all_loops);
   vector<string>   LookForParallelLoops();
};


 };
#endif
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////  END   /////////////////////////////////
//////////////////////////////////////////////////////////////////////////


