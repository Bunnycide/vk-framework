//
// Created by jae on 11/04/23.
//

#include "includes.h"

void getAssetPath(const std::string& relativePath, std::string& asset_path){
    std::string asset_folder_path = std::string(ASSET_FOLDER);
    asset_path = ( asset_folder_path + relativePath);
}
