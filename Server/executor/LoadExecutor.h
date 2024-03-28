//
// Created by elfxe on 15.02.2024.
//

#ifndef DOTAPARSERSERVER_LOADEXECUTOR_H
#define DOTAPARSERSERVER_LOADEXECUTOR_H

#include "../../cfg.h"

#include "../include/StlInclude.h"
#include "../include/BoostJsonInclude.h"

#include "../storage/TimelineJsonStorage.h"

class LoadExecutor {
public:
    LoadExecutor(TimelineJsonStorage *timelineStorage)
    : timeline_storage_(timelineStorage) { }

    void load(std::string steam_id, std::string match_id) {
        std::string path(INSTALL_PREFIX + "\\Server\\collected_data\\" + steam_id);
        if (!std::filesystem::exists(path))
            throw std::exception(); // todo

        path += "\\" + match_id + ".json";
        if (!std::filesystem::exists(path))
            throw std::exception(); // todo

        timeline_storage_->clear();
        std::ifstream fs(path);
        ptree::ptree root;

        ptree::read_json(fs, root);
        fs.close();

        timeline_storage_->initialize(root.get<std::string>("steamid"), root.get<std::string>("matchid"));

        for (auto &i : root.get_child("data")) {
            ptree::ptree item_root;
            for (auto &item : i.second) {
                if (item.first != "kda") {
                    item_root.put(item.first, std::stoi(item.second.data()));
                } else {
                    ptree::ptree kda_root;
                    kda_root.put("kills", std::stoi(item.second.get<std::string>("kills")));
                    kda_root.put("deaths", std::stoi(item.second.get<std::string>("deaths")));
                    kda_root.put("assists", std::stoi(item.second.get<std::string>("assists")));
                    item_root.add_child("kda", kda_root);
                }
            }

            timeline_storage_->add_state(item_root);
        }
    }
private:
    TimelineJsonStorage *timeline_storage_;
};


#endif //DOTAPARSERSERVER_LOADEXECUTOR_H
