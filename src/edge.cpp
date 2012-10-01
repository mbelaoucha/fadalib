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



#include <iostream>
#include "edge.h"
#include "global.h"

namespace fada{
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void
Edge::Initialize()
{
Set("",NULL,NULL,NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void   
Edge::Set(std::string name, FADA_Index* i, Condition* valid,FADA_Index*s){
SetVariableName(name);
SetIndex(i);
SetCondition(valid);
SetSigma(s);
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Edge::Edge(){
Initialize();
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Edge::Edge(std::string name, FADA_Index* i, Condition* valid,FADA_Index*s){
Initialize();
Set(name,i,valid,s);
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Edge::~Edge(){
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

string
Edge::Print_str(std::string indent)   {
ostringstream s;
s<<""<<indent<<" Var ="<<variable;
s<<""<<indent<<" Index="<<indent<<GetIndex()->Print_str();
s<<""<<indent<<" Validity ="<<indent<<GetCondition()->Print_str();
s<<""<<indent<<" Sigma ="<<indent<<GetSigma()->Print_str();
return s.str();
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void 
Edge::Print(std::string s){
std::cout<<Print_str(s);
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


string
Edge::ToVCG(long unsigned int rank,vector<string> loops,long unsigned int write,GraphPrinterOptions op){

ostringstream result;
result<< op.sigma<<"( "+GetVariableName() 
      +GetIndex()->Generate_C_Code()+")   =   "<<PrintOperation(write,GetSigma()->GetIndex(),op);
result<<"\\n  IF \\n "<<GetCondition()->Generate_C_Code();
return result.str();

}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
string
Edge::ToGraphViz(long unsigned int rank,vector<string> loops, long unsigned int write,GraphPrinterOptions op){
ostringstream result;
result<< op.sigma<<"( "+GetVariableName() 
      +GetIndex()->Generate_C_Code()+")   =   "<<PrintOperation(write,GetSigma()->GetIndex(),op);
result<<"\\n  IF \\n "<<GetCondition()->Generate_C_Code();
return result.str();
}

};


