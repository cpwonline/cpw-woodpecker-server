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

#include "Factory/frontend_handler.h"

using namespace CPW::Factory;

FrontendHandler::FrontendHandler(std::string api_version) :
	RootHandler(api_version)
{
	supported_files_ = new std::map<std::string, FileProperties>;
	AddSupportedFiles_();
}

FrontendHandler::~FrontendHandler()
{
	delete supported_files_;
}

void FrontendHandler::AddRoutes_()
{
	get_routes_list().push_back(new Route("",std::vector<std::string>{""}));
}

void FrontendHandler::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	if(!IsSupported_(request.getURI()))
	{
		std::cout << "\nFile not supported";
	}
	if(!CheckFile_(request.getURI()))
	{
		std::cout << "\nFile bad check";
	}

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	out << "{GET}";
	out.flush();
}

void FrontendHandler::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{

}

void FrontendHandler::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{

}

void FrontendHandler::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{

}

void FrontendHandler::AddSupportedFiles_()
{
	supported_files_->insert(std::make_pair
	(
		".html",
		FileProperties{"text/html", false, {".xhtml", ".htm", ".html5"}}
	));
	supported_files_->insert(std::make_pair
	(
		".js",
		FileProperties{"text/javascript", false, {".html", ".htm", ".html5"}}
	));
	supported_files_->insert(std::make_pair
	(
		".css",
		FileProperties{"text/css", false, {".html", ".htm", ".html5"}}
	));
}

bool FrontendHandler::IsSupported_(std::string path)
{
	Path requested_path(path);
	if(requested_path.isDirectory())
		requested_path.setFileName("index.html");

	auto file_found = supported_files_->find(requested_path.getExtension());
	if(file_found != supported_files_->end())
	{
		return true;
	}
	else
	{
		for(auto& it : *supported_files_)
		{
			auto other_extensions = &it.second.other_extensions;
			auto find_depth = std::find(other_extensions->begin(), other_extensions->end(), requested_path.getExtension());
			if(find_depth != other_extensions->end())
				return true;
		}
		return false;
	}
}

bool FrontendHandler::CheckFile_(std::string path)
{
	Path requested_path(path);
	if(requested_path.isDirectory())
		requested_path.setFileName("index.html");

	File requested_file(requested_path);

	if(!requested_file.canRead())
		return false;
	else if(!requested_file.exists())
		return false;
	else
		return true;
}