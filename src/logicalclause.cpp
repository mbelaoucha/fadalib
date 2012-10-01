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

#include <logicalclause.h>
#include <global.h>
#include <algorithm>
#include <solver.h>
#include <graphviz.h>
#include <latex.h>

namespace fada{
////////////////////////////////////////////////////////////////
LogicalClause*
LoopProperty(vector<string>* enclosing_loops, string loop_counter, Inequation* non_affine_condition){
vector<string> var=*enclosing_loops;
var.push_back(loop_counter+"1");
var.push_back(loop_counter+"2");
map<string,Expression*> __map;
__map[loop_counter]=new Expression(loop_counter+"2");

 Inequation*copy2=non_affine_condition->Substitute(&__map);
__map.clear();
__map[loop_counter]=new Expression(loop_counter+"1");
Inequation* copy1=non_affine_condition->Negate(true)->Substitute(&__map);

vector<Inequation*> non_affine;
vector<Inequation*> affine;
affine.push_back(new Inequation(new Expression(loop_counter+"1"),FADA_LESS_EQ,new Expression(loop_counter+"2")));
non_affine.push_back(copy1);
non_affine.push_back(copy2);
vector<LC_RHS_Term*> empty;
return new LogicalClause(&var,&affine,&non_affine,&empty);
}
////////////////////////////////////////////////////////////////
vector<Inequation*>
EliminateDoubles(vector<Inequation*>* liste,vector<string>* var, vector<string>* param){
vector<Inequation*> res;
for(vector<Inequation*>::iterator it=liste->begin(); it != liste->end(); ++it){
   string __ineq=(*it)->PrintAll_str(var,param);


   bool found=false;
   
   for(vector<Inequation*>::iterator it2=res.begin(); it2 != res.end(); ++it2){
      if(__ineq == (*it2)->PrintAll_str(var, param)){
         found=true;   
         break;
         }
      }
   if(!found)
      res.push_back(*it);
   }
return res;
}

////////////////////////////////////////////////////////////////

LogicalClause::~LogicalClause(){

}
////////////////////////////////////////////////////////////////
void
LogicalClause::Set(vector<string>* __var,vector<Inequation*>* __affine, vector<Inequation*>* __non_affine, vector<LC_RHS_Term*>* __lex){
SetVariables(__var);
SetAffineInequations(__affine);
SetNonAffineInequations(__non_affine);
SetRHS(__lex);
}
////////////////////////////////////////////////////////////////
void
LogicalClause::Initialize(void){
vector<string> empty_var;
vector<Inequation*> empty_ineq;
vector<LC_RHS_Term*> empty__lex;
Set(&empty_var,&empty_ineq,&empty_ineq,&empty__lex);
}

////////////////////////////////////////////////////////////////
LogicalClause::LogicalClause(vector<string>* __var,vector<Inequation*>* __affine, vector<Inequation*>* __non_affine,vector<LC_RHS_Term*>* __lex){
Initialize();
Set(__var,__affine,__non_affine,__lex);
}
////////////////////////////////////////////////////////////////

string
LogicalClause::Print_Tex_str(){

string result="";
if(GetVariables()->size() > 0)
   result+="\\forall "+PrintVector(GetVariables())+",";
if(GetNonAffineInequations()->size() > 0)
   result+=" ( "+TexPrintDNFTerm(GetNonAffineInequations())+" ), ";
if(GetAffineInequations()->size() > 0)
   result+="("+TexPrintDNFTerm(GetAffineInequations())+")";
 result+=" \\Rightarrow ";
for(vector<LC_RHS_Term*>::iterator it=GetLexRelations()->begin(); it != GetLexRelations()->end();++it){
   if(it != GetLexRelations()->begin())
      result+=" \\vee ";
   result+=(*it)->Print_Tex_str();
   }
return result;
}
////////////////////////////////////////////////////////////////
string
LogicalClause::Full_Tex_Print_str(vector<string>* param){
string result="";
if(GetVariables()->size() > 0)
   result+="\\forall "+PrintVector(GetVariables())+",";
else
   result+="";
if(GetNonAffineInequations()->size() > 0)
   result+=" ( "+TexFullPrintDNFTerm(GetNonAffineInequations(),GetVariables(),param)+" ), ";
if(GetAffineInequations()->size() > 0)
   result+="("+TexPrintDNFTerm(GetAffineInequations())+")";
 result+=" \\Rightarrow ";
for(vector<LC_RHS_Term*>::iterator it=GetLexRelations()->begin(); it != GetLexRelations()->end();++it){
   if(it != GetLexRelations()->begin())
      result+=" \\vee ";
   result+=(*it)->Print_Tex_str();
   }
return result;
}
////////////////////////////////////////////////////////////////
string
LogicalClause::Print_str(){
string result="";
result+="[ for all "+PrintVector(GetVariables())+",";
result+="na("+PrintInequations(GetNonAffineInequations())+"), ";
result+="a("+PrintInequations(GetAffineInequations())+")";
 result+=" ==> ";
for(vector<LC_RHS_Term*>::iterator it=GetLexRelations()->begin(); it != GetLexRelations()->end();++it)
   result+=","+(*it)->Print_str();
return result+"]";
}
////////////////////////////////////////////////////////////////
string
LogicalClause::ToGraphVizLine(string __nn){
string aff,naff,goal;
aff=__nn+"_affine";
naff=__nn+"_non_affine";
goal=__nn+"_goal";
string result=AddDOTNode(aff,PrintInequations(GetAffineInequations()),"green");
result+=AddDOTNode(naff,PrintInequations(GetNonAffineInequations()),"red");
string rhs_label="";
for(vector<LC_RHS_Term*>::iterator it=GetLexRelations()->begin(); it != GetLexRelations()->end();++it)
   rhs_label+=","+(*it)->Print_str();

result+=AddDOTNode(goal,rhs_label,"blue");
result+=AddDotEdge(false,aff,naff,"");
result+=AddDotEdge(false,naff,goal,"");
return result;
}
////////////////////////////////////////////////////////////////
LogicalClause*
LogicalClause::Unify(LogicalClause* __clause, vector<string>* __param){
if(GetNonAffineInequations()->size() > 0 && __clause->GetNonAffineInequations()->size() > 0){
   for(vector<Inequation*>::iterator it=__clause->GetNonAffineInequations()->begin(); it != __clause->GetNonAffineInequations()->end(); ++it){
      for(vector<Inequation*>::iterator itg=GetNonAffineInequations()->begin(); itg!=GetNonAffineInequations()->end(); ++itg){
         vector<Inequation*>   valid;
         if(__Unify(*itg,GetVariables(),*it,__clause->GetVariables(),__param,&valid)){
//             cout<<"\nLogicalClause::Unify ... UNIFICATION RÉUSSIE\n";
            vector<Inequation*>      _non_affine;
            vector<Inequation*>      _affine;
            vector<LC_RHS_Term*>      _lex;
            vector<string>         _var;

            for(vector<Inequation*>::iterator iter=GetNonAffineInequations()->begin(); iter != GetNonAffineInequations()->end(); ++iter)
               if(iter != itg)
                  _non_affine.push_back(*iter);

            for(vector<Inequation*>::iterator iter=__clause->GetNonAffineInequations()->begin(); iter != __clause->GetNonAffineInequations()->end(); ++iter)
               if(iter != it)
                  _non_affine.push_back(*iter);

//             _non_affine=MergeInequations(GetNonAffineInequations(),__clause->GetNonAffineInequations());
            
            if(GetNonAffineInequations()->size() + __clause->GetNonAffineInequations()->size() -2 != _non_affine.size()){
               cerr<<"\nThis :"<<PrintInequations(GetNonAffineInequations()) << " size " <<GetNonAffineInequations()->size();
               cerr<<"\nclause :"<<PrintInequations(__clause->GetNonAffineInequations())<< " size " <<__clause->GetNonAffineInequations()->size();
               cerr<<"\n Non affine :"<<PrintInequations(&_non_affine)<< " size " <<_non_affine.size();
               cerr<<"\n IT :";(*it)->Print();
               cerr<<"\n Ineq1 :";Print();
               cerr<<"\n Ineq2 :";__clause->Print();
               cerr<<"\nLogicalClause::Unify ... WARNING (unified term not found)[1]\n";
               throw(INAPPROPRIATE_CASE);
               exit(EXIT_FAILURE);
               }

            _non_affine=EliminateDoubles(&_non_affine,GetVariables(),__param);
/*            vector<Inequation*>::iterator founded=find(_non_affine.begin(),_non_affine.end(),*it);
            if(founded != _non_affine.end())
               _non_affine.erase(founded);
            else{
               cerr<<"\nThis :"<<PrintInequations(GetNonAffineInequations());
               cerr<<"\nclause :"<<PrintInequations(__clause->GetNonAffineInequations());
               cerr<<"\n Non affine :"<<PrintInequations(&_non_affine);
               cerr<<"\n IT :";(*it)->Print();
               cerr<<"\n Ineq1 :";Print();
               cerr<<"\n Ineq2 :";__clause->Print();
               cerr<<"\nLogicalClause::Unify ... WARNING (unified term not found)[1]\n";
               throw(INAPPROPRIATE_CASE);
               exit(EXIT_FAILURE);
               }
            
            founded=find(_non_affine.begin(),_non_affine.end(),*itg);
            if(founded != _non_affine.end())
               _non_affine.erase(founded);
            else{
               cerr<<"\nLogicalClause::Unify ... WARNING (unified term not found)[2]\n";
               throw(INAPPROPRIATE_CASE);
               exit(EXIT_FAILURE);
               }*/
//             cout<<"\n APRÉS UNIFICATION ......."<<PrintInequations(&_non_affine);
            _affine=EliminateTrueAndDoubles(&MergeInequations(GetAffineInequations(),__clause->GetAffineInequations()));
/*            vector<Inequation*>* __negated=new vector<Inequation*>();
            *__negated=NegateInequations(&valid);*/
            _affine=MergeInequations(&_affine,&valid);
            _lex = Merge(GetLexRelations() , __clause->GetLexRelations());
            _var=MergeVariables(GetVariables(),__clause->GetVariables());
            LogicalClause* new_clause=new LogicalClause(&_var,&_affine,&_non_affine,&_lex);
//             new_clause->Print();
            return new_clause;
            //FIXME : look for all constraints can be unified when unification succeed for a single one.
            //TODO : look for all constraints can be unified when unification succeed for a single one.
            }
         }
      }
   return NULL;      // unification failed
   }

return NULL;         // unification failed
}






////////////////////////////////////////////////////////////////
ContextQuast*
LogicalClause::TraduceToQuast(vector<vector<Inequation*> >* env,vector<string>* __param){
if(IsAffine()){
//     cout<<"\nLogicalClause::TraduceToQuast...............[debug]\n";
//     cout<<"\n ENV = "<<PrintInequations(&env);
   Solver* solver=new Solver(-1,-1,*GetVariables(),*__param,GetAffineInequations());
   Quast* max=solver->Max();

   ContextQuast* context=TraduceToContextQuast(max,GetVariables(),&EliminateDoubles(&RHS2Inequations()));
//    cout<<"\nTraduit direct .......\n"<<context->Print_str("\n");
   context=context->Simplify(*env,__param);
//    cout<<"\nsimplifié .......\n"<<context->Print_str("\n");

   if(context->IsLeaf() && context->GetPolyhedrons()->size()==0){
      vector<vector<Inequation*> > __true;
      vector<vector<Inequation* > >__false;
      vector<Inequation*> v;v.push_back(new Inequation(false));
      __false.push_back(v);
      return new ContextQuast(new Condition(new Inequation(true)),new ContextQuast(&__false), new ContextQuast());
      }
   return context;
   }
return new ContextQuast();
return NULL;
}
////////////////////////////////////////////////////////////////
vector<vector<Inequation*> >
LogicalClause::RHS2Inequations(void){
vector<vector<Inequation*> > result;

for(vector<LC_RHS_Term*>::iterator it=GetLexRelations()->begin(); it != GetLexRelations()->end(); ++it)
   result.push_back((*it)->ToInequations());
return result;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

bool
__Unify(Inequation* ineq1,vector<string>* var1, Inequation* ineq2, vector<string>* var2, vector<string>* param,vector<Inequation*>* inequations){
// cout<<"\n__Unify... "<<ineq1->PrintAll_str(var1,param)<<"   avec   "<<ineq2->PrintAll_str(var2,param);
if(Opposite(ineq1->GetPredicate()) != ineq2->GetPredicate()){
//    cout<<"Fail (operateurs non opposés\n";
   return false;
   }
if(!Equal(ineq1->GetRHS(),var1,ineq2->GetRHS(),var2,param,inequations)){
//    cout<<"Fail 1\n";
   return false;
   }
if(Equal(ineq1->GetLHS(),var1,ineq2->GetLHS(),var2,param,inequations)){
//    cout<<"(SUCCESSFUL)\n"<<PrintInequations(inequations);
   
   return true;
   }
else{
//    cout<<"Fail 2\n";
   return false;
   }
}

////////////////////////////////////////////////////////////////

bool
Equal(Expression* exp1, vector<string>* var1,Expression* exp2, vector<string>* var2, vector<string>* param,vector<Inequation*>* inequations){
//assert(param);
if(exp1->IsLeaf() && exp2->IsLeaf()){
   
   if(exp1->IsVariable() && exp2->IsVariable() && exp1->GetVariableName() == exp2->GetVariableName() &&
   find(param->begin(), param->end(), exp1->GetVariableName() ) != param->end())
   {
      return true;
   }


   if(exp1->IsValue() && exp2->IsValue() && exp1->GetValue() == exp2->GetValue())
      return true;

   if(exp1->IsValue() && exp2->IsValue() && exp1->GetValue() != exp2->GetValue())
      return false;

   if(exp1->IsFunction() && exp2->IsFunction() && exp1->GetFunctionName() != exp2->GetFunctionName())
      return false;

    if(exp1->IsArray() && exp2->IsArray() && exp1->GetArrayName() != exp2->GetArrayName())
      return false;


   if(exp1->IsFunction() && exp2->IsFunction() && exp1->GetFunctionName() == exp2->GetFunctionName()){
      vector<Expression*>::iterator it2=exp2->GetArguments()->begin();
      for(vector<Expression*>::iterator it=exp1->GetArguments()->begin(); it != exp1->GetArguments()->end() && it2 != exp2->GetArguments()->end(); ++it,++it2){
         if(!Equal(*it,var1,*it2,var2,param,inequations))
            return false;
         }
      return true;

      }

    if(exp1->IsArray() && exp2->IsArray() && exp1->GetArrayName() == exp2->GetArrayName()){
      if(exp1->GetReadStmt()!=exp2->GetReadStmt())
         return false;
      if(exp1->GetIteration()->size() != exp2->GetIteration()->size())
         return false;

      vector<Expression*>::iterator it2=exp2->GetIteration()->begin();
      for(vector<Expression*>::iterator it=exp1->GetIteration()->begin(); it != exp1->GetIteration()->end() && it2 != exp2->GetIteration()->end(); ++it,++it2){
         if(!Equal(*it,var1,*it2,var2,param,inequations))
            return false;
         }
      it2=exp2->GetIndex()->begin();
      for(vector<Expression*>::iterator it=exp1->GetIndex()->begin(); it != exp1->GetIndex()->end() && it2 != exp2->GetIndex()->end(); ++it,++it2){
         if(!Equal(*it,var1,*it2,var2,param,inequations))
            return false;
         }
      return true;
      }

   if((exp1->IsValue() && (exp2->IsFunction() || exp2->IsArray())) || (exp2->IsValue() && (exp1->IsFunction() || exp1->IsArray())))
      return false;

   if(exp1->IsVariable() && find(var1->begin(), var1->end(),exp1->GetVariableName()) != var1->end()){
      if(exp2->IsVariable() && find(var2->begin(), var2->end(),exp2->GetVariableName()) != var2->end()){
         if(exp1->GetVariableName() == exp2->GetVariableName())
            return true;
         }
      inequations->push_back(new Inequation(exp1,FADA_EQ,exp2));
      return true;      
            // FIXME : it's strongly recommended to generate affine inequations only 
            // TODO  : check wether substituted entity is a parameter
      }

   if(exp2->IsVariable() && find(var2->begin(), var2->end(),exp2->GetVariableName()) != var2->end()){
      inequations->push_back(new Inequation(exp2,FADA_EQ,exp1));
      return true;
            // FIXME : it's strongly recommended to generate affine inequations only 
            // TODO  : check wether substituted entity is a parameter
      }

   if(exp2->IsVariable() && find(var2->begin(), var2->end(),exp2->GetVariableName()) == var2->end() && exp1->IsVariable() && find(var1->begin(), var1->end(),exp1->GetVariableName()) == var1->end() && exp2->GetVariableName() != exp1->GetVariableName())
      return false;

   if(exp2->IsVariable() && find(var2->begin(), var2->end(),exp2->GetVariableName()) == var2->end() && exp1->IsVariable() && find(var1->begin(), var1->end(),exp1->GetVariableName()) == var1->end() && exp2->GetVariableName() == exp1->GetVariableName())
      {
      if(exp1->GetReadStmt() != exp2->GetReadStmt())
         return false;
      if(exp1->GetIteration()->size() != exp2->GetIteration()->size())
         return false;

      

       vector<Expression*>::iterator it2=exp2->GetIteration()->begin();
       for(vector<Expression*>::iterator it=exp1->GetIteration()->begin(); it != exp1->GetIteration()->end() && it2 != exp2->GetIteration()->end();++it, ++it2){
      #if 0
         cout<<"\n";

         (*it)->Print();
         cout<<" == ";
           (*it2)->Print();
         cout<<" ?!!";
      #endif
         if(!Equal(*it,var1,*it2,var2,param,inequations))
            return false;
         }
      return true;
      }

   return false;
   cerr<<"\n Equal (Expression1*, var1, Expression2 ....) ... FATAL ERROR(no matching case)\n";
   cerr<<"\nExp1 : "<<exp1->Print_str();
   cerr<<"\nExp2 : "<<exp2->Print_str()<<"\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }

if(exp1->IsLeaf() && !exp2->IsLeaf()){
   if(exp1->IsVariable() && find(var1->begin(), var1->end(),exp1->GetVariableName()) != var1->end()){
      inequations->push_back(new Inequation(exp1, FADA_EQ,exp2));
      return true;
         // FIXME : it's strongly recommended to generate affine inequations only 
         // TODO  : check wether substituted entity is a parameter
      }
   return false;
   }

if(exp2->IsLeaf() && !exp1->IsLeaf()){
   if(exp2->IsVariable() && find(var2->begin(), var2->end(),exp2->GetVariableName()) != var2->end()){
      inequations->push_back(new Inequation(exp2, FADA_EQ,exp1));
      return true;
         // FIXME : it's strongly recommended to generate affine inequations only 
         // TODO  : check wether substituted entity is a parameter
      }
   return false;
   }

if(exp1->GetOperation() == exp2->GetOperation())
   if(Equal(exp1->GetRightChild(),var1,exp2->GetLeftChild(),var2,param,inequations))
      return Equal(exp1->GetLeftChild(),var1,exp2->GetLeftChild(),var2,param,inequations);

return false;
}


};


