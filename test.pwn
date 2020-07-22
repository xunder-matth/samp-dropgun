#include < a_samp >
#include < streamer >
#include < Pawn.CMD >
#include < sscanf2 >
#include < dropgun >

main() { 
	print("[INC] Drop gun by Xunder > Test");
}

public OnGameModeInit() {
	SetGameModeText("Testing testing");
	return 1;
}

public OnPlayerSpawn(playerid) {
	SetPlayerPos(playerid, 0, 0, 3);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason) {
	DropPlayerWeapons(playerid);
	ClearPlayerDropWeaponData(playerid);
	return 1;
}

public OnPlayerPickUpDroppedWeapon(playerid, weaponid, ammo, pickupid) {
	GivePlayerWeapon(playerid, weaponid, ammo);
	DestroyDynamicPickup(pickupid);
	return 1;
}

CMD:assign(playerid, params[]) {
	new weaponid,
		ammo,
		slot;

	for(new i = 0; i < 13; ++ i) {
		GetPlayerWeaponData(playerid, i, weaponid, ammo);

		switch(i) {
			case 4, 5, 7: SetPlayerDropWeaponData(playerid, (slot ++), weaponid, ammo);
		}
	}
	SendClientMessage(playerid, -1, "Data assigned.");
	return 1;
}

CMD:gimme(playerid, params[]) {
	GivePlayerWeapon(playerid, 38, 1000);
	GivePlayerWeapon(playerid, 24, 1000);
	GivePlayerWeapon(playerid, 31, 1000);
	GivePlayerWeapon(playerid, 32, 1000);
	SendClientMessage(playerid, -1, "UwU");
	return 1;
}

CMD:killme(playerid, params[]) return SetPlayerHealth(playerid, 0.0);