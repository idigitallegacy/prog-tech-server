//
// Created by elfxe on 12.02.2024.
//

#ifndef DOTAPARSERSERVER_ITEMSPROCESSOR_H
#define DOTAPARSERSERVER_ITEMSPROCESSOR_H

#include "../include/BoostJsonInclude.h"
#include "../include/StlInclude.h"

#include "../../cfg.h"

class ItemsProcessor {
public:
    ItemsProcessor() {
        boost::property_tree::ptree items_properties;
        boost::property_tree::read_json(INSTALL_PREFIX + "\\Server\\static\\items.json", items_properties);
        items_properties_ = items_properties;
    }

    int get_cost(const std::string &item_name, int item_charges = 1) {
        std::string name = std::regex_replace(item_name, std::regex("item_"), "");
        int default_charges;
        try {
            default_charges = items_properties_.get<int>(name + ".charges");
        } catch (std::exception &ex) {
            default_charges = 1;
            item_charges = 1;
        }
        int default_cost = items_properties_.get<int>(name + ".cost");

        return (int) (((float)item_charges / (float)default_charges) * default_cost);
    }

private:
    boost::property_tree::ptree items_properties_;
};


#endif //DOTAPARSERSERVER_ITEMSPROCESSOR_H
