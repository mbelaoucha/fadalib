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

#include "read_reference.h"
#include <constants.h>
#include <global.h>
#include <html.h>
#include <graphviz.h>
#include <latex.h>

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
namespace fada{

void
Read_Reference::Set(Expression* __expr){
if(__expr->IsArray()){
        Set(__expr->GetArrayName(), new FADA_Index(__expr->GetIndex()));
        return;
        }
if(__expr->IsVariable()){
        Set(__expr->GetVariableName(), new FADA_Index());
   return;
        }
cerr<<"\nRead_Reference::Set(Expression* __expr) ... inappropriate case \n";
throw(10);
}

Read_Reference::Read_Reference(Expression* __expr){
Initialize();
Set(__expr);
}



////////////////////////////////////////////////////////
Read_Reference::Read_Reference()
{
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

Read_Reference::~Read_Reference()
{
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////


Read_Reference::Read_Reference(string __str)
{
Initialize();
Set(__str);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

Read_Reference::Read_Reference(string __str, FADA_Index* __ind)
{
Initialize();
Set(__str, __ind);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

Read_Reference::Read_Reference(string __str, FADA_Index*__ind,Quast* __def)
{
Initialize();
Set(__str, __ind, __def);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
Read_Reference::Set(string __str)
{
SetArrayName(__str);
SetArrayIndex(new FADA_Index());
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
Read_Reference::Set(string __str, FADA_Index* __ind)
{
SetArrayName(__str);
SetArrayIndex(__ind);
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
Read_Reference::Set(string __str, FADA_Index* __ind, Quast* __def)
{
SetArrayName(__str);
SetArrayIndex(__ind);
SetDefinition(__def);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
Read_Reference::Initialize()
{
properties_on_parameter= new LDemonstrator();
SetArrayName("");
SetArrayIndex(NULL);
SetDefinition(NULL);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

bool
Read_Reference::operator==(Read_Reference* __ref)
{
return (GetArrayName() == __ref->GetArrayName()) && (__ref->GetArrayIndex()->Print_str() == GetArrayIndex()->Print_str());
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

bool
Read_Reference::operator==(string __str)
{
return GetArrayName() == __str;
}


////////////////////////////////////////////////////////
string
Read_Reference::Print_str(int __level,Options* op){
string   result=GetArrayName()+GetArrayIndex()->Generate_C_Code();
if(__level == 0)
   return   result;
vector<string> s;
result+=this->GetDefinition()->Print_str("\n\t");
if(GetParameterOfMaximum()->size()>0){
   result+="\nWith :";
   result+="\n======\n";
   }

for(vector<ElementaryDependence*>::iterator it=GetSchedule()->begin(); it != GetSchedule()->end();++it){
   if((*it)->GetQuast()==NULL)
      break;
   result+="\n"+(*it)->PrintProperties();
   }
if(op->printing.verbose){
   result+="\n\nInference Stack :";
   result+="\n===================";
   result+="\n"+GetProperties()->Print_str();
   }
return result;
}
////////////////////////////////////////////////////////

string
Read_Reference::Print_str(int level)
{
string   result=GetArrayName()+GetArrayIndex()->Generate_C_Code();
if(level == 0)
   return   result;

// if(GetSchedule()->size() >0)
//    result+="taille non nulle";
// else
//    result+="taille  nulle";
// 
// for(vector<ElementaryDependence*>::iterator it=GetSchedule()->begin(); it != GetSchedule()->end(); ++it)
//    result+="\n"+(*it)->Print_str(0);

result+=this->GetDefinition()->Print_str("\n\t")+"\n\n"+GetProperties()->Print_str();
if(level == 1)
   return result+"\n";

return result;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
Read_Reference::Print(int level)
{
cout<<Print_str(level);
}


void 
Read_Reference::ToHtml(string dir, string name_prefix,Options* op){
string   full_file_name=dir+name_prefix+".html";
WriteToFile(full_file_name,ToHtml(dir,name_prefix,op, true));
}
////////////////////////////////////////////////////////
string 
Read_Reference::ToHtml(string dir, string name_prefix,Options* op, bool __useless){
ostringstream html;

bool tex=op->printing.tex_printing;
bool dot=!tex;
bool flat=op->quast.flat;
bool fuzzy=op->quast.fuzzy;
bool ppts=op->printing.verbose;

if(dot && !flat){
   string   quast_dot=name_prefix+"_def.dot";
   string   full_quast_name=dir+"images/"+quast_dot;
   
   string   quast_img=name_prefix+"_def."_dot_out_format_;
   string   full_img_name=dir+"images/"+quast_img;
   
   GetDefinition()->ToGraphVizFile(full_quast_name);
   
   
   string command="dot -T"_dot_out_format_" "+full_quast_name+" -o "+full_img_name;
   
   system(command.c_str());
   

   html<<"<H3> Definition of : "<<GetArrayName()<< GetArrayIndex()->Generate_C_Code() <<"</H3>";
   html<<   "<HR WIDTH=\"40%\" COLOR=\"#FF0000\" SIZE=\"4\">";
   html<<"<img src=\"./images/"+quast_img+"\" size=\"100%\" >";
   }
else{
   if(dot && flat){
      string   quast_dot=name_prefix+"_def.dot";
      string   full_quast_name=dir+"images/"+quast_dot;
      
      string   quast_img=name_prefix+"_def."_dot_out_format_;
      string   full_img_name=dir+"images/"+quast_img;
      
      FlatQuast* flat_quast=new FlatQuast();
      GetDefinition()->Flatten(flat_quast);
      if(fuzzy)
         flat_quast->RemoveConstraintsOnAlphas(GetParameterOfMaximum());

      flat_quast->ToGraphVizFile(full_quast_name);
      
      string command="dot -T"_dot_out_format_" "+full_quast_name+" -o "+full_img_name;
      
      system(command.c_str());
         
      html<<"<H3> Definition of : "<<GetArrayName()<< GetArrayIndex()->Generate_C_Code() <<"</H3>";
      html<<   "<HR WIDTH=\"40%\" COLOR=\"#FF0000\" SIZE=\"4\">";
      html<<"<img src=\"./images/"+quast_img+"\" size=\"100%\" >";
      }
   else
      if(tex){
         string   quast_tex=name_prefix+"_def.tex";
         string   full_tex_file_name=dir+quast_tex;
         
         string   relative_link=name_prefix+"_def/index.html";
         string   full_html_page_link=dir+relative_link;
         
         FlatQuast* flat_quast=new FlatQuast();
         GetDefinition()->Flatten(flat_quast);
         if(fuzzy)
            flat_quast->RemoveConstraintsOnAlphas(GetParameterOfMaximum());


         string tex_content=flat_quast->Print_Tex_str();

         if(GetProperties()->Size() > 0){
            tex_content+="With :\\";
            tex_content+=Full_Tex_Print_str();
            }

         if(ppts){
            tex_content+="\n\n\n************************ Logical Demonstrator Stack **************************************\n";
            tex_content+=TexEqArray(GetProperties()->Full_Tex_Print_str(GetAllParameters()));

            }
            
         tex_content=TexDocument(tex_content);
         tex_content=TexDocumentClass("",tex_content);
         WriteToFile(full_tex_file_name,tex_content);

         
         string command="latex2html -no_navigation "+full_tex_file_name+">"+full_tex_file_name+".noise";
         
         system(command.c_str());

         html<<"<H3> Definition of : "<<GetArrayName()<< GetArrayIndex()->Generate_C_Code() <<"</H3>";
         html<<   "<HR WIDTH=\"40%\" COLOR=\"#FF0000\" SIZE=\"4\">";
         html<<AddHTMLLink(relative_link,"clic here");
   
         }
      else{
         cerr<<"\nRead_Reference::ToHtml ..........inappropriate case\n";
         throw(INAPPROPRIATE_CASE);
         exit(EXIT_FAILURE);
         }
   }


/*
if(GetParameterOfMaximum()->size() > 0){
   html<<"\n<H3> Properties On Parameters ...</H3>";
   html<<   "<HR WIDTH=\"40%\" COLOR=\"#FF0000\" SIZE=\"4\">";
   
   string   prop_dot=name_prefix+"_prop.dot";
   string   prop_img=name_prefix+"_prop."_dot_out_format_;
   string   full_prop_name=dir+"images/"+prop_dot;
   string   full_prop_img=dir+"images/"+prop_img;
   
   string __properties;

   __properties=AddDOTNode("root",__properties);
   __properties=AddDOTHeader(true,__properties);


   WriteToFile(full_prop_name,__properties);

   string command="dot -T"_dot_out_format_" "+full_prop_name+" -o "+full_prop_img;
   
   system(command.c_str());


   html<<"<img src=\"./images/"+prop_img+"\" size=\"100%\" >";
   }*/

return AddHTMLHeader("coucou", html.str());

}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

string
Read_Reference::Generate_C_Test_Code(string prefix)
{
return this->GetDefinition()->Generate_C_Test_Code(this->GetArrayName(), this->GetArrayIndex(), prefix);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
Read_Reference::ReadScalars(set<string>* __read){
if(GetArrayIndex()->GetSize()==0)
   __read->insert(GetArrayName());
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
Read_Reference::ComputeTheDefinition(
               // global
            vector<string>*       __param,
            void      *       __textual_prec,
            int   (*fct)(void*,int,int),
            LDemonstrator*         __global_properties,
            vector<References*>*      __references,
               // parameterized read
            vector<string>*       __parameterized_read_iteration,
            map<string,Expression*>*   __map_to_parameterized_iteration,
            vector<vector<Inequation*> >*    __env,
            vector<vector<Inequation*> >*    __non_affine_env,
               // options
            Options* options
            ){
int last_deep=10000000;   
   // in all cases, PIP seems to not be able to solve systems with more than 36 variables for complexe systems(unless we increase the size of allocated memory)
   //FIXME: may be i should use a constant


SetGlobalParameters(__param);
SetAllParameters(__param);
GetAllParameters()->push_back(FADA_Bottom);
FADA_Index* parameterized_index=GetArrayIndex()->Substitute(__map_to_parameterized_iteration);
string   conflictual_array=GetArrayName();


Quast* result=new Quast();      // _|_
Quast* local_result=new Quast();   // _|_
bool recover_non_affine_constraints=options->processing.apply_structural_analysis;

for(vector<ElementaryDependence*>::iterator it=GetSchedule()->begin(); it != GetSchedule()->end(); ++it){
   if((*it)->GetDeep() < last_deep){
      if(!local_result->ContainBottoms()){   // no bottoms => an eventual definition reached
         result=result->PutToBottoms(local_result);

         result=result->Simplify(*__env,GetAllParameters())->Compress();

         SetDefinition(result);
         return;
         }
      else{
         if(GetParameterOfMaximum()->size() != 0 && recover_non_affine_constraints){
                  // FIXME : here i've to check only parameters coming from previous level

             Quast* res=local_result;

             GetProperties()->EliminateDoubles();

             if(GetProperties()->Inference(GetAllParameters())){


               ContextQuast* quast=GetProperties()->Traduce2ContextQuast(__env,GetAllParameters());

                  //FIXME


               quast=quast->Simplify(*__env,GetAllParameters());
                  //FIXME
//                res=res->Uncompress();
               res=res->Simplify(quast,*__env,GetAllParameters());


               if(!res->ContainBottoms()){
                  result=result->PutToBottoms(res);
                  result=result->Simplify(*__env,GetAllParameters())->Compress();
                  SetDefinition(result);
                  
                  return;
                  }
               else
                  local_result=res;


               }
            }

         result=result->PutToBottoms(local_result);
         last_deep=(*it)->GetDeep();
         local_result=new Quast();
         GetProperties()->CLEAR();
         }
      }
   

      
   (*it)->BuildConstraints(
      GetGlobalParameters(),
      __references,
      __parameterized_read_iteration,
      __env,
      __non_affine_env,
      conflictual_array,
      parameterized_index);

   (*it)->ComputeTheMaximum(__global_properties,options);

   for(vector<string>::iterator iti=(*it)->GetParameterOfTheMaximum()->begin(); iti != (*it)->GetParameterOfTheMaximum()->end(); ++iti){
      GetAllParameters()->push_back(*iti);
      GetParameterOfMaximum()->insert(*iti);
      }
   GetProperties()->Merge((*it)->GetProperties());
   local_result=Max(local_result,(*it)->GetQuast(),__references,__textual_prec,fct,*__env,GetAllParameters());
   //FIXME:
   }
if(GetParameterOfMaximum()->size() != 0 && recover_non_affine_constraints){
   vector<Inequation*> empty;
   Quast* res=local_result;
   GetProperties()->EliminateDoubles();
   GetProperties()->Inference(GetAllParameters());
   ContextQuast* quast=GetProperties()->Traduce2ContextQuast(__env,GetAllParameters());

   res=res->Uncompress();
   res=res->Simplify(quast,*__env,GetAllParameters());

   if(!res->ContainBottoms()){
      result=result->PutToBottoms(res)->Compress();
      SetDefinition(result);
      return;
      }
   else
      local_result=res;
   
   }

result=result->PutToBottoms(local_result);
result=result->EliminateRedondantLeaves(true);

result=result->Simplify(*__env,GetAllParameters())->Compress();

this->SetDefinition(result);

}

////////////////////////////////////////////////////////
void
Read_Reference::ComputeTheDefinition(
               // global
            vector<string>*       __param,
            vector<vector<bool> >*       __textual_prec,
            LDemonstrator*         __global_properties,
            vector<References*>*      __references,
               // parameterized read
            vector<string>*       __parameterized_read_iteration,
            map<string,Expression*>*   __map_to_parameterized_iteration,
            vector<vector<Inequation*> >*    __env,
            vector<vector<Inequation*> >*    __non_affine_env,
               // options
            Options* options
            ){


int last_deep=10000000;   
   // in all cases, PIP seems to not be able to solve systems with more than 36 variables for complexe systems(unless we increase the size of allocated memory)
   //FIXME: may be i should use a constant



SetGlobalParameters(__param);
SetAllParameters(__param);
GetAllParameters()->push_back(FADA_Bottom);
FADA_Index* parameterized_index=GetArrayIndex()->Substitute(__map_to_parameterized_iteration);
string   conflictual_array=GetArrayName();


Quast* result=new Quast();      // _|_
Quast* local_result=new Quast();   // _|_
bool recover_non_affine_constraints=options->processing.apply_structural_analysis;

for(vector<ElementaryDependence*>::iterator it=GetSchedule()->begin(); it != GetSchedule()->end(); ++it){
   if((*it)->GetDeep() < last_deep){
      if(!local_result->ContainBottoms()){   // no bottoms => an eventual definition reached

         local_result=local_result->Simplify(*__env,GetAllParameters());
         local_result=local_result->EliminateRedondantLeaves(true);
         result=result->PutToBottoms(local_result->Compress());
         SetDefinition(result->Compress());
         return;
         }
      else{
         if(GetParameterOfMaximum()->size() != 0 && recover_non_affine_constraints){
                  // FIXME : here i've to check only parameters coming from previous level

             Quast* res=local_result;

             GetProperties()->EliminateDoubles();

             if(GetProperties()->Inference(GetAllParameters())){

               ContextQuast* quast=GetProperties()->Traduce2ContextQuast(__env,GetAllParameters());

               quast=quast->Simplify(*__env,GetAllParameters());

               res=res->Simplify(quast,*__env,GetAllParameters());


               if(!res->ContainBottoms()){
                  result=result->PutToBottoms(res->Compress());
                  SetDefinition(result);
                  return;
                  }
               else
                  local_result=res;
               }
            }
         local_result=local_result->Simplify(*__env,GetAllParameters());
         result=result->PutToBottoms(local_result->Compress());
         last_deep=(*it)->GetDeep();
         local_result=new Quast();
         GetProperties()->CLEAR();
         }
      }
   

      
   (*it)->BuildConstraints(
      GetGlobalParameters(),
      __references,
      __parameterized_read_iteration,
      __env,
      __non_affine_env,
      conflictual_array,
      parameterized_index);

   (*it)->ComputeTheMaximum(__global_properties,options);

   for(vector<string>::iterator iti=(*it)->GetParameterOfTheMaximum()->begin(); iti != (*it)->GetParameterOfTheMaximum()->end(); ++iti){
      GetAllParameters()->push_back(*iti);
      GetParameterOfMaximum()->insert(*iti);
      }
   GetProperties()->Merge((*it)->GetProperties());
   local_result=Max(local_result,(*it)->GetQuast()/*->Compress()*/,__references,__textual_prec,*__env,GetAllParameters());
   //FIXME:
   }
if(GetParameterOfMaximum()->size() != 0 && recover_non_affine_constraints){
   vector<Inequation*> empty;
   Quast* res=local_result;

   GetProperties()->EliminateDoubles();
   GetProperties()->Inference(GetAllParameters());
   ContextQuast* quast=GetProperties()->Traduce2ContextQuast(__env,GetAllParameters());

   res=res->Uncompress();
   res=res->Simplify(quast,*__env,GetAllParameters());
   

   if(!res->ContainBottoms()){
      result=result->PutToBottoms(res->Compress());
      SetDefinition(result);
      return;
      }
   else
      local_result=res;
   }
local_result=local_result->Simplify(*__env,GetAllParameters());
result=result->PutToBottoms(local_result->Compress());
// result=result->EliminateRedondantLeaves(true);
// result=result->Simplify(*__env,GetAllParameters());
this->SetDefinition(result);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void   
Read_Reference::CollectParameterOfTheMaximumValues(vector<string> loops){
map<string,string> map;

for(vector<ElementaryDependence*>::iterator it=GetSchedule()->begin(); it!=GetSchedule()->end() && (*it)->GetQuast() != NULL;++it){
   (*it)->CollectMaximumOfParameterValues(&map,loops);
   }
if(map.size()>0)
   GetDefinition()->SubstituteMaximumOfParameterValues(map);
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

string
Read_Reference::PrettyPrint_str(void){
return    GetArrayName()+GetArrayIndex()->Generate_C_Code();
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
string
Read_Reference::Full_Tex_Print_str(void){
string result="";
for(vector<ElementaryDependence*>::iterator it=GetSchedule()->begin(); it != GetSchedule()->end();++it){
   if((*it)->GetQuast()==NULL)
      break;
   result+=TexFigure("","",(*it)->Full_Tex_Print_str());
   }
return result;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////


void
Read_Reference::TagIndex(int __stmt, vector<Expression*>* __iter)   {
GetArrayIndex()->Instanciate(__stmt,__iter);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void
Read_Reference::LookForParallelism(vector<string> original, vector<string>* parallel,vector<References*> program){
GetDefinition()->LookForLoopParallelism(original,parallel,program);
}

}
///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////  END  ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////



