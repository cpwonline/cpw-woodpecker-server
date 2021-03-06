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


#include <istream>
#include <string>
#include <map>
#include <stdexcept>

#include "Poco/Util/Application.h"
#include <Poco/Net/HTTPServerRequest.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/RecordSet.h>


namespace CPW
{
	enum class TypeAction;
	enum class TypeQuery;
	class ManageJSON;
	class Filters;
	class QueryActions;
}

using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;


enum class CPW::TypeAction
{
	kInsert
	,kSelect
	,kUpdate
	,kDelete
};

enum class CPW::TypeQuery
{
	kFields
	,kPage
	,kLimit
	,kSort
	,kIqual
	,kNotIqual
	,kGreatherThan
	,kSmallerThan
	,kBetween
	,kIn
	,kNotIn
	,kValues
	,kSet
};

class CPW::ManageJSON
{
	public:
		ManageJSON();
		~ManageJSON();

		Dynamic::Struct<std::string>& get_dynamic_json_body()
		{
			Dynamic::Struct<std::string>& d = dynamic_json_body_;
			return d;
		}

		std::string ReadBody_(std::istream& stream);
		bool Parse_(std::string string_to_parse);

	protected:
		bool VerifyJSON_();

	private:
		Dynamic::Struct<std::string> dynamic_json_body_;
};

class CPW::Filters
{
	public:
		Filters();
		~Filters();

		std::vector<std::string>& get_fields()
		{
			auto& var = fields_;
			return var;
		}
		std::string get_page() const { return page_; }
		std::string get_limit() const { return limit_; }
		std::vector<std::string>& get_sorts_conditions()
		{
			auto& var = sorts_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_iquals_conditions()
		{
			auto& var = iquals_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_not_iquals_conditions()
		{
			auto& var = not_iquals_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_greather_than()
		{
			auto& var = greather_than_;
			return var;
		}
		std::map<std::string, std::string>& get_smaller_than()
		{
			auto& var = smaller_than_;
			return var;
		}
		std::map<std::string, std::pair<std::string, std::string>>& get_between()
		{
			auto& var = between_;
			return var;
		}
		std::map<std::string, std::vector<std::string>>& get_in()
		{
			auto& var = in_;
			return var;
		}
		std::map<std::string, std::vector<std::string>>& get_not_in()
		{
			auto& var = not_in_;
			return var;
		}
		std::vector<std::vector<std::string>>& get_values()
		{
			auto& var = values_;
			return var;
		}
		std::map<std::string, std::string>& get_set()
		{
			auto& var = set_;
			return var;
		}

		void set_page(std::string page) { page_ = page; }
		void set_limit(std::string limit) { limit_ = limit; }

	private:
		std::vector<std::string> fields_;
		std::string page_;
		std::string limit_;
		std::vector<std::string> sorts_conditions_;
		std::map<std::string, std::string> iquals_conditions_;
		std::map<std::string, std::string> not_iquals_conditions_;
		std::map<std::string, std::string> greather_than_;
		std::map<std::string, std::string> smaller_than_;
		std::map<std::string, std::pair<std::string, std::string>> between_;
		std::map<std::string, std::vector<std::string>> in_;
		std::map<std::string, std::vector<std::string>> not_in_;
		std::vector<std::vector<std::string>> values_;
		std::map<std::string, std::string> set_;
};


class CPW::QueryActions : public ManageJSON
{
	public:
		QueryActions();
		~QueryActions();

		std::string get_final_query() const {return final_query_;}
		Filters& get_current_filters_()
		{
			auto& var = current_filters_;
			return var;
		}
		Data::Session get_session() const {return session_;}
		Data::Statement get_query() const {return query_;}
		std::map<std::string, std::string>* get_table_rows() const {return table_rows_;}
		JSON::Array::Ptr get_result_json() const {return result_json_;}

		void ResetQuery_();
		void IdentifyFilters_();
		void ComposeQuery_(TypeAction action_type, std::string table);
		void ExecuteQuery_();

	protected:
		void CreateJSONResult_();
		std::string ComposeInsertSentence_(std::string table);
		std::string ComposeSelectSentence_(std::string table);
		std::string ComposeUpdateSentence_(std::string table);
		std::string ComposeDeleteSentence_(std::string table);
		std::string MakeFinalQuery_(std::vector<std::string>& tmp_query);

		void IncorporeWhere_(std::vector<std::string>& tmp_query);
		void IncorporeAND_(std::vector<std::string>& tmp_query);
		void IncorporeFields_(std::vector<std::string>& tmp_query);
		void IncorporePageLimit_(std::vector<std::string>& tmp_query);
		void IncorporeSort_(std::vector<std::string>& tmp_query);
		void IncorporeIqual_(std::vector<std::string>& tmp_query);
		void IncorporeNotIqual_(std::vector<std::string>& tmp_query);
		void IncorporeGreatherThan_(std::vector<std::string>& tmp_query);
		void IncorporeSmallerThan_(std::vector<std::string>& tmp_query);
		void IncorporeBetween_(std::vector<std::string>& tmp_query);
		void IncorporeIn_(std::vector<std::string>& tmp_query);
		void IncorporeNotIn_(std::vector<std::string>& tmp_query);
		void IncorporeValues_(std::vector<std::string>& tmp_query);
		void IncorporeSet_(std::vector<std::string>& tmp_query);

	private:
		void FillTypeActionsText_();
		bool ExistsType_(std::string type);

		std::string final_query_;
		Filters current_filters_;
		Data::Session session_;
		Data::Statement query_;
		std::map<std::string, std::string>* table_rows_;
		JSON::Array::Ptr result_json_;
		std::map<std::string, TypeQuery> type_actions_map_;
		Application& app_;
};


#endif // CPW_QUERYACTIONS_H
