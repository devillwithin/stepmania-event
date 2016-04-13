#ifndef LUA_REFERENCE_H
#define LUA_REFERENCE_H

#include "RageUtil_AutoPtr.h"
#include "LuaManager.h"

struct lua_State;
typedef lua_State Lua;
/** @brief A self-cleaning Lua reference. */
class LuaReference
{
public:
	LuaReference();
	virtual ~LuaReference();

	/* Copying a reference makes a new reference pointing to the same object. */
	LuaReference( const LuaReference &cpy );
	LuaReference &operator=( const LuaReference &cpy );

	// Convenience constructor.
	LuaReference(Lua *L)
		:m_iReference(LUA_NOREF)
	{
		SetFromStack(L);
	}

	/* Create a reference pointing to the item at the top of the stack, and pop
	 * the stack. */
	void SetFromStack( Lua *L );
	void SetFromNil();

	/* Evaluate an expression that returns an object; store the object in a reference.
	 * For example, evaluating "{ 1, 2, 3 }" will result in a reference to a table.
	 * On success, return true.  On error, set to nil and return false. */
	bool SetFromExpression( const RString &sExpression );

	/** @brief Deep-copy tables, detaching this reference from any others. */
	void DeepCopy();

	/* Push the referenced object onto the stack.  If not set (or set to nil), push nil. */
	virtual void PushSelf( Lua *L ) const;

	/**
	 * @brief Determine if the reference is set.
	 *
	 * SetFromNil() counts as being set.
	 * @return true if it's set. */
	bool IsSet() const;
	/**
	 * @brief Determine if the reference is nil.
	 * @return true if it's nil. */
	bool IsNil() const;
	void Unset() { Unregister(); }

	/* Return the referenced type, or LUA_TNONE if not set. */
	int GetLuaType() const;

	RString Serialize() const;

	template<typename T>
	static LuaReference Create( const T &val )
	{
		Lua *L = LUA->Get();
		LuaReference ref;
		LuaHelpers::Push( L, val );
		ref.SetFromStack( L );
		LUA->Release( L );

		return ref;
	}

	template<class T>
	static LuaReference CreateFromPush( T &obj )
	{
		Lua *L = LUA->Get();
		LuaReference ref;
		obj.PushSelf( L );
		ref.SetFromStack( L );
		LUA->Release( L );

		return ref;
	}

private:
	void Unregister();
	int m_iReference;
};

typedef AutoPtrCopyOnWrite<LuaReference> apActorCommands;

class LuaTable: public LuaReference
{
public:
	LuaTable();

	/* Get the key with the given name, and push it on the stack. */
	void Get( Lua *L, const RString &sKey );

	/* Set a key by the given name to a value on the stack, and pop the value
	 * off the stack. */
	void Set( Lua *L, const RString &sKey );
};

#endif

/**
 * @file
 * @author Glenn Maynard, Chris Danford (c) 2005
 * @section LICENSE
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE. 
 * 
 * (c) 2016- Electromuis, Anton Grootes
 * This branch of https://github.com/stepmania/stepmania
 * will from here on out be released as GPL v3 (wich converts from the previous MIT license)
 */
