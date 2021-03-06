#if defined _isamp_mask_included
	#endinput
#endif
#define _isamp_mask_included

/*
+----------------------------------------------------------------------------+
|                                                                            |
|            				  Sistema de M�scara       					     |
|							  	  por crEpid							     |
|                                                                            |
|              					  Mask System    					         |
|							  	   by crEpid							     |
|                                                                            |
+----------------------------------------------------------------------------+
*/

#define LOG_MASK                            5

new Text3D:maskLabel[MAX_PLAYERS],
	maskNumber[MAX_PLAYERS],
	bool:maskActive[MAX_PLAYERS];

//===================================PUBLIC'S===================================

forward OnLogMasksLoad(playerid, targetname[]);
public OnLogMasksLoad(playerid, targetname[])
{
	new result[128],
		result2[128],
	    rows,
	    fields,
	    cont,
	    aux = 0,
	    str[128] = "";

	cache_get_data(rows, fields);

	if(rows)
	{
		SendFMessage(playerid, COLOR_LIGHTYELLOW2, "=========================[Registros de m�scaras de %s]=========================", targetname);
		while(aux < rows)
		{
		    cache_get_field_content(aux, "randommaskid", result);
   			cache_get_field_content(aux, "date", result2);
			format(str, sizeof(str), "%s[%s] %s | ", str, result2, result);
			cont ++;
			if(cont == 3)
			{
			   cont = 0;
			   SendClientMessage(playerid, COLOR_WHITE, str);
			   format(str, sizeof(str), "");
			}
			aux ++;
		}
		if(cont != 3)
			SendClientMessage(playerid, COLOR_WHITE, str);
	}
	else
		SendClientMessage(playerid, COLOR_YELLOW2, "El usuario no posee ning�n registro de m�scara en la base de datos.");
	return 1;
}

forward OnLogMaskLoad(playerid);
public OnLogMaskLoad(playerid)
{
	new result[128],
		result2[128],
		result3[128],
	    rows,
	    fields,
	    cont,
	    aux = 0,
	    str[128] = "";

	cache_get_data(rows, fields);

    SendClientMessage(playerid, COLOR_LIGHTYELLOW2, "=========================[Registros de jugadores con m�scara]=========================");
	if(rows)
	{
		while(aux < rows)
		{
		    cache_get_field_content(aux, "randommaskid", result);
   			cache_get_field_content(aux, "pName", result2);
   			cache_get_field_content(aux, "date", result3);
			format(str, sizeof(str), "%s[%s] [%s] %s | ", str, result3, result2, result);
			cont ++;
			if(cont == 2)
			{
			   cont = 0;
			   SendClientMessage(playerid, COLOR_WHITE, str);
			   format(str, sizeof(str), "");
			}
			aux ++;
		}
		if(cont != 2)
			SendClientMessage(playerid, COLOR_WHITE, str);
	}
	else
		SendClientMessage(playerid, COLOR_YELLOW2, "El id de m�scara no posee ning�n usuario en la base de datos.");
	return 1;
}

forward ReplenishMask(playerid);
public ReplenishMask(playerid)
{
    if(!maskActive[playerid])
    {
        new str[12];
        valstr(str, maskNumber[playerid]);
        maskLabel[playerid] = CreateDynamic3DTextLabel(str, COLOR_RED, 0.0, 0.0, 0.3, 15.0, playerid, INVALID_VEHICLE_ID, 1);
        maskActive[playerid] = true;
	}
	return 1;
}

//===================================STOCK'S====================================

stock ResetMaskVariables(playerid)
{
    maskNumber[playerid] = 0;
    maskActive[playerid] = false;
}

stock bool:HasPlayerMask(playerid)
{
	return maskActive[playerid];
}

stock ResetMaskLabel(playerid)
{
	if(maskActive[playerid])
	{
		DestroyDynamic3DTextLabel(maskLabel[playerid]);
		maskActive[playerid] = false;
	}
}

stock HidePlayerMask(playerid, time)
{
    DestroyDynamic3DTextLabel(maskLabel[playerid]);
    maskActive[playerid] = false;
    SetTimerEx("ReplenishMask", time, false, "i", playerid);
}

stock logmask(playerid, logType, maskid)
{
	new year, month, day,
	    hour, minute, second,
	    name[32],
		query[512];

	getdate(year, month, day);
	gettime(hour, minute, second);
	GetPlayerName(playerid, name, 24);
	mysql_real_escape_string(name, name, 1, sizeof(name));

	if(logType == LOG_MASK) {
		format(query, sizeof(query), "INSERT INTO `log_mask` (pID, pName, pIP, date, randommaskid) VALUES (%d, '%s', '%s', '%02d-%02d-%02d %02d:%02d:%02d', %d)",
			PlayerInfo[playerid][pID],
			name,
			PlayerInfo[playerid][pIP],
			year,
			month,
			day,
			hour,
			minute,
			second,
   			maskid
		);
		mysql_function_query(dbHandle, query, false, "", "");
	}
	return 1;
}

PlayerTakeMaskID(playerid, maskid)
{
	new str[12];
 	valstr(str, maskid);
	maskLabel[playerid] = CreateDynamic3DTextLabel(str, COLOR_WHITE, 0.0, 0.0, 0.5, 15.0, playerid, INVALID_VEHICLE_ID, 1);
	maskNumber[playerid] = maskid;
	maskActive[playerid] = true;
	logmask(playerid, LOG_MASK, maskNumber[playerid]);
}

