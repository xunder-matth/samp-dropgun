#if !defined _INC_DROP_GUN
	#define _INC_DROP_GUN
#else
	#endinput
#endif

#if !defined _samp_included
	#error Include \"a_samp\" first.
#endif

#if !defined _streamer_included
	#error Please use \"streamer\".
#endif

forward OnPlayerPickUpDroppedWeapon(playerid, weaponid, ammo, pickupid);

// Don't touch me 
#if defined PI
	#define PI_TEMP PI
	#undef PI
#endif
#define PI 										(3.14159265354)

#define DG_DEBUG 								(true)

#if DG_DEBUG
	#define DG_DebugMessageEx(%0,%1)			printf(%1)
#endif 

#define WEAPON_DROP_RADIUS						(2.0) 		// Drop radius (default: 2.0)
#define MAX_DROP_SLOTS							(5)			// Drop slots (default 5)

enum E_PLAYER_WEAPON_DATA {
	player_weapon_id[13],
	player_weapon_ammo[13]
};

enum e_DROP_PICKUP_DATA {
	drop_pickup_player,
	drop_pickup_weapon_id,
	drop_pickup_ammo
};

static const g_WeaponPickupModels[] = {
	-1, 		// 0
	331,		// 1
	333,		// 2
	334,		// 3
	335,		// 4
	336,		// 5
	337,		// 6
	338,		// 7
	339,		// 8
	341,		// 9
	321,		// 10
	322,		// 11
	323,		// 12
	324,		// 13
	325,		// 14
	326,		// 15
	342,		// 16
	343,		// 17
	344,		// 18
	-1,			// 19
	-1,			// 20
	-1,			// 21
	346,		// 22
	347,		// 23
	348,		// 24
	349,		// 25
	350,		// 26
	351,		// 27
	352,		// 28
	353,		// 29
	355,		// 30
	356,		// 31
	372,		// 32
	357,		// 33
	358,		// 34
	359,		// 35
	360,		// 36
	361,		// 37
	362,		// 38
	363,		// 39
	364,		// 40
	365,		// 41
	366,		// 42
	367,		// 43
	368,		// 44
	369,		// 45
	371			// 46
};

static PlayerWeaponData[MAX_PLAYERS][E_PLAYER_WEAPON_DATA];

stock DG_GetWeaponPickupModel(weaponid) {
	if (0 > weaponid > 46) return -1;

	return g_WeaponPickupModels[weaponid];
}

