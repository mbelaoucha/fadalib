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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include <fada.h>
#include <string.h>

using namespace std;




int main(int argc, char *argv[])
{
// FADA_Index v;
// vector<string> vc;
// Quast* q=new Quast(new Condition(new Inequation(true)), new Quast(1,1,&vc,&v), new Quast(1,1,&vc,&v));
// q=new Quast(new Condition(new Inequation(true)), q, new Quast(1,1,&vc,&v));
// q=new Quast(new Condition(new Inequation(true)), q, new Quast(1,1,&vc,&v));
// q=new Quast(new Condition(new Inequation(true)), q, new Quast(1,1,&vc,&v));
// q=new Quast(new Condition(new Inequation(true)), q, new Quast(1,1,&vc,&v));
// q->Print("\n");
// 
// q=q->Compress();
// 
// q->Print("\n");



if(argc <=1)
   {
   cout<<"No input file...!\n";
   PrintHelp();
   exit(EXIT_SUCCESS);
   }


Options* op=new Options();
ParseOptions(argc,argv,op);
op->printing.human_friendly=false;

string OutPutDirectory=op->io.output_directory;
if(OutPutDirectory == "")
   OutPutDirectory="./";
else{
   OutPutDirectory+="/";
   string command ="mkdir "+OutPutDirectory+ " ";
   system(command.c_str());
   }

if(fopen(op->io.file_name.c_str(),"r")==NULL)
   {
   cout<<"'"<<op->io.file_name<<"' :Invalid input file.\n";
   PrintHelp();
   exit(EXIT_FAILURE);
   }
string file_to_process=op->io.file_name+".preprocessed";
string preprocessing_command="gcc -E --no-line -ansi "+op->io.file_name+" -o "+file_to_process;
system(preprocessing_command.c_str());
op->io.file_name=file_to_process;

Program  __prog;
string ResultName=op->io.output_prefix;

cout<<op->io.file_name<<"\n";
__prog.Parse(op->io.file_name);
__prog.SetOptions(op);

if(ResultName=="")
   ResultName=__prog.GetLabel();



__prog.ComputeSourcesForAllReadVariables();

__prog.PrintNormalized(1);

__prog.SetSyntaxTree(NULL);
__prog.LookForParallelLoops();



if(!op->printing.command_line_printing || op->io.log || op->graph.dg){
   string command="mkdir "+OutPutDirectory+ResultName;
   system(command.c_str());
   op->io.output_directory=OutPutDirectory+ResultName+"/";
   OutPutDirectory=op->io.output_directory;
   }

if(op->io.log)
   __prog.PrintSources(OutPutDirectory+ResultName);

if(!op->printing.command_line_printing){  
   string result=__prog.ToHtml();
   string command= "firefox "+result+"&";
   system(command.c_str());   
   }

if(op->graph.dg){
   string result=__prog.Generate_GraphViz_Graph(OutPutDirectory+ResultName,op->graph.id,op->graph.label,op->graph.statement,op->graph.condition,op->graph.relation,op->printing.human_friendly,op->graph.bottom,true);
   string command= "firefox "+result+"&";
   system(command.c_str());   
   }

cout<<"\n\n ===============================( DONE )=========================\
\n";
/*Graph g;
g.Build(__prog.GetNormalizedStmts());
GraphPrinterOptions gpo;
g.ToVCG("g.vcg",gpo);*/
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////




Program*       _prog;
vector<Control*>* _controls;

extern "C"
{

void fada_init ()
{
   _prog = new Program ();
   *_prog->GetNormalizedStmts() = vector<References*>();
   _prog->SetSyntaxTree(NULL);
   _controls = new vector<Control*>;
   Options* op=new Options();
   op->processing.apply_structural_analysis=true;
   _prog->SetOptions(op);
}

void fada_compute (const char *fct_name)
{

   // TODO: à enlever
   for(vector<References*>::iterator it=_prog->GetNormalizedStmts()->begin(); it != _prog->GetNormalizedStmts()->end(); ++it)
   (*it)->NormalizeDomainConstraints(_prog->GetGlobalParameters());

   _prog->SetLabel(fct_name);

cout << "\nCOMPUTE SOURCES...";
   _prog->ComputeSourcesForAllReadVariables();

cout << "\nEND...";
_prog->PrintNormalized(1);


_prog->LookForParallelLoops();


Graph g;
GraphPrinterOptions op;
op = GraphPrinterOptions();
op.go_after_statements=false;
op.print_rank=true;

g.Build(_prog->GetNormalizedStmts());
g.ToGraphViz(string("output.dot"), op);
//system("dot -Tpng -o output.png  output.dot");
//system("eog output.png");
cout << endl;

delete _prog;
}




/* returns vector<Statement*>*.  */
void *new_statements ()
{
   vector<Statement*> *vect = new vector<Statement*>;
   return vect;
}

/* push STMT into vector<Statement*>* _VECT.  */
void *push_statement (void * _vect, void *stmt)
{
   vector<Statement*> *vect = (vector<Statement*>*) _vect;
   vect->push_back ((Statement*) stmt);   
   return vect;
}


void adaan_add_for_loop (char *name, void *ub)
{
   _controls->push_back (new Control (name, new Expression (0), (Expression*) ub));
}

void adaan_add_while_loop (char *counter, void *cond)
{
   _controls->push_back (new Control (counter, (Condition*) cond));
}

void collect_fada_while_properties(char** counters, int n_counters, char *current_counter, void *while_condition)
{
   vector<string> enclosing_counters;  
   for (int i=0; i<n_counters; i++)
      enclosing_counters.push_back (string(counters[i]));

   LogicalClause* lc = LoopProperty (&enclosing_counters, string(current_counter), (Inequation*)while_condition);
// cout<<"\n ... INEQ :"<<((Inequation*) while_condition)->Print_str();
// cout<<"\n .. enclosing :"<<PrintVector(&enclosing_counters);
// cout<<"\n .. current :"<<current_counter;
// cout<<"\n clause générée :"<<lc->Print_str();
   ((Inequation*)while_condition)->PrintAll (new vector<string>,_prog->GetGlobalParameters());
   _prog->GetGlobalProperties()->PUSHBACK (lc);
}

void *new_adaan_expr_int_cst (long val) {return (void*) new Expression ((int) val);}
void *new_adaan_expr_variable (char *name) {return (void*) new Expression (string(name));}

void tag_fada_inequation (void *ineq, int id, void *iter) {((Inequation*) ineq)->Instanciate(id, (vector<Expression*>*) iter);}
void tag_fada_condition (void *cond, int id, void *iter) {((Condition*) cond)->Instanciate(id, (vector<Expression*>*) iter);}
void tag_fada_written_reference (void *wr, int id, void *iter) {((Written_Reference*) wr)->TagIndex(id, (vector<Expression*>*) iter);}
void tag_fada_read_reference (void *rr, int id, void *iter) {((Read_Reference*) rr)->TagIndex(id, (vector<Expression*>*) iter);}

void adaan_vec_expr_push (void **v, void* expr)
{
   if (expr)
   {
      if (!*v)
         *v = new vector<Expression*>(1,(Expression*) expr);
      else
         ((vector<Expression*>*)(*v))->push_back ((Expression*) expr);
   }
   else if (!*v)
      *v = new vector<Expression*>();
}

void *new_adaan_expr_op (void *e1, T_operation op, void *e2) 
{return (void*) new Expression ((Expression*)e1, op, (Expression*)e2);}

void *new_adaan_expr_call (char *name, void** ptr, int vec_length) 
{
   Expression **exprs = (Expression**) ptr;
   vector<Expression*> *vec = new vector<Expression*>(exprs, exprs + vec_length);
   return new Expression (FADA_function, string (name), vec);
}

void *new_adaan_expr_array (char *name, void* index) 
{
   return new Expression (FADA_array, string (name), ((FADA_Index*) index)->GetIndex());
}

const char *written_reference_name (void *wr)
{
   return ((Written_Reference*) wr)->GetName().c_str();

}

void *new_adaan_index_1 (void* array, int size)
{
   if (size == 0)
      return new FADA_Index ();

   Expression **Array = (Expression**) array;
   vector<Expression*> *vec = new vector<Expression*>(Array, Array + size);
   return new FADA_Index (vec);
}

void *new_fada_index_2 (void* expr)
{
   return new FADA_Index (new vector<Expression*>(1,(Expression*)expr));
}

void *new_fada_written_reference (void *expr)
{
   Expression *E = (Expression*) expr;
   if (E->IsArray() || E->IsVariable())
      return new Written_Reference ((Expression*)expr);
   else 
      return NULL;
}

void *new_fada_read_reference (void *expr)
{
   Expression *E = (Expression*) expr;
   if (E->IsArray() || E->IsVariable())
      return new Read_Reference ((Expression*)expr);
   else 
   {
      E->Print();getchar();
      return NULL;
   }
}

/* Add EXPR to current index. Must be unidimensionnal array.  */
void fada_add_to_index (void *fada_index, void *expr)
{
   FADA_Index *index = (FADA_Index*) fada_index;
   Expression *old_expr = index->GetIndex()->at (0);
   index->GetIndex()->pop_back ();
   index->GetIndex()->push_back (new Expression (old_expr, FADA_ADD, (Expression*) expr));
}

void *new_adaan_stmt (int id, int bb, int *counters, int n_counters, void *context, void *read_references, int n_read_references, void *written_references, int n_written_references)
{
   Condition* domain = (Condition*) context;
   char counter_name[50] = "i";
   vector<string> *vec_counters = new vector<string>();
   Read_Reference **rr = (Read_Reference**) read_references;
   Written_Reference **wr = (Written_Reference**) written_references;

   vec_counters->reserve (n_counters);
   for (int i=0; i < n_counters; i++)
   {
      sprintf (counter_name + strlen(counter_name), "_%d", counters[i]);
      vec_counters->push_back (string (counter_name));
   }


   References* stmt = new References (id, vec_counters, 
         domain, new vector<Read_Reference*> (rr, rr + n_read_references), new vector<Written_Reference*> (wr, wr + n_written_references), NULL);

   _prog->GetNormalizedStmts()->push_back(stmt);

cout<<"\nID    :"<<stmt->GetStmtID();
cout<<"\nCounters :"<<PrintVector(stmt->GetCounters());
cout<<"\nDomain      :"<<stmt->GetDomain()->Print_str();
cout<<"\nWritten variable:";
for(vector<Written_Reference*>::iterator it=stmt->GetWV()->begin(); it != stmt->GetWV()->end(); ++it)
   cout<<" ",(*it)->Print(0);
   
cout<<"\nRead variable  :";
for(vector<Read_Reference*>::iterator it=stmt->GetRV()->begin(); it != stmt->GetRV()->end(); ++it)
   cout<<" ",(*it)->Print(0);
cout<<"\n";

}

void adaan_free_stmts (void)
{
   for (vector<References*>::iterator it=_prog->GetNormalizedStmts()->begin(); it != _prog->GetNormalizedStmts()->end(); ++it)
      delete (*it);  

}
// to remove...
void print_fada_inequation (void *ineq)
{
   ((Inequation*) ineq)->Print();
}

void *new_adaan_inequation (void* expr1, T_predicate p, void* expr2)
   { return new Inequation ((Expression*) expr1, p, (Expression*) expr2);}

void *new_adaan_inequation_value (bool value)
   { return new Inequation (value);}

void *adaan_inequation_negate (void* ineq)
   { return ((Inequation*) ineq)->Negate(true);}

void *new_adaan_condition_leaf_1 (void *ineq)
   {return (void*) new Condition ((Inequation*) ineq);}

void *new_adaan_condition (void* c1, T_Logical_operator op, void* c2)
   {return (void*) new Condition((Condition*)c1, op, (Condition*)c2);}

void *new_adaan_domain_constraint (void *condition)
   {return (void*) new DomainConstraint ((Condition*)condition);}

void *new_adaan_elementary_dependance_1 (int _1, int _2, int d, void *index)
   {return new ElementaryDependence (_1,_2,d,(FADA_Index*) index);}

void adaan_add_schedule (void *read_ref, void *dep)
{
   Read_Reference* ref = (Read_Reference*) read_ref;
   vector<ElementaryDependence*>* vec = ref->GetSchedule();
   vec->push_back ((ElementaryDependence*)dep); 
}


void
fada_add_global_parameter (const char* name)
{
   for (vector<string>::iterator it=_prog->GetGlobalParameters()->begin(); it < _prog->GetGlobalParameters()->end(); it++)
      if (it->compare (name) == 0)
         return;

   _prog->GetGlobalParameters()->push_back(string(name));
}


/* Textual order.  */

static bool *textual_order;
int n_basic_blocks;

void set_textual_order (int n_stmts, bool *order) 
{
   vector<string> common;
   for(int i=0; i < n_stmts; i++){
      vector<bool> local;
      for(int j=0; j < n_stmts; j++)
      {
         if (order[j + i * n_stmts]==1)
            local.push_back(true);
         else
            local.push_back(false);
      }
      _prog->GetSequentialPrecedenceMatrix()->push_back(local);
   }
}
}
