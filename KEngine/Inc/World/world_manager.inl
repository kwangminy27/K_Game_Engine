#pragma once

template <typename T>
void K::WorldManager::_CreateLevel(std::string const& _tag)
{
	auto level = std::shared_ptr<Level>{ new T, [](T* _p) {
		_p->_Finalize();
		delete _p;
	} };

	auto id = std::count_if(level_list_.begin(), level_list_.end(), [&_tag](std::shared_ptr<T> const& _level) {
		return _level->tag().first == _tag;
	});

	level->set_tag(std::make_pair(_tag, static_cast<uint32_t>(id)));

	level->Initialize();

	level_list_.push_back(std::move(level));
}