#if defined _INC_y_hooks
	hook OnPlayerConnect(playerid) {
#else 
	public OnPlayerConnect(playerid) {
#endif
	ClearPlayerDropWeaponData(playerid);

	#if !defined _INC_y_hooks
		#if defined DG_OnPlayerConnect
			return DG_OnPlayerConnect(playerid);
		#else
			return 1;
		#endif
	#endif
}

#if defined _INC_y_hooks
	hook OnPlayerPickUpDynamicPickup(playerid, STREAMER_TAG_PICKUP: pickupid) {
#else 
	public OnPlayerPickUpDynamicPickup(playerid, STREAMER_TAG_PICKUP: pickupid) {
#endif
	new pickupData[e_DROP_PICKUP_DATA];

	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, pickupData);

	if (pickupData[drop_pickup_weapon_id] != 0) {
		CallLocalFunction("OnPlayerPickUpDroppedWeapon", "dddd", 
			playerid, pickupData[drop_pickup_weapon_id], pickupData[drop_pickup_ammo], pickupid);
	}
	
	#if defined DG_OnPlayerPickUpDynamicPickup
		return DG_OnPlayerPickUpDynamicPickup(playerid, STREAMER_TAG_PICKUP: pickupid);
	#else
		return 1;
	#endif
}

stock SetPlayerDropWeaponData(playerid, slot, weaponid, ammo) {
	if (0 > playerid > MAX_PLAYERS - 1 || 0 > slot > MAX_DROP_SLOTS - 1 || DG_GetWeaponPickupModel(weaponid) == -1) return 0;

	PlayerWeaponData[playerid][player_weapon_id][slot] = weaponid;
	PlayerWeaponData[playerid][player_weapon_ammo][slot] = ammo;
	return 1;
}

stock ClearPlayerDropWeaponSlot(playerid, slot) {
	if (0 > playerid > MAX_PLAYERS - 1 || 0 > slot > MAX_DROP_SLOTS - 1) return 0;

	PlayerWeaponData[playerid][player_weapon_id][slot] = 
	PlayerWeaponData[playerid][player_weapon_ammo][slot] = 0;
	return 1;
}

stock ClearPlayerDropWeaponData(playerid) {
	if (0 > playerid > MAX_PLAYERS - 1) return 0;

	for(new i = 0; i < MAX_DROP_SLOTS; ++ i) 
		PlayerWeaponData[playerid][player_weapon_id][i] = 
		PlayerWeaponData[playerid][player_weapon_ammo][i] = 0;
	return 1;
}

stock DropPlayerWeapons(playerid) {
	if (0 > playerid > MAX_PLAYERS - 1) return 0;

	new Float: x, 
		Float: y,
		Float: z,
		Float: tempX,
		Float: tempY,
		Float: radianAngle,
		dots,
		weapon[MAX_DROP_SLOTS], 
		ammo[MAX_DROP_SLOTS];

	GetPlayerPos(playerid, x, y, z);

	#if DG_DEBUG
		DG_DebugMessageEx(playerid, "%d position is %.02f, %.02f, %.02f", playerid, x, y, z);
	#endif 

	for(new i = 0; i < MAX_DROP_SLOTS; ++ i) {
		weapon[i] = PlayerWeaponData[playerid][player_weapon_id][i];
		ammo[i] = PlayerWeaponData[playerid][player_weapon_ammo][i];

		#if DG_DEBUG
			DG_DebugMessageEx(playerid, "%d) Weapon %d, Ammo %d", i, weapon[i], ammo[i]);
		#endif 

		if (ammo[i] > 0) dots ++;
	}

	new Float: interval = (PI * 2) / dots,
		pickupid,
		pickupData[e_DROP_PICKUP_DATA];

	for(new i = 0, j = dots; i < MAX_DROP_SLOTS; ++ i) {
		if (ammo[i] > 0) {
			radianAngle = interval * (j --);

			tempX = x + WEAPON_DROP_RADIUS * floatcos(radianAngle);
			tempY = y + WEAPON_DROP_RADIUS * floatsin(radianAngle);

			pickupData[drop_pickup_player] = playerid;
			pickupData[drop_pickup_weapon_id] = weapon[i];
			pickupData[drop_pickup_ammo] = ammo[i];

			pickupid = CreateDynamicPickup(DG_GetWeaponPickupModel(weapon[i]), 1, tempX, tempY, z, -1, -1);
			Streamer_SetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, pickupData);
			
			#if DG_DEBUG
				DG_DebugMessageEx(playerid, "Created model %d on %.02f, %.02f, %.02f", DG_GetWeaponPickupModel(weapon[i]), tempX, tempY, z);
			#endif 
		}
	}
	return 1;
}

// Don't touch me
#if defined PI_TEMP
	#undef PI
	#define PI PI_TEMP
	#undef PI_TEMP
#endif

#if !defined _INC_y_hooks
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect DG_OnPlayerConnect
	#if defined DG_OnPlayerConnect
		forward DG_OnPlayerConnect(playerid);
	#endif


	#if defined _ALS_OnPlayerPickUpDynPickup
		#undef OnPlayerPickUpDynamicPickup
	#else
		#define _ALS_OnPlayerPickUpDynPickup
	#endif
	#define OnPlayerPickUpDynamicPickup DG_OnPlayerPickUpDynamicPickup
	#if defined DG_OnPlayerPickUpDynamicPickup
		forward DG_OnPlayerPickUpDynamicPickup(playerid, STREAMER_TAG_PICKUP: pickupid);
	#endif
#endif