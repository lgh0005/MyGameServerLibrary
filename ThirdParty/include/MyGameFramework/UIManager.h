#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Camera)
	MGSL_CLASS_PTR(GLBuffer)

	class UIManager
	{
		MGSL_DECLARE_SINGLE(UIManager)

	private:
		UIManager();
		~UIManager();

	public:
		bool Init();
		void Update();
		void Clear();

	// TODO : 이게 필요한 것인가?
	public:
		void SetReferenceResolution(const Shared::vec2& resolution);
		void Resize(const Shared::vec2& windowSize);
		const Shared::vec2& GetReferenceResolution() const noexcept;
		const Shared::vec2& GetWindowSize() const noexcept;

	/*===============================//
	//   ui camera setting methods   //
	//===============================*/
	public:
		void SetUICamera(Camera* camera);
		Camera* GetUICamera() const;
		void ClearUICamera();
		bool HasUICamera() const;

	/*===============//
	//   ui camera   //
	//===============*/
	private:
		bool CreateCameraBuffer();
		void UpdateCameraBuffer();
		void BindCameraBuffer() const;

	private:
		Camera* m_uiCamera = nullptr;
		GLBufferUPtr m_uiCameraBuffer = nullptr;
		Shared::vec2 m_referenceResolution{ 1280.0f, 720.0f };
		Shared::vec2 m_windowSize{ 1280.0f, 720.0f };
	};
}



