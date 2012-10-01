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

#include "graphviz.h"

std::string
AddDOTHeader(bool directed, std::string body){
std::string result="digraph G {\n\tbgcolor=azure;\n\tnode [shape=polygon, sides=6, color=lightblue2, style=filled];\n\tedge [arrowsize=2, color=gold];";
result += body;
result += "\n}";

return result;
}


std::string 
AddDOTNode(std::string node_name, std::string label){
std::string result;
result="\n"+node_name+" [ label=\""+ label + "\"];";
return result;
}

std::string 
AddDOTNode(std::string node_name, std::string label,std::string color){
std::string result;
result="\n"+node_name+" [ label=\""+ label + "\",color="+color+"];";
return result;
}

std::string
AddDotSubGraph(std::string __node_name,std::string __label,std::string __shape,std::string __edges,std::string bg_color,std::string __code){
std::string result="subgraph "+__node_name+"{";
result+="\nlabel=\""+__label+"\";";
result+=__code;
result+="\n}";
return result;
}

std::string
AddDotEdge(bool directed, std::string parent, std::string child, std::string label){
std::string result;
if(directed)
   result="\n"+parent + " -> " + child ;
else
   result="\n"+parent + " - " + child ;

result+=" [label =\""+label+"\"];";
return result;
}


