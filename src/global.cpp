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

#include <global.h>
#include <constants.h>
#include <latex.h>
#include <solver.h>

#include <cstring>      // gcc 4.3.x   restriction
#include <algorithm>      // gcc 4.3.x


namespace fada{
AGC    ____myGC;
set<string>   all_types;

bool
IsType(string identifier){

bool res=(all_types.find(identifier)!=all_types.end());
return res;
}


void PrintAllTypes(){
cout<<"\n\n";
for(set<string>::iterator it=all_types.begin(); it != all_types.end(); ++it)
   cout<<"\n"<<*it;
cout<<"\n\n";
}

bool   AddType(string identifier){

if(identifier != "")
   all_types.insert(identifier);
return true;
}

void   InitTypes(){
AddType("__gnuc_va_list");
AddType("__builtin_va_list");
AddType("int32_t");
}


string
PrintOperation(int  stmt_id ,vector<Expression*>* __iteration,GraphPrinterOptions op){
ostringstream sout;
sout<<"<"<<op.internal_node_prefix<<stmt_id;
if(__iteration->size() == 0){
   sout<<">";
   return sout.str();
   }
sout<<": (";
for(vector<Expression*>::iterator it=__iteration->begin(); it != __iteration->end(); ++it){
   if(it == __iteration->begin())
      sout<<(*it)->Print_str();
   else
      sout<<","<<(*it)->Print_str();
   }
sout<<") >";
return sout.str();   

}


string
PrintOperation(int  stmt_id ,vector<Expression*>* __iteration){
ostringstream sout;
sout<<"<S_"<<stmt_id;
if(__iteration->size() == 0){
   sout<<">";
   return sout.str();
   }
sout<<": (";
for(vector<Expression*>::iterator it=__iteration->begin(); it != __iteration->end(); ++it){
   if(it == __iteration->begin())
      sout<<(*it)->Print_str();
   else
      sout<<","<<(*it)->Print_str();
   }
sout<<") >";
return sout.str();   
}

string
PrintTexOperation(int  stmt_id ,vector<Expression*>* __iteration){
ostringstream sout;
sout<<stmt_id;

if(__iteration->size() == 0)
   return TexIndex("S",sout.str());


string instance="(";

for(vector<Expression*>::iterator it=__iteration->begin(); it != __iteration->end(); ++it){
   if(it == __iteration->begin())
      instance+=(*it)->Print_str();
   else
      instance+=" , "+(*it)->Print_str();
   }
instance+=")";

return " \\langle "+TexExponent(TexIndex("S",sout.str())+"",instance)+" \\rangle ";
}

void
WriteToFile(string file_name, string contnent){
FILE* f;
f=fopen(file_name.c_str(), "w");
fprintf(f,"%s",contnent.c_str());
fclose(f);
}
/////////////////////////////////////////////////////////////////
/*string
PrintTexInequations(vector<Inequation*>* vecteur){
string result="";
for(vector<vector<Inequation*> >::iterator itt=vecteur->begin(); itt != vecteur->end(); ++itt){
      result+="    "+(*itt)->Print_Tex_str();
   }
return result;
}*/
/////////////////////////////////////////////////////////////////
string
PrintInequations(vector<vector<Inequation*> >* vecteur){
string result="";
for(vector<vector<Inequation*> >::iterator itt=vecteur->begin(); itt != vecteur->end(); ++itt){
   result+="\n>>";
   for(vector<Inequation*>:: iterator it=(*itt).begin(); it != (*itt).end(); ++it)
      result+="    "+(*it)->Print_str();
   }
return result;
}
/////////////////////////////////////////////////////////////////
string
PrintInequations(string prefix, vector<Inequation*>* vecteur){
string result=prefix;
for(vector<Inequation*>:: iterator it=vecteur->begin(); it !=vecteur->end(); ++it)
      result+=prefix+(*it)->Print_str();
   
return result;

}
/////////////////////////////////////////////////////////////////
string
PrintInequations(vector<Inequation*>* vecteur){
string result="";
for(vector<Inequation*>::iterator it=vecteur->begin(); it != vecteur->end(); ++it)
   result+="    "+(*it)->Print_str();
return result;
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

string   PrintVector(vector<string>* v)
{
string result="( ";
for(vector<string>::iterator it=v->begin(); it != v->end(); ++it)
   {
   result += *it;
   vector<string>::iterator it2=it;
   it2++;
   if(it2 != v->end())
      result +=", ";
   }
result+=" )";
return result;
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// string getStringForArgument(char * s, int argc, char *argv[])
// {
// for (int i = 1 ; i<argc; i++)
//    {
//         if (strcmp(argv[i],s) == 0)
//            { 
//            if (i+1<argc)
//                   return argv[i+1];
//            else
//                   fprintf(stderr,"Argument value missing for %s\n",s);
//            return("");
//            }
//     }
//     return("");
// }

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void DefaultOptions(bool *rank,bool *label,bool *assign,bool *rela,bool *cond,bool *html,bool *code,bool *rename,bool *bottom,bool* alphas,bool*log,bool*fuzzy,bool*sa,bool*dg,bool*flat,bool*tex,bool* ppts)
{
*rank=false;
*label=false;
*assign=false;
*rela=true;
*cond=false;
*html=false;
*code=false;
*rename=true;
*bottom=false;
*alphas=false;
*log=false;
*fuzzy=false;
*sa=false;
*dg=false;
*flat=true;
*tex=false;
*ppts=false;
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

vector<Condition*> 
lex_preced(int rank1, vector<string>* counters1, int rank2, vector<string>* counters2, vector<string>* param_counters2)
{
int commun=0;
vector<string>::iterator it1=counters1->begin();
vector<string>::iterator it2=counters2->begin();
vector<Condition*>   result;
while(it1 != counters1->end() && it2 != counters2->end() && *it1==*it2 )
   {
   ++commun;
   ++it2;
   ++it1;
   }

for(int i=0; i<commun; ++i)
   {
   Inequation*   preced_i=
         new Inequation(
               new Expression(counters1->operator[](i)),
               FADA_LESS,
               new Expression(param_counters2->operator[](i)));

   Condition*   preced_deep_i=new Condition(preced_i);
   for(int j=0; j<i ;++j)
      {
      Inequation*   equal_j=new Inequation(
                        new Expression(counters1->operator[](j)),
                        FADA_EQ, 
                        new Expression(param_counters2->operator[](j)));

      preced_deep_i = new Condition( preced_deep_i,FADA_AND, new Condition(equal_j));
      };
   result.push_back(preced_deep_i);
   }

if(rank1 < rank2)
   {
   Condition   *preced_tex=new Condition(new Inequation (true));
   for(int i=0; i<commun ;++i)
      {
      Inequation*   equal_j=new Inequation(
                           new Expression(counters1->operator[](i)),
                           FADA_EQ, 
                           new Expression(param_counters2->operator[](i)));

      preced_tex = new Condition( preced_tex,FADA_AND, new Condition(equal_j));
      };
   result.push_back(preced_tex);
   }
return result;
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

bool
IsIn(string str, vector<string>* __vector)
{
return (find( __vector->begin(), __vector->end(), str) != __vector->end());
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

Expression*
VectorToExpression(vector<int>* __coefs, vector<string>* __var, vector<string>* __param){
int var_org=0;
int var_end=__var->size()-1;
int param_org=var_end+1;
int param_end=__coefs->size()-2;
int constant=param_end+1;

Expression*   result=new Expression(__coefs->operator[](constant));

for(int i=var_org; i <= var_end;++i)
   if(__coefs->operator[](i) != 0)
      if(__coefs->operator[](i) == 1)
         result=new Expression(result, FADA_ADD, new Expression(__var->operator [](i)));
      else
         result=new Expression(result, FADA_ADD, new Expression(
                              new Expression(__coefs->operator[](i)), 
                              FADA_MUL, 
                              new Expression(__var->operator[](i))
                                    )
                     );

for(int i=param_org; i<= param_end; ++i)
   if(__coefs->operator[](i) != 0)
      if(__coefs->operator[](i) == 1)
         result=new Expression(result, FADA_ADD, new Expression(__param->operator [](i-param_org)));
      else
         result=new Expression(result, FADA_ADD, new Expression(new Expression(__coefs->operator[](i)), FADA_MUL, new Expression(__param->operator[](i-param_org))));
return result;
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

Expression*
PipVectorToExpression(PipVector* __pipv, vector<string>* __param, map<int, Expression*> __new_param){

int nb_parameters = __param->size();

if(__pipv->nb_elements != nb_parameters+__new_param.size()+1){
      cerr << "PipVectorToExpression:: Fatal error ... bad vector size" << endl;
      throw(INAPPROPRIATE_CASE);
      exit(EXIT_FAILURE);
    }
// cout<<"\n..................................\n";
// pip_vector_print(stdout,__pipv);

Expression *result_pos;
Expression *result_neg;
if(__pipv->the_vector[nb_parameters+__new_param.size()] >= 0){    //constant
   result_pos = new Expression(__pipv->the_vector[nb_parameters+__new_param.size()]);
   result_neg = new Expression(0);
   }
else{
   result_pos = new Expression(0);
   result_neg = new Expression(-1*__pipv->the_vector[nb_parameters+__new_param.size()]);
   }


for(int i = 0; i<nb_parameters; i++){    // handle old parameters
   int coef = __pipv->the_vector[i];
//         cout<<"\n i = "<<i<<"    coef = "<<coef<<"     nom param = "<<__param->operator[](i);
   if(coef != 0){
      Expression * param = new Expression(__param->operator[](i));
      switch(coef){
         case 1 :
            result_pos = new Expression(result_pos,FADA_ADD, param);
            break;
         case -1:
            result_neg = new Expression(result_neg,FADA_ADD, param);
            break;
         case 0 : break;
         default :
            {
            if(coef > 0){
               Expression *__coef = new Expression(coef);
               Expression *__term = new Expression(__coef,FADA_MUL,param);
               result_pos = new Expression(result_pos,FADA_ADD,__term);
               }
            else{
               Expression *__coef = new Expression(-1*coef);
               Expression *__term = new Expression(__coef,FADA_MUL,param);
               result_neg = new Expression(result_pos,FADA_ADD,__term);
               }
            break;
            }
         }
      }
   }
// cout<<"\n\nhhhhhhhhhhhhhhhhh\n";

   //Newparms handling
for(map<int,Expression*>::const_iterator it=__new_param.begin(); it != __new_param.end(); ++it)    //For each newparm
   {
   Expression *param = it->second;
   int coef = __pipv->the_vector[it->first];
   switch(coef){
      case 0 : break;
      case 1 : result_pos = new Expression(result_pos,FADA_ADD,param); break;
      case -1: result_neg = new Expression(result_neg,FADA_ADD,param); break;
      default:
         if(coef > 0){
            Expression *__coef = new Expression(coef);
            Expression *__term = new Expression(__coef,FADA_MUL,param);
            result_pos = new Expression(result_pos,FADA_ADD,__term);
            }
         else{
            Expression *__coef = new Expression(-1*coef);
            Expression *__term = new Expression(__coef,FADA_MUL,param);
            result_neg = new Expression(result_pos,FADA_ADD,__term);
            }
         break;
      }
   }
result_pos=result_pos->Simplify();
// result_pos->Print();
Expression* final_res=new Expression(result_pos->Simplify(),FADA_SUB, result_neg->Simplify());
//  cout<<"\n\nhhhhhhhhhhhhhhhhh\n";
//  final_res->Print();
return final_res;
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

FADA_Index*
PipListToIndex(PipList* __pipl, int __nb_variables, vector<string>* __params, map<int, Expression*> __new_param){

vector<Expression*> * result = new vector<Expression*> (__nb_variables);
PipList *current = __pipl;
for(int i=0; i<__nb_variables; i++)
   {
   if(current == NULL){
      cerr << "PipListToIndex : incorrect size" << endl;
      throw(INAPPROPRIATE_CASE);
      exit(1);
      }
   result->operator[](i) = PipVectorToExpression(current->vector,__params,__new_param)->Simplify();
   current = current->next;
   }
return new FADA_Index(result);
}

/////////////////////////////////////////////////////////////////
vector<Inequation*>
EliminateTrueAndDoubles(vector<Inequation*>* liste){
vector<Inequation*> res;
for(vector<Inequation*>::iterator it=liste->begin(); it != liste->end(); ++it){
   if(!((*it)->IsValue() && (*it)->GetValue())){
      string __ineq=(*it)->Print_str();
      bool found=false;
      Inequation* s;
      
       vector<Inequation*>::iterator it2=it;++it2;
      Inequation* in;

      for(vector<Inequation*>::iterator it2=res.begin(); it2 != res.end(); ++it2){
         if(__ineq == (*it2)->Print_str()){
            found=true;   
            break;
            }
         }
      if(!found)
         res.push_back(*it);
      }
   }
return res;
}
/////////////////////////////////////////////////////////////////

vector<Inequation*>
EliminateDoubles(vector<Inequation*>* liste){
vector<Inequation*> res;
for(vector<Inequation*>::iterator it=liste->begin(); it != liste->end(); ++it){
   string __ineq=(*it)->Print_str();
   bool found=false;
   
   for(vector<Inequation*>::iterator it2=res.begin(); it2 != res.end(); ++it2){
      if(__ineq == (*it2)->Print_str()){
         found=true;   
         break;
         }
      }
   if(!found)
      res.push_back(*it);
   }
return res;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

vector<int> 
GenerateTruePipVector(int size){
vector<int>   result(size+2);
for(int i=0; i<size+2;i++)
   result[i]=0;

return result;      // true <=> (0 == 0)
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

vector<int>
GenerateFalsePipVector(int size){
vector<int>   result(size+2);

for(int i=0;i<size+2;i++)
   result[i]=0;

result[size+1]=1;   //constant

return result;      // false <=> (1 == 0)
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

Quast*
FromPipQuast(int __index, int __deep,vector <string>* __counters, vector<string>* __params,PipQuast* __quast, map <int, Expression*> __new_param){

if(__quast->newparm != NULL)      //Add current newparms
   {
   cout << "FromPipQuast ...  WARNING: new parameters" << endl;
   PipNewparm *current_newparm = __quast->newparm;

   while(current_newparm != NULL){
      int new_rank = current_newparm->rank;
      Expression *num =  PipVectorToExpression(current_newparm->vector,__params, __new_param);
      Expression *deno = new Expression(current_newparm->deno);
      Expression *newparm_def = new Expression(num,FADA_DIV,deno);
      __new_param[new_rank] = newparm_def;
      current_newparm = current_newparm->next;
      }
       }

if(__quast->condition == NULL)         //leaf or empty (_|_)
   {
   if(__quast->list == NULL)       //empty
      return new Quast();

         else   //leaf
      return new Quast(__index,__deep,__counters,PipListToIndex(__quast->list,__counters->size(),__params, __new_param));
       }
else      //conditional node
   {
   Expression * ineq=PipVectorToExpression(__quast->condition,__params,__new_param);
   Condition * __cond= 
         new Condition(
            new Inequation(
                  ineq->GetLeftChild(),
                  FADA_GREATER_EQ,
                  ineq->GetRightChild())
                  );
   return new Quast(__cond,
            FromPipQuast(__index,__deep, __counters, __params, __quast->next_then, __new_param),
            FromPipQuast(__index,__deep, __counters, __params, __quast->next_else, __new_param)
            );
   }
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void
CollectReferences(Expression* __expr, vector<Read_Reference*>* _ref){
if(!__expr->IsLeaf()){
   CollectReferences(__expr->GetLeftChild(),_ref);
   CollectReferences(__expr->GetRightChild(),_ref);
   return;
   }

if(__expr->IsVariable()){
   vector<Expression*>   v;
   FADA_Index*   empty=new FADA_Index(&v);
   _ref->push_back(new Read_Reference(__expr->GetVariableName( ), empty));
   return;
   }

if(__expr->IsArray( ))
   {
   FADA_Index*   __index=new FADA_Index(__expr->GetIndex( ));
   _ref->push_back(new Read_Reference( __expr->GetArrayName( ), __index));

   for(vector<Expression*>::iterator it= __expr->GetIndex( )->begin(); it !=  __expr->GetIndex( )->end(); ++it)
      CollectReferences(  (*it), _ref );
   return;
   }

if(__expr->IsFunction( ))
   {
   for(vector<Expression*>::iterator it= __expr->GetArguments( )->begin(); it !=  __expr->GetArguments( )->end(); ++it)
      CollectReferences( *it, _ref );
   return;
   }
if(__expr->IsValue())
   return;

cerr<<"\n\nCollectReferences(expression), fata error : invalid leaf information\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}

//////////////////////////////////////
//////////////////////////////////////

void
CollectReferences(FADA_Index* __i, vector<Read_Reference*>* _ref){
for(int i=0; i<__i->GetSize( ); ++i)
   CollectReferences( __i->GetItem( i ), _ref );
return;
}

//////////////////////////////////////
//////////////////////////////////////

void
CollectReferences(Condition* __c, vector<Read_Reference*>* _ref){
if(__c->IsLeaf()){
   CollectReferences(__c->GetInequation(), _ref);
   return;
   }
if(__c->IsAND() || __c->IsOR()){
   CollectReferences(__c->GetLeftChild(),_ref);
   CollectReferences(__c->GetRightChild(),_ref);
   return;
   }
if(__c->IsNot()){
   CollectReferences(__c->GetLeftChild(),_ref);
   }
cerr<<"\nCollectReferences(condition), inappropriate value \n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}
//////////////////////////////////////
//////////////////////////////////////

void
CollectReferences(Inequation* __i, vector<Read_Reference*>* _ref){
if(__i->IsValue())
   return;
CollectReferences(__i->GetLHS(),_ref);
CollectReferences(__i->GetRHS(),_ref);
}

//////////////////////////////////////
//////////////////////////////////////

void
Connect(Statement* __stmt, References* __ref){
__stmt->SetID(__ref->GetStmtID());
__stmt->SetReferences(__ref);
__ref->SetOrigin(__stmt);
}


//////////////////////////////////////
//////////////////////////////////////


vector<ElementaryDependence*>
ComputeDependenciesConfigs(int __begin,int __end, int __read_stmt,string read_array, FADA_Index* __i,int __common, vector<References*>* __references){
bool    reversed= __begin > __end;
int   stride=reversed ? -1:1;
int real_end= reversed ? __end:__end;
// int real_end= __end;

vector<ElementaryDependence*>   result;
for(int i=__begin; i != real_end; i+=stride){
   References* ref= __references->operator[](i);
   if(ref->IsWrittenIn(read_array)){
      ElementaryDependence* config= new ElementaryDependence(__read_stmt,i, __common,__i);
      result.push_back(config);
      }
   }

return result;
}

//////////////////////////////////////
//////////////////////////////////////

string
NewCounterName(string counter, vector<string>* to_be_avoided){
int i=0;
while(true){
   ostringstream   result;
   result<<counter<<"_"<<i;
   ++i;
   if(find(to_be_avoided->begin(), to_be_avoided->end(), result.str()) == to_be_avoided->end())
      return result.str();
   }
}

//////////////////////////////////////
//////////////////////////////////////


vector<ElementaryDependence*>
EliminateValues2In(vector<ElementaryDependence*>* config, map<int,int>* _to_eliminate){
vector<ElementaryDependence*> result;

for(vector<ElementaryDependence*>::iterator it=config->begin(); it != config->end(); it++){
   int found=false;
   for(map<int,int>::iterator ite=_to_eliminate->begin(); ite != _to_eliminate->end(); ++ite){
      int val=(*it)->GetStmt2();
      if(val>= ite->first && val <= ite->second){
         found=true;
         break;
         }
      }
   if(!found)
      result.push_back(*it);
   }
return result;
}

//////////////////////////////////////
//////////////////////////////////////

vector<ElementaryDependence*>
EliminateDoubles(vector<ElementaryDependence*>* configs){
vector<ElementaryDependence*>   result;

for(vector<ElementaryDependence*>::iterator it=configs->begin(); it != configs->end(); ++it){
   bool found =false;
   for(vector<ElementaryDependence*>::iterator itr=result.begin(); itr!= result.end(); ++itr){
      if((*it)->Equal(*itr)){
         found=true;
         break;
         }
      }
   if(!found)
      result.push_back(*it);
   }
return result;
}
//////////////////////////////////////
//////////////////////////////////////

vector<string>
ParametrizeReadIteration(vector<string>* loop_counters){
vector<string>   result;
for(vector<string>::iterator it=loop_counters->begin(); it != loop_counters->end(); ++it)
   result.push_back(FADA_Prefix+(*it));

return result;
}

//////////////////////////////////////
//////////////////////////////////////

vector<Inequation*>
Precedence(vector<string>* __write, vector<string>* __read,int __common_loops,int __common){
vector<Inequation*>   result;
if(__common_loops==0)
   return result;
for(int i=0; i<__common;i++)
   result.push_back(new Inequation(new Expression(__write->operator[](i)),FADA_EQ,new Expression( __read->operator[](i))));


if(__common < __write->size() && __common < __read->size() && __common< __common_loops)
      result.push_back(
         new Inequation
            (
            new Expression(__write->operator[](__common)),
            FADA_LESS,
            new Expression(__read->operator[](__common))
            )
      );

return result;
}










/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

bool
InvariableIn(vector<string>* __variable, vector<string>* __invariable){
if(__variable->size() > __invariable->size())
   return false;
for(vector<string>::iterator it= __variable->begin(); it != __variable->end(); ++it)
   if(find(__invariable->begin(), __invariable->end(),*it) == __invariable->end())
      return false;
return true;
}
//////////////////////////////////////
//////////////////////////////////////
Condition*
DNFTerms_to_Condition(vector<vector<Inequation*> >* inequations){
Condition* result=NULL;
if(inequations->size() == 0)   
   return new Condition(new Inequation(true));
for(vector<vector<Inequation*> >::iterator it=inequations->begin(); it != inequations->end();++it){
   if(result==NULL)
      result=ToDNFTerm(&(*it));
   else
      result=new Condition(result,FADA_OR,ToDNFTerm(&(*it)));
   }
return result;
}


Condition*
ToDNFTerm(vector<Inequation*>* __affine_constraints){
Condition* result=NULL;
for(vector<Inequation*>::iterator it=__affine_constraints->begin(); it != __affine_constraints->end(); ++it){
   if(!(*it)->IsValue() || ((*it)->IsValue() && !(*it)->GetValue()))
      if(result==NULL)
         result=new Condition(*it);
      else
         result=new Condition(result, FADA_AND, new Condition(*it));
   }

if(result==NULL)
   return new Condition(new Inequation(true));
return result;
}
//////////////////////////////////////
//////////////////////////////////////


vector<vector<Inequation*> > 
AND_DNF(vector<vector<Inequation*> > * cond1, vector<vector<Inequation*> > * cond2){
vector<vector<Inequation*> >  result;
if(cond1->size() == 0)
   return *cond2;
if(cond2->size() == 0)
   return *cond1;

for(vector<vector<Inequation*> > ::iterator it=cond1->begin(); it!= cond1->end(); ++it)
   for(vector<vector<Inequation*> > ::iterator it2=cond2->begin(); it2 != cond2->end(); ++it2){
      vector<Inequation*> term=*it;
      for(vector<Inequation*>::iterator iti=(*it2).begin(); iti != (*it2).end(); ++iti){
         term.push_back(*iti);
         }
      result.push_back(term);
      }
return result;
}
//////////////////////////////////////
//////////////////////////////////////

vector<vector<Inequation*> >
RENAME(vector<vector<Inequation*> >* constraints, map<string,Expression*>* __mapping){
vector<vector<Inequation*> > result;

for(vector<vector<Inequation*> >::iterator itt=constraints->begin(); itt != constraints->end(); ++itt){
   vector<Inequation*>  term;
   for(vector<Inequation*>::iterator iti=(*itt).begin(); iti != (*itt).end(); ++iti){
      term.push_back((*iti)->Substitute(__mapping));
      }
   result.push_back(term);
   }
return result;
}

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////

//////////////////////////////////////

//////////////////////////////////////
vector<vector<Inequation*> >
Merge(vector<vector<Inequation*> >* __rela1,vector<vector<Inequation*> >* __rela2){
vector<vector<Inequation*> > result=*__rela1;
for(vector<vector<Inequation*> >::iterator it=__rela2->begin(); it != __rela2->end();++it)
   result.push_back(*it);
return result;
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
Quast*         Max(Quast* _q1, Quast* _q2, vector<References*>* __ref, void* _textual_preced, int (*fct)(void*,int,int), vector<vector<Inequation*> > _context, vector<string>* _param){
if(_q1->IsLeaf() && _q1->IsEmpty())
   return _q2;
if(_q2->IsLeaf() && _q2->IsEmpty())
   return _q1;

if(_q1->IsLeaf() && _q2->IsLeaf()){
   vector<string>* c1,*c2;
   c1=(__ref->operator[](_q1->GetAssignment()))->GetCounters();
   c2=(__ref->operator[](_q2->GetAssignment()))->GetCounters();

   vector<string> common_counters=Common(_q1->GetCounters(),_q2->GetCounters());

   bool q1_before_q2,q2_before_q1;
   Condition*   lex_eq=NULL;
   Condition* preced=   SmartPrecedenceComputing(_textual_preced,
               fct,
               _q1->GetAssignment(), 
               _q2->GetAssignment(),
               _q1->GetVertex(),
                _q2->GetVertex(), 
               &common_counters,
               &q1_before_q2,
               &q2_before_q1
                 );
   if(preced != NULL){
      lex_eq=preced->GetRightChild();      // S1 =lex S2
      preced=preced->GetLeftChild();      // S1 <lex[strict] S2
      }
   if(preced == NULL && lex_eq == NULL){
      if(q1_before_q2)
         return _q2;
      if(q2_before_q1)
         return _q1;
      cerr<<"\nQuast*   Max(global.h) ... (Warning) have to consider this particular case [1]\n";
      cout<<"\n [DEBUG] ........ \n";
      cout<<"\nQuast1: ";_q1->Print("\n");
      cout<<"\nQuast2: ";_q2->Print("\n");
      cout<<"\n [DEBUG] ........ DONE\n";
      throw(UNHANDLED_CASE);
      exit(EXIT_FAILURE);
      }
   else{
      if(preced == NULL){   //lex_eq != NULL
         cerr<<"\nQuast*   Max(global.h) ... ('FATAL' Warning) that part is (in normal case) UNREACHABLE \n";
         throw(INAPPROPRIATE_CASE);
         exit(EXIT_FAILURE);
         
         

         if(q1_before_q2)
            return new Quast( lex_eq, _q2, _q1);
         if(q2_before_q1)
            return new Quast( lex_eq, _q1, _q2);

         }
      else{      //preced != NULL  and   lex_eq != NULL

         Quast*   quast_resultat;

         if(q1_before_q2)         // S1 <text S2   quast= IF  ( S1 <lex S2  OR S1 =lex S2 )  then S2
                        //         ELSE S1
            {               
            quast_resultat= new Quast( new Condition(preced, FADA_OR,lex_eq), _q2, _q1);
            quast_resultat=quast_resultat->Uncompress();
            return quast_resultat->Simplify(_context,_param)->Compress();
            }

                     
         if(q2_before_q1)         // S1 >tex S2    quast= IF( S1 <lex S2)   then S2
            {            //         ELSE S1
            quast_resultat= new Quast( preced, _q2, _q1);
            
            quast_resultat=quast_resultat->Uncompress();
            return quast_resultat->Simplify(_context,_param)->Compress();

            }

               // not(S1 <tex S2) and not(S1 >tex S2)      typical case of  if-then S1 else S2;
         
            //quast =    IF (S1 <lex S2)  S2
            //      ELSE IF S1 =lex S2 (impossible case)
            //           ELSE S1

            //to  simplify, we assume that "impossible case" is remplaced by S1. we got in this case :
            
               // IF (S1 <lex S2) S2
               // ELSE S1

         quast_resultat= new Quast( preced, _q2, _q1);
         quast_resultat=quast_resultat->Uncompress();
         return quast_resultat->Simplify(_context,_param)->Compress();

         }
      }
   //   if( _q1 <lex[strict] _q2)      _q2;
   //   else
   //      if( _q1 ==[lex] _q2)
   //         if( s1 <tex s2 )   _q2;
   //         if( s1 >tex s2)      _q1;


   return new Quast(preced,_q2->Clone(), _q1->Clone());
   }
if(_q1->IsLeaf()){
   return new Quast(_q2->GetCondition(), 
         Max(_q1,_q2->GetThenPart(),__ref, _textual_preced,fct, /*env_then*/_context, _param),
         Max(_q1,_q2->GetElsePart(),__ref,_textual_preced,fct,/*env_else*/_context,_param)
         );
   }

   return new Quast(_q1->GetCondition(), 
         Max(_q1->GetThenPart(),_q2,__ref,_textual_preced,fct, _context,_param),
         Max(_q1->GetElsePart(),_q2,__ref,_textual_preced,fct,_context,_param));

cerr<<"\nQuast*   Max, ... fatal error (inappropriate case) ....\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
Quast*
Max(Quast* _q1, Quast* _q2, vector<References*>* __ref, vector<vector<bool> >* _textual_preced, vector<vector<Inequation*> > _context, vector<string>* _param){
if(_q1->IsLeaf() && _q1->IsEmpty())
   return _q2;
if(_q2->IsLeaf() && _q2->IsEmpty())
   return _q1;

if(_q1->IsLeaf() && _q2->IsLeaf()){
   vector<string>* c1,*c2;
   c1=(__ref->operator[](_q1->GetAssignment()))->GetCounters();
   c2=(__ref->operator[](_q2->GetAssignment()))->GetCounters();

   vector<string> common_counters=Common(_q1->GetCounters(),_q2->GetCounters());

   bool q1_before_q2,q2_before_q1;
   Condition*   lex_eq=NULL;
   Condition* preced=   SmartPrecedenceComputing(_textual_preced,
               _q1->GetAssignment(), 
               _q2->GetAssignment(),
               _q1->GetVertex(),
                _q2->GetVertex(), 
               &common_counters,
               &q1_before_q2,
               &q2_before_q1
                 );
   if(preced != NULL){
      lex_eq=preced->GetRightChild();      // S1 =lex S2
      preced=preced->GetLeftChild();      // S1 <lex[strict] S2
      }
   if(preced == NULL && lex_eq == NULL){
      if(q1_before_q2)
         return _q2;
      if(q2_before_q1)
         return _q1;

         //FIXME: this line is added to bypass Graphite+FADA BUG while computing sequential precedence.
      if(_q1->GetAssignment() < _q2->GetAssignment())
         return _q2;
      else
         return _q1;

      cerr<<"\nQuast*   Max(global.h) ... (Warning) have to consider this particular case [1]\n";
      cout<<"\n [DEBUG] ........ \n";
      cout<<"\nQuast1: ";_q1->Print("\n");
      cout<<"\nQuast2: ";_q2->Print("\n");
      cout<<"\n Sequential Precedence ["<<_q1->GetAssignment()<<"]["<<_q2->GetAssignment()<<"] = " << _textual_preced->operator[](_q1->GetAssignment())[_q2->GetAssignment()] ? "true": "false";
      cout<<"\n [DEBUG] ........ DONE\n";

      throw(UNHANDLED_CASE);
      exit(EXIT_FAILURE);
      }
   else{
      if(preced == NULL){   //lex_eq != NULL
         cerr<<"\nQuast*   Max(global.h) ... ('FATAL' Warning) that part is (in normal case) UNREACHABLE \n";
         throw(INAPPROPRIATE_CASE);
         exit(EXIT_FAILURE);

         if(q1_before_q2)
            return new Quast( lex_eq, _q2, _q1);
         if(q2_before_q1)
            return new Quast( lex_eq, _q1, _q2);

         }
      else{      //preced != NULL  and   lex_eq != NULL

         Quast*   quast_resultat;

         if(q1_before_q2)         // S1 <text S2   quast= IF  ( S1 <lex S2  OR S1 =lex S2 )  then S2
                        //         ELSE S1
            {               
            quast_resultat= new Quast( new Condition(preced, FADA_OR,lex_eq), _q2, _q1);
            quast_resultat=quast_resultat->Uncompress();
            return quast_resultat->Simplify(_context,_param)->Compress();
            }

                     
         if(q2_before_q1)         // S1 >tex S2    quast= IF( S1 <lex S2)   then S2
            {            //         ELSE S1
            quast_resultat= new Quast( preced, _q2, _q1);
            
            quast_resultat=quast_resultat->Uncompress();
            return quast_resultat->Simplify(_context,_param)->Compress();

            }

               // not(S1 <tex S2) and not(S1 >tex S2)      typical case of  if-then S1 else S2;
         
            //quast =    IF (S1 <lex S2)  S2
            //      ELSE IF S1 =lex S2 (impossible case)
            //           ELSE S1

            //to  simplify, we assume that "impossible case" is remplaced by S1. we got in this case :
            
               // IF (S1 <lex S2) S2
               // ELSE S1

         quast_resultat= new Quast( preced, _q2, _q1);
         quast_resultat=quast_resultat->Uncompress();
         return quast_resultat->Simplify(_context,_param)->Compress();

         }
      }
   //   if( _q1 <lex[strict] _q2)      _q2;
   //   else
   //      if( _q1 ==[lex] _q2)
   //         if( s1 <tex s2 )   _q2;
   //         if( s1 >tex s2)      _q1;


   return new Quast(preced,_q2->Clone(), _q1->Clone());
   }
if(_q1->IsLeaf()){
/*   vector<vector<Inequation*> >   env_then,env_else;
   env_then=_context;
    env_else=_context;

   vector<vector<Inequation*> >   term_ineq1;
   term_ineq1.push_back(_q2->GetCondition()->GetTermInequations());

   env_then=AND_DNF(&_context,&term_ineq1);*/
//    for(vector<Inequation*>::iterator it=term_ineq1.begin(); it != term_ineq1.end(); ++it)
//       env_then.push_back(*it);

   return new Quast(_q2->GetCondition(), 
         Max(_q1,_q2->GetThenPart(),__ref, _textual_preced, /*env_then*/_context, _param),
         Max(_q1,_q2->GetElsePart(),__ref,_textual_preced,/*env_else*/_context,_param)
         );
   }
// if(_q2->IsLeaf())

//    vector<Inequation*>   env_then,env_else;
//    env_then=_context;
//     env_else=_context;
// 
//    vector<Inequation*>   term_ineq1= _q1->GetCondition()->GetTermInequations();
// 
//    for(vector<Inequation*>::iterator it=term_ineq1.begin(); it != term_ineq1.end(); ++it)
//       env_then.push_back(*it);

   return new Quast(_q1->GetCondition(), 
         Max(_q1->GetThenPart(),_q2,__ref,_textual_preced, _context,_param),
         Max(_q1->GetElsePart(),_q2,__ref,_textual_preced,_context,_param));

cerr<<"\nQuast*   Max, ... fatal error (inappropriate case) ....\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}
//////////////////////////////////////
///////////////////////////////////////////////////////////////////////
   Condition*      SmartPrecedenceComputing(void* textual_preced, 
                     int (*fct)(void*,int,int),
                     int s1, int s2, 
                     FADA_Index* index1, FADA_Index* index2,
                     vector<string>* common_counters, bool*s1_before_s2, bool*s2_before_s1){
if(common_counters->size() > index1->GetSize() || common_counters->size() > index2->GetSize()){
   cerr<<"Condition*   Precedence (indexes) ... FATAL ERROR (inappropriate case)\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }
Condition* result=NULL;
Condition* lex_eq=NULL;
lex_eq=NULL;
for(int i=0; i< common_counters->size(); ++i){
   Condition *nest_pred = new Condition(new Inequation(index1->GetItem(i),FADA_LESS,index2->GetItem(i)));
   Condition *equal = new Condition(new Inequation(index1->GetItem(i),FADA_EQ,index2->GetItem(i)));
   for(int j=1;j<i;++j){
      nest_pred=new Condition(nest_pred, FADA_AND, 
               new Condition( 
                  new Inequation(index1->GetItem(j),FADA_EQ,index2->GetItem(j))));
      }
   if(result == NULL)
      result=nest_pred;
   else
      result=new Condition(result, FADA_OR, result);
   if(lex_eq == NULL)
      lex_eq=equal;
   else
      lex_eq=new Condition(lex_eq, FADA_AND, equal);
   }

*s1_before_s2=(*fct)(textual_preced,s1,s2);
*s2_before_s1=(*fct)(textual_preced,s2,s1);

if(result != NULL)
   return new Condition(result,FADA_OR,lex_eq);
else
   return NULL;

cerr<<"\nCondition*   SmartPrecedenceComputing (global.h) ... have to handle this special case ...\n";
cerr<<"ID1 ="<<s1<<" ID2="<<s2<<"\n";
throw(UNHANDLED_CASE);
exit(EXIT_FAILURE);


}

///////////////////////////////////////////////////////////////////////
Condition*
SmartPrecedenceComputing(vector<vector<bool> >* textual_preced, 
               int s1, int s2, 
               FADA_Index* index1, FADA_Index* index2,
               vector<string>* common_counters, bool*s1_before_s2, bool*s2_before_s1){
if(common_counters->size() > index1->GetSize() || common_counters->size() > index2->GetSize()){
   cerr<<"Condition*   Precedence (indexes) ... FATAL ERROR (inappropriate case)\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }
Condition* result=NULL;
Condition* lex_eq=NULL;
lex_eq=NULL;
for(int i=0; i< common_counters->size(); ++i){
   Condition *nest_pred = new Condition(new Inequation(index1->GetItem(i),FADA_LESS,index2->GetItem(i)));
   Condition *equal = new Condition(new Inequation(index1->GetItem(i),FADA_EQ,index2->GetItem(i)));
   for(int j=1;j<i;++j){
      nest_pred=new Condition(nest_pred, FADA_AND, 
               new Condition( 
                  new Inequation(index1->GetItem(j),FADA_EQ,index2->GetItem(j))));
      }
   if(result == NULL)
      result=nest_pred;
   else
      result=new Condition(result, FADA_OR, result);
   if(lex_eq == NULL)
      lex_eq=equal;
   else
      lex_eq=new Condition(lex_eq, FADA_AND, equal);
   }

*s1_before_s2=textual_preced->operator[](s1)[s2];
*s2_before_s1=textual_preced->operator[](s2)[s1];

if(result != NULL)
   return new Condition(result,FADA_OR,lex_eq);
else
   return NULL;

cerr<<"\nCondition*   SmartPrecedenceComputing (global.h) ... have to handle this special case ...\n";
cerr<<"ID1 ="<<s1<<" ID2="<<s2<<"\n";
throw(UNHANDLED_CASE);
exit(EXIT_FAILURE);
}

//////////////////////////////////////
//////////////////////////////////////
Condition*
SmartPrecedenceComputing(Program*_pgm, int s1, int s2,FADA_Index* index1,FADA_Index* index2,vector<string>* common_counters, bool * s1_before_s2, bool* s2_before_s1){

if(common_counters->size() > index1->GetSize() || common_counters->size() > index2->GetSize()){
   cerr<<"Condition*   Precedence (indexes) ... FATAL ERROR (inappropriate case)\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }
Condition* result=NULL;
Condition* lex_eq=NULL;
lex_eq=NULL;
for(int i=0; i< common_counters->size(); ++i){
   Condition *nest_pred = new Condition(new Inequation(index1->GetItem(i),FADA_LESS,index2->GetItem(i)));
   Condition *equal = new Condition(new Inequation(index1->GetItem(i),FADA_EQ,index2->GetItem(i)));
   for(int j=1;j<i;++j){
      nest_pred=new Condition(nest_pred, FADA_AND, 
               new Condition( 
                  new Inequation(index1->GetItem(j),FADA_EQ,index2->GetItem(j))));
      }
   if(result == NULL)
      result=nest_pred;
   else
      result=new Condition(result, FADA_OR, result);
   if(lex_eq == NULL)
      lex_eq=equal;
   else
      lex_eq=new Condition(lex_eq, FADA_AND, equal);
   }

*s1_before_s2=_pgm->IsSequentiallyBefore(s1,s2,common_counters);
*s2_before_s1=_pgm->IsSequentiallyBefore(s2,s1,common_counters);


if(result != NULL)
   return new Condition(result,FADA_OR,lex_eq);
else
   return NULL;

cerr<<"\nCondition*   SmartPrecedenceComputing (global.h) ... have to handle this special case ...\n";
cerr<<"ID1 ="<<s1<<" ID2="<<s2<<"\n";
throw(UNHANDLED_CASE);
exit(EXIT_FAILURE);

}


//////////////////////////////////////
//////////////////////////////////////
ContextQuast*
TraduceToContextQuast(Quast* quast, vector<string>* var, vector<vector<Inequation*> >* inequations){
if(quast->IsLeaf()){
   if(quast->IsEmpty()){
      return new ContextQuast();
      }
   else{
      map<string, Expression*>   __mapping;
      vector<Expression*>::iterator ite=quast->GetVertex()->GetIndex()->begin();
      for(vector<string>::iterator it=var->begin(); it != var->end(); ++it,++ite){
         if(ite == quast->GetVertex()->GetIndex()->end()){
            cerr<<"\nTraduceToContextQuast ..... FATAL ERROR(inappropriate case)\n";
            throw(INAPPROPRIATE_CASE);
            }
         __mapping.operator[](*it)=*ite;
         }

      vector<vector<Inequation*> > leaf_const;
      
      for(vector<vector<Inequation*> >::iterator it=inequations->begin(); it != inequations->end(); ++it){
         vector<Inequation*>  term;
         for(vector<Inequation*>::iterator iti=(*it).begin(); iti != (*it).end(); ++iti)
            term.push_back((*iti)->Substitute(&__mapping));
         leaf_const.push_back(term);
         }
            
      
      return new ContextQuast(&leaf_const);
      }
   }
else
   return new ContextQuast(
         quast->GetCondition(),
         TraduceToContextQuast(quast->GetThenPart(), var, inequations), 
         TraduceToContextQuast(quast->GetElsePart(), var, inequations));

cerr<<"\nContextQuast*   TraduceToContextQuast ...... Unhandled case\n";
throw(INAPPROPRIATE_CASE);
exit(EXIT_FAILURE);
}


//////////////////////////////////////
vector<Inequation*>
MergeInequations(vector<Inequation*>* __ineq1, vector<Inequation*> * __ineq2){
vector<Inequation*>   result;
for(vector<Inequation*>::iterator it=__ineq1->begin(); it != __ineq1->end(); ++it)
   result.push_back(*it);
for(vector<Inequation*>::iterator it=__ineq2->begin(); it != __ineq2->end(); ++it)
   result.push_back(*it);
return result;
}


//////////////////////////////////////
vector<Inequation*>
NegateInequations(vector<Inequation*>* __ineqs){
//WARNING : input= conjuctive condition ... so output = disjonctive term

vector<Inequation*>   result;
for(vector<Inequation*>::iterator it=__ineqs->begin(); it != __ineqs->end(); ++it)
   result.push_back((*it)->Negate(true));
return result;
}
//////////////////////////////////////
vector<LexicographicInequation*>
Merge(vector<LexicographicInequation*>* __rela1,vector<LexicographicInequation*>* __rela2){
vector<LexicographicInequation*> result;
for(vector<LexicographicInequation*>::iterator it=__rela1->begin(); it != __rela1->end(); ++it)
   result.push_back(*it);

for(vector<LexicographicInequation*>::iterator it=__rela2->begin(); it != __rela2->end(); ++it)
   result.push_back(*it);
return result;
}

///////////////////////////////////////
vector<LC_RHS_Term*>
Merge(vector<LC_RHS_Term*>* __rela1,vector<LC_RHS_Term*>* __rela2){
vector<LC_RHS_Term*> result;
bool __false=false;;
for(vector<LC_RHS_Term*>::iterator it=__rela1->begin(); it != __rela1->end(); ++it){
   LC_RHS_Term* lc=*it;
   if(lc->GetInequation()->IsValue() && !lc->GetInequation()->GetValue()){
      __false=true;
      break;
      }
   result.push_back(*it);
   }
if(__false){
   result.clear();
   result.push_back(new LC_RHS_Term(new Inequation(false)));
   return result;
   }
   

for(vector<LC_RHS_Term*>::iterator it=__rela2->begin(); it != __rela2->end(); ++it){
   LC_RHS_Term* lc=*it;
   if(lc->GetInequation()->IsValue() && !lc->GetInequation()->GetValue()){
      __false=true;
      break;
      }
   result.push_back(*it);
   }
if(__false){
   result.clear();
   result.push_back(new LC_RHS_Term(new Inequation(false)));
   return result;
   }
return result;
}

//////////////////////////////////////

vector<string>
MergeVariables(vector<string>* var1,vector<string>* var2){
vector<string> result;
result=*var1;
for(vector<string>::iterator it=var2->begin(); it != var2->end(); ++it)
   if(find(result.begin(), result.end(),*it) == result.end())
      result.push_back(*it);
return result;
}
//////////////////////////////////////
//////////////////////////////////////

void
PrintVersion(void){
cout<<__fada_version"\n";
// cout<<"\n PIPLIB-1.3.6 (recommended)\n";
}

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////

void
PrintHelp(void){
cout<<"Using fadatool\n\
$ fadatool [options]\n\
Where options are :\n\
\n\
    => Options relative to the FADA processing :\n\
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
\n\
      '--structural-analysis' or '-s' :  apply the structural analysis.\n\
   \
\n\
    => Options relative to the printing :\n\
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
\n\
      '--format=FORMAT'      : where 'FORMAT' in {'html+tex','html+dot', 'console'}\
      '--dir=dir' or '-d dir'      : set 'dir' as working and output directory ('./' is taken as default). if doesn't exist it will be created\n\
      '--tree' or '-T'      : print definitions in Quasi Affine Selection trees.\n\
      '--bottom' or '-b'      : consider a node 'bottom' for not initialized variables in the dataflow graph.\n\
      '--log' or'-L'         : write results in a file.\n\
      '--fuzzy' or '-f'      : remove constraints involving new parameters.\n\
      '--graph' or '-g'      : generate a dataflow graph.\n\
      '--verbose' or '-v'      : print extra information for debugging.\n\
      '--help' or '-h'      : print a help screen and exit.\n\
      '--version' or '-V'      : print the version and exit.\n\
\n\
      '--input=file' or '-i file'   : specify the input file.\n\
\n\
  Examples :\n\
\n\
   $ fadatool -i input.c\n\
        Perform dataflow analysis, results are printed in the standard output file.\n\
\n\
   $ fadatool -i input.c --format=html+tex \n\
        Results are provided in html pages created in the directory 'dir'\n\
\n";
}

//////////////////////////////////////
//////////////////////////////////////

vector<vector<Inequation*> >
SimplifyInequations(vector<vector<Inequation*> >* __to_be_simplified, vector<vector<Inequation*> >* __env, vector<string>* __param){
vector<vector<Inequation*> >  result;
for(vector<vector<Inequation*> >::iterator it=__to_be_simplified->begin(); it != __to_be_simplified->end(); ++it){
   vector<vector<Inequation*> >   local_term;local_term.push_back(*it);
   vector<vector<Inequation*> >    local=AND_DNF(&local_term,__env);
   if(!IsUnsatisfiable(&local,__param)){
      result.push_back(*it);
      }
   }
return result;
}
//////////////////////////////////////
//////////////////////////////////////
/*vector<vector<Inequation*> > 
SimplifyInequations(vector<vector<Inequation*> >* __to_be_simplified, vector<Inequation*>* __env,vector<string>* __param){
vector<vector<Inequation*> >  result;
for(vector<vector<Inequation*> >::iterator it=__to_be_simplified->begin(); it != __to_be_simplified->end(); ++it){
   vector<Inequation*> term=MergeInequations(&(*it),__env);
//    cout<<"\n\nSimplifyInequations==========================================\n\n";
//    cout<<PrintInequations(&term)<<"\n";
   bool res,insert=true;
   if(IsUpHeld(&term,__param,&res)){
      if(res){
//          cout<<"Tout le temps vrai";
         insert=true;
         }
      else{
         insert=false;
//          cout<<"Tout le temps faux";
         }
      }
   else{
//       cout<<"Tout le temps on sait pas quoi";
   }

   if(insert)
      result.push_back(*it);
//     cout<<"\nSimplifyInequations==========================================\n";
   }
return result;
}*/
//////////////////////////////////////
//////////////////////////////////////

vector<vector<Inequation*> >
TraduceNEQOperations(vector<vector<Inequation*> >* __constraints){
vector<vector<Inequation*> > result;
for(vector<vector<Inequation*> >::iterator it = __constraints->begin(); it != __constraints->end(); ++it){
   
   vector<vector<Inequation*> > local=TraduceNEQOperations(&(*it));
   
   
   for(vector<vector<Inequation*> >::iterator itt = local.begin(); itt != local.end(); ++itt)
      result.push_back(*itt);
   }

return result;
}
//////////////////////////////////////
//////////////////////////////////////
vector<vector<Inequation*> > 
TraduceNEQOperations(vector<Inequation*>* __constraint){

vector<Inequation*> __neq, __others;
CollectNEQInequations(__constraint,&__neq,&__others);

vector<vector<Inequation*> > result;
result.push_back(__others);
for(vector<Inequation*>::iterator it=__neq.begin(); it != __neq.end();++it){
   Condition* cond=new Condition(*it);
   cond=cond->TraduceNEQInequations();
   Inequation* ineq1,*ineq2;
   ineq1=cond->GetLeftChild()->GetInequation();
   ineq2=cond->GetRightChild()->GetInequation();

   vector<vector<Inequation*> > local_res=DistributeNEQuation(ineq1,ineq2,&result);
   result=local_res;
   }

return result;
}

//////////////////////////////////////
//////////////////////////////////////
void
CollectNEQInequations(vector<Inequation*>* __all,vector<Inequation*>* __neq,vector<Inequation*>* __others){

for(vector<Inequation*>::iterator it=__all->begin(); it != __all->end(); ++it){
   if(!(*it)->IsValue() && (*it)->GetPredicate()==FADA_NEQ)
      __neq->push_back(*it);
      
   else
      __others->push_back(*it);
   }
}
//////////////////////////////////////
//////////////////////////////////////
vector<vector<Inequation*> >
DistributeNEQuation(Inequation* ineq1,Inequation* ineq2,vector<vector<Inequation*> >* constraints){
vector<vector<Inequation*> > result;
for(vector<vector<Inequation*> >::iterator it=constraints->begin(); it != constraints->end(); ++it){
   vector<Inequation*> local1,local2;
   local1=*it;
   local2=*it;
   local1.push_back(ineq1);
   local2.push_back(ineq2);
   result.push_back(local1);
   result.push_back(local2);
   }
return result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<vector<Inequation*> >
EliminateDoubles(vector<vector<Inequation*> >* __in){
vector<vector<Inequation*> > result;
/*cout<<"\nEliminateDoubles(vector<vector<Inequation*> >* __in){************************** AVANT\n";
cout<<PrintInequations(__in);*/
for(vector<vector<Inequation*> >::iterator it=__in->begin(); it != __in->end();++it){
   bool found=false;
   for(vector<vector<Inequation*> >::iterator itf=result.begin(); itf != result.end();++itf)
      if(PrintInequations(&(*it))==PrintInequations(&(*itf))){
         found=true;
         break;
         }
   if(!found)
      result.push_back(*it);
   }
/*cout<<"\nEliminateDoubles(vector<vector<Inequation*> >* __in){************************** APRES\n";
cout<<PrintInequations(&result);*/
return result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<string>
RemoveDoubles(vector<string>* _v){
vector<string> result;
for(vector<string>::iterator it=_v->begin(); it != _v->end();++it)
   if(find(result.begin(),result.end(),*it)==result.end())
      result.push_back(*it);
return result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<vector<Inequation*> >
RemoveAlphas(vector<vector<Inequation*> >* ineq,vector<string>* alphas){
cout<<"\nRemoveAlphas... is not implemented \n";
exit(EXIT_FAILURE);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<Inequation*>
RemoveAlphas(vector<Inequation*>* ineq,vector<string>* alphas){
cout<<"\nRemoveAlphas... is not implemented \n";
exit(EXIT_FAILURE);

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


vector<vector<Inequation*> >
RemoveFlase(vector<vector<Inequation*> >* __v, vector<string>* __param){
vector<vector<Inequation*> > result;
for(vector<vector<Inequation*> >::iterator it=__v->begin(); it != __v->end(); ++it){
   bool   __true;
   if(!IsUnsatisfiable(&(*it),__param))
         result.push_back(EliminateTrueAndDoubles(&(*it)));
   }
return result;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<Inequation*>
Substitute(vector<Inequation*>* ineq,map<string, Expression*>* __map){
vector<Inequation*> result;

for(vector<Inequation*>::iterator it=ineq->begin(); it != ineq->end();++it){
   result.push_back((*it)->Substitute(__map));
   }
return result;

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<vector<Inequation*> >
Substitute(vector<vector<Inequation*> >* __v, map<string, Expression*>* __map){
vector<vector<Inequation*> > result;
for(vector<vector<Inequation*> >::iterator it=__v->begin(); it != __v->end();++it){
   result.push_back(Substitute(&(*it),__map));
   }
return result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void
ParseOptions(int argc, char* argv[],Options* options){

static struct option long_options[] =
{
   // dataflow graph options
   {"graph",      no_argument,      0, 'g'},
   {"bottom",      no_argument,      0, 'b'},
   {"condition",      no_argument,      0, 'c'},

   // printing options
   
//    {"html", no_argument,                 0, 'H'},
//   {"tex",         no_argument,      0, 't'},
   {"tree",      no_argument,      0, 'T'},
// 
//    {"rename",   no_argument,             0, 'R'},
//    {"alphas",   no_argument,             0, 'A'},
// 
   {"log",         no_argument,      0, 'l'},
//    {"fuzzy",   no_argument,              0, 'f'},
   {"structural-analysis",   no_argument,      0, 's'},
   {"verbose",         no_argument,            0, 'v'},

   {"dir",         required_argument,   0, 'd'},
   {"input",      required_argument,   0, 'i'},
   {"format",      required_argument,   0, 'f'},
   {"version",      no_argument,      0, 'V'},
   {"help",      no_argument,      0, 'h'},
   {0, 0, 0, 0}
};

while (true){
   int option_index = 0;
   
   unsigned int c = getopt_long (argc, argv, "gbcTlsVvhd:i:f:",long_options, &option_index);

   if (c == -1)
      break;
   switch (c){
      case 'h':
         PrintHelp();
         exit(EXIT_SUCCESS);

      case 'V':
         PrintVersion();
         exit(EXIT_SUCCESS);

      case 'T':
         options->quast.flat=false;
         break;

      case 'b':
         options->graph.bottom=true;
         
         break;
      case 'L':
         options->io.log=true;
         break;
      case 's':
         options->processing.apply_structural_analysis=true;
         break;
      case 'g':
         options->graph.dg=true;
         options->graph.statement=true;
         break;
      case 'd':
         options->io.output_directory=optarg;
         options->io.output_directory+="/";
         break;
      case 'c':
         options->graph.condition=true;
         break;
      case 'i':
         options->io.file_name=optarg;
         break;
      case 'v':
         options->printing.verbose=true;
         break;
      case 'f':{
         string format=(char*)optarg;
         if(format=="html+tex"){
            options->printing.command_line_printing=false;
            options->printing.tex_printing=true;
            }
         else
            if(format=="html+dot"){
               options->printing.command_line_printing=false;
               options->printing.tex_printing=false;
               }
            else{
               if(format=="console")
                  options->printing.tex_printing=true;
               else{
                  cout<<"Invalid argument for the option '--format='\n";
                  PrintHelp();
                  exit(EXIT_FAILURE);
                  }
               }
         break;
         }

      default:
         break;
      }
   }


if (optind < argc){
   printf ("Invalid options '");
   while (optind < argc)
      printf ("%s ", argv[optind++]);
   cout<<"'\n";
   PrintHelp();
   exit(EXIT_FAILURE);
   }

return;
}

//////////////////////////////////
//////////////////////////////////

bool
IsUnsatisfiable(vector<Inequation*>* constraints, vector<string>* parameters){
vector<string> var;
Solver s(-1,-1,RemoveDoubles(parameters),var,constraints);
Quast *m=s.Max();
/*cout<<"\n =======IsUnsatisfiable*********************************** \n";
s.Print();
m->Print("\n");
cout<<"\n =======IsUnsatisfiable (END)*********************************** \n";*/
if(m->IsLeaf() && m->IsEmpty())
   return true;
return false;
}

//////////////////////////////////
//////////////////////////////////
bool   IsUnsatisfiable(vector<vector<Inequation*> >* constraints, vector<string>* parameters){
for(vector<vector<Inequation* > >::iterator it=constraints->begin(); it != constraints->end(); ++it){
   if(!IsUnsatisfiable(&(*it),parameters))
      return false;
   }
return true;
}

//////////////////////////////////
//////////////////////////////////
vector<Inequation*>
RecoverAndSubstituteAlphas(vector<string>* alphas, vector<Inequation*>* constraints){
map<string, Expression*> mapping;
for(vector<Inequation*>::iterator it=constraints->begin(); it!= constraints->end(); ++it){
   Inequation* current=*it;
   if(!current->IsValue()){
      if(current->GetLHS()->IsVariable()&&find(alphas->begin(), alphas->end(),current->GetLHS()->GetVariableName()) != alphas->end())
         mapping[current->GetLHS()->GetVariableName()]=current->GetRHS();
      if(current->GetRHS()->IsVariable() && find(alphas->begin(), alphas->end(),current->GetRHS()->GetVariableName()) != alphas->end())
         mapping[current->GetRHS()->GetVariableName()]=current->GetLHS();
      }
   }
if(mapping.size()==0)
   return *constraints;
vector<Inequation*> result;
for(map<string,Expression*>::iterator it=mapping.begin(); it != mapping.end();++it)
   result.push_back(new Inequation(new Expression(it->first),FADA_EQ,it->second));
for(vector<Inequation*>::iterator it=constraints->begin(); it!= constraints->end(); ++it)
   result.push_back((*it)->Substitute(&mapping));
return result;
}
//////////////////////////////////
//////////////////////////////////

vector<vector<Inequation*> > 
RecoverAndSubstituteAlphas(vector<string>* alphas, vector<vector<Inequation*> > * constraints){
vector<vector<Inequation*> >  result;
for(vector<vector<Inequation*> > ::iterator it=constraints->begin(); it != constraints->end(); ++it)
   result.push_back(EliminateTrueAndDoubles(&RecoverAndSubstituteAlphas(alphas,&(*it))));
return result;
}

//////////////////////////////////
//////////////////////////////////

}   //namesapace fada
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////  END   /////////////////////////////////
//////////////////////////////////////////////////////////////////////////


