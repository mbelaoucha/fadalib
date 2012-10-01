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
\file adaan_written_reference.h
\author Marouane Belaoucha
\brief This file contains the definition of the class Written_Reference
*/


#ifndef ADAAN_WRITTEN_REFERENCE_H
#define ADAAN_WRITTEN_REFERENCE_H



#include "quast.h"

namespace fada{

class Quast;
class FADA_Index;


/*!
* \class Written_Reference
* \brief It defines write accesses on variables and array cells.
* This class is used de model write accesses, for those, we want to compute anti and output dependences.
* Within this class scalars are generalized into a zero-dimensional array. So, the property "index"  is always required.
*/
class Written_Reference
{

private:
//! Required data
string name;            /*!< The name of the array*/
FADA_Index* index;         /*!< The access-index*/
//! Results of FADA processing
Quast* anti_dep;         /*!< The anti-dependent read statement */
Quast* output_dep;      /*!< The output-dependent write statement */

//! \name Setters
   //! \brief A setter for "anti_dep"
   inline void SetAnti(Quast* __anti) {anti_dep=__anti;}
   //! \brief A setter for "output_dep"
   inline void SetOutput(Quast*__out) {output_dep=__out;}
   //! \brief A setter for "name"
   inline void SetName(string __name) {name=__name;}
   //! \brief A setter for "index"
   inline void SetIndex(FADA_Index* __index) {index=__index;}


//! \name Advanced Setters
   /*!
   *\brief An initialization method. The entire properties are initialized into the NULL-values.
   *\return nothing
   * This method is called within every constructor, that lead us to be sure of the correct initialization of the object. In fact, this method is a simple call to Set("",NULL,NULL,NULL);
   * \see Set()
   */
   void Initialize();
   
   /*!
   *\brief An advanced setter for the whole object.
   *\param __name : the name of the array.
   *\param __index : the access index.
   *\return nothing
   * After calling this setter, the "index" and the "name" properties will be set to "__index" and "__name". The other properties will be set to NULL. This method creates a fully valid object.
   */
   void Set(string __name, FADA_Index* __index);
   
   /*!
   *\brief An advanced setter for the whole object.
   *\param __name : the name of the array.
   *\param __index : the access index.
   *\param __anti : QUAST for the anti-dependent operation.
   *\param __out : Quast for the output-dependent operation
   *\return nothing
   * All propetrties will be set to their appropriate values.
   */
   void Set(string __name, FADA_Index* __index, Quast* __anti, Quast* __out);

   void Set(Expression* __expr);
   
public:

 ~Written_Reference();

//! \name Building
   /*!
   *\brief A constructor to create one written scalar.
   *\param __name : the name of the scalar.
   * this constructor will create an empty index, then it calls the advanced setter "Set". For as, a scalar is an zero-dimensional-array.
   *\see Set()
   */
   Written_Reference(string);
   
   /*!
   *\brief A constructor to create a written array cell.
   *\param __name : the name of the array.
   *\param __index : the access index.
   * the call is forwarded by another call to the advanced setter "Set".
   * \see Set()
   */
   Written_Reference(string __name, FADA_Index* __index);
   
   /*!
   *\brief A constructor to create a written array cell with the definition of the anti and the output dependent operations.
   *\param __name : the name of the array.
   *\param __index : the access index.
   *\param __anti : QUAST for the anti-dependent operation.
   *\param __out : Quast for the output-dependent operation
   * It, simply, calls the advanced setter Set
   * \see Set()
   */
   Written_Reference(string __name, FADA_Index* __index ,Quast* __anti, Quast* __out);

      // pour Adrien ... intégration Graphite
   Written_Reference(Expression* __expr);
//!Printing
   /*!
   *\brief It prints the object into a string.
   *\param __level : a level of printing.
   *\return a string containing the printed information
   * Incremental values for "__level" lead to an incremental printing. With the current implementation only level 0 is considered, and it prints only the name and the access-index.
   *
   */
   string Print_str(int __level);
   
   /*!
   *\brief It prints the object into the standart output file.
   *\param __level : a level of printing.
   *\return nothing
   * It is a forward call to Print_str(__level).
   * \see Print_str()
   */
   inline void Print(int __level) {cout<<Print_str(__level);}
   



//! \name Getters
   //! \brief A getter for "anti_dep"
   inline Quast* GetAnti(void) {return anti_dep;}
   //! \brief A getter for "output_dep"
   inline Quast* GetOutput(void) {return output_dep;}
   //! \brief A getter for "name"
   inline string GetName(void) {return name;}
   //! \brief A getter for "index"
   inline FADA_Index* GetIndex(void) {return index;}

//!\name Miscellaneous
   /*!
   *\brief Collecting scalars.
   *\param __written : a set of scalars identified by their names.
   *\return nothing
   * If current object is a scalar, it will be added to the set __written.
   */
   void WrittenScalars(set<string>* __written);

   void   TagIndex(int __stmt, vector<Expression*>* __iter);   
};
};
#endif


