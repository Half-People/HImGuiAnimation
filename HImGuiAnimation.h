//birth: created by HalfPeople on 2024/4/14
#pragma once
#include <vector>
#include <string>

namespace HAnimationSystem
{
	typedef void* HValue;
	template <typename T>
	/*
	*	 @brief: Convert input variables
	*	 @param:  ...
	* 	 @birth: created by HalfPeople on 2024/4/14
	*/
	T& HValue_Get(HValue value)
	{
		return *static_cast<T*>(value);
	}
	namespace PlayerCallBack
	{
		struct HInterpolationInfo
		{
			size_t PreviousKey = 0;
			size_t LastOneKey = 0;
			float alpha = 0;
		};
		typedef void (*AnimationPlayerCallBack)(int32_t Frame, HValue value);
		//Get the information you need for the current transition
		//	 @brief: GetInterpolationInfoFromKeys
		//	 @param:  ...
		//	 @birth: created by HalfPeople on 2024/4/14
		HInterpolationInfo GetInterpolationInfoFromKeys(const std::vector<int> keys, int32_t Frame);
		//Interpolation tools (LERP)
		//	 @brief:  ...
		//	 @param:  ...
		//	 @birth: created by HalfPeople on 2024/4/14
		inline float LinearInterpolation(float a, float b, float alpha)
		{
			return a + (b - a) * alpha;
		}
		//Interpolation tools (Bezier)
		//	 @brief:  ...
		//	 @param:  ...
		//	 @birth: created by HalfPeople on 2024/4/14
		float SimpleBezierInterpolation(float a, float b, float alpha);
		//Interpolation tools (Bezier)
		//	 @brief:  ...
		//	 @param:  ...
		//	 @birth: created by HalfPeople on 2024/4/14
		float CubicBezierInterpolation(float a, float control_point_a, float control_point_b, float b, float alpha);
		//Interpolation tools (String)
		//	 @brief:  ...
		//	 @param:  ...
		//	 @birth: created by HalfPeople on 2024/4/14
		inline std::string StringInterpolation(std::string FullString, float alpha)
		{
			return FullString.substr(0, (size_t)FullString.length() * alpha);
		}
	}

	/*
	*	 @brief: single animation sequence
	*	 @param:  ...
	* 	 @birth: created by HalfPeople on 2024/4/14
	*/
	struct AnimationSequence
	{
		//	 @brief:  information
		struct information
		{
			//internal code (callback cache)
			PlayerCallBack::AnimationPlayerCallBack callback;
			//internal code (data transmission)
			HAnimationSystem::AnimationSequence** pos;
			//MaxFPS (!!Need to be calculated   1.f/FPS)
			float FPS_delta_time;
			//Loop animation
			bool IsLoop;
			size_t CurrentFrame=0, MaxFrame=0;
			//Play speed
			float speed;
			//internal code (data transmission)
			HValue data;
			void reset()
			{
				IsLoop = false;
				CurrentFrame = 0;
				speed = 1;
			}
		};
		//internal code (Init information)
		AnimationSequence(int FPS, HAnimationSystem::AnimationSequence** pos, HValue data, float speed, size_t MaxFrame, bool IsLoop, PlayerCallBack::AnimationPlayerCallBack callback)
		{
			info.pos = pos;
			info.FPS_delta_time = 1.f / FPS;
			info.IsLoop = IsLoop;
			info.callback = callback;
			info.MaxFrame = MaxFrame;
			info.speed = speed;
			info.data = data;
			Playing = true;
		}
		//Exit animation playback
		void Stop();
		//pause animation
		inline void Pause() { Playing = false; }
		//Resume paused animation
		inline void Play() { Playing = true; }
		//Whether animation is playing
		inline bool IsPlaying() { return Playing; }
		//Get underlying information (All information can be dynamically adjusted  Speed,IsLoop ...) (!Please do not use this adjustment MaxFPS,data,MaxFrame,pos,callback!)
		inline information& getinfo() { return info; }
		//Please use this to dynamic adjust MaxFPS
		void updata_MaxFPS(float delta_time);
		//internal code
		void updata(float delta_time);
	private:
		bool Playing = false;
		information info;
		void UnloadFromArray();
	};

	/*
	*    Play animation
	*	 @brief: Play animation
	*	 @param: callback (Animation overprocessed.See AnimationPlayerCallBack comments for details) , MaxFrame (The largest Frame among animations) , value (The value transferred to the callback) , out (Animation sequence returns) , speed (Play speed) ,FPS (Max FPS : Lift (-1) ) , IsLoop (keep looping)
	* 	 @birth: created by HalfPeople on 2024/4/14
	*/
	void Play(PlayerCallBack::AnimationPlayerCallBack callback, size_t MaxFrame, HValue value = nullptr, HAnimationSystem::AnimationSequence** out = 0, float speed = 1, int FPS = 60, bool IsLoop = false);

	/*
	*    - Animation Manager Update (used in main loop)(Manager FPS limit, Animation sequences will be disabled FPS limit)
	*
	*    - Recommended Use : void updata(float delta_time);
	*	 @brief: Animation Manager Update (Manager FPS limit, Animation sequences will be disabled FPS limit)
	*	 @param: delta_time (Please use ImGui to obtain  ImGui::GetIO().DeltaTime) , MaxFPS (FPS limit : Lift (-1) )
	* 	 @birth: created by HalfPeople on 2024/4/14
	*/
	void updata(float delta_time, int MaxFPS);

	/*
	*    Animation Manager Update (used in main loop)
	*	 @brief: Animation Manager Update
	*	 @param: delta_time (Please use ImGui to obtain  ImGui::GetIO().DeltaTime)
	* 	 @birth: created by HalfPeople on 2024/4/14
	*/
	void updata(float delta_time);
}

//example :
//
//	void Sequencer(int32_t Frame, HAnimationSystem::HValue value)
//	{
//		const std::vector<int> keys = { 0 , 30 , 64 };
//		const std::vector<float> values = { 80,200,80 };
//		const std::vector<ImVec4> rgbvalues = { ImVec4(1,0,0,1),ImVec4(0,1,0,1),ImVec4(0,0,1,1) };
//
//		HAnimationSystem::PlayerCallBack::HInterpolationInfo info = HAnimationSystem::PlayerCallBack::GetInterpolationInfoFromKeys(keys, Frame);
//		HAnimationSystem::HValue_Get<float>(value) = HAnimationSystem::PlayerCallBack::LinearInterpolation(values[info.PreviousKey], values[info.LastOneKey], info.alpha);
//	}
//
//	...
//	while (...)
//	{
//		...
//		if (ImGui::Button("play"))
//			HAnimationSystem::Play(Sequencer, 64, &value, &curr, speed, fps, loop);
//		ImGui::SliderFloat("Test", &value, 80, 200)
//		...
//		HAnimationSystem::updata(ImGui::GetIO().DeltaTime);
//	}
