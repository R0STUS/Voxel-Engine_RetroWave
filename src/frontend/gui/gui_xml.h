#ifndef FRONTEND_GUI_GUI_XML_H_
#define FRONTEND_GUI_GUI_XML_H_

#include <memory>
#include <unordered_set>
#include <unordered_map>

#include "GUI.h"
#include "../../coders/xml.h"

namespace scripting {
    class Environment;
}

class AssetsLoader;

namespace gui {
    class UiXmlReader;

    using uinode_reader = std::function<std::shared_ptr<UINode>(UiXmlReader&, xml::xmlelement)>;

    class UiXmlReader {
        std::unordered_map<std::string, uinode_reader> readers;
        std::unordered_set<std::string> ignored;
        std::string filename;
        const scripting::Environment& env;
        AssetsLoader& assetsLoader;
        
    public:
        UiXmlReader(const scripting::Environment& env, AssetsLoader& assetsLoader);

        void add(const std::string& tag, uinode_reader reader);
        bool hasReader(const std::string& tag) const;
        void addIgnore(const std::string& tag);
        
        std::shared_ptr<UINode> readUINode(xml::xmlelement element);
        
        void readUINode(
            UiXmlReader& reader, 
            xml::xmlelement element, 
            UINode& node
        );

        void readUINode(
            UiXmlReader& reader, 
            xml::xmlelement element, 
            Container& container
        );

        std::shared_ptr<UINode> readXML(
            const std::string& filename,
            const std::string& source
        );

        std::shared_ptr<UINode> readXML(
            const std::string& filename,
            xml::xmlelement root
        );

        const scripting::Environment& getEnvironment() const;
        const std::string& getFilename() const;

        AssetsLoader& getAssetsLoader();
    };
}

#endif // FRONTEND_GUI_GUI_XML_H_
