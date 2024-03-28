//
// Created by elfxe on 11.02.2024.
//

#ifndef DOTAPARSERSERVER_POSTREQUESTPROCESSOR_H
#define DOTAPARSERSERVER_POSTREQUESTPROCESSOR_H

#include "../../definitions.h"
#include "../../cfg.h"

#include "../include/BoostBeastInclude.h"
#include "../include/BoostJsonInclude.h"
#include "../include/StlInclude.h"

#include <boost/algorithm/string.hpp>

#include "../storage/TimelineJsonStorage.h"
#include "ItemsProcessor.h"
#include "../executor/SaveExecutor.h"

namespace algorithm = boost::algorithm;

class PostRequestProcessor {
public:
    PostRequestProcessor(TimelineJsonStorage *timelineStorage)
            : user_agent_valve_dota_regex_(R"(valve\/steam.*\(570\))"),
              content_type_json_("(application\\/json)|(text\\/json)"),
              timeline_storage_(timelineStorage),
              save_executor_(timelineStorage),
              last_timestamp_(0) {
    }

    void parse_request(http::request<http::dynamic_body> &request) {
        if (request.method() != http::verb::post)
            throw std::exception(); // todo

        const http::header<true> headers = request.base();
        std::string user_agent = headers["user-agent"];
        std::string content_type = headers["content-type"];

        algorithm::to_lower(user_agent);
        algorithm::to_lower(content_type);


        if (std::regex_match(user_agent.data(), user_agent_valve_dota_regex_) && std::regex_match(content_type.data(), content_type_json_)) {
            process_valve_dota_json_request(request);
        }
    }

private:
    const std::regex user_agent_valve_dota_regex_;
    const std::regex content_type_json_;

    unsigned long long last_timestamp_;

    TimelineJsonStorage *timeline_storage_;
    SaveExecutor save_executor_;

    ItemsProcessor items_processor;

    // only should be called if dota is in the main menu
    void save_data() {
        // if we have some data
        if (!timeline_storage_->empty())
            save_executor_.save();
    }

    bool validate_valve_dota_request(ptree::ptree root, unsigned long long timestamp) {
        if (!root.get_child_optional("map").is_initialized()) {
            // Game is in the main menu
            // Attempting to save stored data to file, if we have some data to be saved
            save_data();
            timeline_storage_->clear();
            return false;
        }

        if (timestamp - last_timestamp_ < DEFAULT_VALVE_DOTA_POST_TIMEOUT_SECONDS) {
            // too frequent requests
            return false;
        }

        // cleaning up if something went wrong
        if (timeline_storage_->get_steam_id() != extract_steam_id(root) || timeline_storage_->get_match_id() !=
                                                                                   extract_match_id(root)) {
            save_data();
            timeline_storage_->clear();
            timeline_storage_->initialize(extract_steam_id(root), extract_match_id(root));
        }

        return true;
    }

    void process_valve_dota_json_request(http::request<http::dynamic_body> &request) {
        std::string json = beast::buffers_to_string(request.body().data());

        std::stringstream ss;
        ss << json;

        ptree::ptree root;
        ptree::read_json(ss, root);

        unsigned long long timestamp = extract_timestamp(root);
        if (!validate_valve_dota_request(root, timestamp))
            return;


        last_timestamp_ = timestamp;
        std::string game_state = extract_game_state(root);

        if (game_state == "DOTA_GAMERULES_STATE_PRE_GAME" || game_state == "DOTA_GAMERULES_STATE_GAME_IN_PROGRESS") {
            if (!timeline_storage_->is_initialized())
                timeline_storage_->initialize(extract_steam_id(root), extract_match_id(root));

            int net_worth = extract_net_worth(root);
            int gpm = extract_gpm(root);
            int xpm = extract_xpm(root);
            ptree::ptree kda = extract_kda(root);
            int game_time = extract_game_time(root);

            // Build stored structure
            ptree::ptree stored_root;

            stored_root.put("game_time", game_time);
            stored_root.put("net_worth", net_worth);
            stored_root.put("gpm", gpm);
            stored_root.put("xpm", xpm);
            stored_root.put("kda.kills", kda.get<int>("kills"));
            stored_root.put("kda.deaths", kda.get<int>("deaths"));
            stored_root.put("kda.assists", kda.get<int>("assists"));

            std::cout << extract_match_id(root) << '\n';

            timeline_storage_->add_state(stored_root);
        }

    }

    std::string extract_steam_id(boost::property_tree::ptree root) {
        return root.get<std::string>("player.steamid", "0");
    }

    std::string extract_match_id(boost::property_tree::ptree root) {
        return root.get<std::string>("map.matchid", "0");
    }

    unsigned long long extract_timestamp(boost::property_tree::ptree root) {
        return root.get<unsigned long long>("provider.timestamp", 0);
    }

    std::string extract_game_state(boost::property_tree::ptree root) {
        return root.get<std::string>("map.game_state", "null");
    }

    int extract_net_worth(boost::property_tree::ptree root) {
        int net_worth = 0;
        ptree::write_json(std::cout, root.get_child("items"));
        for (boost::property_tree::ptree::value_type &item : root.get_child("items")) {
            std::string name = item.second.get<std::string>("name", "empty");

            if (name != "empty") {
                int item_charges = item.second.get<int>("charges", 1);
                net_worth += items_processor.get_cost(name, item_charges);
            }
        }
        net_worth += root.get<int>("player.gold", 0);
        return net_worth;
    }

    int extract_gpm(boost::property_tree::ptree root) {
        return root.get<int>("player.gpm", 0);
    }

    int extract_xpm(boost::property_tree::ptree root) {
        return root.get<int>("player.xpm", 0);
    }

    boost::property_tree::ptree extract_kda(boost::property_tree::ptree root) {
        boost::property_tree::ptree kda_root;
        kda_root.put("kills", root.get<int>("player.kills", 0));
        kda_root.put("deaths", root.get<int>("player.deaths", 0));
        kda_root.put("assists", root.get<int>("player.assists", 0));
        return kda_root;
    }

    int extract_game_time(boost::property_tree::ptree root) {
        return root.get<int>("map.game_time");
    }
};


#endif //DOTAPARSERSERVER_POSTREQUESTPROCESSOR_H
