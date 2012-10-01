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

#include "elementarydependence.h"
#include <global.h>
#include <solver.h>
#include <latex.h>
#include <algorithm>

namespace fada{
////////////////////////////////
////////////////////////////////

ElementaryDependence::~ElementaryDependence()
{
}

////////////////////////////////
////////////////////////////////

void
ElementaryDependence::Set(int __1, int __2, int __d,FADA_Index* __i){
SetStmt1(__1);
SetStmt2(__2);
SetDeep(__d);
SetIndex( __i);
}

////////////////////////////////
////////////////////////////////

void
ElementaryDependence::Set(int __1, int __2, int __d){
SetStmt1(__1);
SetStmt2(__2);
SetDeep(__d);
}
////////////////////////////////
////////////////////////////////

void
ElementaryDependence::Set(vector<string>* __stmt1, vector<string>* __stmt2, vector<string>* __p, vector<vector<Inequation*> >* __ae,
         vector<vector<Inequation*> >* __nae, vector<vector<Inequation*> >* __ac, 
         vector<vector<Inequation*> >* __nac, vector<Inequation*> * __preced,
          vector<Inequation*> * __aie, vector<Inequation*>* __anie, Quast* __dep){

SetStmt1Iteration(__stmt1);
SetStmt2Iteration(__stmt2);
SetParameters(__p);
SetAffineEnvironment(__ae);
SetNonAffineEnvironment(__nae);
SetAffineValidStmt2(__ac);
SetNonAffineValidStmt2(__nac);
SetPrecedenceConstraints(__preced);
SetAffineIndexEquality(__aie);
SetNonAffineIndexEquality(__anie);
SetQuast(__dep);
}
////////////////////////////////
////////////////////////////////

void
ElementaryDependence::Initialize(){
vector<string>   empty;
vector<vector< Inequation* > > empty2;
vector< Inequation* >   empty3;
Set(-1,-1,-1,NULL);
Set(&empty,&empty,&empty,&empty2,&empty2,&empty2,&empty2,&empty3,&empty3,&empty3,NULL);
properties=new LDemonstrator();
}

////////////////////////////////
////////////////////////////////

ElementaryDependence::ElementaryDependence(int __1, int __2, int __d,FADA_Index*__i){
Initialize();
Set(__1,__2,__d,__i);
}

ElementaryDependence::ElementaryDependence(int __1, int __2, int __d){
Initialize();
Set(__1,__2,__d);
}

////////////////////////////////
////////////////////////////////

string
ElementaryDependence::Print_str(int level){
ostringstream   out;
out<<"( r: "<< GetStmt1() << " , w : "<<GetStmt2() << " , d : "<<GetDeep()<< " )";
if( level == 0 )   return out.str();

out<<"\nSTMT 1 iteration vector ...";
for(vector<string>::iterator it=GetStmt1Iteration()->begin(); it != GetStmt1Iteration()->end(); ++it){
   out<<"\t"<<*it;
   }

out<<"\nSTMT 2 iteration vector...";
for(vector<string>::iterator it=GetStmt2Iteration()->begin(); it != GetStmt2Iteration()->end(); ++it){
   out<<"\t"<<*it;
   }

out<<"\nAffine Environment ...";
for(vector<vector<Inequation*> >::iterator it=GetAffineEnvironment()->begin(); it != GetAffineEnvironment()->end(); ++it){
   out<<"\n";
   for(vector<Inequation*>::iterator iti = (*it).begin(); iti != (*it).end() ; ++iti)
      out<<"\t"<<(*iti)->Print_str();
   }

out<<"\nNON-Affine Environment ...";
for(vector<vector<Inequation*> >::iterator it=GetNonAffineEnvironment()->begin(); it != GetNonAffineEnvironment()->end(); ++it){
   out<<"\n";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      out<<"\t"<<(*iti)->Print_str();
   }

out<<"\nAffine inequation from \"valid(statement2)\"  ...";
for(vector<vector<Inequation*> >::iterator it=GetAffineValidStmt2()->begin(); it != GetAffineValidStmt2()->end(); ++it){
   out<<"\n";
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      out<<"\t"<<(*iti)->Print_str();
   }

out<<"\nNon-affine inequations from \"valid(statement2)\"  ... ";
out<<"\n";
for(vector<vector<Inequation*> >::iterator it=GetNonAffineValidStmt2()->begin(); it != GetNonAffineValidStmt2()->end(); ++it){
   for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
      out<<"\t"<<(*iti)->Print_str();
   }

out<<"\nAffine inequations from 'indexes equality'....";
out<<"\n";
for(vector<Inequation*> ::iterator it=GetAffineIndexesEquality()->begin(); it != GetAffineIndexesEquality()->end(); ++it)
   out<<"\t"<<(*it)->Print_str();

out<<"\nNon-affine inequations from 'indexes equality'";
out<<"\n";
for(vector<Inequation*> ::iterator it=GetNonAffineIndexesEquality()->begin(); it != GetNonAffineIndexesEquality()->end(); ++it)
   out<<"\t"<<(*it)->Print_str();

out<<"\nPrecedence";
out<<"\n";
for(vector<Inequation*> ::iterator it=GetPrecedence()->begin(); it != GetPrecedence()->end(); ++it)
   out<<"\t"<<(*it)->Print_str();
return out.str();
}

////////////////////////////////
void 
ElementaryDependence::BuildConstraints(
               vector<string>*      __global_parameters,
               vector<References*>*    __references,
               vector<string>*      __parameterized_iteration,
               vector<vector<Inequation* > >* __affine_env,
               vector<vector<Inequation* > >* __non_affine_env,
               string __conflictual_variables,
               FADA_Index* __parameterized_Index
               ){

int read_stmt =GetStmt1();
int write_stmt=GetStmt2();
int common=GetDeep();
SetStmt1Iteration(__parameterized_iteration);
SetStmt2Iteration(__references->operator[](write_stmt)->GetCounters());
SetParameters(__global_parameters);


   // recover the write index
FADA_Index* write_index=NULL;
vector<Written_Reference*>* written_variables_=(__references->operator[](write_stmt)->GetWV());

for(vector<Written_Reference*>::iterator it=written_variables_->begin(); it != written_variables_->end(); ++it){
   if((*it)->GetName()==__conflictual_variables){
      write_index=(*it)->GetIndex();
      break;
      }
   }
if(write_index==NULL){
   cerr<<"\nElementaryDependence::BuildConstraints2 ... FATAL ERROR (inapropriate case [variable"<<__conflictual_variables<<" is not written by statment #"<<write_stmt<<" ])\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }


   // compute common loops
vector<string>*   read_loops,*write_loops, common_loops;
read_loops =__references->operator[](read_stmt)->GetCounters();
write_loops=__references->operator[](write_stmt)->GetCounters();

common_loops=Common(read_loops,write_loops);


if(common_loops.size() < common){
   cerr<<"\nElementaryDependence::BuildConstraints2 ... FATAL ERROR (inappropriate case [common_loops < common]) \n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }


   // Compute precedence constraints (always affine)
vector<Inequation*>*  __precedence=new vector<Inequation*>();
*__precedence=Precedence(write_loops, __parameterized_iteration,common_loops.size(),common);
SetPrecedenceConstraints(__precedence);      //lexicographic precedence

   // Set the environment
SetAffineEnvironment(__affine_env);
SetNonAffineEnvironment(__non_affine_env);

   // valid write (domain)

SetAffineValidStmt2(__references->operator[](write_stmt)->GetAffineConstraints());
SetNonAffineValidStmt2(__references->operator[](write_stmt)->GetNonAffineConstraints());

   // indexes equality

Condition*   __same_cell=__parameterized_Index->Equal(write_index);
vector<Inequation*> all_eq=__same_cell->GetTermInequations();
vector<Inequation*> affine_eq,non_affine_eq;
SortInequations(&all_eq,&affine_eq,&non_affine_eq,write_loops, GetParameters());
SetAffineIndexEquality(&affine_eq);
SetNonAffineIndexEquality(&non_affine_eq);


   // inject pragma-constraints into properties stack
// if(GetDeep()>=GetStmt2Iteration()->size()){   
      // local pragma constraints are applyied only for current iteration.

{
   vector<Inequation*>    empty;
   LC_RHS_Term* __false=new LC_RHS_Term(new Inequation(false));
   vector<LC_RHS_Term*> ___false;
   ___false.push_back(__false);
   vector<Inequation*>* write_pragma_constraints=__references->operator[](write_stmt)->GetPragmaConstraints();
   if(write_pragma_constraints->size() > 0)
      GetProperties()->PUSHBACK(new LogicalClause(GetStmt2Iteration(),&empty,write_pragma_constraints,&___false));
 }


}

////////////////////////////////
////////////////////////////////
////////////////////////////////
////////////////////////////////

void
ElementaryDependence::ComputeTheMaximum(
      LDemonstrator*      __global_properties,
      Options* op
      ){

   // this part is to be removed when the structural analysis will be able to recover exact values for the parameter of the maximum.
if(op->processing.apply_structural_analysis){

   vector<Inequation*>   non_affine_env;
   vector<Inequation*>   non_affine_valid2;

   if(GetNonAffineEnvironment()->size()>0)
      non_affine_env=*(GetNonAffineEnvironment()->begin());

   if(GetNonAffineValidStmt2()->size() > 0)
      non_affine_valid2=*(GetNonAffineValidStmt2()->begin());


//    cout<<"\n ENV NON AFFINE "<<PrintInequations(&non_affine_env);
//    cout<<"\n ENV NON AFFINE VALID STMT2 "<<PrintInequations(&non_affine_valid2);

   vector<Inequation*> result;
   
   for(vector<Inequation*>::iterator it2=non_affine_valid2.begin(); it2!= non_affine_valid2.end(); ++it2){
      bool insert=true;
      Inequation* __ineq2=(*it2);
      for(vector<Inequation*>::iterator it=non_affine_env.begin(); it != non_affine_env.end(); ++it){
         Inequation* __ineq=(*it)->Negate(true);
         vector<string> empty;
         vector<Inequation*> res;
   
         if(__Unify(__ineq2,GetStmt2Iteration(),__ineq,&empty,GetParameters(),&res)){
/*            cout<<"\n UNIFY \n";
            getchar();*/
            LDemonstrator* local=new LDemonstrator();
            *local=*__global_properties;
            vector<Inequation*> empty;
            vector<string> empty3;
            vector<Inequation*> non_affine;
            vector<LC_RHS_Term*> empty2;
   
            non_affine.push_back(__ineq2);
            local->PUSHINFRONT(new LogicalClause(&empty3,&empty,&non_affine,&empty2));
            local->Inference(GetParameters());
            local->POPAffines();
            non_affine.clear();
            non_affine.push_back(__ineq);
            local->PUSHINFRONT(new LogicalClause(GetStmt2Iteration(),&empty,&non_affine,&empty2));
            local->Inference(GetParameters());
            local=local->POPAffines();
//             local->Print();
            if(local->IsAccurate(GetPrecedence(),GetParameters())){
//                cout<<"\nHi............";
               insert=false;
               break;
               }
            }
         }
      if(insert)
         result.push_back(*it2);
      }

   vector<vector<Inequation*> > non_aff;
   if(result.size()>0)
      non_aff.push_back(result);
    SetNonAffineValidStmt2(&non_aff);

/*   
   cout<<"\ncontraintes restantes sont \n"<<PrintInequations(&non_aff);
   getchar();*/
   }



if(IsThereNonAffineConstraints()){

   //parameterization
   vector<string>         __new_variables;
   vector<string>         __local_parameters=*GetParameters();

   vector<string>         __the_parameter_of_the_maximum;
   map<string,string>      __mapping_counters_alphas;
   map<string,Expression*>      __hard_map_counters_alphas;
   int i=0;
   vector<Inequation*>   added_constraints;
   for(vector<string>::iterator it=GetStmt2Iteration()->begin(); it != GetStmt2Iteration()->end();++it){
      ostringstream   nv;
      ostringstream   alpha;
      
      nv<<FADA_New_var<<i;
      if(!op->printing.human_friendly)
         alpha<<FADA_Alpha"_"<<GetStmt2()<<"_"<<GetDeep()<<"_"<<i;
      else
         if(!op->printing.command_line_printing  && op->printing.tex_printing)
            alpha<<*it<<"_{"<<GetStmt2()<<"}^{"<<GetDeep()<<"}";
         else
            alpha<<*it<<"_"<<GetStmt2()<<"_"<<GetDeep();



      __mapping_counters_alphas[*it]=alpha.str();
      __hard_map_counters_alphas[*it]=new Expression(alpha.str());
      added_constraints.push_back(new Inequation(new Expression(*it),FADA_EQ,new Expression(nv.str())));
      __new_variables.push_back(nv.str());
      __local_parameters.push_back(*it);
      __local_parameters.push_back(alpha.str());
      GetParameters()->push_back(alpha.str());
      __the_parameter_of_the_maximum.push_back(alpha.str());
      ++i;
      }
   ostringstream   _alpha;
   _alpha<<FADA_Alpha<<"_"<<GetStmt2()<<"_"<<GetDeep();

   SetParameterOfTheMaximum(&__the_parameter_of_the_maximum);

   vector<vector<Inequation*> >   constraints_on_parameter;
   
   Quast*   result=new Quast();
   //build affine systems

   for(vector<vector <Inequation*> >::iterator itw=GetAffineValidStmt2()->begin(); itw != GetAffineValidStmt2()->end(); ++itw){
      //valid read
      vector<Inequation*> system;//=*itr;


         //valid write
      for(vector<Inequation*>::iterator iti=(*itw).begin(); iti != (*itw).end(); ++iti){
         system.push_back(*iti);
         }

         // precedence
      for(vector<Inequation*>::iterator itp=GetPrecedence()->begin(); itp != GetPrecedence()->end(); ++itp){
         system.push_back(*itp);
         }

         // same variable
      for(vector<Inequation*>::iterator itsc=GetAffineIndexesEquality()->begin(); itsc != GetAffineIndexesEquality()->end(); ++itsc){
         system.push_back(*itsc);
         }

      system=EliminateTrueAndDoubles(&system);

         ///////////////////////////////////////////////

      LexicographicInequation* lex=new LexicographicInequation(GetStmt2Iteration(),LEX_LESS, GetStmt2Iteration()->size(), GetParameterOfTheMaximum());
//       if(GetParameterOfTheMaximum()->size()<=0){
//          cout<<"\nElementaryDependence::ComputeTheMaximum .......FATAL ERROR : got non affine constraints from statement 'ID#"<<GetStmt2()<<"' supposed to be invariant\n";
//          throw(INAPPROPRIATE_CASE);
//          exit(EXIT_FAILURE);
//          }

      Inequation*ineq=new Inequation(new Expression(_alpha.str()),FADA_GREATER,new Expression(FADA_Bottom));
      LC_RHS_Term* rhs=new LC_RHS_Term(lex,ineq);
      vector<LC_RHS_Term*> vect;
      vect.push_back(rhs);

      if(GetNonAffineValidStmt2()->size()>0){
         for(vector<vector<Inequation*> >::iterator itnaff=GetNonAffineValidStmt2()->begin(); itnaff != GetNonAffineValidStmt2()->end(); ++itnaff){
            vector<Inequation*> non_affine_constraints=MergeInequations(&(*itnaff),GetNonAffineIndexesEquality());
            
            LogicalClause* __clause= new LogicalClause(GetStmt2Iteration(),/*&(*itaff)*/&system,&non_affine_constraints/*&(*itnaff)*/,&vect);
            GetProperties()->PUSHBACK(__clause);
            }
         }
      else{
         LogicalClause* __clause= new LogicalClause(GetStmt2Iteration(),&system,GetNonAffineIndexesEquality(),&vect);
         GetProperties()->PUSHBACK(__clause);
         }
      Inequation*no_bottom=new Inequation(new
      Expression(_alpha.str()),FADA_LESS_EQ,new Expression(FADA_Bottom));
      vector<Inequation*> no_bottom_vect;
      no_bottom_vect.push_back(no_bottom);
      vector<Inequation*> __v;
      __v=*(GetAffineValidStmt2()->begin());
      __v=EliminateTrueAndDoubles(&__v);

      for(vector<Inequation*>::iterator it=__v.begin(); it != __v.end(); ++it){
            LC_RHS_Term* rhs=new LC_RHS_Term((*it)->Negate(true)->Substitute(&__hard_map_counters_alphas));
            vector<LC_RHS_Term*> vect;
            vector<string> var;
            vector<Inequation*> non_aff;
            vect.push_back(rhs);
            LogicalClause* __clause= new LogicalClause(&var,&no_bottom_vect,&non_aff,&vect);
            GetProperties()->PUSHBACK(__clause);
            }
         
      Inequation*bottom=new Inequation(new
      Expression(_alpha.str()),FADA_GREATER,new Expression(FADA_Bottom));
      vector<Inequation*> bottom_vect;
      bottom_vect.push_back(bottom);
      for(vector<Inequation*>::iterator it=__v.begin(); it != __v.end(); ++it){
         LC_RHS_Term* rhs=new LC_RHS_Term((*it)->Substitute(&__hard_map_counters_alphas));
         vector<LC_RHS_Term*> vect;
         vector<string> var;
         vector<Inequation*> non_aff;
         vect.push_back(rhs);
         LogicalClause* __clause= new LogicalClause(&var,&bottom_vect,&non_aff,&vect);
         GetProperties()->PUSHBACK(__clause);
         }
            ///////////////////////////////////////////////

         
            // added constraints

      for(vector<Inequation*>::iterator ita=added_constraints.begin(); ita != added_constraints.end(); ++ita)
         system.push_back(*ita);

      Solver   solver(GetStmt2(),GetDeep(),__new_variables,__local_parameters,&system);
/*      cout<<"\n Solver = \n";
      solver.Print();*/
      Quast* local__result=solver.Max();
/*      cout<<"\n local = \n";
      local__result->Print();*/
      local__result->Substitute(&__mapping_counters_alphas);
      result=result->Max(local__result);
//       cout<<"\n final = \n";
//       result->Print();
//       getchar();

      }
   GetParameterOfTheMaximum()->push_back(_alpha.str());
   result->Substitute(&__mapping_counters_alphas);   
   result=result->EliminateRedondantLeaves(false);
   result=result->Simplify(*GetAffineEnvironment(),GetParameters());
//    result=result->Compress();
   SetQuast(result);
   }
else{
   // all constraints are affine ==> compute the exact definition
   vector<string>         __local_parameters=*GetParameters();
   Quast*   result=new Quast();    // _|_
   //build affine systems
   int i=0;
   for(vector<vector <Inequation*> >::iterator itw=GetAffineValidStmt2()->begin(); itw != GetAffineValidStmt2()->end(); ++itw){
         
            // valid read
         vector<Inequation*> system;//=*itr;
//          if(GetAffineEnvironment()->size()>0);
//             system=*GetAffineEnvironment()->begin();

            //valid write
         for(vector<Inequation*>::iterator iti=(*itw).begin(); iti != (*itw).end(); ++iti)
            system.push_back(*iti);

            // lexicographic precedence
         for(vector<Inequation*>::iterator itp=GetPrecedence()->begin(); itp != GetPrecedence()->end(); ++itp)
            system.push_back(*itp);

            // same variable
         for(vector<Inequation*>::iterator itsc=GetAffineIndexesEquality()->begin(); itsc != GetAffineIndexesEquality()->end(); ++itsc)
            system.push_back(*itsc);

         Solver   solver(GetStmt2(),GetDeep(),*GetStmt2Iteration(),__local_parameters,&system);
         Quast*   local_result=solver.Max();
         result=result->Max(local_result);
      }

   
//    cout<<"\n***************************************************************************\n";
//    cout<<"\n AVANT LES SIMPLIFS"<<result->Print_str("\n");
   result=result->EliminateRedondantLeaves(false);
   result=result->Simplify(*GetAffineEnvironment(),GetParameters());
//    cout<<"\n"<<PrintInequations(GetAffineEnvironment());
//    cout<<"\n après simplifs : \n"<<result->Print_str("\n");
//    cout<<"\n***************************************************************************\n";
   SetQuast(result);
   }
}

////////////////////////////////
////////////////////////////////
////////////////////////////////
////////////////////////////////
bool
ElementaryDependence::IsThereNonAffineConstraints(){
return ((GetNonAffineIndexesEquality()->size() != 0) || GetNonAffineValidStmt2()->size() != 0);
}

string prettry_print_parameter_of_maximum(vector<string>& v)
{

string result="";
if(!v.size())
   return"";

if(v.size() >= 2)
   {
   result+= v[v.size()-1];
   result+=" = (";
   for(int it=0; it<v.size()-2;it++)
      result+= v[it] + " , ";

   result += v[v.size()-2];
   result += ")";
   return result;
   }

return PrintVector(&v);
}
////////////////////////////////
string
ElementaryDependence::PrintProperties(){
map<string,Expression*>   var;
map<string,Expression*>   param;
vector<string> vector2;
vector<string> param2;

for(vector<string>::iterator it=GetStmt2Iteration()->begin(); it != GetStmt2Iteration()->end(); ++it){
   var[*it]=new Expression(*it+"_w");
   vector2.push_back((*it+"_w"));
   }
for(vector<string>::iterator it=GetStmt1Iteration()->begin(); it != GetStmt1Iteration()->end();++it){
   string str(*it,sizeof("current_")-1,it->size()-sizeof("current_")+1);
   param[*it]=new Expression(str);
   param2.push_back(str);
   }
if(GetParameterOfTheMaximum()->size() > 0){
   string result="";

   
   result+= prettry_print_parameter_of_maximum (*GetParameterOfTheMaximum());
   //PrintVector(GetParameterOfTheMaximum());

   result+="= Max_lex { "+PrintVector(&vector2)+" \\ ";
   result+=PrintInequations(&Substitute(&Substitute(GetPrecedence(),&var),&param));
   if(GetPrecedence()->size() >0)
      result+="  , ";

   result+=PrintInequations(&Substitute(&Substitute(GetNonAffineValidStmt2(),&var),&param)/*,&vector2,&param2*/);
   if(GetNonAffineValidStmt2()->size() > 0 && GetNonAffineIndexesEquality()->size() > 0)
      result+="  , ";

   result+=PrintInequations(&Substitute(&Substitute(GetNonAffineIndexesEquality(),&var),&param)/*,&vector2,&param2*/);

   result+=" } ";


   return result;
   }
return "";
}
////////////////////////////////
string
ElementaryDependence::Full_Tex_Print_str(){
map<string,Expression*>   var;
map<string,Expression*>   param;
vector<string> vector2;
vector<string> param2;

for(vector<string>::iterator it=GetStmt2Iteration()->begin(); it != GetStmt2Iteration()->end(); ++it){
   var[*it]=new Expression(*it+"_w");
   vector2.push_back((*it+"_w"));
   }
for(vector<string>::iterator it=GetStmt1Iteration()->begin(); it != GetStmt1Iteration()->end();++it){
   string str(*it,sizeof("current_")-1,it->size()-sizeof("current_")+1);
   param[*it]=new Expression(str);
   param2.push_back(str);
   }
if(GetParameterOfTheMaximum()->size() > 0){
   string result="";
   
   result+=PrintVector(GetParameterOfTheMaximum());
   result+="= Max_{lex} \\{ "+PrintVector(&vector2)+" \\ ";
   result+=TexPrintDNFTerm(&Substitute(&Substitute(GetPrecedence(),&var),&param));
   if(GetPrecedence()->size() >0)
      result+="   \\wedge  ";

   result+=TexFullPrintDNFCondition(&Substitute(&Substitute(GetNonAffineValidStmt2(),&var),&param),&vector2,&param2);
   if(GetNonAffineValidStmt2()->size() > 0 && GetNonAffineIndexesEquality()->size() > 0)
      result+="   \\wedge  ";

   result+=TexFullPrintDNFTerm(&Substitute(&Substitute(GetNonAffineIndexesEquality(),&var),&param),&vector2,&param2);

   result+=" \\} ";


   return result;
   }
return "";
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void
ElementaryDependence::CollectMaximumOfParameterValues(map<string,string>* map,vector<string> loops){
if(GetDeep() > 0 && GetQuast()!= NULL && GetParameterOfTheMaximum()->size() >0){
   if(GetDeep()>loops.size() && GetDeep()>GetParameterOfTheMaximum()->size()){
      cout<<"\n ElementaryDependence::CollectMaximumOfParameterValues .... FATAL ERROR [INAPPROPRIATE CASE] \n";
      throw(INAPPROPRIATE_CASE);
      }
   for(int i=0; i<GetDeep();++i)
      map->operator[](GetParameterOfTheMaximum()->operator[](i))=loops[i];
   }
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
};
/////////////////////////////////////////////////////////////////////////
////////////////////////////////   END   ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
//



