#pragma once
#include "Imgui.h"
#include "HImGuiAnimation.h"

std::string test;
ImVec4 rgb;

inline ImVec4 ImVec4Interpolation(ImVec4 a, ImVec4 b, float af)
{
	ImVec4 buff(1, 1, 1, 1);
	buff.x = HAnimationSystem::PlayerCallBack::SimpleBezierInterpolation(a.x, b.x, af);
	buff.y = HAnimationSystem::PlayerCallBack::SimpleBezierInterpolation(a.y, b.y, af);
	buff.z = HAnimationSystem::PlayerCallBack::SimpleBezierInterpolation(a.z, b.z, af);
	return buff;
}

void Sequencer(int32_t Frame, HAnimationSystem::HValue value)
{
	const std::vector<int> keys = { 0 , 30 , 64 };
	const std::vector<float> values = { 80,200,80 };
	const std::vector<ImVec4> rgbvalues = { ImVec4(1,0,0,1),ImVec4(0,1,0,1),ImVec4(0,0,1,1) };

	HAnimationSystem::PlayerCallBack::HInterpolationInfo info = HAnimationSystem::PlayerCallBack::GetInterpolationInfoFromKeys(keys, Frame);
	HAnimationSystem::HValue_Get<float>(value) = HAnimationSystem::PlayerCallBack::LinearInterpolation(values[info.PreviousKey], values[info.LastOneKey], info.alpha);
	test = HAnimationSystem::PlayerCallBack::StringInterpolation("hello world aaaaaaaaa vvvvvvv oooooooooo dawdad :/", info.alpha);
	rgb = ImVec4Interpolation(rgbvalues[info.PreviousKey], rgbvalues[info.LastOneKey], info.alpha);
}

float speed = 1;
int fps = 60;
bool loop = false;
HAnimationSystem::AnimationSequence* curr;

float value;
void DrawGUI()
{
	ImGui::Begin(u8"×÷˜IÓÃ");
	if (ImGui::Button("play"))
		HAnimationSystem::Play(Sequencer, 64, &value, &curr, speed, fps, loop);
	if (curr)
	{
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			curr->Stop();
		}
		ImGui::SameLine();
		if (ImGui::Button("pause"))
		{
			curr->Pause();
		}
		ImGui::SameLine();
		if (ImGui::Button("KeepPlay"))
		{
			curr->Play();
		}
		ImGui::SameLine();
		ImGui::Text("IsPlaying : %s", (curr->IsPlaying() ? "true" : "false"));
	}
	if (ImGui::BeginChild("init", ImVec2(270, 125), true, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("StartPlaySetting :");
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("speed", &speed, 0.1, 1);
		ImGui::SetNextItemWidth(200);
		ImGui::DragInt("FPS", &fps, 0.1, 1);
		ImGui::SetNextItemWidth(200);
		ImGui::Checkbox("loop", &loop);
	}
	ImGui::EndChild();
	if (curr)
	{
		ImGui::SameLine();
		if (ImGui::BeginChild("initrnt", ImVec2(270, 125), true, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("RuningTimePlayerSetting :");
			ImGui::SetNextItemWidth(200);
			ImGui::DragFloat("speed", &curr->getinfo().speed, 0.1, 1);
			ImGui::SetNextItemWidth(200);
			static int fpsbuff = 1000 / curr->getinfo().FPS_delta_time;
			if (ImGui::DragInt("FPS", &fpsbuff, 0.1, 1))
				curr->getinfo().FPS_delta_time = 1 / fpsbuff;
			ImGui::SetNextItemWidth(200);
			ImGui::Checkbox("loop", &curr->getinfo().IsLoop);
		}
		ImGui::EndChild();
	}

	if (ImGui::SliderFloat("Test", &value, 80, 200))
	{
	}
	ImGui::ColorEdit4("ColorTest", (float*)&rgb);
	ImGui::Text(test.c_str());
	ImGui::End();
	HAnimationSystem::updata(ImGui::GetIO().DeltaTime);
}