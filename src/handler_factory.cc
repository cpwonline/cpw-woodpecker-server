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

#include "handler_factory.h"

using namespace CPW;

HandlerConnection::HandlerConnection(Route route, HandlerFunctor handler) :
	current_route_(route)
	,return_handler_(handler)
{

}

HandlerConnection::~HandlerConnection()
{

}

HandlerFactory::HandlerFactory() :
	api_version_("v0")
	,app_(Application::instance())
{
	CreateConnections_();
}

HandlerFactory::~HandlerFactory()
{
	for(auto it = connections_.begin(); it != connections_.end(); ++it)
		delete it->second;
}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
	try
	{
		std::vector<std::string> segments;

		URI(request.getURI()).getPathSegments(segments);

		Route requested_route("", segments);

		switch(requested_route.get_current_route_type())
		{
			case RouteType::kEndpoint:
			{
				return connections_[HandlerType::kBackend]->return_handler_();
				break;
			}
			case RouteType::kEntrypoint:
			{
				return connections_[HandlerType::kFrontend]->return_handler_();
				break;
			}
		}
	}
	catch (std::exception const& error)
	{
		app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
	}

	return connections_[HandlerType::kNull]->return_handler_();
}

void HandlerFactory::CreateConnections_()
{
	connections_.insert(std::make_pair
	(
		HandlerType::kNull,
		new HandlerConnection
		{
			Route("null", std::vector<std::string>{""})
			,[&](){return new CPW::Factory::NullHandler(api_version_);}
		}
	));
	connections_.insert(std::make_pair
	(
		HandlerType::kBackend,
		new HandlerConnection
		{
			Route("", std::vector<std::string>{"api", api_version_})
			,[&](){return new CPW::Factory::BackendHandler(api_version_);}
		}
	));
	connections_.insert(std::make_pair
	(
		HandlerType::kFrontend,
		new HandlerConnection
		{
			Route("", std::vector<std::string>{""})
			,[&](){return new CPW::Factory::FrontendHandler(api_version_);}
		}
	));
}
