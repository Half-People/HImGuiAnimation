//birth: created by HalfPeople on 2024/4/14

#include "HImGuiAnimation.h"
#include <algorithm>

std::vector<HAnimationSystem::AnimationSequence> Sequences;
std::vector<HAnimationSystem::AnimationSequence**> WaitingForDeletePointers;

void HAnimationSystem::updata(float delta_time, int MaxFPS)
{
	static float buffer;
	if (buffer > 1.f / MaxFPS)
	{
		for (HAnimationSystem::AnimationSequence& Sequence : Sequences)
			Sequence.updata(delta_time);
		buffer = 0;
	}
	else
		buffer += delta_time;
}

void HAnimationSystem::updata(float delta_time)
{
	if (!WaitingForDeletePointers.empty())
	{
		for (HAnimationSystem::AnimationSequence** DeletePointers : WaitingForDeletePointers)
			if (DeletePointers)
			{
				*DeletePointers = 0;
			}
		WaitingForDeletePointers.clear();
	}
	for (HAnimationSystem::AnimationSequence& Sequence : Sequences)
		Sequence.updata_MaxFPS(delta_time);
}

void HAnimationSystem::Play(PlayerCallBack::AnimationPlayerCallBack callback, size_t MaxFrame, HValue value, HAnimationSystem::AnimationSequence** out, float speed, int FPS, bool IsLoop)
{
	for (HAnimationSystem::AnimationSequence& Sequence : Sequences)
	{
		if (Sequence.getinfo().callback == callback)
			return;
	}
	Sequences.push_back(HAnimationSystem::AnimationSequence(FPS, out, value, speed, MaxFrame, IsLoop, callback));
	if (out)
	{
		*out = &Sequences.back();
	}

	return;
}

void HAnimationSystem::AnimationSequence::Stop()
{
	UnloadFromArray();
	WaitingForDeletePointers.push_back(info.pos);
}

void HAnimationSystem::AnimationSequence::updata_MaxFPS(float delta_time)
{
	if (info.FPS_delta_time)
	{
		static float delta_time_buffer;
		if (delta_time_buffer > info.FPS_delta_time)
		{
			updata(delta_time_buffer);
			delta_time_buffer = 0;
		}
		else
			delta_time_buffer += delta_time;
	}
	else
		updata(delta_time);
}

void HAnimationSystem::AnimationSequence::updata(float delta_time)
{
	if (!Playing)
		return;
	float buff = delta_time * (info.speed * 100);
	info.CurrentFrame += buff;

	if (info.CurrentFrame >= info.MaxFrame)
	{
		if (info.IsLoop)
			info.CurrentFrame = 0;
		else
			Stop();
	}

	info.callback(info.CurrentFrame, info.data);
	return;
}

void HAnimationSystem::AnimationSequence::UnloadFromArray()
{
	for (size_t i = 0; i < Sequences.size(); i++)
	{
		if (this == &Sequences[i])
		{
			Sequences.erase(Sequences.begin() + i);
			return;
		}
	}
}

HAnimationSystem::PlayerCallBack::HInterpolationInfo HAnimationSystem::PlayerCallBack::GetInterpolationInfoFromKeys(const std::vector<int> keys, int32_t Frame)
{
	HInterpolationInfo info;
	// Binary search finds the closest key
	auto upper = std::upper_bound(keys.begin(), keys.end(), Frame);
	info.LastOneKey = std::distance(keys.begin(), upper);
	info.PreviousKey = info.LastOneKey - 1;

	if (info.LastOneKey < keys.size() && info.PreviousKey >= 0) {
		int32_t max = keys[info.LastOneKey];
		int32_t min = keys[info.PreviousKey];

		if (max != min) {
			info.alpha = static_cast<float>(Frame - min) / (max - min);
		}
	}
	return info;
}

float HAnimationSystem::PlayerCallBack::SimpleBezierInterpolation(float a, float b, float alpha)
{
	float u = 1.0f - alpha;
	float tt = alpha * alpha;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * alpha;

	float p = uuu * a + ttt * b;
	return p;
}

float HAnimationSystem::PlayerCallBack::CubicBezierInterpolation(float a, float control_point_a, float control_point_b, float b, float alpha)
{
	float offset = a + (b - a);
	control_point_a = offset * control_point_a;
	control_point_b = offset * control_point_b;
	float u = 1.0f - alpha;
	float tt = alpha * alpha;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * alpha;

	float p = uuu * a + 3.0f * uu * alpha * control_point_a + 3.0f * u * tt * control_point_b + ttt * b;
	return p;
}