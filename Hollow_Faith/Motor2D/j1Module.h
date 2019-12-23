// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

struct Collider;
class j1App;
class j1GUIelement;

enum class GUI_Event
{
	EVENT_ERROR,

	EVENT_ONCLICK,
	EVENT_DRAG,

	EVENT_MAX
};



class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init(bool init)
	{
		active = init;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	// Module activation ---
	bool IsEnabled() const { return active; }

	void Enable()
	{
		if (active == false)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active == true)
		{
			active = false;
			CleanUp();
		}
	}


	virtual void GuiObserver(GUI_Event type, j1GUIelement* element) {}

	virtual void OnCollision(Collider*, Collider*) {}

public:

	p2SString	name;
	bool		active;

};

#endif // __j1MODULE_H__