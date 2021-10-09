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

#ifndef CPW_HANDLERFACTORY_H
#define CPW_HANDLERFACTORY_H


#include <map>
#include <functional>
#include <exception>
#include <vector>

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/StreamCopier.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>

#include "route.h"
#include "error_report.h"
#include "Factory/root_handler.h"
#include "Factory/null_handler.h"
#include "Factory/business_handler.h"
#include "Factory/web_handler.h"

using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Data::Keywords;

namespace CPW
{
	class HandlerConnection;
	class HandlerFactory;
}


class CPW::HandlerConnection
{
	public:
		using HandlerFunctor = std::function<CPW::Factory::RootHandler*()>;

		HandlerConnection(Route route, HandlerFunctor handler);
		~HandlerConnection();

		Route current_route_;
		HandlerFunctor return_handler_;
};

class CPW::HandlerFactory : public HTTPRequestHandlerFactory, public ErrorReport
{
	public:
		enum Endpoint
		{
			kBusiness,
			kAccounts,
			kDeals,
			kPays,
			kCurrency,
			kInventories,
			kInvoices,
			kWeb,
			kNull
		};

		struct EndpointProperties
		{
			std::vector<std::string> segments;
			std::function<CPW::Factory::RootHandler*()> return_handler;
		};

		HandlerFactory();
		virtual ~HandlerFactory();
		virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

	protected:
		void PrepareEndpoints_();
		Endpoint GetEndpoint_(std::vector<std::string> segments);

	private:
		std::map<Endpoint, EndpointProperties> endpoints_handlers_;
		std::string api_version_;
};

#endif // CPW_HANDLERFACTORY_H
