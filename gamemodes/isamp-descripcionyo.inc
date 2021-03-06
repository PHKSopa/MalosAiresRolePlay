#if defined _isamp_descripcion_yo
#endinput
#endif 
#define _isamp_descripcion_yo

new Text3D:descLabel[MAX_PLAYERS],
	descText[MAX_PLAYERS][70],
	bool:descActive[MAX_PLAYERS],
	ReplenishDescTimer[MAX_PLAYERS];
	
stock ResetDescVariables(playerid)
{
    descText[playerid] = "BUG";
    descActive[playerid] = false;
}

stock bool:HasPlayerDesc(playerid)
{
	return descActive[playerid];
}

stock ResetDescLabel(playerid)
{
	if(descActive[playerid])
		DestroyDynamic3DTextLabel(descLabel[playerid]);
}

forward ReplenishDesc(playerid);
public ReplenishDesc(playerid)
{
    if(!descActive[playerid])
    {
        descLabel[playerid] = CreateDynamic3DTextLabel(descText[playerid], COLOR_RED, 0.0, 0.0, 0.3, 15.0, playerid, INVALID_VEHICLE_ID, 1);
        descActive[playerid] = true;
	}
	return 1;
}

stock HidePlayerDesc(playerid, time)
{
    DestroyDynamic3DTextLabel(descLabel[playerid]);
    descActive[playerid] = false;
    ReplenishDescTimer[playerid] = SetTimerEx("ReplenishDesc", time, false, "i", playerid);
}

CMD:yo(playerid, params[])
{ 
	new desc[70], str[70];
	    
	if(sscanf(params, "s[70]", desc))
	     return SendClientMessage(playerid, COLOR_LIGHTYELLOW2, "{5CCAF1}[Sintaxis]:{C8C8C8} /yo [descripción] (hasta 65 caracteres)");
	if(strlen(desc) > 65)
		return SendClientMessage(playerid, COLOR_YELLOW2, "La descripción es demasiada larga, ingresa una más corta. (MAX 65 CARACTERES)");

	if(!descActive[playerid]){
		format(str, sizeof(str), "%s", desc);
		descLabel[playerid] = CreateDynamic3DTextLabel(str, COLOR_RED, 0.0, 0.0, 0.3, 15.0, playerid, INVALID_VEHICLE_ID, 1);
		SendFMessage(playerid, COLOR_WHITE, "Tu descripción es: %s. Usa /yob para eliminarla.", str);
		strmid(descText[playerid], str, 0, strlen(str));
		descActive[playerid] = true;
		return 1;
	} else {
	    DestroyDynamic3DTextLabel(descLabel[playerid]);
		SendClientMessage(playerid, COLOR_WHITE, "Has actualizado tu descripción actual. Usa /yob para eliminarla.");
		ResetDescVariables(playerid);
		format(str, sizeof(str), "%s", desc);
		descLabel[playerid] = CreateDynamic3DTextLabel(str, COLOR_RED, 0.0, 0.0, 0.3, 15.0, playerid, INVALID_VEHICLE_ID, 1);
		strmid(descText[playerid], str, 0, strlen(str));
		descActive[playerid] = true;
		return 1;
	}
} 

CMD:yob(playerid, params[])
{
	if(!descActive[playerid])
	    return SendClientMessage(playerid, COLOR_YELLOW2, "No tienes ninguna descripción activa.");
	    
	DestroyDynamic3DTextLabel(descLabel[playerid]);
	SendClientMessage(playerid, COLOR_WHITE, "Has borrado tu descripción actual. Para escribir una nueva usa /yo.");
	ResetDescVariables(playerid);
	return 1;
}

CMD:yodebug(playerid, params[])
{
	new targetid, param;

	if(sscanf(params, "ui", targetid, param))
		return SendClientMessage(playerid, COLOR_LIGHTYELLOW2, "{5CCAF1}[Sintaxis]:{C8C8C8} /yodebug [ID/Jugador] [descActive (0 = true - 1 = false)]");
	if(param != 0 && param != 1)
	    return 1;
		
	if(param == 0)
		descActive[targetid] = true;
	else {
		DestroyDynamic3DTextLabel(descLabel[targetid]);
		ResetDescVariables(playerid);
	}
	return 1;
}

