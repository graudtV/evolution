#pragma once

#include "IGameModelNotifications.h"
#include "GameObjectLocation.h"
#include "Support/TiledMap.h"
#include <unordered_map>
#include <boost/range/adaptors.hpp>

namespace evo {
namespace snake {

class ICollisionResolver;
class IGameFinalizer;

class TiledGameModel : public IGameModelNotifications {
public:
	class Error;
	class BadLocationError;

	using GameMap = TiledMap<IGameObject *>;

	TiledGameModel(size_t height, size_t width, ICollisionResolver *resolver, IGameFinalizer *finalizer);

	GameMap& game_map() { return m_map; }
	const GameMap& game_map() const { return m_map; }

	GameObjectLocation& location_of(IGameObject& object);
	const GameObjectLocation& location_of(const IGameObject& object) const;

	/* read-only list of all game objects */
	auto objects() const { return m_locations | boost::adaptors::map_keys; }

	void add_object(IGameObject *object, const CoordArray& initial_location);
	void kill_object(IGameObject& object);

	void start_game();

	bool is_running() const { return m_is_model_running; }

private:
	ICollisionResolver& m_collision_resolver;
	IGameFinalizer& m_finalizer;
	GameMap m_map;
	bool m_is_model_running = false;
	std::unordered_map<IGameObject *, GameObjectLocation> m_locations;

	void move_game_objects();
	void update_objects_visible_segments();
};

/* Base class for all TiledGameModel errors */
class TiledGameModel::Error : public std::runtime_error {
protected:
	Error(const char *what_arg) : std::runtime_error(what_arg) {}
};

class TiledGameModel::BadLocationError : public TiledGameModel::Error {
public:
	BadLocationError(const char *what_arg) : Error(what_arg) {}
};

} // snake namespace end
} // evo namespace end