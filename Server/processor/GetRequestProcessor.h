//
// Created by elfxe on 11.02.2024.
//

#ifndef DOTAPARSERSERVER_GETREQUESTPROCESSOR_H
#define DOTAPARSERSERVER_GETREQUESTPROCESSOR_H


#include "../include/BoostBeastInclude.h"
#include "../include/BoostJsonInclude.h"
#include "../include/StlInclude.h"

#include <boost/algorithm/string.hpp>

#include "../storage/TimelineJsonStorage.h"

#include "../../cfg.h"
#include "../executor/LoadExecutor.h"

class GetRequestProcessor {
public:
    GetRequestProcessor(TimelineJsonStorage *timelineStorage) : timeline_storage_(timelineStorage), load_executor_(timelineStorage) {}

    void parse_request(http::request<http::dynamic_body> &request) {
        if (request.method() != http::verb::get)
            throw std::exception(); // todo

        if (std::regex_match(std::string(request.target()), std::regex("\\/test_connection.*"))) {
            std::string params = std::string(request.target());
            size_t pos = params.find('?');
            if (pos == std::string::npos) {
                make_200_header();
                return;
            }

            params.erase(0, pos + 1);
            ptree::ptree root = parse_json_from_uri(params);
            if (!validate_params(root)) {
                make_400_header();
                return;
            }

            std::string steam_id = root.get<std::string>("steamid");
            std::string game_id = root.get<std::string>("gameid");

            try {
                load_executor_.load(steam_id, game_id);
            } catch (std::exception &ex) {
                make_400_header();
                return;
            }

            make_200_header();
            make_header_text_plain();
            make_response("ok");
            return;
        }
        if (std::regex_match(std::string(request.target()), std::regex("\\/get_net_worth.*"))) {
            std::string params = std::string(request.target());
            size_t pos = params.find('?');

            if (pos == std::string::npos) {
                make_400_header();
                return;
            }

            params.erase(0, pos + 1);
            ptree::ptree root = parse_json_from_uri(params);

            if (!validate_params(root)) {
                make_400_header();
                return;
            }

            std::string steam_id = root.get<std::string>("steamid");
            std::string game_id = root.get<std::string>("gameid");

            try {
                load_executor_.load(steam_id, game_id);
            } catch (std::exception &ex) {
                make_400_header();
                return;
            }

            ptree::ptree json_root = timeline_storage_->get_net_worth();
            timeline_storage_->clear();

            make_200_header();
            make_header_text_json();
            make_response(json_root);
            return;
        }

        if (std::regex_match(std::string(request.target()), std::regex("\\/get_gpm.*"))) {
            std::string params = std::string(request.target());
            size_t pos = params.find('?');

            if (pos == std::string::npos) {
                make_400_header();
                return;
            }

            params.erase(0, pos + 1);
            ptree::ptree root = parse_json_from_uri(params);

            if (!validate_params(root)) {
                make_400_header();
                return;
            }

            std::string steam_id = root.get<std::string>("steamid");
            std::string game_id = root.get<std::string>("gameid");

            try {
                load_executor_.load(steam_id, game_id);
            } catch (std::exception &ex) {
                make_400_header();
                return;
            }

            ptree::ptree json_root = timeline_storage_->get_gpm();
            timeline_storage_->clear();

            make_200_header();
            make_header_text_json();
            make_response(json_root);
            return;
        }

        if (std::regex_match(std::string(request.target()), std::regex("\\/get_xpm.*"))) {
            std::string params = std::string(request.target());
            size_t pos = params.find('?');

            if (pos == std::string::npos) {
                make_400_header();
                return;
            }

            params.erase(0, pos + 1);
            ptree::ptree root = parse_json_from_uri(params);

            if (!validate_params(root)) {
                make_400_header();
                return;
            }

            std::string steam_id = root.get<std::string>("steamid");
            std::string game_id = root.get<std::string>("gameid");

            try {
                load_executor_.load(steam_id, game_id);
            } catch (std::exception &ex) {
                make_400_header();
                return;
            }

            ptree::ptree json_root = timeline_storage_->get_xpm();
            timeline_storage_->clear();

            make_200_header();
            make_header_text_json();
            make_response(json_root);
            return;
        }

        if (std::regex_match(std::string(request.target()), std::regex("\\/get_kda.*"))) {
            std::string params = std::string(request.target());
            size_t pos = params.find('?');

            if (pos == std::string::npos) {
                make_400_header();
                return;
            }

            params.erase(0, pos + 1);
            ptree::ptree root = parse_json_from_uri(params);

            if (!validate_params(root)) {
                make_400_header();
                return;
            }

            std::string steam_id = root.get<std::string>("steamid");
            std::string game_id = root.get<std::string>("gameid");

            try {
                load_executor_.load(steam_id, game_id);
            } catch (std::exception &ex) {
                make_400_header();
                return;
            }

            ptree::ptree json_root = timeline_storage_->get_kda();
            timeline_storage_->clear();

            make_200_header();
            make_header_text_json();
            make_response(json_root);
            return;
        }

        if (std::regex_match(std::string(request.target()), std::regex("\\/get_game_time.*"))) {
            std::string params = std::string(request.target());
            size_t pos = params.find('?');

            if (pos == std::string::npos) {
                make_400_header();
                return;
            }

            params.erase(0, pos + 1);
            ptree::ptree root = parse_json_from_uri(params);

            if (!validate_params(root)) {
                make_400_header();
                return;
            }

            std::string steam_id = root.get<std::string>("steamid");
            std::string game_id = root.get<std::string>("gameid");

            try {
                load_executor_.load(steam_id, game_id);
            } catch (std::exception &ex) {
                make_400_header();
                return;
            }

            ptree::ptree json_root = timeline_storage_->get_game_time();
            timeline_storage_->clear();

            make_200_header();
            make_header_text_json();
            make_response(json_root);
            return;
        }

        if (request.target() == "/clear_history") {
            make_200_header();
            timeline_storage_->clear();
            clear_response();
            return;
        }

        response_.result(http::status::not_found);
        response_.set(http::field::content_type, "text/html");
    }

