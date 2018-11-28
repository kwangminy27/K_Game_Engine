#include "KEngine.h"
#include "world_manager.h"

#include "level.h"

std::shared_ptr<K::Level> K::WorldManager::level_dummy_{};

void K::WorldManager::Initialize()
{
	try
	{
		_CreateLevel<Level>("Default");
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "WorldManager::Initialize" << std::endl;
	}
}

void K::WorldManager::Input(float _time)
{
	for (auto iter = level_list_.begin(); iter != level_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Input(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = level_list_.erase(iter);
			break;
		}
	}
}

void K::WorldManager::Update(float _time)
{
	for (auto iter = level_list_.begin(); iter != level_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Update(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = level_list_.erase(iter);
			break;
		}
	}
}

void K::WorldManager::Collision(float _time)
{
	for (auto iter = level_list_.begin(); iter != level_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Collision(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = level_list_.erase(iter);
			break;
		}
	}
}

void K::WorldManager::Render(float _time)
{
	for (auto iter = level_list_.begin(); iter != level_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Render(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = level_list_.erase(iter);
			break;
		}
	}
}

std::shared_ptr<K::Level> const& K::WorldManager::FindLevel(TAG const& _tag) const
{
	auto iter = std::find_if(level_list_.begin(), level_list_.end(), [&_tag](std::shared_ptr<Level> const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == level_list_.end())
		return level_dummy_;

	return *iter;
}

std::shared_ptr<K::Layer> const& K::WorldManager::FindLayer(TAG const& _tag) const
{
	for (auto const& level : level_list_)
	{
		if (auto const& layer = level->FindLayer(_tag))
			return layer;
	}

	return Level::layer_dummy_;
}

void K::WorldManager::_Finalize()
{
}
