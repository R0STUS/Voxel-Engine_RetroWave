#include "AssetsLoader.h"
#include "Assets.h"

#include "assetload_funcs.h"

#include <iostream>
#include <memory>

#include "../constants.h"
#include "../data/dynamic.h"
#include "../files/files.h"
#include "../files/engine_paths.h"
#include "../content/Content.h"
#include "../content/ContentPack.h"
#include "../logic/scripting/scripting.h"

AssetsLoader::AssetsLoader(Assets* assets, const ResPaths* paths) 
  : assets(assets), paths(paths) 
{
	addLoader(AssetType::shader, assetload::shader);
	addLoader(AssetType::texture, assetload::texture);
	addLoader(AssetType::font, assetload::font);
	addLoader(AssetType::atlas, assetload::atlas);
    addLoader(AssetType::layout, assetload::layout);
    addLoader(AssetType::sound, assetload::sound);
}

void AssetsLoader::addLoader(AssetType tag, aloader_func func) {
	loaders[tag] = func;
}

void AssetsLoader::add(AssetType tag, const std::string filename, const std::string alias, std::shared_ptr<AssetCfg> settings) {
	entries.push(aloader_entry{tag, filename, alias, settings});
}

bool AssetsLoader::hasNext() const {
	return !entries.empty();
}

bool AssetsLoader::loadNext() {
	const aloader_entry& entry = entries.front();
	std::cout << "    loading " << entry.filename << " as " << entry.alias << std::endl;
	std::cout.flush();
	auto found = loaders.find(entry.tag);
	if (found == loaders.end()) {
		std::cerr << "unknown asset tag " << static_cast<int>(entry.tag) << std::endl;
		return false;
	}
	aloader_func loader = found->second;
	bool status = loader(*this, assets, paths, entry.filename, entry.alias, entry.config);
	entries.pop();
	return status;
}

void addLayouts(int env, const std::string& prefix, const fs::path& folder, AssetsLoader& loader) {
    if (!fs::is_directory(folder)) {
        return;
    }
    for (auto& entry : fs::directory_iterator(folder)) {
        const fs::path file = entry.path();
        if (file.extension().u8string() != ".xml")
            continue;
        std::string name = prefix+":"+file.stem().u8string();
        loader.add(AssetType::layout, file.u8string(), name, std::make_shared<LayoutCfg>(env));
    }
}

void AssetsLoader::tryAddSound(std::string name) {
    if (name.empty()) {
        return;
    }
    std::string file = SOUNDS_FOLDER+"/"+name;
    add(AssetType::sound, file, name);
}

static std::string assets_def_folder(AssetType tag) {
    switch (tag) {
        case AssetType::font: return FONTS_FOLDER;
        case AssetType::shader: return SHADERS_FOLDER;
        case AssetType::texture: return TEXTURES_FOLDER;
        case AssetType::atlas: return TEXTURES_FOLDER;
        case AssetType::layout: return LAYOUTS_FOLDER;
        case AssetType::sound: return SOUNDS_FOLDER;
    }
    return "<error>";
}

void AssetsLoader::processPreload(
    AssetType tag, 
    const std::string& name, 
    dynamic::Map* map
) {
    std::string defFolder = assets_def_folder(tag);
    std::string path = defFolder+"/"+name;
    if (map == nullptr) {
        add(tag, path, name);
        return;
    }
    map->str("path", path);
    switch (tag) {
        case AssetType::sound:
            add(tag, path, name, std::make_shared<SoundCfg>(
                map->getBool("keep-pcm", false)
            ));
            break;
        default:
            add(tag, path, name);
            break;
    }
}

void AssetsLoader::processPreloadList(AssetType tag, dynamic::List* list) {
    if (list == nullptr) {
        return;
    }
    for (uint i = 0; i < list->size(); i++) {
        auto value = list->get(i);
        switch (value->type) {
            case dynamic::valtype::string:
                processPreload(tag, *value->value.str, nullptr);
                break;
            case dynamic::valtype::map: {
                auto name = value->value.map->getStr("name");
                processPreload(tag, name, value->value.map);
                break;
            }
            default:
                throw std::runtime_error("invalid entry type");
        }
    }
}

void AssetsLoader::processPreloadConfig(fs::path file) {
    auto root = files::read_json(file);
    processPreloadList(AssetType::font, root->list("fonts"));
    processPreloadList(AssetType::shader, root->list("shaders"));
    processPreloadList(AssetType::texture, root->list("textures"));
    processPreloadList(AssetType::sound, root->list("sounds"));
    // layouts are loaded automatically
}

void AssetsLoader::processPreloadConfigs(const Content* content) {
    for (auto& entry : content->getPacks()) {
        const auto& pack = entry.second;
        auto preloadFile = pack->getInfo().folder / fs::path("preload.json");
        if (fs::exists(preloadFile)) {
            processPreloadConfig(preloadFile);
        }
    }
    auto preloadFile = paths->getMainRoot()/fs::path("preload.json");
    if (fs::exists(preloadFile)) {
        processPreloadConfig(preloadFile);
    }
}

void AssetsLoader::addDefaults(AssetsLoader& loader, const Content* content) {
    loader.add(AssetType::font, FONTS_FOLDER+"/font", "normal");
    loader.add(AssetType::shader, SHADERS_FOLDER+"/ui", "ui");
    loader.add(AssetType::shader, SHADERS_FOLDER+"/main", "main");
    loader.add(AssetType::shader, SHADERS_FOLDER+"/lines", "lines");
    loader.add(AssetType::texture, TEXTURES_FOLDER+"/gui/menubg", "gui/menubg");
    loader.add(AssetType::texture, TEXTURES_FOLDER+"/gui/delete_icon", "gui/delete_icon");
    loader.add(AssetType::texture, TEXTURES_FOLDER+"/gui/no_icon", "gui/no_icon");
    loader.add(AssetType::texture, TEXTURES_FOLDER+"/gui/warning", "gui/warning");
    loader.add(AssetType::texture, TEXTURES_FOLDER+"/gui/error", "gui/error");
    loader.add(AssetType::texture, TEXTURES_FOLDER+"/gui/cross", "gui/cross");
    if (content) {
        loader.processPreloadConfigs(content);

        for (auto& entry : content->getBlockMaterials()) {
            auto& material = entry.second;
            loader.tryAddSound(material.stepsSound);
            loader.tryAddSound(material.placeSound);
            loader.tryAddSound(material.breakSound);
        }

        addLayouts(0, "core", loader.getPaths()->getMainRoot()/fs::path("layouts"), loader);
        for (auto& entry : content->getPacks()) {
            auto pack = entry.second.get();
            auto& info = pack->getInfo();
            fs::path folder = info.folder / fs::path("layouts");
            addLayouts(pack->getEnvironment()->getId(), info.id, folder, loader);
        }
    }
    loader.add(AssetType::atlas, TEXTURES_FOLDER+"/blocks", "blocks");
    loader.add(AssetType::atlas, TEXTURES_FOLDER+"/items", "items");
}

const ResPaths* AssetsLoader::getPaths() const {
	return paths;
}
