/*
 * CPW Woodpecker Server
 * Copyright (C) 2021 CPW Online support@cpwonline.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPW_QUERYACTIONS_H
#define CPW_QUERYACTIONS_H


#include <string>

#include <Poco/Net/HTTPServerRequest.h>


namespace CPW
{
	class QueryActions;
}

using namespace Poco::Net;

class CPW::QueryActions
{
	public:
		QueryActions();
		~QueryActions();

	protected:
		void IdentidyFilter_(const HTTPServerRequest& request);
		void CreateRows_();
		void ComposeQuery_(const HTTPServerRequest& request);

	private:
		bool iqual_;
		bool sort_;
		bool page_;
		bool limit_;
		std::string final_query_;
};


#endif // CPW_QUERYACTIONS_H