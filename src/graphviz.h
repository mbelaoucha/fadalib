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

#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include <string>
std::string AddDOTHeader(bool directed, std::string body);
std::string AddDOTNode(std::string node_name, std::string label);
std::string AddDotSubGraph(std::string __node_name,std::string __label,std::string __shape,std::string __edges,std::string bg_color);
std::string AddDOTNode(std::string node_name, std::string label,std::string color);
std::string AddDotEdge(bool directed, std::string parent, std::string child, std::string label);


#endif


