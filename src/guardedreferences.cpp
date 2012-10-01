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
#include "guardedreferences.h"

namespace fada{
GuardedReferences::GuardedReferences()
{
}


GuardedReferences::~GuardedReferences()
{
}


void
GuardedReferences::TagIndexes(void){
for(vector<References*>::iterator it=GetGuaredReferences()->begin(); it != GetGuaredReferences()->end(); ++it)
   (*it)->TagIndexes();

}

void
GuardedReferences::LookForParameters(void){
set<string> read,written;
for(vector<References*>::iterator it=GetGuaredReferences()->begin(); it != GetGuaredReferences()->end(); it++)
   (*it)->CollectReferencedScalars(&written,&read);

// References* s;
// s->
for(set<string>::iterator it=read.begin(); it != read.end(); ++it)
   if(written.find(*it) != written.end())
      GetGlobalParameters()->push_back(*it);
}

void
GuardedReferences::Preprocess(void){
for(vector<References*>::iterator it=GetGuaredReferences()->begin(); it != GetGuaredReferences()->end(); it++)
   (*it)->NormalizeDomainConstraints(GetGlobalParameters());

TagIndexes();
}

void
GuardedReferences::ComputeSources(void){


for(vector<References*>::iterator it=GetGuaredReferences()->begin(); it != GetGuaredReferences()->end(); ++it)
   (*it)->ComputeDefinitions(   
         GetGlobalParameters(),
         GetGuaredReferences(),
         GetTextualPrecedences(),
         GetGlobalProperties(),
         GetOptions()
         );

}

void
GuardedReferences::Initialize(){
guarded_references.clear();
textual_precedences.clear();
global_parameters.clear();
global_properties= new LDemonstrator();
options=new Options();
}

void
GuardedReferences::Set(   vector<References*>* __ref,
            vector<vector<bool> >* __to,
            vector<string>* __gp, 
            LDemonstrator* __lp, 
            Options* __op){
SetGuardedReferences(__ref);
SetTextualPrecedences(__to);
SetGlobalParameters(__gp);
SetGlobalProperties(__lp);
SetOptions(__op);
}
};



