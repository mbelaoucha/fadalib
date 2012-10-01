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

#ifndef EDGE_H
#define EDGE_H

#include "index.h"
#include "condition.h"
#include "graphprinter.h"

namespace fada{


enum   dependence_type{flow,anti,output};

class Edge{

private:
   std::string      variable;
   FADA_Index*      index;
   Condition*      validity;
   FADA_Index*      sigma;
   dependence_type    dep_type;


void   Initialize();

void      Set(std::string name, FADA_Index* i, Condition* valid,FADA_Index*s);
inline void   SetVariableName(std::string name)   {variable=name;};
inline void   SetIndex(FADA_Index* __i)      {index=__i;};
inline void   SetCondition(Condition*c)      {validity=c;};
inline void   SetSigma(FADA_Index* fct)      {sigma=fct;};
inline void   SetDependenceType(dependence_type t)   {dep_type=t;};
inline void   SetFlowDependence(void)         {dep_type=flow;}
inline void   SetAntiDependence(void)         {dep_type=anti;};
inline void   SetOutputDependence(void)      {dep_type=output;};

public:
   Edge();
   Edge(std::string name, FADA_Index* i, Condition* valid,FADA_Index*s);
   ~Edge();
   

std::string   GetVariableName(){return variable;}
FADA_Index*   GetIndex(){return index;}
FADA_Index*   GetSigma(){return sigma;}
Condition*   GetCondition(){return validity;}

   string   Print_str(std::string indent);
   void Print(std::string s);
   string   ToGraphViz(long unsigned int rank,vector<string> loops,long unsigned int write,GraphPrinterOptions op);
   string   ToVCG(long unsigned int rank,vector<string> loops,long unsigned int write,GraphPrinterOptions op);

   };   // END class Edge


};   // END namespace


#endif


