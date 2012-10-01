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

#ifndef ADAAN_READ_REFERENCE_H
#define ADAAN_READ_REFERENCE_H

#include <index.h>
#include <ldemonstrator.h>
#include <options.h>


/*!
* @file adaan_read_reference.h
* @author Marouane Belaoucha
* This file contains the definition of class "Read_Reference". It models a read variable or read array cell.
*/


namespace fada{
         // cyclic includes
class FADA_Index;   
class Quast;
class ElementaryDependence;
class References;
class Program;
class LDemonstrator;


/*!
* \class Read_Reference
* \brief It models a read memory cells, for which we want to compute the source.
* It is about scalars and array cells accessible in read only by a statement. It is defined by the name of the scalar(for an array, we need the name and the cell-index). after the FADA processing, the definition is computed.
* In fact, scalars are considered as arrays. A scalar is an array with zero dimentionals. So for the processing, both the name and the access index are required.
*/

class Read_Reference{
private:

//! \name Absolutely required.
   string array_name;             /*!< The name of the array */
   FADA_Index* index;            /*!< The access index */
   vector<ElementaryDependence*> schedule;      /*!< The schedule to be followed in order to compute the definition*/
   vector<string>  global_parameters;      /*!< A pointer for global parameters */
   vector<string>* all_parameters;         /*!< A pointer for all parameters (= global parameters + parameterized read iteration) */

//! \name Can be filled automatically
   Quast* definition;         /*!< Its source (to be computed by the FADA processing) */
   set<string>    parameter_of_maximum;      /*!< The name of parameter of the maximum (computed by the FADA processing for irregular accesses */
   LDemonstrator*   properties_on_parameter;   /*!< Some properties on the parameter of the maximum */

//! \name Setters

   //! \brief A setter for " properties_on_parameters"
   void      SetProperties(LDemonstrator* ppt)   {properties_on_parameter=ppt;}
   //! \brief A setter for "array_name"
   inline void SetArrayName(string __str) {array_name=__str;}
   //! \brief A setter for "index"
   inline void SetArrayIndex(FADA_Index* __ai) {index=__ai;}
   //! \brief A setter for "global_parameters"
   inline void SetGlobalParameters(vector<string>* __gp) {global_parameters=*__gp;}
   //! \brief A setter for "all_parameters"
   inline void SetAllParameters(vector<string>* __ap) {all_parameters=__ap;}
   //! \brief A setter for "parameter_of_maximum"
   inline void SetMaximumsOfParameters(set<string>* __mop) {parameter_of_maximum=*__mop;}

//! \name Advanced setters
   /*! 
   \brief It builds a read varaible.
   \param __name[in] : its name.
   \see Set(), ADaAn_Read_reference()
   */
   void Set(string __name);
   /*! 
   \brief It builds a read array-cell.
   \param __name[in] : its name.
   \param __index[in] : access index.
   \see Set(), ADaAn_Read_reference()
   */
   void Set(string __name , FADA_Index* __index);
   /*! 
   \brief It builds a read array-cell with its source
   \param __name[in] : its name.
   \param __index[in] : its access index.
   \param __quast[in] : its source.
   \see Set(), ADaAn_Read_reference()
   */
   void Set(string __name, FADA_Index* __index, Quast* __quast);

   void   Set(Expression*);
//    void Set(Quast
   /*!
   \brief Set all properties to their NULL values.
   */
   void Initialize();

   /*!
   \brief Unused constructor.
   */
   Read_Reference();

public:

   Read_Reference(Expression*);
//! \name Building
   /*! 
   \brief It builds a read varaible.
   \param __name[in] : its name.
   \see Set(), ADaAn_Read_reference()
   */
   Read_Reference(string __name);
   /*! 
   \brief It builds a read array-cell.
   \param __name[in] : its name.
   \param __index[in] : access index.
   \see Set(), Read_reference()
   */
    Read_Reference(string __name, FADA_Index* __index);
   /*! 
   \brief It builds a read array-cell with its source
   \param __name[in] : its name.
   \param __index[in] : its access index.
   \param __quast[in] : its source.
   \see Set(), Read_reference()
   */

