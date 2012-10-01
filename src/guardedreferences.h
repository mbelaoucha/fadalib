/***************************************************************************
 *   Copyright (C) 2008 by Marouane Belaoucha    *
 *   bem@prism.uvsq.fr   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef GUARDEDREFERENCES_H
#define GUARDEDREFERENCES_H

#include <references.h>
#include <vector>
#include <ldemonstrator.h>
#include <options.h>
/**
   @author Marouane Belaoucha  <bem@prism.uvsq.fr>
*/
namespace fada{

class GuardedReferences{
private:
vector<References*>   guarded_references;
vector<vector<bool> >   textual_precedences;
vector<string>      global_parameters;
// vector<string>      all_parameters;
LDemonstrator*      global_properties;
Options*      options;
inline void      SetGuardedReferences(vector<References*>* ref)      {guarded_references=*ref;}
inline void      SetTextualPrecedences(vector<vector<bool> > * matrix)   {textual_precedences=*matrix;}
inline void      SetGlobalParameters(vector<string>* __param)      {global_parameters=*__param;}
//       all_parameters;
inline void      SetGlobalProperties(LDemonstrator* __ppts)      {global_properties=__ppts;}
inline void      SetOptions(Options* op)               {options=op;}

void         Set(   vector<References*>* __ref,
            vector<vector<bool> >* __to,
            vector<string>* __gp, 
            LDemonstrator* __lp, 
            Options* __op);

void         Initialize();
inline void      TagIndexes(void);
void         LookForParameters(void);
public:
    GuardedReferences();
    ~GuardedReferences();


vector<References*>*   GetGuaredReferences()      {return &guarded_references;}
vector<vector<bool> > *   GetTextualPrecedences()      {return &textual_precedences;}
vector<string>*      GetGlobalParameters()      {return &global_parameters;}
Options*      GetOptions(void)      {return options;}
//       all_parameters;
LDemonstrator*      GetGlobalProperties()      {return global_properties;}

void         Preprocess(void);
void         ComputeSources(void);
};
};
#endif


