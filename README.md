# samp-dropgun

Single player style drop gun.

## Installation

Include in your code and begin using the library:

```pawn
#include < dropgun >
```

## Usage

```pawn
SetPlayerDropWeaponData(playerid, slot, weaponid, ammo);
```

- Set player weapon data to drop
- Returning 1 on success
	
```pawn
ClearPlayerDropWeaponSlot(playerid, slot); 
```

- Clear player one slot data 
- Returning  1 on success
	
```pawn
ClearPlayerDropWeaponData(playerid); 
```

- Clear player all slots data 
- Returning  1 on success
	
```pawn
DropPlayerWeapons(playerid); 
``` 

- Dropping weapons from assigned drop data
- Returning  1 on success

```pawn
public OnPlayerPickUpDroppedWeapon(playerid, weaponid, ammo, pickupid)
```

- Called when dropped weapon is picked up

## Configuration

```pawn
#define WEAPON_DROP_RADIUS
```

- Setting weapon drop radius (float)

```pawn
#define MAX_DROP_SLOTS
```

- Setting player max drop slots (int)

These settings are **optional**.