    http::response<http::dynamic_body> get_response() {
        return response_;
    }

    void clear_response() {
        response_ = http::response<http::dynamic_body>();
    }

private:
    TimelineJsonStorage *timeline_storage_;
    http::response<http::dynamic_body> response_;
    LoadExecutor load_executor_;

    bool validate_params(ptree::ptree root) {
        if (!root.get_optional<std::string>("steamid").is_initialized())
            return false;

        if (!root.get_optional<std::string>("gameid").is_initialized())
            return false;
        return true;
    }

    ptree::ptree parse_json_from_uri(std::string uri) {
        std::string delimiter = "&";
        std::vector<std::string> kv_pairs;
        ptree::ptree root;

        size_t pos = 0;
        std::string token;
        while ((pos = uri.find(delimiter)) != std::string::npos) {
            token = uri.substr(0, pos);
            kv_pairs.emplace_back(token);
            uri.erase(0, pos + delimiter.length());
        }
        kv_pairs.emplace_back(uri);

        for (auto &i : kv_pairs) {
            std::string delimiter = "=";

            pos = i.find(delimiter);
            token = i.substr(0, pos);
            i.erase(0, pos + delimiter.length());
            root.put(token, i);
        }
        return root;
    }

    void make_400_header() {
        response_.result(http::status::bad_request);
        response_.set(http::field::server, "Boost.Asio");
    }

    void make_200_header() {
        response_.result(http::status::ok);
        response_.set(http::field::server, "Boost.Asio");
    }

    void make_header_text_plain() {
        response_.set(http::field::content_type, "text/plain");
    }

    void make_header_text_json() {
        response_.set(http::field::content_type, "text/json");
    }

    void make_response(ptree::ptree root) {
        std::stringstream ss;
        std::string json;

        ptree::write_json(ss, root);
        json = ss.str();
        beast::ostream(response_.body()) << json;
    }

    void make_response(std::string message) {
        beast::ostream(response_.body()) << message;
    }
};


#endif //DOTAPARSERSERVER_GETREQUESTPROCESSOR_H
