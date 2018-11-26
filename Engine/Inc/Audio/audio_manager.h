#pragma once

namespace K
{
	class K_ENGINE_DLL AudioManager final : public Singleton<AudioManager>
	{
		friend class Singleton<AudioManager>;
	public:
		virtual void Initialize() override;

		void Update();
		void Suspend();
		void Resume();

		std::shared_ptr<DirectX::SoundEffect> const& FindSoundEffect(std::string const& _tag) const;
		std::shared_ptr<DirectX::SoundEffectInstance> const& FindSoundEffectInstance(std::string const& _tag) const;

		void AddSoundEffectInstance(std::string const& _tag, std::unique_ptr<DirectX::SoundEffectInstance> _sound_effect_instance);
		void RemoveSoundEffectInstance(std::string const& _tag);

	private:
		AudioManager() = default;
		AudioManager(AudioManager const&) = delete;
		AudioManager(AudioManager&&) noexcept = delete;
		AudioManager& operator=(AudioManager const&) = delete;
		AudioManager& operator=(AudioManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _CreateSoundEffect(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag);

		static std::shared_ptr<DirectX::SoundEffect> sound_effect_nullptr_;
		static std::shared_ptr<DirectX::SoundEffectInstance> sound_effect_instance_nullptr_;
		std::unique_ptr<DirectX::AudioEngine> audio_engine_{};
		std::unordered_map<std::string, std::shared_ptr<DirectX::SoundEffect>> sound_effect_map_{};
		std::unordered_map<std::string, std::shared_ptr<DirectX::SoundEffectInstance>> sound_effect_instance_map_{};
	};
}
