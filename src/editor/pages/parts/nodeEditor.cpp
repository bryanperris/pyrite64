/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "nodeEditor.h"

#include "imgui.h"
#include "../../../context.h"
#include "../../../utils/logger.h"
#include "../../imgui/helper.h"

#include "ImNodeFlow.h"
#include "json.hpp"
#include "../../../project/graph/nodes/baseNode.h"
#include "../../../utils/fs.h"

namespace
{

}

Editor::NodeEditor::NodeEditor()
{
  auto &stylePin = *Project::Graph::Node::PIN_STYLE_LOGIC;
  stylePin = ImFlow::PinStyle{
    IM_COL32(0xAA, 0xAA, 0xAA, 0xFF),
    4, // shape
    6.0f, 7.0f, 6.5f, // radius: base, hover, connected
    1.3f // thickness
  };
  stylePin.extra.padding.y = 16;

  auto &stylePinVal = *Project::Graph::Node::PIN_STYLE_VALUE;
  stylePinVal = ImFlow::PinStyle{
    IM_COL32(0xFF, 0x99, 0x55, 0xFF),
    0, // shape
    6.0f, 7.0f, 6.5f, // radius: base, hover, connected
    1.3f // thickness
  };
  stylePinVal.extra.padding.y = 16;

  currentAsset = ctx.project->getAssets().getByName("test.p64graph");
  graph.deserialize(currentAsset
    ? Utils::FS::loadTextFile(currentAsset->path)
    : "{}"
  );

  graph.graph.droppedLinkPopUpContent([&](ImFlow::Pin* dragged)
  {
    ImGui::Text("Create New");
    ImGui::Separator();
    auto &names = Project::Graph::Graph::getNodeNames();
    for(size_t i = 0; i < names.size(); ++i) {
      if(ImGui::Selectable(names[i].c_str())) {

        auto newPos = dragged->getParent()->getPos();
        newPos.x += 150;
        auto node = graph.addNode(static_cast<uint32_t>(i), newPos);

        auto &ins = node->getIns();
        if(!ins.empty())ins[0]->createLink(dragged);

        node->setPos(newPos);
        ImGui::CloseCurrentPopup();
      }
    }
  });

  graph.graph.rightClickPopUpContent([&](ImFlow::BaseNode* node)
  {

    if(node) {
      if(ImGui::Selectable(ICON_MDI_TRASH_CAN_OUTLINE " Remove")) {
        node->destroy();
        ImGui::CloseCurrentPopup();
      }
    } else {
      ImGui::Text("@TODO: Create new node");
    }
  });

}

Editor::NodeEditor::~NodeEditor()
{
}

void Editor::NodeEditor::draw()
{
  if(!currentAsset)
  {
    ImGui::Text("No Asset loaded");
    return;
  }

  auto size = ImGui::GetContentRegionAvail();
  size.y -= 32;
  graph.graph.setSize(size);
  graph.graph.update();

  ImGui::Text("@TODO: Menu bar");

  ImGui::SameLine();
  if(ImGui::Button("Save"))
  {
    Utils::FS::saveTextFile(currentAsset->path, graph.serialize());
  }
}
