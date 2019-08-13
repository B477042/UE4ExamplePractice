// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterSetting.generated.h"

/**
 *	This Class is Manage Character Assetes. By Using INI Files. 
	
 */
//To Use INI File, add 'config' and file name
//in this case, Use  ArenaBattle.ini 
UCLASS(config=ArenaBattle)
class ARENABATTLESETTING_API UABCharacterSetting : public UObject
{
	GENERATED_BODY()
public:
	UABCharacterSetting();
	UPROPERTY(config)
		FSoftObjectPath CharacterAssets;
	/*UPROPERTY(config)
*/
};
