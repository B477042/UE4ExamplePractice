// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.


#pragma once

#include "EngineMinimal.h"

//Not use Blueprint Type Enum class but for educate
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	//Before Creating Character. Character Asset had been set .But, Hide Character and UI. This State is not being damaged from anyother
	PREINIT,
	//Load Asset being selected Character. In this State, Decide Is AIController or Is PlayerController. 
	//If Is PlayerController, make cant control character before other Asset Loaded
	LOADING,
	//The Phase that   Character Asset loaded complete. Show Hiden Character and UI. And now can Damaged from others
	//Player can control character and AI controlled by BehaviorTrees
	READY,
	//Character's Hp is 0. Play Dead Animation and Turn off UI. Disable Collision and Set can't be damaged.
	//if PlayerControlled, Set Disable Input. And a few Time passed, Restart Levels and  Get off AI from Levels
	//If AI, Turn off BehaviorTrees
	DEAD
};

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define ABCHECK(Expr, ...){if(!(Expr)){ABLOG(Error,TEXT("ASSERTION : %s"),TEXT("'"#Expr"'")); return __VA_ARGS__;}}