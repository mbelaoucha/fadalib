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

#ifndef HTML_H
#define HTML_H

#include <string>
   std::string Verbatimize(std::string);
   std::string    Linking(std::string hyper_link, std::string target, std::string click_me_message);
   std::string    AddHTMLHeader(std::string title, std::string body);
   std::string    AddHTMLHeader(std::string title, std::string body, bool is_framed);
   std::string    AddHTMLHeader(std::string title,std::string base, std::string body);
   std::string    CreateFrames(std::string pgm,std::string ref,std::string def, std::string page1,std::string page2, std::string page3);
   std::string   ComputeLink(std::string prefix);
   std::string   GenerateHomePage(void);
   std::string   Concat(std::string s, int i);
   std::string   AddHTMLLink(std::string link,std::string text);
#endif


