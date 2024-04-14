
[discord :)](https://discord.gg/2UEwXjNtv8)

# Display of results :

https://github.com/Half-People/HImGuiAnimation/assets/56476339/3abb5369-20f1-4d5c-88d9-b0350d34554d


Easy to use (To learn more, please visit the wiki): 
```cpp
  
  void Sequencer(int32_t Frame/*current Frame*/, HAnimationSystem::HValue value) //Define callback function (The straight part is convertible)
  {
	  const std::vector<int> keys = { 0 , 30 , 64 }; // define key
	  const std::vector<float> values = { 80,200,80 }; //define key value

	  HAnimationSystem::PlayerCallBack::HInterpolationInfo info = HAnimationSystem::PlayerCallBack::GetInterpolationInfoFromKeys(keys, Frame); //Get interpolation information
	  HAnimationSystem::HValue_Get<float>(value) = HAnimationSystem::PlayerCallBack::LinearInterpolation(values[info.PreviousKey], values[info.LastOneKey], info.alpha); //Interpolation and return value
  }

  ...
  while (...)
  {
	  ...
          static float value;
	  if (ImGui::Button("play"))
		  HAnimationSystem::Play(Sequencer, 64, &value, &curr, speed, fps, loop); //Play animation (You don’t need to fill in so much data to use it)
	  ImGui::SliderFloat("Test", &value, 80, 200) //apply testing
	  ...
	  HAnimationSystem::updata(ImGui::GetIO().DeltaTime); // updata animation manager
  }
```
