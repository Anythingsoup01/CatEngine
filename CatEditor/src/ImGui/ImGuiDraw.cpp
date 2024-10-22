#include "ImGuiDraw.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace CatEngine
{

	template<typename T>
	static void DrawImGuiColorEdit(T& values)
	{
		static_assert(false);
	}

	template<>
	static void DrawImGuiColorEdit<glm::vec4>(glm::vec4& values)
	{

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3,0 });

		ImGui::ColorEdit4("##CO", glm::value_ptr(values), ImGuiColorEditFlags_NoInputs);

		ImGui::PopStyleVar(1);
	}

	template<>
	static void DrawImGuiColorEdit<glm::vec3>(glm::vec3& values)
	{

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3,0 });

		ImGui::ColorEdit3("##CO", glm::value_ptr(values), ImGuiColorEditFlags_NoInputs);

		ImGui::PopStyleVar(1);
	}

	template<typename T>
	static void DrawImGuiDragFloat(T& values, float step, float min, float max)
	{
		static_assert(false);
	}

	template<>
	static void DrawImGuiDragFloat<glm::vec4>(glm::vec4& values, float step, float min, float max)
	{

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3,0 });
		for (int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat("##", &values[i], step, min, max, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PopID();
		}
		ImGui::PopStyleVar(1);
	}

	template<>
	static void DrawImGuiDragFloat<glm::vec3>(glm::vec3& values, float step, float min, float max)
	{

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3,0 });
		for (int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat("##", &values[i], step, min, max, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PopID();
		}
		ImGui::PopStyleVar(1);
	}

	template<>
	static void DrawImGuiDragFloat<glm::vec2>(glm::vec2& values, float step, float min, float max)
	{

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3,0 });
		for (int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat("##", &values[i], step, min, max, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PopID();
		}
		ImGui::PopStyleVar(1);
	}

	template<>
	static void DrawImGuiDragFloat<float>(float& value, float step, float min, float max)
	{

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3,0 });

		ImGui::PushID(1);
		ImGui::DragFloat("##", &value, step, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopID();
		
		ImGui::PopStyleVar(1);
	}

	void ImGuiDraw::DrawColorEdit4Control(const std::string& label, glm::vec4& values, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		DrawImGuiColorEdit<glm::vec4>(values);

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void ImGuiDraw::DrawColorEdit3Control(const std::string& label, glm::vec3& values, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		DrawImGuiColorEdit<glm::vec3>(values);

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void ImGuiDraw::DrawVec4Control(const std::string& label, glm::vec4& values, float step, float min, float max, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		DrawImGuiDragFloat<glm::vec4>(values, step, min, max);

		ImGui::Columns(1);

		ImGui::PopID();
	}
	void ImGuiDraw::DrawVec3Control(const std::string& label, glm::vec3& values, float step, float min, float max, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		DrawImGuiDragFloat<glm::vec3>(values, step, min, max);

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void ImGuiDraw::DrawVec2Control(const std::string& label, glm::vec2& values, float step, float min, float max, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		DrawImGuiDragFloat<glm::vec2>(values, step, min, max);

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void ImGuiDraw::DrawVec1Control(const std::string& label, float& value, float step, float min, float max, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		DrawImGuiDragFloat<float>(value, step, min, max);

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void ImGuiDraw::DrawCheckBox(const std::string& label, bool& active, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::Checkbox("##CB", &active);

		ImGui::Columns(1);

		ImGui::PopID();
	}
}
