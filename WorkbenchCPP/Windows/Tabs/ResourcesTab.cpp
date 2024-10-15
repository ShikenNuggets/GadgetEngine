#include "ResourcesTab.h"

#include <imgui.h>

#include <Resource/BinaryBlobResource.h>

#include "EditorApp.h"

using namespace Gadget;
using namespace Workbench;

static constexpr StaticArray<const char*, 10> gResourceTypes{
	"---",
	Mesh::typeName,
	GL_Shader::typeName,
	Font::typeName,
	FreetypeFont::typeName,
	Texture::typeName,
	BinaryBlobResource::typeName,
	AudioClip::typeName,
	AnimMesh::typeName,
	AnimClip::typeName
};

ResourcesTab::ResourcesTab(){}

void ResourcesTab::Update(){}

void Test(const char* const items[], int items_count, int height_in_items){}

void ResourcesTab::Draw(){
	if(ImGui::BeginTable("##ResourcesTab_Table1", 3, ImGuiTableFlags_Hideable | ImGuiTableFlags_SizingFixedFit)){
		ImGui::TableNextColumn();
		ImGui::Button("Add");

		ImGui::TableNextColumn();
		ImGui::Button("Remove");

		ImGui::TableNextColumn();
		ImGui::Button("Save");

		ImGui::EndTable();
	}

	auto* pRes = EditorApp::GetProjectManager().GetResources();
	GADGET_BASIC_ASSERT(pRes != nullptr);
	if(pRes != nullptr && ImGui::BeginTable("##ResourcesTab_Table2", 4, ImGuiTableFlags_BordersH | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings)){
		ImGui::TableSetupColumn("##ResourcesTab_Table2_Column1", ImGuiTableColumnFlags_None, 5.0f);
		ImGui::TableSetupColumn("##ResourcesTab_Table2_Column2");
		ImGui::TableSetupColumn("##ResourcesTab_Table2_Column3", ImGuiTableColumnFlags_None, 200.0f);
		ImGui::TableSetupColumn("##ResourcesTab_Table2_Column4");

		int idx = 0;

		const auto& resources = pRes->GetResources();
		for(const auto& r : resources){
			if(idx % 2 == 0){
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32_BLACK);
			}

			ImGui::TableNextRow(ImGuiTableRowFlags_None, 60.0f);
			ImGui::TableNextColumn();
			ImGui::TableNextColumn();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
			ImGui::Text(r.key.GetString().c_str());

			StringID resourceType = r.value.GetValue(SID("resource_type"), StringID::None).ToStr();
			GADGET_BASIC_ASSERT(resourceType != StringID::None);
			int ddv = 0;
			
			for(int64_t i = 0; i < gResourceTypes.Size(); i++){
				if(resourceType.GetString() == gResourceTypes[i]){
					ddv = i;
					break;
				}
			}

			ImGui::TableNextColumn();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
			ImGui::Combo(("##ResourceTypeComboBox_" + std::to_string(idx)).c_str(), &ddv, gResourceTypes.data, gResourceTypes.Size());

			ImGui::TableNextColumn();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
			for(size_t i = 0; i < r.value.Size(); i++){
				if(r.value[i].Name() == SID("resource_type")){
					continue;
				}

				ImGui::Text((r.value[i].Name().GetString() + ": " + r.value[i].ToStr().GetString()).c_str());
			}

			ImGui::Dummy(ImVec2(0.0f, 20.0f));

			idx++;
		}

		ImGui::EndTable();
	}
}