    Read_Reference(string __name, FADA_Index* __index,Quast* __quast);

 ~Read_Reference();

//! \name Getters
   //! \brief A getter for "array_name"
   inline string GetArrayName(void) {return array_name;}
   //! \brief A getter for "index"
   inline FADA_Index* GetArrayIndex(void) {return index;}
   //! \brief A getter for "definition"
   inline Quast* GetDefinition(void) {return definition;}
   //! \brief A getter for "global_parameters"
   inline vector<string>* GetGlobalParameters(void) {return &global_parameters;}
   //! \brief A getter for "all_parameters"
   inline vector<string>* GetAllParameters(void) {return all_parameters;}
   //! \brief A getter for "parameter_of_maximum"
   inline set<string>* GetParameterOfMaximum(void) {return &parameter_of_maximum;}
   //! \brief A getter for "schedule"
   vector<ElementaryDependence*>* GetSchedule(void) {return &schedule;}
   //! \brief A getter for "properties_on_parameter"
   LDemonstrator*   GetProperties(void)      {return properties_on_parameter;}


//! \name Printing
   string      Full_Tex_Print_str(void);
   /*!
   \brief It prints properties in an incrementing way following values of "__level"
   \return the string containing the result of printing.
   \see Print()
   */
   string Print_str(int __level);
   string Print_str(int __level,Options*);
   /*!
   \brief It prints properties in an incrementing way following values of "__level"
   \return the string containing the result of printing.
   \see Print_str()
   */
   void Print(int __level);
   /*!
   \brief Print only the name of the array followed by its access function.
   */
   string PrettyPrint_str(void);
   /*!
   \brief It generates a C code, which will serve to check wether computed source is correct.
   \param __prefix[in] : an indentation to be added to all lines.
   \return a string containing the result of printing.
   */
   string Generate_C_Test_Code(string __prefix);



   string ToHtml(string __dir, string __name_prefix,Options* op, bool __useless);
   void ToHtml(string __dir, string __name_prefix,Options* op);



//! \name Setters
   //! \brief A setter for "schedule"
   void SetSchedule(vector<ElementaryDependence*>* __sch) {schedule=*__sch;}
   //! \brief A setter for "definition"
inline    void SetDefinition(Quast* __def) {definition=__def;}



//!\name Miscellaneous
   //! \brief Check wether the current and "__rr" represent the same memory cell.
   bool operator==(Read_Reference* __rr);
   //! \brief Check wether the current references the variable or the array "__name"
   bool operator==(string __name);
   /*!
   \brief Collect all scalars.
   \param __read[inout] : the set scalars will be added to.
   */
   void ReadScalars(set<string>* __read);

   void ComputeTheDefinition(
               // global
            vector<string>*       __param,
            vector<vector<bool> >*       __textual_prec,
            LDemonstrator*         __global_properties,
            vector<References*>*      __references,
               // parameterized read
            vector<string>*       __parameterized_read_iteration,
            map<string,Expression*>*   __map_to_parameterized_iteration,
            vector<vector<Inequation*> >*    __env,
            vector<vector<Inequation*> >*    __non_affine_env,
               // options
            Options* options
            );

   void ComputeTheDefinition(
               // global
            vector<string>*       __param,
            void      *       __textual_prec,
            int   (*fct)(void*,int,int),
            LDemonstrator*         __global_properties,
            vector<References*>*      __references,
               // parameterized read
            vector<string>*       __parameterized_read_iteration,
            map<string,Expression*>*   __map_to_parameterized_iteration,
            vector<vector<Inequation*> >*    __env,
            vector<vector<Inequation*> >*    __non_affine_env,
               // options
            Options* options
            );

   void   TagIndex(int __stmt, vector<Expression*>* __iter);
   void   LookForParallelism(vector<string> original, vector<string>* parallel,vector<References*> program);
   void   CollectParameterOfTheMaximumValues(vector<string>loops);


};
};
#endif


