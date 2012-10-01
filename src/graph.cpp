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
#include <cstdlib>

#include "graph.h"
#include "global.h"
namespace fada{
//////**********************************************************************
//////**********************************************************************

void
Graph::InitNodeIterators(){
   tie(first_node, last_node) = vertices(instance_wise_dataflow_graph);
   current_node=first_node;
   InitEdgeIterators();
   };
//////**********************************************************************
//////**********************************************************************

void   
Graph::InitEdgeIterators(){
   if(current_node != last_node){
      tie(first_edge,last_edge)=out_edges(*current_node, instance_wise_dataflow_graph);
         current_edge=first_edge;
      }
   };
//////**********************************************************************
//////**********************************************************************

void
Graph::AddNode(Node val){
   instance_wise_dataflow_graph[add_vertex(instance_wise_dataflow_graph)]=val;
}
//////**********************************************************************
//////**********************************************************************

void
Graph::AddEdge(Node  source,Node target, Edge* info){
    add_edge(source,target,*info,instance_wise_dataflow_graph);
   };

//////**********************************************************************
//////**********************************************************************

std::string
Graph::Print_str(std::string indent)
   {
   ostringstream sout;
   InitNodeIterators();
   while(HasMoreNodes()){
      sout<<indent<<"\n N° = "<<*current_node;
      
      while(HasMoreEdges()){
         sout<<indent<<"\n      ----> "<<instance_wise_dataflow_graph[*current_edge].Print_str(indent+"      ");
         NextEdge();
         }
      NextNode();
      }
   return sout.str();
   };
//////**********************************************************************
//////**********************************************************************

void
Graph::Print(std::string s)      
   {std::cout<<Print_str(s);};

//////**********************************************************************
//////**********************************************************************

int
Graph::Size()   {return num_vertices(instance_wise_dataflow_graph);}

//////**********************************************************************
//////**********************************************************************


std::string
Graph::ToVCG(GraphPrinterOptions options){
ostringstream s;
///////////// Nodes Generation ///////////////
graph_traits<MyGraph>::vertex_iterator it_n, last_n;
tie(it_n,last_n)=vertices(instance_wise_dataflow_graph);
for(; it_n!=last_n;++it_n)
   s<<GetNode(instance_wise_dataflow_graph[*it_n])->GenerateVCGNodes(options);

///////////// edges Generation ///////////////
graph_traits<MyGraph>::edge_iterator it_e, last_e;
tie(it_e,last_e)=edges(instance_wise_dataflow_graph);
for(;it_e!=last_e;++it_e){
   s<<"\n edge: { sourcename: \"";
   s<<options.internal_node_prefix<<instance_wise_dataflow_graph[source(*it_e,instance_wise_dataflow_graph)]<<"\" targetname: \"" << options.internal_node_prefix<<instance_wise_dataflow_graph[target(*it_e,instance_wise_dataflow_graph)] <<"\" label: \"";

   unsigned long read,write;
   vector<string>   read_iteration;
   read=instance_wise_dataflow_graph[target(*it_e,instance_wise_dataflow_graph)];
   write=instance_wise_dataflow_graph[source(*it_e,instance_wise_dataflow_graph)];
   read_iteration=*(GetNode(read)->GetCounters());

   s<<instance_wise_dataflow_graph[*it_e].ToGraphViz(read,read_iteration,write,options);
   s<<"\"}";
   }

string result="\n  graph: {";
result+="display_edge_labels: yes\nnode.shape: ellipse\nnode.color:white\n"+s.str()+"\n}";
return result;
}

void
Graph::ToVCG(std::string file_name,GraphPrinterOptions op){
WriteToFile(file_name,ToVCG(op));
}

//////**********************************************************************
//////**********************************************************************

std::string
Graph::ToGraphViz(GraphPrinterOptions options){
ostringstream s;
///////////// Nodes Generation ///////////////
graph_traits<MyGraph>::vertex_iterator it_n, last_n;
tie(it_n,last_n)=vertices(instance_wise_dataflow_graph);
for(; it_n!=last_n;++it_n)
   s<<GetNode(instance_wise_dataflow_graph[*it_n])->GenerateGraphVizNodes(options);
///////////// edges Generation ///////////////
graph_traits<MyGraph>::edge_iterator it_e, last_e;
tie(it_e,last_e)=edges(instance_wise_dataflow_graph);
for(;it_e!=last_e;++it_e){
   s<<"\n";
   s<<options.internal_node_prefix<<instance_wise_dataflow_graph[source(*it_e,instance_wise_dataflow_graph)]<<" -> " << options.internal_node_prefix<<instance_wise_dataflow_graph[target(*it_e,instance_wise_dataflow_graph)] <<" [ label =\"";
//    s<<instance_wise_dataflow_graph[*it_e].
   unsigned long read,write;
   vector<string>   read_iteration;
   read=instance_wise_dataflow_graph[target(*it_e,instance_wise_dataflow_graph)];
   write=instance_wise_dataflow_graph[source(*it_e,instance_wise_dataflow_graph)];
   read_iteration=*(GetNode(read)->GetCounters());

   s<<instance_wise_dataflow_graph[*it_e].ToGraphViz(read,read_iteration,write,options);
   s<<"\"];\n";


   }

string result="\n  digraph G { \n\
       bgcolor=azure; \n \
        node [shape=polygon, sides=6, color=lightblue2, style=filled]; \n\
        edge [arrowsize=2, color=gold]; \n"+s.str()+"\n}";

return result;
}

//////**********************************************************************
//////**********************************************************************

void
Graph::SetNodes(vector<References*>*   references){
nodes=*references;
};

//////**********************************************************************
//////**********************************************************************

References*
Graph::GetNode(int i){
if(i > nodes.size()){
   
   cout<<"\n Graph::GetNode .... requested node is out of range "<<i<<"\n";
   throw(INAPPROPRIATE_CASE);
   }
return this->nodes[i];
}
//////**********************************************************************
//////**********************************************************************

void
Graph::ToGraphViz(std::string file_name,GraphPrinterOptions op){
WriteToFile(file_name,ToGraphViz(op));
}

//////**********************************************************************
//////**********************************************************************

void
Graph::Build(vector<References*>* references){
SetNodes(references);
// AddNode(-1);   // for bottom  '_|_'   (not initialized array cell

for(int i=0; i<references->size();i++){
//    cout<<"\n i = "<<i<<"\n";
   AddNode(GetNode(i)->GetStmtID());
   };
for(vector<References*>::iterator it=references->begin(); it!= references->end(); ++it){
   References* ref;
   (*it)->BuildInstanceWiseEdges(this);
   }
}

//////**********************************************************************
//////**********************************************************************

};  // END NAMESPACE


