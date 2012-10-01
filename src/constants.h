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

/*!
\file constants.h
\brief this file contain the definition of some global symbolic constants
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>

//! \brief Prefix used to define the parameters of maximums.
#define FADA_Alpha      "Alpha"
//! \brief Prefix used to parameterize iteration vector
#define FADA_Prefix      "current_"
//! \brief Prefix used to define new variables.
#define FADA_New_var      "FADA_Z"
//! \brief Bottom of quasts.
#define FADA_Bottom      "Bottom"
//#define FADA_TracePrefix   "FADA_Trace__"

//! \brief Prefix used to define
#define FADA_Size      "FADA_Size__"
//! \brief The tracing class name
#define FADA_Tracer_Class   "__Touch_Traces_Stack"
//! \brief The name of the tracer.
#define FADA_Tracer_Prefix   "__FADA_Tracer_"

//! \brief Method for the tarcing class
#define FADA_Upper_bound   "AdaAn_Up__"
//! \brief Method for the tarcing class
#define FADA_Lower_bound   "AdaAn_Low__"
//! \brief Tracing function prototype
#define FADA_Tracer_Prototype   "int __FADA_Trace_Function(void)"


//! \brief Prefix used to define vitual loop counters for while loops
#define __FADA_WHILE "_W_"
////! \brief FADA version
//#define __FADA_VERSION "fada-0.1.7"

//! \brief constant used to secure print for some printing functions
#define __NORMAL_PRINT 0
//! \brief constant used to secure print for some printing functions
#define __SECURE_PRINT 0
//! \brief constant used for a fully print.
#define __PRINT_ALL 1
//! \brief the output file name extention for GraphViz/dot compilation
#define _dot_out_format_ "png"
#define __html_reference "_reference_"

#define __fada_version "fada-0.1.12"








// Exception

// potential bugs 
#define NON_AFFINE_ENTITY      0
#define NON_AFFINE_EXPRESSION      1
#define BAD_OPERATOR         2
#define UNHANDLED_CASE         3
#define INAPPROPRIATE_CASE      4
#define BAD_OBJECT         5







#endif



