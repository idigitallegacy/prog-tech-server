//
// Created by elfxe on 15.02.2024.
//

#ifndef DOTAPARSERSERVER_SAVEEXECUTOR_H
#define DOTAPARSERSERVER_SAVEEXECUTOR_H

#include "../../cfg.h"

#include "../include/StlInclude.h"
#include "../include/BoostJsonInclude.h"

#include "../storage/TimelineJsonStorage.h"

class SaveExecutor {
public:
    SaveExecutor(TimelineJsonStorage *timelineStorage)
        : timeline_storage_(timelineStorage) {}

    void save() {
        if (!timeline_storage_->is_initialized())
            // nothing to save
            return;

        std::string dstFolder = timeline_storage_->get_steam_id();
        std::string path(INSTALL_PREFIX + "\\Server\\collected_data\\" + dstFolder);

        if (!std::filesystem::exists(path))
            std::filesystem::create_directory(path);

        std::ofstream fs(INSTALL_PREFIX + "\\Server\\collected_data\\" + timeline_storage_->get_steam_id() + "\\" + timeline_storage_->get_match_id() + ".json");
        std::vector<ptree::ptree> data = timeline_storage_->get_timeline();
        ptree::ptree root;

        root.put("steamid", timeline_storage_->get_steam_id());
        root.put("matchid", timeline_storage_->get_match_id());

        ptree::ptree data_root;
        for (int i = 0; i < data.size(); ++i) {
            data_root.push_back(std::make_pair("", data[i]));
        }
        root.add_child("data", data_root);

        ptree::write_json(fs, root);
        fs.close();

        timeline_storage_->clear();
    }
private:
    TimelineJsonStorage *timeline_storage_;
};


#endif //DOTAPARSERSERVER_SAVEEXECUTOR_H
