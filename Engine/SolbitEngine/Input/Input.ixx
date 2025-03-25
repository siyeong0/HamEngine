module;

#include <SDL2\SDL.h>

import std.core;
import Common;
import Math;
import SolbitSTL;
import SolbitEngine.Renderer;
import SolbitEngine.Input.EInputCode;

export module SolbitEngine.Input;

export namespace solbit
{
	enum EButtonState : uint8
	{
		Released,
		Pressed	,
		JustReleased,
		JustPressed,
	};

	enum class EInputType
	{
		Vector2,
		Button,
	};

	class Input
	{
	public:
		static Input* GetInstance();
		static bool Initialize();
		static void Finalize();

		void Regist(ID id, const EInputType& inputType);
		void Map(const EInputCode& inputCode, ID id);

		void Update(FLOAT dt);

		inline const FVector2& GetVectorState(ID id) const;
		inline bool GetButtonState(ID id) const;
		inline bool GetButtonPressed(ID id) const;
		inline bool GetButtonReleased(ID id) const;
	private:
		Input() = default;
		~Input() = default;
		Input(const Input&) = delete;
		Input(const Input&&) = delete;

	private:
		static Input* msInstance;

		HashMap<EInputCode, ID> mInputMap;
		HashMap<ID, FVector2, DoNothing> mVectorInputStateMap;
		HashMap<ID, EButtonState, DoNothing> mButtonInputStateMap;
	};
}

namespace solbit
{
	Input* Input::msInstance = nullptr;

	Input* Input::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool Input::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new Input;

		return msInstance != nullptr;
	}

	void Input::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	void Input::Regist(ID id, const EInputType& inputType)
	{
		ASSERT(mVectorInputStateMap.find(id) == mVectorInputStateMap.end() && mButtonInputStateMap.find(id) == mButtonInputStateMap.end());

		switch (inputType)
		{
		case EInputType::Vector2:
			mVectorInputStateMap.insert({ id, FVector2{0.f, 0.f} });
			break;
		case EInputType::Button:
			mButtonInputStateMap.insert({ id, EButtonState::Released });
			break;
		default:
			ASSERT(false);
		}
	}

	void Input::Map(const EInputCode& inputCode, ID id)
	{
		ASSERT(mInputMap.find(inputCode) == mInputMap.end());
		mInputMap.insert({ inputCode, id });
	}

	void Input::Update(FLOAT dt)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		for (auto iter = mButtonInputStateMap.begin(); iter != mButtonInputStateMap.end(); ++iter)
		{
			switch (iter->second)
			{
			case EButtonState::JustPressed:
				iter->second = EButtonState::Pressed;
				break;
			case EButtonState::JustReleased:
				iter->second = EButtonState::Released;
				break;
			}
		}

		EButtonState* state;
		const FVector2 halfScreenSize = static_cast<FVector2>(Renderer::GetInstance()->GetRTSize()) / 2.f;
		switch (event.type)
		{
		case SDL_KEYDOWN:
			state = &mButtonInputStateMap[mInputMap[static_cast<EInputCode>(event.key.keysym.sym)]];
			switch (*state)
			{
			case Released:
			case JustReleased:
				*state = JustPressed;
				break;
			}
			break;
		case SDL_KEYUP:
			state = &mButtonInputStateMap[mInputMap[static_cast<EInputCode>(event.key.keysym.sym)]];
			switch (*state)
			{
			case Pressed:
			case JustPressed:
				*state = JustReleased;
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case 1:	// SDL MOUSE LEFT
				state = &mButtonInputStateMap[mInputMap[static_cast<EInputCode>(EInputCode::MOUSE_LEFTBUTTON)]];
				switch (*state)
				{
				case Released:
				case JustReleased:
					*state = JustPressed;
					break;
				}
				break;
			case 3:	// SDL MOUSE RIGHT
				state = &mButtonInputStateMap[mInputMap[static_cast<EInputCode>(EInputCode::MOUSE_RIGHTBUTTON)]];
				switch (*state)
				{
				case Released:
				case JustReleased:
					*state = JustPressed;
					break;
				}
				break;
			default:
				ASSERT(false);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case 1:	// SDL MOUSE LEFT
				state = &mButtonInputStateMap[mInputMap[static_cast<EInputCode>(EInputCode::MOUSE_LEFTBUTTON)]];
				switch (*state)
				{
				case Pressed:
				case JustPressed:
					*state = JustReleased;
					break;
				}
				break;
			case 3:	// SDL MOUSE RIGHT
				state = &mButtonInputStateMap[mInputMap[static_cast<EInputCode>(EInputCode::MOUSE_RIGHTBUTTON)]];
				switch (*state)
				{
				case Pressed:
				case JustPressed:
					*state = JustReleased;
					break;
				}
				break;
			default:
				ASSERT(false);
			}
		case SDL_MOUSEMOTION:
			mVectorInputStateMap[mInputMap[static_cast<EInputCode>(EInputCode::MOUSE_MOVE)]] = FVector2{
				(event.motion.x - halfScreenSize.X) / halfScreenSize.X  ,
				(event.motion.y - halfScreenSize.Y) / halfScreenSize.Y };
			break;
		}
	}

	inline const FVector2& Input::GetVectorState(ID id) const
	{
		ASSERT(mVectorInputStateMap.find(id) != mVectorInputStateMap.end());
		return mVectorInputStateMap.at(id);
	}

	inline bool Input::GetButtonState(ID id) const
	{
		ASSERT(mButtonInputStateMap.find(id) != mButtonInputStateMap.end());
		return mButtonInputStateMap.at(id) == EButtonState::JustPressed || mButtonInputStateMap.at(id) == EButtonState::Pressed;
	}

	inline bool Input::GetButtonPressed(ID id) const
	{
		ASSERT(mButtonInputStateMap.find(id) != mButtonInputStateMap.end());
		return mButtonInputStateMap.at(id) == EButtonState::JustPressed;
	}

	inline bool Input::GetButtonReleased(ID id) const
	{
		ASSERT(mButtonInputStateMap.find(id) != mButtonInputStateMap.end());
		return mButtonInputStateMap.at(id) == EButtonState::JustReleased;
	}
}