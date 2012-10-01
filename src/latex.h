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

#ifndef LATEX_H
#define LATEX_H

#include <string>
#include <inequation.h>
#include <vector>
namespace fada{
class Inequation;

string   TexFigure(string caption,string label,string figure_content);
string   TexSection(string title, string content);
string   TexEqArray(string equations);
string   TexDocument(string document);
string   TexMath(string equations);
string   TexDocumentClass(string options, string doc);
string   TexIndex(string indexed, string index);
string   TexExponent(string base, string exponent);
string   TexPrintDNFTerm(vector<Inequation*>* __v);
string   TexFullPrintDNFTerm(vector<Inequation*>* __v, vector<string>* __var,vector<string>* __param);
string   TexPrintDNFCondition(vector<vector<Inequation*> >* __v);
string   TexFullPrintDNFCondition(vector<vector<Inequation*> >* __v, vector<string>* __var,vector<string>* __param);
string   PreprocessVariableName(string equations);

};
#endif


