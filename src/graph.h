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

//
// C++ Interface: graph
//
// Description: 
//
//
// Author: marouane <bem@pissaro>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GRAPH_H
#define GRAPH_H

/**
   @author marouane <bem@pissaro>
*/


#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include "edge.h"
#include "references.h"
#include "graphprinter.h"

#define Node long unsigned int


using namespace boost;
typedef adjacency_list< listS,vecS, directedS, Node, fada::Edge>    MyGraph;



namespace fada{
class References;
//////////////////////////////////////////////////////////////////////////////:
//////////////////////////////////////////////////////////////////////////////:


//////////////////////////////////////////////////////////////////////////////:
//////////////////////////////////////////////////////////////////////////////:

class Graph{
private:
   MyGraph instance_wise_dataflow_graph;
   graph_traits<MyGraph >::vertex_iterator current_node, first_node, last_node;
   graph_traits<MyGraph>::out_edge_iterator  current_edge,first_edge,last_edge;

   void   InitNodeIterators();
   void   InitEdgeIterators();

   std::string   ToGraphViz(GraphPrinterOptions options);
   std::string   ToVCG(GraphPrinterOptions options);

   void   SetNodes(vector<References*>*   references);
   vector<References*>   nodes;
   
public:

void   NextNode()      {current_node++; InitEdgeIterators();}
bool   HasMoreNodes()      {return current_node!= last_node;}
void   NextEdge()      {current_edge++;}
bool   HasMoreEdges()      {return current_edge!= last_edge;}
void   PreviousNode()      {current_edge--;}
bool   HasPreviousNodes()   {return current_node!= first_node;}
void   PreviousEdge()      {current_edge--;}
bool   HasPreviousEdges()   {return current_edge != first_edge;}


References*   GetNode(int i);
void      AddNode(Node val);
void      AddEdge(Node  source,Node target, Edge* info);

std::string   Print_str(std::string indent);
void      Print(std::string s);
void      Build(vector<References*>* references);


   Graph(){};
    ~Graph(){};
int   Size();
void   ToGraphViz(std::string file_name,GraphPrinterOptions op);
void   ToVCG(std::string file_name,GraphPrinterOptions op);
};

};
#endif


