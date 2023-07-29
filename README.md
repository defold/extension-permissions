# extension-permissions
Extension to query for and request application permissions from the user or operating system.

## permissions.check(permission)
Determine whether you have been granted a particular permission.

```lua
local result = permissions.check("android.permission.ACCESS_NETWORK_STATE")
if result == permissions.PERMISSION_DENIED then
    -- You can directly ask for the permission.
elseif result == permissions.PERMISSION_GRANTED then
    -- You can use the API that requires the permission.
elseif result == permissions.PERMISSION_SHOW_RATIONALE then
    -- In an educational UI, explain to the user why your app requires this
    -- permission for a specific feature to behave as expected, and what
    -- features are disabled if it's declined. In this UI, include a
    -- "cancel" or "no thanks" button that lets the user continue
    -- using your app without granting the permission.
end

```

## permissions.request(permissions_table, callback)
Requests permissions to be granted to this application [Android](https://developer.android.com/reference/androidx/core/app/ActivityCompat#requestPermissions(android.app.Activity,java.lang.String[],int))

```lua
local permissions_table = {"android.permission.WRITE_EXTERNAL_STORAGE", "android.permission.READ_CONTACTS"}
permissions.request(permissions_table,
    function(self, result)
        for permission, result in pairs(result) do
        	if result == permissions.PERMISSION_DENIED then
			    -- You can directly ask for the permission.
			elseif result == permissions.PERMISSION_GRANTED then
			    -- You can use the API that requires the permission.
			elseif result == permissions.PERMISSION_SHOW_RATIONALE then
			    -- In an educational UI, explain to the user why your app requires this
			    -- permission for a specific feature to behave as expected, and what
			    -- features are disabled if it's declined. In this UI, include a
			    -- "cancel" or "no thanks" button that lets the user continue
			    -- using your app without granting the permission.
			end
        end
    end)

```
