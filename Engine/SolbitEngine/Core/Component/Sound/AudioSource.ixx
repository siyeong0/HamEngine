module;

import Common;
import SolbitEngine.Audio;
import SolbitEngine.Resource;

export module SolbitEngine.Component.AudioSource;

export namespace solbit
{
	struct AudioSource
	{
		ID ClipId;
		uint32 Channel;
		int Loop;

		AudioSource()
			: ClipId(0)
			, Channel(-1)
			, Loop(0)
		{

		}

		void Play()
		{
			AudioData& data = AudioDataManager::GetInstance()->Get(ClipId);
			Audio::GetInstance()->PlayChannel(data, Channel, Loop);
		}

		void Pause()
		{
			Audio::GetInstance()->PauseChannel(Channel);
		}

		void Halt()
		{
			Audio::GetInstance()->HaltChannel(Channel);
		}
	};
}