#pragma once

namespace SpelSylt
{
	class ITimeGetter;
	class IInputEventGetter;
	class IAsyncLoader;
	class CMessageQueue;
	class CAssetManager;

	struct SGameContext
	{
	public:
		SGameContext() = delete;
		SGameContext(
			IInputEventGetter& InInputEventGetter,
			ITimeGetter& InTimeGetter,
			IAsyncLoader& InAsyncLoader,
			CMessageQueue& InMessageQueue,
			SpelSylt::CAssetManager& InAssetManager
		)
			: Input(InInputEventGetter)
			, Time(InTimeGetter)
			, Loader(InAsyncLoader)
			, MessageQueue(InMessageQueue)
			, AssetManager(InAssetManager)
		{}

		IInputEventGetter& Input;
		ITimeGetter& Time;
		IAsyncLoader& Loader;
		CMessageQueue& MessageQueue;
		SpelSylt::CAssetManager& AssetManager;
	};
}

namespace SS = SpelSylt;