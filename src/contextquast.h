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

#ifndef ADAAN_CONTEXTQUAST_H
#define ADAAN_CONTEXTQUAST_H

#include "condition.h"
#include <inequation.h>
#include <vector>

namespace fada{
class Expression;
class Inequation;
class Condition;
class Quast;
class FADA_Index;

/*!
\file adaan_contextquast.h
\brief This file contain the description of the class ContextQuast
*/

/*!
\class ContextQuast
\brief Describes a context quast.
*/

class ContextQuast{
private:
//! \name For all kind of nodes
   bool            is_leaf;   //!< Am i a leaf ?
//! \name For leaves only
   vector<vector<Inequation*> >   polyhedrons;   //!< affine constraints
//! \name form internal nodes only
   ContextQuast*      then_part;   //!< then sub-quast
   ContextQuast*      else_part;   //!< else sub-quast
   Condition*      cond;      //!< the internal code' condition

//! \name Setters
   //!\brief A setter "is_leaf"
   inline      void   SetLeaf(bool __leaf=true)      {is_leaf=__leaf;}
   //!\brief A setter "cond"
   inline      void   SetCondition(Condition*   __cond)   {cond=__cond;}
   //!\brief A setter "polyhedron"
   inline      void   SetLeafConstraints(vector<vector<Inequation*> >*   poly)   {polyhedrons=*poly;}
   //!\brief A setter "then_part"
   inline      void   SetThenPart(ContextQuast* __then)   {then_part=__then;}
   //!\brief A setter "else_part"
   inline      void   SetElsePart(ContextQuast* __else)   {else_part=__else;}

//! \name Advanced Setters
      //!\brief Set to an empty leaf
   void      Set(void);
      //!\brief Set to non-empty leaf
   void      Set(vector<vector<Inequation*> >*   __polyhedron);
      //!\brief Set to an internal node
   void      Set(Condition*   __cond, ContextQuast* __then, ContextQuast* __else);
      //!\brief Set all properties to their NULL values
   void      Initialize(void);

public:

~ContextQuast();
//!\name Building
   //!\brief Build an empty quast
   ContextQuast(void);
   //!\brief Build a leaf filled with affine constraints
   ContextQuast(vector<vector<Inequation*> >*   __polyhedron);
   //!\brief Build an internal node
   ContextQuast(Condition*   __cond, ContextQuast* __then, ContextQuast* __else);

//!\name Getters
   //!\brief A setter for "is_leaf"
   bool            IsLeaf(void)      {return is_leaf;}
   //!\brief A setter for "then_part"
   ContextQuast*      GetThenPart(void)   {return then_part;}
   //!\brief A setter for "else_part"
   ContextQuast*      GetElsePart(void)   {return else_part;}
   //!\brief A setter for "cond"
   Condition*      GetCondition(void)   {return cond;}
   //!\brief A setter for "polyhedron"
   vector<vector<Inequation*> >*   GetPolyhedrons(void)   {return &polyhedrons;}
   
//! \name Miscellaneous
   //!\brief A setter for "polyhedron"
   void   SetPolyhedron(vector<vector<Inequation*> >* poly)   {polyhedrons=*poly;}
   //!\brief Compute the and of two context quast.
   ContextQuast*      Max (ContextQuast* context_quast);
   string            Print_str(string prefix);
   //!\brief Simplification
//    ContextQuast*      Simplify(vector<Inequation*> __env, vector<string>* parameters);
   ContextQuast*      Simplify(vector<vector<Inequation*> > __env,vector<string>* parameters);

   vector<vector<Inequation*> >   GetPolyhedrons(vector<Inequation*>);
   ContextQuast*         EliminateUnreachableNodes(vector<vector<Inequation*> > __env, vector<string>* __param);
};


};
#endif