//===================================COMANDOS===================================

CMD:masc(playerid, params[])
{
	cmd_mascara(playerid, params);
	return 1;
}

CMD:mascara(playerid, params[])
{
	if(PlayerInfo[playerid][pMask] == 0)
	    return SendClientMessage(playerid, COLOR_YELLOW2, "No tienes una m�scara en tu bolsillo.");
	if(!usingMask[playerid])
	{
 		if(IsPlayerAttachedObjectSlotUsed(playerid, INDEX_ID_MASK))
			return SendClientMessage(playerid, COLOR_YELLOW2, "Ya tienes una m�scara puesta.");
		SetPlayerAttachedObject(playerid, INDEX_ID_MASK, PlayerInfo[playerid][pMask], BONE_ID_HEAD, 0.058999, 0.026000, 0.004999, 87.400039, 159.800033, 84.100013, 1.0, 1.0, 1.0);
		usingMask[playerid] = true;
  		EditAttachedObject(playerid, INDEX_ID_MASK);
  		PlayerTakeMaskID(playerid, random(99999999));
		foreach(new i : Player)
		{
			if(PlayerInfo[i][pAdmin] < 2) // Si el tipo es admin no se lo ocultamos
				ShowPlayerNameTagForPlayer(i, playerid, false);
		}
	} else
		{
		    if(GetPVarInt(playerid, "disabled") == DISABLE_STEALING)
	            return SendClientMessage(playerid, COLOR_YELLOW2, "No puedes hacerlo en este momento.");
			RemovePlayerAttachedObject(playerid, INDEX_ID_MASK);
			PlayerActionMessage(playerid, 15.0, "se quita el pa�uelo que ocultaba su rostro y lo guarda en su bolsillo.");
			usingMask[playerid] = false;
			DestroyDynamic3DTextLabel(maskLabel[playerid]);
			ResetMaskVariables(playerid);
		    foreach(new i : Player)
	  		{
	            if(NicksEnabled[i])
		        	ShowPlayerNameTagForPlayer(i, playerid, true);
			}
		}
	return 1;
}

CMD:comprarmascara(playerid, params[])
{
    new idmask;
    new maskModels[13] = {18912, 18913, 18914, 18915, 18916, 18917, 18918, 18919, 18920, 18911, 19801, 19036, 19163};
    new business = GetPlayerBusiness(playerid);
    if(sscanf(params, "i", idmask))
		return SendClientMessage(playerid, COLOR_YELLOW2, "{5CCAF1}[Sintaxis]:{C8C8C8} /comprarmascara [modelo]. (modelos del 0 al 12) Precio: $50.");
    if(business == 0 || (Business[business][bType] != BIZ_CLOT &&  Business[business][bType] != BIZ_CLOT2))
        return SendClientMessage(playerid, COLOR_YELLOW2, "Para comprar un pa�uelo debes dirigirte a alg�n negocio de ropa que los venda.");
	if(idmask < 0 || idmask > 8)
        return SendClientMessage(playerid, COLOR_YELLOW2, "{5CCAF1}[Sintaxis]:{C8C8C8} /comprarmascara [modelo]. (modelos del 0 al 12) Precio: $50.");
	if(GetPlayerCash(playerid) < 50)
	    return SendClientMessage(playerid, COLOR_YELLOW2, "No tienes el dinero suficiente, necesitas $50. �Vuelve cuando los tengas!");
	if(Business[business][bProducts] < 1)
	    return SendClientMessage(playerid, COLOR_YELLOW2, "La tienda no tiene productos!");
 	GivePlayerCash(playerid, -50);
 	PlayerInfo[playerid][pMask] = maskModels[idmask];
 	PlayerActionMessage(playerid, 15.0, "le paga al empleado $50 y compra un pa�uelo, que luego guarda en su bolsillo.");
 	SendClientMessage(playerid, COLOR_WHITE, "Has comprado un pa�uelo en la tienda. Para cubrirte/descubrirte el rostro con este usa /mascara.");
	Business[business][bTill] += 50;
	Business[business][bProducts]--;
	saveBusiness(business);
	return 1;
}

CMD:vermascaras(playerid, params[])
{
	new targetname[MAX_PLAYER_NAME],
	    query[128];

    if(sscanf(params, "s[24]", targetname))
		return SendClientMessage(playerid, COLOR_GRAD2, "{5CCAF1}[Sintaxis]:{C8C8C8} /vermascaras [Nombre del jugador] (Con el '_')");

	mysql_real_escape_string(targetname, targetname);
	format(query, sizeof(query), "SELECT * FROM `log_mask` WHERE `pName` = '%s' ORDER BY date DESC LIMIT 30", targetname);
	mysql_function_query(dbHandle, query, true, "OnLogMasksLoad", "is", playerid, targetname);
	return 1;
}

CMD:vermascara(playerid, params[])
{
	new maskid,
	    query[128];

    if(sscanf(params, "i", maskid))
		return SendClientMessage(playerid, COLOR_GRAD2, "{5CCAF1}[Sintaxis]:{C8C8C8} /vermascara [ID de la m�scara]");

	format(query, sizeof(query), "SELECT * FROM `log_mask` WHERE `randommaskid` = %d", maskid);
	mysql_function_query(dbHandle, query, true, "OnLogMaskLoad", "i", playerid);
	return 1;
}

