#include "ItemDef.h"
#include "../util/stringutil.h"

ItemDef::ItemDef(std::string name) : name(name) {
    caption = util::id_to_caption(name);
}
