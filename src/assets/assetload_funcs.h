#ifndef ASSETS_ASSET_LOADERS_H_
#define ASSETS_ASSET_LOADERS_H_

#include <string>
#include <memory>

class ResPaths;
class Assets;
class AssetsLoader;
class Atlas;
struct AssetCfg;

/// @brief see AssetsLoader.h: aloader_func
namespace assetload {
    bool texture(
        AssetsLoader&,
        Assets*, 
        const ResPaths* paths,
        const std::string filename, 
        const std::string name,
        std::shared_ptr<AssetCfg> settings
    );
    bool shader(
        AssetsLoader&,
        Assets*,
        const ResPaths* paths,
        const std::string filename, 
        const std::string name,
        std::shared_ptr<AssetCfg> settings
    );
    bool atlas(
        AssetsLoader&,
        Assets*,
        const ResPaths* paths, 
        const std::string directory, 
        const std::string name,
        std::shared_ptr<AssetCfg> settings
    );
    bool font(
        AssetsLoader&,
        Assets*, 
        const ResPaths* paths,
        const std::string filename, 
        const std::string name,
        std::shared_ptr<AssetCfg> settings
    );
    bool layout(
        AssetsLoader&,
        Assets*,
        const ResPaths* paths,
        const std::string file,
        const std::string name,
        std::shared_ptr<AssetCfg> settings
    );

    bool sound(
        AssetsLoader&,
        Assets*,
        const ResPaths* paths,
        const std::string file,
        const std::string name,
        std::shared_ptr<AssetCfg> settings
    );
}

#endif // ASSETS_ASSET_LOADERS_H_
