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

// authors: 
// Christophe Alias
// Marouane Belaoucha


#include "quast.h"
#include "constants.h"
#include "global.h"
#include <algorithm>
// #include "adaan_demonstrator.h"


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

namespace fada{
void
Quast::Set(int __index, int __deep, vector<string>* __counters, vector<string>*__params, PipQuast* __quast, map<int, Expression*> __new_param){

if(__quast->newparm != NULL)      //Add current newparms
   {
   cout << "Quast::Set ...  WARNING: new parms" << endl;
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
      this->Set();

         else   //leaf
      this->Set(__index,__deep, __counters,PipListToIndex(__quast->list,__counters->size(),__params, __new_param));
       }
else      //conditional node
   {
   Expression *__e = PipVectorToExpression(__quast->condition,__params,__new_param);
   Condition * __cond= new Condition(new Inequation(__e,FADA_GREATER_EQ,new Expression(0)));

   Quast*   __then = new Quast();
   Quast*   __else = new Quast();
   __then->Set(__index, __deep, __counters, __params, __quast->next_then, __new_param);
   __else->Set(__index, __deep,__counters, __params, __quast->next_else, __new_param);

   this->Set(__cond,__then,__else);
   }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


Quast::Quast()
{
Initialize( );
Set();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Quast::~Quast()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


void
Quast::Initialize(void)
{
SetEmpty( true );
SetLeaf( true );
SetCondition( NULL );
SetThenPart( NULL );
SetElsePart( NULL );
SetVertex( NULL );
SetAssignment( -1 );
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Quast::Set()
{
Initialize( );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Quast::Set(Condition* __cond, Quast* __then, Quast* __else)
{

SetLeaf( false );
SetEmpty( false );
SetCondition( __cond );
SetThenPart(  __then );
SetElsePart( __else );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Quast::Quast(Condition* __cond, Quast* __then, Quast* __else)
{
Initialize( );
Set(__cond,__then,__else);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
Quast::Quast(int __index, int __deep, vector <string>* __counters, vector<string>* __params,PipQuast* __quast){
map<int, Expression*>   mapping;
Set(__index,__deep,__counters,__params,__quast ,mapping);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

string
Quast::Print_str(string prefix)
{
if(IsLeaf( ))
   {
   if(IsEmpty( ))
      {

         return prefix+"_|_";
      }


   ostringstream oss;
   oss<<prefix<<" < "<<GetAssignment( )<<" : "<<GetVertex( )->Print_str( ) <<" > ";

   return oss.str();
   }

string result=prefix+"if ( "+GetCondition()->Print_str()+" )";
result+=prefix+"\t{";
result+=this->GetThenPart( )->Print_str(prefix+"\t");
result+=prefix+"\t}";
result+=prefix+"else";
result+=prefix+"\t{";
result+=this->GetElsePart( )->Print_str(prefix+"\t");
result+=prefix+"\t}";
return result;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Quast::Print(string prefix)
{
cout<<Print_str(prefix );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Quast*
Quast::Max(Quast* quast)
{
if(IsEmpty( ))
   return quast;

if(quast->IsEmpty( ))
   return this;


if(IsLeaf( ) && quast->IsLeaf( ))
   {
   vector<string>      *counters1,*counters2;
   counters1=GetCounters();
   counters2=quast->GetCounters();

   vector<string> common_counters;

   for(int i=0; i < min(counters1->size(),counters2->size()); i++)
      {
      if(counters1->operator[](i) == counters2->operator[](i))
      common_counters.push_back(counters1->operator[](i));
      else break;
      }



   Condition *pred;
   bool firsttime=true;

   //Compute the lexicographic order
   //q1->point << q2->point ?
   for(int i=0; i<common_counters.size(); i++)
   // < level
      {
      Condition *nest_pred = new Condition(new Inequation(GetVertex( )->GetItem(i),FADA_LESS,quast->GetVertex( )->GetItem(i)));
      //i1_i < i2_i
      for(int j=0; j<i; j++)
         {
         nest_pred = new Condition(nest_pred,FADA_AND,new Condition( new Inequation(GetVertex( )->GetItem(j),FADA_EQ,quast->GetVertex( )->GetItem(j))));
         }

      if(!firsttime)
         pred = new Condition(pred,FADA_OR, nest_pred);
      else
         {
         firsttime=false;
         pred = nest_pred;
         }
      //add it to pred
      }
      
      if(GetAssignment() < quast->GetAssignment())
      //S1 <text S2 ?
         {
         Condition *text_pred = new Condition(new Inequation(true));
         for(int i=0; i<common_counters.size(); i++)
            text_pred = new Condition(text_pred, FADA_AND,new Condition(new Inequation(GetVertex( )->GetItem(i),FADA_EQ,quast->GetVertex( )->GetItem(i))));
            //i1_i = i2_i for each i

         if(!firsttime)
            pred = new Condition(pred,FADA_OR,text_pred);
         else
            {
            firsttime=false;
            pred = text_pred;
            }
         }
   
      Quast *result = new Quast(pred,quast, this);
      return result;
      
   }

if(IsIf( ))
   return new Quast(GetCondition( ),GetThenPart( )->Max(quast),GetElsePart( )->Max(quast));

if(quast->IsIf( ))
    return new Quast(quast->GetCondition( ),Max(quast->GetThenPart( )),Max(quast->GetElsePart( )));

cerr<<endl<<endl<<"Quast::Max, fatal error ==> unhandled case"<<endl;
throw(UNHANDLED_CASE);
exit(1);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Quast::Set(int __index, int __deep,vector<string>* __counters, FADA_Index* __vertex)
{

SetLeaf( true );
SetEmpty( false );
SetDeep(__deep);
SetAssignment   (   __index    );
SetVertex    (   __vertex   );
SetCounters   (   __counters   );
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Quast::Quast(int __index, int __deep,vector<string>* __counters, FADA_Index* __vertex)
{
Initialize( );
Set(__index,__deep, __counters, __vertex );
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

string
Quast::ToGraphViz(string parent_node_name)
{

if(IsEmpty( ))
   return parent_node_name+ " [label =\"_|_\",shape=circle, color=thistle1, fontcolor=purple];\n";

   
if(IsLeaf())
   {
   string ch=parent_node_name;
   string result;
   ostringstream   ss;
   ss << "S"<<GetAssignment( )<<"  :\\n";
   ss << GetVertex()->Print_str();
   ostringstream   Curr;
   Curr<<ch<<" [label =\""<<ss.str()<<"\",shape=box, color=blue, style=filled, fontcolor=white, fontstyle=bold];"<<endl;
   result += Curr.str();

   return result;
   }

if(IsIf() )
   {
   string   Then, Else, result;
   Then = parent_node_name +" -> " +parent_node_name+"T" + "[label = \"Yes\"];\n";
   Else = parent_node_name +" -> " +parent_node_name+"E" + "[label = \"No\"];\n";
   result = Then + Else + GetThenPart( )->ToGraphViz(parent_node_name+"T") + GetElsePart( ) -> ToGraphViz( parent_node_name+"E" );

   result += parent_node_name + " [label =\""+GetCondition()->Print_str()+"\"];";
   return result;
   }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

string
Quast::ToGraphViz(void)
{
string result="digraph G {\n\tbgcolor=azure;\n\tnode [shape=polygon, sides=6, color=lightblue2, style=filled];\n\tedge [arrowsize=2, color=gold];";
result += ToGraphViz("R");
result += "\n}";
return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void
Quast::ToGraphVizFile(string filename)
{
FILE *f;
f=fopen(filename.c_str(),"w");
fprintf(f,"%s",ToGraphViz( ).c_str( ));
fclose(f);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


void
Quast::Substitute(map<string, string>* __mapping)
{
if(IsLeaf( ))
   {
   if(!IsEmpty( )){
      GetVertex( )->Substitute( __mapping );
      return;
      }
   return;
   }
if(IsIf( ))
   {
   GetCondition( )->Substitute( __mapping );
   GetThenPart( )->Substitute( __mapping );
   GetElsePart( )->Substitute( __mapping );
   return;
   }
return; 
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Quast*
Quast::Substitute(map<string, Expression*>* __mapping )
{
if(IsLeaf()){
   if(IsEmpty())
      return new Quast();


   return new Quast(GetAssignment(),GetDeep(), GetCounters(), GetVertex()->Substitute(__mapping));
   }

return new Quast(GetCondition()->Substitute(__mapping), GetThenPart()->Substitute(__mapping), GetElsePart()->Substitute(__mapping));
};


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

Quast*
Quast::Compress()
{
if(this->IsLeaf())
   return this;

Condition*   __cond=GetCondition();

Quast* __else=GetElsePart()->Compress();
string __else_str=__else->Print_str();
return GetThenPart()->Compress( __cond, __else,&__else_str );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
Quast*
Quast::Compress(Condition* __cond, Quast* __else_part, string * __else_str)
{

if(!IsLeaf( ))
   {
   if(GetElsePart()->Print_str() == *__else_str)
      {
      Condition* local__cond=new Condition(__cond,FADA_AND,GetCondition( ));
      return GetThenPart( )->Compress( local__cond, __else_part, __else_str);
      }
   }
return new Quast(__cond,this,__else_part);


//TODO: generalized version

/*if(IsIf( ))
   {
   if(GetElsePart()->IsLeaf())
      { 
      if( GetElsePart( )->IsEmpty( ))
         {
         Condition* local__cond=new Condition(__cond,FADA_AND,GetCondition( ));
         return GetThenPart( )->Compress( local__cond);
         }
      Condition* __true=new Condition(new Inequation (true));
      Quast*   quast1=new Quast( GetCondition(),   GetThenPart()->Compress(__true),GetElsePart());
      return new Quast (__cond,   quast1, new Quast());
      }

      Condition* __true=new Condition(new Inequation (true));
      Quast*   quast1=new Quast( GetCondition(), GetThenPart()->Compress(__true), GetElsePart()->Compress(__true));
      return new Quast (__cond,quast1, new Quast());
   }
return new Quast(__cond,this,new Quast());
*/
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


Quast*
Quast::Clone()
{
if(IsLeaf() && IsEmpty( ))
   return new Quast();

if(IsLeaf( ) )
   return new Quast (GetAssignment( ),GetDeep(), GetCounters( ), GetVertex( ));

if(IsIf())
   return new Quast(GetCondition( ), GetThenPart( ), GetElsePart( ));

cerr<<"\nQuast::Clone(), fatal error : illegal information"<<endl;
throw(BAD_OBJECT);
exit(EXIT_FAILURE);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


Quast*
Quast::Copy()
{
if(IsEmpty( ))
   return new Quast();
if(IsLeaf( ))
   return new Quast (GetAssignment( ), GetDeep(),GetCounters( ), GetVertex( )->Copy( ));

return new Quast(GetCondition( )->Copy(), GetThenPart( )->Copy( ), GetElsePart( )->Copy());
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

string
Quast::Generate_GraphViz_Edges(Program* prog, int read_stmt, int read_ref, Condition* __cond,bool print_condition, bool print_relation, bool rename,bool bottom)
{
if(IsLeaf())
   {
   if(!IsEmpty())
      { 
      ostringstream edge;
      edge<<endl<<"S"<<this->GetAssignment();
      edge<<" -> "<<"S"<<read_stmt;
      edge<<" [label=\"";
      if(print_condition)
         {
         if(__cond != NULL)
            {
            Condition* cond2=__cond->EliminateNotOperations();

            if(!cond2->IsLeaf())
               edge<<cond2->GetRightChild()->Print_str(  );
            else
               edge<<"true ";
                  
            edge<<" \\n ";
            }
         else
            edge<<"true \\n";
         }
      if(print_relation)
         edge<<this->GetVertex()->Print_str();
      else      //output as distances
         {
         References* s;
         
         vector<string>   v= Common(prog->GetNormalizedStmts()->operator[](GetAssignment())->GetCounters(),
            prog->GetNormalizedStmts()->operator[](read_stmt)->GetCounters()
            );

         FADA_Index*   dist=this->GetVertex()->ComputeDistance(&v,prog->GetNormalizedStmts()->operator[](read_stmt)->GetRV()->operator[](read_ref)->GetAllParameters(),rename);


         edge<< " ( " <<dist->Print_str() << " ) ";
         }

      edge<<" \"];"; 
      return edge.str();
      }

   if(IsEmpty())
      {
      if(bottom){
         if(__cond != NULL)
            {
            ostringstream edge;
            edge<<endl<<FADA_Bottom" -> S"<<read_stmt<< "[label=\"";
            if(print_condition)
               {
               cout<<endl<<"(bottom) ==> ";
               Condition* cond2=__cond->EliminateNotOperations();

               if(!cond2->IsLeaf())
                  edge<<cond2->GetRightChild()->Print_str(  );
               else
                  edge<<"true ";

//                edge<<cond2->Print_str(  )<<"\\n";
               }
            edge<<"\"];";
            return edge.str();
            }
         }
      return "";
      }
   }



Condition * then_cond, *else_cond;
if(__cond != NULL )
   {
   then_cond= new Condition(__cond,FADA_AND,this->GetCondition());
   else_cond= new Condition(__cond,FADA_AND,this->GetCondition()->FastNegation());
   }
else
   {
   then_cond= this->GetCondition();
   else_cond= this->GetCondition()->FastNegation();
   }
   
string edges=GetThenPart()->Generate_GraphViz_Edges(prog,read_stmt,read_ref,then_cond,print_condition, print_relation,rename ,bottom);
edges+=GetElsePart()->Generate_GraphViz_Edges(prog,read_stmt,read_ref,else_cond,print_condition, print_relation, rename, bottom);
return edges;
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

vector<string>   Common(vector<string>* v1,vector<string>* v2)
{
vector<string>::iterator it1, it2;
vector<string>   result;
it1=v1->begin();
it2=v2->begin();
while(it1!=v1->end() && it2 != v2->end() && *it1 == *it2)
   {
   result.push_back(*it1);
   ++it1;++it2;
   }
return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
string
Quast::Generate_C_Test_Code(string ArrayID, FADA_Index* AccessIndex,string prefix)
{
string indent="\t";
if(this->IsLeaf())
   {
   if(this->IsEmpty())
      {
      string result;
      result=prefix+"//// Empty leaf ==> No dependence detected --> generate code to be sure";
      result+=prefix+"//#ifdef __TEST_DEBUG__";
      result+=prefix+"if("+FADA_Tracer_Prefix+ArrayID+AccessIndex->Generate_C_Code()+".WasTouched())\t\t //array-case was defined ?";
      result+=prefix+indent+"{\t\t\t //yes (and no dependence detected) ==> Fatal analysis error";
      result+=prefix+indent+"cout<<endl<<\"Fatal Error : quast indicates no dependecies when they exists !!!\"<<endl;";
      result+=prefix+indent+"exit(1);";
      result+=prefix+indent+"}";
      result+=prefix+"else\t\t //array-case not previously defined ==> all right goon";
      result+=prefix+indent+"{";
      result+=prefix+indent+"cout << endl <<\"no dependence here !!\" << endl;";
      result+=prefix+indent+"}";
      result+=prefix+"//#endif";
      result+="//////////////////////////////////////////////// Empty leaf ==> End";
      return result;
      }
   else
      {
      string array;
      array=FADA_Tracer_Prefix+ArrayID+AccessIndex->Generate_C_Code();

      ostringstream   s;

      s<<"\n"<<prefix<<"//++++++   flow-dependency detected ==> generate code to be sure";
      s<<"\n"<<prefix<<"vector < int >  __v_i;";

      vector<Expression*>::iterator   it_index=this->GetVertex()->GetIndex()->begin();

      for(vector<string>::iterator it= this->GetCounters()->begin(); it != this->GetCounters()->end(); ++it, ++it_index)
         s<<prefix+"__v_i.push_back("+(*it_index)->Generate_C_Code()+");";


      s<<"\n"<<prefix<<"__Test_Info  __t_i("<<this->GetAssignment()<<",__v_i); \t\t//statement the dependency occurs whith";
      s<<"\n"<<prefix<<"if("<<array<<".WasTouched())\t\t//array case was defined ?";
      s<<"\n"<<prefix<<"\t{";
      s<<"\n"<<prefix<<"\tif("<<array<<".LastTouch()==__t_i)\t\t// yes, by the same operation ?";
      s<<"\n"<<prefix<<"\t\t{\t\t//yes, computed dependecy relation is correct !!";
      s<<"\n"<<prefix<<"\t\tcout<<endl<<\"Ok, dependence correctly computed\"<<endl;";
      s<<"\n"<<prefix<<"\t\t}";
      s<<"\n"<<prefix<<"\telse\t\t//was defined, but not with the evaluated operation ==> Fatal analysis error";
      s<<"\n"<<prefix<<"\t\t{";
      s<<"\n"<<prefix<<"\t\tcout<<endl<<\"Fatal Error : Computed dependence and real dont match !!!\"<<endl;";
      s<<"\n"<<prefix<<"\t\tcout<<\"Computed = \";__t_i.print();cout<<endl;";
      s<<"\n"<<prefix<<"\t\tcout<<\"Last =   \";"<<array<<".LastTouch().print();cout<<endl;";
      s<<"\n"<<prefix<<"\t\tcout<<\"Definitions stack = ***********  \";"<<array<<".print();cout<<endl;";
      s<<"\n"<<prefix<<"\t\texit(1);";
      s<<"\n"<<prefix<<"\t\t}";
      s<<"\n"<<prefix<<"\t}";
      s<<"\n"<<prefix<<"else\t\t// was not defines ==> false dependency computed, just warning";
      s<<"\n"<<prefix<<"\t{";
      s<<"\n"<<prefix<<"\tcout<<endl<<\"Warning :  Analysis computed dependecies doesn't occurs\";";
      s<<"\n"<<prefix<<"\t}";
      return s.str();
      };
   }
if(this->IsIf())
   {
   string ch;
   ch=prefix+ "if("+this->GetCondition()->Generate_C_Code()+")";
   ch+=prefix+indent+"{";
   ch+=this->GetThenPart()->Generate_C_Test_Code(ArrayID, AccessIndex,prefix+indent);
   ch+=prefix+indent+"}";
   ch+=prefix+"else";
   ch+=prefix+indent+"{";
   ch+=this->GetElsePart()->Generate_C_Test_Code(ArrayID, AccessIndex,prefix+indent);
   ch+=prefix+indent+"}";
   return ch;
   };

cerr<<endl<<endl<<"Quast::GenerateCCode    inappropriate value";
throw(INAPPROPRIATE_CASE);
exit(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

vector<FADA_Index*>
Quast::GetVertices(void)
{
if(this->IsLeaf())
   {
   if(this->IsEmpty())
      {
      vector<FADA_Index*>   vect;
      return   vect;
      }
   else
      {
      vector<FADA_Index*>   __vect;
      __vect.push_back(this->GetVertex());
      return   __vect;
      }
   }
vector<FADA_Index*> __vect1, __vect2;
__vect1=this->GetThenPart()->GetVertices();
__vect2=this->GetElsePart()->GetVertices();

for(vector<FADA_Index*>::iterator   it=__vect2.begin(); it != __vect2.end(); ++it)
   __vect1.push_back(*it);
return __vect1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////



Quast*
Quast::Uncompress(void){
if(IsLeaf())
   return Clone();


Condition*   __node=GetCondition();
return SingleInequation(__node, GetThenPart()->Uncompress(), GetElsePart()->Uncompress());
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

Quast*  
SingleInequation(Condition* __cond, Quast* __then, Quast* __else){
if(__cond->IsLeaf())
   return new Quast(__cond, __then, __else);

if(__cond->IsOR())
   {

//   if A or B then      if(A) then T1
//      T1      else if(B) then T1
//   else    T2      else T2

   Quast*   __if1=SingleInequation(__cond->GetRightChild(), __then, __else );
   Quast*   __if2=SingleInequation(__cond->GetLeftChild(), __then, __if1);   
   return __if2;
   };
if(__cond->IsAND())
   {

   Quast*   __if1=SingleInequation(__cond->GetRightChild(), __then, __else );
   Quast*   __if2=SingleInequation(__cond->GetLeftChild(), __if1, __else);
   return __if2;
   };

cerr<<"\nSingleInequation, condition can contain NOT operators ... \n";
cerr<<__cond->Print_str();
cerr<<"\nSingleInequation, condition can contain NOT operators ... \n";
throw(INAPPROPRIATE_CASE);
exit(0);
}


////////////////////////////////////////////////////
////////////////////////////////////////////////////

////////////////////////////////////////////////////
////////////////////////////////////////////////////

bool
Quast::ContainBottoms(void){
if(IsLeaf())
   return IsEmpty();

return GetElsePart()->ContainBottoms() || GetThenPart()->ContainBottoms();
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////

Quast*
Quast::PutToBottoms(Quast* q){

if(IsLeaf())
   if(IsEmpty())
      return q;
   else
      return this;

return new Quast(GetCondition(), GetThenPart()->PutToBottoms(q), GetElsePart()->PutToBottoms(q));
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
Quast*
Quast::Simplify(vector<vector<Inequation*> > __environmnt, vector<string>* __param){

if(__environmnt.size() == 0)   // env=false
   return new Quast();

if(IsLeaf()){
   return this;
   }


if(GetThenPart()->IsLeaf() && GetThenPart()->IsEmpty() && GetElsePart()->IsLeaf() && GetElsePart()->IsEmpty())
   return GetThenPart();

if(!GetCondition()->IsLeaf()){
   cerr<<"\nQuast::Simplify(ndf terms), FATAL ERROR ('this' must be uncompressed quast)<<"<<GetCondition()->Print_str()<<"<<\n";
   throw(INAPPROPRIATE_CASE);
   exit(EXIT_FAILURE);
   }

if(this->GetCondition()->GetInequation()->IsValue())
   if(GetCondition()->GetInequation()->GetValue())
      return GetThenPart()->Simplify(__environmnt,__param);
   else
      return GetElsePart()->Simplify(__environmnt,__param);

vector<Inequation*>   __env_then, __env_else;
__env_then.push_back(GetCondition()->GetInequation());
Inequation* local_may_be_neq=GetCondition()->GetInequation()->Negate(true);
bool ___neq=false;
vector<vector<Inequation*> > __local_then, __local_else;
if(!local_may_be_neq->IsValue() && local_may_be_neq->GetPredicate()==FADA_NEQ){
   Inequation* ineq1,*ineq2;
   ineq1=new Inequation(local_may_be_neq->GetLHS(),FADA_LESS,local_may_be_neq->GetRHS());
   ineq2=new Inequation(local_may_be_neq->GetLHS(),FADA_GREATER,local_may_be_neq->GetRHS());
   __local_else=DistributeNEQuation(ineq1,ineq2,&__environmnt);
   ___neq=true;
   }
else{
   __env_else.push_back(local_may_be_neq);
   __local_else.push_back(__env_else);
   __local_else=AND_DNF(&__environmnt,&__local_else);
   }


__local_then.push_back(__env_then);
__local_then=AND_DNF(&__environmnt,&__local_then);
/*
if(!___neq){
   vector<vector<Inequation*> > current;
   vector<Inequation*> hi;
   hi.push_back
   current.push_back(local_may_be_neq);
   __local_else=AND_DNF(&__environmnt,&current);
   }*/

// cout<<"\n Quast::Simplify .......................... BEGIN \n";
// cout<<"\nENV THEN : "<<PrintInequations(&__local_then);
// cout<<"\nENV ELSE : "<<PrintInequations(&__local_else);

if(IsUnsatisfiable(&__local_then,__param)){
//    cout<<"\n THEN = FALSE \n";
   
   return GetElsePart()->Simplify(__local_else,__param);
   }
if(IsUnsatisfiable(&__local_else,__param)){
//    cout<<"\n ELSE = FALSE \n";
   return GetThenPart()->Simplify(__local_then,__param);
   }

Quast* then_quast=GetThenPart()->Simplify(__local_then,__param);
Quast* else_quast=GetElsePart()->Simplify(__local_else,__param);
if(then_quast->IsLeaf() && else_quast->IsLeaf() && then_quast->Print_str("")== else_quast->Print_str(""))
   return then_quast;
return new Quast(GetCondition(),then_quast,else_quast );

}
////////////////////////////////////////////////////

Quast*
Quast::EliminateRedondantLeaves(bool __hardlier){
if(IsLeaf())
   return this;
if(GetThenPart()->IsLeaf() && GetElsePart()->IsLeaf() )
   if(GetThenPart()->Print_str() == GetElsePart()->Print_str())
      return GetThenPart();

Quast* quast=GetElsePart()->EliminateRedondantLeaves(__hardlier);
Quast* quast2=GetThenPart()->EliminateRedondantLeaves(__hardlier);
if(quast2->IsLeaf()  && quast->IsLeaf()  && quast->Print_str() == quast2->Print_str())
   return quast;

if(__hardlier)
   if(quast->Print_str() == quast2->Print_str())
      return quast;
   
return new Quast(GetCondition(), quast2, quast);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

Quast* 
Quast::Simplify(ContextQuast* __cq, vector<vector<Inequation*> > __context,vector<string>* __parameters){
vector<Inequation*> empty;
vector<vector<Inequation*> > polyhedrons=__cq->GetPolyhedrons(empty);
polyhedrons=TraduceNEQOperations(&polyhedrons);
polyhedrons=SimplifyInequations(&polyhedrons,&__context,__parameters);
return Simplify(polyhedrons,__parameters);
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void
Quast::Flatten(vector<vector<Inequation *> > env, FlatQuast* q){
if(IsLeaf())
   {

   if(IsEmpty()){

      Branch* b=new Branch(&env);

      q->AddBranch(b);

      }
   else{

      Branch* b=new Branch(&env,GetAssignment(),GetVertex());

      q->AddBranch(b);

      }
   return;
   }

vector<Inequation*>  current=GetCondition()->GetTermInequations();

vector<Inequation*>  negation=NegateInequations(&current);

vector<vector<Inequation*> > __then;
__then.push_back(current);

__then=AND_DNF(&__then,&env);

vector<vector<Inequation*> > __else;
for(vector<Inequation*>::iterator it=negation.begin(); it != negation.end(); ++it){
   vector<Inequation*> ineq;
   ineq.push_back(*it);
   __else.push_back(ineq);
   }

__else=AND_DNF(&__else,&env);

GetThenPart()->Flatten(__then,q);

GetElsePart()->Flatten(__else,q);

return;
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void   
Quast::Flatten( FlatQuast* q){
vector<vector<Inequation*> > __true;
Flatten(__true,q);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void
Quast::DropAddedVariables(int nb_original){
if(IsLeaf() && !IsEmpty()){
   if(GetVertex()->GetSize() < nb_original){
      cout<<"\n........ Quast::DropAddedVariables(int nb_original) ... inappropriate case \n";
      throw(10);
      }
   if(GetVertex()->GetSize() == nb_original)
      return ;

   vector<Expression*> index;
   for(int i=0;i<nb_original;i++)
      index.push_back(GetVertex()->GetItem(i));

//    cout<<"\n AVANT =";GetVertex()->Print();
   
   SetVertex(new FADA_Index(&index));

//    cout<<"\n APRES="; GetVertex()->Print();
   }
if(!IsLeaf()){
   GetThenPart()->DropAddedVariables(nb_original);
   GetElsePart()->DropAddedVariables(nb_original);
   }
return ;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void
Quast::LookForLoopParallelism(vector<string> original, vector<string>*parallel,vector<References*> program){
if(IsLeaf() && IsEmpty())
   return;
if(!IsLeaf()){
   if(parallel->size() == 0)
      return;
   GetThenPart()->LookForLoopParallelism(original,parallel,program);
   
   if(parallel->size() == 0)
      return;
   GetElsePart()->LookForLoopParallelism(original,parallel,program);
   return;
   }
if(IsLeaf()){
   int write_id=GetAssignment();
   References* ref;
   
   vector<string> dependent_loops=*program[write_id]->GetCounters();
   vector<string> non_parallel_loops;
   vector<string>::iterator it2;
   vector<Expression*>::iterator ite=GetVertex()->GetIndex()->begin();
/*   cout<<"\n Original loops : "<<PrintVector(&original);
   cout<<"\n dependent loops : "<<PrintVector(&dependent_loops);*/
   for(   vector<string>::iterator it=original.begin(), 
      it2 = dependent_loops.begin(); 
         it != original.end() 
         && it2 != dependent_loops.end() 
         && *it == *it2 
         && ite != GetVertex()->GetIndex()->end();
            ++it,++it2,++ite){

//       cout<<"\n Je traite la boucle :"<<*it<<"\n";
      vector<string>::iterator itnp;
      itnp=find(parallel->begin(), parallel->end(),*it);
      if(itnp != parallel->end()){
         Expression* distance=new Expression(new Expression(*it),FADA_SUB,*ite);
         distance=distance->Simplify();
         if(!distance->IsLeaf() || !distance->IsValue() || distance->GetValue() != 0){
//             cout<<"\n la boucle "<<*itnp<<" n'est pas parallel \n Distante = "<<distance->Print_str();
            parallel->erase(itnp);
            }
         }
      }
   return;
   }
cout<<"\nQuast::LookForLoopParallelism ... FATAL ERROR (unhandled case) \n";
throw(INAPPROPRIATE_CASE);
}


void
Quast::SubstituteMaximumOfParameterValues(map<string,string> map){
if(IsLeaf())
   if(IsEmpty()) return;
   else GetVertex()->Substitute(&map);
else{
   GetThenPart()->SubstituteMaximumOfParameterValues(map);
   GetElsePart()->SubstituteMaximumOfParameterValues(map);
   return;
   }
}


}// end namespace fada


