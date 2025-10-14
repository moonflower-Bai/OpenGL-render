//
// Created by Jingren Bai on 25-9-8.
//

#ifndef LEARNOPENGL_INPUTSYSTEM_H
#define LEARNOPENGL_INPUTSYSTEM_H

#include <functional>
#include <unordered_map>
#include <string>

enum class InputAction{
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	ROTATE_X,
	ROTATE_Y,
	ROTATE_Z,
	ROTATE_AROUND_CENTER,
};

class InputSystem {
public:
	using ActionCallback = std::function<void(InputAction, bool)>;

	static InputSystem& getInstance();

	void RegisterCallback(const std::string& name, ActionCallback callback);
	void unregisterCallback(const std::string& name);

	void MapkeyToAction(int key, InputAction action);

	static
};


#endif //LEARNOPENGL_INPUTSYSTEM_H
