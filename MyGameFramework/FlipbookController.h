#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Texture2D)
	MGSL_CLASS_PTR(FlipbookClip)
	MGSL_CLASS_PTR(FlipbookController)

	class FlipbookController
	{
		MGSL_DISABLE_COPY(FlipbookController)
		MGSL_DISABLE_MOVE(FlipbookController)

	public:
		~FlipbookController();
		static FlipbookControllerPtr Create(const Texture2DPtr& atlasTexture);

	public:
		void ResizeClips(Shared::usize size);
		bool SetClip(Shared::uint32 stateIndex, const FlipbookClipPtr& clip);
		bool RemoveClip(Shared::uint32 stateIndex);
		void Clear();

	public:
		const Texture2DPtr& GetAtlasTexture() const;
		FlipbookClipPtr GetClip(Shared::uint32 stateIndex) const;
		bool HasClip(Shared::uint32 stateIndex) const;
		bool IsValid() const;

	private:
		FlipbookController(const Texture2DPtr& atlasTexture);

		Texture2DPtr m_atlasTexture;
		Shared::List<FlipbookClipPtr> m_flipbookClips;
	};
}

