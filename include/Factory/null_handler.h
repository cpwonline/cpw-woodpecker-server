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

#ifndef CPW_FACTORY_NULLHANDLER_H
#define CPW_FACTORY_NULLHANDLER_H


#include "Factory/root_handler.h"

namespace CPW
{
	namespace Factory
	{
		class NullHandler;
	}
}


class CPW::Factory::NullHandler : public RootHandler
{
	public:
		using RootHandler::RootHandler;
		virtual ~NullHandler();

	protected:
		virtual void HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void AddRoutes_() override;
};

#endif // CPW_FACTORY_NULLHANDLER_H
