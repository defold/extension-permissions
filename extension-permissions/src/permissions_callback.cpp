#if defined(DM_PLATFORM_ANDROID)

#include "permissions_callback_private.h"
#include "utils/LuaUtils.h"
#include <stdlib.h>

namespace dmPermissions {

static dmScript::LuaCallbackInfo* m_luaCallback = 0x0;
static dmArray<CallbackData> m_callbacksQueue;
static dmMutex::HMutex m_mutex;
static bool m_acceptCallbacks = false;

static void FreeCallbackData(dmArray<CallbackData>& callbacks)
{
    for (uint32_t i = 0; i != callbacks.Size(); ++i)
    {
        if (callbacks[i].json)
        {
            free(callbacks[i].json);
            callbacks[i].json = 0;
        }
    }
}

static void DestroyCallback()
{
    if (m_luaCallback != 0x0)
    {
        dmScript::DestroyCallback(m_luaCallback);
        m_luaCallback = 0x0;
    }
}

static void InvokeCallback(const char*json)
{
    if (!dmScript::IsCallbackValid(m_luaCallback))
    {
        dmLogError("Permissions callback is invalid.");
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(m_luaCallback);
    int top = lua_gettop(L);

    if (!dmScript::SetupCallback(m_luaCallback))
    {
        return;
    }

    dmScript::JsonToLua(L, json, strlen(json)); // throws lua error if it fails

    int number_of_arguments = 2;
    int ret = dmScript::PCall(L, number_of_arguments, 0);
    (void)ret;
    dmScript::TeardownCallback(m_luaCallback);

    assert(top == lua_gettop(L));
}

void InitializeCallback()
{
    if (!m_mutex)
    {
        // Keep the mutex alive until process teardown: Android permission results
        // may arrive after the extension's finalization callback.
        m_mutex = dmMutex::New();
    }
    DM_MUTEX_SCOPED_LOCK(m_mutex);
    m_acceptCallbacks = true;
}

void FinalizeCallback()
{
    dmArray<CallbackData> pending;
    {
        DM_MUTEX_SCOPED_LOCK(m_mutex);
        m_acceptCallbacks = false;
        pending.Swap(m_callbacksQueue);
    }
    FreeCallbackData(pending);
    DestroyCallback();
}

void SetLuaCallback(lua_State* L, int pos)
{
    int type = lua_type(L, pos);
    if (type == LUA_TNONE || type == LUA_TNIL)
    {
        DestroyCallback();
    }
    else
    {
        m_luaCallback = dmScript::CreateCallback(L, pos);
    }
}

void AddToQueueCallback(const char*json)
{
    CallbackData data;
    data.json = json ? strdup(json) : NULL;

    DM_MUTEX_SCOPED_LOCK(m_mutex);
    if (!m_acceptCallbacks)
    {
        if (data.json)
        {
            free(data.json);
        }
        return;
    }
    if(m_callbacksQueue.Full())
    {
        m_callbacksQueue.OffsetCapacity(2);
    }
    m_callbacksQueue.Push(data);
}

void UpdateCallback()
{
    dmArray<CallbackData> tmp;
    {
        DM_MUTEX_SCOPED_LOCK(m_mutex);
        if (m_callbacksQueue.Empty())
        {
            return;
        }
        tmp.Swap(m_callbacksQueue);
    }
    
    for(uint32_t i = 0; i != tmp.Size(); ++i)
    {
        CallbackData* data = &tmp[i];
        InvokeCallback(data->json);
    }
    FreeCallbackData(tmp);
}

} //namespace

#endif
