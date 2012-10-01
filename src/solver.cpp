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

// author: Christophe Alias
// author: Marouane Belaoucha

#include "solver.h"
#include "global.h"


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace fada{
vector<vector<int> >
Solver::CodeConstraints(void){

map<string, int>   var;
map<string, int>   param;
int i=0;
for(vector<string>::iterator it=this->GetVariables()->begin(); it != this->GetVariables()->end(); ++it)
   var[*it]=i++;
i=0;
for(vector<string>::iterator it=this->GetParameters()->begin(); it != this->GetParameters()->end(); ++it)
   param[*it]=i++;

vector<vector<int> > result;
for(vector<Inequation*>::iterator it=this->GetConstraints()->begin(); it != this->GetConstraints()->end(); ++it)
   result.push_back((*it)->ToVector(this->GetVariables(), &var, this->GetParameters(), &param));

return result;
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

PipMatrix*
Solver::ToPIPMatrix(void)
{
// cout<<"////////////////////////////////////\n";
// Print();
// cout<<"////////////////////////////////////\n";
vector<vector<int> > coded_constraints= CodeConstraints();
int columns = this->GetVariables()->size() + this->GetParameters()->size()+2;

PipMatrix * pip_constraints = pip_matrix_alloc(coded_constraints.size(),columns);

for(int i=0; i<coded_constraints.size(); i++)
   for(int j=0; j<coded_constraints[i].size(); j++)
      pip_constraints->p[i][j] = coded_constraints[i][j];

return pip_constraints;
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


Quast*
Solver::PIPMax(void){
if(IsPIPized()){
   PipMatrix*   __constraints=ToPIPMatrix();
   // cout<<"\n============================\n";
   // pip_matrix_print(stdout,__constraints);
   
   PipMatrix * __context = pip_matrix_alloc(0,this->GetParameters()->size()+2);
   //Empty context (set the parameters numbers)
   
   PipOptions * __options = pip_options_init();
   __options->Maximize = 1; //to get the max_lex
   //Setting up PipLib options
   
   PipQuast *result = pip_solve(__constraints,__context,-1,__options);
   //Launch PIP
//     cout<<"\n Launch PIP :============================\n";
//      pip_quast_print(stdout,result,0);


   pip_options_free(__options);
   pip_matrix_free(__constraints)  ;
   pip_matrix_free(__context) ;
   //Piplib freeing
   map<int, Expression*> mapping;
   return FromPipQuast(this->GetIndex(),this->GetDeep(),GetVariables(),GetParameters(),result, mapping);
   }
else{
   int nb_variables=GetVariables()->size();
   vector<Inequation*> new_constraints;
   PIPize(GetVariables(),&new_constraints);
//    RemoveNegativeVariablesPIPLimitation(GetVariables(),GetParameters(),&new_constraints);
   Solver s(GetIndex(),GetDeep(),*GetVariables(),*GetParameters(),&new_constraints,true);
//     cout<<"\n======================================================\n";
//     s.Print();
   Quast *result=s.PIPMax();
//     cout<<"\n NB variables .... ="<<nb_variables;
//    result->Print("\n");
//     cout<<"\n======================================================\n";
   result->DropAddedVariables(nb_variables);
   return result;
   }
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


Quast*
Solver::PIPMin(void){
PipMatrix*   __constraints=ToPIPMatrix();

PipMatrix * __context = pip_matrix_alloc(0,this->GetParameters()->size()+2);
  //Empty context (set the parameters numbers)

PipOptions * __options = pip_options_init();
  //Setting up PipLib options

PipQuast *result = pip_solve(__constraints,__context,-1,__options);
  //Launch PIP

pip_options_free(__options);
pip_matrix_free(__constraints)  ;
pip_matrix_free(__context) ;
  //Piplib freeing
map<int, Expression*> mapping;
return FromPipQuast(this->GetIndex(),GetDeep(),GetVariables(),GetParameters(),result, mapping);

}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

Solver::Solver()
{
Initialize( );
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

Solver::~Solver()
{
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void
Solver::Initialize(void)
{
vector<string> v;
SetParameters( v );
SetVariables( v );
SetConstraints( NULL );
SetIndex( -1 );
SetPIPized(false);
}

Solver::Solver(int  __stmt,int __deep, vector<string> __var, vector<string> __param, vector<Inequation*>* __constraints,bool __pipized){
Set(__stmt,__deep, __var, __param, __constraints,__pipized);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

Solver::Solver(int __index,int __deep, vector<string> var, vector<string> param, vector<Inequation*>* cons) 
{
Set(__index,__deep, var, param, cons,false);
}


////////////////////////////////////////////////////////////////////////////////////
/*Solver::Solver(int  __stmt,int __deep, vector<string> __var, vector<string> __param, vector<Inequation*>* __const,bool pipized){
Set(__index,__deep, var, param, cons,pipized);
}*/
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void
Solver::Set(int __index,int __deep, vector<string> var, vector<string> param, vector<Inequation*>* cons,bool v)
{
SetIndex( __index );
SetParameters(RemoveDoubles(& param) );
SetVariables( var );
SetConstraints( cons );
SetDeep(__deep);
SetPIPized(v);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////



Quast*   
Solver::Max(void)
{
/*cout<<"\nSolver::Max ..... parameters= "<<PrintVector(GetParameters());
cout<<"\nConstraints ..."<<Prin*/
// Print();
return PIPMax();
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
Quast*
Solver::Max(int __index,int __deep, vector<string> var, vector<string> param, vector<Inequation*>* cons)
{
Set(__index, __deep,var, param, cons );
return Max();
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

Quast*
Solver::Min(vector<string>* __var, vector<string>* __param, vector<Inequation*>* __cons){
Set(0,0,*__var,*__param,__cons);
return Min();
};

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

Quast*
Solver::Min(void){
return PIPMin();
exit(1);
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

string
Solver::Print_str(void)
{
string result;

ostringstream i;
i<<"\n STMT : "<<GetIndex();
result=i.str();
result+="\nVariables :";

for(vector<string>::iterator it=GetVariables( )->begin(); it != GetVariables( )->end(); ++it)
   result+="\n"+*it;

result+="\nParameters :";
for(vector<string>::iterator it=GetParameters( )->begin(); it != GetParameters( )->end(); ++it)
   result+="\n"+*it;
result+="\n\nConstraints :";
for(vector<Inequation*>::iterator it=GetConstraints( )->begin(); it != GetConstraints( )->end(); ++it)
   result+="\n"+(*it)->Print_str();

return result;
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void
Solver::Print(void){
cout<<Print_str( );
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void
Solver::PIPize(vector<string>* new_var, vector<Inequation*>* new_ineq){
for(vector<Inequation*>::iterator it=GetConstraints()->begin(); it!=GetConstraints()->end(); ++it)
   new_ineq->push_back((*it)->PIPize(new_var,new_ineq));
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void
Solver::RemoveNegativeVariablesPIPLimitation(vector<string>*__var,vector<string>*__param,vector<Inequation*>* __constraints){
vector<string> to_be_ducplicated=*__var;
for(vector<string>::iterator it=to_be_ducplicated.begin(); it != to_be_ducplicated.end(); it++){
   ostringstream positive,negative;
   positive<<"_pos_"<<*it;
   negative<<"_neg_"<<*it;
   Expression * sub=new Expression(new Expression(positive.str()),FADA_SUB,new Expression(negative.str()));
   __constraints->push_back(new Inequation(new Expression(*it), FADA_EQ, sub));
   __constraints->push_back(new Inequation(new Expression(positive.str()),FADA_GREATER_EQ,new Expression(0)));
   __constraints->push_back(new Inequation(new Expression(negative.str()),FADA_GREATER_EQ,new Expression(0)));
   __var->push_back(positive.str());
   __var->push_back(negative.str());
   }

// for(vector<string>::iterator it=__param->begin(); it != __param->end(); it++){
//    ostringstream positive,negative;
//    positive<<"_pos_"<<*it;
//    negative<<"_neg_"<<*it;
//    Expression * sub=new Expression(new Expression(positive.str()),FADA_SUB,new Expression(negative.str()));
//    __constraints->push_back(new Inequation(new Expression(*it), FADA_EQ, sub));
//    __constraints->push_back(new Inequation(new Expression(positive.str()),FADA_GREATER_EQ,new Expression(0)));
//    __constraints->push_back(new Inequation(new Expression(negative.str()),FADA_GREATER_EQ,new Expression(0)));
// 
//    __var->push_back(positive.str());
//    __var->push_back(negative.str());
//    }
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
}


