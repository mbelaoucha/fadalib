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

#ifndef GRAPHPRINTER_H
#define GRAPHPRINTER_H

#include <iostream>
#include <cstdlib>

namespace fada{
class GraphPrinterOptions{
public:
bool       show_condition;
bool       show_full_relation;
bool       go_after_statements;
bool       print_label;
bool       print_rank;
bool       print_statement;
std::string   internal_node_prefix;
std::string   sigma;
public:
GraphPrinterOptions(){
   show_condition=true;
   show_full_relation=true;
   go_after_statements=true;
   print_label=false;
   print_rank=true;
   print_statement=true;
   internal_node_prefix="S";
   sigma="Source";
   };
~GraphPrinterOptions(){
   };

};
};
#endif


