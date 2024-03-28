//
// Created by elfxe on 11.02.2024.
//

#ifndef DOTAPARSERSERVER_TIMELINEJSONSTORAGE_H
#define DOTAPARSERSERVER_TIMELINEJSONSTORAGE_H

#include "../include/BoostJsonInclude.h"
#include "../include/StlInclude.h"

class TimelineJsonStorage {
public:
    TimelineJsonStorage()
    : timeline_(),
    has_steam_id_(false),
    has_match_id_(false) {}

    void initialize(std::string steam_id = "", std::string match_id = "") {
        if (!is_initialized()) {
            if (!has_steam_id_ && !steam_id.empty()) {
                related_steam_id_ = steam_id;
                has_steam_id_ = true;
            }

            if (!has_match_id_ && !match_id.empty()) {
                related_match_id_ = match_id;
                has_match_id_ = true;
            }
        }
    }

    void add_state(const ptree::ptree &json) {
        timeline_.emplace_back(json);
    }

    ptree::ptree get_net_worth() {
        ptree::ptree result_root;
        ptree::ptree values_root;
        for (auto &item : timeline_) {
            ptree::ptree node;
            node.put("", item.get<int>("net_worth"));
            values_root.push_back(std::make_pair("", node));
        }
        result_root.add_child("values", values_root);
        return result_root;
    }

    ptree::ptree get_gpm() {
        ptree::ptree result_root;
        ptree::ptree values_root;
        for (auto &item : timeline_) {
            ptree::ptree node;
            node.put("", item.get<int>("gpm"));
            values_root.push_back(std::make_pair("", node));
        }
        result_root.add_child("values", values_root);
        return result_root;
    }

    ptree::ptree get_xpm() {
        ptree::ptree result_root;
        ptree::ptree values_root;
        for (auto &item : timeline_) {
            ptree::ptree node;
            node.put("", item.get<int>("xpm"));
            values_root.push_back(std::make_pair("", node));
        }
        result_root.add_child("values", values_root);
        return result_root;
    }

    ptree::ptree get_kda() {
        ptree::ptree result_root;
        ptree::ptree kills_root;
        ptree::ptree deaths_root;
        ptree::ptree assists_root;
        for (auto &item : timeline_) {
            ptree::ptree node = item.get_child("kda");
            ptree::ptree kills;
            ptree::ptree deaths;
            ptree::ptree assists;
            kills.put("", node.get<int>("kills"));
            deaths.put("", node.get<int>("deaths"));
            assists.put("", node.get<int>("assists"));
            kills_root.push_back(std::make_pair("", kills));
            deaths_root.push_back(std::make_pair("", deaths));
            assists_root.push_back(std::make_pair("", assists));
        }
        result_root.add_child("kills", kills_root);
        result_root.add_child("deaths", deaths_root);
        result_root.add_child("assists", assists_root);
        return result_root;
    }

    ptree::ptree get_game_time() {
        ptree::ptree result_root;
        ptree::ptree values_root;
        for (auto &item : timeline_) {
            ptree::ptree node;
            node.put("", item.get<int>("game_time"));
            values_root.push_back(std::make_pair("", node));
        }
        result_root.add_child("values", values_root);
        return result_root;
    }

    bool empty() {
        return timeline_.empty();
    }

    void clear() {
        related_steam_id_.clear();
        has_steam_id_ = false;
        timeline_.clear();
    }

    std::vector<ptree::ptree> get_timeline() {
        return timeline_;
    }

    unsigned long long get_size() {
        return timeline_.size();
    }

    bool is_initialized() {
        return has_steam_id_ && has_match_id_;
    }

    std::string get_steam_id() {
        return related_steam_id_;
    }

    std::string get_match_id() {
        return related_match_id_;
    }

private:
    bool has_steam_id_;
    bool has_match_id_;

    std::string related_match_id_;
    std::string related_steam_id_;
    std::vector<boost::property_tree::ptree> timeline_;
};


#endif //DOTAPARSERSERVER_TIMELINEJSONSTORAGE_H
