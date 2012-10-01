/***************************************************************************
 *   Copyright (C) 2008 by Marouane Belaoucha    *
 *   bem@prism.uvsq.fr   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef OPTIONS_H
#define OPTIONS_H
#include <string>
/**
   @author Marouane Belaoucha  <bem@prism.uvsq.fr>
*/

namespace fada{
class FadaProcessingOptions{
public:
bool apply_structural_analysis;
bool apply_iterative_analysis;


void   Default(void){
   apply_structural_analysis=false;
   apply_iterative_analysis=false;
   }
   FadaProcessingOptions(){
            Default();
            };
   ~FadaProcessingOptions(){};

};

class PrintingOptions{
public:
   bool tex_printing;
   bool command_line_printing;
        bool human_friendly;
   bool verbose;



inline void   Default(void){
         tex_printing=false;
         command_line_printing=true;
         human_friendly=true;
         verbose=false;
         }

   PrintingOptions(){
         Default();
         };
   ~PrintingOptions(){};
};

class QuastPrinting{
public:
bool flat;
bool fuzzy;

inline void   Default(void){flat=true; fuzzy=false;}

QuastPrinting(){Default();}
~QuastPrinting(){};
};

class GraphPrinting{
public:
bool dg;
bool id;
bool statement;
bool condition;
bool label;
bool relation;
bool bottom;

inline void Default(void){
      dg=false;
      id=false;
      statement=true;
      condition=false;
      label=false;
      relation=false;
      bottom=false;
      }
GraphPrinting()   {Default();}
~GraphPrinting(){};
};

class InputOutput{
public:
std::string file_name;
std::string output_prefix;
std::string output_directory;
bool   log;

inline void Default(void){
   log=false;
   file_name="";
   output_prefix="";
   output_directory="";
   }
InputOutput(void)   {Default();}
~InputOutput(void)   {};
};

class Options{


public:
   FadaProcessingOptions   processing;
   PrintingOptions      printing;
   QuastPrinting      quast;
   GraphPrinting      graph;
   InputOutput      io;
   Options();
   ~Options();

};
};
#endif



